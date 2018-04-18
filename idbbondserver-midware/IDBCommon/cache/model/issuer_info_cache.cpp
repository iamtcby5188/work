#include "cache_def.h"
#include "issuer_info_cache.h"
#include <set>

std::string IssuerInfoCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string IssuerInfoCache::TABLE_NAME = "p_issuer_info";
std::string IssuerInfoCache::PKEY_NAME = "id";
std::string IssuerInfoCache::SQL_TAG = "FetchIssuerInfoSQL";

IssuerInfoCache::IssuerInfoCache() {
	// TODO Auto-generated constructor stub
}

std::string IssuerInfoCache::getPrimaryKey() {
	return id;
}

IssuerInfoCache::~IssuerInfoCache() {
	// TODO Auto-generated destructor stub
}

bool IssuerInfoCache::operator==(const IssuerInfoCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool IssuerInfoCache::operator!=(const IssuerInfoCache &rhs)
{
	return !operator==(rhs);
}

REGISTER(IssuerInfoCache);