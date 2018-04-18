/*
 * BondQuoteCacheController.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#include "cache/cache_center.h"
#include "bondquote_cachecontroller.h"

BondQuoteCacheController::BondQuoteCacheController() {
    // TODO Auto-generated constructor stub
}

BondQuoteCacheController::~BondQuoteCacheController() {
    // TODO Auto-generated destructor stub
}

void BondQuoteCacheController::insertCache(sql::ResultSet* resSet)
{
    SET_PROPERTIES_BEGIN(BondQuoteCache);
	
    cache.create_time_milli_sec = ParseTimeString(cache.create_time) * 1000;
    cache.deal_time_milli_sec = ParseTimeString(cache.deal_time) * 1000;

    // Set customerized properties
    //CacheCenter::sharedInstance()->AttachCurrentThread();
    cache.loadCombinationFields();
    //CacheCenter::sharedInstance()->DetachCurrentThread();

    if (cache.isValidBond) {
        SET_PROPERTIES_END;
    }
}
