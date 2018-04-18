/*
 * BondTraderCache.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "bondtrader_cache.h"

std::string BondTraderCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BondTraderCache::TABLE_NAME = "bond_bank_agent";
std::string BondTraderCache::PKEY_NAME = "id";
std::string BondTraderCache::SQL_TAG = "FetchBondTraderSQL";

BondTraderCache::BondTraderCache() {
    // TODO Auto-generated constructor stub
	status = "1";
    flag_vip = flag_bad = false;
    crm_id = -1;
}

BondTraderCache::~BondTraderCache() {
    // TODO Auto-generated destructor stub
}

std::string BondTraderCache::getPrimaryKey() {
	return id;
}

bool BondTraderCache::operator==(const BondTraderCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool BondTraderCache::operator!=(const BondTraderCache &rhs)
{
    return !operator==(rhs);
}

REGISTER(BondTraderCache);
