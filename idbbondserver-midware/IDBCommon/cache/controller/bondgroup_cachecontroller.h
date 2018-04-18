/*
 * bondgroup_cachecontroller.h
 *
 *  Created on: Jun 24, 2014
 *      Author: yudan.chen
 */

#ifndef BONDGROUP_CACHECONTROLLER_H_
#define BONDGROUP_CACHECONTROLLER_H_

#include "cache/simple_cache_controller.h"
#include "cache/model/bondgroup_cache.h"

class BondGroupCacheController : public SimpleCacheController<BondGroupCache, BondGroupCachePtr, BondGroupCacheVecPtr> {
public:
	static BondGroupCacheController* singleton();

    virtual ~BondGroupCacheController();
protected:
	BondGroupCacheController();
	virtual void loadCache(std::map<std::string, BondGroupCachePtr> &data_mp, sql::ResultSet* resSet);

	virtual int updateDiskDB(BondGroupCachePtr cache_new, BondGroupCachePtr cache_old);
	virtual int insertDiskDB(BondGroupCachePtr cache);
	virtual int deleteDiskDB(const std::vector<std::string> &id_list);

	static BondGroupCacheController* singleton_;
};

#endif /* BONDGROUP_CACHECONTROLLER_H_ */
