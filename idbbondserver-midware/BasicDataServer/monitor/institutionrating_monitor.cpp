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
#include "institutionrating_monitor.h"
#include "cache/cache_center.h"

InstitutionRatingMonitor::InstitutionRatingMonitor()
    : Monitor()
{

}

InstitutionRatingCachePtr InstitutionRatingMonitor::get(const std::string &id) const
{
    std::map<std::string, InstitutionRatingCachePtr>::const_iterator it = institutionrating_mp_.find(id);

    if (it != institutionrating_mp_.end()) {
        return (*it).second;
    }
    return InstitutionRatingCachePtr();
}

int InstitutionRatingMonitor::size() const
{
    return institutionrating_mp_.size();
}

std::map<std::string, InstitutionRatingCachePtr>& InstitutionRatingMonitor::refMap()
{
    return institutionrating_mp_;
}

void InstitutionRatingMonitor::load(sql::ResultSet* resSet)
{
    InstitutionRatingCachePtr cache(new InstitutionRatingCache());

    std::string id = resSet->getString("id");
    if (id.length() == 0) {
        return;
    }

    cache->id = resSet->getString("id");
    cache->delflag = resSet->getString("delflag");
    cache->sts = resSet->getString("sts");
    cache->institution_code = resSet->getString("institution_code");
    cache->institution_type = resSet->getString("institution_type");
    cache->rating_institution_code = resSet->getString("rating_institution_code");
	cache->rating_date = resSet->getInt("rating_date");
	cache->rate = resSet->getString("rate");
	cache->outlook = resSet->getString("outlook");

    institutionrating_mp_[id] = cache;
}