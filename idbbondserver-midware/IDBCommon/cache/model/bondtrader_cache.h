/*
 * BondTraderCache.h
 *
 *  Created on: Jun 12, 2014
 *      Author: theochen
 */

#ifndef BONDTRADER_CACHE_H_
#define BONDTRADER_CACHE_H_

#include "cache_def.h"

class BondTraderCache;
typedef std::tr1::shared_ptr<BondTraderCache> BondTraderCachePtr;
typedef std::tr1::shared_ptr<std::vector<BondTraderCachePtr> > BondTraderCacheVecPtr;

class BondTraderCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    BondTraderCache();
    virtual ~BondTraderCache();
	std::string getPrimaryKey();
    
    bool operator ==(const BondTraderCache &rhs);
    bool operator !=(const BondTraderCache &rhs);

    std::string id;
    std::string company_id;
    std::string name;
    std::string code;
    std::string f_code;
    std::string attribute;
    std::string attribute1;
    std::string attribute2;
    std::string attribute3;
    std::string attribute4;
    std::string attribute5;
    std::string create_time;
    std::string status;
    std::string flag;
    bool flag_vip;
    bool flag_bad;
    std::string flag_bargain;
    std::string bank_id;
    std::string customer_id;
    std::string pinyin;
    std::string pinyin_full;
	std::string modify_time;
	int crm_id;

public:
    TYPE_DESCRIPTOR(
            (
                    KEY(id, HASHED /*| INDEXED*/),
                    KEY(company_id, 0/*HASHED | INDEXED*/),
                    FIELD(name),
                    FIELD(code),
                    FIELD(f_code),
                    FIELD(attribute),
                    FIELD(attribute1),
                    FIELD(attribute2),
                    FIELD(attribute3),
                    FIELD(attribute4),
                    FIELD(attribute5),
                    FIELD(create_time),
                    FIELD(status),
                    FIELD(flag),
                    FIELD(flag_vip),
                    FIELD(flag_bad),
                    FIELD(flag_bargain),
                    KEY(bank_id, HASHED /*| INDEXED*/),
                    KEY(customer_id, HASHED /*| INDEXED*/),
                    FIELD(pinyin),
                    FIELD(pinyin_full),
					FIELD(modify_time),
					FIELD(crm_id)
            )
    );

};

#endif /* BONDTRADER_CACHE_H_ */
