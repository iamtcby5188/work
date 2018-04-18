/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    InstitutionRatingCache 提供访问发行人评级数据缓存的接口
====================================================================================================================
   Date       Name                                  Description of Change
19-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _INSTITUTION_RATING_CACHE_H_
#define _INSTITUTION_RATING_CACHE_H_

#include "cache_def.h"

class InstitutionRatingCache;
typedef std::tr1::shared_ptr<InstitutionRatingCache> InstitutionRatingCachePtr;
typedef std::tr1::shared_ptr<std::vector<InstitutionRatingCachePtr> > InstitutionRatingCacheVecPtr;

class InstitutionRatingCache 
{
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	InstitutionRatingCache();
	virtual ~InstitutionRatingCache();
	std::string getPrimaryKey();

	bool operator ==(const InstitutionRatingCache &rhs);
	bool operator !=(const InstitutionRatingCache &rhs);

public:
	std::string id;
	std::string delflag;
	std::string sts;
	std::string institution_code;
	std::string institution_type;
	std::string rating_institution_code;
	int4 rating_date;
	std::string rate;
	std::string outlook;

public:
	TYPE_DESCRIPTOR(
		(KEY(id, HASHED /*| INDEXED*/),
		FIELD(delflag),
		FIELD(sts),
		KEY(institution_code, HASHED /*INDEXED*/),
		FIELD(institution_type),
		FIELD(rating_institution_code),
		KEY(rating_date, INDEXED),
		FIELD(rate),
		FIELD(outlook)
		)
		);

};

#endif