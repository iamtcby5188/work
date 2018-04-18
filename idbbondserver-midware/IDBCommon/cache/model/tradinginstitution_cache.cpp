/*
 * tradinginstitution_cache.cpp
 *
 *  Created on: May 30, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "tradinginstitution_cache.h"

std::string TradingInstitutionCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string TradingInstitutionCache::TABLE_NAME = "bond_bank";
std::string TradingInstitutionCache::PKEY_NAME  = "id";
std::string TradingInstitutionCache::SQL_TAG = "FetchTradingInstitutionSQL";

TradingInstitutionCache::TradingInstitutionCache() {
    // TODO Auto-generated constructor stub
	status = "1";
    crm_id = -1;
}

TradingInstitutionCache::~TradingInstitutionCache() {
    // TODO Auto-generated destructor stub
}

std::string TradingInstitutionCache::getPrimaryKey() {
	return id;
}

bool TradingInstitutionCache::operator==(const TradingInstitutionCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool TradingInstitutionCache::operator!=(const TradingInstitutionCache &rhs)
{
    return !operator==(rhs);
}

REGISTER(TradingInstitutionCache);
