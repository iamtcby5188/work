/*
* bondgroup_cachecontroller.cpp
*
*  Created on: Jun 24, 2014
*      Author: yudan.chen
*/
#include "bondgroup_cachecontroller.h"

BondGroupCacheController* BondGroupCacheController::singleton_ = NULL;

BondGroupCacheController* BondGroupCacheController::singleton()
{
	if (singleton_ == NULL) {
		singleton_ = new BondGroupCacheController();
	}

	return singleton_;
}

BondGroupCacheController::BondGroupCacheController() 
	: SimpleCacheController<BondGroupCache, BondGroupCachePtr, BondGroupCacheVecPtr>("FetchBondGroupSQL") 
{

}

BondGroupCacheController::~BondGroupCacheController() {
    // TODO Auto-generated destructor stub
}

void BondGroupCacheController::loadCache(std::map<std::string, BondGroupCachePtr> &data_mp, sql::ResultSet* resSet)
{
	BondGroupCachePtr cache(new BondGroupCache());

	std::string id = resSet->getString("id");
	if (id.length() == 0) {
		return;
	}

    cache->id = resSet->getString("id");
    cache->company_id = resSet->getString("company_id");
	cache->ua_id = resSet->getString("ua_id");
	cache->group_name = resSet->getString("group_name");
	cache->status = resSet->getString("status");
	cache->ua_flag = resSet->getString("ua_flag");
	cache->create_time = resSet->getString("create_time");
	cache->filter_condition = resSet->getString("filter_condition");


	data_mp[id] = cache;
}

int BondGroupCacheController::insertDiskDB(BondGroupCachePtr cache)
{
	BondGroupCache* newCache = cache.get();

	INSERT_DATABASE_BEGIN(BondGroupCache::TABLE_NAME);
    INSERT_STRING_FIELD(id);
    INSERT_STRING_FIELD(company_id);
	INSERT_STRING_FIELD(ua_id);
	INSERT_STRING_FIELD(group_name);
	INSERT_STRING_FIELD(status);
	INSERT_STRING_FIELD(ua_flag);
	INSERT_STRING_FIELD(create_time);
	INSERT_STRING_FIELD(filter_condition);
	INSERT_DATABASE_END(BondGroupCache::TABLE_NAME, BondGroupCache::SCHEMA_NAME);
}

int BondGroupCacheController::updateDiskDB(BondGroupCachePtr cache_new, BondGroupCachePtr cache_old) {

	BondGroupCache* newCache = cache_new.get();
	BondGroupCache* oldCache = cache_old.get();
	UPDATE_DATABASE_BEGIN(BondGroupCache::TABLE_NAME);

    UPDATE_STRING_FIELD(id);
    UPDATE_STRING_FIELD(company_id);
	UPDATE_STRING_FIELD(ua_id);
	UPDATE_STRING_FIELD(group_name);
	UPDATE_STRING_FIELD(status);
	UPDATE_STRING_FIELD(ua_flag);
	UPDATE_STRING_FIELD(create_time);
	UPDATE_STRING_FIELD(filter_condition);

	CONDITION_STRING_FIELD(id);

	UPDATE_DATABASE_END(BondGroupCache::TABLE_NAME, BondGroupCache::SCHEMA_NAME);
}

int BondGroupCacheController::deleteDiskDB(const std::vector<std::string> &id_list)
{
	std::vector<std::string> primaryKeyList;
	for (unsigned int i=0; i<id_list.size(); ++i){
		primaryKeyList.push_back("'"+id_list[i]+"'");
	}

	DELETE_DATABASE_BEGIN(BondGroupCache::TABLE_NAME);
	DELETE_DATABASE_END(BondGroupCache::PKEY_NAME, BondGroupCache::SCHEMA_NAME);
}
