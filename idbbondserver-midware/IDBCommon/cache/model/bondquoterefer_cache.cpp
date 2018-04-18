/*
* BondQuoteReferCache.cpp
*
*  Created on: Jun 10, 2014
*      Author: theochen
*/

#include "bondquoterefer_cache.h"
#include "bondquote_cache.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "include/common.h"

std::string BondQuoteReferCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BondQuoteReferCache::TABLE_NAME = "bond_offer";
std::string BondQuoteReferCache::PKEY_NAME = "id";
std::string BondQuoteReferCache::SQL_TAG = "FetchBondQuoteReferSQL";

BondQuoteReferCache::BondQuoteReferCache() {
	// TODO Auto-generated constructor stub
	// real8
	price = DOUBLE_NULL;
	net_price = DOUBLE_NULL;
	full_price = DOUBLE_NULL;
	yield = DOUBLE_NULL;
	volume = DOUBLE_NULL;
	spread = DOUBLE_NULL;
	return_point = DOUBLE_NULL;     //返点

	// double
	coupon_rate = DOUBLE_NULL;
	time_to_maturity_real = DOUBLE_NULL;
	cdc_offset = DOUBLE_NULL;
	val_modified_duration = DOUBLE_NULL;
	val_convexity = DOUBLE_NULL;
	val_basis_point_value = DOUBLE_NULL;

	// bool
	flag_vip = false;
	flag_bad = false;
	isValidBond = true;
	trader_is_vip = false;
	trader_is_bad = false;

	// int8
	symbol = 0;

	// int4
	calculate_status = -1;
	listed_date = -1;
	maturity_date = -1;
	issue_year = -1;
	filter_maturity_date = -1;

	// int
	maturity_holidays = -1; // 到期日的节假日天数		
	issuer_rating_order = -1;
	issuer_rating_npy_order = -1;
	bond_rating_order = -1;
	bond_rating_npy_order = -1;
	redemption_no = -1;

	// time_t
	create_time_milli_sec = -1;
	deal_time_milli_sec = -1;

	// string
	status = "1";
	deal_status = "2";
	quote_type = "0";
	exercise = "1";
}

BondQuoteReferCache::~BondQuoteReferCache() {
	// TODO Auto-generated destructor stub
}

std::string BondQuoteReferCache::getPrimaryKey() {
	return id;
}

void BondQuoteReferCache::loadCombinationFields() {
	//TIME_COST_BEGIN("loadCombinationFields")
	loadBondSmallViewFields();
	loadTradingInstitutionFields();
	loadTraderFields();
	loadBrokerFields();
	loadCdcValuationFields();
	//TIME_COST_END("loadCombinationFields")
}

void BondQuoteReferCache::loadBondSmallViewFields() {
	//BondSmallViewCacheController bond_small_view_cc;
	//dbQuery q;
	//q = "bond_key_listed_market = ", bond_key_listed_market;
	//BondSmallViewCachePtr bond_small_view_cache = bond_small_view_cc.getCacheByQuery(q);
	BondSmallViewCachePtr bond_small_view_cache = BondSmallViewCacheController::getCacheInMem("bond_key_listed_market", bond_key_listed_market);

	if (bond_small_view_cache && !bond_small_view_cache->is_expired) {
		bond_key = bond_small_view_cache->bond_key;
		listed_market = bond_small_view_cache->listed_market;
		bond_id = bond_small_view_cache->bond_id;
		bond_short_name = bond_small_view_cache->short_name;
		pin_yin = bond_small_view_cache->pin_yin;
		pin_yin_full = bond_small_view_cache->pin_yin_full;
		institution_rating = bond_small_view_cache->institution_rating;
		bond_category = bond_small_view_cache->product_type;

		bond_type = bond_small_view_cache->goods_type_client;

		enterprise_type = bond_small_view_cache->institution_type;
		pledged = bond_small_view_cache->pledged;
		across_market = bond_small_view_cache->across_market;
		listed_date = bond_small_view_cache->listed_date;
		maturity_date = bond_small_view_cache->maturity_date;
		maturity_date_type = bond_small_view_cache->maturity_date_type;
		maturity_holidays = bond_small_view_cache->maturity_holidays;
		issuer_rating = bond_small_view_cache->issuer_rating_current;
		issuer_rating_npy = bond_small_view_cache->issuer_rating_current_npy;
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
		filter_maturity_date = has_option == "Y" ? bond_small_view_cache->option_date : bond_small_view_cache->maturity_date;
		on_the_run = CheckOnTheRun(bond_small_view_cache->listed_date);
		redemption_no_client = bond_small_view_cache->redemption_no_client;
		redemption_no = bond_small_view_cache->redemption_no;

		rate_index = bond_small_view_cache->rate_index;
		rate_type_client = bond_small_view_cache->coupon_type_client;
		time_to_maturity = bond_small_view_cache->time_to_maturity;
		time_to_maturity_real = bond_small_view_cache->time_to_maturity_real;
		coupon_rate = bond_small_view_cache->coupon_rate_current;
		guarantee_type = bond_small_view_cache->rating_augment_client;
		issue_institution = bond_small_view_cache->issuer_client;
		rating_institution = bond_small_view_cache->rating_institution_client;
		across_market_bond_ids = bond_small_view_cache->across_market_bond_ids;

		// old fileds
		goods_code = bond_small_view_cache->bond_id;
		goods_short_name = bond_small_view_cache->short_name;
		goods_type = bond_small_view_cache->goods_type_client;
		goods_level = bond_small_view_cache->rating_current;
		issue_year = bond_small_view_cache->issue_year;
		sw_sector_code = bond_small_view_cache->sw_sector_code;
		sw_subsector_code = bond_small_view_cache->sw_subsector_code;
		province_code = bond_small_view_cache->province_code;
		financing_platform = bond_small_view_cache->financing_platform;
		issue_institution_id = bond_small_view_cache->issue_institution_id;
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
		bond_category = "";
		bond_type = "";
		enterprise_type = "";
		pledged = "N";
		across_market = "N";
		listed_date = -1;
		filter_maturity_date = -1;
		maturity_date = -1;
		maturity_date_type = kMaturityDateNormal;
		issuer_rating = "";
		issuer_rating_npy = "";
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
		has_option = "N";
		on_the_run = "N";
		redemption_no_client = "";
		rate_index = "";
		rate_type_client = "";
		time_to_maturity = "";
		time_to_maturity_real = 0.0;
		coupon_rate = 0.0;
		guarantee_type = "";
		issue_institution = "";
		rating_institution = "";
		across_market_bond_ids = "";

		goods_code = "";
		goods_short_name = "";
		goods_type = "";
		goods_level = "";
		issue_year = 2000;
		sw_sector_code = "";
		sw_subsector_code = "";
		province_code = "";
		issue_institution_id = "";
		financing_platform = "N";
		company_bond = "";
		outlook = ""; //Add by Young 20-Apr-2016
		bond_subtype = "";
	}
}

void BondQuoteReferCache::loadTradingInstitutionFields() {
	//TradingInstitutionCacheController trading_institution_cc;
	//dbQuery q;
	//q = "id = ", bank_id;
	//TradingInstitutionCachePtr trading_institution_cache = trading_institution_cc.getCacheByQuery(q);
	TradingInstitutionCachePtr trading_institution_cache = TradingInstitutionCacheController::getCacheInMem("id", bank_id);

	if (trading_institution_cache) {
		institution_name = trading_institution_cache->name;
		bank_name = trading_institution_cache->name;
	} else {
		institution_name = "";
		bank_name = "";
	}
}

void BondQuoteReferCache::loadTraderFields() {
	//BondTraderCacheController bond_trader_cc;
	//dbQuery q;
	//q = "id = ", bank_agent_id;
	//BondTraderCachePtr bond_trader_cache = bond_trader_cc.getCacheByQuery(q);
	BondTraderCachePtr bond_trader_cache = BondTraderCacheController::getCacheInMem("id", bank_agent_id);

	if (bond_trader_cache) {
		trader_name = bond_trader_cache->name;
		agent_code = bond_trader_cache->name;
		trader_is_vip = bond_trader_cache->flag_vip;
		trader_is_bad = bond_trader_cache->flag_bad;
	} else {
		trader_name = "";
		agent_code = "";
		trader_is_vip = false;
		trader_is_bad = false;
	}
}

void BondQuoteReferCache::loadBrokerFields() {
	//AccountCacheController account_cc;
	//dbQuery q;
	//q = "id = ", operator_id;
	//AccountCachePtr account_cache = account_cc.getCacheByQuery(q);
	AccountCachePtr account_cache = AccountCacheController::getCacheInMem("id", operator_id);

	if (account_cache) {
		broker_name = account_cache->display_name;
	} else {
		broker_name = "";
	}
}

void BondQuoteReferCache::loadCdcValuationFields() {
	cdc_valuation_yield = "";
	cdc_valuation_yield = "";
	cdc_offset = DOUBLE_NULL;
	if (bond_key_listed_market.length() > 0) {
		//ChinaBondValuationCacheController chinaBondValuationCC;
		//ChinaBondValuationCachePtr cache = chinaBondValuationCC.getValuation(bond_id);
		//std::string datetime = GetTDateString("%Y%m%d");
		//dbQuery q;
		//q = "bond_key_listed_market = ", bond_key_listed_market, " and valuation_date = ", datetime, " order by valuation_date desc limit 1";
		//ChinaBondValuationCachePtr cache = chinaBondValuationCC.getCacheByQueryInThreadSafty(q);
		ChinaBondValuationCachePtr cache = ChinaBondValuationCacheController::getMemValuation(bond_key_listed_market);
		if (cache == NULL || cache.get() == NULL || cache.get()->id.empty()) {
			return;
		}
		cdc_valuation_yield = cache->valuation_yield;
		cdc_valuation_net_price = cache->valuation_net_price;
		if (symbol == kOfrQuote && cdc_valuation_yield.length() > 0) { // 中债偏移只考虑ofr
			double tCdcValuationYield = DOUBLE_NULL;
			if ("Y" == has_option && cdc_valuation_yield.find("|") != std::string::npos) {
				tCdcValuationYield = atof(cdc_valuation_yield.substr(0, cdc_valuation_yield.find_first_of("|")).c_str());
			} else {
				tCdcValuationYield = atof(cdc_valuation_yield.c_str());
			}
			if (yield != DOUBLE_NULL) {
				cdc_offset = (yield - tCdcValuationYield) * 100;
			} else if (price != DOUBLE_NULL && quote_type == "3") {
				cdc_offset = (price - tCdcValuationYield) * 100;
			}
		}
		//val_modified_duration = cache->val_modified_duration;
		if (fabs(cache->val_modified_duration - 0) <= FLOATING_PRECISION) {
			val_modified_duration = DOUBLE_NULL;
		} else {
			val_modified_duration = cache->val_modified_duration;
		}
		val_convexity = cache->val_convexity;
		val_basis_point_value = cache->val_basis_point_value;
	}
}

BondQuoteReferCachePtr BondQuoteReferCache::fromBondQuoteCache(BondQuoteCache* ref_cache) {
	BondQuoteReferCachePtr cache(new BondQuoteReferCache());

	cache->isValidBond = ref_cache->isValidBond;
	cache->id = ref_cache->id;
	cache->company_id = ref_cache->company_id;
	cache->bond_key = ref_cache->bond_key;
	cache->listed_market = ref_cache->listed_market;
	cache->bond_key_listed_market = ref_cache->bond_key_listed_market;
	cache->goods_code = ref_cache->goods_code;
	cache->goods_id = ref_cache->goods_id;
	cache->bank_name = ref_cache->bank_name;
	cache->bank_id = ref_cache->bank_id;
	cache->symbol = ref_cache->symbol;
	cache->price = ref_cache->price;
	cache->net_price = ref_cache->net_price;
	cache->full_price = ref_cache->full_price;
	cache->yield = ref_cache->yield;
	cache->volume = ref_cache->volume;
	cache->status = ref_cache->status;
	cache->spread = ref_cache->spread;
	cache->deal_status = "2";
	cache->deal_time = ref_cache->deal_time;
	cache->operator_id = ref_cache->operator_id;
	cache->review_id = ref_cache->review_id;
	cache->create_time = ref_cache->create_time;
	cache->agent_code = ref_cache->agent_code;
	cache->rate_type = ref_cache->rate_type;
	cache->internally = ref_cache->internally;
	cache->goods_short_name = ref_cache->goods_short_name;
	cache->return_point = ref_cache->return_point;     //返点
	cache->delivery_date = ref_cache->delivery_date; //结算日期
	cache->goods_term = ref_cache->goods_term;
	cache->goods_level = ref_cache->goods_level;
	cache->rebate = ref_cache->rebate;
	cache->bank_agent_id = ref_cache->bank_agent_id;
	cache->flag_vip = ref_cache->flag_vip;
	cache->flag_bad = ref_cache->flag_bad;
	cache->flag_bargain = ref_cache->flag_bargain;
	cache->flag_relation = ref_cache->flag_relation; //0：代表不显示，1：显示oco 2:oco打包
	cache->price_description = ref_cache->price_description;
	cache->goods_type = ref_cache->goods_type;
	cache->profit_code = ref_cache->profit_code;
	cache->description = ref_cache->description;
	cache->goods_name = ref_cache->goods_name;
	cache->option_type = ref_cache->option_type;
	cache->option_client = ref_cache->option_client;
	cache->trader_id = ref_cache->trader_id;
	cache->quote_type = ref_cache->quote_type;
	cache->clear_speed = ref_cache->clear_speed; //清算速度  clear_speed（varchar:  T+0,T+1），
	cache->exercise = ref_cache->exercise; //行权到期 (varchar  0-行权 1-到期)
	cache->send_status = ref_cache->send_status;
	cache->calculate_status = ref_cache->calculate_status;

	// ----------------- Combination Fields ----------------------
	//
	cache->institution_name = ref_cache->institution_name;
	cache->trader_name = ref_cache->trader_name;
	cache->trader_is_vip = ref_cache->trader_is_vip;
	cache->trader_is_bad = ref_cache->trader_is_bad;
	cache->broker_name = ref_cache->broker_name;
	cache->rate_index = ref_cache->rate_index;
	cache->rate_type_client = ref_cache->rate_type_client;
	cache->coupon_rate = ref_cache->coupon_rate;

	cache->bond_id = ref_cache->bond_id;
	cache->bond_short_name = ref_cache->bond_short_name;
	cache->pin_yin = ref_cache->pin_yin;
	cache->pin_yin_full = ref_cache->pin_yin_full;
	cache->institution_rating = ref_cache->institution_rating;

	cache->bond_category = ref_cache->bond_category;
	cache->bond_type = ref_cache->bond_type;
	cache->enterprise_type = ref_cache->enterprise_type;
	cache->pledged = ref_cache->pledged;
	cache->across_market = ref_cache->across_market;
	cache->time_to_maturity = ref_cache->time_to_maturity;
	cache->time_to_maturity_real = ref_cache->time_to_maturity_real;
	cache->listed_date = ref_cache->listed_date;
	cache->maturity_date = ref_cache->maturity_date;
	cache->maturity_date_type = ref_cache->maturity_date_type;
	cache->maturity_holidays = ref_cache->maturity_holidays;
	cache->filter_maturity_date = ref_cache->filter_maturity_date;
	cache->issuer_rating_filter = ref_cache->issuer_rating_filter;
	cache->issuer_rating = ref_cache->issuer_rating;
	cache->issuer_rating_npy = ref_cache->issuer_rating_npy;
	cache->bond_rating = ref_cache->bond_rating;
	cache->bond_rating_npy = ref_cache->bond_rating_npy;
	cache->issuer_rating_order = ref_cache->issuer_rating_order;
	cache->issuer_rating_npy_order = ref_cache->issuer_rating_npy_order;
	cache->bond_rating_order = ref_cache->bond_rating_order;
	cache->bond_rating_npy_order = ref_cache->bond_rating_npy_order;

	cache->market_type = ref_cache->market_type;
	cache->municipal = ref_cache->municipal;
	cache->guarenteed = ref_cache->guarenteed;
	cache->has_option = ref_cache->has_option;
	cache->on_the_run = ref_cache->on_the_run;
	cache->redemption_no_client = ref_cache->redemption_no_client;
	cache->redemption_no = ref_cache->redemption_no;

	cache->cdc_valuation_yield = ref_cache->cdc_valuation_yield;
	cache->cdc_valuation_net_price = ref_cache->cdc_valuation_net_price;
	cache->cdc_offset = ref_cache->cdc_offset;
	cache->val_modified_duration = ref_cache->val_modified_duration;
	cache->val_convexity = ref_cache->val_convexity;
	cache->val_basis_point_value = ref_cache->val_basis_point_value;

	cache->guarantee_type = ref_cache->guarantee_type;
	cache->issue_institution = ref_cache->issue_institution;
	cache->rating_institution = ref_cache->rating_institution;
	cache->across_market_bond_ids = ref_cache->across_market_bond_ids;

	cache->create_time_milli_sec = ref_cache->create_time_milli_sec;
	cache->deal_time_milli_sec = ref_cache->deal_time_milli_sec;
	cache->issue_year = ref_cache->issue_year;
	cache->sw_sector = ref_cache->sw_sector;
	cache->sw_sector_code = ref_cache->sw_sector_code;
	cache->sw_subsector = ref_cache->sw_subsector;
	cache->sw_subsector_code = ref_cache->sw_subsector_code;
	cache->province = ref_cache->province;
	cache->province_code = ref_cache->province_code;
	cache->financing_platform = ref_cache->financing_platform;
	cache->issue_institution_id = ref_cache->issue_institution_id;
	cache->company_bond = ref_cache->company_bond;
	cache->outlook = ref_cache->outlook; //Add by Young 20-Apr-2016
	cache->bond_subtype = ref_cache->bond_subtype;
	return cache;
}

REGISTER(BondQuoteReferCache);
