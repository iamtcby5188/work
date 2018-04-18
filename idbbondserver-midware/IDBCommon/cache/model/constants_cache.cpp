/*
 * constants_cache.cpp
 *
 *  Created on: August 13, 2014
 *      Author: yudan.chen
 */

#include "cache_def.h"
#include "constants_cache.h"

std::string ConstantsCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string ConstantsCache::TABLE_NAME = "constants";
std::string ConstantsCache::PKEY_NAME = "id";
std::string ConstantsCache::SQL_TAG = "FetchConstantsSQL";

ConstantsCache::ConstantsCache() {
	id = name = "";
	//value = DOUBLE_NULL;
    status = "1";
	create_time = constants_key = "";
}

ConstantsCache::~ConstantsCache() {
    // TODO Auto-generated destructor stub
}

std::string ConstantsCache::getPrimaryKey() {
    return id;
}

REGISTER(ConstantsCache);
