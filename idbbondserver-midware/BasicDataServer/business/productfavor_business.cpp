#include "productfavor_business.h"
#include "include/common.h"
#include "cache/controller/bondgroup_cachecontroller.h"
#include "cache/controller/bondgroupdetail_cachecontroller.h"

ProductFavorBusiness::ProductFavorBusiness(ProductFavorService* service)
    : service_(service)
{
}

BondGroupCacheVecPtr ProductFavorBusiness::GetBondGroupList()
{
	return BondGroupCacheController::singleton()->getVec();
}

BondGroupDetailCacheVecPtr ProductFavorBusiness::GetBondGroupDetailList()
{
	return BondGroupDetailCacheController::singleton()->getVec();
}

bool ProductFavorBusiness::AddGroup(const BondGroupCachePtr &cache)
{
	std::vector<BondGroupCachePtr> bond_group_vec;

	bool succeed = BondGroupCacheController::singleton()->insertCache(cache->id, cache);
	if (succeed) {
		bond_group_vec.push_back(cache);
		LOGGER_INFO("AddBondGroup: Success for add BondGroupCache: " << cache->id)
        service_->PublishBondGroup(bond_group_vec, "AddList");
	}else{
		LOGGER_ERROR("AddBondGroup: Failed for add BondGroupCache: " << cache->id)
	}

    return succeed;
}

void ProductFavorBusiness::UpdateGroup(const BondGroupCachePtr &cache)
{
	bool succeed = BondGroupCacheController::singleton()->updateCache(cache->id, cache);
    if (succeed) {
		std::vector<BondGroupCachePtr> bond_group_vec;
		bond_group_vec.push_back(cache);
        service_->PublishBondGroup(bond_group_vec, "UpdateList");
		LOGGER_INFO("UpdateBondGroup: Success for updating BondGroupCache: " << cache->id)
    }else{
		LOGGER_ERROR("UpdateBondGroup: Failed for updating BondGroupCache: " << cache->id)
	}
}

void ProductFavorBusiness::DeleteGroup(const BondGroupCachePtr &cache)
{
	//删除已选择产品
	bool succeed = BondGroupDetailCacheController::singleton()->deleteCacheByGroupId(cache->id);
	if (succeed) {
		LOGGER_INFO("DeleteBondGroupDetail: Success for deleting selected Product: " << cache->id)
	}else{
		LOGGER_ERROR("DeleteBondGroupDetail: Failed for deleting selected Product: " << cache->id)
	}
	//状态置2
	succeed = BondGroupCacheController::singleton()->updateCache(cache->id, cache);
	if (succeed){
		LOGGER_INFO("DeleteBondGroup: Success for update status: " << cache->id)
		std::vector<std::string> del_list;
		del_list.push_back(cache->id);
		BondGroupCacheController::singleton()->deleteCacheWithoutDBSync(del_list);

		std::vector<BondGroupCachePtr> bond_group_vec;
		bond_group_vec.push_back(cache);
		service_->PublishBondGroup(bond_group_vec, "DeleteList");
	}else{
		LOGGER_INFO("DeleteBondGroup: Failed for update status: " << cache->id)
	}
}

bool ProductFavorBusiness::AddAttentionBonds(const BondGroupCachePtr &cache, sdbus::Variant::List &bondkeylistmarkets)
{
    //step1: delete old relation
	bool succeed = BondGroupDetailCacheController::singleton()->deleteCacheByGroupId(cache->id);
	if (succeed) {
		LOGGER_INFO("DeleteBondGroupDetail: Success for delete old relation: " << cache->id)

		std::vector<BondGroupDetailCachePtr> bond_group_detail_vec;
		BondGroupDetailCachePtr bondgroupdetailcache(new BondGroupDetailCache());
		bondgroupdetailcache->group_id = cache->id;
		bond_group_detail_vec.push_back(bondgroupdetailcache);
		service_->PublishBondGroupDetail(bond_group_detail_vec, "DeleteList");	
	}else{
		LOGGER_ERROR("DeleteBondGroupDetail: Failed for delete old relation: " << cache->id)
	}

    //step2: add new relation
    for (unsigned int i=0; i<bondkeylistmarkets.Size(); ++i)
    {
        sdbus::string bondkeylistmarket = bondkeylistmarkets[i]->ToString();

        BondGroupDetailCachePtr bondgroupdetailcache(new BondGroupDetailCache());

        bondgroupdetailcache->id = GetNewGuid();
        bondgroupdetailcache->bond_key_listed_market = bondkeylistmarket;
        bondgroupdetailcache->group_id = cache->id;
		bondgroupdetailcache->create_time = GetCurrentTimeString();

		bool succeed = BondGroupDetailCacheController::singleton()->insertCache(bondgroupdetailcache->id, bondgroupdetailcache);
		if (!succeed){
			LOGGER_ERROR("DeleteBondGroupDetail: Failed for add new relation: " << cache->id)
		}
        //这里无需更新model,故不需推送
    }

	//step3: save group filter
	cache->status = "1";
	cache->ua_flag = "2";
	cache->create_time = GetCurrentTimeString();
	UpdateGroup(cache);

    return true;
}