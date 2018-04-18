/*
 * logintoken_cache.h
 *
 *  Created on: Jan 21, 2015
 *      Author: yudan.chen
 */

#include "logintoken_cache.h"

std::string LoginTokenCache::SCHEMA_NAME = SCHEMA_IDB_ACCOUNT;
std::string LoginTokenCache::TABLE_NAME = "idb_login_status";
std::string LoginTokenCache::PKEY_NAME = "id";

LoginTokenCache::LoginTokenCache() {
}

LoginTokenCache::~LoginTokenCache() {
}

std::string LoginTokenCache::getPrimaryKey() {
	return id;
}

bool LoginTokenCache::operator==(const LoginTokenCache &rhs)
{
    return id == rhs.id && 
        account_id == rhs.account_id &&
        token == rhs.token &&
        activation == rhs.activation &&
        client_type == rhs.client_type &&
        ip == rhs.ip &&
		mac == rhs.mac && 
        server == rhs.server &&
        status == rhs.status;
}

bool LoginTokenCache::operator!=(const LoginTokenCache &rhs)
{
	return !operator==(rhs);
}