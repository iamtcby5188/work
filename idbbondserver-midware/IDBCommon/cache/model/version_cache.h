/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    SystemCompanyCache 提供访问Company数据缓存的接口
====================================================================================================================
   Date       Name                                  Description of Change
14-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/

#ifndef VERSION_CACHE_H_
#define VERSION_CACHE_H_

#include "cache_def.h"

class VersionCache;
typedef std::tr1::shared_ptr<VersionCache> VersionCachePtr;
typedef std::tr1::shared_ptr<std::list<VersionCachePtr> > VersionCacheListPtr;

class VersionCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	VersionCache();
	virtual ~VersionCache();
	std::string getPrimaryKey();

    bool operator ==(const VersionCache &rhs);
    bool operator !=(const VersionCache &rhs);

	std::string methodcompany;
	int version;

public:
	TYPE_DESCRIPTOR(
		(KEY(methodcompany, HASHED),
		FIELD(version)
		)
		);
};

#endif
