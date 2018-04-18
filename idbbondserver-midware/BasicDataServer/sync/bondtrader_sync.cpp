#include "bondtrader_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/cache_controller_declear.h"
#include "monitor/bondtrader_monitor.h"
#include "service/service_manager.h"
#include "service/bondservice.h"

BondTraderSync::BondTraderSync()
{

}

BondTraderSync::~BondTraderSync()
{

}

//同步加载本地数据
void BondTraderSync::sync()
{
	BondTraderCacheController bondtrader_cc;
	BondTraderMonitor monitor;

	monitor.init("FetchBondTraderSQL", BondTraderCache::SCHEMA_NAME);

	BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);

	dbQuery q;
	q = "";
	BondTraderCacheVecPtr vec = bondtrader_cc.getCacheListByQueryInThreadSafty(q);

	std::vector<BondTraderCache*> update_vec;
	std::vector<BondTraderCache*> add_vec;
	std::vector<BondTraderCache*> delete_vec;

	db_sync<BondTraderCacheController, BondTraderMonitor, BondTraderCache, BondTraderCachePtr, BondTraderCacheVecPtr>(
		bondtrader_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);

	if (update_vec.size() > 0) {
		bond_service->PublishTraderInfo(update_vec, "UpdateList");
	}

	if (add_vec.size() > 0) {
		bond_service->PublishTraderInfo(add_vec, "AddList");
	}

	if (delete_vec.size() > 0) {
		bond_service->PublishTraderInfo(delete_vec, "DeleteList");
	}

	LOGGER_DEBUG("------ sync Local update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
}

//同步加载CRM数据
void BondTraderSync::sync(BondTraderCacheVecPtr& bond_trader_vec, bool is_init)
{
    BondTraderCacheController bondtrader_cc;
    BondTraderMonitor monitor;

	if (!monitor.initCrm(bond_trader_vec)) return;

    dbQuery q;
    q = "";
    BondTraderCacheVecPtr vec = bondtrader_cc.getCacheListByQueryInThreadSafty(q);

    std::vector<BondTraderCache*> update_vec;
    std::vector<BondTraderCache*> add_vec;
    std::vector<BondTraderCache*> delete_vec;

    crm_sync<BondTraderCacheController, BondTraderMonitor, BondTraderCache, BondTraderCachePtr, BondTraderCacheVecPtr>(
        bondtrader_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);

	if (!is_init)
	{
		LOGGER_DEBUG("------ sync PublishTraderInfo");
		BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);
		if (update_vec.size() > 0) {
			bond_service->PublishTraderInfo(update_vec, "UpdateList");
		}

		if (add_vec.size() > 0) {
			bond_service->PublishTraderInfo(add_vec, "AddList");
		}

		if (delete_vec.size() > 0) {
			bond_service->PublishTraderInfo(delete_vec, "DeleteList");
		}
	}
    
	LOGGER_DEBUG("------ sync CRM update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
}