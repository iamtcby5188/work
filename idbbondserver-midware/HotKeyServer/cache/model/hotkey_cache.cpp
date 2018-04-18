/*
 * hotkey_cache.cpp
 *
 *  Created on: August 27, 2014
 *      Author: yudan.chen
 */

#include "hotkey_cache.h"

std::string HotKeyCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string HotKeyCache::TABLE_NAME = "hot_key";
std::string HotKeyCache::PKEY_NAME = "Id";

HotKeyCache::HotKeyCache() {
    // TODO Auto-generated constructor stub
}

HotKeyCache::~HotKeyCache() {
    // TODO Auto-generated destructor stub
}

std::string HotKeyCache::getPrimaryKey() {
    return Id;
}

bool HotKeyCache::operator==(const HotKeyCache &rhs)
{
    return Id == rhs.Id && 
        HotKey == rhs.HotKey &&
        FunctionName == rhs.FunctionName &&
        Description == rhs.Description &&
        Function == rhs.Function;
}

bool HotKeyCache::operator!=(const HotKeyCache &rhs)
{
    return !operator==(rhs);
}

