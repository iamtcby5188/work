/*
 * BondQuoteReferCacheController.h
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#ifndef BONDOFFERREFER_CACHECONTROLLER_H_
#define BONDOFFERREFER_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/bondquoterefer_cache.h"

class BondQuoteReferCacheController : public CacheController<BondQuoteReferCache> {
public:
    BondQuoteReferCacheController();
    virtual ~BondQuoteReferCacheController();

private:
	void insertCache(sql::ResultSet* resSet);

};


#endif /* BONDOFFERREFER_CACHECONTROLLER_H_ */
