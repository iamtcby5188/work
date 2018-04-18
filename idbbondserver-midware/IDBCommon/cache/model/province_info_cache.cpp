#include "cache_def.h"
#include "province_info_cache.h"
//#include <set>

std::string ProvinceInfoCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string ProvinceInfoCache::TABLE_NAME = "p_province_info";
std::string ProvinceInfoCache::PKEY_NAME = "id";
std::string ProvinceInfoCache::SQL_TAG = "FetchProvinceInfoSQL";

ProvinceInfoCache::ProvinceInfoCache() {
	// TODO Auto-generated constructor stub
}

std::string ProvinceInfoCache::getPrimaryKey() {
	return id;
}

ProvinceInfoCache::~ProvinceInfoCache() {
	// TODO Auto-generated destructor stub
}

bool ProvinceInfoCache::operator==(const ProvinceInfoCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool ProvinceInfoCache::operator!=(const ProvinceInfoCache &rhs)
{
	return !operator==(rhs);
}

REGISTER(ProvinceInfoCache);