/*
 * account_cache.cpp
 *
 *  Created on: July 08, 2014
 *      Author: theochen
 */
#include "cache_def.h"
#include "account_cache.h"
#include "cache/cache_controller_declear.h"
#include "service/send_service.h"
#include <set>
#include "pinyin.h"

std::string AccountCache::SCHEMA_NAME = SCHEMA_IDB_ACCOUNT;
std::string AccountCache::TABLE_NAME = "idb_account";
std::string AccountCache::PKEY_NAME = "id";
std::string AccountCache::SQL_TAG = "FetchAccountSQL";

std::map<std::string, std::string> AccountCache::pinyin_mp;

AccountCache::AccountCache() {
    // TODO Auto-generated constructor stub
}

std::string AccountCache::getPrimaryKey() {
	return id;
}

AccountCache::~AccountCache() {
    // TODO Auto-generated destructor stub
}


bool AccountCache::operator==(const AccountCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool AccountCache::operator!=(const AccountCache &rhs)
{
    return !operator==(rhs);
}

void AccountCache::loadCombinationFields() 
{
    loadAccountGroupFields();
    loadAccountPermissionFields();
    loadAccountPinyinFields();
}

void AccountCache::loadAccountGroupFields()
{
    AccountGroupCacheController account_group_cc;
    department_codes = "";
    role_codes = "";

    dbQuery q;
    q = "account_id = ", id, " order by department_code";
    AccountGroupCacheVecPtr account_group_cache_list = account_group_cc.getCacheListByQueryInThreadSafty(q);
    for (size_t i = 0; i < account_group_cache_list->size(); ++i) {
        if (i != 0) {
            department_codes += ", ";
            role_codes += ", ";
        }
        department_codes += account_group_cache_list->at(i)->department_code;
        role_codes += account_group_cache_list->at(i)->department_code + "-" + account_group_cache_list->at(i)->group_name;
    }
}

void AccountCache::loadAccountPermissionFields()
{
    AccountPermissionCacheController account_permission_cc;
    permission_codes = "";
    
    dbQuery q;
    q = "account_id = ", id, " order by permission_name";
    AccountPermissionCacheVecPtr account_permission_cache_list = account_permission_cc.getCacheListByQueryInThreadSafty(q);

    std::vector<std::string> permission_vec;        
    for (size_t i = 0; i < account_permission_cache_list->size(); ++i) {
        AccountPermissionCachePtr account_permission = account_permission_cache_list->at(i);

        std::string account_group_id = account_permission->account_group_id;

        AccountGroupCacheController account_group_cc;
        dbQuery query;
        query = "account_id = ", id, " and account_group_id = ", account_group_id;
        AccountGroupCachePtr account_group = account_group_cc.getCacheByQueryInThreadSafty(query);
        if (account_group) {
             permission_vec.push_back(account_group->department_code + "-" + account_permission->permission_name);
        }
    }

    std::vector<std::string>::const_iterator it = permission_vec.begin(); 
    for ( ; it != permission_vec.end(); ++it) {
        if (permission_codes.length() > 0) {
            permission_codes += ", ";
        }

        permission_codes += *it;
    }
}

void AccountCache::loadAccountPinyinFields()
{
    //LOGGER_INFO("display_name:" + display_name)
    if (display_name.length() > 0) {
        std::map<std::string, std::string>::iterator it = pinyin_mp.find(display_name);
        if (it != pinyin_mp.end()) {
            pinyin = (*it).second;
        } else {
            pinyin = getFullAndInitialWithSeperator(display_name, "|");
            //LOGGER_INFO("getFullAndInitialNoSeperator result:" + pinyin)
        }
    } else {
        pinyin = "";
    }
    //LOGGER_INFO("AccountPinyin:" + pinyin)
}

REGISTER(AccountCache);
