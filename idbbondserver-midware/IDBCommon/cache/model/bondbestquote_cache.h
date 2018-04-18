/*
 * BondBestQuoteCache.h
 *
 *  Created on: Jun 10, 2014
 *      Author: theochen
 */

#ifndef BONDBESTQUOTE_CACHE_H_
#define BONDBESTQUOTE_CACHE_H_

#include "cache_def.h"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;


class BondBestQuoteCache;
typedef std::tr1::shared_ptr<BondBestQuoteCache> BondBestQuoteCachePtr;
typedef std::tr1::shared_ptr<std::vector<BondBestQuoteCachePtr> > BondBestQuoteCacheVecPtr;

class BondBestQuoteCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	BondBestQuoteCache();
	virtual ~BondBestQuoteCache();
	std::string getPrimaryKey();

    void loadCombinationFields();
    void loadBondSmallViewFields();
    void loadBondQuoteFields();
	void loadCdcValuationFields();
	ptree getRFAJson();

    std::string id;
    std::string company_id;
	std::string goods_id;
	std::string bond_key;
	std::string listed_market;
	std::string bond_key_listed_market;
	std::string goods_code;
	std::string short_name;
	std::string goods_term;
	std::string goods_type;
	std::string goods_level;
	std::string profit_code;
	std::string ofr_agent_code;
	real8 ofr_price;
	std::string ofr_volume;
	bool ofr_flag_vip;
	bool ofr_flag_bad;
	std::string ofr_flag_bargain;
	std::string ofr_flag_relation;
	std::string bid_agent_code;
	real8 bid_price;
	std::string bid_volume;
	bool bid_flag_vip;
	bool bid_flag_bad;
	std::string bid_flag_bargain;
	std::string bid_flag_relation;
	std::string status;
	std::string create_time;
	std::string underwriter;
	std::string underwriter_type;
	std::string bid_price_description;
	std::string ofr_price_description;
	std::string rate_type;
	std::string option_type;
	int8 bid_offer_count;
	int8 ofr_offer_count;
	std::string bid_agent_id;
	std::string ofr_agent_id;
	std::string bid_offer_id;
	std::string ofr_offer_id;
	real8 ofr_before_price;
	real8 bid_before_price;
	std::string internally;
	std::string ofr_quote_type;
	std::string bid_quote_type;

	//BondOffer bidOffer;
	//BondOffer ofrOffer;

	real8 bid_return_point; // 返点
	real8 bid_net_price;
	real8 bid_full_price;
	real8 bid_yield;
	real8 bid_spread;
	std::string bid_clear_speed; // 清算速度
	std::string bid_delivery_date; // 结算日期
	std::string bid_exercise; // 行权到期  exercise (varchar  0-行权 1-到期)
	std::string bid_rebate;
	real8 ofr_return_point; // 返点
	real8 ofr_net_price;
	real8 ofr_full_price;
	real8 ofr_yield;
	real8 ofr_spread;
	std::string ofr_clear_speed; // 清算速度
	std::string ofr_delivery_date;
	std::string ofr_exercise; // 行权到期  exercise (varchar  0-行权 1-到期)
	std::string ofr_rebate;

	//与此最优报价相同产品的报价个数
	int8 bid_offer_num;
	int8 ofr_offer_num;
	double bid_volume_total;
	double ofr_volume_total;
	std::string bid_description;
	std::string ofr_description;

	//添加内部报价字段
	real8 internally_bid_return_point;
	real8 internally_bid_price;
	std::string internally_bid_rebate;
	real8 internally_ofr_return_point;
	real8 internally_ofr_price;
	std::string internally_ofr_rebate;
	std::string internally_bid_offer_id;
	std::string internally_ofr_offer_id;
    int8 internally_bid_offer_count;
    int8 internally_ofr_offer_count;

    // ----------------- Combination Fields ----------------------
    //
    bool isValidBond;

    std::string bid_institution_id;
    std::string ofr_institution_id;
    std::string bid_institution_name;
    std::string ofr_institution_name;
    std::string bid_trader_id;
    std::string ofr_trader_id;
    std::string bid_trader_name;
    std::string ofr_trader_name;
    std::string trader_id_list;
    bool   bid_trader_is_vip;
    bool   ofr_trader_is_vip;
    bool   bid_trader_is_bad;
    bool   ofr_trader_is_bad;
    std::string bid_broker_id;
    std::string ofr_broker_id;
    std::string bid_broker_name;
    std::string ofr_broker_name;
    std::string broker_id_list;
    std::string best_bid_internal_flag;
    std::string best_ofr_internal_flag;
    std::string best_bid_description;
    std::string best_ofr_description;

    std::string rate_index;
    std::string rate_type_client;
    double coupon_rate;

    std::string bond_id;
    std::string bond_short_name;
    std::string pin_yin;
    std::string pin_yin_full;
    std::string institution_rating;
    std::string bond_type;
	std::string bond_type_id;
    std::string enterprise_type;
    std::string pledged;
    std::string across_market;
    std::string time_to_maturity;
    double ttm;
    double time_to_maturity_real;
    int4 maturity_date;
    std::string maturity_date_type;
    int maturity_holidays; // 到期日的节假日天数
	int4 option_date;
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

    std::string cdc_valuation_yield;
    std::string cdc_valuation_net_price;
    double cdc_offset;
	double val_modified_duration;
	double val_convexity;
	double val_basis_point_value;

	std::string guarantee_type;
	std::string issue_institution;
	std::string rating_institution;	

    std::string having_quote;
    std::string having_bid_quote;
    std::string having_ofr_quote;
    std::string two_sided;
    std::string having_external_quote;
    std::string have_internal_quote;
    double bid_ofr_spread;
    std::string redemption_no_client;
    int redemption_no; // > 0 : "提前还本" , 否则: 到期一次还本
	//Non-Dabase Fields
	double vol_ofr_client; //排序vol_ofr
	double vol_bid_client; //排序vol_bid
	real8 ofr_agent_client; //排序ofr
	real8 bid_agent_client; //排序bid
    int8 bond_type_sort_client;  //排序字段，用于完整报价中债券类型 调整数据显示顺序
    std::string bond_category;        //债券种类 信用债 利率债等
    std::string completed_bond_type;  //完整报价的债券类型
    int8   listed_date;
	int4   filter_maturity_date;    
    std::string across_market_bond_ids;
    
    time_t create_time_milli_sec;
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
    
	std::string company_bond; //公司债过滤
	std::string outlook; //展望 Add by Young 20-Apr-2016
	std::string bond_subtype;

public:
    TYPE_DESCRIPTOR(
        (KEY(id, 0/*HASHED | INDEXED*/),
         KEY(company_id, 0/*HASHED | INDEXED*/),
         KEY(goods_id, 0/*HASHED | INDEXED*/),
         KEY(bond_key, HASHED /*| INDEXED*/),
         FIELD(listed_market),
         KEY(bond_key_listed_market, HASHED /*| INDEXED*/),
         KEY(goods_code, 0/*HASHED | INDEXED*/),
         FIELD(short_name),
         FIELD(goods_term),
         FIELD(goods_type),
         FIELD(goods_level),
         FIELD(profit_code),
         FIELD(ofr_agent_code),
         FIELD(ofr_price),
         FIELD(ofr_volume),
         FIELD(ofr_flag_vip),
         FIELD(ofr_flag_bad),
         FIELD(ofr_flag_bargain),
         FIELD(bid_agent_code),
         FIELD(bid_price),
         FIELD(bid_volume),
         FIELD(bid_flag_vip),
         FIELD(bid_flag_bad),
         FIELD(bid_flag_bargain),
         FIELD(status),
         KEY(create_time, INDEXED),
         FIELD(underwriter),
         FIELD(bid_price_description),
         FIELD(ofr_price_description),
         FIELD(underwriter_type),
         FIELD(option_type),
         FIELD(bid_offer_count),
         FIELD(ofr_offer_count),
         FIELD(bid_agent_id),
         FIELD(ofr_agent_id),
         FIELD(rate_type),
         FIELD(bid_offer_id),
         FIELD(ofr_offer_id),
         FIELD(internally_bid_offer_id),
         FIELD(internally_ofr_offer_id),
         FIELD(bid_before_price),
         FIELD(ofr_before_price),
         FIELD(internally),
         FIELD(bid_flag_relation),
         FIELD(ofr_flag_relation),
         FIELD(ofr_quote_type),
         FIELD(bid_quote_type),
         FIELD(bid_return_point),
         FIELD(ofr_return_point),
         FIELD(bid_full_price),
         FIELD(ofr_full_price),
         FIELD(bid_yield),
         FIELD(ofr_yield),
         FIELD(bid_spread),
         FIELD(ofr_spread),
         FIELD(bid_clear_speed),
         FIELD(ofr_clear_speed),
         FIELD(bid_delivery_date),
         FIELD(ofr_delivery_date),
         FIELD(bid_exercise),
         FIELD(ofr_exercise),
         FIELD(bid_rebate),
         FIELD(ofr_rebate),
         FIELD(bid_offer_num),
         FIELD(ofr_offer_num),
         FIELD(bid_net_price),
         FIELD(ofr_net_price),
         FIELD(internally_bid_price),
         FIELD(internally_ofr_price),
         FIELD(internally_bid_return_point),
         FIELD(internally_ofr_return_point),
         FIELD(internally_bid_rebate),
         FIELD(internally_ofr_rebate),
		 FIELD(bid_volume_total),
		 FIELD(ofr_volume_total),
		 FIELD(bid_description),
		 FIELD(ofr_description),

         COMBINED(internally_bid_offer_count),
         COMBINED(internally_ofr_offer_count),
         COMBINED(bid_institution_id),
         COMBINED(ofr_institution_id),
         COMBINED(bid_institution_name),
         COMBINED(ofr_institution_name),
         COMBINED(bid_trader_id),
         COMBINED(ofr_trader_id),
         COMBINED(bid_trader_name),
         COMBINED(ofr_trader_name),
         COMBINED(trader_id_list),
         COMBINED(bid_trader_is_vip),
         COMBINED(ofr_trader_is_vip),
         COMBINED(bid_trader_is_bad),
         COMBINED(ofr_trader_is_bad),
         COMBINED(bid_broker_id),
         COMBINED(ofr_broker_id),
         COMBINED(bid_broker_name),
         COMBINED(ofr_broker_name),
         COMBINED(broker_id_list),
         COMBINED(best_bid_internal_flag),
         COMBINED(best_ofr_internal_flag),
         COMBINED(best_bid_description),
         COMBINED(best_ofr_description),
         COMBINED(rate_index),
         COMBINED(rate_type_client),
         COMBINED(coupon_rate),
         COMBINED(bond_id),
         COMBINED(bond_short_name),
         COMBINED(pin_yin),
         COMBINED(pin_yin_full),
         COMBINED(institution_rating),
         COMBINED(bond_type),
         COMBINED(bond_type_id),
         COMBINED(enterprise_type),
         COMBINED(pledged),
         COMBINED(across_market),
         COMBINED(time_to_maturity),
         COMBINED(ttm),
         COMBINED(time_to_maturity_real),
         COMBINED(maturity_date),
         COMBINED(maturity_date_type),
         COMBINED(maturity_holidays),
         COMBINED(option_date),
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
         COMBINED(cdc_valuation_yield),
         COMBINED(cdc_valuation_net_price),
         COMBINED(cdc_offset),
         COMBINED(val_modified_duration),
         COMBINED(val_convexity),
         COMBINED(val_basis_point_value),
         COMBINED(guarantee_type),
         COMBINED(issue_institution),
         COMBINED(rating_institution),
         COMBINED(bond_type_sort_client),
         COMBINED(having_quote),
         COMBINED(having_bid_quote),
         COMBINED(having_ofr_quote),
         COMBINED(two_sided),
         COMBINED(having_external_quote),
         COMBINED(have_internal_quote),
         COMBINED(bid_ofr_spread),
         COMBINED(redemption_no_client),
         COMBINED(redemption_no),
         COMBINED(bond_category),
         COMBINED(completed_bond_type),
         COMBINED(listed_date),
         COMBINED(filter_maturity_date),
         COMBINED(across_market_bond_ids),
         COMBINED(vol_ofr_client),
         COMBINED(vol_bid_client),
         COMBINED(create_time_milli_sec),
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

#endif /* BONDBESTQUOTE_CACHE_H_ */
