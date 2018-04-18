#include "chinabonddeviatedvalue_monitor.h"
#include "cache/cache_center.h"

ChinaBondDeviatedValueMonitor::ChinaBondDeviatedValueMonitor()
    : Monitor()
{

}

ChinaBondDeviatedValueCachePtr ChinaBondDeviatedValueMonitor::get(const std::string &id) const
{
    std::map<std::string, ChinaBondDeviatedValueCachePtr>::const_iterator it = chinabonddeviatedvalue_mp_.find(id);

    if (it != chinabonddeviatedvalue_mp_.end()) {
        return (*it).second;
    }
    return ChinaBondDeviatedValueCachePtr();
}

int ChinaBondDeviatedValueMonitor::size() const
{
    return chinabonddeviatedvalue_mp_.size();
}

std::map<std::string, ChinaBondDeviatedValueCachePtr>& ChinaBondDeviatedValueMonitor::refMap()
{
    return chinabonddeviatedvalue_mp_;
}

void ChinaBondDeviatedValueMonitor::load(sql::ResultSet* resSet)
{
    ChinaBondDeviatedValueCachePtr cache(new ChinaBondDeviatedValueCache());

    std::string id = resSet->getString("id");
    if (id.length() == 0) {
        return;
    }

    cache->id = resSet->getString("id");
    cache->company_id = resSet->getString("company_id");
    cache->bond_type_id = resSet->getString("bond_type_id");
    cache->bond_type_name = resSet->getString("bond_type_name");
    cache->deviated_value = resSet->getDouble("deviated_value");
    cache->status = resSet->getString("status");
    cache->modify_time = resSet->getString("modify_time");

    chinabonddeviatedvalue_mp_[id] = cache;
}