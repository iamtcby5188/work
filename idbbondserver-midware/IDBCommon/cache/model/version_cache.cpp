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
#include "version_cache.h"

std::string VersionCache::SCHEMA_NAME = "";
std::string VersionCache::TABLE_NAME = "";
std::string VersionCache::PKEY_NAME = "methodcompany";
std::string VersionCache::SQL_TAG = "";

VersionCache::VersionCache() {
    // TODO Auto-generated constructor stub
    version = -1;
}

std::string VersionCache::getPrimaryKey() {
	return methodcompany;
}

VersionCache::~VersionCache() {
    // TODO Auto-generated destructor stub
}


bool VersionCache::operator==(const VersionCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool VersionCache::operator!=(const VersionCache &rhs)
{
    return !operator==(rhs);
}


REGISTER(VersionCache);
