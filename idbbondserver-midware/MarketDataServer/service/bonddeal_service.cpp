/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Deal Service
====================================================================================================================
   Date       Name                                  Description of Change
05-Jun-2014   James Xu                              Initial version
20-Aug-2014   James Xu                              Add CopyDeal and CopyDetailedDeal
26-Dec-2014   James Xu                              Remove AddDealFromQuote
$HISTORY$
===================================================================================================================*/
#include "bonddeal_service.h"
#include "business/bonddeal_business.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "sdbus/codec.h"
#include "config.h"
#include "datamanager/deal_data_manager.h"
#include "msg2db/msg_log_service.h"

BondDealService::BondDealService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{

}

void BondDealService::processReqMsg(const sdbus::Message* msg)
{
	sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply, zip_reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "BondDeal.DealList") {
        if (subject == "model") {
            GetBondDealList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, DealDataManager::GetVersion(GetCurrentCompanyId(msg)));
        }
    } else if (method == "BondDeal.GetBondDealByID") {
        GetBondDealByID(msg, reply);
    } else if(method =="BondDeal.CopyDeal"){
        CopyDeal(msg, reply);
    } else if(method =="BondDeal.CopyDetailedDeal"){
        CopyDetailedDeal(msg, reply);
    }
    
    if (msg->GetReplyTo().length() > 0) {   
        size_t len = -1;
        VariantMapCalcSize(reply.MapValue(), len);
        LOGGER_INFO("Message Size: " << len)

        bool flag;
        if (zipMessage(reply, zip_reply)) {
            zip_reply.SetType(msg->GetType());
            zip_reply.SetSubject(msg->GetSubject());
            zip_reply.SetMessageID(msg->GetMessageID());

            VariantMapCalcSize(zip_reply.MapValue(), len);
            LOGGER_INFO("Zip Message Size: " << len)

            flag = conn_->Send(msg->GetReplyTo(), zip_reply);
        } else {           
            flag = conn_->Send(msg->GetReplyTo(), reply);
        }

        if (flag) {
            LOGGER_INFO("Reply success: " << msg->GetType())
        } else {
            LOGGER_ERROR("Reply failed: " << msg->GetType())
        }
    }
	MsgLogService::singleton()->FilterAndSaveMsg(*msg,reply);
}

void BondDealService::SetBondDealMap(const BondDealCache *cache, sdbus::VariantMap *mp)
{
	mp->SetString(FID_ID, cache->id);
	mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);

	mp->SetString(FID_DEL_FLAG, cache->status);
	mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);

	mp->SetString(FID_DIRECTION, cache->deal_type);
	mp->SetString(FID_CREATE_TIME, cache->create_time);
	mp->SetString(FID_DEAL_NO, cache->odd_numbers);
	mp->SetString(FID_BOND_KEY, cache->bond_key);

	mp->SetString(FID_BOND_CODE, cache->bond_id);
	mp->SetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
	mp->SetString(FID_PIN_YIN, cache->pin_yin);
	mp->SetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
	mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
	mp->SetDouble(FID_COUPON_RATE, cache->coupon_rate);
	mp->SetString(FID_RATE_TYPE, cache->rate_type_client);

	mp->SetDouble(FID_PRICE, cache->price);
	mp->SetDouble(FID_FAN_DIAN, cache->return_point);
	mp->SetString(FID_FAN_DIAN_FLAG, cache->rebate);
	mp->SetString(FID_QUOTE_TYPE, cache->quote_type);

	mp->SetDouble(FID_VOLUME, cache->volume);

	mp->SetString(FID_CURRENCY, cache->currency);
	mp->SetString(FID_YIELD_TYPE, cache->exercise);
	mp->SetString(FID_SETTLEMENT_TYPE, cache->time_style);
	mp->SetString(FID_SETTLEMENT_AMOUNT, cache->total_price);
	mp->SetString(FID_DEAL_TIME, cache->deal_time.substr(0, 10));
	mp->SetString(FID_DELIVERY_DATE, cache->delivery_date);
	mp->SetString(FID_SETTLEMENT_DATE, cache->settlement_date);

	mp->SetDouble(FID_YIELD, cache->yield);
	mp->SetDouble(FID_SPREAD, cache->spread);
	mp->SetDouble(FID_CLEAN_PRICE, cache->net_price);
	mp->SetDouble(FID_DIRTY_PRICE, cache->full_price);

	mp->SetString(FID_BID_IS_BRIDGE, cache->bid_bridge);
	mp->SetString(FID_BID_IS_CHARGED, cache->bid_charge);
	mp->SetString(FID_BID_INSTITUTION_NAME, cache->bid_institution_name);
	mp->SetString(FID_BID_INSTITUTION_CITY, cache->bid_bank_city);
    mp->SetString(FID_BID_TRADER_NAME, cache->bid_trader_name);
	mp->SetString(FID_BID_TRADER_ACCOUNT, cache->bid_trader_account);
    mp->SetBool(FID_BID_CHECK, cache->bid_check);
    mp->SetString(FID_BID_CHECK_DATE, cache->bid_check_date);

    mp->SetString(FID_BID_INSTITUTION_ID, cache->bid_bank_id);
    mp->SetString(FID_BID_TRADER_ID, cache->bid_agent_id);
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

	sdbus::string bid_ins_trader_names;
	if (!cache->bid_institution_name.empty() && !cache->bid_trader_name.empty()) {
		bid_ins_trader_names = cache->bid_institution_name + "(" + cache->bid_trader_name + ")";
	}
	mp->SetString(FID_BID_INSTITUTION_TRADER_NAMES, bid_ins_trader_names);
	mp->SetDouble(FID_BID_BROKERAGE_RATE, cache->bid_brokerage_rate);
	mp->SetDouble(FID_BID_DISCOUNT, cache->bid_discount);

	mp->SetString(FID_OFR_IS_BRIDGE, cache->ofr_bridge);
	mp->SetString(FID_OFR_IS_CHARGED, cache->ofr_charge);
	mp->SetString(FID_OFR_INSTITUTION_NAME, cache->ofr_institution_name);
	mp->SetString(FID_OFR_INSTITUTION_CITY, cache->ofr_bank_city);
    mp->SetString(FID_OFR_TRADER_NAME, cache->ofr_trader_name);
	mp->SetString(FID_OFR_TRADER_ACCOUNT, cache->ofr_trader_account);
    mp->SetBool(FID_OFR_CHECK, cache->ofr_check);
    mp->SetString(FID_OFR_CHECK_DATE, cache->ofr_check_date);

    mp->SetString(FID_OFR_INSTITUTION_ID, cache->ofr_bank_id);
    mp->SetString(FID_OFR_TRADER_ID, cache->ofr_agent_id);
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

	sdbus::string ofr_ins_trader_names;
	if (!cache->ofr_institution_name.empty() && !cache->ofr_trader_name.empty()) {
		ofr_ins_trader_names = cache->ofr_institution_name + "(" + cache->ofr_trader_name + ")";
	}
	mp->SetString(FID_OFR_INSTITUTION_TRADER_NAMES, ofr_ins_trader_names);
	mp->SetDouble(FID_OFR_BROKERAGE_RATE, cache->ofr_brokerage_rate);
	mp->SetDouble(FID_OFR_DISCOUNT, cache->ofr_discount);

	mp->SetString(FID_SETTLEMENT_MODE, cache->delivery_type);
	mp->SetString(FID_DESCRIPTION, cache->description);

	mp->SetBool(FID_BID_TRADER_IS_VIP, cache->bid_trader_is_vip);
	mp->SetBool(FID_OFR_TRADER_IS_VIP, cache->ofr_trader_is_vip);

	mp->SetString(FID_ISSUER_RATING, cache->issuer_rating);
	mp->SetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
	mp->SetString(FID_BOND_RATING, cache->bond_rating);
	mp->SetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
	mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
	mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
	mp->SetDouble(FID_DURATION_FILTER, cache->val_modified_duration);
	mp->SetDouble(FID_VAL_CONVEXITY, cache->val_convexity);
	mp->SetDouble(FID_VAL_BASIC_POINT_VALUE, cache->val_basis_point_value);

	mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
	mp->SetString(FID_RATE_INDEX, cache->rate_index);

	mp->SetString(FID_BID_IS_NO_BROKERAGE, cache->bidIsNoBrokerage);
	mp->SetString(FID_BID_BROKERAGE, cache->bidBrokerrageRemark);
	mp->SetString(FID_BID_TRADE_MODE, cache->bidTradeMode);
	mp->SetString(FID_BID_NO_CONFIRM, cache->bidNoConfirm);
	mp->SetString(FID_BID_NO_COMMENTS, cache->bidNoComments);
	mp->SetString(FID_OFR_IS_NO_BROKERAGE, cache->ofrIsNoBrokerage);
	mp->SetString(FID_OFR_BROKERAGE, cache->ofrBrokerrageRemark);
	mp->SetString(FID_OFR_TRADE_MODE, cache->ofrTradeMode);
	mp->SetString(FID_OFR_NO_CONFIRM, cache->ofrNoConfirm);
	mp->SetString(FID_OFR_NO_COMMENTS, cache->ofrNoComments);

	mp->SetString(FID_DEAL_STATUS, cache->deal_status);
	mp->SetBool(FID_DCS_IN_HAND, cache->inHand); //用于判断成交单是否正在处理中
	mp->SetString(FID_URGENT_STATUS, cache->urgent);

	mp->SetString(FID_BID_REMARKS, cache->bid_remarks);
	mp->SetString(FID_OFR_REMARKS, cache->ofr_remarks);
	mp->SetString(FID_REMARKS, cache->remarks);
	mp->SetString(FID_BID_SPECIAL_PARTY, cache->specialPartyBid);
	mp->SetString(FID_OFR_SPECIAL_PARTY, cache->specialPartyOfr);
	mp->SetString(FID_DCS_SPECIAL_TERMS, cache->specialTerms);
	mp->SetString(FID_FEEDBACK, cache->feedback);

	mp->SetInt32(FID_SERIAL_NUMBER, cache->sequence_number);

	mp->SetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
	mp->SetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
	mp->SetString(FID_RATING_INSTITUTION, cache->rating_institution);
	mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->SetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);
	// 提前还本
	mp->SetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);

	//发行年份、行业、地区过滤
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
	mp->SetDouble(FID_TIME_TO_MATURITY_REAL, cache->time_to_maturity_real);

	mp->SetString(FID_MATURITY, cache->maturity_date);
	mp->SetString(FID_DEAL_INTERNAL_FLAG, cache->internally);
	// set FID_BROKER_ID for filter
	// only for filter, not use in other place
	mp->SetString(FID_BROKER_ID, cache->broker_ids);

	// 计算价格字段和返点字段
	std::string price_string, fan_dian_string;
	GetPriceStrings(cache->price, cache->rebate, cache->return_point, 0, 1, price_string, fan_dian_string);

	mp->SetString(FID_PRICE_STRING, price_string);
	mp->SetString(FID_FAN_DIAN_STRING, fan_dian_string);
	mp->SetString(FID_DEAL_STATUS_STRING, GBKToUTF8(GetDcsDealStatusString(cache->deal_status, cache->inHand)));

	mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);

	// 跨市场所有Code
	mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void BondDealService::SetBondDealCache(const sdbus::VariantMap *mp, BondDealCache *cache)
{
	mp->GetString(FID_ID, cache->id);
	mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);

	mp->GetString(FID_DEL_FLAG, cache->status);
	mp->GetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);

	mp->GetString(FID_DIRECTION, cache->deal_type);
	mp->GetString(FID_CREATE_TIME, cache->create_time);
	mp->GetString(FID_DEAL_NO, cache->odd_numbers);
	mp->GetString(FID_BOND_KEY, cache->bond_key);

	mp->GetString(FID_BOND_CODE, cache->bond_id);
	mp->GetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
	mp->GetString(FID_PIN_YIN, cache->pin_yin);
	mp->GetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
	mp->GetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
	mp->GetDouble(FID_COUPON_RATE, cache->coupon_rate);
	mp->GetString(FID_RATE_TYPE, cache->rate_type_client);

	mp->GetDouble(FID_PRICE, cache->price);
	mp->GetDouble(FID_FAN_DIAN, cache->return_point);
	mp->GetString(FID_FAN_DIAN_FLAG, cache->rebate);
	mp->GetString(FID_QUOTE_TYPE, cache->quote_type);

	mp->GetDouble(FID_VOLUME, cache->volume);

	mp->GetString(FID_CURRENCY, cache->currency);
	mp->GetString(FID_YIELD_TYPE, cache->exercise);
	mp->GetString(FID_SETTLEMENT_TYPE, cache->time_style);
	mp->GetString(FID_SETTLEMENT_AMOUNT, cache->total_price);
	mp->GetString(FID_DEAL_TIME, cache->deal_time);
	mp->GetString(FID_DELIVERY_DATE, cache->delivery_date);
	mp->GetString(FID_SETTLEMENT_DATE, cache->settlement_date);

	mp->GetDouble(FID_YIELD, cache->yield);
	mp->GetDouble(FID_SPREAD, cache->spread);
	mp->GetDouble(FID_CLEAN_PRICE, cache->net_price);
	mp->GetDouble(FID_DIRTY_PRICE, cache->full_price);

	mp->GetString(FID_BID_IS_BRIDGE, cache->bid_bridge);
	mp->GetString(FID_BID_IS_CHARGED, cache->bid_charge);
	mp->GetString(FID_BID_INSTITUTION_NAME, cache->bid_institution_name);
	mp->GetString(FID_BID_INSTITUTION_CITY, cache->bid_bank_city);
    mp->GetString(FID_BID_TRADER_NAME, cache->bid_trader_name);
	mp->GetString(FID_BID_TRADER_ACCOUNT, cache->bid_trader_account);
    mp->GetBool(FID_BID_CHECK, cache->bid_check);
    mp->GetString(FID_BID_CHECK_DATE, cache->bid_check_date);

    mp->GetString(FID_BID_INSTITUTION_ID, cache->bid_bank_id);
    mp->GetString(FID_BID_TRADER_ID, cache->bid_agent_id);
    mp->GetString(FID_BID_BROKER_ID_A, cache->bid_broker_id);
	mp->GetString(FID_BID_BROKER_ID_B, cache->bidBrokerIdB);
	mp->GetString(FID_BID_BROKER_ID_C, cache->bidBrokerIdC);
	mp->GetString(FID_BID_BROKER_ID_D, cache->bidBrokerIdD);
	mp->GetString(FID_BID_BROKER_NAME_A, cache->bid_broker_name);
	mp->GetString(FID_BID_BROKER_NAME_B, cache->bidBrokerNameB);
	mp->GetString(FID_BID_BROKER_NAME_C, cache->bidBrokerNameC);
	mp->GetString(FID_BID_BROKER_NAME_D, cache->bidBrokerNameD);
	mp->GetString(FID_BID_PERCENT_A, cache->bidPercentA);
	mp->GetString(FID_BID_PERCENT_B, cache->bidPercentB);
	mp->GetString(FID_BID_PERCENT_C, cache->bidPercentC);
	mp->GetString(FID_BID_PERCENT_D, cache->bidPercentD);
	mp->GetDouble(FID_BID_BROKERAGE_RATE, cache->bid_brokerage_rate);
	mp->GetDouble(FID_BID_DISCOUNT, cache->bid_discount);

	mp->GetString(FID_OFR_IS_BRIDGE, cache->ofr_bridge);
	mp->GetString(FID_OFR_IS_CHARGED, cache->ofr_charge);
	mp->GetString(FID_OFR_INSTITUTION_NAME, cache->ofr_institution_name);
	mp->GetString(FID_OFR_INSTITUTION_CITY, cache->ofr_bank_city);
    mp->GetString(FID_OFR_TRADER_NAME, cache->ofr_trader_name);
	mp->GetString(FID_OFR_TRADER_ACCOUNT, cache->ofr_trader_account);
    mp->GetBool(FID_OFR_CHECK, cache->ofr_check);
    mp->GetString(FID_OFR_CHECK_DATE, cache->ofr_check_date);

    mp->GetString(FID_OFR_INSTITUTION_ID, cache->ofr_bank_id);
    mp->GetString(FID_OFR_TRADER_ID, cache->ofr_agent_id);
	mp->GetString(FID_OFR_BROKER_ID_A, cache->ofr_broker_id);
	mp->GetString(FID_OFR_BROKER_ID_B, cache->ofrBrokerIdB);
	mp->GetString(FID_OFR_BROKER_ID_C, cache->ofrBrokerIdC);
	mp->GetString(FID_OFR_BROKER_ID_D, cache->ofrBrokerIdD);
	mp->GetString(FID_OFR_BROKER_NAME_A, cache->ofr_broker_name);
	mp->GetString(FID_OFR_BROKER_NAME_B, cache->ofrBrokerNameB);
	mp->GetString(FID_OFR_BROKER_NAME_C, cache->ofrBrokerNameC);
	mp->GetString(FID_OFR_BROKER_NAME_D, cache->ofrBrokerNameD);
	mp->GetString(FID_OFR_PERCENT_A, cache->ofrPercentA);
	mp->GetString(FID_OFR_PERCENT_B, cache->ofrPercentB);
	mp->GetString(FID_OFR_PERCENT_C, cache->ofrPercentC);
	mp->GetString(FID_OFR_PERCENT_D, cache->ofrPercentD);
	mp->GetDouble(FID_OFR_BROKERAGE_RATE, cache->ofr_brokerage_rate);
	mp->GetDouble(FID_OFR_DISCOUNT, cache->ofr_discount);

	mp->GetString(FID_SETTLEMENT_MODE, cache->delivery_type);
	mp->GetString(FID_DESCRIPTION, cache->description);

	mp->GetBool(FID_BID_TRADER_IS_VIP, cache->bid_trader_is_vip);
	mp->GetBool(FID_OFR_TRADER_IS_VIP, cache->ofr_trader_is_vip);

	mp->GetString(FID_ISSUER_RATING, cache->issuer_rating);
	mp->GetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
	mp->GetString(FID_BOND_RATING, cache->bond_rating);
	mp->GetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
	mp->GetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
	mp->GetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
	mp->GetDouble(FID_DURATION_FILTER, cache->val_modified_duration);
	mp->GetDouble(FID_VAL_CONVEXITY, cache->val_convexity);
	mp->GetDouble(FID_VAL_BASIC_POINT_VALUE, cache->val_basis_point_value);

	mp->GetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
	mp->GetString(FID_RATE_INDEX, cache->rate_index);

	mp->GetString(FID_BID_IS_NO_BROKERAGE, cache->bidIsNoBrokerage);
	mp->GetString(FID_BID_BROKERAGE, cache->bidBrokerrageRemark);
	mp->GetString(FID_BID_TRADE_MODE, cache->bidTradeMode);
	mp->GetString(FID_BID_NO_CONFIRM, cache->bidNoConfirm);
	mp->GetString(FID_BID_NO_COMMENTS, cache->bidNoComments);
	mp->GetString(FID_OFR_IS_NO_BROKERAGE, cache->ofrIsNoBrokerage);
	mp->GetString(FID_OFR_BROKERAGE, cache->ofrBrokerrageRemark);
	mp->GetString(FID_OFR_TRADE_MODE, cache->ofrTradeMode);
	mp->GetString(FID_OFR_NO_CONFIRM, cache->ofrNoConfirm);
	mp->GetString(FID_OFR_NO_COMMENTS, cache->ofrNoComments);

	mp->GetString(FID_DEAL_STATUS, cache->deal_status);
	mp->GetBool(FID_DCS_IN_HAND, cache->inHand); //用于判断成交单是否正在处理中
	mp->GetString(FID_URGENT_STATUS, cache->urgent);

	mp->GetString(FID_BID_REMARKS, cache->bid_remarks);
	mp->GetString(FID_OFR_REMARKS, cache->ofr_remarks);
	mp->GetString(FID_REMARKS, cache->remarks);
	mp->GetString(FID_BID_SPECIAL_PARTY, cache->specialPartyBid);
	mp->GetString(FID_OFR_SPECIAL_PARTY, cache->specialPartyOfr);
	mp->GetString(FID_DCS_SPECIAL_TERMS, cache->specialTerms);
	mp->GetString(FID_FEEDBACK, cache->feedback);

	mp->GetInt32(FID_SERIAL_NUMBER, cache->sequence_number);

	mp->GetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
	mp->GetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
	mp->GetString(FID_RATING_INSTITUTION, cache->rating_institution);
	mp->GetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->GetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);
	// 提前还本
	mp->GetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);

	//发行年份、行业、地区过滤
	mp->GetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->GetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code); // 使用二级行业进行过滤
	mp->GetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code); //一级行业暂时没有使用到，预留
	mp->GetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->GetString(FID_FINANCING_PLATFORM, cache->financing_platform);

	mp->GetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->GetString(FID_COMPANY_BOND, cache->company_bond);
	mp->GetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016

	mp->GetInt32(FID_ISSUER_RATING_ORDER, cache->issuer_rating_order);
	mp->GetInt32(FID_ISSUER_RATING_NPY_ORDER, cache->issuer_rating_npy_order);
	mp->GetInt32(FID_BOND_RATING_ORDER, cache->bond_rating_order);
	mp->GetInt32(FID_BOND_RATING_NPY_ORDER, cache->bond_rating_npy_order);
	mp->GetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);
    mp->GetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);

	// 设置过滤条件数据
	mp->GetString(FID_BOND_CATEGORY, cache->bond_category);
	mp->GetString(FID_BOND_TYPE, cache->bond_type);
	mp->GetString(FID_ENTERPRISE_TYPE, cache->enterprise_type);
	mp->GetString(FID_LISTED_MARKET, cache->listed_market);
	mp->GetString(FID_PLEDGED, cache->pledged);
	mp->GetString(FID_ACROSS_MARKET, cache->across_market);
	mp->GetString(FID_ISSUER_RATING_FILTER, cache->issuer_rating_filter);
	mp->GetString(FID_MARKET_TYPE, cache->market_type);
	mp->GetString(FID_MUNICIPAL, cache->municipal);
	mp->GetString(FID_GUARENTEED, cache->guarenteed);
	mp->GetString(FID_HAS_OPTION, cache->has_option);
	mp->GetString(FID_ON_THE_RUN, cache->on_the_run);
	mp->GetString(FID_RATE_INDEX, cache->rate_index);
	mp->GetDouble(FID_TIME_TO_MATURITY_REAL, cache->time_to_maturity_real);

	mp->GetString(FID_MATURITY, cache->maturity_date);
	mp->GetString(FID_DEAL_INTERNAL_FLAG, cache->internally);
	// set FID_BROKER_ID for filter
	// only for filter, not use in other place
	mp->GetString(FID_BROKER_ID, cache->broker_ids);

	mp->GetString(FID_BOND_SUBTYPE, cache->bond_subtype);

	// 跨市场所有Code
	mp->GetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void BondDealService::GetBondDealList(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Get Bond Deal List");
    
    BondDealBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    int start = 0, cnt = 0;
    ParseRange(range, start, cnt);

    int row_cnt = business.GetBondDealCount(cond);

    BondDealCacheVecPtr deals;
    if (start < row_cnt) {
        // Get bond quotes based on CONDITION, SORTBY & RANGE
        deals = business.FetchBondDeals(cond, sortby, range);
    } else {
        deals = BondDealCacheVecPtr(new std::vector<BondDealCachePtr>());
    }
    
    sdbus::VariantList *list = reply.AddList(FID_LIST);

	int loop = deals->size() >= kAmountLimit ? kAmountLimit : deals->size();

	for (int i = 0; i != loop; ++i)
	{
		BondDealCache* bond_deal = deals->at(i).get();
		sdbus::VariantMap *mp = list->AddMap();

		SetBondDealMap(bond_deal, mp);
	}

    /*std::vector<BondDealCachePtr>::const_iterator it = deals->begin();
    for ( ; it != deals->end(); ++it) {
        BondDealCache* bond_deal = it->get();
        
        sdbus::VariantMap *mp = list->AddMap();

        SetBondDealMap(bond_deal, mp);
    }*/

    reply.SetInt32(FID_ROW_COUNT, row_cnt);
}

void BondDealService::GetBondDealByID(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Get Bond Deal by ID");

    BondDealBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    std::string bond_deal_id;
    msg->GetString(FID_ID, bond_deal_id);

    BondDealCacheVecPtr deals = business.GetBondDealByID(bond_deal_id);
    sdbus::VariantList *list = reply.AddList(FID_LIST);
    std::vector<BondDealCachePtr>::const_iterator it = deals->begin();
    for ( ; it != deals->end(); ++it) 
    {
        BondDealCache* bond_deal = it->get();
        sdbus::VariantMap *mp = list->AddMap();
        SetBondDealMap(bond_deal, mp);
    }
}

void BondDealService::CopyDeal(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);

    BondDealBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString = business.CopyDeal(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}

void BondDealService::CopyDetailedDeal(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);

    BondDealBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString = business.CopyDetailedDeal(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}