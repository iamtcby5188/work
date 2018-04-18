/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Quote Service
====================================================================================================================
   Date       Name                                  Description of Change
12-Jun-2014   Guoying Feng                          Initial version
19-Aug-2014   James Xu                              Add CopyQuote
21-Aug-2014   James Xu                              Add CopyReferredQuote
05-Sep-2014   James Xu                              Clean code and adjust create_time and deal_time
$HISTORY$
===================================================================================================================*/
#include "bondquote_service.h"
#include "business/bondquote_business.h"
#include "business/bonddeal_business.h"
#include "business/bondbestquotecalc_business.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "config.h"
#include "datamanager/quote_data_manager.h"
#include "datamanager/referquote_data_manager.h"
#include "msg2db/msg_log_service.h"

BondQuoteService::BondQuoteService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{

}

void BondQuoteService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply, zip_reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "BondQuote.QuoteList") {
        if (subject == "model") {
            GetBondQuoteList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, QuoteDataManager::GetVersion(GetCurrentCompanyId(msg)));
        } else if (subject == "model.bid") {
            GetBondBidQuoteList(msg, reply);
        } else if (subject == "model.ofr") {
            GetBondOfrQuoteList(msg, reply);
        } else if (subject == "ids") {
            GetBondQuoteIdList(msg, reply);
        }
    } else if (method == "BondQuote.ReferQuoteList") {
        if (subject == "model") {
            GetBondReferQuoteList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, ReferQuoteDataManager::GetVersion(GetCurrentCompanyId(msg)));
        }
    } else if (method == "BondQuote.CopyQuote") {
        CopyQuote(msg, reply);
    } else if (method == "BondQuote.CopyReferredQuote") {
        CopyReferredQuote(msg, reply);
    } else if (method == "BondQuote.GetBondQuoteByID") {
        GetBondQuoteByID(msg, reply);
    } else if (method == "BondQuote.GetBondQuoteByConditions") {
        GetBondQuoteByConditions(msg, reply);
	} else if (method == "BondQuote.GetBondValuation") {
		GetBondValuation(msg, reply);
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

void BondQuoteService::SetBondQuoteMap(const BondQuoteCache *cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->SetString(FID_DEL_FLAG, cache->status);
    mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->SetString(FID_BOND_KEY, cache->bond_key);
    mp->SetString(FID_BOND_CODE, cache->bond_id);
    mp->SetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
    mp->SetString(FID_PIN_YIN, cache->pin_yin);
    mp->SetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);

    mp->SetDouble(FID_COUPON_RATE, cache->coupon_rate);
    mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->SetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
    mp->SetString(FID_OPTION_TYPE, cache->option_client);

    // 计算价格字段和返点字段
    std::string price_string, fan_dian_string;
    GetPriceStrings(cache->price, cache->rebate, cache->return_point, cache->symbol, 0, price_string, fan_dian_string);

    if (cache->symbol == 1) {
        mp->SetDouble(FID_BID_PRICE, cache->price);
        mp->SetDouble(FID_BID_FAN_DIAN, cache->return_point);
        mp->SetString(FID_BID_FAN_DIAN_FLAG, cache->rebate);
        mp->SetString(FID_BID_INTERNAL_FLAG, cache->internally);
        mp->SetString(FID_BID_DESCRIPTION, cache->price_description);
        mp->SetString(FID_BID_BARGAIN_FLAG, cache->flag_bargain);
        mp->SetString(FID_BID_OCO_FLAG, cache->flag_relation);

        // 价格和返点字符串
        mp->SetString(FID_BID_PRICE_STRING, price_string);
        mp->SetString(FID_BID_FAN_DIAN_STRING, fan_dian_string);

    } else {
        mp->SetDouble(FID_OFR_PRICE, cache->price);
        mp->SetDouble(FID_OFR_FAN_DIAN, cache->return_point);
        mp->SetString(FID_OFR_FAN_DIAN_FLAG, cache->rebate);
        mp->SetString(FID_OFR_INTERNAL_FLAG, cache->internally);
        mp->SetString(FID_OFR_DESCRIPTION, cache->price_description);
        mp->SetString(FID_OFR_BARGAIN_FLAG, cache->flag_bargain);
        mp->SetString(FID_OFR_OCO_FLAG, cache->flag_relation);

        // 价格和返点字符串
        mp->SetString(FID_OFR_PRICE_STRING, price_string);
        mp->SetString(FID_OFR_FAN_DIAN_STRING, fan_dian_string);
    }

    mp->SetString(FID_QUOTE_TYPE, cache->quote_type);
    mp->SetString(FID_YIELD_TYPE, cache->exercise);

    mp->SetDouble(FID_VOLUME, cache->volume);
    mp->SetInt32(FID_QUOTE_SIDE,cache->symbol);

    mp->SetString(FID_BROKER_NAME, cache->broker_name);

    mp->SetString(FID_INSTITUTION_ID, cache->bank_id);
    mp->SetString(FID_TRADER_ID, cache->bank_agent_id);
    mp->SetString(FID_BROKER_ID, cache->operator_id);
    mp->SetString(FID_QUOTE_STATUS, cache->deal_status); 

    //为了前断减少逻辑处理，区分使用BID_BROKER_ID和OFR_BROKER_ID
    mp->SetString(FID_BID_BROKER_ID, cache->operator_id);
    mp->SetString(FID_OFR_BROKER_ID, cache->operator_id);

    mp->SetString(FID_INSTITUTION_NAME, cache->institution_name);
    mp->SetString(FID_TRADER_NAME, cache->trader_name);

    sdbus::string ins_trader_names;
    if (!cache->institution_name.empty() && !cache->trader_name.empty()) {
        ins_trader_names = cache->institution_name + "(" + cache->trader_name + ")";
    }
    mp->SetString(FID_INSTITUTION_TRADER_NAMES, ins_trader_names);

    mp->SetString(FID_CREATE_TIME, cache->create_time);
    mp->SetString(FID_REFER_TIME, cache->deal_time);

    mp->SetBool(FID_VIP_FLAG, cache->trader_is_vip);
    mp->SetBool(FID_BAD_FLAG, cache->trader_is_bad);

    mp->SetDouble(FID_CLEAN_PRICE, cache->net_price);
    mp->SetDouble(FID_DIRTY_PRICE, cache->full_price);
    mp->SetDouble(FID_SPREAD, cache->spread);

    mp->SetString(FID_ISSUER_RATING, cache->issuer_rating);
    mp->SetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
    mp->SetString(FID_BOND_RATING, cache->bond_rating);
    mp->SetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
    mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
    mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
    mp->SetDouble(FID_CDC_OFFSET, cache->cdc_offset);
	mp->SetDouble(FID_DURATION_FILTER, cache->val_modified_duration);
	mp->SetDouble(FID_VAL_CONVEXITY, cache->val_convexity);
	mp->SetDouble(FID_VAL_BASIC_POINT_VALUE, cache->val_basis_point_value);

    mp->SetString(FID_RATE_TYPE, cache->rate_type_client);
    mp->SetString(FID_RATE_INDEX, cache->rate_index);
    mp->SetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
    mp->SetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
    mp->SetString(FID_RATING_INSTITUTION, cache->rating_institution);
    mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->SetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);

    mp->SetInt32(FID_ISSUER_RATING_ORDER, cache->issuer_rating_order);
    mp->SetInt32(FID_ISSUER_RATING_NPY_ORDER, cache->issuer_rating_npy_order);
    mp->SetInt32(FID_BOND_RATING_ORDER, cache->bond_rating_order);
    mp->SetInt32(FID_BOND_RATING_NPY_ORDER, cache->bond_rating_npy_order);

	// 提前还本
	mp->SetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);
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
    mp->SetInt32(FID_MATURITY, cache->maturity_date);
    mp->SetString(FID_QUOTE_INTERNAL_FLAG, cache->internally);     // for filter
	mp->SetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->SetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code);
	mp->SetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code);
    mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);
    mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->SetString(FID_COMPANY_BOND, cache->company_bond);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
    
	mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);

    // 跨市场所有Code
    mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void BondQuoteService::SetBondQuoteCache(const sdbus::VariantMap *mp, BondQuoteCache *cache)
{
    mp->GetString(FID_ID, cache->id);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->GetString(FID_DEL_FLAG, cache->status);
    mp->GetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->GetString(FID_BOND_KEY, cache->bond_key);
    mp->GetString(FID_BOND_CODE, cache->bond_id);
    mp->GetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
    mp->GetString(FID_PIN_YIN, cache->pin_yin);
    mp->GetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->GetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);

    mp->GetDouble(FID_COUPON_RATE, cache->coupon_rate);
    mp->GetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->GetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
    mp->GetString(FID_OPTION_TYPE, cache->option_client);

    mp->GetString(FID_QUOTE_TYPE, cache->quote_type);
    mp->GetString(FID_YIELD_TYPE, cache->exercise);

    mp->GetDouble(FID_VOLUME, cache->volume);
    mp->GetInt64(FID_QUOTE_SIDE,cache->symbol);

    if (cache->symbol == 1) {
        mp->GetDouble(FID_BID_PRICE, cache->price);
        mp->GetDouble(FID_BID_FAN_DIAN, cache->return_point);
        mp->GetString(FID_BID_FAN_DIAN_FLAG, cache->rebate);
        mp->GetString(FID_BID_INTERNAL_FLAG, cache->internally);
        mp->GetString(FID_BID_DESCRIPTION, cache->price_description);
        mp->GetString(FID_BID_BARGAIN_FLAG, cache->flag_bargain);
        mp->GetString(FID_BID_OCO_FLAG, cache->flag_relation);
    } else {
        mp->GetDouble(FID_OFR_PRICE, cache->price);
        mp->GetDouble(FID_OFR_FAN_DIAN, cache->return_point);
        mp->GetString(FID_OFR_FAN_DIAN_FLAG, cache->rebate);
        mp->GetString(FID_OFR_INTERNAL_FLAG, cache->internally);
        mp->GetString(FID_OFR_DESCRIPTION, cache->price_description);
        mp->GetString(FID_OFR_BARGAIN_FLAG, cache->flag_bargain);
        mp->GetString(FID_OFR_OCO_FLAG, cache->flag_relation);
    }

    mp->GetString(FID_BROKER_NAME, cache->broker_name);

    mp->GetString(FID_INSTITUTION_ID, cache->bank_id);
    mp->GetString(FID_TRADER_ID, cache->bank_agent_id);
    mp->GetString(FID_BROKER_ID, cache->operator_id);
    mp->GetString(FID_QUOTE_STATUS, cache->deal_status); 

    //为了前断减少逻辑处理，区分使用BID_BROKER_ID和OFR_BROKER_ID
    mp->GetString(FID_BID_BROKER_ID, cache->operator_id);
    mp->GetString(FID_OFR_BROKER_ID, cache->operator_id);

    mp->GetString(FID_INSTITUTION_NAME, cache->institution_name);
    mp->GetString(FID_TRADER_NAME, cache->trader_name);

    mp->GetString(FID_CREATE_TIME, cache->create_time);
    mp->GetString(FID_REFER_TIME, cache->deal_time);

    mp->GetBool(FID_VIP_FLAG, cache->trader_is_vip);
    mp->GetBool(FID_BAD_FLAG, cache->trader_is_bad);

    mp->GetDouble(FID_CLEAN_PRICE, cache->net_price);
    mp->GetDouble(FID_DIRTY_PRICE, cache->full_price);
    mp->GetDouble(FID_SPREAD, cache->spread);

    mp->GetString(FID_ISSUER_RATING, cache->issuer_rating);
    mp->GetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
    mp->GetString(FID_BOND_RATING, cache->bond_rating);
    mp->GetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
    mp->GetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
    mp->GetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
    mp->GetDouble(FID_CDC_OFFSET, cache->cdc_offset);
	mp->GetDouble(FID_DURATION_FILTER, cache->val_modified_duration);
	mp->GetDouble(FID_VAL_CONVEXITY, cache->val_convexity);
	mp->GetDouble(FID_VAL_BASIC_POINT_VALUE, cache->val_basis_point_value);

    mp->GetString(FID_RATE_TYPE, cache->rate_type_client);
    mp->GetString(FID_RATE_INDEX, cache->rate_index);
    mp->GetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
    mp->GetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
    mp->GetString(FID_RATING_INSTITUTION, cache->rating_institution);
    mp->GetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->GetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);

    mp->GetInt32(FID_ISSUER_RATING_ORDER, cache->issuer_rating_order);
    mp->GetInt32(FID_ISSUER_RATING_NPY_ORDER, cache->issuer_rating_npy_order);
    mp->GetInt32(FID_BOND_RATING_ORDER, cache->bond_rating_order);
    mp->GetInt32(FID_BOND_RATING_NPY_ORDER, cache->bond_rating_npy_order);

	// 提前还本
	mp->GetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);
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
    mp->GetInt32(FID_MATURITY, cache->maturity_date);
    mp->GetString(FID_QUOTE_INTERNAL_FLAG, cache->internally);     // for filter
	mp->GetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->GetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code);
	mp->GetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code);
    mp->GetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->GetString(FID_FINANCING_PLATFORM, cache->financing_platform);
    mp->GetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->GetString(FID_COMPANY_BOND, cache->company_bond);
	mp->GetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
    
    mp->GetString(FID_BOND_SUBTYPE, cache->bond_subtype);

    // 跨市场所有Code
    mp->GetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void BondQuoteService::SetBondReferQuoteMap(const BondQuoteReferCache *cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->SetString(FID_DEL_FLAG, cache->status);
    mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->SetString(FID_BOND_KEY, cache->bond_key);
    mp->SetString(FID_BOND_CODE, cache->bond_id);
    mp->SetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
    mp->SetString(FID_PIN_YIN, cache->pin_yin);
    mp->SetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);

    mp->SetDouble(FID_COUPON_RATE, cache->coupon_rate);
    mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->SetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
    mp->SetString(FID_OPTION_TYPE, cache->option_client);

    // 计算价格字段和返点字段
    std::string price_string, fan_dian_string;
    GetPriceStrings(cache->price, cache->rebate, cache->return_point, cache->symbol, 0, price_string, fan_dian_string);

    if (cache->symbol == 1) {
        mp->SetDouble(FID_BID_PRICE, cache->price);
        mp->SetDouble(FID_BID_FAN_DIAN, cache->return_point);
        mp->SetString(FID_BID_FAN_DIAN_FLAG, cache->rebate);
        mp->SetString(FID_BID_INTERNAL_FLAG, cache->internally);
        mp->SetString(FID_BID_DESCRIPTION, cache->price_description);
        mp->SetString(FID_BID_BARGAIN_FLAG, cache->flag_bargain);
        mp->SetString(FID_BID_OCO_FLAG, cache->flag_relation);

        // 价格和返点字符串
        mp->SetString(FID_BID_PRICE_STRING, price_string);
        mp->SetString(FID_BID_FAN_DIAN_STRING, fan_dian_string);

    } else {
        mp->SetDouble(FID_OFR_PRICE, cache->price);
        mp->SetDouble(FID_OFR_FAN_DIAN, cache->return_point);
        mp->SetString(FID_OFR_FAN_DIAN_FLAG, cache->rebate);
        mp->SetString(FID_OFR_INTERNAL_FLAG, cache->internally);
        mp->SetString(FID_OFR_DESCRIPTION, cache->price_description);
        mp->SetString(FID_OFR_BARGAIN_FLAG, cache->flag_bargain);
        mp->SetString(FID_OFR_OCO_FLAG, cache->flag_relation);

        // 价格和返点字符串
        mp->SetString(FID_OFR_PRICE_STRING, price_string);
        mp->SetString(FID_OFR_FAN_DIAN_STRING, fan_dian_string);
    }

    mp->SetString(FID_QUOTE_TYPE, cache->quote_type);
    mp->SetString(FID_YIELD_TYPE, cache->exercise);

    mp->SetDouble(FID_VOLUME, cache->volume);
    mp->SetInt32(FID_QUOTE_SIDE,cache->symbol);

    mp->SetString(FID_BROKER_NAME, cache->broker_name);

    mp->SetString(FID_INSTITUTION_ID, cache->bank_id);
    mp->SetString(FID_TRADER_ID, cache->bank_agent_id);
    mp->SetString(FID_BROKER_ID, cache->operator_id);
    mp->SetString(FID_QUOTE_STATUS, cache->deal_status); 

    //为了前断减少逻辑处理，区分使用BID_BROKER_ID和OFR_BROKER_ID
    mp->SetString(FID_BID_BROKER_ID, cache->operator_id);
    mp->SetString(FID_OFR_BROKER_ID, cache->operator_id);

    mp->SetString(FID_INSTITUTION_NAME, cache->institution_name);
    mp->SetString(FID_TRADER_NAME, cache->trader_name);

    sdbus::string ins_trader_names;
    if (!cache->institution_name.empty() && !cache->trader_name.empty()) {
        ins_trader_names = cache->institution_name + "(" + cache->trader_name + ")";
    }
    mp->SetString(FID_INSTITUTION_TRADER_NAMES, ins_trader_names);

    mp->SetString(FID_CREATE_TIME, cache->create_time);
    mp->SetString(FID_REFER_TIME, cache->deal_time);

    mp->SetBool(FID_VIP_FLAG, cache->trader_is_vip);
    mp->SetBool(FID_BAD_FLAG, cache->trader_is_bad);

    mp->SetDouble(FID_CLEAN_PRICE, cache->net_price);
    mp->SetDouble(FID_DIRTY_PRICE, cache->full_price);
    mp->SetDouble(FID_SPREAD, cache->spread);

    mp->SetString(FID_ISSUER_RATING, cache->issuer_rating);
    mp->SetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
    mp->SetString(FID_BOND_RATING, cache->bond_rating);
    mp->SetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
    mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
    mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
    mp->SetDouble(FID_CDC_OFFSET, cache->cdc_offset);
	mp->SetDouble(FID_DURATION_FILTER, cache->val_modified_duration);
	mp->SetDouble(FID_VAL_CONVEXITY, cache->val_convexity);
	mp->SetDouble(FID_VAL_BASIC_POINT_VALUE, cache->val_basis_point_value);

    mp->SetString(FID_RATE_TYPE, cache->rate_type_client);
    mp->SetString(FID_RATE_INDEX, cache->rate_index);
    mp->SetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
    mp->SetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
    mp->SetString(FID_RATING_INSTITUTION, cache->rating_institution);
    mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->SetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);
	// 提前还本
	mp->SetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);

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
    mp->SetInt32(FID_MATURITY, cache->maturity_date);
    mp->SetString(FID_QUOTE_INTERNAL_FLAG, cache->internally);     // for filter
	mp->SetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->SetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code);
	mp->SetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code);
    mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);
    mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->SetString(FID_COMPANY_BOND, cache->company_bond);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016

    mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);

    // 跨市场所有Code
    mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void BondQuoteService::SetBondReferQuoteCache(const sdbus::VariantMap *mp, BondQuoteReferCache *cache)
{
    mp->GetString(FID_ID, cache->id);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->GetString(FID_DEL_FLAG, cache->status);
    mp->GetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->GetString(FID_BOND_KEY, cache->bond_key);
    mp->GetString(FID_BOND_CODE, cache->bond_id);
    mp->GetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
    mp->GetString(FID_PIN_YIN, cache->pin_yin);
    mp->GetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->GetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);

    mp->GetDouble(FID_COUPON_RATE, cache->coupon_rate);
    mp->GetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->GetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
    mp->GetString(FID_OPTION_TYPE, cache->option_client);

    mp->GetString(FID_QUOTE_TYPE, cache->quote_type);
    mp->GetString(FID_YIELD_TYPE, cache->exercise);

    mp->GetDouble(FID_VOLUME, cache->volume);
    mp->GetInt64(FID_QUOTE_SIDE,cache->symbol);

    if (cache->symbol == 1) {
        mp->GetDouble(FID_BID_PRICE, cache->price);
        mp->GetDouble(FID_BID_FAN_DIAN, cache->return_point);
        mp->GetString(FID_BID_FAN_DIAN_FLAG, cache->rebate);
        mp->GetString(FID_BID_INTERNAL_FLAG, cache->internally);
        mp->GetString(FID_BID_DESCRIPTION, cache->price_description);
        mp->GetString(FID_BID_BARGAIN_FLAG, cache->flag_bargain);
        mp->GetString(FID_BID_OCO_FLAG, cache->flag_relation);
    } else {
        mp->GetDouble(FID_OFR_PRICE, cache->price);
        mp->GetDouble(FID_OFR_FAN_DIAN, cache->return_point);
        mp->GetString(FID_OFR_FAN_DIAN_FLAG, cache->rebate);
        mp->GetString(FID_OFR_INTERNAL_FLAG, cache->internally);
        mp->GetString(FID_OFR_DESCRIPTION, cache->price_description);
        mp->GetString(FID_OFR_BARGAIN_FLAG, cache->flag_bargain);
        mp->GetString(FID_OFR_OCO_FLAG, cache->flag_relation);
    }

    mp->GetString(FID_BROKER_NAME, cache->broker_name);

    mp->GetString(FID_INSTITUTION_ID, cache->bank_id);
    mp->GetString(FID_TRADER_ID, cache->bank_agent_id);
    mp->GetString(FID_BROKER_ID, cache->operator_id);
    mp->GetString(FID_QUOTE_STATUS, cache->deal_status); 

    //为了前断减少逻辑处理，区分使用BID_BROKER_ID和OFR_BROKER_ID
    mp->GetString(FID_BID_BROKER_ID, cache->operator_id);
    mp->GetString(FID_OFR_BROKER_ID, cache->operator_id);

    mp->GetString(FID_INSTITUTION_NAME, cache->institution_name);
    mp->GetString(FID_TRADER_NAME, cache->trader_name);

    mp->GetString(FID_CREATE_TIME, cache->create_time);
    mp->GetString(FID_REFER_TIME, cache->deal_time);

    mp->GetBool(FID_VIP_FLAG, cache->trader_is_vip);
    mp->GetBool(FID_BAD_FLAG, cache->trader_is_bad);

    mp->GetDouble(FID_CLEAN_PRICE, cache->net_price);
    mp->GetDouble(FID_DIRTY_PRICE, cache->full_price);
    mp->GetDouble(FID_SPREAD, cache->spread);

    mp->GetString(FID_ISSUER_RATING, cache->issuer_rating);
    mp->GetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_npy);
    mp->GetString(FID_BOND_RATING, cache->bond_rating);
    mp->GetString(FID_BOND_RATING_NPY, cache->bond_rating_npy);
    mp->GetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
    mp->GetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
    mp->GetDouble(FID_CDC_OFFSET, cache->cdc_offset);
	mp->GetDouble(FID_DURATION_FILTER, cache->val_modified_duration);
	mp->GetDouble(FID_VAL_CONVEXITY, cache->val_convexity);
	mp->GetDouble(FID_VAL_BASIC_POINT_VALUE, cache->val_basis_point_value);

    mp->GetString(FID_RATE_TYPE, cache->rate_type_client);
    mp->GetString(FID_RATE_INDEX, cache->rate_index);
    mp->GetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
    mp->GetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
    mp->GetString(FID_RATING_INSTITUTION, cache->rating_institution);
    mp->GetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->GetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);

    mp->GetInt32(FID_ISSUER_RATING_ORDER, cache->issuer_rating_order);
    mp->GetInt32(FID_ISSUER_RATING_NPY_ORDER, cache->issuer_rating_npy_order);
    mp->GetInt32(FID_BOND_RATING_ORDER, cache->bond_rating_order);
    mp->GetInt32(FID_BOND_RATING_NPY_ORDER, cache->bond_rating_npy_order);

	// 提前还本
	mp->GetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);
    mp->GetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);

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
    mp->GetInt32(FID_MATURITY, cache->maturity_date);
    mp->GetString(FID_QUOTE_INTERNAL_FLAG, cache->internally);     // for filter
	mp->GetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->GetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code);
	mp->GetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code);
    mp->GetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->GetString(FID_FINANCING_PLATFORM, cache->financing_platform);
    mp->GetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->GetString(FID_COMPANY_BOND, cache->company_bond);
	mp->GetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
    
	mp->GetString(FID_BOND_SUBTYPE, cache->bond_subtype);

    // 跨市场所有Code
    mp->GetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
}

void BondQuoteService::GetBondQuoteList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
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

    int row_cnt = business.GetBondQuotesCount(cond);

    BondQuoteCacheVecPtr quotes;
    if (start < row_cnt) {
        // Get bond quotes based on CONDITION, SORTBY & RANGE
        quotes = business.FetchBondQuotes(cond, sortby, range);
    } else {
        quotes = BondQuoteCacheVecPtr(new std::vector<BondQuoteCachePtr>());
    }

    sdbus::VariantList *list = reply.AddList(FID_LIST);

	int loop = quotes->size() >= kAmountLimit ? kAmountLimit : quotes->size();

	for (int i = 0; i != loop; ++i)
	{
		BondQuoteCache* bond_quote = quotes->at(i).get();

		sdbus::VariantMap *mp = list->AddMap();

		SetBondQuoteMap(bond_quote, mp);
	}
    /*std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteCache* bond_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondQuoteMap(bond_quote, mp);
    }*/

    reply.SetInt32(FID_ROW_COUNT, row_cnt);
}

void BondQuoteService::GetBondBidQuoteList(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondQuoteBusiness business(this);
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

	int row_cnt = business.GetBondQuotesCount(cond);

	BondQuoteCacheVecPtr quotes;
	if (start < row_cnt) {
		// Get bond quotes based on CONDITION, SORTBY & RANGE
		quotes = business.FetchBondQuotes(cond, sortby, range);
	} else {
		quotes = BondQuoteCacheVecPtr(new std::vector<BondQuoteCachePtr>());
	}

	if (quotes->size() > 0)
	{
		std::sort(quotes->begin(), quotes->end(), better_bid());
	}	

	sdbus::VariantList *list = reply.AddList(FID_LIST);

	int loop = quotes->size() >= kAmountLimit ? kAmountLimit : quotes->size();

	for (int i = 0; i != loop; ++i)
	{
		BondQuoteCache* bond_quote = quotes->at(i).get();

		sdbus::VariantMap *mp = list->AddMap();

		SetBondQuoteMap(bond_quote, mp);
	}

	/*std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
	for ( ; it != quotes->end(); ++it) {
		BondQuoteCache* bond_quote = it->get();

		sdbus::VariantMap *mp = list->AddMap();

		SetBondQuoteMap(bond_quote, mp);
	}*/

	reply.SetInt32(FID_ROW_COUNT, row_cnt);
}

void BondQuoteService::GetBondOfrQuoteList(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondQuoteBusiness business(this);
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

	int row_cnt = business.GetBondQuotesCount(cond);

	BondQuoteCacheVecPtr quotes;
	if (start < row_cnt) {
		// Get bond quotes based on CONDITION, SORTBY & RANGE
		quotes = business.FetchBondQuotes(cond, sortby, range);
	} else {
		quotes = BondQuoteCacheVecPtr(new std::vector<BondQuoteCachePtr>());
	}

	if (quotes->size() > 0)
	{
		std::sort(quotes->begin(), quotes->end(), better_ofr());
	}	

	sdbus::VariantList *list = reply.AddList(FID_LIST);

	int loop = quotes->size() >= kAmountLimit ? kAmountLimit : quotes->size();

	for (int i = 0; i != loop; ++i)
	{
		BondQuoteCache* bond_quote = quotes->at(i).get();

		sdbus::VariantMap *mp = list->AddMap();

		SetBondQuoteMap(bond_quote, mp);
	}

	/*std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
	for ( ; it != quotes->end(); ++it) {
		BondQuoteCache* bond_quote = it->get();

		sdbus::VariantMap *mp = list->AddMap();

		SetBondQuoteMap(bond_quote, mp);
	}*/

	reply.SetInt32(FID_ROW_COUNT, row_cnt);
}

void BondQuoteService::GetBondQuoteIdList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);

    // Get bond quotes based on CONDITION, SORTBY & RANGE
    std::vector<std::string> quotes = business.GetBondQuoteIdList(cond, sortby);

    LOGGER_INFO("[Id List Size]: " << quotes.size());

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<std::string>::const_iterator it = quotes.begin();
    for ( ; it != quotes.end(); ++it) {
        sdbus::Variant* var = list->AddVariant();
        var->SetString(*it);
    }
}

void BondQuoteService::GetBondQuoteByID(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Get Bond Quote by ID");

    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    std::string bond_quote_id;
    msg->GetString(FID_ID, bond_quote_id);

    BondQuoteCacheVecPtr quotes = business.GetBondQuoteByID(bond_quote_id);
    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteCache* bond_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondQuoteMap(bond_quote, mp);
    }
}

void BondQuoteService::GetBondQuoteByConditions(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Get Bond Quote by conditions");

    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    std::string bond_key_listed_market;
    std::string institution_id;
    std::string trader_id;
    std::string internal_flag;
    int quote_side;
    msg->GetString(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market);
    msg->GetString(FID_INSTITUTION_ID, institution_id);
    msg->GetString(FID_TRADER_ID, trader_id);
    msg->GetInt32(FID_QUOTE_SIDE, quote_side);
    msg->GetString(FID_QUOTE_INTERNAL_FLAG, internal_flag);

    BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions(quote_side, internal_flag, bond_key_listed_market, institution_id, trader_id);
    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteCache* bond_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondQuoteMap(bond_quote, mp);
    }
}

void BondQuoteService::GetBondValuation(const sdbus::Message* msg, sdbus::Message& reply)
{
	sdbus::VariantList bond_key_listed_market_list;
	msg->GetList(FID_LIST, bond_key_listed_market_list);

	sdbus::VariantList *list = reply.AddList(FID_LIST);

	for (int i = 0; i != bond_key_listed_market_list.Size(); ++i)
	{
		const sdbus::VariantMap& mp = bond_key_listed_market_list.Get(i)->ToMap();
		sdbus::string bond_key_listed_market;
		mp.GetString(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market);

		ChinaBondValuationCacheController chinabondvaluation_cc;
		dbQuery q;
		q = "bond_key_listed_market = ", bond_key_listed_market;

		ChinaBondValuationCacheVecPtr chinabondvaluations = chinabondvaluation_cc.getCacheListByQueryInThreadSafty(q);

		std::vector<ChinaBondValuationCachePtr>::const_iterator it = chinabondvaluations->begin();
		for ( ; it != chinabondvaluations->end(); ++it)
		{
			ChinaBondValuationCache* chinabondvaluation = it->get();

			sdbus::VariantMap *mp = list->AddMap();

			std::string valuation_yield = chinabondvaluation->valuation_yield;
			std::string::size_type index = valuation_yield.find("|");

			if (index != std::string::npos)
			{
				std::string tmp = valuation_yield.substr(0, index);
				valuation_yield = tmp;
			}

			mp->SetString(FID_BOND_KEY_LISTED_MARKET, chinabondvaluation->bond_key_listed_market);
			mp->SetString(FID_CDC_VALUATIONS_YIELD, valuation_yield);
			mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, chinabondvaluation->valuation_net_price);
		}
	}
}

void BondQuoteService::GetBondReferQuoteList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
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

    int row_cnt = business.GetBondReferQuotesCount(cond);

    BondQuoteReferCacheVecPtr quotes;
    if (start < row_cnt) {
        // Get bond refer quotes based on CONDITION, SORTBY & RANGE
        quotes = business.FetchBondReferQuotes(cond, sortby, range);
    } else {
        quotes = BondQuoteReferCacheVecPtr(new std::vector<BondQuoteReferCachePtr>());
    }

    sdbus::VariantList *list = reply.AddList(FID_LIST);

	int loop = quotes->size() >= kAmountLimit ? kAmountLimit : quotes->size();

	for (int i = 0; i != loop; ++i)
	{
		BondQuoteReferCache* bond_refer_quote = quotes->at(i).get();

		sdbus::VariantMap *mp = list->AddMap();

		SetBondReferQuoteMap(bond_refer_quote, mp);
	}

    /*std::vector<BondQuoteReferCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteReferCache* bond_refer_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondReferQuoteMap(bond_refer_quote, mp);
    }*/

    reply.SetInt32(FID_ROW_COUNT, row_cnt);
}    

void BondQuoteService::CopyQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);

    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString = business.CopyQuote(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}

void BondQuoteService::CopyReferredQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);

    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString = business.CopyReferredQuote(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}
