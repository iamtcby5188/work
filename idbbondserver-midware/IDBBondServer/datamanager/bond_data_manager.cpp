#include "bond_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/model/bondsmallview_cache.h"
#include "service/send_service.h"
#include "sync/bondquote_sync.h"
#include "sync/bondreferquote_sync.h"
#include "sync/bonddeal_sync.h"
#include "sync/bondbestquote_sync.h"
#include <boost/function/function0.hpp>

void SetBondSmallViewDetailCache(const sdbus::VariantMap *mp, BondSmallViewCache *cache)
{
    mp->GetString(FID_LISTED_MARKET, cache->listed_market);
    mp->GetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->GetString(FID_BOND_CODE, cache->bond_id);
    mp->GetString(FID_BOND_SUBTYPE, cache->bond_subtype);
    mp->GetString(FID_ISSUER_RATING, cache->issuer_rating_current);
    mp->GetString(FID_BOND_SHORT_NAME, cache->short_name);
    mp->GetString(FID_COUPON_TYPE, cache->coupon_type);
    mp->GetString(FID_OPTION_TYPE, cache->option_client);
    mp->GetString(FID_PIN_YIN, cache->pin_yin);
    mp->GetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->GetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
    mp->GetDouble(FID_COUPON_RATE, cache->coupon_rate_current);
    mp->GetInt32(FID_MATURITY, cache->maturity_date);
    mp->GetInt32(FID_INTEREST_START_DATE, cache->interest_start_date);
    mp->GetInt32(FID_LISTED_DATE, cache->listed_date);
    mp->GetInt32(FID_REDEMPTION_NO, cache->redemption_no);
    mp->GetString(FID_ENTERPRISE_TYPE, cache->institution_type);
    mp->GetString(FID_BOND_RATING, cache->rating_current);
    mp->GetString(FID_BOND_KEY, cache->bond_key);
    mp->GetString(FID_MARKET_TYPE, cache->mkt_type);
    mp->GetInt32(FID_OPTION_DATE, cache->option_date);
    mp->GetDouble(FID_ISSUE_AMOUNT, cache->issue_amount);
    mp->GetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_current_npy);
    mp->GetString(FID_BOND_RATING_NPY, cache->rating_current_npy);

    mp->GetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);
    mp->GetString(FID_BOND_TYPE, cache->goods_type_client);
    mp->GetString(FID_BOND_CATEGORY, cache->product_type);
    mp->GetBool(FID_BOND_IS_EXPIRED, cache->is_expired);
	
    // ----------------- Combination Fields ----------------------
    //
    mp->GetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
    mp->GetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);

    mp->GetString(FID_GUARANTEE_TYPE, cache->rating_augment_client);
    mp->GetString(FID_RATE_TYPE, cache->coupon_type_client);
    mp->GetString(FID_RATING_INSTITUTION, cache->rating_institution_client);
    mp->GetString(FID_ISSUE_INSTITUTION, cache->issuer_client);
    mp->GetString(FID_ISSUE_INSTITUTION_PINYIN, cache->issue_institution_pinyin_);
    mp->GetString(FID_ISSUE_INSTITUTION_PINYINFULL, cache->issue_institution_pinyinfull_);
    mp->GetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);
    mp->GetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
    // ------------------------- Combined Fields (above) ----------------------------
    mp->GetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->GetDouble(FID_TIME_TO_MATURITY_REAL, cache->time_to_maturity_real);
    cache->ttm = GetTimeToMaturity(cache->maturity_date, cache->interest_start_date);
    mp->GetString(FID_HAS_OPTION, cache->has_option);
    mp->GetString(FID_ISSUER_RATING_FILTER, cache->issuer_rating_filter);
    mp->GetString(FID_GUARENTEED, cache->guarenteed);
    mp->GetString(FID_MUNICIPAL, cache->municipal); 
    mp->GetString(FID_RATE_INDEX, cache->rate_index);
    mp->GetString(FID_PLEDGED, cache->pledged);
    mp->GetString(FID_ACROSS_MARKET, cache->across_market);
	mp->GetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->GetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code);
	mp->GetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code);
	mp->GetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->GetString(FID_FINANCING_PLATFORM, cache->financing_platform);
	mp->GetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id);
    mp->GetString(FID_COMPLETED_BOND_TYPE, cache->completed_bond_type);
    mp->GetInt32(FID_COMPLETED_QUOTE_SORT, cache->bond_type_sort_client);
    mp->GetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
	mp->GetString(FID_COMPANY_BOND, cache->ent_cor);
	mp->GetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
}

//int BondDataManager::version_ = -1;

BondDataManager::BondDataManager(sdbus::Connection *conn)
    : conn_(conn)
{

}

BondDataManager::~BondDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool BondDataManager::init()
{    
    if (!Init(kTopicBondDetail, sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&BondDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void BondDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {
        const sdbus::Message * msg = *(msgs + i);
          
		if ("BondDetail.BondSmallViewDetail" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())

        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}

void BondDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in BondDataManager" )

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

void BondDataManager::processSubMsg(const sdbus::Message* msg)
{
    int new_version = -1;
    sdbus::string action_type;
    msg->GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
    msg->GetString(FID_ACTION_TYPE, action_type);
    int oldVersion = GetVersion();
    LOGGER_DEBUG("-----------------process sub msg--------------------");
    LOGGER_DEBUG("[old_version]: " << oldVersion);
    LOGGER_DEBUG("[new_version]: " << new_version);
    LOGGER_DEBUG("[action_type]: " << action_type);

    if (new_version == -1) {
        return;
    }

    if (oldVersion == -1 || (new_version < oldVersion && new_version == 1) || new_version > oldVersion + 30) {
        FetchBondRemote();
    } else if (new_version == oldVersion + 1) {
        SetVersion(new_version);

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
            if (!FetchIncrementBondRemote(i)) {
                break;
            }
        }
    }   
}

bool BondDataManager::FetchBondRemote()
{    
	/*sdbus::Message request, response;
    request.SetType("BondDetail.BondSmallViewDetail");
    request.SetSubject("model");
    LOGGER_DEBUG("-----------------fetch bond remote--------------------");
    if (SendService::singleton()->call(kServiceQueueBondInfo, request, response, 30000)) {
        sdbus::Message unzip_msg;
        if (unzipMessage(response, unzip_msg)) {
            LOGGER_DEBUG("[unzip] type: " << unzip_msg.GetType());

            int new_version = -1;
            sdbus::VariantList list;
            unzip_msg.GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
            unzip_msg.GetList(FID_LIST, list);           
            LOGGER_DEBUG("[new_version]: " << new_version);
            LOGGER_DEBUG("[size]: " << list.Size());
            
            if (new_version >= 0) {     
                SetVersion(new_version);

                BondSmallViewCacheController bondsmallview_cc;
                bondsmallview_cc.clearCache();
                bondsmallview_cc.commit();

                for (unsigned int i = 0; i < list.Size(); ++i) {
                    const sdbus::VariantMap& mp = list.Get(i)->ToMap();

                    BondSmallViewCache cache;
                    SetBondSmallViewDetailCache(&mp, &cache);

                    bondsmallview_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
                }
                bondsmallview_cc.commit();
            }
        }
        LOGGER_DEBUG("------------------------------------------------")
        return true;
    }*/
	LOGGER_DEBUG("-----------------fetch bond remote--------------------");
	BondSmallViewCacheController bondsmallview_cc;
	bondsmallview_cc.clearCache();
	bondsmallview_cc.commit();

	int total_row_counts = 0;	//总记录数，每次请求都返回总记录数
	int loop = 0;				//标记分页次数，设置分页偏移
	int ncounts = 0;			//返回结果数集合
	do {
		std::string offset = IntToString(loop * kServiceSyncLimit);
		std::string range = offset + ", " + IntToString(kServiceSyncLimit);  //每次请求kServiceSyncLimit条数据
		sdbus::Message request, response;
		request.SetType("BondDetail.BondSmallViewDetail");
		request.SetSubject("model");
		request.SetString(FID_RANGE, range);
		request.SetString(FID_SORT_BY, "bond_key_listed_market");

		if (SendService::singleton()->call(kServiceQueueBondInfo, request, response, 30000)) {
			sdbus::Message unzip_msg;
			if (unzipMessage(response, unzip_msg)) {
				LOGGER_DEBUG("[unzip] type: " << unzip_msg.GetType());
				unzip_msg.GetInt32(FID_ROW_COUNT, total_row_counts);

				int new_version = -1;
				sdbus::VariantList list;
				unzip_msg.GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
				unzip_msg.GetList(FID_LIST, list);
				LOGGER_DEBUG("[new_version]: " << new_version);
				LOGGER_DEBUG("[size]: " << list.Size());

				ncounts += list.Size();

				if (new_version >= 0) {
					SetVersion(new_version);
					for (unsigned int i = 0; i < list.Size(); ++i) {
						const sdbus::VariantMap& mp = list.Get(i)->ToMap();

						BondSmallViewCache cache;
						SetBondSmallViewDetailCache(&mp, &cache);

						bondsmallview_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
					}
					bondsmallview_cc.commit();
				}
			}

			++loop;
			LOGGER_INFO("---Bond Requested Rows: " << ncounts);
			LOGGER_INFO("---Bond Total Rows: " << total_row_counts << "----Request times: " << loop);
		}

	} while (total_row_counts != 0 && total_row_counts > ncounts && ncounts >= loop * kServiceSyncLimit);

	if (total_row_counts == ncounts) {
		return true;
	} else {
		return false;
	}

    return false;
}

bool BondDataManager::FetchIncrementBondRemote(int version)
{    
    sdbus::Message request, response;
    request.SetType("BondDetail.BondSmallViewDetail");
    request.SetSubject("increment");
    request.SetInt32(FID_PUSH_MESSAGE_VERSION, version);
    LOGGER_DEBUG("----------------fetch inc bond remote------------------");
    if (SendService::singleton()->call(kServiceQueueBondInfo, request, response, 30000)) {
        sdbus::Message unzip_msg;
        if (unzipMessage(response, unzip_msg)) {
            LOGGER_DEBUG("[unzip] type: " << unzip_msg.GetType());

            int new_version = -1;
            sdbus::string action_type;
            unzip_msg.GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
            unzip_msg.GetString(FID_ACTION_TYPE, action_type);

            LOGGER_DEBUG("[new_version]: " << new_version);
            LOGGER_DEBUG("[action_type]: " << action_type);

            if (new_version >= 0) {
                SetVersion(new_version);

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

void BondDataManager::AddList(const sdbus::VariantList &list)
{    
    BondSmallViewCacheController bondsmallview_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        
		BondSmallViewCachePtr cache(new BondSmallViewCache);
        SetBondSmallViewDetailCache(&mp, cache.get());

        bondsmallview_cc.insertCacheInThreadSaftyWithoutDBSync(cache.get(), false);
    }
    bondsmallview_cc.commit();
}

void BondDataManager::UpdateList(const sdbus::VariantList &list)
{
    BondSmallViewCacheController bondsmallview_cc;
    std::vector<std::string> updated_bond_vec;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

		BondSmallViewCachePtr cache(new BondSmallViewCache);
        SetBondSmallViewDetailCache(&mp, cache.get());

        dbQuery q;
        q = "bond_key_listed_market = ", cache->bond_key_listed_market;
        bondsmallview_cc.updateCacheInThreadSaftyWithoutDBSync(q, cache.get(), false);

        updated_bond_vec.push_back(cache->bond_key_listed_market);
    }
    bondsmallview_cc.commit();

    if (updated_bond_vec.size() > 0) {
        BondQuoteSync bondquote_sync;
        bondquote_sync.syncForBondChanged(updated_bond_vec);

//        BondReferQuoteSync bondreferquote_sync;
//        bondreferquote_sync.syncForBondChanged(updated_bond_vec);

        BondBestQuoteSync bondbestquote_sync;
        bondbestquote_sync.syncForBondChanged(updated_bond_vec);

//        BondDealSync bonddeal_sync;
//        bonddeal_sync.syncForBondChanged(updated_bond_vec);
    }
}

void BondDataManager::DeleteList(const sdbus::VariantList &list)
{
    BondSmallViewCacheController bondsmallview_cc;

    for (unsigned int i = 0; i < list.Size(); ++i) {
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

		BondSmallViewCache cache;
        SetBondSmallViewDetailCache(&mp, &cache);
        
        dbQuery q;
		q = "bond_key_listed_market = ", cache.bond_key_listed_market;
        bondsmallview_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q, false);
    }
    bondsmallview_cc.commit();
}

int BondDataManager::GetVersion() {
    return versionBusiness.GetVersion(mBondDetail_BondSmallViewDetail, kCompanyTP);
}

void BondDataManager::SetVersion(const int& version) {
    versionBusiness.SetVersion(mBondDetail_BondSmallViewDetail, kCompanyTP, version);
}
