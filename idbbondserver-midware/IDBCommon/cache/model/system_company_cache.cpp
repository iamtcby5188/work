/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    SystemCompanyCache 提供访问Company数据缓存的接口
====================================================================================================================
   Date       Name                                  Description of Change
14-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#include "cache_def.h"
#include "system_company_cache.h"

std::string SystemCompanyCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string SystemCompanyCache::TABLE_NAME = "sys_company";
std::string SystemCompanyCache::PKEY_NAME = "id";
std::string SystemCompanyCache::SQL_TAG = "FetchSystemCompanySQL";

SystemCompanyCache::SystemCompanyCache() {
    // TODO Auto-generated constructor stub
    status = "1";
}

std::string SystemCompanyCache::getPrimaryKey() {
	return id;
}

SystemCompanyCache::~SystemCompanyCache() {
    // TODO Auto-generated destructor stub
}


bool SystemCompanyCache::operator==(const SystemCompanyCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool SystemCompanyCache::operator!=(const SystemCompanyCache &rhs)
{
    return !operator==(rhs);
}


REGISTER(SystemCompanyCache);
