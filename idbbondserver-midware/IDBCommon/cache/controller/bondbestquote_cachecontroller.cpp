/*
 * BondBestQuoteCacheController.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#include "cache/cache_center.h"
#include "bondbestquote_cachecontroller.h"

BondBestQuoteCacheController::BondBestQuoteCacheController() {
	// TODO Auto-generated constructor stub

}

BondBestQuoteCacheController::~BondBestQuoteCacheController() {
	// TODO Auto-generated destructor stub
}

void BondBestQuoteCacheController::insertCache(sql::ResultSet* resSet)
{
	SET_PROPERTIES_BEGIN(BondBestQuoteCache);

    // Set customerized properties
    //CacheCenter::sharedInstance()->AttachCurrentThread();
    cache.loadCombinationFields();
    //CacheCenter::sharedInstance()->DetachCurrentThread();

    if (cache.isValidBond) {
        SET_PROPERTIES_END;
    }
}

