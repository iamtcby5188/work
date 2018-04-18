#ifndef _TRADINGINSTITUTION_MONITOR_H_
#define _TRADINGINSTITUTION_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/cache_controller_declear.h"

namespace sql {
    class ResultSet;
}

class TradingInstitutionMonitor : public Monitor
{
public:
    TradingInstitutionMonitor();

	bool initCrm(TradingInstitutionCacheVecPtr& inst_vec);
    TradingInstitutionCachePtr get(const std::string &id) const;
    int size() const;

    std::map<std::string, TradingInstitutionCachePtr>& refMap();

protected:
	void load(TradingInstitutionCachePtr cache);
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, TradingInstitutionCachePtr> tradinginstitution_mp_;
};

#endif // _TRADINGINSTITUTION_MONITOR_H_