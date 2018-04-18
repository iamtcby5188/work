/*
 * ChinaBondValuationCache.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "chinabondvaluation_cache.h"

std::string ChinaBondValuationCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string ChinaBondValuationCache::TABLE_NAME = "china_bond_valuation";  
std::string ChinaBondValuationCache::PKEY_NAME = "id";
std::string ChinaBondValuationCache::SQL_TAG = "FetchChinaBondValuationSQL";

ChinaBondValuationCache::ChinaBondValuationCache() {
    // TODO Auto-generated constructor stub
	status = "0";
    val_modified_duration = val_convexity = val_basis_point_value = DECIMAL_NULL;
    rowNum = -1;
}

ChinaBondValuationCache::~ChinaBondValuationCache() {
    // TODO Auto-generated destructor stub
}

std::string ChinaBondValuationCache::getPrimaryKey() {
	return id;
}

bool ChinaBondValuationCache::operator==(const ChinaBondValuationCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool ChinaBondValuationCache::operator!=(const ChinaBondValuationCache &rhs)
{
    return !operator==(rhs);
}

REGISTER(ChinaBondValuationCache);

