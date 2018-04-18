/*
 * logintoken_cache.h
 *
 *  Created on: Jan 21, 2015
 *      Author: yudan.chen
 */

#ifndef LOGIN_TOKEN_CACHE_H_
#define LOGIN_TOKEN_CACHE_H_

#include "cache_def.h"

class LoginTokenCache;
typedef std::tr1::shared_ptr<LoginTokenCache> LoginTokenCachePtr;
typedef std::tr1::shared_ptr<std::vector<LoginTokenCachePtr> > LoginTokenCacheVecPtr;

class LoginTokenCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
public:
    LoginTokenCache();
    virtual ~LoginTokenCache();
	std::string getPrimaryKey();

	bool operator ==(const LoginTokenCache &rhs);
	bool operator !=(const LoginTokenCache &rhs);
public:
    std::string id;   
    std::string account_id;
    std::string token;
	std::string activation;
	std::string client_type;
	std::string client_version;
	std::string ip;
	std::string mac;
	std::string server;
	std::string status;
	std::string last_update_time;   //数据库中没有该字段，用于判断离线用
};

#endif /* LOGIN_TOKEN_CACHE_H_ */