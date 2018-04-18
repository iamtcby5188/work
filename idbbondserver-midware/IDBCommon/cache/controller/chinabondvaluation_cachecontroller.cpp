/*
 * ChinaBondValuationCacheController.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "cache/cache_center.h"
#include "chinabondvaluation_cachecontroller.h"
#include "fid.h"

ChinaBondValuationCacheController::ChinaBondValuationCacheController() {
    // TODO Auto-generated constructor stub
}

ChinaBondValuationCacheController::~ChinaBondValuationCacheController() {
	// TODO Auto-generated destructor stub
}

static std::unordered_map<std::string, ChinaBondValuationCachePtr> chinaBondValuationCacheMap; // key: ${bond_key_listed_market}_${valuation_date}, value: ChinaBondValuationCachePtr
static ChinaBondValuationCacheVecPtr tmp_china_bond_valuation_cache_vec;
void ChinaBondValuationCacheController::memCacheInit() {
	LOGGER_DEBUG("chinaBondValuationCacheMap init mem cache ...");
	memCacheClear();
	dbQuery q;
	q = "order by valuation_date, bond_key_listed_market, listed_market";
	ChinaBondValuationCacheController china_bond_valuation_cc;
	tmp_china_bond_valuation_cache_vec = china_bond_valuation_cc.getCacheListByQueryInThreadSafty(q);
	if (!tmp_china_bond_valuation_cache_vec->empty()) {
		for (std::vector<ChinaBondValuationCachePtr>::iterator it = tmp_china_bond_valuation_cache_vec->begin(); it != tmp_china_bond_valuation_cache_vec->end(); ++it) {
			std::string key = (*it)->bond_key_listed_market + "_" + (*it)->valuation_date;
			std::unordered_map<std::string, ChinaBondValuationCachePtr>::iterator iter = chinaBondValuationCacheMap.find(key);
			if (iter == chinaBondValuationCacheMap.end()) { // only add one record
				chinaBondValuationCacheMap[key] = *it;
			}
		}
	}
	LOGGER_DEBUG("chinaBondValuationCacheMap init mem cache success. size:" << chinaBondValuationCacheMap.size());
}

void ChinaBondValuationCacheController::memCacheClear() {
	//tmp_china_bond_valuation_cache_list->clear();
	chinaBondValuationCacheMap.clear();
}

ChinaBondValuationCachePtr ChinaBondValuationCacheController::getMemValuation(const std::string& bond_key_listed_market) {
	std::string datetime = GetTDateString("%Y%m%d");
	ChinaBondValuationCachePtr cache(new ChinaBondValuationCache());
	if (bond_key_listed_market.empty())
		return cache;
	if (useCacheInMem && !chinaBondValuationCacheMap.empty()) {
		std::string key = bond_key_listed_market + "_" + datetime;
		std::unordered_map<std::string, ChinaBondValuationCachePtr>::const_iterator iter = chinaBondValuationCacheMap.find(key);
		if (iter != chinaBondValuationCacheMap.end()) {
			cache = chinaBondValuationCacheMap[key];
		}
	} else {
		dbQuery q;
		q = "bond_key_listed_market = ", bond_key_listed_market, " and valuation_date = ", datetime, " order by valuation_date desc limit 1";
		ChinaBondValuationCacheController chinaBondValuationCC;
		cache = chinaBondValuationCC.getCacheByQueryInThreadSafty(q);
	}
	return cache;
}

ChinaBondValuationCachePtr ChinaBondValuationCacheController::getValuation(const std::string& bond_id)
{
	std::string datetime = GetTDateString("%Y%m%d");
	dbQuery q;
	q = "bond_id = ", bond_id, " and valuation_date = ", datetime, " order by valuation_date desc limit 1";
	return getCacheByQueryInThreadSafty(q);
}
