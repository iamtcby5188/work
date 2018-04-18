/*
 * BondSmallViewCache.h
 *
 *  Created on: May 22, 2014
 *      Author: theochen
 */

#ifndef BONDSMALLVIEW_CACHE_H_
#define BONDSMALLVIEW_CACHE_H_

#include "selectiveinfo_cache.h"
#include "issuinginstitution_cache.h"

class BondSmallViewCache;
typedef std::tr1::shared_ptr<BondSmallViewCache> BondSmallViewCachePtr;
typedef std::tr1::shared_ptr<std::vector<BondSmallViewCachePtr> > BondSmallViewCacheVecPtr;

class BondSmallViewCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	BondSmallViewCache();
	~BondSmallViewCache();
	std::string getPrimaryKey();

	static void memCacheInit();
	static void memCacheClear();

    bool operator ==(const BondSmallViewCache &rhs);
    bool operator !=(const BondSmallViewCache &rhs);

public:
	// The exchange market of this security, 银行间|上交所|深交所 for CIB|SSE|SZE
	std::string listed_market;
	std::string bond_key_listed_market;
	std::string bond_id;
	std::string bond_subtype;
	std::string issuer_rating_current;
	std::string maturity_term;
	std::string create_date;
	std::string short_name;
	std::string warranter;
	std::string coupon_type;
	std::string rating_augment;
	int4   rating_date;
	std::string option_type;
	std::string pin_yin;
	std::string pin_yin_full;
	int4   delflag;
	real8  coupon_rate_current;
	int4   maturity_date;
	int4   listed_date;
	int4   delisted_date;
	std::string issuer_code;
	int4   next_coupon_date;
	int4   redemption_no;
	std::string is_cross_mkt;
	std::string is_mortgage;
	std::string rating_current;
	std::string bond_key;
	std::string mkt_type;
	int4   interest_start_date;
	std::string frn_index_id;
	int4   fixing_ma_days;
	real8  coupon_rate_spread;
	int4   option_date;
	real8  issue_amount;
	std::string underwriter_code;
	std::string issuer_rating_institution_code;
	std::string term_unit;
	std::string rating_institution_code;
	std::string is_municipal;
	std::string issuer_rating_current_npy;
	std::string rating_current_npy;
    int4 issue_year;
    std::string ent_cor;	//公司债=COP

    std::string coupon_rate_spread_client;
    std::string option_type_client;
    std::string redemption_no_client;
    std::string goods_type_client;
	std::string bond_goods_type_id;
    std::string maturity_term_client;
    std::string coupon_rate_current_client;
    int4   sort_remain;
    std::string product_type;
    bool is_expired;

	// ----------------- Combination Fields ----------------------
	//
    std::string institution_type;

    std::string cdc_valuation_yield;
    std::string cdc_valuation_net_price;

    std::string rating_augment_client;
    std::string coupon_type_client;
    std::string base_interest_rate_client;
    std::string warranter_client;
    std::string underwriter_client;
    std::string issuer_rating_institution_client;
    std::string rating_institution_client;
    std::string issuer_client;
    std::string issue_institution_pinyin_;
    std::string issue_institution_pinyinfull_;
    std::string institution_rating;
    std::string issue_institution_id;
    std::string maturity_date_type;
    int maturity_holidays; // 到期日的节假日天数
	int issuer_rating_order;

	/*******************行业和地区过滤，数据源来自issuer_info_cache**************************/
	std::string sw_sector;
	std::string sw_subsector;
	std::string sw_sector_code;
	std::string sw_subsector_code;
	std::string province;
	std::string province_code;
	std::string financing_platform;
	/*************************************************************************************/

	// ------------------------- Combined Fields (above) ----------------------------
	std::string time_to_maturity;
    double ttm;
	double time_to_maturity_real;
    std::string has_option;
    std::string option_client;
	std::string issuer_rating_filter;
	std::string guarenteed;
	std::string municipal; 
	std::string rate_index;
	std::string pledged;
	std::string across_market;
    std::string completed_bond_type;
    int bond_type_sort_client;
    std::string across_market_bond_ids;
	std::string outlook; //Add by Young 20-Apr-2016

	std::string company_bond;
	std::string on_the_run;

	// Non-FastDB related properties
	std::string method;

	// ------------------------- Merged from IDBBondServer ----------------------------
	int8 version;
	std::string operation_type;
	std::string create_time;

public:
    void loadCombinationFields();
	void loadRatingAugmentClient();
	void loadIssuingInstitution();
	void loadWarranterInstitution();
	void loadUnderwriterInstitution();
	void loadIusserRatingInstitution();
	void loadRatingInstitution();
	void loadCouponTypeClient();
    //void loadChinaBondValuationFields();
    void loadHolidayInfoFields();
	void loadTimeToMaturityFields();
	void loadHasOptionFields();
	void loadIssuerRatingFilter();
	void loadGuarenteed();
	void loadOtherFields();
	void loadIssuerInfoFields();
	void loadIssuerRatingFields();

public:
	TYPE_DESCRIPTOR(
			(FIELD(listed_market),
			KEY(bond_key_listed_market, /*INDEXED |*/ HASHED),
			KEY(bond_id, /*INDEXED |*/ HASHED),
			FIELD(bond_subtype),
			FIELD_NOCOMPARE(issuer_rating_current),
			FIELD(maturity_term),
			FIELD(create_date),
			FIELD(short_name),
			FIELD(warranter),
			FIELD(coupon_type),
			FIELD(rating_augment),
			FIELD(rating_date),
			FIELD(option_type),
			FIELD(pin_yin),
			FIELD(pin_yin_full),
			FIELD(delflag),
			FIELD(coupon_rate_current),
			FIELD(maturity_date),
			FIELD(listed_date),
			FIELD(delisted_date),
			FIELD(issuer_code),
			FIELD(next_coupon_date),
			FIELD(redemption_no),
			FIELD(is_cross_mkt),
			FIELD(is_mortgage),
			FIELD(rating_current),
			FIELD(bond_key),
			FIELD(mkt_type),
			FIELD(interest_start_date),
			FIELD(frn_index_id),
			FIELD(fixing_ma_days),
			FIELD(coupon_rate_spread),
			FIELD(option_date),
			FIELD(issue_amount),
			FIELD(underwriter_code),
			FIELD(issuer_rating_institution_code),
			FIELD(term_unit),
			FIELD(rating_institution_code),
			FIELD(is_municipal),
			FIELD(issuer_rating_current_npy),
			FIELD(rating_current_npy),
            FIELD(issue_year),
            FIELD(ent_cor),
            FIELD(coupon_rate_spread_client),
			FIELD(option_type_client),
            FIELD(redemption_no_client),
			FIELD(goods_type_client),
			FIELD(bond_goods_type_id),
            FIELD(maturity_term_client),
            FIELD(coupon_rate_current_client),
            FIELD(sort_remain),
            FIELD(product_type),
            FIELD(is_expired),
            COMBINED(issuer_rating_order),
            COMBINED(maturity_date_type),
            COMBINED(maturity_holidays),
            COMBINED(institution_type),
            COMBINED(method),
			COMBINED(rating_augment_client),
			COMBINED(coupon_type_client),
			COMBINED(issuer_client),
			COMBINED(base_interest_rate_client),
			COMBINED(warranter_client),
			COMBINED(underwriter_client),
			COMBINED(issuer_rating_institution_client),
			COMBINED(rating_institution_client),
            COMBINED(issue_institution_pinyin_),
            COMBINED(issue_institution_pinyinfull_),
            COMBINED(institution_rating),
            COMBINED(issue_institution_id),
            COMBINED(cdc_valuation_yield),
            COMBINED(cdc_valuation_net_price),
			COMBINED(time_to_maturity),
            COMBINED(ttm),
			COMBINED(time_to_maturity_real),
            COMBINED(has_option),
            COMBINED(option_client),
			COMBINED(issuer_rating_filter), 
			COMBINED(guarenteed),
			COMBINED(municipal),
			COMBINED(rate_index),
			COMBINED(pledged),
            COMBINED(across_market),
      		COMBINED_NOCOMPARE(version),
			COMBINED_NOCOMPARE(operation_type),
			COMBINED_NOCOMPARE(create_time),
            COMBINED(completed_bond_type),
            COMBINED(bond_type_sort_client),
			COMBINED(sw_sector),
			COMBINED(sw_subsector),
			COMBINED(sw_sector_code),
			COMBINED(sw_subsector_code),
			COMBINED(province),
			COMBINED(province_code),
			COMBINED(financing_platform),
            COMBINED_NOCOMPARE(across_market_bond_ids),
			COMBINED(company_bond),
			COMBINED(on_the_run),
			COMBINED_NOCOMPARE(outlook) //Add by Young 20-Apr-2016
			)
			);
};

#endif /* BONDSMALLVIEW_CACHE_H_ */
