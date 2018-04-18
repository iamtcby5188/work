#include "chinabondvaluation_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "monitor/chinabondvaluation_monitor.h"
#include "service/service_manager.h"

ChinaBondValuationSync::ChinaBondValuationSync()
{

}

ChinaBondValuationSync::~ChinaBondValuationSync()
{

}

void ChinaBondValuationSync::sync()
{
    ChinaBondValuationCacheController chinabondvaluation_controller;
    ChinaBondValuationMonitor monitor;

    monitor.init("FetchChinaBondValuationSQL", ChinaBondValuationCache::SCHEMA_NAME);

    std::vector<ChinaBondValuationCache*> update_vec;
    std::vector<ChinaBondValuationCache*> add_vec;
    std::vector<ChinaBondValuationCache*> delete_vec;
    
	dbQuery q;
	q = "";
    ChinaBondValuationCacheVecPtr vec = chinabondvaluation_controller.getCacheListByQueryInThreadSafty(q);

    valuation_sync<ChinaBondValuationCacheController, ChinaBondValuationMonitor, ChinaBondValuationCache, ChinaBondValuationCachePtr, ChinaBondValuationCacheVecPtr>(
        chinabondvaluation_controller, monitor, "id", vec, update_vec, add_vec, delete_vec);
    
	LOGGER_INFO("cdc value changed. update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
	if ((update_vec.size() > 0) || (add_vec.size() > 0) || (delete_vec.size() > 0))
	{
		chinabondvaluation_controller.cacheTable();
		chinabondvaluation_controller.commit();
	}

	if(add_vec.size() > 0 || update_vec.size() > 0){
		LOGGER_INFO("set cdc value from add_vec, add_vec.size:" << add_vec.size() << ", update_vec.size:" << update_vec.size());
		TIME_COST_BEGIN("");
		std::unordered_map<std::string, ChinaBondValuationCache*> cache;
		for(size_t i = 0; i < add_vec.size(); ++i){
			cache[add_vec.at(i)->bond_id] = add_vec.at(i);
		}
		for(size_t i = 0; i < update_vec.size(); ++i){
			cache[update_vec.at(i)->bond_id] = update_vec.at(i);
		}
		TIME_COST_END("build cdc value map cache. cache.size:" << cache.size());
		updateBondCache(cache, 0);
	}
	if(delete_vec.size() > 0){
		LOGGER_INFO("set cdc value from delete_vec, delete_vec.size:" << delete_vec.size());
		TIME_COST_BEGIN("");
		std::unordered_map<std::string, ChinaBondValuationCache*> cache;
		for(size_t i = 0; i < delete_vec.size(); ++i){
			cache[delete_vec.at(i)->bond_id] = delete_vec.at(i);
		}
		TIME_COST_END("build cdc value map cache. cache.size:" << cache.size());
		updateBondCache(cache, 1);
	}
}

void ChinaBondValuationSync::updateBondCache(const std::unordered_map<std::string, ChinaBondValuationCache*> &cache, const int& updateType){
	if(cache.empty()){
		return;
	}
	TIME_COST_BEGIN("");
	BondQuoteCacheController quoteCC;
	BondQuoteReferCacheController quoteReferCC;
	BondBestQuoteCacheController bestQuoteCC;
	BondDealCacheController dealCC;
	dbQuery q;
	q = "bond_id != ", "";
	if(updateType == 1){
		quoteCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, ClearBondQuoteCallBack, (void*)&cache, NULL, NULL);
		dealCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, ClearBondDealCallBack, (void*)&cache, NULL, NULL);
		bestQuoteCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, ClearBondBestQuoteCallBack, (void*)&cache, NULL, NULL);
		quoteReferCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, ClearBondQuoteReferCallBack, (void*)&cache, NULL, NULL);
	}else{
		quoteCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, UpdateBondQuoteCallBack, (void*)&cache, NULL, NULL);
		dealCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, UpdateBondDealCallBack, (void*)&cache, NULL, NULL);
		bestQuoteCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, UpdateBondBestQuoteCallBack, (void*)&cache, NULL, NULL);
		quoteReferCC.updateCacheByQueryInThreadSaftyWithoutDBSync(q, UpdateBondQuoteReferCallBack, (void*)&cache, NULL, NULL);
	}
	TIME_COST_END("set cdc value from map success, cache.size: " << cache.size());
}

void ChinaBondValuationSync::UpdateBondQuoteCallBack(BondQuoteCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->cdc_offset = DOUBLE_NULL;
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
	ChinaBondValuationCache* cc = map->at(cache->bond_id);
	if(cc == NULL){
		return;
	}
	cache->cdc_valuation_yield = cc->valuation_yield;
	cache->cdc_valuation_net_price = cc->valuation_net_price;
	//cache->val_modified_duration = cc->val_modified_duration;
    if (fabs(cc->val_modified_duration - 0) <= FLOATING_PRECISION) {
        cache->val_modified_duration = DOUBLE_NULL;
    } else {
        cache->val_modified_duration = cc->val_modified_duration;
    }
	cache->val_convexity = cc->val_convexity;
	cache->val_basis_point_value = cc->val_basis_point_value;
	if(cache->symbol == kOfrQuote && cache->cdc_valuation_yield.length() > 0){ // ÖÐÕ®Æ«ÒÆÖ»¿¼ÂÇofr
		double tCdcValuationYield = DOUBLE_NULL;
		if("Y" == cache->has_option && cache->cdc_valuation_yield.find("|") != std::string::npos){
			tCdcValuationYield = atof(cache->cdc_valuation_yield.substr(0, cache->cdc_valuation_yield.find_first_of("|")).c_str());
		}else{
			tCdcValuationYield = atof(cache->cdc_valuation_yield.c_str());
		}
		if(cache->yield != DOUBLE_NULL){
			cache->cdc_offset = (cache->yield - tCdcValuationYield) * 100;
		}else if(cache->price != DOUBLE_NULL && cache->quote_type == "3"){
			cache->cdc_offset = (cache->price - tCdcValuationYield) * 100;
		}
	}
}

void ChinaBondValuationSync::UpdateBondQuoteReferCallBack(BondQuoteReferCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->cdc_offset = DOUBLE_NULL;
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
	ChinaBondValuationCache* cc = map->at(cache->bond_id);
	if(cc == NULL){
		return;
	}
	cache->cdc_valuation_yield = cc->valuation_yield;
	cache->cdc_valuation_net_price = cc->valuation_net_price;
	//cache->val_modified_duration = cc->val_modified_duration;
    if (fabs(cc->val_modified_duration - 0) <= FLOATING_PRECISION) {
        cache->val_modified_duration = DOUBLE_NULL;
    } else {
        cache->val_modified_duration = cc->val_modified_duration;
    }
	cache->val_convexity = cc->val_convexity;
	cache->val_basis_point_value = cc->val_basis_point_value;
	if(cache->symbol == kOfrQuote && cache->cdc_valuation_yield.length() > 0){ // ÖÐÕ®Æ«ÒÆÖ»¿¼ÂÇofr
		double tCdcValuationYield = DOUBLE_NULL;
		if("Y" == cache->has_option && cache->cdc_valuation_yield.find("|") != std::string::npos){
			tCdcValuationYield = atof(cache->cdc_valuation_yield.substr(0, cache->cdc_valuation_yield.find_first_of("|")).c_str());
		}else{
			tCdcValuationYield = atof(cache->cdc_valuation_yield.c_str());
		}
		if(cache->yield != DOUBLE_NULL){
			cache->cdc_offset = (cache->yield - tCdcValuationYield) * 100;
		}else if(cache->price != DOUBLE_NULL && cache->quote_type == "3"){
			cache->cdc_offset = (cache->price - tCdcValuationYield) * 100;
		}
	}
}

void ChinaBondValuationSync::UpdateBondBestQuoteCallBack(BondBestQuoteCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->cdc_offset = DOUBLE_NULL;
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
	ChinaBondValuationCache* cc = map->at(cache->bond_id);
	if(cc == NULL){
		return;
	}
	cache->cdc_valuation_yield = cc->valuation_yield;
	cache->cdc_valuation_net_price = cc->valuation_net_price;
	//cache->val_modified_duration = cc->val_modified_duration;
    if (fabs(cc->val_modified_duration - 0) <= FLOATING_PRECISION) {
        cache->val_modified_duration = DOUBLE_NULL;
    } else {
        cache->val_modified_duration = cc->val_modified_duration;
    }
	cache->val_convexity = cc->val_convexity;
	cache->val_basis_point_value = cc->val_basis_point_value;
	if(cache->cdc_valuation_yield.length() > 0){ // ÖÐÕ®Æ«ÒÆÖ»¿¼ÂÇofr
		double tCdcValuationYield = DOUBLE_NULL;
		if("Y" == cache->has_option && cache->cdc_valuation_yield.find("|") != std::string::npos){
			tCdcValuationYield = atof(cache->cdc_valuation_yield.substr(0, cache->cdc_valuation_yield.find_first_of("|")).c_str());
		}else{
			tCdcValuationYield = atof(cache->cdc_valuation_yield.c_str());
		}
		if(cache->ofr_yield != DOUBLE_NULL){
			cache->cdc_offset = (cache->ofr_yield - tCdcValuationYield) * 100;
		}else if(cache->ofr_price != DOUBLE_NULL && cache->ofr_quote_type == "3"){
			cache->cdc_offset = (cache->ofr_price - tCdcValuationYield) * 100;
		}
	}
}

void ChinaBondValuationSync::UpdateBondDealCallBack(BondDealCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
	ChinaBondValuationCache* cc = map->at(cache->bond_id);
	if(cc == NULL){
		return;
	}
	cache->cdc_valuation_yield = cc->valuation_yield;
	cache->cdc_valuation_net_price = cc->valuation_net_price;
	//cache->val_modified_duration = cc->val_modified_duration;
    if (fabs(cc->val_modified_duration - 0) <= FLOATING_PRECISION) {
        cache->val_modified_duration = DOUBLE_NULL;
    } else {
        cache->val_modified_duration = cc->val_modified_duration;
    }
	cache->val_convexity = cc->val_convexity;
	cache->val_basis_point_value = cc->val_basis_point_value;
}

void ChinaBondValuationSync::ClearBondQuoteCallBack(BondQuoteCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->cdc_offset = DOUBLE_NULL;
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
}

void ChinaBondValuationSync::ClearBondQuoteReferCallBack(BondQuoteReferCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->cdc_offset = DOUBLE_NULL;
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
}

void ChinaBondValuationSync::ClearBondBestQuoteCallBack(BondBestQuoteCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->cdc_offset = DOUBLE_NULL;
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
}

void ChinaBondValuationSync::ClearBondDealCallBack(BondDealCache* cache, void* param){
	std::unordered_map<std::string, ChinaBondValuationCache*>* map = (std::unordered_map<std::string, ChinaBondValuationCache*>*)param;
	if(map == NULL || map->empty() || cache == NULL || cache->bond_id.empty() || map->find(cache->bond_id) == map->end()){
		return;
	}
	cache->cdc_valuation_yield = "";
	cache->cdc_valuation_net_price = "";
	cache->val_modified_duration = DOUBLE_NULL;
	cache->val_convexity = DOUBLE_NULL;
	cache->val_basis_point_value = DOUBLE_NULL;
}