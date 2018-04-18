/*
 * constants_cache.h
 *
 *  Created on: August 18, 2014
 *      Author: yudan.chen
 */

#ifndef CHINABONDDEVIATEDVALUE_CACHE_H_
#define CHINABONDDEVIATEDVALUE_CACHE_H_

#include "cache_def.h"

class ChinaBondDeviatedValueCache;
typedef std::tr1::shared_ptr<ChinaBondDeviatedValueCache> ChinaBondDeviatedValueCachePtr;
typedef std::tr1::shared_ptr<std::vector<ChinaBondDeviatedValueCachePtr> > ChinaBondDeviatedValueCacheVecPtr;

class ChinaBondDeviatedValueCache {
public:
    static std::string SCHEMA_NAME;
    static std::string TABLE_NAME;
    static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    ChinaBondDeviatedValueCache();
    virtual ~ChinaBondDeviatedValueCache();
    std::string getPrimaryKey();

    bool operator ==(const ChinaBondDeviatedValueCache &rhs);
    bool operator !=(const ChinaBondDeviatedValueCache &rhs);

public:
    std::string id;   
    std::string company_id;   
    std::string bond_type_id;
    std::string bond_type_name;
    double deviated_value;
    std::string status;
    std::string modify_time;

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
         KEY(company_id, 0/*HASHED | INDEXED*/),
         KEY(bond_type_id, HASHED /*| INDEXED*/),
         FIELD(bond_type_name),
         FIELD(deviated_value),
         FIELD(status),
         FIELD(modify_time)
        )
    );
};

#endif /* CHINABONDDEVIATEDVALUE_CACHE_H_ */