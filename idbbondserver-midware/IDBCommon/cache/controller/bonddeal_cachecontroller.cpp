/*
 * BondDealCacheController.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#include "cache/cache_center.h"
#include "bonddeal_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/model/system_company_cache.h"
#include "business/SystemCompanyBusiness.h"
BondDealCacheController::BondDealCacheController() {
    // TODO Auto-generated constructor stub
}

BondDealCacheController::~BondDealCacheController() {
    // TODO Auto-generated destructor stub
}

static std::unordered_map<std::string, BondDealCachePtr> bondDealCacheMap; // key: ${bond_key_listed_market}, value: BondDealCachePtr latest one
static BondDealCacheVecPtr tmp_bond_deal_cache_vec;
void BondDealCacheController::memCacheInit() {
	LOGGER_DEBUG("bondDealCacheMap init mem cache ...");
	memCacheClear();
	SystemCompanyBusiness sc;
	SystemCompanyCacheVecPtr scv = sc.getSysCompany();
	for (std::vector<SystemCompanyCachePtr>::iterator it = scv->begin(); it != scv->end(); ++it) {
		const std::string& company_id = (*it)->id;
		dbQuery q;
		q = "company_id = ", company_id, " order by bond_key_listed_market, create_time desc";
		BondDealCacheController bond_deal_cc;
		tmp_bond_deal_cache_vec = bond_deal_cc.getCacheListByQueryInThreadSafty(q);
		if (!tmp_bond_deal_cache_vec->empty()) {
			for (std::vector<BondDealCachePtr>::iterator it = tmp_bond_deal_cache_vec->begin(); it != tmp_bond_deal_cache_vec->end(); ++it) {
				std::string key = (*it)->bond_key_listed_market + "|" + company_id;
				std::unordered_map<std::string, BondDealCachePtr>::iterator iter = bondDealCacheMap.find(key);
				if (iter == bondDealCacheMap.end()) { // only add one record
					bondDealCacheMap[key] = *it;
				}
			}
		}
	}	
	LOGGER_DEBUG("bondDealCacheMap init mem cache success. size:" << bondDealCacheMap.size());
}

void BondDealCacheController::memCacheClear() {
	//tmp_bond_deal_cache_list->clear();
	bondDealCacheMap.clear();
}

BondDealCachePtr BondDealCacheController::getMemDeal(const std::string& bond_key_listed_market, const std::string company_id) {
	BondDealCachePtr cache(new BondDealCache());
	if (bond_key_listed_market.empty() || company_id.empty())
		return cache;
	std::string datetime = GetTDateString("%Y%m%d");
	if (useCacheInMem && !bondDealCacheMap.empty()) {
		std::string key = bond_key_listed_market + "|" + company_id;
		std::unordered_map<std::string, BondDealCachePtr>::const_iterator iter = bondDealCacheMap.find(key);
		if (iter != bondDealCacheMap.end()) {
			cache = bondDealCacheMap[key];
		}
	} else {
		dbQuery q;
		q = GetFastdbQueryByBondKeyListedMarket(bond_key_listed_market, company_id) + " order by create_time desc";
		BondDealCacheController bondDealCC;
		cache = bondDealCC.getCacheByQueryInThreadSafty(q);
	}
	return cache;
}

void BondDealCacheController::insertCache(sql::ResultSet* resSet)
{
    SET_PROPERTIES_BEGIN(BondDealCache);

	cache.create_time_milli_sec = ParseTimeString(cache.create_time) * 1000;
    cache.deal_time_milli_sec = ParseTimeString(cache.deal_time) * 1000;

    // Set customerized properties
	//CacheCenter::sharedInstance()->AttachCurrentThread();
    cache.loadCombinationFields();
	//CacheCenter::sharedInstance()->DetachCurrentThread();
    SET_PROPERTIES_END;
}

