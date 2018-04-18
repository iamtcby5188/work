#include "bondreferquote_sync.h"
#include "service/bondquote_service.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/cache_center.h"
#include "service/service_manager.h"

BondReferQuoteSync::BondReferQuoteSync()
{

}

BondReferQuoteSync::~BondReferQuoteSync()
{

}
/*
void BondReferQuoteSync::syncForBondChanged(const std::vector<std::string> &updated_bond_vec)
{
    BondQuoteReferCacheController bond_quote_refer_cc;

    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<BondQuoteReferCachePtr> publish_bond_quote_refer_vec;
    std::vector<std::string>::const_iterator it = updated_bond_vec.begin();
    for ( ; it != updated_bond_vec.end(); ++it) {
        std::string bond_key_listed_market = *it;

        dbQuery q;
        q = "bond_key_listed_market = ", bond_key_listed_market;

        BondQuoteReferCacheVecPtr cache_vec = bond_quote_refer_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteReferCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBondSmallViewFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_refer_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_refer_vec.push_back(*iter);
        }
    }

    if (publish_bond_quote_refer_vec.size() > 0) { 
        bond_quote_service->PublishBondReferQuote(publish_bond_quote_refer_vec, "UpdateList", true);
    }
}

void BondReferQuoteSync::syncForAccountChanged(const std::vector<std::string> &updated_account_vec)
{
    BondQuoteReferCacheController bond_quote_refer_cc;

    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<BondQuoteReferCachePtr> publish_bond_quote_refer_vec;
    std::vector<std::string>::const_iterator it = updated_account_vec.begin();
    for ( ; it != updated_account_vec.end(); ++it) {
        std::string account_id = *it;

        dbQuery q;
        q = "operator_id = ", account_id;

        BondQuoteReferCacheVecPtr cache_vec = bond_quote_refer_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteReferCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadBrokerFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_refer_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_refer_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_quote_refer_vec.size() > 0) { 
        bond_quote_service->PublishBondReferQuote(publish_bond_quote_refer_vec, "UpdateList", true);
    }
}

void BondReferQuoteSync::syncForBondTraderChanged(const std::vector<std::string> &updated_bondtrader_vec)
{
    BondQuoteReferCacheController bond_quote_refer_cc;

    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<BondQuoteReferCachePtr> publish_bond_quote_refer_vec;
    std::vector<std::string>::const_iterator it = updated_bondtrader_vec.begin();
    for ( ; it != updated_bondtrader_vec.end(); ++it) {
        std::string bondtrader_id = *it;

        dbQuery q;
        q = "bank_agent_id = ", bondtrader_id;

        BondQuoteReferCacheVecPtr cache_vec = bond_quote_refer_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteReferCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadTraderFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_refer_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_refer_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_quote_refer_vec.size() > 0) { 
        bond_quote_service->PublishBondReferQuote(publish_bond_quote_refer_vec, "UpdateList", true);
    }
}

void BondReferQuoteSync::syncForTradingInstitutionChanged(const std::vector<std::string> &updated_tradinginstitution_vec)
{
    BondQuoteReferCacheController bond_quote_refer_cc;

    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);

    std::vector<BondQuoteReferCachePtr> publish_bond_quote_refer_vec;
    std::vector<std::string>::const_iterator it = updated_tradinginstitution_vec.begin();
    for ( ; it != updated_tradinginstitution_vec.end(); ++it) {
        std::string tradinginstitution_id = *it;

        dbQuery q;
        q = "bank_id = ", tradinginstitution_id;

        BondQuoteReferCacheVecPtr cache_vec = bond_quote_refer_cc.getCacheListByQueryInThreadSafty(q);
        if (!cache_vec || cache_vec->size() == 0) {
            continue;
        }

        std::vector<BondQuoteReferCachePtr>::const_iterator iter = cache_vec->begin();
        for ( ; iter != cache_vec->end(); ++iter) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*iter)->loadTradingInstitutionFields();
            CacheCenter::sharedInstance()->DetachCurrentThread();

            dbQuery query;
            query = "id = ", (*iter)->id;
            bond_quote_refer_cc.updateCacheInThreadSaftyWithoutDBSync(query, (*iter).get());

            publish_bond_quote_refer_vec.push_back(*iter);
        }
    }
    
    if (publish_bond_quote_refer_vec.size() > 0) { 
        bond_quote_service->PublishBondReferQuote(publish_bond_quote_refer_vec, "UpdateList", true);
    }
}
*/