/*
 * SelectiveInfoCache.h
 *
 *  Created on: May 30, 2014
 *      Author: theochen
 */

#ifndef SELECTIVEINFO_CACHE_H_
#define SELECTIVEINFO_CACHE_H_

#include "cache_def.h"

class SelectiveInfoCache;

typedef std::tr1::shared_ptr<SelectiveInfoCache> SelectiveInfoCachePtr;
typedef std::tr1::shared_ptr<std::vector<SelectiveInfoCachePtr> > SelectiveInfoCacheVecPtr;

class SelectiveInfoCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    SelectiveInfoCache();
    virtual ~SelectiveInfoCache();
	std::string getPrimaryKey();

public:
	std::string id;
    std::string selective_code;
    std::string selective_content;
    std::string group_name;
	std::string delflag;

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
		 KEY(selective_code, HASHED /*| INDEXED*/),
         FIELD(selective_content),
         KEY(group_name, HASHED /*| INDEXED*/),
		 FIELD(delflag)
        )
    );
};


/*
#define KEY(x, index) \
    (*FASTDB_NS::dbDescribeField(new FASTDB_NS::dbFieldDescriptor(#x, (char*)&x-(char*)this, \
                                                                  sizeof(x), index), x))
                                                                  */

#endif /* SELECTIVEINFO_CACHE_H_ */
