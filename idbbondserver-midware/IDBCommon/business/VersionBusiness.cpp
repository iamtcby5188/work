/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    BaseBusiness 提供获取当前用户的CompanyId接口
====================================================================================================================
   Date       Name                                  Description of Change
05-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#include "VersionBusiness.h"
#include <iostream>
#include "logger.h"
#include "cache/cache_controller_declear.h"


VersionBusiness::VersionBusiness(void)
{
}

VersionBusiness::~VersionBusiness(void)
{
}

void VersionBusiness::InitVersion(const std::string& method, const std::string& company)
{
    VersionCache vc;
    vc.methodcompany = method + "." + company;
    vc.version = 0;
    VersionCacheController vcc;

    vcc.insertCacheInThreadSaftyWithoutDBSync(&vc);
}

int VersionBusiness::GetVersion(const std::string& method, const std::string& company_id)
{
    VersionCacheController vcc;
    std::string sql = "methodcompany = '" + method + "." + company_id + "'";
    dbQuery q(sql.c_str());
    VersionCachePtr vcp =  vcc.getCacheByQueryInThreadSafty(q);
    if(vcp.get() != NULL)
        return vcp->version;

    return 0;
}

void VersionBusiness::SetVersion(const std::string& method, const std::string& company, const int& version) {
    std::string methodcompany = method + "." + company;
    VersionCacheController vcc;
    std::string sql = "methodcompany = '" + methodcompany + "'";
    dbQuery q(sql.c_str());
    VersionCache cache;
    cache.methodcompany = methodcompany;
    cache.version = version;
    vcc.updateCacheInThreadSaftyWithoutDBSync(q, &cache);
}

void VersionBusiness::IncOneAndGetVersion(const std::string& method, const std::string& company, int &version)
{
    std::string methodcompany = method + "." + company;
    VersionCacheController vcc;
    std::string sql = "methodcompany = '" + method + "." + company + "'";
    dbQuery q(sql.c_str());
    vcc.updateCacheByQueryInThreadSaftyWithoutDBSync(q, UpdateVersionCallBack, &version, NULL, NULL, NULL);
}

void VersionBusiness::UpdateVersionCallBack(VersionCache* cache, void* param)
{
    int *version = (int*)param;
    cache->version++;
    *version = cache->version;
}
