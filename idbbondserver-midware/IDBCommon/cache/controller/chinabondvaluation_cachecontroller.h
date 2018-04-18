/*
 * ChinaBondValuationCacheController.h
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#ifndef CHINABONDVALUATION_CACHECONTROLLER_H_
#define CHINABONDVALUATION_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/chinabondvaluation_cache.h"

class ChinaBondValuationCacheController : public CacheController<ChinaBondValuationCache> {
public:
    ChinaBondValuationCacheController();
    virtual ~ChinaBondValuationCacheController();

	ChinaBondValuationCachePtr getValuation(const std::string& bond_id);

	static void memCacheInit();
	static void memCacheClear();
	static ChinaBondValuationCachePtr getMemValuation(const std::string& bond_key_listed_market);
};

#endif /* CHINABONDVALUATION_CACHECONTROLLER_H_ */
