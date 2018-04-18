#include "broker_binding_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "cache/model/brokerbindingtrader_cache.h"
#include "service/send_service.h"
#include <boost/function/function0.hpp>

void SetBrokerBindingTraderCache(const sdbus::VariantMap *mp, BrokerBindingTraderCache *cache)
{
    mp->GetString(FID_ID, cache->id);
    mp->GetString(FID_BROKER_ID, cache->account_id);
    mp->GetString(FID_TRADER_ID, cache->bank_agent_id);
    mp->GetString(FID_ACCOUNT_DEPARTMENT, cache->account_department);
	mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
}

//std::map<std::string, int> BrokerBindingDataManager::version_map_ = std::map<std::string, int>();

BrokerBindingDataManager::BrokerBindingDataManager(sdbus::Connection *conn, std::string companyId)
    : conn_(conn), company_id_(companyId)
{
    //SetVersion(company_id_, -1);
}

BrokerBindingDataManager::~BrokerBindingDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool BrokerBindingDataManager::init()
{    
    if (!Init(kTopicBondInfo + (company_id_.empty() ? "" : ("." + company_id_)), sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&BrokerBindingDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void BrokerBindingDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {
        const sdbus::Message * msg = *(msgs + i);

        if ("Bond.BrokerBindingTrader" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())

        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}

void BrokerBindingDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in BrokerBindingDataManager" )

    while(true) {

        boost::this_thread::interruption_point();
        sdbus::Message msg;

        {
            boost::mutex::scoped_lock lock(sub_queue_mutex_);

            while(sub_msg_queue_.empty()) {        
                sub_queue_cond_var_.wait(lock);
            }

            msg = sub_msg_queue_.front();
            sub_msg_queue_.pop();
        }

        processSubMsg(&msg);

    }
}

void BrokerBindingDataManager::processSubMsg(const sdbus::Message* msg)
{
    int new_version = -1;
    sdbus::string action_type;
    msg->GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
    msg->GetString(FID_ACTION_TYPE, action_type);
    int oldVersion = GetVersion(company_id_);
    LOGGER_DEBUG("-----------------process sub msg--------------------");
    LOGGER_DEBUG("[old_version]: " << oldVersion);
    LOGGER_DEBUG("[new_version]: " << new_version);
    LOGGER_DEBUG("[action_type]: " << action_type);

    if (new_version == -1) {
        return;
    }

    if (oldVersion == -1 || (new_version < oldVersion && new_version == 1) || new_version > oldVersion + 30) {
        FetchBrokerBindingRemote();
    } else if (new_version == oldVersion + 1) {
        SetVersion(company_id_, new_version);

        sdbus::VariantList list;
        msg->GetList(FID_LIST, list);

        LOGGER_DEBUG("[size]: " << list.Size())

        if (list.Size() == 0) {
            return;
        }

        if ("AddList" == action_type) {
            AddList(list);
        } else if ("UpdateList" == action_type) {
            UpdateList(list);
        } else if ("DeleteList" == action_type) {
            DeleteList(list);
        }
    } else if (new_version > oldVersion + 1 && new_version <= oldVersion + 30) {
        for (int i = oldVersion + 1; i <= new_version; ++i) {
            if (!FetchIncrementBrokerBindingRemote(i)) {
                break;
            }
        }
    }   
}

bool BrokerBindingDataManager::FetchBrokerBindingRemote()
{    
    sdbus::Message request, response;
    request.SetType("Bond.BrokerBindingTrader");
    request.SetSubject("model");
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("-----------------fetch binding remote--------------------")
    if (SendService::singleton()->call(kServiceQueueBondInfo, request, response, 30000)) {
        sdbus::Message unzip_msg;
        if (unzipMessage(response, unzip_msg)) {
            LOGGER_DEBUG("[unzip] type: " << unzip_msg.GetType())

            int new_version = -1;
            sdbus::VariantList list;
            unzip_msg.GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
            unzip_msg.GetList(FID_LIST, list);           
            LOGGER_DEBUG("[new_version]: " << new_version)
            LOGGER_DEBUG("[size]: " << list.Size())

            if (new_version >= 0) {     
                SetVersion(company_id_, new_version);

                BrokerBindingTraderCacheController account_cc;
                std::string str = "company_id = '" + company_id_ + "'";
                dbQuery q(str.c_str());
                account_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

                for (unsigned int i = 0; i < list.Size(); ++i) {
                    const sdbus::VariantMap& mp = list.Get(i)->ToMap();

                    BrokerBindingTraderCache cache;
                    SetBrokerBindingTraderCache(&mp, &cache);

                    account_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
                }
                account_cc.commit();
            }
        }
        LOGGER_DEBUG("------------------------------------------------")
            return true;
    }  

    return false;
}

bool BrokerBindingDataManager::FetchIncrementBrokerBindingRemote(int version)
{    
    sdbus::Message request, response;
    request.SetType("Bond.BrokerBindingTrader");
    request.SetSubject("increment");
    request.SetInt32(FID_PUSH_MESSAGE_VERSION, version);  
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("----------------fetch inc binding remote------------------")
    if (SendService::singleton()->call(kServiceQueueBondInfo, request, response, 30000)) {
        sdbus::Message unzip_msg;
        if (unzipMessage(response, unzip_msg)) {
            LOGGER_DEBUG("[unzip] type: " << unzip_msg.GetType())

            int new_version = -1;
            sdbus::string action_type;
            unzip_msg.GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
            unzip_msg.GetString(FID_ACTION_TYPE, action_type);

            LOGGER_DEBUG("[new_version]: " << new_version)
            LOGGER_DEBUG("[action_type]: " << action_type)

            if (new_version >= 0) {
                SetVersion(company_id_, new_version);

                sdbus::VariantList list;
                unzip_msg.GetList(FID_LIST, list);

                LOGGER_DEBUG("[size]: " << list.Size())

                if (list.Size() > 0) {
                    if ("AddList" == action_type) {
                        AddList(list);
                    } else if ("UpdateList" == action_type) {
                        UpdateList(list);
                    } else if ("DeleteList" == action_type) {
                        DeleteList(list);
                    }    
                }
            }
        }

        LOGGER_DEBUG("------------------------------------------------")
        return true;
    } 

    return false;  
}

void BrokerBindingDataManager::AddList(const sdbus::VariantList &list)
{    
    BrokerBindingTraderCacheController brokerbinding_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        BrokerBindingTraderCache cache;
        SetBrokerBindingTraderCache(&mp, &cache);

        brokerbinding_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
    }
    brokerbinding_cc.commit();
}

void BrokerBindingDataManager::UpdateList(const sdbus::VariantList &list)
{
    BrokerBindingTraderCacheController brokerbinding_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        BrokerBindingTraderCache cache;
        SetBrokerBindingTraderCache(&mp, &cache);

        dbQuery q;
        q = "id = ", cache.id;
        brokerbinding_cc.updateCacheInThreadSaftyWithoutDBSync(q, &cache, false);
    }
    brokerbinding_cc.commit();
}

void BrokerBindingDataManager::DeleteList(const sdbus::VariantList &list)
{
    BrokerBindingTraderCacheController brokerbinding_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        BrokerBindingTraderCache cache;
        SetBrokerBindingTraderCache(&mp, &cache);

        dbQuery q;
        q = "id = ", cache.id;
        brokerbinding_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q, false);
    }
    brokerbinding_cc.commit();
}

int BrokerBindingDataManager::GetVersion(const std::string& company_id_) {
    return versionBusiness.GetVersion(mBond_BrokerBindingTrader, company_id_);
}

void BrokerBindingDataManager::SetVersion(std::string company, int& version) {
    versionBusiness.SetVersion(mBond_BrokerBindingTrader, company, version);
}
