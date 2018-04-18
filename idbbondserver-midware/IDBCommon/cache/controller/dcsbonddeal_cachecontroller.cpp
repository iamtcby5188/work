/*
 * DcsBondDealCacheController.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#include "cache/cache_center.h"
#include "dcsbonddeal_cachecontroller.h"

DcsBondDealCacheController::DcsBondDealCacheController() {
    // TODO Auto-generated constructor stub
}

DcsBondDealCacheController::~DcsBondDealCacheController() {
    // TODO Auto-generated destructor stub
}

void DcsBondDealCacheController::clearCache()
{
	try {
		_clearCache();
	}
	catch (const dbException& e) {
		LOGGER_ERROR("DcsBondDealCacheController::clearCache(): " << e.getMsg());
	}
}

void DcsBondDealCacheController::cacheTable(DcsBondDealCacheVecPtr& deal_vec)
{
	LOGGER_INFO("++++++ DcsBondDealCacheController::cacheTable()");
	LOGGER_INFO("cache... " << getFastdbTableName() << " - Result Set: " << deal_vec->size());

	try {
		std::vector<DcsBondDealCachePtr>::const_iterator it = deal_vec->begin();
		for (; it != deal_vec->end(); ++it)
		{
			DcsBondDealCache& cache = *(it->get());
			//CacheCenter::sharedInstance()->AttachCurrentThread();
			insertCache(cache);
			//CacheCenter::sharedInstance()->DetachCurrentThread();
		}

		LOGGER_INFO("Successfully Cached ... " << getFastdbTableName());

	}catch (const dbException& e) {
		LOGGER_ERROR("DcsBondDealCacheController::cacheTable(): " << e.getMsg());
	}
}

void DcsBondDealCacheController::insertCache(DcsBondDealCache& cache)
{
    cache.create_time_milli_sec = ParseTimeString(cache.create_time) * 1000;
    cache.deal_time_milli_sec = ParseTimeString(cache.deal_time) * 1000;

    // Set customerized properties
    //CacheCenter::sharedInstance()->AttachCurrentThread();
    cache.loadCombinationFields();
    //CacheCenter::sharedInstance()->DetachCurrentThread();

    SET_PROPERTIES_END;
}

