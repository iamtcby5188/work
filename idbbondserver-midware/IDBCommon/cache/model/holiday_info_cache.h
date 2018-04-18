/*
* holiday_info_cache.h
*
*  Created on: 2015 01 08
*      Author: Samuel.liu
*/

#ifndef HOLIDAY_INFO_CACHE_H_
#define HOLIDAY_INFO_CACHE_H_

class HolidayInfoCache;
typedef std::tr1::shared_ptr<HolidayInfoCache> HolidayInfoCachePtr;
typedef std::tr1::shared_ptr<std::vector<HolidayInfoCachePtr> > HolidayInfoCacheVecPtr;

class HolidayInfoCache {
public:
    static std::string SCHEMA_NAME;
    static std::string TABLE_NAME;
    static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    HolidayInfoCache();
    virtual ~HolidayInfoCache();
    std::string getPrimaryKey();

    bool operator ==(const HolidayInfoCache &rhs);
    bool operator !=(const HolidayInfoCache &rhs);

    std::string id;
    std::string delflag;
    std::string create_date;
    std::string modify_date;
    std::string username;
    std::string inputer;
    std::string checker;
    std::string sts;
    std::string country;
    std::string market_type;
    std::string holiday_date;
    std::string holiday_reason;

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
        FIELD(delflag),
        FIELD(create_date),
        FIELD(modify_date),
        FIELD(username),
        FIELD(inputer),
        FIELD(checker),
        FIELD(sts),
        FIELD(country),
        FIELD(market_type),
        FIELD(holiday_date),
        FIELD(holiday_reason)
        )
        );
};

#endif /* HOLIDAY_INFO_CACHE_H_ */
