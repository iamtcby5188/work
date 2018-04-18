/*
 * constants_cache.cpp
 *
 *  Created on: August 13, 2014
 *      Author: yudan.chen
 */

#include "cache_def.h"
#include "chinabonddeviatedvalue_cache.h"

std::string ChinaBondDeviatedValueCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string ChinaBondDeviatedValueCache::TABLE_NAME = "china_bond_deviated_value";
std::string ChinaBondDeviatedValueCache::PKEY_NAME = "id";
std::string ChinaBondDeviatedValueCache::SQL_TAG = "FetchChinaBondDeviatedValueSQL";

ChinaBondDeviatedValueCache::ChinaBondDeviatedValueCache() {
	id = company_id = bond_type_id = bond_type_name = "";
	deviated_value = DOUBLE_NULL;
    status = "1";
	modify_time = "";
}

ChinaBondDeviatedValueCache::~ChinaBondDeviatedValueCache() {
    // TODO Auto-generated destructor stub
}

std::string ChinaBondDeviatedValueCache::getPrimaryKey() {
    return id;
}

bool ChinaBondDeviatedValueCache::operator==(const ChinaBondDeviatedValueCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool ChinaBondDeviatedValueCache::operator!=(const ChinaBondDeviatedValueCache &rhs)
{
    return !operator==(rhs);
}

REGISTER(ChinaBondDeviatedValueCache);
