/*
 * brokerbindingtrader_cache.h
 *
 *  Created on: September 22, 2014
 *      Author: yudan.chen
 */

#ifndef BROKER_BINDING_TRADER_CACHE_H_
#define BROKER_BINDING_TRADER_CACHE_H_

#include "cache_def.h"

class BrokerBindingTraderCache;
typedef std::tr1::shared_ptr<BrokerBindingTraderCache> BrokerBindingTraderCachePtr;
typedef std::tr1::shared_ptr<std::vector<BrokerBindingTraderCachePtr> > BrokerBindingTraderCacheVecPtr;

class BrokerBindingTraderCache {
public:
    static std::string SCHEMA_NAME;
    static std::string TABLE_NAME;
    static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    BrokerBindingTraderCache();
    virtual ~BrokerBindingTraderCache();
    std::string getPrimaryKey();

    bool operator ==(const BrokerBindingTraderCache &rhs);
    bool operator !=(const BrokerBindingTraderCache &rhs);
	void loadCombinationFields();
	void loadDepartmentFields();
public:
    std::string id;   
    std::string account_id;
    std::string status;
    std::string create_time;
    std::string bank_agent_id;
    int sort;

	std::string account_department;
	std::string company_id;
public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
         KEY(account_id, HASHED /*| INDEXED*/),
         FIELD(status),
         FIELD(create_time),
         KEY(bank_agent_id, HASHED /*| INDEXED*/),
         FIELD(sort),
		 COMBINED(account_department),
		 COMBINED(company_id)
        )
    );
};

#endif /* BROKER_BINDING_TRADER_CACHE_H_ */