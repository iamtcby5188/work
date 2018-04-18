#include "bondquote_sync.h"
#include "bondbestquote_sync.h"
#include "service/bondquote_service.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/cache_center.h"
#include "service/service_manager.h"

BondQuoteSync::BondQuoteSync()
{

}

BondQuoteSync::~BondQuoteSync()
{

}

void BondQuoteSync::syncForBondChanged(const std::vector<std::string> &updated_bond_vec)
{
    BondQuoteCacheController bond_quote_cc;
    
    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<BondQuoteCachePtr> publish_bond_quote_vec;
    std::vector<std::string>::const_iterator it = updated_bond_vec.begin();
    for ( ; it != updated_bond_vec.end(); ++it) {
        std::string bond_key_listed_market = *it;

        dbQuery q;
        q = "bond_key_listed_market = ", bond_key_listed_market;

        BondQuoteCacheVecPtr cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteCachePtr>::const_iterator iter = cache_vec->begin();
        for (; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBondSmallViewFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_quote_vec.size() > 0) {
        bond_quote_service->PublishBondQuote(publish_bond_quote_vec, "UpdateList", true);
    }
}
/*
void BondQuoteSync::syncForAccountChanged(const std::vector<std::string> &updated_account_vec)
{
    BondQuoteCacheController bond_quote_cc;

    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<std::string> updated_quote_vec;
    std::vector<BondQuoteCachePtr> publish_bond_quote_vec;
    std::vector<std::string>::const_iterator it = updated_account_vec.begin();
    for ( ; it != updated_account_vec.end(); ++it) {
        std::string account_id = *it;

        dbQuery q;
        q = "operator_id = ", account_id;

        BondQuoteCacheVecPtr cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBrokerFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_vec.push_back(*iter);
            updated_quote_vec.push_back((*iter)->id);
        }
    }
    
    if (publish_bond_quote_vec.size() > 0) {
        bond_quote_service->PublishBondQuote(publish_bond_quote_vec, "UpdateList", true);
    }

    if (updated_quote_vec.size() > 0) {
        BondBestQuoteSync bondbestquote_sync;
        bondbestquote_sync.syncForQuoteChanged(updated_quote_vec);
    }
}


void BondQuoteSync::syncForBondTraderChanged(const std::vector<std::string> &updated_bondtrader_vec)
{
    BondQuoteCacheController bond_quote_cc;

    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<std::string> updated_quote_vec;
    std::vector<BondQuoteCachePtr> publish_bond_quote_vec;
    std::vector<std::string>::const_iterator it = updated_bondtrader_vec.begin();
    for ( ; it != updated_bondtrader_vec.end(); ++it) {
        std::string bondtrader_id = *it;

        dbQuery q;
        q = "bank_agent_id = ", bondtrader_id;

        BondQuoteCacheVecPtr cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadTraderFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_vec.push_back(*iter);
            updated_quote_vec.push_back((*iter)->id);
        }
    }
    
    if (publish_bond_quote_vec.size() > 0) {
        bond_quote_service->PublishBondQuote(publish_bond_quote_vec, "UpdateList", true);
    }

    if (updated_quote_vec.size() > 0) {
        BondBestQuoteSync bondbestquote_sync;
        bondbestquote_sync.syncForQuoteChanged(updated_quote_vec);
    }
}

void BondQuoteSync::syncForTradingInstitutionChanged(const std::vector<std::string> &updated_tradinginstitution_vec)
{
    BondQuoteCacheController bond_quote_cc;

    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<std::string> updated_quote_vec;
    std::vector<BondQuoteCachePtr> publish_bond_quote_vec;
    std::vector<std::string>::const_iterator it = updated_tradinginstitution_vec.begin();
    for ( ; it != updated_tradinginstitution_vec.end(); ++it) {
        std::string tradinginstitution_id = *it;

        dbQuery q;
        q = "bank_id = ", tradinginstitution_id;

        BondQuoteCacheVecPtr cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadTradingInstitutionFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_vec.push_back(*iter);
            updated_quote_vec.push_back((*iter)->id);
        }
    }
    
    if (publish_bond_quote_vec.size() > 0) {
        bond_quote_service->PublishBondQuote(publish_bond_quote_vec, "UpdateList", true);
    }

    if (updated_quote_vec.size() > 0) {
        BondBestQuoteSync bondbestquote_sync;
        bondbestquote_sync.syncForQuoteChanged(updated_quote_vec);
    }
}
*/