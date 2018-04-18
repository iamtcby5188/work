#ifndef _BONDTRADER_MONITOR_H_
#define _BONDTRADER_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/cache_controller_declear.h"

namespace sql {
    class ResultSet;
}

class BondTraderMonitor : public Monitor
{
public:
    BondTraderMonitor();

	bool initCrm(BondTraderCacheVecPtr& bond_trader_vec);
    BondTraderCachePtr get(const std::string &id) const;
    int size() const;

    std::map<std::string, BondTraderCachePtr>& refMap();

protected:
	void load(BondTraderCachePtr cache);
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, BondTraderCachePtr> bondtrader_mp_;
};

#endif // _BONDTRADER_MONITOR_H_