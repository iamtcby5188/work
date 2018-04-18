#include "valuation_data_manager.h"
#include "constants.h"
#include "fid.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "cache/model/chinabondvaluation_cache.h"
#include "service/send_service.h"
#include <boost/function/function0.hpp>
#include "service/service_manager.h"

void SetBondValuationCache(const sdbus::VariantMap *mp, ChinaBondValuationCache *cache)
{
    mp->GetString(FID_ID, cache->id);
    mp->GetString(FID_BOND_CODE, cache->bond_id);
    mp->GetString(FID_CDC_VALUATIONS_DATE, cache->valuation_date);
    mp->GetString(FID_CDC_VALUATIONS_YIELD, cache->valuation_yield);
    mp->GetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->valuation_net_price);
    mp->GetString(FID_DEL_FLAG, cache->status);
}

int ValuationDataManager::version_ = -1;

ValuationDataManager::ValuationDataManager(sdbus::Connection *conn)
    : conn_(conn)
{

}

ValuationDataManager::~ValuationDataManager()
{
    thrd.interrupt();
    thrd.join();
    conn_->DelHandler(this);
}

bool ValuationDataManager::init()
{    
    if (!Init(kTopicBondInfo, sdbus::DeliverPublish)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&ValuationDataManager::threadSubMsg, this);
    thrd = boost::thread(f); 

    return true;
}

void ValuationDataManager::OnMsgs(const sdbus::Message** msgs, int count)
{
    for (int i = 0; i < count; ++i) {
        const sdbus::Message * msg = *(msgs + i);

        if ("Bond.BondValuation" != msg->GetType()) {
            continue;
        }

        LOGGER_DEBUG("-----------------get a publish--------------------")
        LOGGER_DEBUG("[type]: " << msg->GetType())

        boost::mutex::scoped_lock lock(sub_queue_mutex_);
        sub_msg_queue_.push(*msg);
        sub_queue_cond_var_.notify_one();
    }
}

void ValuationDataManager::threadSubMsg()
{
    LOGGER_DEBUG("start thread to process subscribed msg in ValuationDataManager" )

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

void ValuationDataManager::processSubMsg(const sdbus::Message* msg)
{
    int new_version = -1;
    sdbus::string action_type;
    msg->GetInt32(FID_PUSH_MESSAGE_VERSION, new_version);
    msg->GetString(FID_ACTION_TYPE, action_type);

    LOGGER_DEBUG("-----------------process sub msg--------------------")
    LOGGER_DEBUG("[old_version]: " << version_)
    LOGGER_DEBUG("[new_version]: " << new_version)
    LOGGER_DEBUG("[action_type]: " << action_type)

    if (new_version == -1) {
        return;
    }

    if ("Sync" == action_type) {
        FetchValuationRemote();
    }
}

bool ValuationDataManager::FetchValuationRemote()
{    
    sdbus::Message request, response;
    request.SetType("Bond.BondValuation");
    request.SetSubject("model");
    LOGGER_DEBUG("-----------------fetch valuation remote--------------------")
    if (SendService::singleton()->call(kServiceQueueBondInfo, request, response, 90000)) {
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
                version_ = new_version;

                ChinaBondValuationCacheController valuation_cc;
                valuation_cc.clearCache();
                valuation_cc.commit();

                for (unsigned int i = 0; i < list.Size(); ++i) {
                    const sdbus::VariantMap& mp = list.Get(i)->ToMap();

                    ChinaBondValuationCache cache;
                    SetBondValuationCache(&mp, &cache);

                    valuation_cc.insertCacheInThreadSaftyWithoutDBSync(&cache, false);
                }
                valuation_cc.commit();
            }
        }
        LOGGER_DEBUG("------------------------------------------------")
        return true;
    }  

    return false;
}

void ValuationDataManager::FetchValuationLocal(BondQuoteCache& cache){
	if(cache.bond_id.empty()){
		return;
	}
	cache.cdc_valuation_yield = "";
	cache.cdc_valuation_net_price = "";
	cache.cdc_offset = DOUBLE_NULL;
	ChinaBondValuationCacheController cbvcc;
	ChinaBondValuationCachePtr cc = cbvcc.getValuation(cache.bond_id);

	if(cc.get() == NULL || cc.get()->id.empty()){
		return;
	}
	cache.cdc_valuation_yield = cc->valuation_yield;
	cache.cdc_valuation_net_price = cc->valuation_net_price;
	if(cache.symbol == kOfrQuote && cache.cdc_valuation_yield.length() > 0){ // ÖÐÕ®Æ«ÒÆÖ»¿¼ÂÇofr
		double tCdcValuationYield = DOUBLE_NULL;
		if("Y" == cache.has_option && cache.cdc_valuation_yield.find("|") != std::string::npos){
			tCdcValuationYield = atof(cache.cdc_valuation_yield.substr(0, cache.cdc_valuation_yield.find_first_of("|")).c_str());
		}else{
			tCdcValuationYield = atof(cache.cdc_valuation_yield.c_str());
		}
		if(cache.yield != DOUBLE_NULL){
			cache.cdc_offset = (cache.yield - tCdcValuationYield) * 100;
		}else if(cache.price != DOUBLE_NULL && cache.quote_type == "3"){
			cache.cdc_offset = (cache.price - tCdcValuationYield) * 100;
		}
	}
}

void ValuationDataManager::FetchValuationLocal(BondQuoteReferCache& cache){
	if(cache.bond_id.empty()){
		return;
	}
	cache.cdc_valuation_yield = "";
	cache.cdc_valuation_net_price = "";
	cache.cdc_offset = DOUBLE_NULL;
	ChinaBondValuationCacheController cbvcc;
	ChinaBondValuationCachePtr cc = cbvcc.getValuation(cache.bond_id);

	if(cc.get() == NULL || cc.get()->id.empty()){
		return;
	}
	cache.cdc_valuation_yield = cc->valuation_yield;
	cache.cdc_valuation_net_price = cc->valuation_net_price;
	if(cache.symbol == kOfrQuote && cache.cdc_valuation_yield.length() > 0){ // ÖÐÕ®Æ«ÒÆÖ»¿¼ÂÇofr
		double tCdcValuationYield = DOUBLE_NULL;
		if("Y" == cache.has_option && cache.cdc_valuation_yield.find("|") != std::string::npos){
			tCdcValuationYield = atof(cache.cdc_valuation_yield.substr(0, cache.cdc_valuation_yield.find_first_of("|")).c_str());
		}else{
			tCdcValuationYield = atof(cache.cdc_valuation_yield.c_str());
		}
		if(cache.yield != DOUBLE_NULL){
			cache.cdc_offset = (cache.yield - tCdcValuationYield) * 100;
		}else if(cache.price != DOUBLE_NULL && cache.quote_type == "3"){
			cache.cdc_offset = (cache.price - tCdcValuationYield) * 100;
		}
	}
}

void ValuationDataManager::FetchValuationLocal(BondBestQuoteCache & cache){
	if(cache.bond_id.empty()){
		return;
	}
	cache.cdc_valuation_yield = "";
	cache.cdc_valuation_net_price = "";
	cache.cdc_offset = DOUBLE_NULL;
	ChinaBondValuationCacheController cbvcc;
	ChinaBondValuationCachePtr cc = cbvcc.getValuation(cache.bond_id);

	if(cc.get() == NULL || cc.get()->id.empty()){
		return;
	}
	cache.cdc_valuation_yield = cc->valuation_yield;
	cache.cdc_valuation_net_price = cc->valuation_net_price;
	if(cache.cdc_valuation_yield.length() > 0){ // ÖÐÕ®Æ«ÒÆÖ»¿¼ÂÇofr
		double tCdcValuationYield = DOUBLE_NULL;
		if("Y" == cache.has_option && cache.cdc_valuation_yield.find("|") != std::string::npos){
			tCdcValuationYield = atof(cache.cdc_valuation_yield.substr(0, cache.cdc_valuation_yield.find_first_of("|")).c_str());
		}else{
			tCdcValuationYield = atof(cache.cdc_valuation_yield.c_str());
		}
		if(cache.ofr_yield != DOUBLE_NULL){
			cache.cdc_offset = (cache.ofr_yield - tCdcValuationYield) * 100;
		}else if(cache.ofr_price != DOUBLE_NULL && cache.ofr_quote_type == "3"){
			cache.cdc_offset = (cache.ofr_price - tCdcValuationYield) * 100;
		}
	}
}

void ValuationDataManager::FetchValuationLocal(BondDealCache & cache){
	if (cache.bond_id.empty()){
		return;
	}
	cache.cdc_valuation_yield = "";
	cache.cdc_valuation_net_price = "";
	ChinaBondValuationCacheController cbvcc;
	ChinaBondValuationCachePtr cc = cbvcc.getValuation(cache.bond_id);

	if (cc.get() == NULL || cc.get()->id.empty()){
		return;
	}
	cache.cdc_valuation_yield = cc->valuation_yield;
	cache.cdc_valuation_net_price = cc->valuation_net_price;
}

void ValuationDataManager::FetchValuationLocal(DcsBondDealCache & cache){
    if (cache.bond_id.empty()){
        return;
    }
    cache.cdc_valuation_yield = "";
    cache.cdc_valuation_net_price = "";
    ChinaBondValuationCacheController cbvcc;
    ChinaBondValuationCachePtr cc = cbvcc.getValuation(cache.bond_id);

    if (cc.get() == NULL || cc.get()->id.empty()){
        return;
    }
    cache.cdc_valuation_yield = cc->valuation_yield;
    cache.cdc_valuation_net_price = cc->valuation_net_price;
}