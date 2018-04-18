/*
* bondgroup_cachecontroller.cpp
*
*  Created on: Jun 24, 2014
*      Author: yudan.chen
*/
#include "bondgroupdetail_cachecontroller.h"

BondGroupDetailCacheController* BondGroupDetailCacheController::singleton_ = NULL;

BondGroupDetailCacheController* BondGroupDetailCacheController::singleton()
{
	if (singleton_ == NULL) {
		singleton_ = new BondGroupDetailCacheController();
	}

	return singleton_;
}

BondGroupDetailCacheController::BondGroupDetailCacheController() 
	: SimpleCacheController<BondGroupDetailCache, BondGroupDetailCachePtr, BondGroupDetailCacheVecPtr>("FetchBondGroupDetailSQL") 
{

}

BondGroupDetailCacheController::~BondGroupDetailCacheController() {
    // TODO Auto-generated destructor stub
}

void BondGroupDetailCacheController::loadCache(std::map<std::string, BondGroupDetailCachePtr> &data_mp, sql::ResultSet* resSet)
{
	BondGroupDetailCachePtr cache(new BondGroupDetailCache());

	std::string id = resSet->getString("id");
	if (id.length() == 0) {
		return;
	}

	cache->id = resSet->getString("id");
	cache->group_id = resSet->getString("group_id");
	cache->bond_key_listed_market = resSet->getString("bond_key_listed_market");
	cache->create_time = resSet->getString("create_time");

	data_mp[id] = cache;
}

int BondGroupDetailCacheController::insertDiskDB(BondGroupDetailCachePtr cache)
{
	BondGroupDetailCache* newCache = cache.get();

	INSERT_DATABASE_BEGIN(BondGroupDetailCache::TABLE_NAME);
	INSERT_STRING_FIELD(id);
	INSERT_STRING_FIELD(group_id);
	INSERT_STRING_FIELD(bond_key_listed_market);
	INSERT_STRING_FIELD(create_time);
	INSERT_DATABASE_END(BondGroupDetailCache::TABLE_NAME, BondGroupDetailCache::SCHEMA_NAME);
}

int BondGroupDetailCacheController::updateDiskDB(BondGroupDetailCachePtr cache_new, BondGroupDetailCachePtr cache_old) {

	BondGroupDetailCache* newCache = cache_new.get();
	BondGroupDetailCache* oldCache = cache_old.get();
	UPDATE_DATABASE_BEGIN(BondGroupDetailCache::TABLE_NAME);

	UPDATE_STRING_FIELD(group_id);
	UPDATE_STRING_FIELD(bond_key_listed_market);
	UPDATE_STRING_FIELD(create_time);

	CONDITION_STRING_FIELD(id);

	UPDATE_DATABASE_END(BondGroupDetailCache::TABLE_NAME, BondGroupDetailCache::SCHEMA_NAME);
}

int BondGroupDetailCacheController::deleteDiskDB(const std::vector<std::string> &id_list)
{
	std::vector<std::string> primaryKeyList;
	for (unsigned int i=0; i<id_list.size(); ++i){
		primaryKeyList.push_back("'"+id_list[i]+"'");
	}
	
	DELETE_DATABASE_BEGIN(BondGroupDetailCache::TABLE_NAME);				
	DELETE_DATABASE_END(BondGroupDetailCache::PKEY_NAME, BondGroupDetailCache::SCHEMA_NAME);
}

bool BondGroupDetailCacheController::deleteCacheByGroupId(const std::string &group_id)
{
	std::vector<std::string> del_list;
	std::map<std::string, BondGroupDetailCachePtr>::const_iterator iter_new = data_mp_.begin();
    for (; iter_new != data_mp_.end(); ++iter_new) {
		std::string id = (*iter_new).first;
		BondGroupDetailCachePtr cache_new = (*iter_new).second;

		if (cache_new == NULL) {
			continue;
		}

		if (cache_new->group_id == group_id){
			del_list.push_back(id);
		}
	}
	return deleteCache(del_list);
}

BondKeyListedMarketVecPtr BondGroupDetailCacheController::getBondByGroupId(const std::string &group_id)
{
	BondKeyListedMarketVecPtr vec(new std::vector<std::string>());
	
	std::map<std::string, BondGroupDetailCachePtr>::const_iterator iter_new = data_mp_.begin();
    for (; iter_new != data_mp_.end(); ++iter_new) {
		std::string id = (*iter_new).first;
		BondGroupDetailCachePtr cache_new = (*iter_new).second;

		if (cache_new == NULL) {
			continue;
		}

		if (cache_new->group_id == group_id){
			vec->push_back(cache_new->bond_key_listed_market);
		}
	}
	return vec;
}