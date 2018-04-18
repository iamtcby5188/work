/*
 * bondgroupdetail_cache.cpp
 *
 *  Created on: May 24, 2014
 *      Author: yudan.chen
 */

#include "bondgroupdetail_cache.h"

std::string BondGroupDetailCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BondGroupDetailCache::TABLE_NAME = "bond_group_relation";
std::string BondGroupDetailCache::PKEY_NAME = "id";

BondGroupDetailCache::BondGroupDetailCache() {
    // TODO Auto-generated constructor stub

}

BondGroupDetailCache::~BondGroupDetailCache() {
    // TODO Auto-generated destructor stub
}

std::string BondGroupDetailCache::getPrimaryKey() {
	return id;
}

bool BondGroupDetailCache::operator==(const BondGroupDetailCache &rhs)
{
    return id == rhs.id && 
        group_id == rhs.group_id &&
        bond_key_listed_market == rhs.bond_key_listed_market;
}

bool BondGroupDetailCache::operator!=(const BondGroupDetailCache &rhs)
{
	return !operator==(rhs);
}