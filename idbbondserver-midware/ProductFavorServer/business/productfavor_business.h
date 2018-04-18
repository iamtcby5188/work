#ifndef _PRODUCTFAVOR_BUSINESS_H_
#define _PRODUCTFAVOR_BUSINESS_H_

#include "logger.h"
#include <vector>
#include "service/productfavor_service.h"
#include "cache/model/bondgroup_cache.h"
#include "cache/model/bondgroupdetail_cache.h"

class ProductFavorBusiness
{
public:
    ProductFavorBusiness(ProductFavorService* service);
    BondGroupCacheVecPtr GetBondGroupList();
    BondGroupDetailCacheVecPtr GetBondGroupDetailList();
    bool AddGroup(const BondGroupCachePtr &cache);
    void UpdateGroup(const BondGroupCachePtr &cache);
    void DeleteGroup(const BondGroupCachePtr &cache);
    bool AddAttentionBonds(const BondGroupCachePtr &cache, sdbus::Variant::List &bondkeylistmarkets);

private:
    ProductFavorService* service_;

};

#endif // _BONDBUSINESS_H_