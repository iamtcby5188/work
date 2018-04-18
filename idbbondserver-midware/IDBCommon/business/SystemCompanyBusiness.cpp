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
#include "SystemCompanyBusiness.h"
#include <iostream>
#include "logger.h"
#include "cache/cache_controller_declear.h"


SystemCompanyBusiness::SystemCompanyBusiness(void)
{
}

SystemCompanyBusiness::~SystemCompanyBusiness(void)
{
}

SystemCompanyCacheVecPtr SystemCompanyBusiness::getSysCompany()
{
    SystemCompanyCacheController cc;
    dbQuery q("");
    return cc.getCacheListByQueryInThreadSafty(q);
}
