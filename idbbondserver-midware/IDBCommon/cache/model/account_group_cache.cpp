									/*
 * account_cache.cpp
 *
 *  Created on: July 08, 2014
 *      Author: theochen
 */
#include "cache_def.h"
#include "account_group_cache.h"

std::string AccountGroupCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string AccountGroupCache::TABLE_NAME = "account_group_member";
std::string AccountGroupCache::PKEY_NAME = "---";
std::string AccountGroupCache::SQL_TAG = "FetchAccountGroupSQL";

AccountGroupCache::AccountGroupCache() {
    // TODO Auto-generated constructor stub
}

std::string AccountGroupCache::getPrimaryKey() {
	return "---";
}

AccountGroupCache::~AccountGroupCache() {
    // TODO Auto-generated destructor stub
}

REGISTER(AccountGroupCache);
