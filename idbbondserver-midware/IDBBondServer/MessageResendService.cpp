#include "MessageResendService.h"
#include "business/bonddeal_business.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "excel/generateexcel.h"
#include "sdbus/codec.h"
#include "service/publish_manager.h"
#include "service/service_manager.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/model/dcsbonddeal_cache.h"
#include "cache/model/bondquote_cache.h"
#include "cache/model/bondquoterefer_cache.h"
#include "config.h"
#include <boost/locale/encoding.hpp>	
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>
#include <sstream>
#include <fstream>

using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;

using namespace qpid::messaging;
const std::string _DEFALUT_COMPANY_ID_TP = "1";
MessageResendService* MessageResendService::singleton_ = NULL;

MessageResendService* MessageResendService::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new MessageResendService();
    }

    return singleton_;
}

MessageResendService::MessageResendService(void)
{
	_connection = NULL;
	_finaled = false;
}

MessageResendService::~MessageResendService(void)
{
	Close();
}

bool MessageResendService::Open(const std::string& url)
{
    LOGGER_DEBUG("Open Message Resend Service")

	_connection = new qpid::messaging::Connection(url) ;
	_connection->setOption("reconnect" , true) ;
	_connection->setOption("heartbeat", Config::singleton()->getIntValue("SDBus.Main.HeartBeat", 5));
	_connection->open() ;

    boost::function0<void> f1 = boost::bind(&MessageResendService::threadSendMsg, this);  
    boost::thread thrd1(f1); 

	boost::function0<void> f2 = boost::bind(&MessageResendService::threadDoRecieveMsgs, this);  
    boost::thread thrd2(f2); 


	return _connection->isOpen() ;
}

void MessageResendService::Close()
{
	if(_connection != NULL)
	{
		_connection->close() ;

		delete _connection ;
		_connection = NULL ;
	}
}

void MessageResendService::PushMsg(const std::string& text) 
{    
    if (_connection == NULL || !_connection->isOpen()) {
        return;
    }

    boost::mutex::scoped_lock lock(queue_mutex_);
    msg_queue_.push(text);
    queue_cond_var_.notify_one();
}

bool MessageResendService::RecieveRequest() {
	if (_connection == NULL || !_connection->isOpen()) {
        return false;
    }

	qpid::messaging::Session session = _connection->createSession();

	std::string addr = _service_name_for_recieve + "; {create:always , node:{type:queue}}";
	qpid::messaging::Receiver receiver = session.createReceiver(addr);

	bool success = true;
	try {
		::qpid::messaging::Duration timeout = ::qpid::messaging::Duration::SECOND;

		Message message;
        while (receiver.fetch(message, timeout)) {
			session.acknowledge();

			std::string msg_content = message.getContent();
			if (msg_content.find("{\"method\"") > 0) {
				msg_content.erase(0, msg_content.find("{\"method\""));
			}
			LOGGER_INFO("Get Message Length: -------------------------------------------------------- " << msg_content.length());
			std::stringstream stream;
			stream << msg_content;
			ptree pt;
			read_json<ptree>(stream, pt);
			std::string method = pt.get<std::string>("method");
			LOGGER_INFO("Method: -------------------------------------------------------- " << method);

			if (method == "BondDeal.AddBondDeal") {
				AUpdateBondDeal(&pt);
			}
			else if (method == "BondDeal.UpdateBondDeal") {
				AUpdateBondDeal(&pt);
			}
			else if (method == "BondDeal.DeleteBondDeal") {
				DeleteBondDeal(&pt);
			}
			else if (method == "BondQuote.Add") {
				AUpdateBondQuote(&pt);
			}
			else if (method == "BondQuote.Update") {
				AUpdateBondQuote(&pt);
			}
			else if (method == "BondQuote.Refer") {
				ReferQuote(&pt);
			}
			else if (method == "BondQuote.DeleteReferredQuote") {
				DeleteReferredQuote(&pt);
			}
			else if (method == "BondQuote.DeleteQuote") {
				DeleteQuote(&pt);
			}
			else if (method == "BondQuote.UnRefer") {
				UnReferQuote(&pt);
			}
        }
	}
	catch(const std::exception& error) {
		success = false ;
        LOGGER_ERROR("exception raise , when fetch message , errmsg[%s] " << error.what()) ;                
    }

	session.sync();
    session.close();

	return success;
}

void MessageResendService::SendRequest(const std::string& text, const std::string subject) {
    if (_connection == NULL || !_connection->isOpen()) {
        return;
    }

	qpid::messaging::Session session = _connection->createSession();

	std::string addr = _service_name + "; {create:always , node:{type:queue}}";
	qpid::messaging::Sender sender = session.createSender(addr) ;


	qpid::messaging::Message msg;
	msg.setSubject(subject);
	//msg.setContentType("application/octet-stream");

	std::string sendStr = boost::locale::conv::between(text, "UTF-8", "ISO8859-1");
	//std::ofstream fout;
	//fout.open("test1", std::fstream::binary);
	//fout << sendStr;
	//fout.close();
	//fout.open("test2", std::fstream::binary);
	//fout << text;
	//fout.close();

	//std::string sendStr = boost::locale::conv::between(text, "GBK", "UTF-8");
	//msg.setContent(sendStr);

	msg.setContent(text);
	sender.send(msg) ;     

	session.sync() ;
    session.close() ;
    LOGGER_INFO("Message Resending Over")
}

void MessageResendService::threadSendMsg()
{    
    LOGGER_DEBUG("Thread Resend Msg")

    while(true) {
        std::string text;

        {
            boost::mutex::scoped_lock lock(queue_mutex_);

            while(msg_queue_.empty()) {        
                queue_cond_var_.wait(lock);
            }

            text = msg_queue_.front();
            msg_queue_.pop();
        }
        
        SendRequest(text, "Non-Subject");
    }
}

void MessageResendService::threadDoRecieveMsgs() {
	LOGGER_DEBUG("Thread Resend Msg")

	while(true) {
		RecieveRequest();
	}
}

double MessageResendService::getDouble(std::string key, ptree* pt, double default_value) {
	try {
		return pt->get<double>(key);
	}
	catch(const std::exception& /*error*/) {
        //LOGGER_ERROR("exception raise , when fetch message , errmsg[%s] " << error.what()) ;  
		return default_value;
    }
}

bool MessageResendService::getBoolean(std::string key, ptree* pt, bool default_value) {
	try {
		return pt->get<bool>(key);
	}
	catch(const std::exception& /*error*/) {
        //LOGGER_ERROR("exception raise , when fetch message , errmsg[%s] " << error.what()) ;  
		return default_value;
    }
}

void MessageResendService::AssembleBondDeal(BondDealPtr bond_deal, ptree* each_bond_deal_data) {
	bond_deal->id_ = each_bond_deal_data->get<std::string>("id");
	bond_deal->bond_key_listed_market_ = each_bond_deal_data->get<std::string>("bond_key_listed_market");
	bond_deal->direction_ = each_bond_deal_data->get<std::string>("direction");          
	bond_deal->create_time_ = each_bond_deal_data->get<std::string>("create_time");
	bond_deal->deal_time_ = each_bond_deal_data->get<std::string>("deal_time");
	bond_deal->deal_no_ = each_bond_deal_data->get<std::string>("deal_no");
	bond_deal->price_ = getDouble("price", each_bond_deal_data, DOUBLE_NULL);//
	bond_deal->fan_dian_ = getDouble("fan_dian", each_bond_deal_data, DOUBLE_NULL);//
	bond_deal->fan_dian_flag_ = each_bond_deal_data->get<std::string>("fan_dian_flag");
	bond_deal->quote_type_ = each_bond_deal_data->get<std::string>("quote_type");
	bond_deal->volume_ = (int)getDouble("volume", each_bond_deal_data, 0);
	bond_deal->currency_ = each_bond_deal_data->get<std::string>("currency");
	bond_deal->description_ = each_bond_deal_data->get<std::string>("description");
	bond_deal->deal_internal_flag_ = each_bond_deal_data->get<std::string>("deal_internal_flag");
	bond_deal->clean_price_ = getDouble("clean_price", each_bond_deal_data, DOUBLE_NULL);//
	bond_deal->dirty_price_ = getDouble("dirty_price", each_bond_deal_data, DOUBLE_NULL);//
	bond_deal->spread_ = getDouble("spread", each_bond_deal_data, DOUBLE_NULL);//
	bond_deal->yield_ = getDouble("yield", each_bond_deal_data, DOUBLE_NULL);//
	bond_deal->yield_type_ = each_bond_deal_data->get<std::string>("yield_type");
	bond_deal->settlement_type_ = each_bond_deal_data->get<std::string>("settlement_type");
	bond_deal->settlement_mode_ = each_bond_deal_data->get<std::string>("settlement_mode");
	bond_deal->settlement_amount_ = each_bond_deal_data->get<std::string>("settlement_amount");
	bond_deal->traded_date_ = each_bond_deal_data->get<std::string>("traded_date");
	bond_deal->delivery_date_ = each_bond_deal_data->get<std::string>("delivery_date");
	bond_deal->settlement_date_ = each_bond_deal_data->get<std::string>("settlement_date");
	bond_deal->bid_is_bridge_ = each_bond_deal_data->get<std::string>("bid_is_bridge");
	bond_deal->bid_is_charged_ = each_bond_deal_data->get<std::string>("bid_is_charged");
	bond_deal->bid_checked_ = getBoolean("bid_checked", each_bond_deal_data, false);
	bond_deal->bid_institution_id_ = each_bond_deal_data->get<std::string>("bid_institution_id");
	bond_deal->bid_trader_id_ = each_bond_deal_data->get<std::string>("bid_trader_id");
	bond_deal->bid_broker_id_ = each_bond_deal_data->get<std::string>("bid_broker_id");
	bond_deal->bid_brokerage_ = getDouble("bid_brokerage", each_bond_deal_data, 0);//
	bond_deal->ofr_is_bridge_ = each_bond_deal_data->get<std::string>("ofr_is_bridge");
	bond_deal->ofr_is_charged_ = each_bond_deal_data->get<std::string>("ofr_is_charged");
	bond_deal->ofr_checked_ = getBoolean("ofr_checked", each_bond_deal_data, false);
	bond_deal->ofr_institution_id_ = each_bond_deal_data->get<std::string>("ofr_institution_id");
	bond_deal->ofr_trader_id_ = each_bond_deal_data->get<std::string>("ofr_trader_id");
	bond_deal->ofr_broker_id_ = each_bond_deal_data->get<std::string>("ofr_broker_id");
	bond_deal->ofr_brokerage_ = getDouble("ofr_brokerage", each_bond_deal_data, 0);//
	bond_deal->deal_status_ = each_bond_deal_data->get<std::string>("deal_status");
	bond_deal->quote_side_ = 1;
	bond_deal->company_id = _DEFALUT_COMPANY_ID_TP;
}

void MessageResendService::AssembleBondQuote(BondQuotePtr bond_quote, ptree* each_bond_quote_data) {
	bond_quote->id_ = each_bond_quote_data->get<std::string>("id");// 报价ID
	bond_quote->bond_key_listed_market_ = each_bond_quote_data->get<std::string>("bond_key_listed_market");
	bond_quote->institution_id_ = each_bond_quote_data->get<std::string>("institution_id"); // 银行ID
	bond_quote->trader_id_ = each_bond_quote_data->get<std::string>("trader_id"); // 交易员ID
	bond_quote->broker_id_ = each_bond_quote_data->get<std::string>("broker_id"); // 银行ID
	bond_quote->quote_side_ = each_bond_quote_data->get<int>("quote_side"); // // 报价方向，bid/ofr
	bond_quote->price_ = getDouble("price", each_bond_quote_data, DOUBLE_NULL); // 报价，bid or ofr
	bond_quote->volume_ = (int)getDouble("volume", each_bond_quote_data, 0);
	bond_quote->description_ = each_bond_quote_data->get<std::string>("description"); // 报价备注/描述
	bond_quote->institution_name_ = each_bond_quote_data->get<std::string>("institution_name"); // 机构名称
	bond_quote->fan_dian_flag_ = each_bond_quote_data->get<std::string>("fan_dian_flag"); // 是否返点， “1”- 有返点，“0”- 无返点
	bond_quote->fan_dian_ = getDouble("fan_dian", each_bond_quote_data, DOUBLE_NULL); // 返点数值, 比如返0.12
	bond_quote->bargain_flag_ = each_bond_quote_data->get<std::string>("bargain_flag"); // // 议价标识, 取值 0, 1, 2
	bond_quote->oco_flag_ = each_bond_quote_data->get<std::string>("oco_flag"); // OCO标识, 0-代表不显示，1-显示oco  2-oco打包
    bond_quote->internal_flag_ = each_bond_quote_data->get<std::string>("internal_flag"); // 1-正常报价，2-内部报价
	bond_quote->quote_type_ = each_bond_quote_data->get<std::string>("quote_type"); // 价格类型: 收益率/净价/全价/利差
	bond_quote->delivery_date_ = each_bond_quote_data->get<std::string>("delivery_date"); // 交割日
	bond_quote->yield_type_ = each_bond_quote_data->get<std::string>("yield_type"); // 收益类型
	bond_quote->company_id_ = _DEFALUT_COMPANY_ID_TP;
}

void MessageResendService::AUpdateBondDeal(const ptree* pt)
{
    LOGGER_INFO("Add or Update Deal From Quote - Old BC");
     
	BondDealService* bond_deal_service = (BondDealService*)ServiceManager::singleton()->getService(kServiceBondDeal);
    if (!bond_deal_service) {
		return;
    }
	BondDealBusiness business(bond_deal_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	ptree data = pt->get_child("data"); 
	
	// 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) {
		BondDealPtr bond_deal(new BondDeal());
		ptree each_bond_deal_data = it->second;
		bond_deal->id_ = each_bond_deal_data.get<std::string>("id");
		AssembleBondDeal(bond_deal, &each_bond_deal_data);
		
		sdbus::string exported_text;
		sdbus::string bond_id;

		BondDealCacheController bonddeal_cc;
		dbQuery q;
		q = "id = ", bond_deal->id_.ref();
		BondDealCachePtr cache = bonddeal_cc.getCacheByQueryInThreadSafty(q);
		if (cache) {
			business.UpdateBondDeal(*bond_deal, &exported_text, NULL);
		}
		else {
			business.AddBondDeal(*bond_deal, &exported_text, &bond_id, NULL, true);
		}
	}
}

void MessageResendService::DeleteBondDeal(const ptree* pt)
{
	LOGGER_INFO("Delete Bond Deal - Old BC");
     
	BondDealService* bond_deal_service = (BondDealService*)ServiceManager::singleton()->getService(kServiceBondDeal);
    if (!bond_deal_service) {
		return;
    }
	BondDealBusiness business(bond_deal_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	ptree data = pt->get_child("data");
	BondDealVecPtr vec(new std::vector<BondDealPtr>());
	// 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) {
		BondDealPtr bond_deal(new BondDeal());
		ptree each_bond_deal_data = it->second;
		bond_deal->id_ = each_bond_deal_data.get<std::string>("id");
		AssembleBondDeal(bond_deal, &each_bond_deal_data);
		vec->push_back(bond_deal);
	}

    business.DeleteBondDeal(vec, NULL);
}

void MessageResendService::AddUpdateSingleQuote(BondQuotePtr bond_quote, BondQuoteBusiness::BondQuoteVecPtr &add_vec, BondQuoteBusiness::BondQuoteVecPtr &update_vec) {
	BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
	if (!bond_quote_service) {
		return;
	}
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	BondQuoteCacheController bondquote_cc;
	dbQuery q;
	q = "id = ", bond_quote->id_.ref();
	BondQuoteCachePtr cache = bondquote_cc.getCacheByQueryInThreadSafty(q);

	if(cache) {            
		BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions(bond_quote->quote_side_.ref(), bond_quote->internal_flag_.ref(), 
			bond_quote->bond_key_listed_market_.ref(), bond_quote->institution_id_.ref(), bond_quote->trader_id_.ref());

		if(quotes && quotes->size() > 0) {
			BondQuotePtr delete_quote(new BondQuote());
			std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
			if((*it)->id != bond_quote->id_.ref()) {
				delete_quote->id_ = (*it)->id;
				BondQuoteBusiness::BondQuoteVecPtr vec_delete(new std::vector<BondQuotePtr>());
                vec_delete->push_back(delete_quote);
                business.DeleteQuote(vec_delete);
			}
		}

		sdbus::string exported_text;
        update_vec->push_back(bond_quote);
	} 
	else {
		BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions(bond_quote->quote_side_.ref(), bond_quote->internal_flag_.ref(), 
			bond_quote->bond_key_listed_market_.ref(), bond_quote->institution_id_.ref(), bond_quote->trader_id_.ref());
		sdbus::string exported_normal_text, exported_update_text;
		if(quotes->size() > 0) {
			std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
			bond_quote->id_ = (*it)->id;
            update_vec->push_back(bond_quote);
		} else {
            add_vec->push_back(bond_quote);
		}            
	}
}

void MessageResendService::EditReferredQuoteSingle(BondQuotePtr bond_quote)
{
	BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
	if (!bond_quote_service) {
		return;
	}
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
	BondQuoteBusiness::BondQuoteVecPtr vec_delete(new std::vector<BondQuotePtr>());

	bool export_is_internal = false;

	BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions(bond_quote->quote_side_.ref(), bond_quote->internal_flag_.ref(), 
		bond_quote->bond_key_listed_market_.ref(), bond_quote->institution_id_.ref(), bond_quote->trader_id_.ref());
	if(quotes->size() > 0) {
		BondQuotePtr delete_quote(new BondQuote());
		std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
        if((*it)->id != bond_quote->id_.ref()) {
            delete_quote->id_ = (*it)->id;
            vec_delete->push_back(delete_quote);
        }
	}
	vec_normal->push_back(bond_quote);

	sdbus::string exported_text;
	business.EditReferredQuote(vec_normal, &exported_text, export_is_internal, NULL);

	if(vec_delete->size() > 0) {
		business.DeleteQuote(vec_delete);
	}
}

void MessageResendService::AUpdateBondQuote(const ptree* pt)
{
    LOGGER_INFO("Add Quote - Old BC");
     
	BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
    if (!bond_quote_service) {
		return;
    }
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
    BondQuoteBusiness::BondQuoteVecPtr vec_add(new std::vector<BondQuotePtr>());
    BondQuoteBusiness::BondQuoteVecPtr vec_update(new std::vector<BondQuotePtr>());

	ptree data = pt->get_child("data");	// 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) {
		BondQuotePtr bond_quote(new BondQuote());
		ptree each_bond_quote_data = it->second;
		bond_quote->id_ = each_bond_quote_data.get<std::string>("id");
		AssembleBondQuote(bond_quote, &each_bond_quote_data);

		BondQuoteReferCacheController bondreferquote_cc;
		dbQuery q;
		q = "id = ", bond_quote->id_.ref();
		BondQuoteReferCachePtr cache = bondreferquote_cc.getCacheByQueryInThreadSafty(q);

		if(cache) {
			EditReferredQuoteSingle(bond_quote);
		}
		else {            
			AddUpdateSingleQuote(bond_quote, vec_add, vec_update);
		}
    }
    if(vec_update->size() > 0) {
        sdbus::string exported_update_text;
        business.UpdateQuote(vec_update, &exported_update_text, false, NULL);
    }
    if(vec_add->size() > 0) {
        sdbus::string exported_normal_text;
        business.AddQuote(vec_add, &exported_normal_text, false, NULL, true);
    }
}

void MessageResendService::ReferQuote(const ptree* pt) {
	LOGGER_INFO("Refer Quote - Old BC");
     
	BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
    if (!bond_quote_service) {
		return;
    }
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	ptree data = pt->get_child("data");

    BondQuoteBusiness::BondQuoteVecPtr vec_refer(new std::vector<BondQuotePtr>());
    BondQuoteBusiness::BondQuoteVecPtr vec_add(new std::vector<BondQuotePtr>());

    bool export_is_internal = false;
	// 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) {
		BondQuotePtr bond_quote(new BondQuote());
		ptree each_bond_quote_data = it->second;
		bond_quote->id_ = each_bond_quote_data.get<std::string>("id");

        BondQuoteCacheController bondquote_cc;
		dbQuery q;
		q = "id = ", bond_quote->id_.ref();
		BondQuoteCachePtr cache = bondquote_cc.getCacheByQueryInThreadSafty(q);

        BondQuoteReferCacheController bondquoterefer_cc;
        BondQuoteReferCachePtr refer_cache = bondquoterefer_cc.getCacheByQueryInThreadSafty(q);
        
        if(!cache && !refer_cache) {
            vec_add->push_back(bond_quote);
        }

        vec_refer->push_back(bond_quote);
	}

    sdbus::string exported_text;

    if(vec_add->size() > 0) {
        business.AddQuote(vec_add, &exported_text, export_is_internal, NULL);
    }

    business.ReferBondQuote(vec_refer, &exported_text, export_is_internal, NULL);  
	UpdateReferredQuote(pt);
}

void MessageResendService::DeleteReferredQuote(const ptree* pt)
{
	LOGGER_INFO("Delete Refer Quote - Old BC");

	BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
    if (!bond_quote_service) {
		return;
    }
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	ptree data = pt->get_child("data");

    BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());

	// 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) {
		BondQuotePtr bond_quote(new BondQuote());
		ptree each_bond_quote_data = it->second;
		bond_quote->id_ = each_bond_quote_data.get<std::string>("id");
		vec->push_back(bond_quote);
	}

    business.DeleteReferredQuote(vec, NULL, NULL);
}

void MessageResendService::DeleteQuote(const ptree* pt)
{
    LOGGER_INFO("Delete Bond Quote - Old BC");

	BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
    if (!bond_quote_service) {
		return;
    }
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	ptree data = pt->get_child("data");
  
    BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
	sdbus::string exported_text;   
    bool export_is_internal = false;

	// 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) {
		BondQuotePtr bond_quote(new BondQuote());
		ptree each_bond_quote_data = it->second;
		bond_quote->id_ = each_bond_quote_data.get<std::string>("id");
		vec->push_back(bond_quote);
	}

    business.DeleteQuote(vec, &exported_text, export_is_internal, NULL);
}

void MessageResendService::UnReferQuote(const ptree* pt)
{
    LOGGER_INFO("Unrefer Bond Quote");

    BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
    if (!bond_quote_service) {
		return;
    }
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	ptree data = pt->get_child("data");

    BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
    BondQuoteBusiness::BondQuoteVecPtr vec_delete(new std::vector<BondQuotePtr>());

    bool export_is_internal = false;

    // 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) { 

        BondQuotePtr bond_quote(new BondQuote());
        ptree each_bond_quote_data = it->second;
		bond_quote->id_ = each_bond_quote_data.get<std::string>("id");

        BondQuoteReferCacheVecPtr quote_refer = business.GetBondReferByID(bond_quote->id_.ref());
        if(quote_refer && quote_refer->size() > 0) {
            std::vector<BondQuoteReferCachePtr>::const_iterator refer_it = quote_refer->begin();

            BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions((*refer_it)->symbol, (*refer_it)->internally, 
                (*refer_it)->bond_key_listed_market, (*refer_it)->bank_id, (*refer_it)->bank_agent_id);
            if(quotes && quotes->size() > 0) {
                BondQuotePtr delete_quote(new BondQuote());
                std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();                
                if((*it)->id != bond_quote->id_.ref()) {
                    delete_quote->id_ = (*it)->id;
                    vec_delete->push_back(delete_quote);
                }
            } 
        }

        vec_normal->push_back(bond_quote);
    }
    sdbus::string exported_text;
	ptree bond_quote_msg;
    business.UnreferBondQuote(vec_normal, &exported_text, export_is_internal, &bond_quote_msg);

    if(vec_delete->size() > 0) {
        business.DeleteQuote(vec_delete);
    }
}

void MessageResendService::UpdateReferredQuote(const ptree* pt)
{
	LOGGER_INFO("Update Bond Quote");

    BondQuoteService* bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);
    if (!bond_quote_service) {
		return;
    }
	BondQuoteBusiness business(bond_quote_service);
	business.SetCompanyId(_DEFALUT_COMPANY_ID_TP);
	ptree data = pt->get_child("data");

	BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
	
	 // 遍历数组   
	for (ptree::iterator it = data.begin(); it != data.end(); ++it) { 

		BondQuotePtr bond_quote(new BondQuote());
		ptree each_bond_quote_data = it->second;
		bond_quote->id_ = each_bond_quote_data.get<std::string>("id");
		AssembleBondQuote(bond_quote, &each_bond_quote_data);
		vec_normal->push_back(bond_quote);
	}

	business.UpdateReferQuote(vec_normal, NULL, false, NULL);
}
