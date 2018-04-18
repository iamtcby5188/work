
#ifndef CHINABONDVALUATION_CACHE_H_
#define CHINABONDVALUATION_CACHE_H_

class ChinaBondValuationCache;
typedef std::tr1::shared_ptr<ChinaBondValuationCache> ChinaBondValuationCachePtr;
typedef std::tr1::shared_ptr<std::vector<ChinaBondValuationCachePtr> > ChinaBondValuationCacheVecPtr;

class ChinaBondValuationCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    ChinaBondValuationCache();
    virtual ~ChinaBondValuationCache();
	std::string getPrimaryKey();

    bool operator ==(const ChinaBondValuationCache &rhs);
    bool operator !=(const ChinaBondValuationCache &rhs);

    std::string id;
    std::string bond_id;
    std::string listed_market;
    std::string valuation_yield;
    std::string valuation_net_price;
    std::string valuation_date;
    std::string status;
    std::string modify_time;
	std::string bond_key_listed_market;
	double val_modified_duration;
	double val_convexity;
	double val_basis_point_value;

    // 导入时记录excel中相关信息, record msgs when import, Non-Database Fields
    int rowNum;
    std::string errReason;
    std::string bondIdExcel;
    std::string repaymentPeriod;

public:
    TYPE_DESCRIPTOR(
            (
                    KEY(id, HASHED /*| INDEXED*/),
                    KEY(bond_id, HASHED /*| INDEXED*/),
                    FIELD(listed_market),
                    FIELD(valuation_yield),
                    FIELD(valuation_net_price),
                    FIELD(valuation_date),
                    FIELD(status),
                    FIELD(modify_time),
					FIELD(bond_key_listed_market),
					FIELD(val_modified_duration),
					FIELD(val_convexity),
					FIELD(val_basis_point_value)
            )
    );

};

#endif /* CHINABONDVALUATION_CACHE_H_ */
