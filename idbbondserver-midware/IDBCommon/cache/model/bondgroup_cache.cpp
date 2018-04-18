/*
 * bondgroup_cache.cpp
 *
 *  Created on: May 24, 2014
 *      Author: yudan.chen
 */

#include "bondgroup_cache.h"

std::string BondGroupCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BondGroupCache::TABLE_NAME = "bond_goods_group";
std::string BondGroupCache::PKEY_NAME = "id";

BondGroupCache::BondGroupCache() {
    // TODO Auto-generated constructor stub
	ua_flag = "2"; 
	status = "1";
}

BondGroupCache::~BondGroupCache() {
    // TODO Auto-generated destructor stub
}

std::string BondGroupCache::getPrimaryKey() {
	return id;
}

bool BondGroupCache::operator==(const BondGroupCache &rhs)
{
    return id == rhs.id && 
        company_id == rhs.company_id &&
        ua_id == rhs.ua_id &&
        group_name == rhs.group_name &&
        status == rhs.status &&
        ua_flag == rhs.ua_flag &&
        filter_condition == rhs.filter_condition;
}

bool BondGroupCache::operator!=(const BondGroupCache &rhs)
{
	return !operator==(rhs);
}