#ifndef _ISSUER_INFO_CACHE_H_
#define _ISSUER_INFO_CACHE_H_

class IssuerInfoCache;
typedef std::tr1::shared_ptr<IssuerInfoCache> IssuerInfoCachePtr;
typedef std::tr1::shared_ptr<std::vector<IssuerInfoCachePtr> > IssuerInfoCacheVecPtr;

class IssuerInfoCache 
{
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	IssuerInfoCache();
	virtual ~IssuerInfoCache();
	std::string getPrimaryKey();

	bool operator ==(const IssuerInfoCache &rhs);
	bool operator !=(const IssuerInfoCache &rhs);

public:
	std::string id;
	std::string delflag;
	std::string create_date;
	std::string modify_date;
	std::string institution_code;
	std::string issuer_name;
	std::string issuer_type;
	std::string sw_sector;
	std::string sw_subsector;
	std::string sw_sector_code;
	std::string sw_subsector_code;
	std::string province;
	std::string city;
	std::string province_code;
	std::string CBRC_Financing_Platform;	//ƽ̨ծ

public:
	TYPE_DESCRIPTOR(
		(KEY(id, HASHED /*| INDEXED*/),
		FIELD(delflag),
		FIELD(create_date),
		FIELD(modify_date),
		FIELD(institution_code),
		FIELD(issuer_name),
		FIELD(issuer_type),
		FIELD(sw_sector),
		FIELD(sw_subsector),
		FIELD(sw_sector_code),
		FIELD(sw_subsector_code),
		FIELD(province),
		FIELD(city),
		FIELD(province_code),
		FIELD(CBRC_Financing_Platform)
		)
		);

};

#endif