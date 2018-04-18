/*
 * BondTypeSelective_cache.cpp
 *
 *  Created on: May 30, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "bondtypeselective_cache.h"

std::string BondTypeSelectiveCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BondTypeSelectiveCache::TABLE_NAME = "bond_goods_type";
std::string BondTypeSelectiveCache::PKEY_NAME  = "id";
std::string BondTypeSelectiveCache::SQL_TAG = "FetchBondTypeSelectiveSQL";

BondTypeSelectiveCache::BondTypeSelectiveCache() {
    // TODO Auto-generated constructor stub
}

BondTypeSelectiveCache::~BondTypeSelectiveCache() {
    // TODO Auto-generated destructor stub
}

std::string BondTypeSelectiveCache::getPrimaryKey() {
	return id;
}

REGISTER(BondTypeSelectiveCache);
