/*
 * SelectiveInfoCache.cpp
 *
 *  Created on: May 30, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "selectiveinfo_cache.h"

std::string SelectiveInfoCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string SelectiveInfoCache::TABLE_NAME = "selective_info";
std::string SelectiveInfoCache::PKEY_NAME = "id";
std::string SelectiveInfoCache::SQL_TAG = "FetchSelectiveInfoSQL";

SelectiveInfoCache::SelectiveInfoCache() {
    // TODO Auto-generated constructor stub
	delflag = "0";
}

SelectiveInfoCache::~SelectiveInfoCache() {
    // TODO Auto-generated destructor stub
}

std::string SelectiveInfoCache::getPrimaryKey() {
	return id;
}

REGISTER(SelectiveInfoCache);




