#include "chinabonddeviatedvalue_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/cache_controller_declear.h"
#include "monitor/chinabonddeviatedvalue_monitor.h"
#include "service/service_manager.h"
#include "service/bondservice.h"
#include "bondsmallview_sync.h"

ChinaBondDeviatedValueSync::ChinaBondDeviatedValueSync()
{

}

ChinaBondDeviatedValueSync::~ChinaBondDeviatedValueSync()
{

}

void ChinaBondDeviatedValueSync::sync()
{
    ChinaBondDeviatedValueCacheController chinabonddeviatedvalue_controller;
    ChinaBondDeviatedValueMonitor monitor;

    monitor.init("FetchChinaBondDeviatedValueSQL", ChinaBondDeviatedValueCache::SCHEMA_NAME);

    BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);
    
    dbQuery q;
    q = "";
    ChinaBondDeviatedValueCacheVecPtr vec = chinabonddeviatedvalue_controller.getCacheListByQueryInThreadSafty(q);

    std::vector<ChinaBondDeviatedValueCache*> update_vec;
    std::vector<ChinaBondDeviatedValueCache*> add_vec;
    std::vector<ChinaBondDeviatedValueCache*> delete_vec;

    db_sync<ChinaBondDeviatedValueCacheController, ChinaBondDeviatedValueMonitor, ChinaBondDeviatedValueCache, ChinaBondDeviatedValueCachePtr, ChinaBondDeviatedValueCacheVecPtr>(
        chinabonddeviatedvalue_controller, monitor, "id", vec, update_vec, add_vec, delete_vec);
        
    LOGGER_INFO("update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size())

    if (update_vec.size() > 0) {
        bond_service->PublishBondDeviatedValue(update_vec, "UpdateList");    
    }

    if (add_vec.size() > 0) {
        bond_service->PublishBondDeviatedValue(add_vec, "AddList");       
    }

    if (delete_vec.size() > 0) {
        bond_service->PublishBondDeviatedValue(delete_vec, "DeleteList");  
    }
}

