#include "tradinginstitution_monitor.h"

TradingInstitutionMonitor::TradingInstitutionMonitor()
    : Monitor()
{

}

bool TradingInstitutionMonitor::initCrm(TradingInstitutionCacheVecPtr& inst_vec)
{
	LOGGER_INFO("Load... CRM TradingInstitution" << " - Result Set: " << inst_vec->size());

	std::vector<TradingInstitutionCachePtr>::const_iterator it = inst_vec->begin();
	for (; it != inst_vec->end(); ++it)
	{
		load(*it);
	}

	LOGGER_INFO("Successfully load data... CRM TradingInstitution");

	return true;
}

TradingInstitutionCachePtr TradingInstitutionMonitor::get(const std::string &id) const
{
    std::map<std::string, TradingInstitutionCachePtr>::const_iterator it = tradinginstitution_mp_.find(id);

    if (it != tradinginstitution_mp_.end()) {
        return (*it).second;
    }
    return TradingInstitutionCachePtr();
}

int TradingInstitutionMonitor::size() const
{
    return tradinginstitution_mp_.size();
}

std::map<std::string, TradingInstitutionCachePtr>& TradingInstitutionMonitor::refMap()
{
    return tradinginstitution_mp_;
}

void TradingInstitutionMonitor::load(TradingInstitutionCachePtr cache)
{
	if (NULL == cache) return;

	std::string id = cache->id;
	if (id.length() == 0) return;

	tradinginstitution_mp_[id] = cache;
}

void TradingInstitutionMonitor::load(sql::ResultSet* resSet)
{
    TradingInstitutionCachePtr cache(new TradingInstitutionCache());

    std::string id = resSet->getString("id");
    if (id.length() == 0) {
        return;
    }

    insertCacheG(cache.get(), resSet);

    tradinginstitution_mp_[id] = cache;
}