/*
 * BondQuoteReferCache.h
 *
 *  Created on: Jun 10, 2014
 *      Author: theochen
 */

#ifndef BONDQUOTEREFER_CACHE_H_
#define BONDQUOTEREFER_CACHE_H_

#include "cache_def.h"
#include "bondsmallview_cache.h"
#include "bondtrader_cache.h"
#include "tradinginstitution_cache.h"

class BondQuoteReferCache;
class BondQuoteCache;
typedef std::tr1::shared_ptr<BondQuoteReferCache> BondQuoteReferCachePtr;
typedef std::tr1::shared_ptr<std::vector<BondQuoteReferCachePtr> > BondQuoteReferCacheVecPtr;

class BondQuoteReferCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    BondQuoteReferCache();
    virtual ~BondQuoteReferCache();
	std::string getPrimaryKey();

    void loadCombinationFields();
    void loadBondSmallViewFields();
    void loadTradingInstitutionFields();
    void loadTraderFields();
    void loadBrokerFields();
	void loadCdcValuationFields();

    static BondQuoteReferCachePtr fromBondQuoteCache(BondQuoteCache* ref_cache);

public:
    std::string id;
    std::string company_id;
    std::string bond_key;
    std::string listed_market;
    std::string bond_key_listed_market;
    std::string goods_code;
    std::string goods_id;
    std::string bank_name;
    std::string bank_id;
    int8 symbol;
    real8 price;
    real8 net_price;
    real8 full_price;
    real8 yield;
    real8 volume;
    std::string status;
    real8 spread;
    std::string deal_status;
    std::string deal_time;
    std::string operator_id;
    std::string review_id;
    std::string create_time;
    std::string agent_code;
    std::string rate_type;
    std::string internally;
    std::string goods_short_name;
    real8 return_point;     //返点
    std::string delivery_date; //结算日期
    std::string goods_term;
    std::string goods_level;
    std::string rebate;
    std::string bank_agent_id;
    bool flag_vip;
    bool flag_bad;
    std::string flag_bargain;
    std::string flag_relation; //0：代表不显示，1：显示oco 2:oco打包
    std::string price_description;
    std::string goods_type;
    std::string profit_code;
    std::string description;
    std::string goods_name;
    std::string option_type;
    std::string trader_id;
    std::string quote_type;
    std::string clear_speed; //清算速度  clear_speed（varchar:  T+0,T+1），
    std::string exercise; //行权到期 (varchar  0-行权 1-到期)
    std::string send_status;
    int4 calculate_status;

    // ----------------- Combination Fields ----------------------
    //
    bool isValidBond;

    std::string institution_name;
    std::string trader_name;
    bool   trader_is_vip;
    bool   trader_is_bad;
    std::string broker_name;
    std::string rate_index;
    std::string rate_type_client;
    double coupon_rate;

    std::string bond_id;
    std::string bond_short_name;
    std::string pin_yin;
	std::string pin_yin_full;
    std::string institution_rating;
    std::string bond_category;
    std::string bond_type;
    std::string enterprise_type;
    std::string pledged;
    std::string across_market;
    std::string time_to_maturity;
    double time_to_maturity_real;
    int4   listed_date;
    int4   maturity_date;
    std::string maturity_date_type;
    int maturity_holidays; // 到期日的节假日天数
	int4   filter_maturity_date;
    std::string issuer_rating_filter;
    std::string issuer_rating;
    std::string issuer_rating_npy;
    std::string bond_rating;
    std::string bond_rating_npy;
    int issuer_rating_order;
    int issuer_rating_npy_order;
    int bond_rating_order;
    int bond_rating_npy_order;
    std::string market_type;
    std::string municipal;
    std::string guarenteed;
    std::string has_option;
    std::string option_client;
    std::string on_the_run;
    std::string redemption_no_client;
    int redemption_no;

    std::string cdc_valuation_yield;
    std::string cdc_valuation_net_price;
    double cdc_offset;
	double val_modified_duration;
	double val_convexity;
	double val_basis_point_value;
    std::string guarantee_type;
    std::string issue_institution;
    std::string rating_institution;
    std::string across_market_bond_ids;
    
    time_t create_time_milli_sec;
    time_t deal_time_milli_sec;
	int4 issue_year;
	
	/*******************行业和地区过滤，数据源来自issuer_info_cache**************************/
	//只使用code进行过滤
	std::string sw_sector;
	std::string sw_subsector;
	std::string sw_sector_code;
	std::string sw_subsector_code;
	std::string province;
	std::string province_code;
	std::string financing_platform;
	/*************************************************************************************/

    /*******************ID过滤，数据源来自issuingInstitution**************************/
    std::string issue_institution_id;
    /*************************************************************************************/

	std::string company_bond;
	std::string outlook; //展望 Add by Young 20-Apr-2016
	std::string bond_subtype;
    // ---- non database fields ------------

    std::string modify_time;
	

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
         KEY(company_id, 0/*HASHED | INDEXED*/),
         KEY(bond_key, HASHED /*| INDEXED*/),
         FIELD(listed_market),
         KEY(bond_key_listed_market, HASHED /*| INDEXED*/),
         KEY(goods_code, 0/*HASHED | INDEXED*/),
         KEY(goods_id, 0/*HASHED | INDEXED*/),
         FIELD(bank_name),
         FIELD(bank_id),
         FIELD(symbol),
         FIELD(price),
         FIELD(net_price),
         FIELD(full_price),
         FIELD(yield),
         FIELD(volume),
         FIELD(status),
         FIELD(spread),
         FIELD(deal_status),
         KEY(deal_time, INDEXED),
         FIELD(operator_id),
         FIELD(review_id),
         FIELD(create_time),
         FIELD(agent_code),
         FIELD(rate_type),
         FIELD(internally),
         FIELD(goods_short_name),
         FIELD(return_point),
         FIELD(delivery_date),
         FIELD(goods_term),
         FIELD(goods_level),
         FIELD(rebate),
         FIELD(bank_agent_id),
         FIELD(flag_vip),
         FIELD(flag_bad),
         FIELD(flag_bargain),
         FIELD(flag_relation),
         FIELD(price_description),
         FIELD(goods_type),
         FIELD(profit_code),
         FIELD(description),
         FIELD(goods_name),
         FIELD(option_type),
         FIELD(trader_id),
         FIELD(quote_type),
         FIELD(clear_speed),
         FIELD(exercise),
         FIELD(send_status),
         FIELD(calculate_status),
         COMBINED(institution_name),
         COMBINED(trader_name),
         COMBINED(trader_is_vip),
         COMBINED(trader_is_bad),
         COMBINED(broker_name),
         COMBINED(rate_index),
         COMBINED(rate_type_client),
         COMBINED(coupon_rate),
         COMBINED(bond_id),
         COMBINED(bond_short_name),
		 COMBINED(pin_yin),
		 COMBINED(pin_yin_full),
         COMBINED(institution_rating),
         COMBINED(bond_category),
         COMBINED(bond_type),
         COMBINED(enterprise_type),
         COMBINED(pledged),
         COMBINED(across_market),
         COMBINED(time_to_maturity),
         COMBINED(time_to_maturity_real),
         COMBINED(listed_date),
         COMBINED(maturity_date),
         COMBINED(maturity_date_type),
         COMBINED(maturity_holidays),
		 COMBINED(filter_maturity_date),
         COMBINED(issuer_rating_filter),
         COMBINED(issuer_rating),
         COMBINED(issuer_rating_npy),
         COMBINED(bond_rating),
         COMBINED(bond_rating_npy),
         COMBINED(issuer_rating_order),
         COMBINED(issuer_rating_npy_order),
         COMBINED(bond_rating_order),
         COMBINED(bond_rating_npy_order),
         COMBINED(market_type),
         COMBINED(municipal),
         COMBINED(guarenteed),
         COMBINED(has_option),
         COMBINED(option_client),
         COMBINED(on_the_run),
         COMBINED(redemption_no_client),
         COMBINED(redemption_no),
         COMBINED(cdc_valuation_yield),
         COMBINED(cdc_valuation_net_price),
         COMBINED(cdc_offset),
		 COMBINED(val_modified_duration),
		 COMBINED(val_convexity),
		 COMBINED(val_basis_point_value),
         COMBINED(guarantee_type),
         COMBINED(issue_institution),
         COMBINED(rating_institution),
         COMBINED(across_market_bond_ids),
         COMBINED(create_time_milli_sec),
         COMBINED(deal_time_milli_sec),
		 COMBINED(issue_year),
		 COMBINED(sw_sector),
		 COMBINED(sw_sector_code),
		 COMBINED(sw_subsector),
		 COMBINED(sw_subsector_code),
         COMBINED(province),
         COMBINED(province_code),
		 COMBINED(financing_platform),
         COMBINED(issue_institution_id),
		 COMBINED(company_bond),
		 COMBINED(outlook), //Add by Young 20-Apr-2016
		 COMBINED(bond_subtype)
        )
    );
};

#endif /* BONDQUOTEREFER_CACHE_H_ */
