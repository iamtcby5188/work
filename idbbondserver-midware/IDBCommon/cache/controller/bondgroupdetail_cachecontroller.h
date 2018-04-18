/*
 * bondgroupdetail_cachecontroller.h
 *
 *  Created on: Jun 24, 2014
 *      Author: yudan.chen
 */

#ifndef BONDGROUPDETAIL_CACHECONTROLLER_H_
#define BONDGROUPDETAIL_CACHECONTROLLER_H_

#include "cache/simple_cache_controller.h"
#include "cache/model/bondgroupdetail_cache.h"

class BondGroupDetailCacheController : public SimpleCacheController<BondGroupDetailCache, BondGroupDetailCachePtr, BondGroupDetailCacheVecPtr> {
public:
	static BondGroupDetailCacheController* singleton();

	virtual ~BondGroupDetailCacheController();
	bool deleteCacheByGroupId(const std::string &group_id);
	BondKeyListedMarketVecPtr getBondByGroupId(const std::string &group_id);
protected:
	BondGroupDetailCacheController();
	virtual void loadCache(std::map<std::string, BondGroupDetailCachePtr> &data_mp, sql::ResultSet* resSet);

	virtual int updateDiskDB(BondGroupDetailCachePtr cache_new, BondGroupDetailCachePtr cache_old);
	virtual int insertDiskDB(BondGroupDetailCachePtr cache);
	virtual int deleteDiskDB(const std::vector<std::string> &id_list);
	
	static BondGroupDetailCacheController* singleton_;
};
#endif /* BONDGROUPDETAIL_CACHECONTROLLER_H_ */
