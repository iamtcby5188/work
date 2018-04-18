#include "chinabondvaluation_monitor.h"
#include "cache/cache_center.h"

ChinaBondValuationMonitor::ChinaBondValuationMonitor()
    : Monitor()
{

}

ChinaBondValuationCachePtr ChinaBondValuationMonitor::get(const std::string &id) const
{
    std::map<std::string, ChinaBondValuationCachePtr>::const_iterator it = chinabondvaluation_mp_.find(id);

    if (it != chinabondvaluation_mp_.end()) {
        return (*it).second;
    }
    return ChinaBondValuationCachePtr();
}

int ChinaBondValuationMonitor::size() const
{
    return chinabondvaluation_mp_.size();
}

std::map<std::string, ChinaBondValuationCachePtr>& ChinaBondValuationMonitor::refMap()
{
    return chinabondvaluation_mp_;
}

void ChinaBondValuationMonitor::load(sql::ResultSet* resSet)
{
    ChinaBondValuationCachePtr cache(new ChinaBondValuationCache());

    std::string id = resSet->getString("id");
    if (id.length() == 0) {
        return;
    }

    cache->id = resSet->getString("id");
    cache->bond_id = resSet->getString("bond_id");
    cache->listed_market = resSet->getString("listed_market");
    cache->valuation_yield = resSet->getString("valuation_yield");
    cache->valuation_net_price = resSet->getString("valuation_net_price");
    cache->valuation_date = resSet->getString("valuation_date");
    cache->status = resSet->getString("status");
    cache->modify_time = resSet->getString("modify_time");
	cache->bond_key_listed_market = resSet->getString("bond_key_listed_market");
	cache->val_modified_duration = resSet->getDouble("val_modified_duration");
	cache->val_convexity = resSet->getDouble("val_convexity");
	cache->val_basis_point_value = resSet->getDouble("val_basis_point_value");

    chinabondvaluation_mp_[id] = cache;
}