#ifndef _DCSBONDDEAL_MONITOR_H_
#define _DCSBONDDEAL_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/controller/dcsbonddeal_cachecontroller.h"

namespace sql {
    class ResultSet;
}

class DCSBondDealMonitor : public Monitor
{
public:
	DCSBondDealMonitor();

	bool init();
    DcsBondDealCachePtr get(const std::string &bond_key_listed_market) const;
    int size() const;

    std::map<std::string, DcsBondDealCachePtr>& refMap();

protected:
	void load(DcsBondDealCachePtr cache);
	void load(sql::ResultSet* resSet) {};

private:
    std::map<std::string, DcsBondDealCachePtr> bond_deal_mp_;
};

#endif // _DCSBONDDEAL_MONITOR_H_