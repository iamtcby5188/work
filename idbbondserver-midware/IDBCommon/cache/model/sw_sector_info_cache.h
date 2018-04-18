#ifndef _SW_SECTOR_INFO_CACHE_H_
#define _SW_SECTOR_INFO_CACHE_H_

class SwSectorInfoCache;
typedef std::tr1::shared_ptr<SwSectorInfoCache> SwSectorInfoCachePtr;
typedef std::tr1::shared_ptr<std::vector<SwSectorInfoCachePtr> > SwSectorInfoCacheVecPtr;

class SwSectorInfoCache 
{
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	SwSectorInfoCache();
	virtual ~SwSectorInfoCache();
	std::string getPrimaryKey();

	bool operator ==(const SwSectorInfoCache &rhs);
	bool operator !=(const SwSectorInfoCache &rhs);

public:
	std::string id;
	std::string delflag;
	std::string create_date;
	std::string modify_date;
	std::string sw_external_code;
	std::string sw_internal_code;
	std::string sw_level;
	std::string sw_name;
	std::string sw_english_name;
	std::string sw_upper_grade_code;
	std::string sw_status;

public:
	TYPE_DESCRIPTOR(
		(KEY(id, HASHED /*| INDEXED*/),
		FIELD(delflag),
		FIELD(create_date),
		FIELD(modify_date),
		FIELD(sw_external_code),
		FIELD(sw_internal_code),
		FIELD(sw_level),
		FIELD(sw_name),
		FIELD(sw_english_name),
		FIELD(sw_upper_grade_code),
		FIELD(sw_status)
		)
		);

};

#endif