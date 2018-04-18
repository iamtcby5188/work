#include "referquote_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/model/bondquoterefer_cache.h"
#include "service/send_service.h"
#include "service/bondquote_service.h"
#include <boost/function/function0.hpp>
#include "valuation_data_manager.h"
#include "service/service_manager.h"

std::map<std::string, int> ReferQuoteDataManager::version_map_ = std::map<std::string, int>();

ReferQuoteDataManager::ReferQuoteDataManager(sdbus::Connection *conn, std::string companyId)
    : conn_(conn), company_id_(companyId)
{
    SetVersion(company_id_, -1);
}

ReferQuoteDataManager::~ReferQuoteDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool ReferQuoteDataManager::init()
{
    if (!Init(kTopicBondQuote + (company_id_.empty() ? "" : ("." + company_id_)), sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }
    
    boost::function0<void> f = boost::bind(&ReferQuoteDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void ReferQuoteDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {
        const sdbus::Message * msg = *(msgs + i);
          
        if ("BondQuote.ReferQuoteList" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())
        
        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}


void ReferQuoteDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in ReferQuoteDataManager" )

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

void ReferQuoteDataManager::processSubMsg(const sdbus::Message* msg)
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
        FetchReferQuoteRemote();
        SetVersion(company_id_, new_version);
    } else if (new_version == GetVersion(company_id_) + 1) {        
        SetVersion(company_id_, new_version);

        if ("Sync" == action_type) {
            FetchReferQuoteRemote();
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
            if (!FetchIncrementReferQuoteRemote(i)) {
                break;
            }
        }
    }
    LOGGER_DEBUG("-----------------process sub msg end--------------------")
}

bool ReferQuoteDataManager::FetchReferQuoteRemote()
{    
    int total_row_counts = 0;	//总记录数，每次请求都返回总记录数
	int loop = 0;				//标记分页次数，设置分页偏移
	int ncounts = 0;			//返回结果数集合
	
	BondQuoteReferCacheController bondquoterefer_cc;
    std::string str = "company_id = '" + company_id_ + "'";
    dbQuery q(str.c_str());
    bondquoterefer_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

	LOGGER_DEBUG("-----------------fetch refer quote remote--------------------")
	do 
	{		
        std::string offset = IntToString(loop * kServiceSyncLimit);
        std::string range = offset + ", " + IntToString(kServiceSyncLimit);  //每次请求kServiceSyncLimit条数据
		
		sdbus::Message request, response;
		request.SetType("BondQuote.ReferQuoteList");
		request.SetSubject("model");
		request.SetString(FID_RANGE, range);
        request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);

		if (SendService::singleton()->call(kServiceQueueBondQuote, request, response, 90000)) 
		{
			sdbus::Message unzip_msg;
			if (unzipMessage(response, unzip_msg)) 
			{
				LOGGER_DEBUG("[unzip] type: " << unzip_msg.GetType());
				unzip_msg.GetInt32(FID_ROW_COUNT, total_row_counts);

				int new_version = -1;				
				unzip_msg.GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
				sdbus::VariantList list;
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

						BondQuoteReferCache cache;
						BondQuoteService::SetBondReferQuoteCache(&mp, &cache);

						ValuationDataManager vdm(conn_);
						vdm.FetchValuationLocal(cache);

						bondquoterefer_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
					}
                    bondquoterefer_cc.commit();
				}
			}

			++loop;

			LOGGER_INFO("---Bond Refer Quote Requested Rows: " << ncounts);
			LOGGER_INFO("---Bond Refer Quote Total Rows: " << total_row_counts << "----Request times: " << loop);
		} //end if
    } while (total_row_counts != 0 && total_row_counts > ncounts && ncounts >= loop * kServiceSyncLimit);

	if (total_row_counts == ncounts) {
		return true;
	} else {
		return false;
	}
}

bool ReferQuoteDataManager::FetchIncrementReferQuoteRemote(int version)
{    
    sdbus::Message request, response;
    request.SetType("BondQuote.ReferQuoteList");
    request.SetSubject("increment");
    request.SetInt32(FID_PUSH_MESSAGE_VERSION, version);
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("----------------fetch inc quote remote------------------")
    if (SendService::singleton()->call(kServiceQueueBondQuote, request, response, 90000)) {
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

void ReferQuoteDataManager::AddList(const sdbus::VariantList &list)
{    
    BondQuoteReferCacheController bondquoterefer_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        
		BondQuoteReferCache cache;
        BondQuoteService::SetBondReferQuoteCache(&mp, &cache);

        bondquoterefer_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
    }
    bondquoterefer_cc.commit();
}

void ReferQuoteDataManager::UpdateList(const sdbus::VariantList &list)
{
    BondQuoteReferCacheController bondquoterefer_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        
		BondQuoteReferCache cache;
        BondQuoteService::SetBondReferQuoteCache(&mp, &cache);
        
        dbQuery q;
		q = "id = ", cache.id;
        bondquoterefer_cc.updateCacheInThreadSaftyWithoutDBSync(q, &cache, false);
    }
    bondquoterefer_cc.commit();
}

void ReferQuoteDataManager::DeleteList(const sdbus::VariantList &list)
{
	if(list.Empty()){
		return;
	}

    BondQuoteReferCacheController bondquoterefer_cc;
	std::string sql = "id in (";
    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        		
        BondQuoteReferCache cache;
        BondQuoteService::SetBondReferQuoteCache(&mp, &cache);
		
		//dbQuery q;
		//q = "id = ", cache.id;
		//bondquoterefer_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
		if(i > 0){
			sql += ",";
		}
		sql += "'" + cache.id + "'";
    }
	sql += ")";
	dbQuery q(sql.c_str());
	bondquoterefer_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
}
