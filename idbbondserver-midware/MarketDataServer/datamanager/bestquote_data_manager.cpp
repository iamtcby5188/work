#include "bestquote_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/model/bondbestquote_cache.h"
#include "valuation_data_manager.h"
#include "service/send_service.h"
#include "service/bondbestquote_service.h"
#include <boost/function/function0.hpp>
#include "service/service_manager.h"

std::map<std::string, int> BestQuoteDataManager::version_map_ = std::map<std::string, int>();

BestQuoteDataManager::BestQuoteDataManager(sdbus::Connection *conn, std::string companyId)
    : conn_(conn), company_id_(companyId)
{
    SetVersion(company_id_, -1);
}

BestQuoteDataManager::~BestQuoteDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool BestQuoteDataManager::init()
{    
    if (!Init(kTopicBondBestQuote + (company_id_.empty() ? "" : ("." + company_id_)), sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&BestQuoteDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void BestQuoteDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {
        const sdbus::Message * msg = *(msgs + i);
    
        if ("BondBestQuote.BestQuoteList" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())
        
        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}

void BestQuoteDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in BestQuoteDataManager" )

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

void BestQuoteDataManager::processSubMsg(const sdbus::Message* msg)
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
        FetchBestQuoteRemote();
    } else if (new_version == GetVersion(company_id_) + 1) {        
        SetVersion(company_id_, new_version);

        if ("Sync" == action_type) {
            FetchBestQuoteRemote();
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
            if (!FetchIncrementBestQuoteRemote(i)) {
                break;
            }
        }
    }
}

bool BestQuoteDataManager::FetchBestQuoteRemote()
{    
	int total_row_counts = 0;	//总记录数，每次请求都返回总记录数
	int loop = 0;				//标记分页次数，设置分页偏移
	int ncounts = 0;			//返回结果数集合

	BondBestQuoteCacheController bondbestquote_cc;
    std::string str = "company_id = '" + company_id_ + "'";
    dbQuery q(str.c_str());
    bondbestquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

    LOGGER_DEBUG("---------------fetch best quote remote------------------");
	do 
	{
		std::string offset = IntToString(loop * kServiceSyncLimit);
        std::string range = offset + ", " + IntToString(kServiceSyncLimit);  //每次请求kServiceSyncLimit条数据

		sdbus::Message request, response;
		request.SetType("BondBestQuote.BestQuoteList");
		request.SetSubject("model");
		request.SetString(FID_RANGE, range);
        request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);

		if (SendService::singleton()->call(kServiceQueueBondBestQuote, request, response, 90000)) 
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

					for (size_t i = 0; i < list.Size(); ++i) {
						const sdbus::VariantMap& mp = list.Get(i)->ToMap();

						BondBestQuoteCache cache;
						BondBestQuoteService::SetBondBestQuoteCache(&mp, &cache);

						ValuationDataManager vdm(conn_);
						vdm.FetchValuationLocal(cache);

						bondbestquote_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
					}
                    bondbestquote_cc.commit();
				}
			}

			++loop;
			LOGGER_INFO("---Bond Best Quote Requested Rows: " << ncounts);
			LOGGER_INFO("---Bond Best Quote Total Rows: " << total_row_counts << "----Request times: " << loop);
		}

    } while (total_row_counts != 0 && total_row_counts > ncounts && ncounts >= loop * kServiceSyncLimit);

	if (total_row_counts == ncounts) {
		return true;
	} else {
		return false;
	}
}

bool BestQuoteDataManager::FetchIncrementBestQuoteRemote(int version)
{    
    sdbus::Message request, response;
    request.SetType("BondBestQuote.BestQuoteList");
    request.SetSubject("increment");
    request.SetInt32(FID_PUSH_MESSAGE_VERSION, version);
    request.SetString(FID_ACCOUNT_COMPANY_ID, company_id_);
    LOGGER_DEBUG("--------------fetch inc best quote remote----------------")
    if (SendService::singleton()->call(kServiceQueueBondBestQuote, request, response, 90000)) {
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

void BestQuoteDataManager::AddList(const sdbus::VariantList &list)
{    
    BondBestQuoteCacheController bondbestquote_cc;

    for (size_t i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        
		BondBestQuoteCache cache;
        BondBestQuoteService::SetBondBestQuoteCache(&mp, &cache);

        bondbestquote_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
    }
    bondbestquote_cc.commit();
}

void BestQuoteDataManager::UpdateList(const sdbus::VariantList &list)
{
    BondBestQuoteCacheController bondbestquote_cc;

    for (size_t i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        		
        BondBestQuoteCache cache;
        BondBestQuoteService::SetBondBestQuoteCache(&mp, &cache);
        
        dbQuery q;
		q = "bond_key_listed_market = ", cache.bond_key_listed_market;
        //bondbestquote_cc.updateCacheInThreadSaftyWithoutDBSync(q, &cache);
        // for update, delete old data first
        if (!bondbestquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q, false)) {
            LOGGER_ERROR("delete old bond best quote occur failure!")        
        }
        if (!bondbestquote_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false))
        {
            LOGGER_ERROR("insert bond best quote occur failure!")
        }
    }
    bondbestquote_cc.commit();
}

void BestQuoteDataManager::DeleteList(const sdbus::VariantList &list)
{
	if(list.Empty()){
		return;
	}

    BondBestQuoteCacheController bondbestquote_cc;
	std::string sql = "id in (";
    for (size_t i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

        BondBestQuoteCache cache;
        BondBestQuoteService::SetBondBestQuoteCache(&mp, &cache);
		
		//dbQuery q;
		//q = "bond_key_listed_market = ", cache.bond_key_listed_market;
		//bondbestquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
		if(i > 0){
			sql += ",";
		}
		sql += "'" + cache.id + "'";
    }
	sql += ")";
	dbQuery q(sql.c_str());
	bondbestquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
}
