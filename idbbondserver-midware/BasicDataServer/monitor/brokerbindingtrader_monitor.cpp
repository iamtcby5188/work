#include "brokerbindingtrader_monitor.h"
#include "cache/cache_center.h"

BrokerBindingTraderMonitor::BrokerBindingTraderMonitor()
    : Monitor()
{

}

bool BrokerBindingTraderMonitor::initCrm(BrokerBindingTraderCacheVecPtr& broker_bind_trader_vec)
{
	LOGGER_INFO("Load... CRM BrokerBindingTrader" << " - Result Set: " << broker_bind_trader_vec->size());

	std::vector<BrokerBindingTraderCachePtr>::const_iterator it = broker_bind_trader_vec->begin();
	for (; it != broker_bind_trader_vec->end(); ++it)
	{
		load(*it);
	}

	LOGGER_INFO("Successfully load data... CRM BrokerBindingTrader");

	return true;
}

BrokerBindingTraderCachePtr BrokerBindingTraderMonitor::get(const std::string &id) const
{
    std::map<std::string, BrokerBindingTraderCachePtr>::const_iterator it = binding_mp_.find(id);

    if (it != binding_mp_.end()) {
        return (*it).second;
    }
    return BrokerBindingTraderCachePtr();
}

int BrokerBindingTraderMonitor::size() const
{
    return binding_mp_.size();
}

std::map<std::string, BrokerBindingTraderCachePtr>& BrokerBindingTraderMonitor::refMap()
{
    return binding_mp_;
}

void BrokerBindingTraderMonitor::load(BrokerBindingTraderCachePtr cache)
{
	if (NULL == cache) return;

	std::string id = cache->id;
	if (id.length() == 0) return;

	CacheCenter::sharedInstance()->AttachCurrentThread();
	cache->loadCombinationFields();
	CacheCenter::sharedInstance()->DetachCurrentThread();

	binding_mp_[id] = cache;
}


void BrokerBindingTraderMonitor::load(sql::ResultSet* resSet)
{
    BrokerBindingTraderCachePtr cache(new BrokerBindingTraderCache());

    std::string id = resSet->getString("id");
    if (id.length() == 0) {
        return;
    }

    cache->id = resSet->getString("id");
    cache->account_id = resSet->getString("account_id");
    cache->status = resSet->getString("status");
    cache->create_time = resSet->getString("create_time");
    cache->bank_agent_id = resSet->getString("bank_agent_id");
    cache->sort = resSet->getInt("sort");

	CacheCenter::sharedInstance()->AttachCurrentThread();
	cache->loadCombinationFields();
	CacheCenter::sharedInstance()->DetachCurrentThread();

    binding_mp_[id] = cache;
}