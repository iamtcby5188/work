/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    InstitutionRatingCache �ṩ���ʷ������������ݻ���Ľӿ�
====================================================================================================================
   Date       Name                                  Description of Change
19-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#include "cache_def.h"
#include "institutionrating_cache.h"
#include <set>

std::string InstitutionRatingCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string InstitutionRatingCache::TABLE_NAME = "p_institution_rating";
std::string InstitutionRatingCache::PKEY_NAME = "id";
std::string InstitutionRatingCache::SQL_TAG = "FetchInstitutionRatingSQL";

InstitutionRatingCache::InstitutionRatingCache() {
	// TODO Auto-generated constructor stub
    rating_date = 0;
}

std::string InstitutionRatingCache::getPrimaryKey() {
	return id;
}

InstitutionRatingCache::~InstitutionRatingCache() {
	// TODO Auto-generated destructor stub
}

bool InstitutionRatingCache::operator==(const InstitutionRatingCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool InstitutionRatingCache::operator!=(const InstitutionRatingCache &rhs)
{
	return !operator==(rhs);
}

REGISTER(InstitutionRatingCache);