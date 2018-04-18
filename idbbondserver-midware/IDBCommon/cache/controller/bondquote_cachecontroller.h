/*
 * BondQuoteCacheController.h
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#ifndef BONDQUOTE_CACHECONTROLLER_H_
#define BONDQUOTE_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/bondquote_cache.h"

class BondQuoteCacheController : public CacheController<BondQuoteCache> {
public:
    BondQuoteCacheController();
    virtual ~BondQuoteCacheController();

private:
	void insertCache(sql::ResultSet* resSet);

};



#endif /* BONDQUOTE_CACHECONTROLLER_H_ */
