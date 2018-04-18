#include "bondbestquotecalc_business.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/cache_center.h"
#include "include/common.h"

bool func_equal_bid(const BondQuoteCachePtr &lhs, const BondQuoteCachePtr &rhs)
{
    better_bid comp(true);
    return (!comp(lhs, rhs) && !comp(rhs, lhs));
}

bool func_equal_ofr(const BondQuoteCachePtr &lhs, const BondQuoteCachePtr &rhs)
{
    better_ofr comp(true);
    return (!comp(lhs, rhs) && !comp(rhs, lhs));
}

void BondBestQuoteCalcBusiness::SortPriceBid(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' " + GetCompanyConditionStringWithPreAnd() + " and symbol = 1 and internally = '1'"; //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_bid_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_bid_->push_back(*it);
    }
    
    best_price_bid_->sort(better_bid());

    return;
}

void BondBestQuoteCalcBusiness::SortPriceOfr(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' " + GetCompanyConditionStringWithPreAnd() + " and symbol = -1 and internally = '1'"; //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_ofr_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_ofr_->push_back(*it);
    }

    best_price_ofr_->sort(better_ofr());

    return;
}

void BondBestQuoteCalcBusiness::SortPriceBidWithInternal(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' " + GetCompanyConditionStringWithPreAnd() + " and symbol = 1"; //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_bid_with_internal_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_bid_with_internal_->push_back(*it);
    }
    
    best_price_bid_with_internal_->sort(better_bid());

    return;
}

void BondBestQuoteCalcBusiness::SortPriceOfrWithInternal(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' " + GetCompanyConditionStringWithPreAnd() + " and symbol = -1"; //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_ofr_with_internal_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_ofr_with_internal_->push_back(*it);
    }
    
    best_price_ofr_with_internal_->sort(better_ofr());

    return;
}

int BondBestQuoteCalcBusiness::GetBestQuoteSortWithInternal(const std::string &bond_key_listed_market, std::vector<std::pair<BondQuoteCachePtr, BondQuoteCachePtr> > &result)
{
    SortPriceBidWithInternal(bond_key_listed_market);
    SortPriceOfrWithInternal(bond_key_listed_market);

    size_t bid_best_quote_count = 0;
    size_t ofr_best_quote_count = 0;

    if (best_price_bid_with_internal_->size() > 0) {
        bid_best_quote_count = 1;
       
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_with_internal_->begin();
        for (it++ ; it != best_price_bid_with_internal_->end(); ++it) {
            if (func_equal_bid(best_price_bid_with_internal_->front(), *it)) {
                bid_best_quote_count++;
            } else {
                break;
            }
        }
    }

    if (best_price_ofr_with_internal_->size() > 0) {
        ofr_best_quote_count = 1;
        
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_with_internal_->begin();
        for (it++ ; it != best_price_ofr_with_internal_->end(); ++it) {
            if (func_equal_ofr(best_price_ofr_with_internal_->front(), *it)) {
                ofr_best_quote_count++;
            } else {
                break;
            }
        }
    }

    size_t i = 0;
    size_t j = 0;
    std::list<BondQuoteCachePtr>::const_iterator it_bid = best_price_bid_with_internal_->begin();
    std::list<BondQuoteCachePtr>::const_iterator it_ofr = best_price_ofr_with_internal_->begin();
    while (i < bid_best_quote_count || j < ofr_best_quote_count){
        std::pair<BondQuoteCachePtr, BondQuoteCachePtr> one_pair;
        if(i < bid_best_quote_count){
            one_pair.first = *it_bid;
            it_bid++;
            i++;
        }

        if(j < ofr_best_quote_count){
            one_pair.second = *it_ofr;
            it_ofr++;
            j++;
        }
        result.push_back(one_pair);
    }

    while (i < best_price_bid_with_internal_->size() || j < best_price_ofr_with_internal_->size()){
        std::pair<BondQuoteCachePtr, BondQuoteCachePtr> one_pair;
        if(i < best_price_bid_with_internal_->size()){
            one_pair.first = *it_bid;
            it_bid++;
            i++;
        }

        if(j < best_price_ofr_with_internal_->size()){
            one_pair.second = *it_ofr;
            it_ofr++;
            j++;
        }
        result.push_back(one_pair);
    }

    return std::max<int>(bid_best_quote_count, ofr_best_quote_count);
}


std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteBidWithInternal(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceBidWithInternal(bond_key_listed_market);

    int bid_best_quote_count = 0;

    if (best_price_bid_with_internal_->size() > 0) {
        bid_best_quote_count = 1;
        result.push_back(best_price_bid_with_internal_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_with_internal_->begin();
        for (it++ ; it != best_price_bid_with_internal_->end(); ++it) {
            if (func_equal_bid(best_price_bid_with_internal_->front(), *it)) {
                bid_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}

std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteOfrWithInternal(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceOfrWithInternal(bond_key_listed_market);

    int ofr_best_quote_count = 0;

    if (best_price_ofr_with_internal_->size() > 0) {
        ofr_best_quote_count = 1;
        result.push_back(best_price_ofr_with_internal_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_with_internal_->begin();
        for (it++ ; it != best_price_ofr_with_internal_->end(); ++it) {
            if (func_equal_ofr(best_price_ofr_with_internal_->front(), *it)) {
                ofr_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}

std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteBid(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceBid(bond_key_listed_market);

    int bid_best_quote_count = 0;

    if (best_price_bid_->size() > 0) {
        bid_best_quote_count = 1;
        result.push_back(best_price_bid_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_->begin();
        for (it++ ; it != best_price_bid_->end(); ++it) {
            if (func_equal_bid(best_price_bid_->front(), *it)) {
                bid_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}

std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteOfr(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceOfr(bond_key_listed_market);

    int ofr_best_quote_count = 0;

    if (best_price_ofr_->size() > 0) {
        ofr_best_quote_count = 1;
        result.push_back(best_price_ofr_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_->begin();
        for (it++ ; it != best_price_ofr_->end(); ++it) {
            if (func_equal_ofr(best_price_ofr_->front(), *it)) {
                ofr_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}