#ifndef _BONDSMALLVIEW_MONITOR_H_
#define _BONDSMALLVIEW_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/controller/bondsmallview_cachecontroller.h"

namespace sql {
    class ResultSet;
}

class BondSmallViewMonitor : public Monitor
{
public:
    BondSmallViewMonitor();

    BondSmallViewCachePtr get(const std::string &bond_key_listed_market) const;
    int size() const;

    std::map<std::string, BondSmallViewCachePtr>& refMap();

protected:
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, BondSmallViewCachePtr> bond_small_view_mp_;
};

#endif // _BONDSMALLVIEW_MONITOR_H_