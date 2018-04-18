#ifndef _PROVINCE_INFO_CACHE_H_
#define _PROVINCE_INFO_CACHE_H_

class ProvinceInfoCache;
typedef std::tr1::shared_ptr<ProvinceInfoCache> ProvinceInfoCachePtr;
typedef std::tr1::shared_ptr<std::vector<ProvinceInfoCachePtr> > ProvinceInfoCacheVecPtr;

class ProvinceInfoCache 
{
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	ProvinceInfoCache();
	virtual ~ProvinceInfoCache();
	std::string getPrimaryKey();

	bool operator ==(const ProvinceInfoCache &rhs);
	bool operator !=(const ProvinceInfoCache &rhs);

public:
    std::string id;
    std::string area_code;
    std::string area_english_name;
    std::string area_name;
    std::string area_level;
    std::string area_upper_grade_code;

public:
	TYPE_DESCRIPTOR(
		(KEY(id, HASHED /*| INDEXED*/),
		FIELD(area_code),
		FIELD(area_english_name),
		FIELD(area_name),
		FIELD(area_level),
		FIELD(area_upper_grade_code)
		)
		);
};

#endif //_PROVINCE_INFO_CACHE_H_