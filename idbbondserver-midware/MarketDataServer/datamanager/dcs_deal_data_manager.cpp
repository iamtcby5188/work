#include "dcs_deal_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/controller/dcsbonddeal_cachecontroller.h"
#include "cache/model/dcsbonddeal_cache.h"
#include "service/send_service.h"
#include "service/dcsbonddeal_service.h"
#include <boost/function/function0.hpp>
#include "service/service_manager.h"

#include "valuation_data_manager.h"

std::map<std::string, int> DcsDealDataManager::version_map_ = std::map<std::string, int>();

DcsDealDataManager::DcsDealDataManager(sdbus::Connection *conn, std::string companyId)
    : conn_(conn), company_id_(companyId)
{
    SetVersion(company_id_, -1);
}

DcsDealDataManager::~DcsDealDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool DcsDealDataManager::init()
{    
    if (!Init(kTopicDcsBondDeal + (company_id_.empty() ? "" : ("." + company_id_)), sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&DcsDealDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void DcsDealDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {
        const sdbus::Message * msg = *(msgs + i);
          
        if ("DcsBondDeal.DealList" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())

        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}

void DcsDealDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in DealDataManager" )

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

void DcsDealDataManager::processSubMsg(const sdbus::Message* msg)
{
    int new_version = -1;
    sdbus::string action_type;
    msg->GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
    msg->GetString(FID_ACTION_TYPE, action_type);
        
    LOGGER_DEBUG("-----------------process sub msg--------------------")
    LOGGER_DEBUG("[old_version]: " << GetVersion(company_id_))
    LOGGER_DEBUG("[new_version]: " << new_version)
    LOGGER_DEBUG("[action_type]: " << action_type)
        
    if (new_version == -1) {
        return;
    }

    if (GetVersion(company_id_) == -1 || (new_version < GetVersion(company_id_) && new_version == 1) || new_version > GetVersion(company_id_) + 30) {
        FetchDcsDealRemote();
        SetVersion(company_id_, new_version);
    } else if (new_version == GetVersion(company_id_) + 1) {        
        SetVersion(company_id_, new_version);

        if ("Sync" == action_type) {
            FetchDcsDealRemote();
            return;
        }

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
    } else if (new_version > GetVersion(company_id_) + 1 && new_version <= GetVersion(company_id_) + 30) {
        for (int i = GetVersion(company_id_) + 1; i <= new_version; ++i) {           
            if (!FetchIncrementDcsDealRemote(i)) {
                break;
            }
        }
    }     
}

bool DcsDealDataManager::FetchDcsDealRemote()
{    
	int total_row_counts = 0;	//总记录数，每次请求都返回总记录数
	int loop = 0;				//标记分页次数，设置分页偏移
	int ncounts = 0;			//返回结果数集合

    DcsBondDealCacheController bonddeal_cc;
    std::string str = "company_id = '" + company_id_ + "'";
    dbQuery q(str.c_str());
    bonddeal_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

	LOGGER_DEBUG("-----------------fetch Dcs deal remote--------------------");
	do 
	{		
        std::string offset = IntToString(loop * kServiceSyncLimit);
        std::string range = offset + ", " + IntToString(kServiceSyncLimit);  //每次请求kServiceSyncLimit条数据
		
		sdbus::Message request, response;
		request.SetType("DcsBondDeal.DealList");
		request.SetSubject("model");
		request.SetString(FID_RANGE, range);
        request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);

		if (SendService::singleton()->call(kServiceQueueDcsBondDeal, request, response, 90000)) 
		{
			sdbus::Message unzip_msg;
			if (unzipMessage(response, unzip_msg)) 
			{
				LOGGER_DEBUG("[unzip] type: " << unzip_msg.GetType());
				unzip_msg.GetInt32(FID_ROW_COUNT, total_row_counts);

				int new_version = -1;
				sdbus::VariantList list;
				unzip_msg.GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
				unzip_msg.GetList(FID_LIST, list);           
				LOGGER_DEBUG("[new_version]: " << new_version);
				LOGGER_DEBUG("[size]: " << list.Size());

				ncounts += list.Size();	

				if (new_version >= 0)
				{     
					SetVersion(company_id_, new_version);

					for (unsigned int i = 0; i < list.Size(); ++i) 
					{
						const sdbus::VariantMap& mp = list.Get(i)->ToMap();

                        DcsBondDealCache cache;
                        DcsBondDealService::SetBondDealCache(&mp, &cache);

						ValuationDataManager vdm(conn_);
						vdm.FetchValuationLocal(cache);

						bonddeal_cc.insertCacheInThreadSaftyWithoutDBSync(&cache);
					}
				}
			}

			++loop;
			LOGGER_INFO("---Dcs Bond Deal Requested Rows: " << ncounts);
			LOGGER_INFO("---Dcs Bond Deal Total Rows: " << total_row_counts << "----Request times: " << loop);
		}
    } while (total_row_counts != 0 && total_row_counts > ncounts && ncounts >= loop * kServiceSyncLimit);

	if (total_row_counts == ncounts) {
		return true;
	} else {
		return false;
	}   
}

bool DcsDealDataManager::FetchIncrementDcsDealRemote(int version)
{    
    sdbus::Message request, response;
    request.SetType("DcsBondDeal.DealList");
    request.SetSubject("increment");
    request.SetInt32(FID_PUSH_MESSAGE_VERSION, version);
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("----------------fetch inc deal remote------------------")
    if (SendService::singleton()->call(kServiceQueueDcsBondDeal, request, response, 90000)) {
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

void DcsDealDataManager::AddList(const sdbus::VariantList &list)
{    
    DcsBondDealCacheController bonddeal_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        DcsBondDealCache cache;
        DcsBondDealService::SetBondDealCache(&mp, &cache);

        bonddeal_cc.insertCacheInThreadSaftyWithoutDBSync(&cache);
    }
}

void DcsDealDataManager::UpdateList(const sdbus::VariantList &list)
{
    DcsBondDealCacheController bonddeal_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        DcsBondDealCache cache;
        DcsBondDealService::SetBondDealCache(&mp, &cache);
        
        dbQuery q;
		q = "id = ", cache.id;
        bonddeal_cc.updateCacheInThreadSaftyWithoutDBSync(q, &cache);
    }
}

void DcsDealDataManager::DeleteList(const sdbus::VariantList &list)
{
	if(list.Empty()){
		return;
	}

    DcsBondDealCacheController bonddeal_cc;
	std::string sql = "id in (";
    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

		DcsBondDealCache cache;
        DcsBondDealService::SetBondDealCache(&mp, &cache);
		
		//dbQuery q;
		//q = "id = ", cache.id;
		//bonddeal_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
		if(i > 0){
			sql += ",";
		}
		sql += "'" + cache.id + "'";
    }
	sql += ")";
	dbQuery q(sql.c_str());
	bonddeal_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
}
