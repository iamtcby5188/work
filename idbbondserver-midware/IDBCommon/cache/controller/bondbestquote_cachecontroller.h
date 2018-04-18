/*
 * BondBestQuoteCacheController.h
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#ifndef BONDBESTQUOTE_CACHECONTROLLER_H_
#define BONDBESTQUOTE_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/bondbestquote_cache.h"

class BondBestQuoteCacheController : public CacheController<BondBestQuoteCache> {
public:
    BondBestQuoteCacheController();
    virtual ~BondBestQuoteCacheController();

private:
	void insertCache(sql::ResultSet* resSet);

};


#endif /* BONDBESTQUOTECACHECONTROLLER_H_ */
