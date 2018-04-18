#include "dcsbonddeal_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/controller/dcsbonddeal_cachecontroller.h"
#include "cache/cache_center.h"
#include "monitor/dcsbonddeal_monitor.h"
#include "service/service_manager.h"
#include "service/dcsbonddeal_service.h"

DCSBondDealSync::DCSBondDealSync()
{

}

DCSBondDealSync::~DCSBondDealSync()
{

}


bool DCSBondDealSync::sync()
{
	DcsBondDealCacheController bond_deal_cc;
	DCSBondDealMonitor monitor;

	bool result = monitor.init();
	if (!result) return false;

	DcsBondDealService *bond_deal_service = (DcsBondDealService*)ServiceManager::singleton()->getService(kServiceDcsBondDeal);

	dbQuery q;
	q = "";
	DcsBondDealCacheVecPtr vec = bond_deal_cc.getCacheListByQueryInThreadSafty(q);

	std::vector<DcsBondDealCachePtr> update_vec;
	std::vector<DcsBondDealCachePtr> add_vec;
	std::vector<DcsBondDealCachePtr> delete_vec;

	dcs_sync<DcsBondDealCacheController, DCSBondDealMonitor, DcsBondDealCache, DcsBondDealCachePtr, DcsBondDealCacheVecPtr>(
		bond_deal_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);

	LOGGER_INFO("update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size());

	if (update_vec.size() > 0)
	{
		bond_deal_service->PublishBondDeal(update_vec, "UpdateList");
	}

	if (add_vec.size() > 0)
	{
		bond_deal_service->PublishBondDeal(add_vec, "AddList");
	}

	if (delete_vec.size() > 0) 
	{
		bond_deal_service->PublishBondDeal(delete_vec, "DeleteList");
	}

	return true;
}
