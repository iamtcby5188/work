/*
 * BondTypeSelective_cache.h
 *
 *  Created on: July 08, 2014
 *      Author: theochen
 */

#ifndef BONDTYPESELECTIVE_CACHE_H_
#define BONDTYPESELECTIVE_CACHE_H_

#include "cache_def.h"

class BondTypeSelectiveCache;
typedef std::tr1::shared_ptr<BondTypeSelectiveCache> BondTypeSelectiveCachePtr;
typedef std::tr1::shared_ptr<std::vector<BondTypeSelectiveCachePtr> > BondTypeSelectiveCacheVecPtr;

class BondTypeSelectiveCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    BondTypeSelectiveCache();
    virtual ~BondTypeSelectiveCache();
	std::string getPrimaryKey();

public:
    std::string id;
	std::string selective_code;
	std::string type_id;
	std::string status;

	// Non bond_type_selective Fields
	std::string name;
	std::string code;
	std::string description;
	std::string product;

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
         FIELD(selective_code),
		 FIELD(type_id),
		 FIELD(status),
		 NOUPDATE(name),
		 NOUPDATE(code),
		 NOUPDATE(description),
		 NOUPDATE(product)
        )
    );
};

#endif /* BONDTYPESELECTIVE_CACHE_H_ */

