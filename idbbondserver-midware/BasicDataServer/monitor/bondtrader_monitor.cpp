#include "bondtrader_monitor.h"

BondTraderMonitor::BondTraderMonitor()
    : Monitor()
{

}

bool BondTraderMonitor::initCrm(BondTraderCacheVecPtr& bond_trader_vec)
{
	LOGGER_INFO("Load... CRM BondTrader" << " - Result Set: " << bond_trader_vec->size());

	std::vector<BondTraderCachePtr>::const_iterator it = bond_trader_vec->begin();
	for (; it != bond_trader_vec->end(); ++it)
	{
		load(*it);
	}

	LOGGER_INFO("Successfully load data... CRM BondTrader");

	return true;
}

BondTraderCachePtr BondTraderMonitor::get(const std::string &id) const
{
    std::map<std::string, BondTraderCachePtr>::const_iterator it = bondtrader_mp_.find(id);

    if (it != bondtrader_mp_.end()) {
        return (*it).second;
    }
    return BondTraderCachePtr();
}

int BondTraderMonitor::size() const
{
    return bondtrader_mp_.size();
}

std::map<std::string, BondTraderCachePtr>& BondTraderMonitor::refMap()
{
    return bondtrader_mp_;
}

void BondTraderMonitor::load(BondTraderCachePtr cache)
{
	if (NULL == cache) return;

	std::string id = cache->id;
	if (id.length() == 0) return;

	bondtrader_mp_[id] = cache;
}

void BondTraderMonitor::load(sql::ResultSet* resSet)
{
    BondTraderCachePtr cache(new BondTraderCache());

    std::string id = resSet->getString("id");
    if (id.length() == 0) {
        return;
    }

    cache->id = resSet->getString("id");
    cache->company_id = resSet->getString("company_id");
    cache->name = resSet->getString("name");
    cache->code = resSet->getString("code");
    cache->f_code = resSet->getString("f_code");
    cache->attribute = resSet->getString("attribute");
    cache->attribute1 = resSet->getString("attribute1");
    cache->attribute2 = resSet->getString("attribute2");
    cache->attribute3 = resSet->getString("attribute3");
    cache->attribute4 = resSet->getString("attribute4");
    cache->attribute5 = resSet->getString("attribute5");
    cache->create_time = resSet->getString("create_time");
    cache->status = resSet->getString("status");
    cache->flag = resSet->getString("flag");
    cache->flag_vip = resSet->getBoolean("flag_vip");
    cache->flag_bad = resSet->getBoolean("flag_bad");
    cache->flag_bargain = resSet->getString("flag_bargain");
    cache->bank_id = resSet->getString("bank_id");
    cache->customer_id = resSet->getString("customer_id");
    cache->pinyin = resSet->getString("pinyin");
    cache->pinyin_full = resSet->getString("pinyin_full");
	cache->modify_time = resSet->getString("modify_time");

    bondtrader_mp_[id] = cache;
}