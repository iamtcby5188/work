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

#ifndef SYSTEM_COMPANY_CACHE_H_
#define SYSTEM_COMPANY_CACHE_H_

#include "cache_def.h"

class SystemCompanyCache;
typedef std::tr1::shared_ptr<SystemCompanyCache> SystemCompanyCachePtr;
typedef std::tr1::shared_ptr<std::vector<SystemCompanyCachePtr> > SystemCompanyCacheVecPtr;

class SystemCompanyCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	SystemCompanyCache();
	virtual ~SystemCompanyCache();
	std::string getPrimaryKey();

    bool operator ==(const SystemCompanyCache &rhs);
    bool operator !=(const SystemCompanyCache &rhs);

	std::string id;
	std::string name;
	std::string status;

public:
	TYPE_DESCRIPTOR(
		(KEY(id, HASHED /*| INDEXED*/),
		FIELD(name),
		FIELD(status)
		)
		);
};

#endif
