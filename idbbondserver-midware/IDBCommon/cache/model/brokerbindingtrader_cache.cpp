/*
 * brokerbindingtrader_cache.h
 *
 *  Created on: September 22, 2014
 *      Author: yudan.chen
 */

#include "cache_def.h"
#include "brokerbindingtrader_cache.h"
#include "cache/controller/account_cachecontroller.h"

std::string BrokerBindingTraderCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BrokerBindingTraderCache::TABLE_NAME = "bond_bank_account";
std::string BrokerBindingTraderCache::PKEY_NAME = "id";
std::string BrokerBindingTraderCache::SQL_TAG = "FetchBrokerBindingTraderSQL";

BrokerBindingTraderCache::BrokerBindingTraderCache() {
    // TODO Auto-generated constructor stub
    sort = -1;
}

BrokerBindingTraderCache::~BrokerBindingTraderCache() {
    // TODO Auto-generated destructor stub
}

std::string BrokerBindingTraderCache::getPrimaryKey() {
    return id;
}

bool BrokerBindingTraderCache::operator==(const BrokerBindingTraderCache &rhs)
{
    return isCacheEqual(*this, rhs);
}

bool BrokerBindingTraderCache::operator!=(const BrokerBindingTraderCache &rhs)
{
    return !operator==(rhs);
}

void BrokerBindingTraderCache::loadCombinationFields(){
	loadDepartmentFields();
}

void BrokerBindingTraderCache::loadDepartmentFields() {
	//AccountCacheController accountCC;				 
	//dbQuery q;
	//q = "id = ", account_id;
	//AccountCachePtr account = accountCC.getCacheByQueryInThreadSafty(q);
	AccountCachePtr account = AccountCacheController::getCacheInMem("id", account_id);
    if (account) {
        account_department = account->department_codes;
        company_id = account->company_id;
    } else {
        account_department = "";
        company_id = kCompanyTP;
    }
}

REGISTER(BrokerBindingTraderCache);
