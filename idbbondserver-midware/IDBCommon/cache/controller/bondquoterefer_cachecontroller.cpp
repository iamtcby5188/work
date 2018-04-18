/*
 * BondQuoteReferCacheController.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#include "cache/cache_center.h"
#include "bondquoterefer_cachecontroller.h"

BondQuoteReferCacheController::BondQuoteReferCacheController() {
    // TODO Auto-generated constructor stub
}

BondQuoteReferCacheController::~BondQuoteReferCacheController() {
    // TODO Auto-generated destructor stub
}

void BondQuoteReferCacheController::insertCache(sql::ResultSet* resSet)
{
    SET_PROPERTIES_BEGIN(BondQuoteReferCache);
    
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
