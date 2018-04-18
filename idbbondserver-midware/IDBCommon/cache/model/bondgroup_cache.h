/*
 * bondgroup_cache.h
 *
 *  Created on: June 24, 2014
 *      Author: yudan.chen
 */

#ifndef BONDGROUP_CACHE_H_
#define BONDGROUP_CACHE_H_

#include "cache_def.h"

class BondGroupCache;
typedef std::tr1::shared_ptr<BondGroupCache> BondGroupCachePtr;
typedef std::tr1::shared_ptr<std::vector<BondGroupCachePtr> > BondGroupCacheVecPtr;

class BondGroupCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
public:
    BondGroupCache();
    virtual ~BondGroupCache();
	std::string getPrimaryKey();

	bool operator ==(const BondGroupCache &rhs);
	bool operator !=(const BondGroupCache &rhs);
public:
    std::string id;   //group_id
    std::string company_id;
    std::string ua_id;
    std::string group_name;
	std::string status;
	std::string ua_flag;
	std::string create_time;
	std::string filter_condition;
};

#endif /* BONDGROUP_CACHE_H_ */