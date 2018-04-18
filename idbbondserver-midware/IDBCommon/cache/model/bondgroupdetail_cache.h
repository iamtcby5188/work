/*
 * bondgroupdetail_cache.h
 *
 *  Created on: June 25, 2014
 *      Author: yudan.chen
 */

#ifndef BONDGROUPDETAIL_CACHE_H_
#define BONDGROUPDETAIL_CACHE_H_

#include "cache_def.h"

class BondGroupDetailCache;
typedef std::tr1::shared_ptr<BondGroupDetailCache> BondGroupDetailCachePtr;
typedef std::tr1::shared_ptr<std::vector<BondGroupDetailCachePtr> > BondGroupDetailCacheVecPtr;
typedef std::tr1::shared_ptr<std::vector<std::string> > BondKeyListedMarketVecPtr;

class BondGroupDetailCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
public:
    BondGroupDetailCache();
    virtual ~BondGroupDetailCache();
	std::string getPrimaryKey();

	bool operator ==(const BondGroupDetailCache &rhs);
	bool operator !=(const BondGroupDetailCache &rhs);
public:
	std::string id;
    std::string group_id;   
    std::string bond_key_listed_market;
	std::string create_time;
};

#endif /* BONDGROUPDETAIL_CACHE_H_ */