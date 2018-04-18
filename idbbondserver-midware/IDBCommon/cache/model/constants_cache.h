/*
 * constants_cache.h
 *
 *  Created on: August 13, 2014
 *      Author: yudan.chen
 */

#ifndef CONSTANTS_CACHE_H_
#define CONSTANTS_CACHE_H_

#include "cache_def.h"

class ConstantsCache;
typedef std::tr1::shared_ptr<ConstantsCache> ConstantsCachePtr;
typedef std::tr1::shared_ptr<std::vector<ConstantsCachePtr> > ConstantsCacheVecPtr;

class ConstantsCache {
public:
    static std::string SCHEMA_NAME;
    static std::string TABLE_NAME;
    static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    ConstantsCache();
    virtual ~ConstantsCache();
    std::string getPrimaryKey();

public:
    std::string id;   
    std::string name;
    std::string value;
    std::string status;
    std::string create_time;
    std::string constants_key;

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
         FIELD(name),
         FIELD(value),
         FIELD(status),
         FIELD(create_time),
         FIELD(constants_key)
        )
    );
};

#endif /* CONSTANTS_CACHE_H_ */