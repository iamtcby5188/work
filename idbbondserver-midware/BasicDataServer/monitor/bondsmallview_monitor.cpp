#include "bondsmallview_monitor.h"
#include "cache/cache_center.h"

BondSmallViewMonitor::BondSmallViewMonitor()
    : Monitor()
{

}

BondSmallViewCachePtr BondSmallViewMonitor::get(const std::string &bond_key_listed_market) const
{
    std::map<std::string, BondSmallViewCachePtr>::const_iterator it = bond_small_view_mp_.find(bond_key_listed_market);

    if (it != bond_small_view_mp_.end()) {
        return (*it).second;
    }
    return BondSmallViewCachePtr();
}

int BondSmallViewMonitor::size() const
{
    return bond_small_view_mp_.size();
}

std::map<std::string, BondSmallViewCachePtr>& BondSmallViewMonitor::refMap()
{
    return bond_small_view_mp_;
}

void BondSmallViewMonitor::load(sql::ResultSet* resSet)
{
    BondSmallViewCachePtr cache(new BondSmallViewCache());

    std::string bond_key_listed_market = resSet->getString("bond_key_listed_market");
    if (bond_key_listed_market.length() == 0) {
        return;
    }

    cache->listed_market = resSet->getString("listed_market");
    cache->bond_key_listed_market = resSet->getString("bond_key_listed_market");
    cache->bond_id = resSet->getString("bond_id");
    cache->bond_subtype = resSet->getString("bond_subtype");
    cache->issuer_rating_current = resSet->getString("issuer_rating_current");
    cache->maturity_term = resSet->getString("maturity_term");
    cache->create_date = resSet->getString("create_date");
    cache->short_name = resSet->getString("short_name");
    cache->warranter = resSet->getString("warranter");
    cache->coupon_type = resSet->getString("coupon_type");
    cache->rating_augment = resSet->getString("rating_augment");
    cache->rating_date = resSet->getInt("rating_date");
    cache->option_type = resSet->getString("option_type");
    cache->pin_yin = resSet->getString("pin_yin");
    cache->pin_yin_full = resSet->getString("pin_yin_full");
    cache->delflag = resSet->getInt("delflag");
    cache->coupon_rate_current = resSet->getDouble("coupon_rate_current");
    cache->maturity_date = resSet->getInt("maturity_date");
    cache->listed_date = resSet->getInt("listed_date");
    cache->delisted_date = resSet->getInt("delisted_date");
    cache->issuer_code = resSet->getString("issuer_code");
    cache->next_coupon_date = resSet->getInt("next_coupon_date");
    cache->redemption_no = resSet->getInt("redemption_no");
    cache->is_cross_mkt = resSet->getString("is_cross_mkt");
    cache->is_mortgage = resSet->getString("is_mortgage");
    cache->rating_current = resSet->getString("rating_current");
    cache->bond_key = resSet->getString("bond_key");
    cache->mkt_type = resSet->getString("mkt_type");
    cache->interest_start_date = resSet->getInt("interest_start_date");
    cache->frn_index_id = resSet->getString("frn_index_id");
    cache->fixing_ma_days = resSet->getInt("fixing_ma_days");
    cache->coupon_rate_spread = resSet->getDouble("coupon_rate_spread");
    cache->option_date = resSet->getInt("option_date");
    cache->issue_amount = resSet->getDouble("issue_amount");
    cache->underwriter_code = resSet->getString("underwriter_code");
    cache->issuer_rating_institution_code = resSet->getString("issuer_rating_institution_code");
    cache->term_unit = resSet->getString("term_unit");
    cache->rating_institution_code = resSet->getString("rating_institution_code");
    cache->is_municipal = resSet->getString("is_municipal");
    cache->issuer_rating_current_npy = resSet->getString("issuer_rating_current_npy");
    cache->rating_current_npy = resSet->getString("rating_current_npy");
    cache->option_type_client = resSet->getString("option_type_client");
    cache->goods_type_client = resSet->getString("goods_type_client");
	cache->bond_goods_type_id = resSet->getString("bond_goods_type_id");
    cache->coupon_rate_spread_client = resSet->getString("coupon_rate_spread_client");
    cache->coupon_rate_current_client = resSet->getString("coupon_rate_current_client");
    cache->maturity_term_client = resSet->getString("maturity_term_client");
    cache->redemption_no_client = resSet->getString("redemption_no_client");
    cache->sort_remain = resSet->getInt("sort_remain");
    cache->product_type = resSet->getString("product_type");
    cache->is_expired = resSet->getBoolean("is_expired");
	cache->issue_year = resSet->getInt("issue_year");
	cache->ent_cor = resSet->getString("ent_cor");

    CacheCenter::sharedInstance()->AttachCurrentThread();
    cache->loadCombinationFields();
    CacheCenter::sharedInstance()->DetachCurrentThread();

    bond_small_view_mp_[bond_key_listed_market] = cache;
}