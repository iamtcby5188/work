#include "qb_sync_service.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"
#include "include/fid.h"
#include <strstream>
#include <boost/lexical_cast.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time.hpp> 
#include <boost/regex.hpp>
#include "cache/controller/bondbestquote_cachecontroller.h"

QBSyncService::QBSyncService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
	: Service(conn, queue_name, topic_name) {
	boost::function0<void> f = boost::bind(&QBSyncService::threadPubMsg, this);
	boost::thread thrd(f);
}

void QBSyncService::processReqMsg(const sdbus::Message* msg) {
	sdbus::string method = msg->GetType();
	sdbus::string subject = msg->GetSubject();

	sdbus::Message reply;
	reply.SetType(msg->GetType());
	reply.SetSubject(msg->GetSubject());
	reply.SetMessageID(msg->GetMessageID());

	if (method == "getData") {
		int version;
		msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
		LOGGER_INFO("version: " << version);
		loadPushMessageData(reply, version);
	} else if (method == "getDataBetween") {
		getPushMessageData(msg, reply);
	} else if (method == "getDataBetweenByTime") {
		getPushMessageDataByTime(msg, reply);
	}

	if (msg->GetReplyTo().length() > 0) {
		size_t len = -1;
		VariantMapCalcSize(reply.MapValue(), len);
		LOGGER_INFO("Message Size: " << len);
		bool flag = conn_->Send(msg->GetReplyTo(), reply);
		if (flag) {
			LOGGER_INFO("Reply success: " << msg->GetType());
		} else {
			LOGGER_ERROR("Reply failed: " << msg->GetType());
		}
	}
}

// goodsCode, remove ".SH" && ".SZ", jialu, 2016/9/22
std::string getHeadOfGoodsCode(std::string gc) {
	int len = gc.length();
	if (len >= 3)
		if (gc[len - 3] == '.' &&
			(gc[len - 2] == 's' || gc[len - 2] == 'S') &&
			(gc[len - 1] == 'h' || gc[len - 1] == 'H' || gc[len - 1] == 'z' || gc[len - 1] == 'Z'))
			return gc.substr(0, len - 3);
	return gc;
}
static const int _iMaxLength = 8;
void QBSyncService::SetBondQuoteMap(const BondQuoteCache *cache, sdbus::VariantMap *mp) {
	mp->SetString(FID_PUSH_MESSAGE_COMPANY_ID, cache->company_id);
	mp->SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(cache->company_id));
	mp->SetString(FID_ID, cache->id);
	mp->SetString(FID_DEL_FLAG, cache->status);
	mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
	mp->SetString(FID_BOND_KEY, cache->bond_key);
	mp->SetString(FID_BOND_TYPE, cache->bond_type);
	mp->SetString(FID_ENTERPRISE_TYPE, cache->enterprise_type);
	mp->SetString(FID_LISTED_MARKET, cache->listed_market);
	mp->SetString(FID_BOND_CODE, getHeadOfGoodsCode(cache->bond_id));
	mp->SetString(FID_BOND_SHORT_NAME, cache->bond_short_name);

	if (DOUBLE_NULL != cache->coupon_rate)
		mp->SetString(FID_COUPON_RATE, truncateDouble(cache->coupon_rate, _iMaxLength));
	mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
	mp->SetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
	mp->SetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);
	mp->SetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);

	if (cache->price != DOUBLE_NULL) {
		mp->SetString(FID_PRICE, Format(cache->price, 2, 4));
	}
	if (cache->return_point != DOUBLE_NULL) {
		mp->SetString(FID_FAN_DIAN, Format(cache->return_point, 2, 4));
	}
	mp->SetString(FID_FAN_DIAN_FLAG, cache->rebate);
	mp->SetString(FID_PRICE_DESCRIPTION, cache->price_description);
	mp->SetString(FID_DESCRIPTION, cache->description);
	mp->SetString(FID_BARGAIN_FLAG, cache->flag_bargain);
	mp->SetString(FID_OCO_FLAG, cache->flag_relation);
	mp->SetString(FID_QUOTE_INTERNAL_FLAG, cache->internally);

	mp->SetString(FID_QUOTE_TYPE, cache->quote_type);
	mp->SetString(FID_YIELD_TYPE, cache->exercise);

	if (cache->volume != DOUBLE_NULL) {
		mp->SetString(FID_VOLUME, Format(cache->volume, 0, 4));
	}
	mp->SetString(FID_QUOTE_SIDE, IntToString(cache->symbol));

	if (cache->net_price != DOUBLE_NULL) {
		mp->SetString(FID_CLEAN_PRICE, Format(cache->net_price, 2, 4));
	}
	if (cache->full_price != DOUBLE_NULL) {
		mp->SetString(FID_DIRTY_PRICE, Format(cache->full_price, 2, 4));
	}
	if (cache->yield != DOUBLE_NULL) {
		mp->SetString(FID_YIELD, Format(cache->yield, 2, 4));
	}
	if (cache->spread != DOUBLE_NULL) {
		mp->SetString(FID_SPREAD, Format(cache->spread, 2, 4));
	}

	mp->SetString(FID_INSTITUTION_ID, cache->bank_id);
	mp->SetString(FID_INSTITUTION_NAME, cache->institution_name);

	mp->SetString(FID_TRADER_ID, cache->bank_agent_id);
	mp->SetString(FID_TRADER_NAME, cache->trader_name);

	mp->SetString(FID_BROKER_ID, cache->operator_id);
	mp->SetString(FID_BROKER_NAME, cache->broker_name);

	mp->SetString(FID_QUOTE_STATUS, cache->deal_status);

	if (cache->create_time_milli_sec != -1000) {
		mp->SetString(FID_CREATE_TIME, boost::lexical_cast<std::string>(cache->create_time_milli_sec));
	}
	if (cache->deal_time_milli_sec != -1000) {
		mp->SetString(FID_REFER_TIME, boost::lexical_cast<std::string>(cache->deal_time_milli_sec));
	}

	mp->SetString(FID_VIP_FLAG, cache->trader_is_vip ? "true" : "false");
	mp->SetString(FID_BAD_FLAG, cache->trader_is_bad ? "true" : "false");

	mp->SetString(FID_ISSUER_RATING, cache->issuer_rating);
	mp->SetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
	mp->SetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
	mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
	mp->SetString(FID_BOND_RATING, cache->bond_rating);
	mp->SetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
	mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
	mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
	if (DOUBLE_NULL != cache->cdc_offset)
		mp->SetString(FID_CDC_OFFSET, truncateDouble(cache->cdc_offset, _iMaxLength));
	if (DOUBLE_NULL != cache->val_modified_duration)
		mp->SetString(FID_DURATION_FILTER, truncateDouble(cache->val_modified_duration, _iMaxLength));
	if (DOUBLE_NULL != cache->val_convexity)
		mp->SetString(FID_VAL_CONVEXITY, truncateDouble(cache->val_convexity, _iMaxLength));
	if (DOUBLE_NULL != cache->val_basis_point_value)
		mp->SetString(FID_VAL_BASIC_POINT_VALUE, truncateDouble(cache->val_basis_point_value, _iMaxLength));

	mp->SetString(FID_PLEDGED, cache->pledged);
	mp->SetString(FID_ACROSS_MARKET, cache->across_market);
	mp->SetString(FID_ISSUER_RATING_FILTER, cache->issuer_rating_filter);
	mp->SetString(FID_MARKET_TYPE, cache->market_type);
	mp->SetString(FID_MUNICIPAL, cache->municipal);
	mp->SetString(FID_GUARENTEED, cache->guarenteed);
	mp->SetString(FID_HAS_OPTION, cache->has_option);
	mp->SetString(FID_ON_THE_RUN, cache->on_the_run);

	mp->SetString(FID_RATE_TYPE, cache->rate_type_client);
	mp->SetString(FID_RATE_INDEX, cache->rate_index);
	mp->SetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
	mp->SetString(FID_OPTION_TYPE, cache->option_client);
	mp->SetString(FID_BOND_CATEGORY, cache->bond_category);
	mp->SetInt32(FID_BOND_RATING_ORDER, cache->bond_rating_order);
	mp->SetInt32(FID_BOND_RATING_NPY_ORDER, cache->bond_rating_npy_order);
	mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->SetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);
	if (DOUBLE_NULL != cache->time_to_maturity_real)
		mp->SetString(FID_TIME_TO_MATURITY_REAL, truncateDouble(cache->time_to_maturity_real, _iMaxLength));
	mp->SetInt32(FID_MATURITY, cache->maturity_date);
	mp->SetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->SetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code);
	mp->SetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code);
	mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);
	mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->SetString(FID_COMPANY_BOND, cache->company_bond);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016

	mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);

	mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void QBSyncService::SetBondDealMap(const BondDealCache *cache, sdbus::VariantMap *mp) {
	mp->SetString(FID_PUSH_MESSAGE_COMPANY_ID, cache->company_id);
	mp->SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(cache->company_id));
	mp->SetString(FID_ID, cache->id);

	mp->SetString(FID_DEL_FLAG, cache->status);
	mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
	mp->SetString(FID_BOND_KEY, cache->bond_key);
	mp->SetString(FID_LISTED_MARKET, cache->listed_market);

	mp->SetString(FID_DIRECTION, cache->deal_type);
	/*if (cache->deal_time_milli_sec != -1000) {
	mp->SetString(FID_DEAL_TIME, boost::lexical_cast<std::string>(cache->deal_time_milli_sec));
	}*/
	if (cache->create_time_milli_sec != -1000) { //推给QB的deal_time_milli_sec改为create_time_milli_sec
		mp->SetString(FID_DEAL_TIME, boost::lexical_cast<std::string>(cache->create_time_milli_sec));
	}
	mp->SetString(FID_DEAL_NO, cache->odd_numbers);

	mp->SetString(FID_BOND_CODE, getHeadOfGoodsCode(cache->bond_id));
	mp->SetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
	mp->SetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
	mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
	if (cache->coupon_rate != DOUBLE_NULL) {
		mp->SetString(FID_COUPON_RATE, Format(cache->coupon_rate, 2, 4));
	}
	mp->SetString(FID_RATE_TYPE, cache->rate_type_client);
	mp->SetString(FID_RATING_INSTITUTION, cache->rating_institution);
	mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->SetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);
	mp->SetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);

	if (cache->price != DOUBLE_NULL) {
		mp->SetString(FID_PRICE, Format(cache->price, 2, 4));
	}
	if (cache->return_point != DOUBLE_NULL) {
		mp->SetString(FID_FAN_DIAN, Format(cache->return_point, 2, 4));
	}
	mp->SetString(FID_FAN_DIAN_FLAG, cache->rebate);
	if (cache->volume != DOUBLE_NULL) {
		mp->SetString(FID_VOLUME, Format(cache->volume, 0, 4));
	}

	if (cache->net_price != DOUBLE_NULL) {
		mp->SetString(FID_CLEAN_PRICE, Format(cache->net_price, 2, 4));
	}
	if (cache->full_price != DOUBLE_NULL) {
		mp->SetString(FID_DIRTY_PRICE, Format(cache->full_price, 2, 4));
	}
	if (cache->yield != DOUBLE_NULL) {
		mp->SetString(FID_YIELD, Format(cache->yield, 2, 4));
	}
	if (cache->spread != DOUBLE_NULL) {
		mp->SetString(FID_SPREAD, Format(cache->spread, 2, 4));
	}

	mp->SetString(FID_CURRENCY, cache->currency);

	mp->SetString(FID_QUOTE_TYPE, cache->quote_type);
	mp->SetString(FID_YIELD_TYPE, cache->exercise);

	mp->SetString(FID_SETTLEMENT_TYPE, cache->time_style);
	mp->SetString(FID_SETTLEMENT_DATE, cache->settlement_date);
	mp->SetString(FID_SETTLEMENT_AMOUNT, cache->total_price);
	mp->SetString(FID_SETTLEMENT_MODE, cache->delivery_type);
	mp->SetString(FID_DELIVERY_DATE, cache->delivery_date);

	if (cache->create_time_milli_sec != -1000) {
		mp->SetString(FID_CREATE_TIME, boost::lexical_cast<std::string>(cache->create_time_milli_sec));
	}

	mp->SetString(FID_BID_IS_BRIDGE, cache->bid_bridge);
	mp->SetString(FID_BID_IS_CHARGED, cache->bid_charge);
	mp->SetString(FID_BID_INSTITUTION_ID, cache->bid_bank_id);
	mp->SetString(FID_BID_INSTITUTION_NAME, cache->bid_institution_name);
	mp->SetString(FID_BID_INSTITUTION_CITY, cache->bid_bank_city);
	mp->SetString(FID_BID_TRADER_ID, cache->bid_agent_id);
	mp->SetString(FID_BID_TRADER_NAME, cache->bid_trader_name);
	mp->SetString(FID_BID_TRADER_ACCOUNT, cache->bid_trader_account);
	mp->SetString(FID_BID_BROKER_ID, cache->bid_broker_id);
	mp->SetString(FID_BID_BROKER_NAME, cache->bid_broker_name);
	mp->SetString(FID_BID_IS_NO_BROKERAGE, cache->bidIsNoBrokerage);
	//if (cache->bid_brokerrage != DOUBLE_NULL) {
	//	mp->SetString(FID_BID_BROKERAGE, Format(cache->bid_brokerrage, 2, 4));
	//}
	mp->SetString(FID_BID_BROKERAGE, cache->bidBrokerrageRemark);
	mp->SetString(FID_BID_TRADE_MODE, cache->bidTradeMode);
	mp->SetString(FID_BID_NO_CONFIRM, cache->bidNoConfirm);
	mp->SetString(FID_BID_NO_COMMENTS, cache->bidNoComments);
	mp->SetString(FID_BID_CHECK, cache->bid_check ? "true" : "false");
	mp->SetString(FID_BID_CHECK_DATE, cache->bid_check_date);
	mp->SetString(FID_BID_REMARKS, cache->bid_remarks);
	if (DOUBLE_NULL != cache->bid_brokerage_rate)
		mp->SetString(FID_BID_BROKERAGE_RATE, truncateDouble(cache->bid_brokerage_rate, _iMaxLength));
	if (DOUBLE_NULL != cache->bid_discount)
		mp->SetString(FID_BID_DISCOUNT, truncateDouble(cache->bid_discount, _iMaxLength));

	mp->SetString(FID_BID_BROKER_ID_A, cache->bid_broker_id);
	mp->SetString(FID_BID_BROKER_ID_B, cache->bidBrokerIdB);
	mp->SetString(FID_BID_BROKER_ID_C, cache->bidBrokerIdC);
	mp->SetString(FID_BID_BROKER_ID_D, cache->bidBrokerIdD);
	mp->SetString(FID_BID_BROKER_NAME_A, cache->bid_broker_name);
	mp->SetString(FID_BID_BROKER_NAME_B, cache->bidBrokerNameB);
	mp->SetString(FID_BID_BROKER_NAME_C, cache->bidBrokerNameC);
	mp->SetString(FID_BID_BROKER_NAME_D, cache->bidBrokerNameD);
	mp->SetString(FID_BID_PERCENT_A, cache->bidPercentA);
	mp->SetString(FID_BID_PERCENT_B, cache->bidPercentB);
	mp->SetString(FID_BID_PERCENT_C, cache->bidPercentC);
	mp->SetString(FID_BID_PERCENT_D, cache->bidPercentD);

	mp->SetString(FID_OFR_IS_BRIDGE, cache->ofr_bridge);
	mp->SetString(FID_OFR_IS_CHARGED, cache->ofr_charge);
	mp->SetString(FID_OFR_INSTITUTION_ID, cache->ofr_bank_id);
	mp->SetString(FID_OFR_INSTITUTION_NAME, cache->ofr_institution_name);
	mp->SetString(FID_OFR_INSTITUTION_CITY, cache->ofr_bank_city);
	mp->SetString(FID_OFR_TRADER_ID, cache->ofr_agent_id);
	mp->SetString(FID_OFR_TRADER_NAME, cache->ofr_trader_name);
	mp->SetString(FID_OFR_BROKER_ID, cache->ofr_broker_id);
	mp->SetString(FID_OFR_BROKER_NAME, cache->ofr_broker_name);
	mp->SetString(FID_OFR_IS_NO_BROKERAGE, cache->ofrIsNoBrokerage);
	//if (cache->ofr_brokerrage != DOUBLE_NULL) {
	//	mp->SetString(FID_OFR_BROKERAGE, Format(cache->ofr_brokerrage, 2, 4));
	//}
	mp->SetString(FID_OFR_BROKERAGE, cache->ofrBrokerrageRemark);
	mp->SetString(FID_OFR_REMARKS, cache->ofr_remarks);
	if (DOUBLE_NULL != cache->ofr_brokerage_rate)
		mp->SetString(FID_OFR_BROKERAGE_RATE, truncateDouble(cache->ofr_brokerage_rate, _iMaxLength));
	if (DOUBLE_NULL != cache->ofr_discount)
		mp->SetString(FID_OFR_DISCOUNT, truncateDouble(cache->ofr_discount, _iMaxLength));
	mp->SetString(FID_OFR_TRADE_MODE, cache->ofrTradeMode);
	mp->SetString(FID_OFR_NO_CONFIRM, cache->ofrNoConfirm);
	mp->SetString(FID_OFR_NO_COMMENTS, cache->ofrNoComments);

	mp->SetString(FID_OFR_BROKER_ID_A, cache->ofr_broker_id);
	mp->SetString(FID_OFR_BROKER_ID_B, cache->ofrBrokerIdB);
	mp->SetString(FID_OFR_BROKER_ID_C, cache->ofrBrokerIdC);
	mp->SetString(FID_OFR_BROKER_ID_D, cache->ofrBrokerIdD);
	mp->SetString(FID_OFR_BROKER_NAME_A, cache->ofr_broker_name);
	mp->SetString(FID_OFR_BROKER_NAME_B, cache->ofrBrokerNameB);
	mp->SetString(FID_OFR_BROKER_NAME_C, cache->ofrBrokerNameC);
	mp->SetString(FID_OFR_BROKER_NAME_D, cache->ofrBrokerNameD);
	mp->SetString(FID_OFR_PERCENT_A, cache->ofrPercentA);
	mp->SetString(FID_OFR_PERCENT_B, cache->ofrPercentB);
	mp->SetString(FID_OFR_PERCENT_C, cache->ofrPercentC);
	mp->SetString(FID_OFR_PERCENT_D, cache->ofrPercentD);

	mp->SetString(FID_OFR_CHECK, cache->ofr_check ? "true" : "false");
	mp->SetString(FID_OFR_CHECK_DATE, cache->ofr_check_date);

	mp->SetBool(FID_BID_TRADER_IS_VIP, cache->bid_trader_is_vip);
	mp->SetBool(FID_OFR_TRADER_IS_VIP, cache->ofr_trader_is_vip);

	mp->SetString(FID_DESCRIPTION, cache->description);

	mp->SetString(FID_DEAL_INTERNAL_FLAG, cache->internally);

	mp->SetString(FID_DEAL_STATUS, cache->deal_status);
	mp->SetString(FID_BOND_CATEGORY, cache->bond_category);
	mp->SetString(FID_MATURITY, cache->maturity_date);
	mp->SetString(FID_ISSUER_RATING, cache->issuer_rating);
	mp->SetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
	mp->SetString(FID_BOND_RATING, cache->bond_rating);
	mp->SetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
	mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
	mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
	if (DOUBLE_NULL != cache->val_modified_duration)
		mp->SetString(FID_DURATION_FILTER, truncateDouble(cache->val_modified_duration, _iMaxLength));
	if (DOUBLE_NULL != cache->val_convexity)
		mp->SetString(FID_VAL_CONVEXITY, truncateDouble(cache->val_convexity, _iMaxLength));
	if (DOUBLE_NULL != cache->val_basis_point_value)
		mp->SetString(FID_VAL_BASIC_POINT_VALUE, truncateDouble(cache->val_basis_point_value, _iMaxLength));
	mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
	mp->SetString(FID_RATE_INDEX, cache->rate_index);

	mp->SetInt32(FID_SERIAL_NUMBER, cache->sequence_number);

	mp->SetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->SetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code); // 使用二级行业进行过滤
	mp->SetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code); //一级行业暂时没有使用到，预留
	mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);

	mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->SetString(FID_COMPANY_BOND, cache->company_bond);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016

	mp->SetInt32(FID_ISSUER_RATING_ORDER, cache->issuer_rating_order);
	mp->SetInt32(FID_ISSUER_RATING_NPY_ORDER, cache->issuer_rating_npy_order);
	mp->SetInt32(FID_BOND_RATING_ORDER, cache->bond_rating_order);
	mp->SetInt32(FID_BOND_RATING_NPY_ORDER, cache->bond_rating_npy_order);
	mp->SetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);
	mp->SetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);

	mp->SetString(FID_BOND_CATEGORY, cache->bond_category);
	mp->SetString(FID_BOND_TYPE, cache->bond_type);
	mp->SetString(FID_ENTERPRISE_TYPE, cache->enterprise_type);

	mp->SetString(FID_BOND_CATEGORY, cache->bond_category);
	mp->SetString(FID_BOND_TYPE, cache->bond_type);
	mp->SetString(FID_ENTERPRISE_TYPE, cache->enterprise_type);
	mp->SetString(FID_LISTED_MARKET, cache->listed_market);
	mp->SetString(FID_PLEDGED, cache->pledged);
	mp->SetString(FID_ACROSS_MARKET, cache->across_market);
	mp->SetString(FID_ISSUER_RATING_FILTER, cache->issuer_rating_filter);
	mp->SetString(FID_MARKET_TYPE, cache->market_type);
	mp->SetString(FID_MUNICIPAL, cache->municipal);
	mp->SetString(FID_GUARENTEED, cache->guarenteed);
	mp->SetString(FID_HAS_OPTION, cache->has_option);
	mp->SetString(FID_ON_THE_RUN, cache->on_the_run);
	if (DOUBLE_NULL != cache->time_to_maturity_real)
		mp->SetString(FID_TIME_TO_MATURITY_REAL, truncateDouble(cache->time_to_maturity_real, _iMaxLength));
	mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);
	mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void QBSyncService::setBondBestQuoteMap(const BondBestQuoteCache* cache, sdbus::VariantMap *mp) {
	mp->SetString(FID_PUSH_MESSAGE_COMPANY_ID, cache->company_id);
	mp->SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(cache->company_id));
	mp->SetString(FID_ID, cache->id);
	mp->SetString(FID_DEL_FLAG, cache->status);

	mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
	mp->SetString(FID_BOND_KEY, cache->bond_key);
	mp->SetString(FID_LISTED_MARKET, cache->listed_market);
	mp->SetString(FID_BOND_CODE, getHeadOfGoodsCode(cache->bond_id));
	mp->SetString(FID_BOND_SHORT_NAME, cache->bond_short_name);

	// Bid
	mp->SetString(FID_BID_QUOTE_ID, cache->bid_offer_id);
	mp->SetString(FID_BID_INSTITUTION_ID, cache->bid_institution_id);
	mp->SetString(FID_BID_INSTITUTION_NAME, cache->bid_institution_name);
	mp->SetString(FID_BID_TRADER_ID, cache->bid_trader_id);
	mp->SetString(FID_BID_TRADER_NAME, cache->bid_trader_name);
	mp->SetString(FID_BID_BROKER_ID, cache->bid_broker_id);
	mp->SetString(FID_BID_BROKER_NAME, cache->bid_broker_name);
	sdbus::string bid_ins_trader_names;
	if (!cache->bid_institution_name.empty() && !cache->bid_trader_name.empty()) {
		bid_ins_trader_names = cache->bid_institution_name + "(" + cache->bid_trader_name + ")";
	}
	mp->SetString(FID_BID_INSTITUTION_TRADER_NAMES, bid_ins_trader_names);

	mp->SetBool(FID_BID_TRADER_IS_VIP, cache->bid_trader_is_vip);
	mp->SetBool(FID_BID_BAD_FLAG, cache->bid_trader_is_bad);

	mp->SetString(FID_BEST_BID_VOLUME, cache->bid_volume);
	if (DOUBLE_NULL != cache->vol_bid_client)
		mp->SetString(FID_BEST_BID_VOLUME_CLIENT, truncateDouble(cache->vol_bid_client, _iMaxLength));
	if (DOUBLE_NULL != cache->bid_price)
		mp->SetString(FID_BID_PRICE, truncateDouble(cache->bid_price, _iMaxLength));
	if (DOUBLE_NULL != cache->bid_return_point)
		mp->SetString(FID_BID_FAN_DIAN, truncateDouble(cache->bid_return_point, _iMaxLength));
	mp->SetString(FID_BID_FAN_DIAN_FLAG, cache->bid_rebate);

	mp->SetInt32(FID_BEST_BID_QUOTE_COUNT, cache->bid_offer_count);
	mp->SetInt32(FID_BID_QUOTE_COUNT, cache->internally_bid_offer_count);
	mp->SetString(FID_BID_INTERNAL_FLAG, cache->best_bid_internal_flag);
	mp->SetString(FID_BID_DESCRIPTION, cache->best_bid_description);

	mp->SetString(FID_BID_BARGAIN_FLAG, cache->bid_flag_bargain);
	mp->SetString(FID_BID_OCO_FLAG, cache->bid_flag_relation);

	if (DOUBLE_NULL != cache->bid_full_price)
		mp->SetString(FID_BEST_BID_DIRTY_PRICE, truncateDouble(cache->bid_full_price, _iMaxLength));
	if (DOUBLE_NULL != cache->bid_net_price)
		mp->SetString(FID_BEST_BID_CLEAN_PRICE, truncateDouble(cache->bid_net_price, _iMaxLength));
	if (DOUBLE_NULL != cache->bid_spread)
		mp->SetString(FID_BEST_BID_SPREAD, truncateDouble(cache->bid_spread, _iMaxLength));

	mp->SetString(FID_BID_QUOTE_TYPE, cache->bid_quote_type);
	mp->SetString(FID_BID_YIELD_TYPE, cache->bid_exercise);
	if (DOUBLE_NULL != cache->bid_yield) {
		mp->SetString(FID_BID_YIELD, truncateDouble(cache->bid_yield, _iMaxLength));
	}
	if (DOUBLE_NULL != cache->internally_bid_price)
		mp->SetString(FID_BEST_INTERNAL_BID_PRICE, truncateDouble(cache->internally_bid_price, _iMaxLength));
	if (DOUBLE_NULL != cache->internally_bid_return_point)
		mp->SetString(FID_BEST_INTERNAL_BID_FANDIAN, truncateDouble(cache->internally_bid_return_point, _iMaxLength));
	mp->SetBool(FID_BEST_INTERNAL_BID_FANDIAN_FLAG, cache->internally_bid_rebate == "1");
	mp->SetString(FID_INTERNAL_BID_QUOTE_ID, cache->internally_bid_offer_id);

	// 计算价格字段和返点字段
	std::string bid_price_string, bid_fan_dian_string;
	GetBestPriceStrings(cache->bid_price, cache->bid_rebate, cache->bid_return_point, cache->bid_offer_id, 1, bid_price_string, bid_fan_dian_string);

	mp->SetString(FID_BID_PRICE_STRING, bid_price_string);
	mp->SetString(FID_BID_FAN_DIAN_STRING, bid_fan_dian_string);

	//计算内部价格和返点
	std::string internal_bid_price_string, internal_bid_fandian_string;
	GetBestPriceStrings(cache->internally_bid_price, cache->internally_bid_rebate, cache->internally_bid_return_point, cache->internally_bid_offer_id, 1, internal_bid_price_string, internal_bid_fandian_string);

	mp->SetString(FID_INTERNAL_BID_PRICE_STRING, internal_bid_price_string);
	mp->SetString(FID_INTERNAL_BID_FAN_DIAN_STRING, internal_bid_fandian_string);

	//
	// Ofr
	mp->SetString(FID_OFR_QUOTE_ID, cache->ofr_offer_id);
	mp->SetString(FID_OFR_INSTITUTION_ID, cache->ofr_institution_id);
	mp->SetString(FID_OFR_INSTITUTION_NAME, cache->ofr_institution_name);
	mp->SetString(FID_OFR_TRADER_ID, cache->ofr_trader_id);
	mp->SetString(FID_OFR_TRADER_NAME, cache->ofr_trader_name);
	mp->SetString(FID_OFR_BROKER_ID, cache->ofr_broker_id);
	mp->SetString(FID_OFR_BROKER_NAME, cache->ofr_broker_name);
	sdbus::string ofr_ins_trader_names;
	if (!cache->ofr_institution_name.empty() && !cache->ofr_trader_name.empty()) {
		ofr_ins_trader_names = cache->ofr_institution_name + "(" + cache->ofr_trader_name + ")";
	}
	mp->SetString(FID_OFR_INSTITUTION_TRADER_NAMES, ofr_ins_trader_names);

	mp->SetBool(FID_OFR_TRADER_IS_VIP, cache->ofr_trader_is_vip);
	mp->SetBool(FID_OFR_BAD_FLAG, cache->ofr_trader_is_bad);

	mp->SetString(FID_BEST_OFR_VOLUME, cache->ofr_volume);
	if (DOUBLE_NULL != cache->vol_ofr_client)
		mp->SetString(FID_BEST_OFR_VOLUME_CLIENT, truncateDouble(cache->vol_ofr_client, _iMaxLength));
	if (DOUBLE_NULL != cache->ofr_price)
		mp->SetString(FID_OFR_PRICE, truncateDouble(cache->ofr_price, _iMaxLength));
	if (DOUBLE_NULL != cache->ofr_return_point)
		mp->SetString(FID_OFR_FAN_DIAN, truncateDouble(cache->ofr_return_point, _iMaxLength));
	mp->SetString(FID_OFR_FAN_DIAN_FLAG, cache->ofr_rebate);

	mp->SetInt32(FID_BEST_OFR_QUOTE_COUNT, cache->ofr_offer_count);
	mp->SetInt32(FID_OFR_QUOTE_COUNT, cache->internally_ofr_offer_count);
	mp->SetString(FID_OFR_INTERNAL_FLAG, cache->best_ofr_internal_flag);
	mp->SetString(FID_OFR_DESCRIPTION, cache->best_ofr_description);

	mp->SetString(FID_OFR_BARGAIN_FLAG, cache->ofr_flag_bargain);
	mp->SetString(FID_OFR_OCO_FLAG, cache->ofr_flag_relation);

	if (DOUBLE_NULL != cache->ofr_full_price)
		mp->SetString(FID_BEST_OFR_DIRTY_PRICE, truncateDouble(cache->ofr_full_price, _iMaxLength));
	if (DOUBLE_NULL != cache->ofr_net_price)
		mp->SetString(FID_BEST_OFR_CLEAN_PRICE, truncateDouble(cache->ofr_net_price, _iMaxLength));
	if (DOUBLE_NULL != cache->ofr_spread)
		mp->SetString(FID_BEST_OFR_SPREAD, truncateDouble(cache->ofr_spread, _iMaxLength));

	mp->SetString(FID_OFR_QUOTE_TYPE, cache->ofr_quote_type);
	mp->SetString(FID_OFR_YIELD_TYPE, cache->ofr_exercise);
	if (DOUBLE_NULL != cache->ofr_yield) {
		mp->SetString(FID_OFR_YIELD, truncateDouble(cache->ofr_yield, _iMaxLength));
	}
	if (DOUBLE_NULL != cache->internally_ofr_price)
		mp->SetString(FID_BEST_INTERNAL_OFR_PRICE, truncateDouble(cache->internally_ofr_price, _iMaxLength));
	if (DOUBLE_NULL != cache->internally_ofr_return_point)
		mp->SetString(FID_BEST_INTERNAL_OFR_FANDIAN, truncateDouble(cache->internally_ofr_return_point, _iMaxLength));
	mp->SetBool(FID_BEST_INTERNAL_OFR_FANDIAN_FLAG, cache->internally_ofr_rebate == "1");
	mp->SetString(FID_INTERNAL_OFR_QUOTE_ID, cache->internally_ofr_offer_id);

	// 计算价格字段和返点字段
	std::string ofr_price_string, ofr_fan_dian_string;
	GetBestPriceStrings(cache->ofr_price, cache->ofr_rebate, cache->ofr_return_point, cache->ofr_offer_id, -1, ofr_price_string, ofr_fan_dian_string);

	mp->SetString(FID_OFR_PRICE_STRING, ofr_price_string);
	mp->SetString(FID_OFR_FAN_DIAN_STRING, ofr_fan_dian_string);

	// 提前还本
	mp->SetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);

	//计算内部价格和返点
	std::string internal_ofr_price_string, internal_ofr_fandian_string;
	GetBestPriceStrings(cache->internally_ofr_price, cache->internally_ofr_rebate, cache->internally_ofr_return_point, cache->internally_ofr_offer_id, -1, internal_ofr_price_string, internal_ofr_fandian_string);

	mp->SetString(FID_INTERNAL_OFR_PRICE_STRING, internal_ofr_price_string);
	mp->SetString(FID_INTERNAL_OFR_FAN_DIAN_STRING, internal_ofr_fandian_string);

	// set FID_BROKER_ID for filter
	// only for filter, not use in other place 
	mp->SetString(FID_BROKER_ID, cache->broker_id_list);
	mp->SetString(FID_TRADER_ID, cache->trader_id_list);
	//if (cache->bid_offer_id.length() > 0 && cache->ofr_offer_id.length() > 0) {
	//    mp->SetString(FID_BROKER_ID, cache->bid_offer_id + ", " + cache->ofr_offer_id);
	//} else if (cache->bid_offer_id.length() > 0) {
	//    mp->SetString(FID_BROKER_ID, cache->bid_offer_id);
	//} else if (cache->ofr_offer_id.length() > 0) {
	//    mp->SetString(FID_BROKER_ID, cache->ofr_offer_id);
	//} else {
	//    mp->SetString(FID_BROKER_ID, "");
	//}

	mp->SetString(FID_ISSUER_RATING, cache->issuer_rating);
	mp->SetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
	mp->SetString(FID_BOND_RATING, cache->bond_rating);
	mp->SetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
	mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
	mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
	if (DOUBLE_NULL != cache->cdc_offset)
		mp->SetString(FID_CDC_OFFSET, truncateDouble(cache->cdc_offset, _iMaxLength));
	if (DOUBLE_NULL != cache->val_modified_duration)
		mp->SetString(FID_DURATION_FILTER, truncateDouble(cache->val_modified_duration, _iMaxLength));
	if (DOUBLE_NULL != cache->val_convexity)
		mp->SetString(FID_VAL_CONVEXITY, truncateDouble(cache->val_convexity, _iMaxLength));
	if (DOUBLE_NULL != cache->val_basis_point_value)
		mp->SetString(FID_VAL_BASIC_POINT_VALUE, truncateDouble(cache->val_basis_point_value, _iMaxLength));

	mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
	if (DOUBLE_NULL != cache->coupon_rate)
		mp->SetString(FID_COUPON_RATE, truncateDouble(cache->coupon_rate, _iMaxLength));
	mp->SetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
	mp->SetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
	mp->SetString(FID_RATING_INSTITUTION, cache->rating_institution);
	mp->SetString(FID_RATE_TYPE, cache->rate_type_client);
	mp->SetInt32(FID_COMPLETED_QUOTE_SORT, cache->bond_type_sort_client);
	mp->SetString(FID_COMPLETED_BOND_TYPE, cache->completed_bond_type);

	mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->SetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);

	mp->SetInt32(FID_ISSUER_RATING_ORDER, cache->issuer_rating_order);
	mp->SetInt32(FID_ISSUER_RATING_NPY_ORDER, cache->issuer_rating_npy_order);
	mp->SetInt32(FID_BOND_RATING_ORDER, cache->bond_rating_order);
	mp->SetInt32(FID_BOND_RATING_NPY_ORDER, cache->bond_rating_npy_order);
	mp->SetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);
	mp->SetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);

	// 设置过滤条件数据
	mp->SetString(FID_BOND_CATEGORY, cache->bond_category);
	mp->SetString(FID_BOND_TYPE, cache->bond_type);
	mp->SetString(FID_ENTERPRISE_TYPE, cache->enterprise_type);
	mp->SetString(FID_LISTED_MARKET, cache->listed_market);
	mp->SetString(FID_PLEDGED, cache->pledged);
	mp->SetString(FID_ACROSS_MARKET, cache->across_market);
	mp->SetString(FID_ISSUER_RATING_FILTER, cache->issuer_rating_filter);
	mp->SetString(FID_MARKET_TYPE, cache->market_type);
	mp->SetString(FID_MUNICIPAL, cache->municipal);
	mp->SetString(FID_GUARENTEED, cache->guarenteed);
	mp->SetString(FID_HAS_OPTION, cache->has_option);
	mp->SetString(FID_ON_THE_RUN, cache->on_the_run);
	mp->SetString(FID_RATE_INDEX, cache->rate_index);
	if (DOUBLE_NULL != cache->time_to_maturity_real)
		mp->SetString(FID_TIME_TO_MATURITY_REAL, truncateDouble(cache->time_to_maturity_real, _iMaxLength));
	mp->SetInt32(FID_MATURITY, cache->maturity_date);
	mp->SetString(FID_HAVING_QUOTE, cache->having_quote);
	mp->SetString(FID_TWO_SIDED, cache->two_sided);
	mp->SetString(FID_HAVING_EXTERNAL_QUOTE, cache->having_external_quote);
	mp->SetString(FID_HAVING_INTERNAL_QUOTE, cache->have_internal_quote);
	mp->SetString(FID_HAVING_BID_QUOTE, cache->having_bid_quote);
	mp->SetString(FID_HAVING_OFR_QUOTE, cache->having_ofr_quote);
	if (DOUBLE_NULL != cache->bid_ofr_spread)
		mp->SetString(FID_BID_OFR_SPREAD, truncateDouble(cache->bid_ofr_spread, _iMaxLength));
	//发行年份、行业、地区过滤
	mp->SetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->SetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code); // 使用二级行业进行过滤
	mp->SetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code); //一级行业暂时没有使用到，预留
	mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);

	mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->SetString(FID_COMPANY_BOND, cache->company_bond);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016

	// 跨市场所有Code
	mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);

	mp->SetString(FID_BEST_BID_VOLUME_TOTAL, cache->bid_volume_total == DOUBLE_NULL ? "0" : truncateDouble(cache->bid_volume_total, _iMaxLength));
	mp->SetString(FID_BEST_OFR_VOLUME_TOTAL, cache->ofr_volume_total == DOUBLE_NULL ? "0" : truncateDouble(cache->ofr_volume_total, _iMaxLength));
	mp->SetString(FID_BEST_BID_DESCRIPTION, cache->bid_description);
	mp->SetString(FID_BEST_OFR_DESCRIPTION, cache->ofr_description);
	if (cache->create_time_milli_sec != -1000) {
		mp->SetString(FID_CREATE_TIME, boost::lexical_cast<std::string>(cache->create_time_milli_sec));
	}
}

void QBSyncService::PublishBondQuote(const std::vector<BondQuoteCachePtr> &bond_quote_vec, int fid_method) {
	if (bond_quote_vec.size() == 0) {
		return;
	}
	sdbus::Message msg;
	sdbus::VariantMap *mp = msg.AddMap(FID_PUSH_MESSAGE_BOND_QUOTE);
	sdbus::VariantMap *method_mp = mp->AddMap(fid_method);

	sdbus::VariantList *list = method_mp->AddList(FID_PUSH_MESSAGE_QUOTE_LIST);

	for (size_t index = 0; index != bond_quote_vec.size(); ++index) {
		sdbus::VariantMap *mp = list->AddMap();
		SetBondQuoteMap(bond_quote_vec.at(index).get(), mp);
	}

	std::string currentCompanyId;
	if (bond_quote_vec.size() >= 1)
		currentCompanyId = bond_quote_vec.at(0).get()->company_id;
	msg.SetString(FID_PUSH_MESSAGE_COMPANY_ID, currentCompanyId);
	msg.SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(currentCompanyId));

	boost::mutex::scoped_lock lock(queue_mutex_);
	msg_queue_.push(msg);
	queue_cond_var_.notify_one();
}

void QBSyncService::PublishBondDeal(const std::vector<BondDealCachePtr> &bond_deal_vec, int fid_method) {
	sdbus::Message msg;
	sdbus::VariantMap *mp = msg.AddMap(FID_PUSH_MESSAGE_BOND_DEAL);
	sdbus::VariantMap *method_mp = mp->AddMap(fid_method);

	sdbus::VariantList *list = method_mp->AddList(FID_PUSH_MESSAGE_DEAL_LIST);

	for (size_t index = 0; index != bond_deal_vec.size(); ++index) {
		sdbus::VariantMap *mp = list->AddMap();
		SetBondDealMap(bond_deal_vec.at(index).get(), mp);
	}

	std::string currentCompanyId;
	if (bond_deal_vec.size() >= 1)
		currentCompanyId = bond_deal_vec.at(0).get()->company_id;
	msg.SetString(FID_PUSH_MESSAGE_COMPANY_ID, currentCompanyId);
	msg.SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(currentCompanyId));

	boost::mutex::scoped_lock lock(queue_mutex_);
	msg_queue_.push(msg);
	queue_cond_var_.notify_one();
}

void QBSyncService::PublishBondBestQuote(const std::vector<BondBestQuoteCachePtr>& bond_best_quote_vec, int fid_method) {
	if (bond_best_quote_vec.empty())
		return;
	LOGGER_DEBUG("Synchronize Bond Best Quote To QB, size: " << bond_best_quote_vec.size());
	sdbus::Message msg;
	sdbus::VariantMap *mp = msg.AddMap(FID_PUSH_MESSAGE_BOND_BEST_QUOTE);
	sdbus::VariantMap *method_mp = mp->AddMap(fid_method);

	sdbus::VariantList *list = method_mp->AddList(FID_PUSH_MESSAGE_BEST_QUOTE_LIST);
	int i = 0;
	for (size_t index = 0; index != bond_best_quote_vec.size(); index++) {
		BondBestQuoteCachePtr cache = bond_best_quote_vec.at(index);
		if (cache != NULL && cache.get() != NULL && cache->isValidBond) {
			i++;
			if ("1" != bond_best_quote_vec.at(index)->status || !bond_best_quote_vec.at(index)->isValidBond) {
				continue;
			}
			sdbus::VariantMap *mp = list->AddMap();
			setBondBestQuoteMap(bond_best_quote_vec.at(index).get(), mp);
		}
	}
	if (i < 1)
		return;
	std::string currentCompanyId;
	if (bond_best_quote_vec.size() >= 1)
		currentCompanyId = bond_best_quote_vec.at(0).get()->company_id;
	msg.SetString(FID_PUSH_MESSAGE_COMPANY_ID, currentCompanyId);
	msg.SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(currentCompanyId));

	boost::mutex::scoped_lock lock(queue_mutex_);
	msg_queue_.push(msg);
	queue_cond_var_.notify_one();
}
static const int _iLimit = 500;
void QBSyncService::PublishBondBestQuote(const std::vector<BondQuoteCachePtr> &bond_quote_vec, const std::string& companyId) {
	if (bond_quote_vec.empty())
		return;
	LOGGER_DEBUG("Synchronize Bond Best Quote To QB, total size: " << bond_quote_vec.size());
	BondBestQuoteCacheController bbqcc;
	std::vector<std::string> bond_key_listed_market_vec;
	int n = 0;
	for (size_t i = 0; i < bond_quote_vec.size(); i++) {
		std::string bond_key_listed_market = bond_quote_vec.at(i)->bond_key_listed_market;
		if (!(std::find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), bond_key_listed_market) != bond_key_listed_market_vec.end())) {
			bond_key_listed_market_vec.push_back(bond_key_listed_market);
		}		
		n++;
		if (n >= _iLimit || i == (bond_quote_vec.size() - 1)) {
			std::string sql = "status='1' and company_id='" + companyId + "' and bond_key_listed_market in (" + joinSql(bond_key_listed_market_vec) + ")";
			dbQuery q(sql.c_str());
			BondBestQuoteCacheVecPtr vecs = bbqcc.getCacheListByQueryInThreadSafty(q);
			sdbus::Message msg;
			sdbus::VariantMap *mp = msg.AddMap(FID_PUSH_MESSAGE_BOND_BEST_QUOTE);
			sdbus::VariantMap *method_mp = mp->AddMap(FID_PUSH_MESSAGE_METHOD_UPDATE);
			sdbus::VariantList *list = method_mp->AddList(FID_PUSH_MESSAGE_BEST_QUOTE_LIST);
			int m = 0;
			for (size_t j = 0; j < vecs->size(); j++) {
				if ("1" != vecs->at(j)->status || !vecs->at(j)->isValidBond) {
					continue;
				}
				sdbus::VariantMap *mp = list->AddMap();
				setBondBestQuoteMap(vecs->at(j).get(), mp);
				m++;
			}
			msg.SetString(FID_PUSH_MESSAGE_COMPANY_ID, companyId);
			msg.SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(companyId));
			LOGGER_DEBUG("Synchronize Bond Best Quote To QB, vec: " << vecs->size() << ", valid: " << m);
			boost::mutex::scoped_lock lock(queue_mutex_);
			msg_queue_.push(msg);
			queue_cond_var_.notify_one();
			bond_key_listed_market_vec.clear();
		}
	}
}

void QBSyncService::PublishBondBestQuote(const std::vector<std::string>& bond_key_listed_market_vec, const std::string& compangId) {
	if (bond_key_listed_market_vec.empty()) {
		return;
	}
	LOGGER_DEBUG("Synchronize Bond Best Quote To QB, total size: " << bond_key_listed_market_vec.size());
	BondBestQuoteCacheController bbqcc;
	std::vector<std::string> t_vec;
	int n = 0;
	for (size_t i = 0; i < bond_key_listed_market_vec.size(); i++) {
		std::string bond_key_listed_market = bond_key_listed_market_vec.at(i);
		if (!(std::find(t_vec.begin(), t_vec.end(), bond_key_listed_market) != t_vec.end())) {
			t_vec.push_back(bond_key_listed_market);
		}
		n++;
		if (n >= _iLimit || i == (bond_key_listed_market_vec.size() - 1)) {
			std::string sql = "status='1' and company_id='" + compangId + "' and bond_key_listed_market in (" + joinSql(t_vec) + ")";
			dbQuery q(sql.c_str());
			BondBestQuoteCacheVecPtr vecs = bbqcc.getCacheListByQueryInThreadSafty(q);
			sdbus::Message msg;
			sdbus::VariantMap *mp = msg.AddMap(FID_PUSH_MESSAGE_BOND_BEST_QUOTE);
			sdbus::VariantMap *method_mp = mp->AddMap(FID_PUSH_MESSAGE_METHOD_UPDATE);
			sdbus::VariantList *list = method_mp->AddList(FID_PUSH_MESSAGE_BEST_QUOTE_LIST);
			int m = 0;
			for (size_t j = 0; j < vecs->size(); j++) {
				if ("1" != vecs->at(j)->status || !vecs->at(j)->isValidBond) {
					continue;
				}
				sdbus::VariantMap *mp = list->AddMap();
				setBondBestQuoteMap(vecs->at(j).get(), mp);
				m++;
			}
			msg.SetString(FID_PUSH_MESSAGE_COMPANY_ID, compangId);
			msg.SetString(FID_PUSH_MESSAGE_COMPANY_NAME, GetCurrentCompanyName(compangId));
			LOGGER_DEBUG("Synchronize Bond Best Quote To QB, vec: " << vecs->size() << ", valid: " << m);
			boost::mutex::scoped_lock lock(queue_mutex_);
			msg_queue_.push(msg);
			queue_cond_var_.notify_one();
			t_vec.clear();
		}
	}
}

void QBSyncService::Publish(sdbus::Message &msg) {
	{
		boost::mutex::scoped_lock lock(mutex_);
		int last_version, version;
		if (!getPushMessageVersion(last_version)) {
			return;
		}
		version = last_version + 1;

		msg.SetString(FID_PUSH_MESSAGE_LAST_VERSION, IntToString(last_version));
		msg.SetString(FID_PUSH_MESSAGE_VERSION, IntToString(version));

		if (!savePushMessageData(msg, version)) {
			return;
		}
	}

	bool flag = conn_->Publish(topic_name_, msg);
	if (flag) {
		LOGGER_INFO("Publish QB Message Succeed. ");
	} else {
		LOGGER_ERROR("Publish QB Message Failed ");
	}
}

bool QBSyncService::getPushMessageVersion(int &version) {
	bool ret = false;
	std::string sql_str = "select max(version) as max_version from push_message_data_qb where status='1'";
	sql::ResultSet *res = DBConnector::getInstance()->executeQuery(sql_str, SCHEMA_IDB_BOND);

	if (res == NULL) {
		return false;
	}

	if (res->next()) {
		version = res->getInt("max_version");
		ret = true;
	}

	delete res;
	return ret;
}

bool QBSyncService::savePushMessageData(const sdbus::Message &msg, int version) {
	bool ret = false;
	size_t len;
	if (!VariantMapCalcSize(msg.MapValue(), len)) {
		return false;
	}

	char* buffer = new char[len];
	if (buffer == NULL) {
		return false;
	}
	memset(buffer, 0, sizeof(char) * len);

	// serialize
	if (!VariantMapEncode(msg.MapValue(), buffer, sizeof(char) * len, len)) {
		delete[] buffer;
		return false;
	}

	// save to db    
	sql::Connection* conn = NULL;
	sql::PreparedStatement *prep_stmt = NULL;
	std::string sql_str = "insert into push_message_data_qb (id, type, version, length, value, server_info, server_ipaddress, status, create_time) values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
	try {
		conn = DBConnector::getInstance()->beginTransaction(SCHEMA_IDB_BOND);
		prep_stmt = conn->prepareStatement(sql_str);
		std::istrstream blob(buffer, len);
		prep_stmt->setString(1, GetNewGuid());
		prep_stmt->setString(2, "PUSH_MESSAGE");
		prep_stmt->setInt(3, version);
		prep_stmt->setInt(4, len);
		prep_stmt->setBlob(5, &blob);
		prep_stmt->setString(6, "");
		prep_stmt->setString(7, "");
		prep_stmt->setString(8, "1");
		prep_stmt->setString(9, GetCurrentTimeString());

		if (prep_stmt->executeUpdate() > 0) {
			DBConnector::getInstance()->commit(conn);
			ret = true;
		} else {
			DBConnector::getInstance()->rollback(conn);
			ret = false;
		}
		delete prep_stmt;
		delete[] buffer;
	} catch (sql::SQLException &e) {
		LOGGER_ERROR("failed to execute update! " << sql_str);
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
	} catch (std::exception &e) {
		LOGGER_ERROR(e.what());
	} catch (...) {
		LOGGER_ERROR("unkonwn error!!! ");
	}
	return ret;
}

bool QBSyncService::loadPushMessageData(sdbus::Message &msg, int version) {
	bool ret = false;
	sdbus::VariantMap &mp = msg.MapValue();

	std::ostringstream oss;
	oss << "select length, value from push_message_data_qb where status='1' and version = " << version;
	sql::ResultSet *res = DBConnector::getInstance()->executeQuery(oss.str(), SCHEMA_IDB_BOND);

	if (res == NULL) {
		return false;
	}

	if (res->next()) {
		size_t len = res->getInt("length");
		std::istream *blob = res->getBlob("value");

		if (len > 0) {
			char* buffer = new char[len];
			memset(buffer, 0, sizeof(char) * len);
			blob->read(buffer, len);

			if (VariantMapDecode(mp, buffer, sizeof(char) * len, len)) {
				ret = true;
			}

			delete[] buffer;
		}
	}

	delete res;
	return ret;
}

void QBSyncService::getPushMessageData(const sdbus::Message *request, sdbus::Message& reply) {
	int lastVersion = 0, version = 0;
	request->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	request->GetInt32(FID_PUSH_MESSAGE_LAST_VERSION, lastVersion);
	reply.SetInt32(FID_PUSH_MESSAGE_VERSION, version);
	reply.SetInt32(FID_PUSH_MESSAGE_LAST_VERSION, lastVersion);
	if (lastVersion < version || lastVersion < 1) {
		LOGGER_WARN("request param error!!! lastVersion:" << lastVersion << ",version:" << version);
		return;
	}
	std::ostringstream oss;
	oss << "select length, value from push_message_data_qb where status='1' and version between " << version << " and " << lastVersion << " order by version";
	procQuery(oss.str(), reply);
}

void QBSyncService::getPushMessageDataByTime(const sdbus::Message *request, sdbus::Message& reply) {
	std::string startTime, endTime;
	request->GetString(FID_START_DATE, startTime);
	request->GetString(FID_END_DATE, endTime);
	reply.GetString(FID_START_DATE, startTime);
	reply.GetString(FID_END_DATE, endTime);
	if (startTime.empty() || endTime.empty()) {
		LOGGER_WARN("request param error!!! startTime:" << startTime << ",endTime:" << endTime);
		return;
	}
	std::ostringstream oss;
	oss << "select length, value from push_message_data_qb where status='1' and create_time between '" << startTime << "' and '" << endTime << "' order by version";
	procQuery(oss.str(), reply);
}

void QBSyncService::procQuery(const std::string sql, sdbus::Message& reply) {
	if (sql.empty()) {
		return;
	}
	LOGGER_DEBUG("getPushMessageData:" << sql);
	sql::ResultSet *res = DBConnector::getInstance()->executeQuery(sql, SCHEMA_IDB_BOND);
	if (res == NULL) {
		return;
	}
	sdbus::VariantList* list = reply.AddList(FID_LIST);
	while (res->next()) {
		size_t len = res->getInt("length");
		std::istream *blob = res->getBlob("value");
		if (len > 0) {
			sdbus::VariantMap* mp = list->AddMap();
			char* buffer = new char[len];
			memset(buffer, 0, sizeof(char) * len);
			blob->read(buffer, len);
			VariantMapDecode(*mp, buffer, sizeof(char) * len, len);
			delete[] buffer;
		}
	}
	delete res;
}

// QBSync Service 无需验证Token
bool QBSyncService::tokenVerification(const sdbus::Message* msg) {
	return true;
}


void QBSyncService::threadPubMsg() {
	LOGGER_DEBUG("start thread to publish QB msg")

		while (true) {
			sdbus::Message pub_msg;

			{
				boost::mutex::scoped_lock lock(queue_mutex_);

				while (msg_queue_.empty()) {
					queue_cond_var_.wait(lock);
				}

				pub_msg = msg_queue_.front();
				msg_queue_.pop();
			}

			Publish(pub_msg);
		}
}

