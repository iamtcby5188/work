#include "chinabondvaluation_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "monitor/chinabondvaluation_monitor.h"
#include "service/service_manager.h"

ChinaBondValuationSync::ChinaBondValuationSync()
{

}

ChinaBondValuationSync::~ChinaBondValuationSync()
{

}

void ChinaBondValuationSync::sync()
{
    ChinaBondValuationCacheController chinabondvaluation_controller;
    ChinaBondValuationMonitor monitor;

    monitor.init("FetchChinaBondValuationSQL", ChinaBondValuationCache::SCHEMA_NAME);

    std::vector<ChinaBondValuationCache*> update_vec;
    std::vector<ChinaBondValuationCache*> add_vec;
    std::vector<ChinaBondValuationCache*> delete_vec;
    
    dbQuery q;
    q = "";
    ChinaBondValuationCacheVecPtr vec = chinabondvaluation_controller.getCacheListByQueryInThreadSafty(q);

    valuation_sync<ChinaBondValuationCacheController, ChinaBondValuationMonitor, ChinaBondValuationCache, ChinaBondValuationCachePtr, ChinaBondValuationCacheVecPtr>(
        chinabondvaluation_controller, monitor, "id", vec, update_vec, add_vec, delete_vec);
    
	LOGGER_INFO("update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
	if ((update_vec.size() > 0) || (add_vec.size() > 0) || (delete_vec.size() > 0))
	{
		chinabondvaluation_controller.cacheTable();
		chinabondvaluation_controller.commit();
	}
}

