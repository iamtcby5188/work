#include "bondbestquote_sync.h"
#include "service/bondbestquote_service.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/cache_center.h"
#include "service/service_manager.h"

BondBestQuoteSync::BondBestQuoteSync()
{

}

BondBestQuoteSync::~BondBestQuoteSync()
{

}

void BondBestQuoteSync::syncForBondChanged(const std::vector<std::string> &updated_bond_vec)
{
    BondBestQuoteCacheController bond_best_quote_cc;

    BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);

    std::vector<BondBestQuoteCachePtr> publish_bond_best_quote_vec;
    std::vector<std::string>::const_iterator it = updated_bond_vec.begin();
    for ( ; it != updated_bond_vec.end(); ++it) {
        std::string bond_key_listed_market = *it;

        dbQuery q;
        q = "bond_key_listed_market = ", bond_key_listed_market;

        BondBestQuoteCacheVecPtr cache_vec = bond_best_quote_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondBestQuoteCachePtr>::const_iterator iter = cache_vec->begin();
        for (; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBondSmallViewFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            bond_best_quote_cc.updateCacheInThreadSaftyWithoutDBSync(q, (*iter).get());

            publish_bond_best_quote_vec.push_back(*iter);
        }
    }

    if (publish_bond_best_quote_vec.size() > 0) {        
        bond_best_quote_service->PublishBondBestQuote(publish_bond_best_quote_vec, "UpdateList", true);
    }
}
/*
void BondBestQuoteSync::syncForQuoteChanged(const std::vector<std::string> &updated_quote_vec)
{
    BondBestQuoteCacheController bond_best_quote_cc;

    BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);

    std::vector<BondBestQuoteCachePtr> publish_bond_best_quote_vec;
    std::vector<std::string>::const_iterator it = updated_quote_vec.begin();
    for ( ; it != updated_quote_vec.end(); ++it) {
        std::string quote_id = *it;

        dbQuery q;
        q = "bid_offer_id = ", quote_id, "or ofr_offer_id = ", quote_id;

        BondBestQuoteCacheVecPtr cache_vec = bond_best_quote_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondBestQuoteCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBondQuoteFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            bond_best_quote_cc.updateCacheInThreadSaftyWithoutDBSync(q, (*iter).get());

            publish_bond_best_quote_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_best_quote_vec.size() > 0) {        
        bond_best_quote_service->PublishBondBestQuote(publish_bond_best_quote_vec, "UpdateList", true);
    }
}
*/