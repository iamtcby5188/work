#include "cache_def.h"
#include "sw_sector_info_cache.h"
#include <set>

std::string SwSectorInfoCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string SwSectorInfoCache::TABLE_NAME = "p_sw_sector_info";
std::string SwSectorInfoCache::PKEY_NAME = "id";
std::string SwSectorInfoCache::SQL_TAG = "FetchSwSectorInfoSQL";

SwSectorInfoCache::SwSectorInfoCache() {
	// TODO Auto-generated constructor stub
}

std::string SwSectorInfoCache::getPrimaryKey() {
	return id;
}

SwSectorInfoCache::~SwSectorInfoCache() {
	// TODO Auto-generated destructor stub
}

bool SwSectorInfoCache::operator==(const SwSectorInfoCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool SwSectorInfoCache::operator!=(const SwSectorInfoCache &rhs)
{
	return !operator==(rhs);
}

REGISTER(SwSectorInfoCache);