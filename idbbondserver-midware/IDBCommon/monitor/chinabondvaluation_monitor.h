#ifndef _CHINABONDVALUATION_MONITOR_H_
#define _CHINABONDVALUATION_MONITOR_H_

#include <map>
#include "monitor.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"

namespace sql {
    class ResultSet;
}

class ChinaBondValuationMonitor : public Monitor
{
public:
    ChinaBondValuationMonitor();

    ChinaBondValuationCachePtr get(const std::string &id) const;
    int size() const;

    std::map<std::string, ChinaBondValuationCachePtr>& refMap();

protected:
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, ChinaBondValuationCachePtr> chinabondvaluation_mp_;
};


#endif // _CHINABONDVALUATION_MONITOR_H_