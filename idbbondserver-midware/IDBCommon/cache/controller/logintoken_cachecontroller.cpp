/*
* bondgroup_cachecontroller.cpp
*
*  Created on: Jun 24, 2014
*      Author: yudan.chen
*/
#include "logintoken_cachecontroller.h"

LoginTokenCacheController* LoginTokenCacheController::singleton_ = NULL;

LoginTokenCacheController* LoginTokenCacheController::singleton()
{
	if (singleton_ == NULL) {
		singleton_ = new LoginTokenCacheController();
	}

	return singleton_;
}

LoginTokenCacheController::LoginTokenCacheController() 
	: SimpleCacheController<LoginTokenCache, LoginTokenCachePtr, LoginTokenCacheVecPtr>("FetchLoginTokenSQL") 
{

}

LoginTokenCacheController::~LoginTokenCacheController() {
    // TODO Auto-generated destructor stub
}

void LoginTokenCacheController::loadCache(std::map<std::string, LoginTokenCachePtr> &data_mp, sql::ResultSet* resSet)
{
	LoginTokenCachePtr cache(new LoginTokenCache());

	std::string id = resSet->getString("ID");
	if (id.length() == 0) {
		return;
	}

	cache->id = resSet->getString("ID");
	cache->account_id = resSet->getString("ACCOUNT_ID");
	cache->token = resSet->getString("TOKEN");
	cache->activation = resSet->getString("ACTIVATION");
	cache->client_type = resSet->getString("CLIENT_TYPE");
	cache->ip = resSet->getString("IP");
	cache->server = resSet->getString("SERVER");
	cache->status = resSet->getString("STATUS");
	cache->last_update_time = GetCurrentTimeString();

	data_mp[id] = cache;
}

int LoginTokenCacheController::insertDiskDB(LoginTokenCachePtr cache)
{
	LoginTokenCache* newCache = cache.get();

	INSERT_DATABASE_BEGIN(LoginTokenCache::TABLE_NAME);
	INSERT_STRING_FIELD(id);
	INSERT_STRING_FIELD(account_id);
	INSERT_STRING_FIELD(token);
	INSERT_STRING_FIELD(activation);
	INSERT_STRING_FIELD(client_type);
	INSERT_STRING_FIELD(ip);
	INSERT_STRING_FIELD(server);
	INSERT_STRING_FIELD(status);
	INSERT_DATABASE_END(LoginTokenCache::TABLE_NAME, LoginTokenCache::SCHEMA_NAME);
}

int LoginTokenCacheController::updateDiskDB(LoginTokenCachePtr cache_new, LoginTokenCachePtr cache_old) {

	LoginTokenCache* newCache = cache_new.get();
	LoginTokenCache* oldCache = cache_old.get();
	UPDATE_DATABASE_BEGIN(LoginTokenCache::TABLE_NAME);

	UPDATE_STRING_FIELD(id);
	UPDATE_STRING_FIELD(account_id);
	UPDATE_STRING_FIELD(token);
	UPDATE_STRING_FIELD(activation);
	UPDATE_STRING_FIELD(client_type);
	UPDATE_STRING_FIELD(ip);
	UPDATE_STRING_FIELD(server);
	UPDATE_STRING_FIELD(status);

	CONDITION_STRING_FIELD(id);

	UPDATE_DATABASE_END(LoginTokenCache::TABLE_NAME, LoginTokenCache::SCHEMA_NAME);
}

int LoginTokenCacheController::deleteDiskDB(const std::vector<std::string> &id_list)
{
	std::vector<std::string> primaryKeyList;
	for (unsigned int i=0; i<id_list.size(); ++i){
		primaryKeyList.push_back("'"+id_list[i]+"'");
	}

	DELETE_DATABASE_BEGIN(LoginTokenCache::TABLE_NAME);
	DELETE_DATABASE_END(LoginTokenCache::PKEY_NAME, LoginTokenCache::SCHEMA_NAME);
}