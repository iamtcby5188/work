#include "deviated_value_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/cache_controller_declear.h"
#include "cache/model/chinabonddeviatedvalue_cache.h"
#include "service/send_service.h"
#include <boost/function/function0.hpp>

void SetBondDeviatedValueCache(const sdbus::VariantMap *mp, ChinaBondDeviatedValueCache *cache)
{
    mp->GetString(FID_ID, cache->id);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->GetString(FID_BOND_TYPE_ID, cache->bond_type_id);
    mp->GetString(FID_BOND_TYPE, cache->bond_type_name);
    mp->GetDouble(FID_DIFF_WITH_VALUATION, cache->deviated_value);
    mp->GetString(FID_BOND_TYPE_STATUS, cache->status);
    mp->GetString(FID_BOND_TYPE_MODIFY_TIME, cache->modify_time);
}

//std::map<std::string, int> DeviatedValueDataManager::version_map_ = std::map<std::string, int>();

DeviatedValueDataManager::DeviatedValueDataManager(sdbus::Connection *conn, std::string companyId)
    : conn_(conn), company_id_(companyId)
{
    //SetVersion(company_id_, -1);
}

DeviatedValueDataManager::~DeviatedValueDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool DeviatedValueDataManager::init()
{    
    if (!Init(kTopicBondInfo + (company_id_.empty() ? "" : ("." + company_id_)), sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&DeviatedValueDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void DeviatedValueDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {

        boost::this_thread::interruption_point();
        const sdbus::Message * msg = *(msgs + i);

        if ("Bond.BondDeviatedValue" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())

        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}

void DeviatedValueDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in DeviatedValueDataManager" )

    while(true) {
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

void DeviatedValueDataManager::processSubMsg(const sdbus::Message* msg)
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
        FetchDeviatedValueRemote();
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
            if (!FetchIncrementDeviatedValueRemote(i)) {
                break;
            }
        }
    }   
}

bool DeviatedValueDataManager::FetchDeviatedValueRemote()
{    
    sdbus::Message request, response;
    request.SetType("Bond.BondDeviatedValue");
    request.SetSubject("model");
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("-----------------fetch deviated remote--------------------")
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

                ChinaBondDeviatedValueCacheController account_cc;
                std::string str = "company_id = '" + company_id_ + "'";
                dbQuery q(str.c_str());
                account_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

                for (unsigned int i = 0; i < list.Size(); ++i) {
                    const sdbus::VariantMap& mp = list.Get(i)->ToMap();

                    ChinaBondDeviatedValueCache cache;
                    SetBondDeviatedValueCache(&mp, &cache);

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

bool DeviatedValueDataManager::FetchIncrementDeviatedValueRemote(int version)
{    
    sdbus::Message request, response;
    request.SetType("Bond.BondDeviatedValue");
    request.SetSubject("increment");
    request.SetInt32(FID_PUSH_MESSAGE_VERSION, version);  
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("----------------fetch inc deviated remote------------------")
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

void DeviatedValueDataManager::AddList(const sdbus::VariantList &list)
{    
    ChinaBondDeviatedValueCacheController deviatedvalue_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        ChinaBondDeviatedValueCache cache;
        SetBondDeviatedValueCache(&mp, &cache);

        deviatedvalue_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
    }
    deviatedvalue_cc.commit();
}

void DeviatedValueDataManager::UpdateList(const sdbus::VariantList &list)
{
    ChinaBondDeviatedValueCacheController deviatedvalue_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        ChinaBondDeviatedValueCache cache;
        SetBondDeviatedValueCache(&mp, &cache);

        dbQuery q;
        q = "id = ", cache.id;
        deviatedvalue_cc.updateCacheInThreadSaftyWithoutDBSync(q, &cache, false);
    }
    deviatedvalue_cc.commit();
}

void DeviatedValueDataManager::DeleteList(const sdbus::VariantList &list)
{
    ChinaBondDeviatedValueCacheController deviatedvalue_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        ChinaBondDeviatedValueCache cache;
        SetBondDeviatedValueCache(&mp, &cache);

        dbQuery q;
        q = "id = ", cache.id;
        deviatedvalue_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q, false);
    }
    deviatedvalue_cc.commit();
}

int DeviatedValueDataManager::GetVersion(const std::string& company_id_) {
    return versionBusiness.GetVersion(mBond_BondDeviatedValue, company_id_);
}

void DeviatedValueDataManager::SetVersion(std::string company, int& version) {
    versionBusiness.SetVersion(mBond_BondDeviatedValue, company, version);
}