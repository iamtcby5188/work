
#include "cache_def.h"
#include "dcsbonddeal_cache.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "include/common.h"

std::string DcsBondDealCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string DcsBondDealCache::TABLE_NAME = "dcs_bond_deal";
std::string DcsBondDealCache::PKEY_NAME = "id";
std::string DcsBondDealCache::SQL_TAG = "FetchDcsBondDealSQL";

DcsBondDealCache::DcsBondDealCache() {
	// TODO Auto-generated constructor stub
	// real8
	price = DOUBLE_NULL;                                          // 价格
	return_point = DOUBLE_NULL;                                   // 返点数值, 比如返0.12  
	volume = DOUBLE_NULL;                                          // 数量 券面总额
	yield = DOUBLE_NULL;                                          // 收益率
	spread = DOUBLE_NULL;                                         // 利差
	net_price = DOUBLE_NULL;                                      // 净价
	full_price = DOUBLE_NULL;                                     // 全价
	bid_brokerrage = DOUBLE_NULL;                                 // Bid方佣金
	ofr_brokerrage = DOUBLE_NULL;                                 // Ofr方佣金

	// double
	bid_brokerage_rate = DOUBLE_NULL;                            // bid佣金比例
	ofr_brokerage_rate = DOUBLE_NULL;                            // ofr佣金比例
	bid_discount = DOUBLE_NULL;                                  // bid折扣
	ofr_discount = DOUBLE_NULL;                                  // ofr折扣
	coupon_rate = DOUBLE_NULL;
	time_to_maturity_real = DOUBLE_NULL;
	val_modified_duration = DOUBLE_NULL;
	val_convexity = DOUBLE_NULL;
	val_basis_point_value = DOUBLE_NULL;

	// bool
	bid_check = false;                                       // Bid方是否确认
	ofr_check = false;                                       // Ofr方是否确认
	bid_trader_check = false;
	ofr_trader_check = false;
	inHand = false;                                          // true:正在处理中 false:正常状态
	backOperated = false;                                    // 成交单是否已经过审核操作  //new Field
	isValidBond = true;
	bid_trader_is_vip = false;
	ofr_trader_is_vip = false;
	bid_trader_is_bad = false;
	ofr_trader_is_bad = false;

	// int8 

	// int4
	listed_date = -1;  //上市日
	filter_maturity_date = -1;
	issue_year = -1;

	// int
	sequence_number = 0;                                  // 序列号
	maturity_holidays = -1; // 到期日的节假日天数
	issuer_rating_order = -1;
	issuer_rating_npy_order = -1;
	bond_rating_order = -1;
	bond_rating_npy_order = -1;

	// time_t
	create_time_milli_sec = -1;
	deal_time_milli_sec = -1;

	// string
	urgent = "false";
	questionMark = "false";
	currency = "CNY";
	time_style = "T+1";
	bidPercentA = "100";
	bidPercentB = bidPercentC = bidPercentD = "0";
	bidTradeMode = "QQ";
	ofrPercentA = "100";
	ofrPercentB = ofrPercentC = ofrPercentD = "0";
	ofrTradeMode = "QQ";
	delivery_type = GBKToUTF8("DVP(券款对付)");
	internally = "1";
	deal_status = kDcsDealtoBeConfirm;
	status = "1";
	bid_brokerage_rate = ofr_brokerage_rate = 0.3;
	bid_discount = ofr_discount = 1;
	checkLevel = "0000";
	currLevel = "0000";
	specialFlag = "000";
}

DcsBondDealCache::~DcsBondDealCache() {
	// TODO Auto-generated destructor stub
}

std::string DcsBondDealCache::getPrimaryKey() {
	return id;
}

bool DcsBondDealCache::operator==(const DcsBondDealCache &rhs) {
	return isCacheEqual(*this, rhs);
}

bool DcsBondDealCache::operator!=(const DcsBondDealCache &rhs) {
	return !operator==(rhs);
}

void DcsBondDealCache::loadCombinationFields() {
	loadBondSmallViewFields();
	loadTradingInstitutionFields();
	loadTraderFields();
	loadBrokerFields();
	loadCdcValuationFields();

	if ("CIB" == listed_market) {
		bond_deal_type = bond_category;
	} else {
		if ("BCO" == bond_category) {
			bond_deal_type = "BBE";
		} else if ("BNC" == bond_category) {
			bond_deal_type = "BBN";
		}
	}
}

void DcsBondDealCache::loadBondSmallViewFields() {
	//BondSmallViewCacheController bond_small_view_cc;
	//dbQuery q;
	//q = "bond_key_listed_market = ", bond_key_listed_market;
	//BondSmallViewCachePtr bond_small_view_cache = bond_small_view_cc.getCacheByQuery(q);
	BondSmallViewCachePtr bond_small_view_cache = BondSmallViewCacheController::getCacheInMem("bond_key_listed_market", bond_key_listed_market);

	if (bond_small_view_cache) {
		bond_key = bond_small_view_cache->bond_key;
		listed_market = bond_small_view_cache->listed_market;
		bond_id = bond_small_view_cache->bond_id;
		bond_short_name = bond_small_view_cache->short_name;
		pin_yin = bond_small_view_cache->pin_yin;
		pin_yin_full = bond_small_view_cache->pin_yin_full;
		institution_rating = bond_small_view_cache->institution_rating;
		bond_category = bond_small_view_cache->product_type;
		bond_subtype = bond_small_view_cache->bond_subtype;
		coupon_type = bond_small_view_cache->coupon_type;

		/*cdc_valuation_yield = bond_small_view_cache->cdc_valuation_yield;
		cdc_valuation_net_price = bond_small_view_cache->cdc_valuation_net_price;*/

		bond_type = bond_small_view_cache->goods_type_client;

		enterprise_type = bond_small_view_cache->institution_type;
		pledged = bond_small_view_cache->pledged;
		across_market = bond_small_view_cache->across_market;

		maturity_date = IntToString(bond_small_view_cache->maturity_date);
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
		on_the_run = CheckOnTheRun(bond_small_view_cache->listed_date);

		rate_index = bond_small_view_cache->rate_index;
		rate_type_client = bond_small_view_cache->coupon_type_client;
		time_to_maturity = bond_small_view_cache->time_to_maturity;
		time_to_maturity_real = bond_small_view_cache->time_to_maturity_real;
		coupon_rate = bond_small_view_cache->coupon_rate_current;
		guarantee_type = bond_small_view_cache->rating_augment_client;
		issue_institution = bond_small_view_cache->issuer_client;
		rating_institution = bond_small_view_cache->rating_institution_client;
		listed_date = bond_small_view_cache->listed_date;
		filter_maturity_date = has_option == "Y" ? bond_small_view_cache->option_date : bond_small_view_cache->maturity_date;
		redemption_no_client = bond_small_view_cache->redemption_no_client;
		across_market_bond_ids = bond_small_view_cache->across_market_bond_ids;

		// old fields
		goods_code = bond_small_view_cache->bond_id;
		goods_short_name = bond_small_view_cache->short_name;
		issue_year = bond_small_view_cache->issue_year;
		sw_sector_code = bond_small_view_cache->sw_sector_code;
		sw_subsector_code = bond_small_view_cache->sw_subsector_code;
		province_code = bond_small_view_cache->province_code;
		financing_platform = bond_small_view_cache->financing_platform;
		issue_institution_id = bond_small_view_cache->issue_institution_id;
		company_bond = bond_small_view_cache->ent_cor;
		outlook = bond_small_view_cache->outlook;
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
		rate_index = "N";
		rate_type_client = "";
		time_to_maturity = "";
		time_to_maturity_real = 0.0;
		coupon_rate = 0.0;
		guarantee_type = "";
		issue_institution = "";
		rating_institution = "";
		listed_date = -1;
		filter_maturity_date = -1;
		redemption_no_client = "";
		bond_subtype = "";
		coupon_type = "";
		/*cdc_valuation_yield = "";
		cdc_valuation_net_price = "";*/
		across_market_bond_ids = "";

		goods_code = "";
		goods_short_name = "";
		issue_year = 2000;
		sw_sector_code = "";
		sw_subsector_code = "";
		province_code = "";
		financing_platform = "N";
		issue_institution_id = "";
		company_bond = "";
		outlook = "";
	}
}

void DcsBondDealCache::loadTradingInstitutionFields() {
	//TradingInstitutionCacheController trading_institution_cc;
	//dbQuery q;
	//q = "id = ", bid_bank_id;
	//TradingInstitutionCachePtr trading_institution_cache = trading_institution_cc.getCacheByQuery(q);
	TradingInstitutionCachePtr trading_institution_cache = TradingInstitutionCacheController::getCacheInMem("id", bid_bank_id);

	if (trading_institution_cache) {
		bid_institution_name = trading_institution_cache->name;
		bid_bank_code = trading_institution_cache->name;
	} else {
		bid_institution_name = "";
		bid_bank_code = "";
	}

	//q = "id = ", ofr_bank_id;
	//trading_institution_cache = trading_institution_cc.getCacheByQuery(q);
	trading_institution_cache = TradingInstitutionCacheController::getCacheInMem("id", ofr_bank_id);

	if (trading_institution_cache) {
		ofr_institution_name = trading_institution_cache->name;
		ofr_bank_code = trading_institution_cache->name;
	} else {
		ofr_institution_name = "";
		ofr_bank_code = "";
	}
}

void DcsBondDealCache::loadTraderFields() {
	//BondTraderCacheController bond_trader_cc;
	//dbQuery q;
	//q = "id = ", bid_agent_id;
	//BondTraderCachePtr bond_trader_cache = bond_trader_cc.getCacheByQuery(q);
	BondTraderCachePtr bond_trader_cache = BondTraderCacheController::getCacheInMem("id", bid_agent_id);

	if (bond_trader_cache) {
		bid_trader_name = bond_trader_cache->name;
		bid_agent_code = bond_trader_cache->code;
		bidBankAgentName = bond_trader_cache->name;
		bid_trader_is_vip = bond_trader_cache->flag_vip;
		bid_trader_is_bad = bond_trader_cache->flag_bad;
	} else {
		bid_trader_name = "";
		bid_agent_code = "";
		bidBankAgentName = "";
		bid_trader_is_vip = false;
		bid_trader_is_bad = false;
	}

	//q = "id = ", ofr_agent_id;
	//bond_trader_cache = bond_trader_cc.getCacheByQuery(q);
	bond_trader_cache = BondTraderCacheController::getCacheInMem("id", ofr_agent_id);

	if (bond_trader_cache) {
		ofr_trader_name = bond_trader_cache->name;
		ofr_agent_code = bond_trader_cache->code;
		ofrBankAgentName = bond_trader_cache->name;
		ofr_trader_is_vip = bond_trader_cache->flag_vip;
		ofr_trader_is_bad = bond_trader_cache->flag_bad;
	} else {
		ofr_trader_name = "";
		ofr_agent_code = "";
		ofrBankAgentName = "";
		ofr_trader_is_vip = false;
		ofr_trader_is_bad = false;
	}
}

void DcsBondDealCache::loadBrokerFields() {
	//AccountCacheController account_cc;
	//dbQuery q;
	//q = "id = ", bid_broker_id;
	//AccountCachePtr bid_account_cache = account_cc.getCacheByQuery(q);
	AccountCachePtr bid_account_cache = AccountCacheController::getCacheInMem("id", bid_broker_id);

	if (bid_account_cache) {
		bid_broker_name = bid_account_cache->display_name;
		bidBrokerCodeA = bid_account_cache->account_code;
	} else {
		bid_broker_name = "";
		bidBrokerCodeA = "";
	}

	AccountCachePtr bid_account_cache_B = AccountCacheController::getCacheInMem("id", bidBrokerIdB);
	if (bid_account_cache_B) {
		bidBrokerNameB = bid_account_cache_B->display_name;
		bidBrokerCodeB = bid_account_cache_B->account_code;
	} else {
		bidBrokerNameB = "";
		bidBrokerCodeB = "";
	}

	AccountCachePtr bid_account_cache_C = AccountCacheController::getCacheInMem("id", bidBrokerIdC);
	if (bid_account_cache_C) {
		bidBrokerNameC = bid_account_cache_C->display_name;
		bidBrokerCodeC = bid_account_cache_C->account_code;
	} else {
		bidBrokerNameC = "";
		bidBrokerCodeC = "";
	}

	AccountCachePtr bid_account_cache_D = AccountCacheController::getCacheInMem("id", bidBrokerIdD);
	if (bid_account_cache_D) {
		bidBrokerNameD = bid_account_cache_D->display_name;
		bidBrokerCodeD = bid_account_cache_D->account_code;
	} else {
		bidBrokerNameD = "";
		bidBrokerCodeD = "";
	}

	AccountCachePtr ofr_account_cache = AccountCacheController::getCacheInMem("id", ofr_broker_id);
	if (ofr_account_cache) {
		ofr_broker_name = ofr_account_cache->display_name;
		ofrBrokerCodeA = ofr_account_cache->account_code;
	} else {
		ofr_broker_name = "";
		ofrBrokerCodeA = "";
	}

	AccountCachePtr ofr_account_cache_B = AccountCacheController::getCacheInMem("id", ofrBrokerIdB);
	if (ofr_account_cache_B) {
		ofrBrokerNameB = ofr_account_cache_B->display_name;
		ofrBrokerCodeB = ofr_account_cache_B->account_code;
	} else {
		ofrBrokerNameB = "";
		ofrBrokerCodeB = "";
	}

	AccountCachePtr ofr_account_cache_C = AccountCacheController::getCacheInMem("id", ofrBrokerIdC);
	if (ofr_account_cache_C) {
		ofrBrokerNameC = ofr_account_cache_C->display_name;
		ofrBrokerCodeC = ofr_account_cache_C->account_code;
	} else {
		ofrBrokerNameC = "";
		ofrBrokerCodeC = "";
	}

	AccountCachePtr ofr_account_cache_D = AccountCacheController::getCacheInMem("id", ofrBrokerIdD);
	if (ofr_account_cache_D) {
		ofrBrokerNameD = ofr_account_cache_D->display_name;
		ofrBrokerCodeD = ofr_account_cache_D->account_code;
	} else {
		ofrBrokerNameD = "";
		ofrBrokerCodeD = "";
	}

	if (bid_broker_id.length() > 0) broker_ids = bid_broker_id;
	if (bidBrokerIdB.length() > 0) broker_ids += ", " + bidBrokerIdB;
	if (bidBrokerIdC.length() > 0) broker_ids += ", " + bidBrokerIdC;
	if (bidBrokerIdD.length() > 0) broker_ids += ", " + bidBrokerIdD;

	if (bid_broker_id.length() > 0 && ofr_broker_id.length() > 0) {
		broker_ids += ", " + ofr_broker_id;
	} else {
		broker_ids = ofr_broker_id;
	}
	if (ofrBrokerIdB.length() > 0) broker_ids += ", " + ofrBrokerIdB;
	if (ofrBrokerIdC.length() > 0) broker_ids += ", " + ofrBrokerIdC;
	if (ofrBrokerIdD.length() > 0) broker_ids += ", " + ofrBrokerIdD;
}

void DcsBondDealCache::loadCdcValuationFields() {
	cdc_valuation_yield = "";
	cdc_valuation_yield = "";
	if (bond_key_listed_market.length() > 0) {
		//ChinaBondValuationCacheController chinaBondValuationCC;
		//ChinaBondValuationCachePtr cache = chinaBondValuationCC.getValuation(bond_id);

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

REGISTER(DcsBondDealCache);