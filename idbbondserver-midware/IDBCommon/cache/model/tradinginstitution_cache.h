/*
 * tradinginstitution_cache.h
 *
 *  Created on: June 30, 2014
 *      Author: theochen
 */

#ifndef TRADINGINSTITUTION_CACHE_H_
#define TRADINGINSTITUTION_CACHE_H_

#include "cache_def.h"

class TradingInstitutionCache;
typedef std::tr1::shared_ptr<TradingInstitutionCache> TradingInstitutionCachePtr;
typedef std::tr1::shared_ptr<std::vector<TradingInstitutionCachePtr> > TradingInstitutionCacheVecPtr;

class TradingInstitutionCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    TradingInstitutionCache();
    virtual ~TradingInstitutionCache();
	std::string getPrimaryKey();

    bool operator ==(const TradingInstitutionCache &rhs);
    bool operator !=(const TradingInstitutionCache &rhs);

public:
    std::string id;
    std::string company_id;
    std::string name;
    std::string name_en;
    std::string description;
    std::string code;
    std::string status;
    std::string attribute;
    std::string attribute1;
    std::string attribute2;
    std::string attribute3;
    std::string attribute4;
    std::string attribute5;
    std::string bank_type;
    std::string full_name;
    std::string city;
    std::string full_name_en;
    std::string pinyin;
    std::string pinyin_full;
    std::string city_name;
	std::string modify_time;
	std::string create_time;
	int crm_id;

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
         KEY(company_id, 0/*HASHED | INDEXED*/),
         FIELD(name),
         FIELD(name_en),
         FIELD(description),
         KEY(code, HASHED /*| INDEXED*/),
         FIELD(status),
         FIELD(attribute),
         FIELD(attribute1),
         FIELD(attribute2),
         FIELD(attribute3),
         FIELD(attribute4),
         FIELD(attribute5),
         FIELD(bank_type),
         FIELD(full_name),
         FIELD(city),
         FIELD(full_name_en),
         FIELD(pinyin),
         FIELD(pinyin_full),
         FIELD(city_name),
		 FIELD(modify_time),
		 FIELD(create_time),
		 FIELD(crm_id)
        )
    );
};

#endif /* TRADINGINSTITUTION_CACHE_H_ */