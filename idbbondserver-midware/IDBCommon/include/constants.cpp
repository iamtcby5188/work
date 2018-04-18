/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Define some constants
====================================================================================================================
   Date       Name                                  Description of Change
14-Jan-2016   Jialu Sun                              Initial version
$HISTORY$
===================================================================================================================*/
#include "constants.h"
#include "cache/controller/account_cachecontroller.h" //Add by Young 05-Apr-2016
#include "cache/model/system_company_cache.h"
#include "cache/cache_controller_declear.h"
#include "fid.h"

// qpid for QB sync
std::string kQBSyncTopic;
std::string kQBSyncServiceQueue;

// best quote make and insert into to mysql database
std::string kCalcAndInsertIntoMysqlDatabase;
bool kFirstLaunch;
bool useCacheInMem = false;

//DCS开关
bool kDcsEnable = false;

//DCS收单状态
bool kDcsSwitchStatus;

//每次请求DCS成交单数据的条数
int kDcsPageSize;

int kServiceSyncLimit; // datamanager sync pagesize

std::string GetCurrentCompanyId(std::string account_id)
{
    std::string companyId;
    if(account_id.empty())
        return std::string();

	AccountCacheController account_cc;
    std::string str = "id = '" + account_id + "'";
    dbQuery q(str.c_str());
    AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
    if (cache != NULL)
        companyId = cache->company_id;

    LOGGER_INFO("Find company id[" + companyId + "] with account id[" + account_id + "]");
	return companyId;
}

std::string GetCurrentCompanyId(const sdbus::Message* msg)
{
    std::string account_id;
    std::string company_id;
    msg->GetString(FID_ACCOUNT_ID, account_id);
    msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
    if(company_id.empty()){
        company_id = GetCurrentCompanyId(account_id);
    }

    return company_id;
}

std::string GetCurrentCompanyName(std::string company_id)
{
	std::string companyName;
    if(company_id.empty()){
		LOGGER_WARN("Find company name[" + companyName + "] with company id[" + company_id + "]");
		return companyName;
	}

    /*AccountCacheController account_cc;
    std::string str = "company_id = '" + company_id + "' limit 1";
    dbQuery q(str.c_str());
    AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
    if (cache != NULL)
        companyName = cache->company_name;
	else
		LOGGER_INFO("Find company name[" + companyName + "] with company id[" + company_id + "]");
    return companyName;*/
	std::string str = "id='" + company_id + "'";
	dbQuery q(str.c_str());
	SystemCompanyCacheController cc;
	SystemCompanyCachePtr cache = cc.getCacheByQueryInThreadSafty(q);
	if(cache.get() != NULL){
		companyName = cache->name;
	}else{
		LOGGER_INFO("Find company name[" + companyName + "] with company id[" + company_id + "]");
	}
	return companyName;
}

std::string GetCurrentCompanyIdByAccountName(std::string account_name)
{
	std::string companyId;
	if (account_name.empty())
		return std::string();

	AccountCacheController account_cc;
	std::string str = "username = '" + account_name + "'";
	dbQuery q(str.c_str());
	AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
	if (cache != NULL)
		companyId = cache->company_id;

	LOGGER_INFO("Find company id [" + companyId + "] with account name [" + account_name + "]");
	return companyId;
}

std::string GetCurrentAccountName(std::string account_id)
{
	std::string user_name;
	if (account_id.empty())
		return std::string();

	AccountCacheController account_cc;
	std::string str = "id = '" + account_id + "'";
	dbQuery q(str.c_str());
	AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
	if (cache != NULL)
		user_name = cache->username;

	LOGGER_INFO("Find user name [" + user_name + "] with account id [" + account_id + "]");
	return user_name;
}
