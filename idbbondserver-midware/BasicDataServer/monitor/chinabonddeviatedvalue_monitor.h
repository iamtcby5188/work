#ifndef _CHINABONDDEVIATEDVALUE_MONITOR_H_
#define _CHINABONDDEVIATEDVALUE_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/cache_controller_declear.h"

namespace sql {
    class ResultSet;
}

class ChinaBondDeviatedValueMonitor : public Monitor
{
public:
    ChinaBondDeviatedValueMonitor();

    ChinaBondDeviatedValueCachePtr get(const std::string &id) const;
    int size() const;

    std::map<std::string, ChinaBondDeviatedValueCachePtr>& refMap();

protected:
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, ChinaBondDeviatedValueCachePtr> chinabonddeviatedvalue_mp_;
};


#endif // _CHINABONDDEVIATEDVALUE_MONITOR_H_