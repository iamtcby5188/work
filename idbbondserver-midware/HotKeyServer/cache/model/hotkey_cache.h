/*
 * hotkey_cache.h
 *
 *  Created on: August 27, 2014
 *      Author: yudan.chen
 */

#ifndef HOTKEY_CACHE_H_
#define HOTKEY_CACHE_H_

#include "cache_def.h"

class HotKeyCache;
typedef std::tr1::shared_ptr<HotKeyCache> HotKeyCachePtr;
typedef std::tr1::shared_ptr<std::vector<HotKeyCachePtr> > HotKeyCacheVecPtr;

class HotKeyCache {
public:
    static std::string SCHEMA_NAME;
    static std::string TABLE_NAME;
    static std::string PKEY_NAME;
public:
    HotKeyCache();
    virtual ~HotKeyCache();
    std::string getPrimaryKey();

    bool operator ==(const HotKeyCache &rhs);
    bool operator !=(const HotKeyCache &rhs);

public:
    std::string Id;   
    std::string HotKey;
    std::string FunctionName;
    std::string Description;
    std::string Function;
    std::string Params;
    int ParamSize;
};

#endif /* HOTKEY_CACHE_H_ */