/*
 * brokerbindingtrader_cachecontroller.cpp
 *
 *  Created on: September 22, 2014
 *      Author: yudan.chen
 */

#include "cache_def.h"
#include "cache/cache_center.h"
#include "brokerbindingtrader_cachecontroller.h"


BrokerBindingTraderCacheController::BrokerBindingTraderCacheController() {
    // TODO Auto-generated constructor stub
}

BrokerBindingTraderCacheController::~BrokerBindingTraderCacheController() {
    // TODO Auto-generated destructor stub
}

void BrokerBindingTraderCacheController::insertCache(sql::ResultSet* resSet)
{
    SET_PROPERTIES_BEGIN(BrokerBindingTraderCache);

	//CacheCenter::sharedInstance()->AttachCurrentThread();
	cache.loadCombinationFields();
	//CacheCenter::sharedInstance()->DetachCurrentThread();

    SET_PROPERTIES_END;
}
