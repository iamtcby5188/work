/*
 * BondDealCacheController.h
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#ifndef BONDDEAL_CACHECONTROLLER_H_
#define BONDDEAL_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/bonddeal_cache.h"

class BondDealCacheController : public CacheController<BondDealCache> {
public:
    BondDealCacheController();
    virtual ~BondDealCacheController();

	static void memCacheInit();
	static void memCacheClear();
	static BondDealCachePtr getMemDeal(const std::string& bond_key_listed_market, const std::string company_id);

private:
	void insertCache(sql::ResultSet* resSet);

};

#endif /* BONDDEAL_CACHECONTROLLER_H_ */
