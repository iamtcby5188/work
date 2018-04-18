/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    InstitutionRatingMonitor 用于监听发行人评级表数据
====================================================================================================================
   Date       Name                                  Description of Change
20-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _INSTITUTIONRATING_MONITOR_H_
#define _INSTITUTIONRATING_MONITOR_H_

#include <map>
#include "monitor/monitor.h"
#include "cache/cache_controller_declear.h"

namespace sql {
    class ResultSet;
}

class InstitutionRatingMonitor : public Monitor
{
public:
    InstitutionRatingMonitor();

    InstitutionRatingCachePtr get(const std::string &id) const;
    int size() const;

    std::map<std::string, InstitutionRatingCachePtr>& refMap();

protected:
    virtual void load(sql::ResultSet* resSet);

private:
    std::map<std::string, InstitutionRatingCachePtr> institutionrating_mp_;
};


#endif