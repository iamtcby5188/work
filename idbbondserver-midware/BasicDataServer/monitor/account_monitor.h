#ifndef _ACCOUNT_MONITOR_H_
#define _ACCOUNT_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/controller/account_cachecontroller.h"

namespace sql {
    class ResultSet;
}

class AccountMonitor : public Monitor
{
public:
    AccountMonitor();

    AccountCachePtr get(const std::string &id) const;
    int size() const;

    std::map<std::string, AccountCachePtr>& refMap();

protected:
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, AccountCachePtr> account_mp_;
};


#endif // _ACCOUNT_MONITOR_H_