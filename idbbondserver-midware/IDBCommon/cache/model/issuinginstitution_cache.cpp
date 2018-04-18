/*
 * issuinginstitution_cache.cpp
 *
 *  Created on: May 30, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "issuinginstitution_cache.h"

std::string IssuingInstitutionCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string IssuingInstitutionCache::TABLE_NAME = "institution";
std::string IssuingInstitutionCache::PKEY_NAME  = "id";
std::string IssuingInstitutionCache::SQL_TAG = "FetchIssuingInstitutionSQL";

IssuingInstitutionCache::IssuingInstitutionCache() {
    // TODO Auto-generated constructor stub
	delflag = "0";
}

IssuingInstitutionCache::~IssuingInstitutionCache() {
    // TODO Auto-generated destructor stub
}

std::string IssuingInstitutionCache::getPrimaryKey() {
	return id;
}

bool IssuingInstitutionCache::operator == (const IssuingInstitutionCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool IssuingInstitutionCache::operator!=(const IssuingInstitutionCache &rhs)
{
    return !operator==(rhs);
}


REGISTER(IssuingInstitutionCache);




