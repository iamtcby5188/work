/*
* hotkey_cachecontroller.cpp
*
*  Created on: August 18, 2014
*      Author: yudan.chen
*/

#include "hotkey_cachecontroller.h"

HotKeyCacheController* HotKeyCacheController::singleton_ = NULL;

HotKeyCacheController* HotKeyCacheController::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new HotKeyCacheController();
    }

    return singleton_;
}

HotKeyCacheController::HotKeyCacheController() 
    : SimpleCacheController<HotKeyCache, HotKeyCachePtr, HotKeyCacheVecPtr>("FetchHotKeySQL") 
{

}

HotKeyCacheController::~HotKeyCacheController() {
    // TODO Auto-generated destructor stub
}

void HotKeyCacheController::loadCache(std::map<std::string, HotKeyCachePtr> &data_mp, sql::ResultSet* resSet)
{
    HotKeyCachePtr cache(new HotKeyCache());

    std::string id = resSet->getString("Id");
    if (id.length() == 0) {
        return;
    }

    cache->Id = resSet->getString("Id");
    cache->HotKey = resSet->getString("HotKey");
    cache->FunctionName = resSet->getString("FunctionName");
    cache->Description = resSet->getString("Description");
    cache->Function = resSet->getString("Function");
    cache->Params = resSet->getString("Params");
    cache->ParamSize = resSet->getInt("ParamSize");

    data_mp[id] = cache;
}

int HotKeyCacheController::insertDiskDB(HotKeyCachePtr cache)
{
    HotKeyCache* newCache = cache.get();

    INSERT_DATABASE_BEGIN(HotKeyCache::TABLE_NAME);
    INSERT_STRING_FIELD(Id);
    INSERT_STRING_FIELD(HotKey);
    INSERT_STRING_FIELD(FunctionName);
    INSERT_STRING_FIELD(Description);
    INSERT_STRING_FIELD(Function);
    INSERT_STRING_FIELD(Params);
    INSERT_INT_FIELD(ParamSize);
    INSERT_DATABASE_END(HotKeyCache::TABLE_NAME, HotKeyCache::SCHEMA_NAME);
}

int HotKeyCacheController::updateDiskDB(HotKeyCachePtr cache_new, HotKeyCachePtr cache_old) {

    HotKeyCache* newCache = cache_new.get();
    HotKeyCache* oldCache = cache_old.get();
    UPDATE_DATABASE_BEGIN(HotKeyCache::TABLE_NAME);

    UPDATE_STRING_FIELD(Id);
    UPDATE_STRING_FIELD(HotKey);
    UPDATE_STRING_FIELD(FunctionName);
    UPDATE_STRING_FIELD(Description);
    UPDATE_STRING_FIELD(Function);
    UPDATE_STRING_FIELD(Params);
    UPDATE_INT_FIELD(ParamSize);
    CONDITION_STRING_FIELD(Id);
    
    UPDATE_DATABASE_END(HotKeyCache::TABLE_NAME, HotKeyCache::SCHEMA_NAME);
}

int HotKeyCacheController::deleteDiskDB(const std::vector<std::string> &id_list)
{
	std::vector<std::string> primaryKeyList;
	for (int i=0; i<id_list.size(); i++){
		primaryKeyList.push_back("'"+id_list[i]+"'");
	}

    DELETE_DATABASE_BEGIN(HotKeyCache::TABLE_NAME);
    DELETE_DATABASE_END(HotKeyCache::PKEY_NAME, HotKeyCache::SCHEMA_NAME);
}