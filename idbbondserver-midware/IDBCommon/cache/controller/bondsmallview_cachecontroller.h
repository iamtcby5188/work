/*
 * BondSmallViewCacheController.h
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#ifndef BONDSMALLVIEW_CACHECONTROLLER_H_
#define BONDSMALLVIEW_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/bondsmallview_cache.h"

class BondSmallViewCacheController : public CacheController<BondSmallViewCache> {
public:
	BondSmallViewCacheController();
	virtual ~BondSmallViewCacheController();

    void loadAcrossMarketBondIds();
    void loadAcrossMarketBondIds(BondSmallViewCache *cache);

	void loadIssuerRating(); //Add by Young 20-Apr-2016
    void loadIssuerRating(BondSmallViewCache *cache); //Add by Young 20-Apr-2016

private:
	void insertCache(sql::ResultSet* resSet);

};

#endif /* BONDSMALLVIEW_CACHECONTROLLER_H_ */
