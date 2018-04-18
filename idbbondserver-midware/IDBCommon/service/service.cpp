#include "service.h"
#include "send_service.h"
#include "connection_manager.h"
#include "sdbus/connection.h"
#include "include/common.h"
#include "include/fid.h"
#include "include/constants.h"
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <exception> 
#include <boost/date_time.hpp>  
#include "sync/thread_sync.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bonddeal_cachecontroller.h"

using namespace boost::gregorian;  
using namespace boost::posix_time;

Service::Service(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : conn_(conn), queue_name_(queue_name), topic_name_(topic_name), is_ready_(true)
{
    
}

Service::~Service()
{
    std::vector<boost::thread*>::iterator it = thrd_vec_.begin();
    for (; it != thrd_vec_.end(); ++it)
    {
        (*it)->interrupt();
        (*it)->join();
    }
}

bool Service::init(int thread_num /* = 1 */)
{
    if (!Init(queue_name_, sdbus::DeliverDirect)) {
        return false;
    }
    
    boost::function0<void> f = boost::bind(&Service::threadReqMsg, this);  
    if (thread_num < 1) thread_num = 1;
    if (thread_num > 10) thread_num = 10;
    for (int i = 0; i < thread_num; ++i) {
        boost::thread *thrd = new boost::thread(f);
        thrd_vec_.push_back(thrd);
    }

    if (!AttachService()) {
        return false;
    }

    return true;
}

bool Service::AttachService()
{
	bool ret = false;
    if (conn_)
        ret = conn_->AddHandler(this);
	return ret;
}

void Service::OnMsgs(const sdbus::Message ** msgs, int count)
{
    if (!is_ready_) {
        // Service还没准备好，忽略消息
        LOGGER_INFO("Ignore " << count << " message.")
        return;
    }

    for(int i =0; i<count; ++i)
    {
        const sdbus::Message* msg = *(msgs + i);

        LOGGER_INFO("Receive request message. [Type]: " << msg->GetType() << " [From]: " << msg->GetReplyTo() << " begin:[" << to_simple_string(microsec_clock::local_time().time_of_day()) << "]");
        
        boost::mutex::scoped_lock lock(req_queue_mutex_);
        req_msg_queue_.push(*msg);
        req_queue_cond_var_.notify_one();
    }
}

void Service::threadReqMsg()
{    
    LOGGER_DEBUG("Start thread to process outside request msg - " << queue_name_);

	while(true) {
		sdbus::Message msg;

		{
			boost::mutex::scoped_lock lock(req_queue_mutex_);

			while(req_msg_queue_.empty()) {        
				req_queue_cond_var_.wait(lock);
			}

			msg = req_msg_queue_.front();
			req_msg_queue_.pop();
		}

		// 处理外部请求消息
		if (tokenVerification(&msg)) {
            ptime bt1(microsec_clock::local_time());
            LOGGER_INFO("processing message...... [Type]: " << msg.GetType() << " [From]: " << msg.GetReplyTo() << " begin:[" << to_simple_string(microsec_clock::local_time().time_of_day()) << "]");
            std::vector<std::string> bond_key_list_market_in_process;
            std::string company_id;
            BeforeProcessReqMsg(&msg, bond_key_list_market_in_process, company_id);
			processReqMsg(&msg);
            AfterProcessReqMsg(bond_key_list_market_in_process, company_id);
            ptime bt2(microsec_clock::local_time());
			LOGGER_INFO("End  processing message. [Type]: " << msg.GetType() << " [ To ]: " << msg.GetReplyTo() << " [Time costing]: " << 
				"[" << to_simple_string(bt2.time_of_day()) << "]-[" << to_simple_string(bt1.time_of_day()) << "]= " << (bt2.time_of_day() - bt1.time_of_day()).total_milliseconds() << " ms");
		} else {
			LOGGER_INFO("Token Verification failed.")
		}
	}	 
}

void Service::BeforeProcessReqMsg(const sdbus::Message* msg, std::vector<std::string>& bond_key_list_market_in_process, std::string &company_id)
{
    sdbus::string method = msg->GetType();
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply, zip_reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    std::string account_id;
    msg->GetString(FID_ACCOUNT_ID, account_id);
    msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
    if (company_id.empty()){
        company_id = GetCurrentCompanyId(account_id);
    }

    sdbus::VariantList list;
    if (method == "BondQuote.Add" ||
        method == "BondQuote.Update" ||
        method == "BondQuote.EditReferredQuote" ||
        method == "BondQuote.UpdateReferredQuote"
        )
    {
        msg->GetList(FID_LIST, list);

        for (unsigned int i = 0; i < list.Size(); ++i)
        {
            std::string bondKeyListedMarket;
            const sdbus::VariantMap& mp = list.Get(i)->ToMap();
            mp.GetString(FID_BOND_KEY_LISTED_MARKET, bondKeyListedMarket);
            if (!bondKeyListedMarket.empty())
                bond_key_list_market_in_process.push_back(bondKeyListedMarket);
        }
    }
    else if (method == "BondQuote.DeleteReferredQuote" ||
        method == "BondQuote.UnRefer")
    {
        msg->GetList(FID_LIST, list);
        BondQuoteReferCacheController bondquoterefer_cc;

        for (unsigned int i = 0; i < list.Size(); ++i)
        {
            std::string id = list.Get(i)->ToString().c_str();
            std::string sql_str = "id = '" + id + "'";
            dbQuery q(sql_str.c_str());
            BondQuoteReferCachePtr c = bondquoterefer_cc.getCacheByQueryInThreadSafty(q);
            if (c.get() && !c->bond_key_listed_market.empty())
                bond_key_list_market_in_process.push_back(c->bond_key_listed_market);
        }

    }
    else if (method == "BondQuote.Refer" ||
        method == "BondQuote.DeleteQuote" ||
        method == "BondQuote.GvnTknQuote")
    {
        msg->GetList(FID_LIST, list);
        BondQuoteCacheController bondquote_cc;

        for (unsigned int i = 0; i < list.Size(); ++i)
        {
            std::string id = list.Get(i)->ToString().c_str();
            std::string sql_str = "id = '" + id + "'";
            dbQuery q(sql_str.c_str());
            BondQuoteCachePtr c = bondquote_cc.getCacheByQueryInThreadSafty(q);
            if (c.get() && !c->bond_key_listed_market.empty())
                bond_key_list_market_in_process.push_back(c->bond_key_listed_market);
        }
    }
    else if (method == "BondQuote.UpdateInfo" ||
        method == "BondQuote.UpdateInternalInfo")
    {
        msg->GetList(FID_LIST, list);
        BondQuoteCacheController bondquote_cc;
        for (unsigned int i = 0; i < list.Size(); ++i)
        {
            std::string id;
            const sdbus::VariantMap& mp = list.Get(i)->ToMap();
            mp.GetString(FID_ID, id);
            std::string sql_str = "id = '" + id + "'";
            dbQuery q(sql_str.c_str());
            BondQuoteCachePtr c = bondquote_cc.getCacheByQueryInThreadSafty(q);
            if (c.get() && !c->bond_key_listed_market.empty())
                bond_key_list_market_in_process.push_back(c->bond_key_listed_market);
        }
    }
    else if (method == "BondDeal.AddBondDeal" ||
        method == "BondDeal.UpdateBondDeal")
    {
        const sdbus::VariantMap& mp = msg->MapValue();
        std::string bondKeyListedMarket;
        mp.GetString(FID_BOND_KEY_LISTED_MARKET, bondKeyListedMarket);
        if (!bondKeyListedMarket.empty())
            bond_key_list_market_in_process.push_back(bondKeyListedMarket);
    }
    else if (method == "BondDeal.UpdateDealInfo" ||
        method == "BondDeal.UpdateDealInfoUndo")
    {
        msg->GetList(FID_LIST, list);
        BondDealCacheController bonddeal_cc;
        for (unsigned int i = 0; i < list.Size(); ++i)
        {
            std::string id;
            const sdbus::VariantMap& mp = list.Get(i)->ToMap();
            mp.GetString(FID_ID, id);
            std::string sql_str = "id = '" + id + "'";
            dbQuery q(sql_str.c_str());
            BondDealCachePtr c = bonddeal_cc.getCacheByQueryInThreadSafty(q);
            if (c.get() && !c->bond_key_listed_market.empty())
                bond_key_list_market_in_process.push_back(c->bond_key_listed_market);
        }
    }
    else if (method == "BondDeal.DeleteBondDeal")
    {
        msg->GetList(FID_LIST, list);
        BondDealCacheController bonddeal_cc;
        for (unsigned int i = 0; i < list.Size(); ++i)
        {
            std::string id;
            id = list.Get(i)->ToString();
            std::string sql_str = "id = '" + id + "'";
            dbQuery q(sql_str.c_str());
            BondDealCachePtr c = bonddeal_cc.getCacheByQueryInThreadSafty(q);
            if (c.get() && !c->bond_key_listed_market.empty())
                bond_key_list_market_in_process.push_back(c->bond_key_listed_market);
        }
    }

    if (!bond_key_list_market_in_process.empty()) {
        std::string keys;
        for (std::vector<std::string>::iterator it = bond_key_list_market_in_process.begin(); it != bond_key_list_market_in_process.end(); ++it)
            keys += *it + ",";
        //LOGGER_DEBUG("bond_key_list_market_in_process:[" + keys + "]");
        thread_sync::singleton()->thread_sync_begin_best_quote_calc_and_publish(company_id, bond_key_list_market_in_process);
    }
}

void Service::AfterProcessReqMsg(const std::vector<std::string>& bond_key_list_market_in_process, std::string& company_id)
{
    if (!bond_key_list_market_in_process.empty())
        thread_sync::singleton()->thread_sync_ennnd_best_quote_calc_and_publish(company_id, bond_key_list_market_in_process);
}

bool Service::tokenVerification(const sdbus::Message* msg)
{
    return true;

    //if (msg->GetReplyTo() == kServiceQueueLogin || 
    //    msg->GetReplyTo() == kServiceQueueProductFavor || 
    //    msg->GetReplyTo() == kServiceQueueHotKey ||
    //    msg->GetReplyTo() == kServiceQueueCalculator) {
    //    return true;
    //}

    //sdbus::Message request, response;
    //sdbus::string account_id, account_token;
    //msg->GetString(FID_ACCOUNT_ID, account_id);
    //msg->GetString(FID_ACCOUNT_TOKEN, account_token);
    //
    //LOGGER_INFO("[Account]: " << account_id)
    //LOGGER_INFO("[Token]: " << account_token)

    //request.SetType("Login.VerifyToken");
    //request.SetString(FID_ACCOUNT_ID, account_id);
    //request.SetString(FID_ACCOUNT_TOKEN, account_token);

    //SendService::singleton()->call(kServiceQueueLogin, request);
    //return true;

    //bool verify_ret = true;
    //if (SendService::singleton()->call(kServiceQueueLogin, request, response, 200)) {
    //    response.GetBool(FID_LOGIN_TOKEN_VERIFY_RET, verify_ret);
    //}

    //return verify_ret;
}
