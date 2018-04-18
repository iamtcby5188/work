#include "bondsmallview_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_center.h"
#include "monitor/bondsmallview_monitor.h"
#include "service/service_manager.h"
#include "service/bondservice.h"

BondSmallViewSync::BondSmallViewSync()
{

}

BondSmallViewSync::~BondSmallViewSync()
{

}


void BondSmallViewSync::sync()
{
    BondSmallViewCacheController bond_small_view_cc;
    BondSmallViewMonitor monitor;

	if(!monitor.init("FetchBondSmallViewSQL", BondSmallViewCache::SCHEMA_NAME))
		return;

    BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);
       
    dbQuery q;
    q = "";
    BondSmallViewCacheVecPtr vec = bond_small_view_cc.getCacheListByQueryInThreadSafty(q);

    std::vector<BondSmallViewCache*> update_vec;
    std::vector<BondSmallViewCache*> add_vec;
    std::vector<BondSmallViewCache*> delete_vec;

    db_sync<BondSmallViewCacheController, BondSmallViewMonitor, BondSmallViewCache, BondSmallViewCachePtr, BondSmallViewCacheVecPtr>(
        bond_small_view_cc, monitor, "bond_key_listed_market", vec, update_vec, add_vec, delete_vec);

	LOGGER_INFO("update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size())

    for (int i = 0; i < update_vec.size(); ++i) {
        bond_small_view_cc.loadAcrossMarketBondIds(update_vec[i]); 
		bond_small_view_cc.loadIssuerRating(update_vec[i]); //Add by Young 20-Apr-2016
	}

	for (int i = 0; i < add_vec.size(); ++i) {
        bond_small_view_cc.loadAcrossMarketBondIds(add_vec[i]);  
		bond_small_view_cc.loadIssuerRating(add_vec[i]); //Add by Young 20-Apr-2016
    }
    
	if (update_vec.size() > 0) {
        bond_service->PublishBondSmallView(update_vec, "UpdateList");
        bond_service->PublishBondSmallViewDetail(update_vec, "UpdateList");
        bond_service->PublishBondInfo(update_vec, "UpdateList"); 
    }

	if (add_vec.size() > 0) {
        bond_service->PublishBondSmallView(add_vec, "AddList");  
        bond_service->PublishBondSmallViewDetail(add_vec, "AddList");
        bond_service->PublishBondInfo(add_vec, "AddList");   
    }

	if (delete_vec.size() > 0) {
        bond_service->PublishBondSmallView(delete_vec, "DeleteList");  
        bond_service->PublishBondSmallViewDetail(delete_vec, "DeleteList");
        bond_service->PublishBondInfo(delete_vec, "DeleteList");            
    }
	
    /*std::vector<std::string> updated_bond_vec;
    if (update_vec.size() > 0) {
        std::vector<BondSmallViewCache*>::const_iterator it = update_vec.begin();
        for ( ; it != update_vec.end(); ++it) {
            updated_bond_vec.push_back((*it)->bond_key_listed_market);
        }
    }

    if (updated_bond_vec.size() > 0) {
        BondQuoteSync bondquote_sync;
        bondquote_sync.syncForBondChanged(updated_bond_vec);

        BondReferQuoteSync bondreferquote_sync;
        bondreferquote_sync.syncForBondChanged(updated_bond_vec);

        BondBestQuoteSync bondbestquote_sync;
        bondbestquote_sync.syncForBondChanged(updated_bond_vec);

        BondDealSync bonddeal_sync;
        bonddeal_sync.syncForBondChanged(updated_bond_vec);
    }
	*/
}
/*
void BondSmallViewSync::syncForChinaBondValuationChanged(const std::vector<std::string> &updated_chinabondvaluation_vec)
{
    BondSmallViewCacheController bondsmallview_cc;

    BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);
    
    //std::vector<BondSmallViewCachePtr> update_cache_ptr_vec;
    //std::vector<BondSmallViewCache*> update_vec;
    //std::vector<std::string> updated_bond_vec;
    std::vector<std::string>::const_iterator it = updated_chinabondvaluation_vec.begin();

    for ( ; it != updated_chinabondvaluation_vec.end(); ++it) {
        std::string bond_id = *it;

        dbQuery q;
        q = "bond_id = ", bond_id;

        BondSmallViewCacheVecPtr cache_vec = bondsmallview_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondSmallViewCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadChinaBondValuationFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "bond_key_listed_market = ", (*iter)->bond_key_listed_market;
            bondsmallview_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            //updated_bond_vec.push_back((*iter)->bond_key_listed_market);

            //update_cache_ptr_vec.push_back(*iter);
        }
    }
       
    //std::vector<BondSmallViewCachePtr>::const_iterator it2 = update_cache_ptr_vec.begin();
    //for ( ; it2 != update_cache_ptr_vec.end(); ++it2) {
    //    update_vec.push_back((*it2).get());
    //}

    //if (updated_bond_vec.size() > 0) {       
    //    bond_service->PublishBondSmallViewDetail(update_vec, "UpdateList");
    //}

    //if (updated_bond_vec.size() > 0) {
    //    BondQuoteSync bondquote_sync;
    //    bondquote_sync.syncForBondChanged(updated_bond_vec);

    //    BondReferQuoteSync bondreferquote_sync;
    //    bondreferquote_sync.syncForBondChanged(updated_bond_vec);

    //    BondBestQuoteSync bondbestquote_sync;
    //    bondbestquote_sync.syncForBondChanged(updated_bond_vec);

    //    BondDealSync bonddeal_sync;
    //    bonddeal_sync.syncForBondChanged(updated_bond_vec);
    //}
	
}*/
