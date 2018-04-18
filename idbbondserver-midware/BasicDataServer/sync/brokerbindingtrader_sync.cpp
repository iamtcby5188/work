#include "brokerbindingtrader_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "monitor/brokerbindingtrader_monitor.h"
#include "service/service_manager.h"
#include "service/bondservice.h"

BrokerBindingTraderSync::BrokerBindingTraderSync()
{

}

BrokerBindingTraderSync::~BrokerBindingTraderSync()
{

}

//同步加载本地数据
void BrokerBindingTraderSync::sync()
{
	BrokerBindingTraderCacheController binding_cc;
	BrokerBindingTraderMonitor monitor;

	monitor.init("FetchBrokerBindingTraderSQL", BrokerBindingTraderCache::SCHEMA_NAME);

	BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);

	dbQuery q;
	q = "";
	BrokerBindingTraderCacheVecPtr vec = binding_cc.getCacheListByQueryInThreadSafty(q);

	std::vector<BrokerBindingTraderCache*> update_vec;
	std::vector<BrokerBindingTraderCache*> add_vec;
	std::vector<BrokerBindingTraderCache*> delete_vec;

	db_sync<BrokerBindingTraderCacheController, BrokerBindingTraderMonitor, BrokerBindingTraderCache, BrokerBindingTraderCachePtr, BrokerBindingTraderCacheVecPtr>(
		binding_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);

	if (update_vec.size() > 0) {
		bond_service->PublishBrokerBindingTrader(update_vec, "UpdateList");
	}

	if (add_vec.size() > 0) {
		bond_service->PublishBrokerBindingTrader(add_vec, "AddList");
	}

	if (delete_vec.size() > 0) {
		bond_service->PublishBrokerBindingTrader(delete_vec, "DeleteList");
	}

	LOGGER_DEBUG("------ sync Local update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
}

//同步加载CRM数据
void BrokerBindingTraderSync::sync(BrokerBindingTraderCacheVecPtr& broker_bind_trader_vec, bool is_init)
{
    BrokerBindingTraderCacheController binding_cc;
    BrokerBindingTraderMonitor monitor;

	if (!monitor.initCrm(broker_bind_trader_vec)) return;
    
    dbQuery q;
    q = "";
    BrokerBindingTraderCacheVecPtr vec = binding_cc.getCacheListByQueryInThreadSafty(q);

    std::vector<BrokerBindingTraderCache*> update_vec;
    std::vector<BrokerBindingTraderCache*> add_vec;
    std::vector<BrokerBindingTraderCache*> delete_vec;

    crm_sync<BrokerBindingTraderCacheController, BrokerBindingTraderMonitor, BrokerBindingTraderCache, BrokerBindingTraderCachePtr, BrokerBindingTraderCacheVecPtr>(
        binding_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);

	if (!is_init)
	{
		LOGGER_DEBUG("------ sync PublishBrokerBindingTrader");
		BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);
		if (update_vec.size() > 0) {
			bond_service->PublishBrokerBindingTrader(update_vec, "UpdateList");
		}

		if (add_vec.size() > 0) {
			bond_service->PublishBrokerBindingTrader(add_vec, "AddList");
		}

		if (delete_vec.size() > 0) {
			bond_service->PublishBrokerBindingTrader(delete_vec, "DeleteList");
		}
	}
    
	LOGGER_DEBUG("------ sync CRM update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());
}
