#ifndef _BROKERBINDINGTRADER_MONITOR_H_
#define _BROKERBINDINGTRADER_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"

namespace sql {
    class ResultSet;
}

class BrokerBindingTraderMonitor : public Monitor
{
public:
    BrokerBindingTraderMonitor();

	bool initCrm(BrokerBindingTraderCacheVecPtr& broker_bind_trader_vec);
    BrokerBindingTraderCachePtr get(const std::string &id) const;
    int size() const;

    std::map<std::string, BrokerBindingTraderCachePtr>& refMap();

protected:
	void load(BrokerBindingTraderCachePtr cache);
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, BrokerBindingTraderCachePtr> binding_mp_;
};


#endif // _BROKERBINDINGTRADER_MONITOR_H_