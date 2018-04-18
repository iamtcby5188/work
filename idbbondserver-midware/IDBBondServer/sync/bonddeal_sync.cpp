#include "bonddeal_sync.h"
#include "service/bonddeal_service.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/cache_center.h"
#include "service/service_manager.h"

BondDealSync::BondDealSync()
{

}

BondDealSync::~BondDealSync()
{

}

/*
void BondDealSync::syncForBondChanged(const std::vector<std::string> &updated_bond_vec)
{
    BondDealCacheController bond_deal_cc;

    BondDealService *bond_deal_service = (BondDealService*)ServiceManager::singleton()->getService(kServiceBondDeal);

    std::vector<BondDealCachePtr> publish_bond_deal_vec;
    std::vector<std::string>::const_iterator it = updated_bond_vec.begin();
    for ( ; it != updated_bond_vec.end(); ++it) {
        std::string bond_key_listed_market = *it;

        dbQuery q;
        q = "bond_key_listed_market = ", bond_key_listed_market;

        BondDealCacheVecPtr cache_vec = bond_deal_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondDealCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBondSmallViewFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_deal_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_deal_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_deal_vec.size() > 0) {
        bond_deal_service->PublishBondDeal(publish_bond_deal_vec, "UpdateList", true);
    }
}

void BondDealSync::syncForAccountChanged(const std::vector<std::string> &updated_account_vec)
{
    BondDealCacheController bond_deal_cc;

    BondDealService *bond_deal_service = (BondDealService*)ServiceManager::singleton()->getService(kServiceBondDeal);

    std::vector<BondDealCachePtr> publish_bond_deal_vec;
    std::vector<std::string>::const_iterator it = updated_account_vec.begin();
    for ( ; it != updated_account_vec.end(); ++it) {
        std::string account_id = *it;

        dbQuery q;
        q = "bid_broker_id = ", account_id, "or ofr_broker_id = ", account_id;

        BondDealCacheVecPtr cache_vec = bond_deal_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondDealCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBrokerFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_deal_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_deal_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_deal_vec.size() > 0) {
        bond_deal_service->PublishBondDeal(publish_bond_deal_vec, "UpdateList", true);
    }
}


void BondDealSync::syncForBondTraderChanged(const std::vector<std::string> &updated_bondtrader_vec)
{
    BondDealCacheController bond_deal_cc;

    BondDealService *bond_deal_service = (BondDealService*)ServiceManager::singleton()->getService(kServiceBondDeal);

    std::vector<BondDealCachePtr> publish_bond_deal_vec;
    std::vector<std::string>::const_iterator it = updated_bondtrader_vec.begin();
    for ( ; it != updated_bondtrader_vec.end(); ++it) {
        std::string bondtrader_id = *it;

        dbQuery q;
        q = "bid_agent_id = ", bondtrader_id, "or ofr_agent_id = ", bondtrader_id;

        BondDealCacheVecPtr cache_vec = bond_deal_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondDealCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadTraderFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_deal_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_deal_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_deal_vec.size() > 0) {
        bond_deal_service->PublishBondDeal(publish_bond_deal_vec, "UpdateList", true);
    }
}

void BondDealSync::syncForTradingInstitutionChanged(const std::vector<std::string> &updated_tradinginstitution_vec)
{
    BondDealCacheController bond_deal_cc;
    
    BondDealService *bond_deal_service = (BondDealService*)ServiceManager::singleton()->getService(kServiceBondDeal);

    std::vector<BondDealCachePtr> publish_bond_deal_vec;
    std::vector<std::string>::const_iterator it = updated_tradinginstitution_vec.begin();
    for ( ; it != updated_tradinginstitution_vec.end(); ++it) {
        std::string tradinginstitution_id = *it;

        dbQuery q;
        q = "bid_bank_id = ", tradinginstitution_id, "or ofr_bank_id = ", tradinginstitution_id;

        BondDealCacheVecPtr cache_vec = bond_deal_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondDealCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadTradingInstitutionFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_deal_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_deal_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_deal_vec.size() > 0) {
        bond_deal_service->PublishBondDeal(publish_bond_deal_vec, "UpdateList", true);
    }
}
*/