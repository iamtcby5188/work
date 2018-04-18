#include "account_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/model/account_cache.h"
#include "service/send_service.h"
#include "sync/bondquote_sync.h"
#include "sync/bondreferquote_sync.h"
#include "sync/bonddeal_sync.h"
#include <boost/function/function0.hpp>
#include <map>

void SetAccountInfoCache(const sdbus::VariantMap *mp, AccountCache *cache)
{
    mp->GetString(FID_ID, cache->id);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
	mp->GetString(FID_ACCOUNT_COMPANY_NAME, cache->company_name);
    mp->GetString(FID_LOGIN_NAME, cache->username);
    mp->GetString(FID_USER_NAME, cache->display_name);
    mp->GetString(FID_ACCOUNT_PASSWORD, cache->password);
    mp->GetString(FID_ACCOUNT_CODE, cache->account_code);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->GetString(FID_ACCOUNT_DEPARTMENT, cache->department_codes);
    mp->GetString(FID_ACCOUNT_ROLE, cache->role_codes);
    mp->GetString(FID_ACCOUNT_PERMISSION, cache->permission_codes);
    mp->GetString(FID_ACCOUNT_CODE, cache->account_code);
    mp->GetString(FID_PIN_YIN, cache->pinyin);
    mp->GetString(FID_MODIFY_TIME, cache->modify_time);
}

//std::map<std::string, int> AccountDataManager::version_map_ = std::map<std::string, int>();

AccountDataManager::AccountDataManager(sdbus::Connection *conn, std::string companyId)
    : conn_(conn), company_id_(companyId)
{
    //SetVersion(company_id_, -1);
}

AccountDataManager::~AccountDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool AccountDataManager::init()
{    
    if (!Init(kTopicBondInfo + (company_id_.empty() ? "" : ("." + company_id_)), sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&AccountDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void AccountDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {
        const sdbus::Message * msg = *(msgs + i);
          
        if ("Bond.AccountInfo" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())

        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}

void AccountDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in AccountDataManager" )

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

void AccountDataManager::processSubMsg(const sdbus::Message* msg)
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
        FetchAccountRemote();
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
            if (!FetchIncrementAccountRemote(i)) {
                break;
            }
        }
    }   
}

bool AccountDataManager::FetchAccountRemote()
{    
    sdbus::Message request, response;
    request.SetType("Bond.AccountInfo");
    request.SetSubject("model");
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("-----------------fetch account remote--------------------")
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

                AccountCacheController account_cc;
                std::string str = "company_id = '" + company_id_ + "'";
                dbQuery q(str.c_str());
                account_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

                for (unsigned int i = 0; i < list.Size(); ++i) {
                    const sdbus::VariantMap& mp = list.Get(i)->ToMap();

                    AccountCache cache;
                    SetAccountInfoCache(&mp, &cache);

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

bool AccountDataManager::FetchIncrementAccountRemote(int version)
{    
    sdbus::Message request, response;
    request.SetType("Bond.AccountInfo");
    request.SetSubject("increment");
    request.SetInt32(FID_PUSH_MESSAGE_VERSION, version);
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("----------------fetch inc account remote------------------")
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

void AccountDataManager::AddList(const sdbus::VariantList &list)
{    
    AccountCacheController account_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        
	    AccountCache cache;
        SetAccountInfoCache(&mp, &cache);

        account_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
    }
    account_cc.commit();
}

void AccountDataManager::UpdateList(const sdbus::VariantList &list)
{
    AccountCacheController account_cc;
    std::vector<std::string> updated_account_vec;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        	    
        AccountCache cache;
        SetAccountInfoCache(&mp, &cache);
        
        dbQuery q;
		q = "id = ", cache.id;
        account_cc.updateCacheInThreadSaftyWithoutDBSync(q, &cache, false);

        updated_account_vec.push_back(cache.id);
    }
    account_cc.commit();
    
//    if (updated_account_vec.size() > 0) {
//        BondQuoteSync bondquote_sync;
//        bondquote_sync.syncForAccountChanged(updated_account_vec);

//        BondReferQuoteSync bondreferquote_sync;
//        bondreferquote_sync.syncForAccountChanged(updated_account_vec);

//        BondDealSync bonddeal_sync;
//        bonddeal_sync.syncForAccountChanged(updated_account_vec);
//    }
}

void AccountDataManager::DeleteList(const sdbus::VariantList &list)
{
    AccountCacheController account_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

	    AccountCache cache;
        SetAccountInfoCache(&mp, &cache);
        
        dbQuery q;
		q = "id = ", cache.id;
        account_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q, false);
    }
    account_cc.commit();
}

int AccountDataManager::GetVersion(const std::string& company_id_) {
    return versionBusiness.GetVersion(mBond_AccountInfo, company_id_);
}

void AccountDataManager::SetVersion(std::string company, int& version) {
    versionBusiness.SetVersion(mBond_AccountInfo, company, version);
}
