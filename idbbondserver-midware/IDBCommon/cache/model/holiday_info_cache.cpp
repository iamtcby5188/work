/*
* holiday_info_cache.h
*
*  Created on: 2015 01 08
*      Author: Samuel.liu
*/

#include "cache_def.h"
#include "holiday_info_cache.h"
#include <set>

std::string HolidayInfoCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string HolidayInfoCache::TABLE_NAME = "holiday_info";
std::string HolidayInfoCache::PKEY_NAME = "id";
std::string HolidayInfoCache::SQL_TAG = "FetchHolidayInfoSQL";

HolidayInfoCache::HolidayInfoCache() {
    // TODO Auto-generated constructor stub
}

std::string HolidayInfoCache::getPrimaryKey() {
	return id;
}

HolidayInfoCache::~HolidayInfoCache() {
    // TODO Auto-generated destructor stub
}


bool HolidayInfoCache::operator==(const HolidayInfoCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool HolidayInfoCache::operator!=(const HolidayInfoCache &rhs)
{
    return !operator==(rhs);
}

REGISTER(HolidayInfoCache);
