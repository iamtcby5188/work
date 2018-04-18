#include "publish_manager.h"
#include "increment/increment_cache_manager.h"
#include <boost/locale/encoding.hpp>	
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include "constants.h"
#include "business/VersionBusiness.h"
#include "increment/increment_cache.h"
#include "increment/increment_cache_manager.h"
#include <boost/assign/list_of.hpp>
PublishManager* PublishManager::singleton_ = NULL;

PublishManager* PublishManager::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new PublishManager();
    }

    return singleton_;
}

PublishManager::PublishManager()
    : conn_(NULL)
{

}

PublishManager::~PublishManager()
{

}

void PublishManager::init(sdbus::Connection *conn)
{
    conn_ = conn;

    boost::function0<void> f = boost::bind(&PublishManager::threadPubMsg, this);  
    boost::thread thrd(f); 
}

int PublishManager::getVersion(const std::string &model_name, const sdbus::Message* msg)
{
    /*boost::mutex::scoped_lock lock(mutex_);

    std::string account_id, company_id;
    if(msg != NULL) {
        msg->GetString(FID_ACCOUNT_ID, account_id);
        msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
        if(company_id.empty()){
            company_id = GetCurrentCompanyId(account_id);
        }

        if(!company_id.empty())
            company_id = "." + company_id;
    }

    std::map<std::string, int>::const_iterator it = version_mp_.find(model_name + company_id);
    if (it == version_mp_.end()) {
        return 0;
    }

    return (*it).second;*/
    std::string company_id = GetCurrentCompanyId(msg);

    int ver = VersionBusiness::GetVersion(model_name, company_id);
    return ver;
}

bool PublishManager::publish(const std::string &topic_name_, const std::string &model_name, sdbus::Message &msg, bool without_data /* = false*/, const std::string action_type/* = "Sync"*/)
{
    if (conn_ == NULL) {
        return false;
    }

    std::string companyId = GetCurrentCompanyId(&msg);
    std::string topic_name = topic_name_;
	if (!companyId.empty()) {
		topic_name = topic_name_ + "." + companyId;
	}

    boost::shared_ptr<PubMsg> pub_msg(new PubMsg);
    pub_msg->model_name = model_name;
    pub_msg->topic_name = topic_name;
    pub_msg->msg = boost::shared_ptr<sdbus::Message>(new sdbus::Message(msg));
    pub_msg->without_data = without_data;
	pub_msg->company_id = companyId;
    // get version and set version should be locked, also for multi application
    boost::mutex::scoped_lock lock(mutex_);
    int version = 0;
    VersionBusiness::IncOneAndGetVersion(model_name, companyId, version);
    pub_msg->version = version;
    pub_msg->action_type = action_type;

    boost::mutex::scoped_lock lock_queue(queue_mutex_);
    msg_queue_.push(pub_msg);
    queue_cond_var_.notify_one();

    return true;
}

static const std::vector<std::string> _MODEL_COMPANYS = boost::assign::list_of
	("BondQuote.QuoteList")("BondQuote.ReferQuoteList")("BondBestQuote.BestQuoteList")
	("BondDeal.DealList")("DcsBondDeal.DealList")
	("BondDetail.BondSmallViewDetail")("Bond.AccountInfo")("Bond.BondDeviatedValue")
	("Bond.InstitutionInfo")("Bond.TraderInfo")("Bond.BrokerBindingTrader");
void PublishManager::publish(const PubMsg &pub_msg)
{
    if (conn_ == NULL) {
        return;
    }

    std::string topic_name = pub_msg.topic_name;
    std::string model_name = pub_msg.model_name;
    boost::shared_ptr<sdbus::Message> msg = pub_msg.msg;
    bool without_data = pub_msg.without_data;
    int version = pub_msg.version;
    std::string action_type = pub_msg.action_type;

    msg->SetType(model_name);
    msg->SetInt32(FID_PUSH_MESSAGE_VERSION, version);

	std::string incCacheName = model_name;
	for (std::vector<std::string>::const_iterator it = _MODEL_COMPANYS.cbegin(); it != _MODEL_COMPANYS.cend(); ++it) {
		if (*it == incCacheName) {
			if (!pub_msg.company_id.empty())
				incCacheName += "." + pub_msg.company_id;
			break;
		}
	}
	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache(incCacheName);
	if (cache) {
		cache->Add(version, *msg.get());
	}

    bool flag = false;
    if (!without_data) {
        flag = conn_->Publish(topic_name, *msg);
    } else {
        sdbus::Message msg_no_data;
        msg_no_data.SetType(model_name);
        msg_no_data.SetInt32(FID_PUSH_MESSAGE_VERSION, version);
		msg_no_data.SetString(FID_ACTION_TYPE, action_type);
        
        flag = conn_->Publish(topic_name, msg_no_data);
    }
    
    if (flag) {
        LOGGER_INFO("Publish " << model_name << " Succeed. Version: " << version);
    } else {
        LOGGER_ERROR("Publish " << model_name << " Failed!");
    }
}

void PublishManager::threadPubMsg()
{    
    LOGGER_DEBUG("start thread to publish msg");
    while (true) {
        boost::shared_ptr<PubMsg> pub_msg;

        {
            boost::mutex::scoped_lock lock(queue_mutex_);

            while (msg_queue_.empty()) {
                queue_cond_var_.wait(lock);
            }

            pub_msg = msg_queue_.front();
            msg_queue_.pop();
        }

        publish(*pub_msg);
    }
}

