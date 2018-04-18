/*
 * BondBestQuoteCache.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: theochen
 */
#include "bondbestquote_cache.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "include/common.h"
#include <boost/locale/encoding.hpp>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <iomanip>
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;


std::string BondBestQuoteCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BondBestQuoteCache::TABLE_NAME = "bond_best_offer";
std::string BondBestQuoteCache::PKEY_NAME = "id";
std::string BondBestQuoteCache::SQL_TAG = "FetchBondBestQuoteSQL";

BondBestQuoteCache::BondBestQuoteCache() {
    // TODO Auto-generated constructor stub
    // real8
    ofr_price = DOUBLE_NULL;
    bid_price = DOUBLE_NULL;
    ofr_before_price = DOUBLE_NULL;
    bid_before_price = DOUBLE_NULL;
    bid_return_point = DOUBLE_NULL; // 返点
    bid_net_price = DOUBLE_NULL;
    bid_full_price = DOUBLE_NULL;
    bid_yield = DOUBLE_NULL;
    bid_spread = DOUBLE_NULL;
    ofr_return_point = DOUBLE_NULL; // 返点
    ofr_net_price = DOUBLE_NULL;
    ofr_full_price = DOUBLE_NULL;
    ofr_yield = DOUBLE_NULL;
    ofr_spread = DOUBLE_NULL;
    internally_bid_return_point = DOUBLE_NULL;
    internally_bid_price = DOUBLE_NULL;
    internally_ofr_return_point = DOUBLE_NULL;
    internally_ofr_price = DOUBLE_NULL;
    ofr_agent_client = DOUBLE_NULL; //排序ofr
    bid_agent_client = DOUBLE_NULL; //排序bid

    // double
    coupon_rate = DOUBLE_NULL;
    ttm = DOUBLE_NULL;
    time_to_maturity_real = DOUBLE_NULL;
    cdc_offset = DOUBLE_NULL;
    val_modified_duration = DOUBLE_NULL;
    val_convexity = DOUBLE_NULL;
    val_basis_point_value = DOUBLE_NULL;
    bid_ofr_spread = DOUBLE_NULL;
    vol_ofr_client = DOUBLE_NULL; //排序vol_ofr
    vol_bid_client = DOUBLE_NULL; //排序vol_bid
	bid_volume_total = 0;
	ofr_volume_total = 0;

    // bool
    ofr_flag_vip = false;
    ofr_flag_bad = false;
    bid_flag_vip = false;
    bid_flag_bad = false;
    isValidBond = true;
    bid_trader_is_vip = false;
    ofr_trader_is_vip = false;
    bid_trader_is_bad = false;
    ofr_trader_is_bad = false;

    // int8
    bid_offer_count = 0;
    ofr_offer_count = 0;
    bid_offer_num = 0;
    ofr_offer_num = 0;
    internally_bid_offer_count = 0;
    internally_ofr_offer_count = 0;
    bond_type_sort_client = 0;  //排序字段，用于完整报价中债券类型 调整数据显示顺序
    listed_date = -1;

    // int4
    maturity_date = -1;
    option_date = 0;
    filter_maturity_date = -1;
    issue_year = -1;

    // int
    maturity_holidays = -1; // 到期日的节假日天数
    issuer_rating_order = -1;
    issuer_rating_npy_order = -1;
    bond_rating_order = -1;
    bond_rating_npy_order = -11;
    redemption_no = -1; // > 0 : "提前还本" , 否则: 到期一次还本

    // time_t
    create_time_milli_sec = -1;

    // string
    bid_rebate = "0";
    ofr_rebate = "0";
    internally_bid_rebate = "0";
    internally_ofr_rebate = "0";
    bid_volume = "--";
    ofr_volume = "--";
    status = "1";
}

std::string BondBestQuoteCache::getPrimaryKey() {
	return id;
}

BondBestQuoteCache::~BondBestQuoteCache() {
    // TODO Auto-generated destructor stub
}

void BondBestQuoteCache::loadCombinationFields() 
{
    loadBondSmallViewFields();
    loadBondQuoteFields();
	loadCdcValuationFields();
}

void BondBestQuoteCache::loadBondSmallViewFields() 
{
    //BondSmallViewCacheController bond_small_view_cc;
    //dbQuery q;
    //q = "bond_key_listed_market = ", bond_key_listed_market;
    //BondSmallViewCachePtr bond_small_view_cache = bond_small_view_cc.getCacheByQuery(q);
	BondSmallViewCachePtr bond_small_view_cache = BondSmallViewCacheController::getCacheInMem("bond_key_listed_market", bond_key_listed_market);
    if (bond_small_view_cache && !bond_small_view_cache->is_expired) {
        bond_key = bond_small_view_cache->bond_key;
        listed_market = bond_small_view_cache->listed_market;
        goods_code = bond_small_view_cache->bond_id;
        bond_id = bond_small_view_cache->bond_id;
        bond_short_name = bond_small_view_cache->short_name;
        pin_yin = bond_small_view_cache->pin_yin;
        pin_yin_full = bond_small_view_cache->pin_yin_full;
        institution_rating = bond_small_view_cache->institution_rating;
        //cdc_valuation_yield = bond_small_view_cache->cdc_valuation_yield;
        //cdc_valuation_net_price = bond_small_view_cache->cdc_valuation_net_price;
		
        bond_type = bond_small_view_cache->goods_type_client;
		bond_type_id = bond_small_view_cache->bond_goods_type_id;

        enterprise_type = bond_small_view_cache->institution_type;
        pledged = bond_small_view_cache->pledged;
        across_market = bond_small_view_cache->across_market;
        maturity_date = bond_small_view_cache->maturity_date;
        maturity_date_type = bond_small_view_cache->maturity_date_type;
        maturity_holidays = bond_small_view_cache->maturity_holidays;
		option_date = bond_small_view_cache->option_date;
        issuer_rating = bond_small_view_cache->issuer_rating_current;
        issuer_rating_npy = bond_small_view_cache->issuer_rating_current_npy;
        listed_date = bond_small_view_cache->listed_date;
        bond_rating = bond_small_view_cache->rating_current;
        bond_rating_npy = bond_small_view_cache->rating_current_npy;
		issuer_rating_filter = bond_small_view_cache->issuer_rating_filter;

        issuer_rating_order = ParseRatingStr(issuer_rating);
        issuer_rating_npy_order = ParseRatingStr(issuer_rating_npy);
        bond_rating_order = ParseRatingStr(bond_rating);
        bond_rating_npy_order = ParseRatingStr(bond_rating_npy);

        market_type = bond_small_view_cache->mkt_type;
        municipal = bond_small_view_cache->municipal;
		guarenteed = bond_small_view_cache->guarenteed;
                
        option_type = bond_small_view_cache->option_type;
        option_client = bond_small_view_cache->option_client;

        has_option = bond_small_view_cache->has_option;
		filter_maturity_date = has_option=="Y" ? bond_small_view_cache->option_date : bond_small_view_cache->maturity_date;
        on_the_run = CheckOnTheRun(bond_small_view_cache->listed_date);

        rate_index = bond_small_view_cache->rate_index;
        rate_type_client = bond_small_view_cache->coupon_type_client;
        time_to_maturity = bond_small_view_cache->time_to_maturity;
        ttm = bond_small_view_cache->ttm;
        time_to_maturity_real = bond_small_view_cache->time_to_maturity_real;
        coupon_rate = bond_small_view_cache->coupon_rate_current;
		
		guarantee_type = bond_small_view_cache->rating_augment_client;
		issue_institution = bond_small_view_cache->issuer_client;
		rating_institution = bond_small_view_cache->rating_institution_client;
        completed_bond_type = bond_small_view_cache->completed_bond_type;
        bond_type_sort_client = bond_small_view_cache->bond_type_sort_client;
        bond_category = bond_small_view_cache->product_type;
        redemption_no_client = bond_small_view_cache->redemption_no_client;
        redemption_no = bond_small_view_cache->redemption_no;
		issue_year = bond_small_view_cache->issue_year;
		sw_sector_code = bond_small_view_cache->sw_sector_code;
		sw_subsector_code = bond_small_view_cache->sw_subsector_code;
        province_code = bond_small_view_cache->province_code;
		financing_platform = bond_small_view_cache->financing_platform;
        issue_institution_id = bond_small_view_cache->issue_institution_id;
        across_market_bond_ids = bond_small_view_cache->across_market_bond_ids;
		company_bond = bond_small_view_cache->ent_cor;
		outlook = bond_small_view_cache->outlook; //Add by Young 20-Apr-2016
		bond_subtype = bond_small_view_cache->bond_subtype;
    } else {
        isValidBond = false;
        bond_id = "";
        bond_short_name = "";
        pin_yin = "";
        pin_yin_full = "";
        institution_rating = "";
        bond_type = "";
		bond_type_id = "";
        enterprise_type = "";
        pledged = "N";
        across_market = "N";
        maturity_date = -1;
        maturity_date_type = kMaturityDateNormal;
		option_date = 0;
        issuer_rating = "";
        issuer_rating_npy = "";
        listed_date = -1;
		filter_maturity_date = -1;
        bond_rating = "";
        bond_rating_npy = "";
        issuer_rating_order = 0;
        issuer_rating_npy_order = 0;
        bond_rating_order = 0;
        bond_rating_npy_order = 0;
        issuer_rating_filter = "Other";
        market_type = "";
        municipal = "N";
        guarenteed = "N";
        bond_type = "";
		bond_type_id = "";
        has_option = "N";
        on_the_run = "N";
        rate_index = "N";
        rate_type_client = "";
        time_to_maturity = "";
        ttm = 0.0;
        time_to_maturity_real = 0.0;
        coupon_rate = 0.0;
		guarantee_type = "";
		issue_institution = "";
		rating_institution = "";
        completed_bond_type = "";
        bond_type_sort_client = 10;
        bond_category = "";
        redemption_no_client = "";
		//cdc_valuation_yield = "";
		//cdc_valuation_net_price = "";
		//cdc_offset = DOUBLE_NULL;
		issue_year = 2000;
		sw_sector_code = "";
		sw_subsector_code = "";
        province_code = "";
		financing_platform = "N";
        issue_institution_id = "";
        across_market_bond_ids = "";
		company_bond = "";
		outlook = ""; //Add by Young 20-Apr-2016
		bond_subtype = "";
    }
}

void BondBestQuoteCache::loadBondQuoteFields()
{
    //BondQuoteCacheController bond_quote_cc;
    //dbQuery q;
    //q = "id = ", bid_offer_id;
    //BondQuoteCachePtr bid_bond_quote_cache = bond_quote_cc.getCacheByQuery(q);
	BondQuoteCachePtr bid_bond_quote_cache = BondQuoteCacheController::getCacheInMem("id", bid_offer_id);
    if (bid_bond_quote_cache) {
        bid_institution_id      = bid_bond_quote_cache->bank_id;
        bid_institution_name    = bid_bond_quote_cache->institution_name;
        bid_trader_id           = bid_bond_quote_cache->bank_agent_id;
        bid_trader_name         = bid_bond_quote_cache->trader_name;
        bid_trader_is_vip       = bid_bond_quote_cache->trader_is_vip;
        bid_trader_is_bad       = bid_bond_quote_cache->trader_is_bad;
        bid_broker_id           = bid_bond_quote_cache->operator_id;
        bid_broker_name         = bid_bond_quote_cache->broker_name;
        best_bid_internal_flag  = bid_bond_quote_cache->internally;
        best_bid_description    = bid_bond_quote_cache->price_description;
    } else {
        bid_institution_id      = "";
        bid_institution_name    = "";
        bid_trader_id           = "";
        bid_trader_name         = "";
        bid_trader_is_vip       = false;
        bid_trader_is_bad       = false;
        bid_broker_id           = "";
        bid_broker_name         = "";
        best_bid_internal_flag  = "";
        best_bid_description    = "";
    }

    //q = "id = ", ofr_offer_id;
    //BondQuoteCachePtr ofr_bond_quote_cache = bond_quote_cc.getCacheByQuery(q);
	BondQuoteCachePtr ofr_bond_quote_cache = BondQuoteCacheController::getCacheInMem("id", ofr_offer_id);
    if (ofr_bond_quote_cache) {
        ofr_institution_id      = ofr_bond_quote_cache->bank_id;
        ofr_institution_name    = ofr_bond_quote_cache->institution_name;
        ofr_trader_id           = ofr_bond_quote_cache->bank_agent_id;
        ofr_trader_name         = ofr_bond_quote_cache->trader_name;
        ofr_trader_is_vip       = ofr_bond_quote_cache->trader_is_vip;
        ofr_trader_is_bad       = ofr_bond_quote_cache->trader_is_bad;
        ofr_broker_id           = ofr_bond_quote_cache->operator_id;
        ofr_broker_name         = ofr_bond_quote_cache->broker_name;
        best_ofr_internal_flag  = ofr_bond_quote_cache->internally;
        best_ofr_description    = ofr_bond_quote_cache->price_description;

        cdc_offset              = ofr_bond_quote_cache->cdc_offset;     // 中债偏值只看ofr
    } else {
        ofr_institution_id      = "";
        ofr_institution_name    = "";
        ofr_trader_id           = "";
        ofr_trader_name         = "";
        ofr_trader_is_vip       = false;
        ofr_trader_is_bad       = false;
        ofr_broker_id           = "";
        ofr_broker_name         = "";
        best_ofr_internal_flag  = "";
        best_ofr_description    = "";
    }

    //q = "id = ", internally_bid_offer_id;
    //BondQuoteCachePtr internally_bid_bond_quote_cache = bond_quote_cc.getCacheByQuery(q);
	BondQuoteCachePtr internally_bid_bond_quote_cache = BondQuoteCacheController::getCacheInMem("id", internally_bid_offer_id);

    //q = "id = ", internally_ofr_offer_id;
    //BondQuoteCachePtr internally_ofr_bond_quote_cache = bond_quote_cc.getCacheByQuery(q);
	BondQuoteCachePtr internally_ofr_bond_quote_cache = BondQuoteCacheController::getCacheInMem("id", internally_ofr_offer_id);

    if (bid_bond_quote_cache || ofr_bond_quote_cache || internally_bid_bond_quote_cache || internally_ofr_bond_quote_cache) {
        having_quote = "Y";
    } else {
        having_quote = "N";
    }

    // 双边报价过滤包含内部报价 huameng.jia 2015.11.06
    if ((bid_bond_quote_cache || internally_bid_bond_quote_cache) && (ofr_bond_quote_cache || internally_ofr_bond_quote_cache)) {
        two_sided = "Y";
        double bid_yield = DOUBLE_NULL, ofr_yield = DOUBLE_NULL;

		if (bid_bond_quote_cache) {
			if (bid_bond_quote_cache->yield != DOUBLE_NULL) {
				bid_yield = bid_bond_quote_cache->yield;
			} else if (bid_bond_quote_cache->price != DOUBLE_NULL && bid_bond_quote_cache->quote_type == "0") {
				bid_yield = bid_bond_quote_cache->price;
			}
			bid_quote_type = bid_bond_quote_cache->quote_type;
		}else if (internally_bid_bond_quote_cache) {
            if (internally_bid_bond_quote_cache->yield != DOUBLE_NULL) {
                bid_yield = internally_bid_bond_quote_cache->yield;
            } else if (internally_bid_bond_quote_cache->price != DOUBLE_NULL && internally_bid_bond_quote_cache->quote_type == "0") {
                bid_yield = internally_bid_bond_quote_cache->price;
            }
			bid_quote_type = internally_bid_bond_quote_cache->quote_type;
        }

		if (ofr_bond_quote_cache) {
			if (ofr_bond_quote_cache->yield != DOUBLE_NULL) {
				ofr_yield = ofr_bond_quote_cache->yield;
			} else if (ofr_bond_quote_cache->price != DOUBLE_NULL && ofr_bond_quote_cache->quote_type == "0") {
				ofr_yield = ofr_bond_quote_cache->price;
			}
			ofr_quote_type = ofr_bond_quote_cache->quote_type;
		}else if (internally_ofr_bond_quote_cache) {
            if (internally_ofr_bond_quote_cache->yield != DOUBLE_NULL) {
                ofr_yield = internally_ofr_bond_quote_cache->yield;
            } else if (internally_ofr_bond_quote_cache->price != DOUBLE_NULL && internally_ofr_bond_quote_cache->quote_type == "0") {
                ofr_yield = internally_ofr_bond_quote_cache->price;
            }
			ofr_quote_type = internally_ofr_bond_quote_cache->quote_type;
        }

        if (bid_yield != DOUBLE_NULL && ofr_yield != DOUBLE_NULL) {
            bid_ofr_spread = (bid_yield - ofr_yield) * 100;
        }
    } else {
        two_sided = "N";
    }

    if (bid_bond_quote_cache || ofr_bond_quote_cache) {
        having_external_quote = "Y";
    } else {
        having_external_quote = "N";
    }

    if (internally_bid_bond_quote_cache || internally_ofr_bond_quote_cache) {
        have_internal_quote = "Y";
    } else {
        have_internal_quote = "N";
    }

    if (bid_bond_quote_cache) {
        having_bid_quote = "Y";
    } else {
        having_bid_quote = "N";
    }
    
    if (ofr_bond_quote_cache) {
        having_ofr_quote = "Y";
    } else {
        having_ofr_quote = "N";
    }
 }

void BondBestQuoteCache::loadCdcValuationFields(){
	cdc_valuation_yield = "";
	cdc_valuation_net_price = "";
	cdc_offset = DOUBLE_NULL;
	if (bond_key_listed_market.empty()) {
		return;
	}
	//ChinaBondValuationCacheController cbvcc;
	//ChinaBondValuationCachePtr cc = cbvcc.getValuation(bond_id);

	//ChinaBondValuationCacheController chinaBondValuationCC;
	//std::string datetime = GetTDateString("%Y%m%d");
	//dbQuery q;
	//q = "bond_key_listed_market = ", bond_key_listed_market, " and valuation_date = ", datetime, " order by valuation_date desc limit 1";
	//ChinaBondValuationCachePtr cache = chinaBondValuationCC.getCacheByQuery(q);
	ChinaBondValuationCachePtr cache = ChinaBondValuationCacheController::getMemValuation(bond_key_listed_market);
	if (cache == NULL || cache.get() == NULL || cache.get()->id.empty()) {
		return;
	}
	cdc_valuation_yield = cache->valuation_yield;
	cdc_valuation_net_price = cache->valuation_net_price;
	if(cdc_valuation_yield.length() > 0){ // 中债偏移只考虑ofr
		double tCdcValuationYield = DOUBLE_NULL;
		if("Y" == has_option && cdc_valuation_yield.find("|") != std::string::npos){
			tCdcValuationYield = atof(cdc_valuation_yield.substr(0, cdc_valuation_yield.find_first_of("|")).c_str());
		}else{
			tCdcValuationYield = atof(cdc_valuation_yield.c_str());
		}
		if(ofr_yield != DOUBLE_NULL){
			cdc_offset = (ofr_yield - tCdcValuationYield) * 100;
		}else if(ofr_price != DOUBLE_NULL && ofr_quote_type == "3"){
			cdc_offset = (ofr_price - tCdcValuationYield) * 100;
		}
	}
	//val_modified_duration = cc->val_modified_duration;	//修正久期
    
    if (fabs(cache->val_modified_duration - 0) <= FLOATING_PRECISION) {
        val_modified_duration = DOUBLE_NULL;
    } else {
        val_modified_duration = cache->val_modified_duration;
    }

	val_convexity = cache->val_convexity;
	val_basis_point_value = cache->val_basis_point_value;
}

ptree BondBestQuoteCache::getRFAJson() {

	//BondSmallViewCacheController bond_small_view_cc;
	//dbQuery q;
	//q = "bond_key_listed_market = ", bond_key_listed_market;
	//BondSmallViewCachePtr bond_small_view_cache = bond_small_view_cc.getCacheByQueryInThreadSafty(q);
	BondSmallViewCachePtr bond_small_view_cache = BondSmallViewCacheController::getCacheInMem("bond_key_listed_market", bond_key_listed_market);

	//BondDealCacheController bond_deal_cc;
	//q = "bond_key_listed_market = ", bond_key_listed_market, " order by create_time desc";
	//BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);
	BondDealCachePtr bond_deal_cache = BondDealCacheController::getMemDeal(bond_key_listed_market, company_id);
	ptree bondBestOffer;
    if (this->bid_offer_id.length() > 0) {
        bondBestOffer.put("bidAgentCode", this->bid_trader_name);
        bondBestOffer.put("bidAgentId", this->bid_trader_id);
        bondBestOffer.put("bidBeforePrice", this->bid_before_price);
        bondBestOffer.put("bidBrokerId", this->bid_broker_id);
        bondBestOffer.put("bidDeliveryDate", ParseTimeString(this->bid_delivery_date)*1000);
        bondBestOffer.put("bidExercise", this->bid_exercise);
        bondBestOffer.put("bidFlagBad", this->bid_flag_bad);
        bondBestOffer.put("bidFlagBargain", this->bid_flag_bargain);
        bondBestOffer.put("bidFlagRelation", this->bid_flag_relation);
        bondBestOffer.put("bidFlagVip", this->bid_trader_is_vip);
        bondBestOffer.put("bidFullPrice", this->bid_full_price);
        bondBestOffer.put("bidNetPrice", this->bid_net_price);
        bondBestOffer.put("bidOfferCount", this->bid_offer_count);
        bondBestOffer.put("bidOfferId", this->bid_offer_id);
        bondBestOffer.put("bidOfferNum", this->bid_offer_num);
        bondBestOffer.put("bidPrice", this->bid_price == DOUBLE_NULL ? "" : Format(this->bid_price, 2, 4));
        bondBestOffer.put("bidPriceDescription", this->bid_price_description);
        bondBestOffer.put("bidQuoteType", this->bid_quote_type);
        bondBestOffer.put("bidRebate", this->bid_rebate);
        bondBestOffer.put("bidVolume", boost::replace_all_copy(this->bid_volume, "--", "0"));
        bondBestOffer.put("bidYield", this->bid_yield == DOUBLE_NULL ? "" : Format(this->bid_yield, 2, 4));
        bondBestOffer.put("bidNetPrice", this->bid_net_price == DOUBLE_NULL ? "" : Format(this->bid_net_price, 2, 4));
        bondBestOffer.put("bidFullPrice", this->bid_full_price == DOUBLE_NULL ? "" : Format(this->bid_full_price, 2, 4));

    }
	bondBestOffer.put("bondKey", this->bond_key);
	bondBestOffer.put("bondKeyListedMarket", this->bond_key_listed_market);
	bondBestOffer.put("createTime", ParseTimeString(this->create_time) * 1000);	
    if (bond_deal_cache) {
	    bondBestOffer.put("dealPrice", bond_deal_cache->price); 
	    bondBestOffer.put("dealTime", ParseTimeString(bond_deal_cache->deal_time) * 1000);
	    bondBestOffer.put("dealType", bond_deal_cache->deal_type);
	    bondBestOffer.put("dealVolume", bond_deal_cache->volume);
        bondBestOffer.put("dealYield", bond_deal_cache->yield);
        bondBestOffer.put("dealNetPrice", bond_deal_cache->net_price);
        bondBestOffer.put("dealFullPrice", bond_deal_cache->full_price);
	}
	bondBestOffer.put("endTime", ""); // 不清楚
	bondBestOffer.put("goodsCode", this->bond_id);
	bondBestOffer.put("goodsId", "");
	bondBestOffer.put("goodsLevel", this->bond_rating);
	bondBestOffer.put("goodsTerm", this->maturity_date);
	bondBestOffer.put("goodsType", this->bond_type);
    bondBestOffer.put("id", this->id);
    bondBestOffer.put("companyId", this->company_id);
	bondBestOffer.put("internally", this->internally);
	bondBestOffer.put("internallyBidRebate", this->internally_bid_rebate);
	bondBestOffer.put("internallyOfrRebate", this->internally_ofr_rebate);
	bondBestOffer.put("listedMarket", this->listed_market);
	bondBestOffer.put("mktType", this->market_type);

    if (this->ofr_offer_id.length() > 0) {
        bondBestOffer.put("ofrAgentCode", this->ofr_trader_name);
        bondBestOffer.put("ofrAgentId", this->ofr_trader_id);
        bondBestOffer.put("ofrBeforePrice", this->ofr_before_price);
        bondBestOffer.put("ofrBrokerId", this->ofr_broker_id);
        bondBestOffer.put("ofrDeliveryDate", ParseTimeString(this->ofr_delivery_date)*1000);
        bondBestOffer.put("ofrExercise", this->ofr_exercise);
        bondBestOffer.put("ofrFlagBad", this->ofr_flag_bad);
        bondBestOffer.put("ofrFlagBargain", this->ofr_flag_bargain);
        bondBestOffer.put("ofrFlagRelation", this->ofr_flag_relation);
        bondBestOffer.put("ofrFlagVip", this->ofr_trader_is_vip);
        bondBestOffer.put("ofrFullPrice", this->ofr_full_price);
        bondBestOffer.put("ofrNetPrice", this->ofr_net_price);
        bondBestOffer.put("ofrOfferCount", this->ofr_offer_count);
        bondBestOffer.put("ofrOfferId", this->ofr_offer_id);
        bondBestOffer.put("ofrOfferNum", this->ofr_offer_num);
        bondBestOffer.put("ofrPrice", this->ofr_price == DOUBLE_NULL ? "" : Format(this->ofr_price, 2, 4));
        bondBestOffer.put("ofrPriceDescription", this->ofr_price_description);
        bondBestOffer.put("ofrQuoteType", this->ofr_quote_type);
        bondBestOffer.put("ofrRebate", this->ofr_rebate);
        bondBestOffer.put("ofrVolume", boost::replace_all_copy(this->ofr_volume, "--", "0"));
        bondBestOffer.put("ofrYield", this->ofr_yield == DOUBLE_NULL ? "" : Format(this->ofr_yield, 2, 4));
        bondBestOffer.put("ofrNetPrice", this->ofr_net_price == DOUBLE_NULL ? "" : Format(this->ofr_net_price, 2, 4));
        bondBestOffer.put("ofrFullPrice", this->ofr_full_price == DOUBLE_NULL ? "" : Format(this->ofr_full_price, 2, 4));
    }

	bondBestOffer.put("rateFlag", ""); // 不清楚
	bondBestOffer.put("rateType", this->rate_type_client);

	bondBestOffer.put("shortName", this->bond_short_name);

	bondBestOffer.put("status", "1");
	bondBestOffer.put("underwriter", this->underwriter);
	bondBestOffer.put("underwriterType", this->underwriter_type);

	ptree bidOffer;
	//BondQuoteCacheController bond_quote_cc;
	//q = "id = ", bid_offer_id;
	//BondQuoteCachePtr bid_bond_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);
	BondQuoteCachePtr bid_bond_quote_cache = BondQuoteCacheController::getCacheInMem("id", bid_offer_id);
	if (bid_bond_quote_cache) {
		bidOffer.put("agentCode", bid_bond_quote_cache->trader_name);
		bidOffer.put("bankAgentId", bid_bond_quote_cache->bank_agent_id);
		bidOffer.put("bankId", bid_bond_quote_cache->bank_id);
		bidOffer.put("bankName", bid_bond_quote_cache->institution_name);
		bidOffer.put("bondKey", bid_bond_quote_cache->bond_key);
		bidOffer.put("bondKeyListedMarket", bid_bond_quote_cache->bond_key_listed_market);
		bidOffer.put("calculateStatus", bid_bond_quote_cache->calculate_status);
		bidOffer.put("createTime", ParseTimeString(bid_bond_quote_cache->create_time) * 1000);
		bidOffer.put("dealStatus", bid_bond_quote_cache->deal_status);
		bidOffer.put("deliveryDate", bid_bond_quote_cache->delivery_date);
		bidOffer.put("exercise", bid_bond_quote_cache->exercise);
		bidOffer.put("flagBad", bid_bond_quote_cache->trader_is_bad);
		bidOffer.put("flagBargain", bid_bond_quote_cache->flag_bargain);
		bidOffer.put("flagRelation", bid_bond_quote_cache->flag_relation);
		bidOffer.put("flagVip", bid_bond_quote_cache->trader_is_vip);
		bidOffer.put("fullPrice", bid_bond_quote_cache->full_price);
		bidOffer.put("goodsCode", bid_bond_quote_cache->bond_id);
		bidOffer.put("goodsId", bid_bond_quote_cache->goods_id);
		bidOffer.put("goodsLevel", bid_bond_quote_cache->bond_rating);
		bidOffer.put("goodsShortName", bid_bond_quote_cache->bond_short_name);
		bidOffer.put("goodsTerm", bid_bond_quote_cache->maturity_date);
		bidOffer.put("goodsType", bid_bond_quote_cache->bond_type);
		bidOffer.put("id", bid_bond_quote_cache->id);
		bidOffer.put("internally", bid_bond_quote_cache->internally);
		bidOffer.put("listedMarket", bid_bond_quote_cache->listed_market);
		bidOffer.put("netPrice", bid_bond_quote_cache->net_price);
		bidOffer.put("operatorId", bid_bond_quote_cache->operator_id);
		bidOffer.put("price", bid_bond_quote_cache->price == DOUBLE_NULL ? "" : Format(bid_bond_quote_cache->price, 2, 4));
		bidOffer.put("priceDescription", bid_bond_quote_cache->price_description);
		bidOffer.put("profitCode", bid_bond_quote_cache->profit_code);
		bidOffer.put("quoteType", bid_bond_quote_cache->quote_type);
		bidOffer.put("rateFlag", "");
		bidOffer.put("rateType", bid_bond_quote_cache->rate_type_client);
		bidOffer.put("rebate", bid_bond_quote_cache->rebate);
		bidOffer.put("status", "1");
		bidOffer.put("symbol", "1");
		bidOffer.put("traderId", bid_bond_quote_cache->trader_id);
		bidOffer.put("volume", bid_bond_quote_cache->volume);
		bidOffer.put("yield", bid_bond_quote_cache->yield == DOUBLE_NULL ? "" : Format(bid_bond_quote_cache->yield, 2, 4));

		ptree smallDto;
		if (bond_small_view_cache) {
			smallDto.put("bondId", bond_small_view_cache->bond_id);
			smallDto.put("bondKey", bond_small_view_cache->bond_key);
			smallDto.put("bondKeyListedMarket", bond_small_view_cache->bond_key_listed_market);
			smallDto.put("bondSubtype", bond_small_view_cache->bond_subtype);
			smallDto.put("couponRateCurrent", bond_small_view_cache->coupon_rate_current);
			smallDto.put("couponRateCurrentClient", bond_small_view_cache->coupon_rate_current_client);
			smallDto.put("couponRateSpread", bond_small_view_cache->coupon_rate_spread);
			smallDto.put("couponRateSpreadClient", bond_small_view_cache->coupon_rate_spread_client);
			smallDto.put("couponType", bond_small_view_cache->coupon_type);
			smallDto.put("couponTypeClient", bond_small_view_cache->coupon_type_client);
			smallDto.put("createDate", bond_small_view_cache->create_date);
			smallDto.put("delflag", bond_small_view_cache->delflag);
			smallDto.put("delistedDate", bond_small_view_cache->delisted_date);
			smallDto.put("goodsTerm", bond_small_view_cache->maturity_term_client);
			smallDto.put("goodsTypeClient", bond_small_view_cache->goods_type_client);
			smallDto.put("institutionFullNameC", bond_small_view_cache->issuer_client);
			smallDto.put("institutionPinYin", bond_small_view_cache->issue_institution_pinyin_);
			smallDto.put("institutionPinYinFull", bond_small_view_cache->issue_institution_pinyinfull_);
			smallDto.put("institutionSubtype", bond_small_view_cache->institution_type);  //subtype 就是 type
			smallDto.put("institutionType", bond_small_view_cache->institution_type);	  //这里type不用了
			smallDto.put("interestStartDate", bond_small_view_cache->interest_start_date);
			smallDto.put("isCrossMkt", bond_small_view_cache->is_cross_mkt);
			smallDto.put("isMortgage", bond_small_view_cache->is_mortgage);
			smallDto.put("isMunicipal", bond_small_view_cache->is_municipal);
			smallDto.put("issueAmount", bond_small_view_cache->issue_amount);
			smallDto.put("issuerCode", bond_small_view_cache->issuer_code);
			smallDto.put("issuerCodeClient", bond_small_view_cache->issuer_client);
			smallDto.put("issuerRatingCurrent", bond_small_view_cache->issuer_rating_current);
			smallDto.put("issuerRatingCurrentNPY", bond_small_view_cache->issuer_rating_current_npy);
			smallDto.put("issuerRatingInstitutionCode", bond_small_view_cache->issuer_rating_institution_code);
			smallDto.put("issuerRatingInstitutionCodeClient", bond_small_view_cache->issuer_rating_institution_client);
			smallDto.put("listedDate", bond_small_view_cache->listed_date);
			smallDto.put("listedMarket", bond_small_view_cache->listed_market);
			smallDto.put("maturityDate", bond_small_view_cache->maturity_date);
			smallDto.put("maturityTerm", bond_small_view_cache->maturity_term);
			smallDto.put("maturityTermClient", bond_small_view_cache->maturity_term_client);
			smallDto.put("mktType", bond_small_view_cache->mkt_type);
			smallDto.put("nextCouponDate", bond_small_view_cache->next_coupon_date);
			smallDto.put("optionType", bond_small_view_cache->option_type);
			smallDto.put("pinyin", bond_small_view_cache->pin_yin);
			smallDto.put("pinyinFull", bond_small_view_cache->pin_yin_full);
			smallDto.put("productType", bond_small_view_cache->product_type);
			smallDto.put("ratingAugment", bond_small_view_cache->rating_augment);
			smallDto.put("ratingAugmentClient", bond_small_view_cache->rating_augment_client);
			smallDto.put("ratingCurrent", bond_small_view_cache->rating_current);
			smallDto.put("ratingCurrentNPY", bond_small_view_cache->rating_current_npy);
			smallDto.put("ratingDate", bond_small_view_cache->rating_date);
			smallDto.put("ratingInstitutionCode", bond_small_view_cache->rating_institution_code);
			smallDto.put("ratingInstitutionCodeClient", bond_small_view_cache->rating_institution_client);
			smallDto.put("redemptionNoClient", bond_small_view_cache->redemption_no_client);
			smallDto.put("shortName", bond_small_view_cache->short_name);
			smallDto.put("sortRemain", bond_small_view_cache->sort_remain);
			smallDto.put("termUnit", bond_small_view_cache->term_unit);
			smallDto.put("warranter", bond_small_view_cache->warranter);
			smallDto.put("warranterClient", bond_small_view_cache->warranter_client);
		}
		bidOffer.put_child("smallDto", smallDto);
	}

	ptree ofrOffer;
	//q = "id = ", ofr_offer_id;
	//BondQuoteCachePtr ofr_bond_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);
	BondQuoteCachePtr ofr_bond_quote_cache = BondQuoteCacheController::getCacheInMem("id", ofr_offer_id);
	if (ofr_bond_quote_cache) {
		ofrOffer.put("agentCode", ofr_bond_quote_cache->trader_name);
		ofrOffer.put("bankAgentId", ofr_bond_quote_cache->bank_agent_id);
		ofrOffer.put("bankId", ofr_bond_quote_cache->bank_id);
		ofrOffer.put("bankName", ofr_bond_quote_cache->institution_name);
		ofrOffer.put("bondKey", ofr_bond_quote_cache->bond_key);
		ofrOffer.put("bondKeyListedMarket", ofr_bond_quote_cache->bond_key_listed_market);
		ofrOffer.put("calculateStatus", ofr_bond_quote_cache->calculate_status);
		ofrOffer.put("createTime", ParseTimeString(ofr_bond_quote_cache->create_time) * 1000);
		ofrOffer.put("dealStatus", ofr_bond_quote_cache->deal_status);
		ofrOffer.put("deliveryDate", ofr_bond_quote_cache->delivery_date);
		ofrOffer.put("exercise", ofr_bond_quote_cache->exercise);
		ofrOffer.put("flagBad", ofr_bond_quote_cache->trader_is_bad);
		ofrOffer.put("flagBargain", ofr_bond_quote_cache->flag_bargain);
		ofrOffer.put("flagRelation", ofr_bond_quote_cache->flag_relation);
		ofrOffer.put("flagVip", ofr_bond_quote_cache->trader_is_vip);
		ofrOffer.put("fullPrice", ofr_bond_quote_cache->full_price);
		ofrOffer.put("goodsCode", ofr_bond_quote_cache->bond_id);
		ofrOffer.put("goodsId", ofr_bond_quote_cache->goods_id);
		ofrOffer.put("goodsLevel", ofr_bond_quote_cache->bond_rating);
		ofrOffer.put("goodsShortName", ofr_bond_quote_cache->bond_short_name);
		ofrOffer.put("goodsTerm", ofr_bond_quote_cache->maturity_date);
		ofrOffer.put("goodsType", ofr_bond_quote_cache->bond_type);
		ofrOffer.put("id", ofr_bond_quote_cache->id);
		ofrOffer.put("internally", ofr_bond_quote_cache->internally);
		ofrOffer.put("listedMarket", ofr_bond_quote_cache->listed_market);
		ofrOffer.put("netPrice", ofr_bond_quote_cache->net_price);
		ofrOffer.put("operatorId", ofr_bond_quote_cache->operator_id);
		ofrOffer.put("price", ofr_bond_quote_cache->price == DOUBLE_NULL ? "" : Format(ofr_bond_quote_cache->price, 2, 4));
		ofrOffer.put("priceDescription", ofr_bond_quote_cache->price_description);
		ofrOffer.put("profitCode", ofr_bond_quote_cache->profit_code);
		ofrOffer.put("quoteType", ofr_bond_quote_cache->quote_type);
		ofrOffer.put("rateFlag", "");
		ofrOffer.put("rateType", ofr_bond_quote_cache->rate_type_client);
		ofrOffer.put("rebate", ofr_bond_quote_cache->rebate);
		ofrOffer.put("status", "1");
		ofrOffer.put("symbol", "-1");
		ofrOffer.put("traderId", ofr_bond_quote_cache->trader_id);
		ofrOffer.put("volume", ofr_bond_quote_cache->volume);
		ofrOffer.put("yield", ofr_bond_quote_cache->yield == DOUBLE_NULL ? "" : Format(ofr_bond_quote_cache->yield, 2, 4));

		ptree smallDto;
		if (bond_small_view_cache) {
			smallDto.put("bondId", bond_small_view_cache->bond_id);
			smallDto.put("bondKey", bond_small_view_cache->bond_key);
			smallDto.put("bondKeyListedMarket", bond_small_view_cache->bond_key_listed_market);
			smallDto.put("bondSubtype", bond_small_view_cache->bond_subtype);
			smallDto.put("couponRateCurrent", bond_small_view_cache->coupon_rate_current);
			smallDto.put("couponRateCurrentClient", bond_small_view_cache->coupon_rate_current_client);
			smallDto.put("couponRateSpread", bond_small_view_cache->coupon_rate_spread);
			smallDto.put("couponRateSpreadClient", bond_small_view_cache->coupon_rate_spread_client);
			smallDto.put("couponType", bond_small_view_cache->coupon_type);
			smallDto.put("couponTypeClient", bond_small_view_cache->coupon_type_client);
			smallDto.put("createDate", bond_small_view_cache->create_date);
			smallDto.put("delflag", bond_small_view_cache->delflag);
			smallDto.put("delistedDate", bond_small_view_cache->delisted_date);
			smallDto.put("goodsTerm", bond_small_view_cache->maturity_term_client);
			smallDto.put("goodsTypeClient", bond_small_view_cache->goods_type_client);
			smallDto.put("institutionFullNameC", bond_small_view_cache->issuer_client);
			smallDto.put("institutionPinYin", bond_small_view_cache->issue_institution_pinyin_);
			smallDto.put("institutionPinYinFull", bond_small_view_cache->issue_institution_pinyinfull_);
			smallDto.put("institutionSubtype", bond_small_view_cache->institution_type);  //subtype 就是 type
			smallDto.put("institutionType", bond_small_view_cache->institution_type);	  //这里type不用了
			smallDto.put("interestStartDate", bond_small_view_cache->interest_start_date);
			smallDto.put("isCrossMkt", bond_small_view_cache->is_cross_mkt);
			smallDto.put("isMortgage", bond_small_view_cache->is_mortgage);
			smallDto.put("isMunicipal", bond_small_view_cache->is_municipal);
			smallDto.put("issueAmount", bond_small_view_cache->issue_amount);
			smallDto.put("issuerCode", bond_small_view_cache->issuer_code);
			smallDto.put("issuerCodeClient", bond_small_view_cache->issuer_client);
			smallDto.put("issuerRatingCurrent", bond_small_view_cache->issuer_rating_current);
			smallDto.put("issuerRatingCurrentNPY", bond_small_view_cache->issuer_rating_current_npy);
			smallDto.put("issuerRatingInstitutionCode", bond_small_view_cache->issuer_rating_institution_code);
			smallDto.put("issuerRatingInstitutionCodeClient", bond_small_view_cache->issuer_rating_institution_client);
			smallDto.put("listedDate", bond_small_view_cache->listed_date);
			smallDto.put("listedMarket", bond_small_view_cache->listed_market);
			smallDto.put("maturityDate", bond_small_view_cache->maturity_date);
			smallDto.put("maturityTerm", bond_small_view_cache->maturity_term);
			smallDto.put("maturityTermClient", bond_small_view_cache->maturity_term_client);
			smallDto.put("mktType", bond_small_view_cache->mkt_type);
			smallDto.put("nextCouponDate", bond_small_view_cache->next_coupon_date);
			smallDto.put("optionType", bond_small_view_cache->option_type);
			smallDto.put("pinyin", bond_small_view_cache->pin_yin);
			smallDto.put("pinyinFull", bond_small_view_cache->pin_yin_full);
			smallDto.put("productType", bond_small_view_cache->product_type);
			smallDto.put("ratingAugment", bond_small_view_cache->rating_augment);
			smallDto.put("ratingAugmentClient", bond_small_view_cache->rating_augment_client);
			smallDto.put("ratingCurrent", bond_small_view_cache->rating_current);
			smallDto.put("ratingCurrentNPY", bond_small_view_cache->rating_current_npy);
			smallDto.put("ratingDate", bond_small_view_cache->rating_date);
			smallDto.put("ratingInstitutionCode", bond_small_view_cache->rating_institution_code);
			smallDto.put("ratingInstitutionCodeClient", bond_small_view_cache->rating_institution_client);
			smallDto.put("redemptionNoClient", bond_small_view_cache->redemption_no_client);
			smallDto.put("shortName", bond_small_view_cache->short_name);
			smallDto.put("sortRemain", bond_small_view_cache->sort_remain);
			smallDto.put("termUnit", bond_small_view_cache->term_unit);
			smallDto.put("warranter", bond_small_view_cache->warranter);
			smallDto.put("warranterClient", bond_small_view_cache->warranter_client);
		}
		ofrOffer.put_child("smallDto", smallDto);
	}

	bondBestOffer.put_child("bidOffer", bidOffer);
	bondBestOffer.put_child("ofrOffer", ofrOffer);

	return bondBestOffer;
 }

REGISTER(BondBestQuoteCache);
