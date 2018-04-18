/*
* account_permission_cache.h
*
*  Created on: Dec 24, 2014
*      Author: huameng
*/

#include "cache_def.h"
#include "account_permission_cache.h"

std::string AccountPermissionCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string AccountPermissionCache::TABLE_NAME = "account_group_member";
std::string AccountPermissionCache::PKEY_NAME = "---";
std::string AccountPermissionCache::SQL_TAG = "FetchAccountPermissionSQL";

AccountPermissionCache::AccountPermissionCache() {
    // TODO Auto-generated constructor stub
}

std::string AccountPermissionCache::getPrimaryKey() {
	return "---";
}

AccountPermissionCache::~AccountPermissionCache() {
    // TODO Auto-generated destructor stub
}

REGISTER(AccountPermissionCache);
