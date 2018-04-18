#include "tradinginstitution_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/cache_controller_declear.h"
#include "monitor/tradinginstitution_monitor.h"
#include "service/service_manager.h"
#include "service/bondservice.h"

TradingInstitutionSync::TradingInstitutionSync()
{

}

TradingInstitutionSync::~TradingInstitutionSync()
{

}

//同步加载本地数据
void TradingInstitutionSync::sync()
{
	TradingInstitutionCacheController tradinginstitution_cc;
	TradingInstitutionMonitor monitor;

	monitor.init("FetchTradingInstitutionSQL", TradingInstitutionCache::SCHEMA_NAME);

	BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);

	std::vector<TradingInstitutionCache*> update_vec;
	std::vector<TradingInstitutionCache*> add_vec;
	std::vector<TradingInstitutionCache*> delete_vec;

	dbQuery q;
	q = "";
	TradingInstitutionCacheVecPtr vec = tradinginstitution_cc.getCacheListByQueryInThreadSafty(q);

	db_sync<TradingInstitutionCacheController, TradingInstitutionMonitor, TradingInstitutionCache, TradingInstitutionCachePtr, TradingInstitutionCacheVecPtr>(
		tradinginstitution_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);

	if (update_vec.size() > 0) {
		bond_service->PublishInstitutionInfo(update_vec, "UpdateList");
	}

	if (add_vec.size() > 0) {
		bond_service->PublishInstitutionInfo(add_vec, "AddList");
	}

	if (delete_vec.size() > 0) {
		bond_service->PublishInstitutionInfo(delete_vec, "DeleteList");
	}

	LOGGER_DEBUG("------ sync Local update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
}

//同步加载CRM数据
void TradingInstitutionSync::sync(TradingInstitutionCacheVecPtr& inst_vec, bool is_init)
{
    TradingInstitutionCacheController tradinginstitution_cc;
    TradingInstitutionMonitor monitor;

	if (!monitor.initCrm(inst_vec)) return;

    std::vector<TradingInstitutionCache*> update_vec;
    std::vector<TradingInstitutionCache*> add_vec;
    std::vector<TradingInstitutionCache*> delete_vec;
    
    dbQuery q;
    q = "";
    TradingInstitutionCacheVecPtr vec = tradinginstitution_cc.getCacheListByQueryInThreadSafty(q);

    crm_sync<TradingInstitutionCacheController, TradingInstitutionMonitor, TradingInstitutionCache, TradingInstitutionCachePtr, TradingInstitutionCacheVecPtr>(
        tradinginstitution_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);

	if (!is_init)
	{
		LOGGER_DEBUG("------ sync PublishInstitutionInfo");
		BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);
		if (update_vec.size() > 0) {
			bond_service->PublishInstitutionInfo(update_vec, "UpdateList");
		}

		if (add_vec.size() > 0) {
			bond_service->PublishInstitutionInfo(add_vec, "AddList");
		}

		if (delete_vec.size() > 0) {
			bond_service->PublishInstitutionInfo(delete_vec, "DeleteList");
		}
	}
    
	LOGGER_DEBUG("------ sync CRM update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
}