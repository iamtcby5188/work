/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Best Quote Service
====================================================================================================================
   Date       Name                                  Description of Change
20-Jun-2014   Huameng Jia                           Initial version
19-Aug-2014   James Xu                              Add CopyBestQuote and CopyCompletedQuote
21-Aug-2014   James Xu                              Add CopyInternalBestQuote
$HISTORY$
===================================================================================================================*/
#include "bondbestquote_service.h"
#include "business/bondbestquote_business.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "config.h"
#include "datamanager/bestquote_data_manager.h"
#include <sstream>
#include "msg2db/msg_log_service.h"

BondBestQuoteService::BondBestQuoteService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{    

}

void BondBestQuoteService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply, zip_reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "BondBestQuote.BestQuoteList") {
        if (subject == "model") {
            GetBondBestQuoteList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, BestQuoteDataManager::GetVersion(GetCurrentCompanyId(msg)));
        } else if (subject == "ids") {
            GetBondBestQuoteIdList(msg, reply);
        }
    }else if (method == "BondBestQuote.CompletedQuoteList") {
        if (subject == "model") {
            GetCompletedQuoteList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, BestQuoteDataManager::GetVersion(GetCurrentCompanyId(msg)));;
        }
    } else if (method == "BondBestQuote.GetBondBestQuoteByID") {
        GetBondBestQuoteByID(msg, reply);
    } else if (method == "BondBestQuote.CopyBestQuote") {
        CopyBestQuote(msg, reply);
    } else if (method == "BondBestQuote.CopyInternalBestQuote") {
        CopyInternalBestQuote(msg, reply);
    }

    if (msg->GetReplyTo().length() > 0) {        
        size_t len = -1;
        VariantMapCalcSize(reply.MapValue(), len);
        LOGGER_INFO("Msg Type: " <<  method << ", Message Size: " << len)

        bool flag;
        if (zipMessage(reply, zip_reply)) {
            zip_reply.SetType(msg->GetType());
            zip_reply.SetSubject(msg->GetSubject());
            zip_reply.SetMessageID(msg->GetMessageID());

            VariantMapCalcSize(zip_reply.MapValue(), len);
            LOGGER_INFO("Msg Type: " << method << ", Zip Message Size: " << len)

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

void BondBestQuoteService::SetBondBestQuoteMap(const BondBestQuoteCache *cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_BEST_QUOTE_ID, cache->id);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->SetString(FID_BOND_CODE, cache->bond_id);
    mp->SetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
    mp->SetString(FID_PIN_YIN, cache->pin_yin);
    mp->SetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
    mp->SetString(FID_OPTION_TYPE, cache->option_client);
    mp->SetString(FID_CREATE_TIME, cache->create_time);
    mp->SetString(FID_BOND_KEY, cache->bond_key);
    mp->SetString(FID_BOND_TYPE, cache->goods_type);

    //
    // Bid
    mp->SetString(FID_BID_QUOTE_ID, cache->bid_offer_id);
    mp->SetString(FID_BID_INSTITUTION_ID, cache->bid_institution_id);
    mp->SetString(FID_BID_INSTITUTION_NAME, cache->bid_institution_name);
    mp->SetString(FID_BID_TRADER_ID, cache->bid_trader_id);
    mp->SetString(FID_BID_TRADER_NAME, cache->bid_trader_name);
    mp->SetString(FID_BID_BROKER_ID, cache->bid_broker_id);
    mp->SetString(FID_BID_BROKER_NAME, cache->bid_broker_name);
    sdbus::string bid_ins_trader_names;
    if (!cache->bid_institution_name.empty() && !cache->bid_trader_name.empty())
    {
        bid_ins_trader_names = cache->bid_institution_name + "(" + cache->bid_trader_name + ")";
    }
    mp->SetString(FID_BID_INSTITUTION_TRADER_NAMES, bid_ins_trader_names);

    mp->SetBool(FID_BID_TRADER_IS_VIP, cache->bid_trader_is_vip);
    mp->SetBool(FID_BID_BAD_FLAG, cache->bid_trader_is_bad);

    mp->SetString(FID_BEST_BID_VOLUME, cache->bid_volume);
    mp->SetDouble(FID_BEST_BID_VOLUME_CLIENT, cache->vol_bid_client);
    mp->SetDouble(FID_BID_PRICE, cache->bid_price);
    mp->SetDouble(FID_BID_FAN_DIAN, cache->bid_return_point);
    mp->SetString(FID_BID_FAN_DIAN_FLAG, cache->bid_rebate);

    mp->SetInt32(FID_BEST_BID_QUOTE_COUNT, cache->bid_offer_count);
    mp->SetInt32(FID_BID_QUOTE_COUNT, cache->internally_bid_offer_count);
    mp->SetString(FID_BID_INTERNAL_FLAG, cache->best_bid_internal_flag);
    mp->SetString(FID_BID_DESCRIPTION, cache->best_bid_description);

    mp->SetString(FID_BID_BARGAIN_FLAG, cache->bid_flag_bargain);
    mp->SetString(FID_BID_OCO_FLAG, cache->bid_flag_relation);

    mp->SetDouble(FID_BEST_BID_DIRTY_PRICE, cache->bid_full_price);
    mp->SetDouble(FID_BEST_BID_CLEAN_PRICE, cache->bid_net_price);
    mp->SetDouble(FID_BEST_BID_SPREAD, cache->bid_spread);

    mp->SetString(FID_BID_QUOTE_TYPE, cache->bid_quote_type);
    mp->SetString(FID_BID_YIELD_TYPE, cache->bid_exercise);

    mp->SetDouble(FID_BEST_INTERNAL_BID_PRICE, cache->internally_bid_price);
    mp->SetDouble(FID_BEST_INTERNAL_BID_FANDIAN, cache->internally_bid_return_point);
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
    if (!cache->ofr_institution_name.empty() && !cache->ofr_trader_name.empty())
    {
        ofr_ins_trader_names= cache->ofr_institution_name + "(" + cache->ofr_trader_name + ")";
    }
    mp->SetString(FID_OFR_INSTITUTION_TRADER_NAMES, ofr_ins_trader_names);

    mp->SetBool(FID_OFR_TRADER_IS_VIP, cache->ofr_trader_is_vip);
    mp->SetBool(FID_OFR_BAD_FLAG, cache->ofr_trader_is_bad);

    mp->SetString(FID_BEST_OFR_VOLUME, cache->ofr_volume); 
    mp->SetDouble(FID_BEST_OFR_VOLUME_CLIENT, cache->vol_ofr_client);
    mp->SetDouble(FID_OFR_PRICE, cache->ofr_price);
    mp->SetDouble(FID_OFR_FAN_DIAN, cache->ofr_return_point);
    mp->SetString(FID_OFR_FAN_DIAN_FLAG, cache->ofr_rebate);

    mp->SetInt32(FID_BEST_OFR_QUOTE_COUNT, cache->ofr_offer_count);
    mp->SetInt32(FID_OFR_QUOTE_COUNT, cache->internally_ofr_offer_count);
    mp->SetString(FID_OFR_INTERNAL_FLAG, cache->best_ofr_internal_flag);
    mp->SetString(FID_OFR_DESCRIPTION, cache->best_ofr_description);

    mp->SetString(FID_OFR_BARGAIN_FLAG, cache->ofr_flag_bargain);
    mp->SetString(FID_OFR_OCO_FLAG, cache->ofr_flag_relation);

    mp->SetDouble(FID_BEST_OFR_DIRTY_PRICE, cache->ofr_full_price);
    mp->SetDouble(FID_BEST_OFR_CLEAN_PRICE, cache->ofr_net_price);
    mp->SetDouble(FID_BEST_OFR_SPREAD, cache->ofr_spread);

    mp->SetString(FID_OFR_QUOTE_TYPE, cache->ofr_quote_type);
    mp->SetString(FID_OFR_YIELD_TYPE, cache->ofr_exercise);

    mp->SetDouble(FID_BEST_INTERNAL_OFR_PRICE, cache->internally_ofr_price);
    mp->SetDouble(FID_BEST_INTERNAL_OFR_FANDIAN, cache->internally_ofr_return_point);
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
    mp->SetDouble(FID_CDC_OFFSET, cache->cdc_offset);
	mp->SetDouble(FID_DURATION_FILTER, cache->val_modified_duration);
	mp->SetDouble(FID_VAL_CONVEXITY, cache->val_convexity);
	mp->SetDouble(FID_VAL_BASIC_POINT_VALUE, cache->val_basis_point_value);

    mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->SetDouble(FID_COUPON_RATE, cache->coupon_rate);
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
    mp->SetDouble(FID_TIME_TO_MATURITY_REAL, cache->time_to_maturity_real);
    mp->SetInt32(FID_MATURITY, cache->maturity_date);
    mp->SetString(FID_HAVING_QUOTE, cache->having_quote);
    mp->SetString(FID_TWO_SIDED, cache->two_sided);
    mp->SetString(FID_HAVING_EXTERNAL_QUOTE, cache->having_external_quote);
    mp->SetString(FID_HAVING_INTERNAL_QUOTE, cache->have_internal_quote);
    mp->SetString(FID_HAVING_BID_QUOTE, cache->having_bid_quote);
    mp->SetString(FID_HAVING_OFR_QUOTE, cache->having_ofr_quote);
    mp->SetDouble(FID_BID_OFR_SPREAD, cache->bid_ofr_spread);
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
}

void BondBestQuoteService::SetBondBestQuoteCache(const sdbus::VariantMap *mp, BondBestQuoteCache *cache)
{
    mp->GetString(FID_BEST_QUOTE_ID, cache->id);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->GetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->GetString(FID_BOND_CODE, cache->bond_id);
    mp->GetString(FID_BOND_SHORT_NAME, cache->bond_short_name);
    mp->GetString(FID_PIN_YIN, cache->pin_yin);
    mp->GetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->GetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
    mp->GetString(FID_OPTION_TYPE, cache->option_client);
    mp->GetString(FID_CREATE_TIME, cache->create_time);
    mp->GetString(FID_BOND_KEY, cache->bond_key);
    mp->GetString(FID_BOND_TYPE, cache->goods_type);

    //
    // Bid
    mp->GetString(FID_BID_QUOTE_ID, cache->bid_offer_id);
    mp->GetString(FID_BID_INSTITUTION_ID, cache->bid_institution_id);
    mp->GetString(FID_BID_INSTITUTION_NAME, cache->bid_institution_name);
    mp->GetString(FID_BID_TRADER_ID, cache->bid_trader_id);
    mp->GetString(FID_BID_TRADER_NAME, cache->bid_trader_name);
    mp->GetString(FID_BID_BROKER_ID, cache->bid_broker_id);
    mp->GetString(FID_BID_BROKER_NAME, cache->bid_broker_name);

    mp->GetBool(FID_BID_TRADER_IS_VIP, cache->bid_trader_is_vip);
    mp->GetBool(FID_BID_BAD_FLAG, cache->bid_trader_is_bad);

    mp->GetString(FID_BEST_BID_VOLUME, cache->bid_volume);
    mp->GetDouble(FID_BEST_BID_VOLUME_CLIENT, cache->vol_bid_client);
    mp->GetDouble(FID_BID_PRICE, cache->bid_price);
    mp->GetDouble(FID_BID_FAN_DIAN, cache->bid_return_point);
    mp->GetString(FID_BID_FAN_DIAN_FLAG, cache->bid_rebate);

    mp->GetInt64(FID_BEST_BID_QUOTE_COUNT, cache->bid_offer_count);
    mp->GetInt64(FID_BID_QUOTE_COUNT, cache->internally_bid_offer_count);
    mp->GetString(FID_BID_INTERNAL_FLAG, cache->best_bid_internal_flag);
    mp->GetString(FID_BID_DESCRIPTION, cache->best_bid_description);

    mp->GetString(FID_BID_BARGAIN_FLAG, cache->bid_flag_bargain);
    mp->GetString(FID_BID_OCO_FLAG, cache->bid_flag_relation);

    mp->GetDouble(FID_BEST_BID_DIRTY_PRICE, cache->bid_full_price);
    mp->GetDouble(FID_BEST_BID_CLEAN_PRICE, cache->bid_net_price);
    mp->GetDouble(FID_BEST_BID_SPREAD, cache->bid_spread);

    mp->GetString(FID_BID_QUOTE_TYPE, cache->bid_quote_type);
    mp->GetString(FID_BID_YIELD_TYPE, cache->bid_exercise);

    mp->GetDouble(FID_BEST_INTERNAL_BID_PRICE, cache->internally_bid_price);
    mp->GetDouble(FID_BEST_INTERNAL_BID_FANDIAN, cache->internally_bid_return_point);

    bool best_internal_bid_fandian_flag = false;
    mp->GetBool(FID_BEST_INTERNAL_BID_FANDIAN_FLAG, best_internal_bid_fandian_flag);
    cache->internally_bid_rebate = best_internal_bid_fandian_flag ? "1" : "0";

    mp->GetString(FID_INTERNAL_BID_QUOTE_ID, cache->internally_bid_offer_id);

    //
    // Ofr
    mp->GetString(FID_OFR_QUOTE_ID, cache->ofr_offer_id);
    mp->GetString(FID_OFR_INSTITUTION_ID, cache->ofr_institution_id);
    mp->GetString(FID_OFR_INSTITUTION_NAME, cache->ofr_institution_name);
    mp->GetString(FID_OFR_TRADER_ID, cache->ofr_trader_id);
    mp->GetString(FID_OFR_TRADER_NAME, cache->ofr_trader_name);
    mp->GetString(FID_OFR_BROKER_ID, cache->ofr_broker_id);
    mp->GetString(FID_OFR_BROKER_NAME, cache->ofr_broker_name);

    mp->GetBool(FID_OFR_TRADER_IS_VIP, cache->ofr_trader_is_vip);
    mp->GetBool(FID_OFR_BAD_FLAG, cache->ofr_trader_is_bad);

    mp->GetString(FID_BEST_OFR_VOLUME, cache->ofr_volume); 
    mp->GetDouble(FID_BEST_OFR_VOLUME_CLIENT, cache->vol_ofr_client);
    mp->GetDouble(FID_OFR_PRICE, cache->ofr_price);
    mp->GetDouble(FID_OFR_FAN_DIAN, cache->ofr_return_point);
    mp->GetString(FID_OFR_FAN_DIAN_FLAG, cache->ofr_rebate);

    mp->GetInt64(FID_BEST_OFR_QUOTE_COUNT, cache->ofr_offer_count);
    mp->GetInt64(FID_OFR_QUOTE_COUNT, cache->internally_ofr_offer_count);
    mp->GetString(FID_OFR_INTERNAL_FLAG, cache->best_ofr_internal_flag);
    mp->GetString(FID_OFR_DESCRIPTION, cache->best_ofr_description);

    mp->GetString(FID_OFR_BARGAIN_FLAG, cache->ofr_flag_bargain);
    mp->GetString(FID_OFR_OCO_FLAG, cache->ofr_flag_relation);

    mp->GetDouble(FID_BEST_OFR_DIRTY_PRICE, cache->ofr_full_price);
    mp->GetDouble(FID_BEST_OFR_CLEAN_PRICE, cache->ofr_net_price);
    mp->GetDouble(FID_BEST_OFR_SPREAD, cache->ofr_spread);

    mp->GetString(FID_OFR_QUOTE_TYPE, cache->ofr_quote_type);
    mp->GetString(FID_OFR_YIELD_TYPE, cache->ofr_exercise);

    mp->GetDouble(FID_BEST_INTERNAL_OFR_PRICE, cache->internally_ofr_price);
    mp->GetDouble(FID_BEST_INTERNAL_OFR_FANDIAN, cache->internally_ofr_return_point);

    bool best_internal_ofr_fandian_flag = false;
    mp->GetBool(FID_BEST_INTERNAL_OFR_FANDIAN_FLAG, best_internal_ofr_fandian_flag);
    cache->internally_ofr_rebate = best_internal_bid_fandian_flag ? "1" : "0";

    mp->GetString(FID_INTERNAL_OFR_QUOTE_ID, cache->internally_ofr_offer_id);

	// 提前还本
	mp->GetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);
    
    mp->GetString(FID_BROKER_ID, cache->broker_id_list);
    mp->GetString(FID_TRADER_ID, cache->trader_id_list);

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

    mp->GetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->GetDouble(FID_COUPON_RATE, cache->coupon_rate);
    mp->GetString(FID_GUARANTEE_TYPE, cache->guarantee_type);
    mp->GetString(FID_ISSUE_INSTITUTION, cache->issue_institution);
    mp->GetString(FID_RATING_INSTITUTION, cache->rating_institution);
    mp->GetString(FID_RATE_TYPE, cache->rate_type_client);
    mp->GetInt64(FID_COMPLETED_QUOTE_SORT, cache->bond_type_sort_client);
    mp->GetString(FID_COMPLETED_BOND_TYPE, cache->completed_bond_type);

    mp->GetInt64(FID_LISTED_DATE, cache->listed_date);
	mp->GetInt32(FID_FILTER_MATURITY, cache->filter_maturity_date);

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
    mp->GetInt32(FID_MATURITY, cache->maturity_date);
    mp->GetString(FID_HAVING_QUOTE, cache->having_quote);
    mp->GetString(FID_TWO_SIDED, cache->two_sided);
    mp->GetString(FID_HAVING_EXTERNAL_QUOTE, cache->having_external_quote);
    mp->GetString(FID_HAVING_INTERNAL_QUOTE, cache->have_internal_quote);
    mp->GetString(FID_HAVING_BID_QUOTE, cache->having_bid_quote);
    mp->GetString(FID_HAVING_OFR_QUOTE, cache->having_ofr_quote);
    mp->GetDouble(FID_BID_OFR_SPREAD, cache->bid_ofr_spread);
	
	//发行年份、行业、地区过滤
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

void BondBestQuoteService::GetBondBestQuoteList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBestQuoteBusiness business(this);
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
    int row_cnt = business.GetBondBestQuotesCount(cond);

    BondBestQuoteCacheVecPtr best_quotes;
    if (start < row_cnt) {
        // Get bond quotes based on CONDITION, SORTBY & RANGE
        best_quotes = business.FetchBondBestQuotes(cond, sortby, range);
    } else {
        best_quotes = BondBestQuoteCacheVecPtr(new std::vector<BondBestQuoteCachePtr>());
    }

    sdbus::VariantList *list = reply.AddList(FID_LIST);

	int loop = best_quotes->size() >= kAmountLimit ? kAmountLimit : best_quotes->size();

	for (int i = 0; i != loop; ++i)
	{
		BondBestQuoteCache* best_quote = best_quotes->at(i).get();
		sdbus::VariantMap *mp = list->AddMap();

		SetBondBestQuoteMap(best_quote, mp);
	}

  /*  std::vector<BondBestQuoteCachePtr>::const_iterator it = best_quotes->begin();
    for ( ; it != best_quotes->end(); ++it) {
        BondBestQuoteCache* best_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondBestQuoteMap(best_quote, mp);		
    }
	*/

    reply.SetInt32(FID_ROW_COUNT, row_cnt);
}        

//完整报价没有条数限制
void BondBestQuoteService::GetCompletedQuoteList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBestQuoteBusiness business(this);
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

    int row_cnt = business.GetBondBestQuotesCount(cond);

    BondBestQuoteCacheVecPtr best_quotes;
    if (start < row_cnt) {
        // Get bond quotes based on CONDITION, SORTBY & RANGE
        best_quotes = business.FetchBondBestQuotes(cond, sortby, range);
    } else {
        best_quotes = BondBestQuoteCacheVecPtr(new std::vector<BondBestQuoteCachePtr>());
    }

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondBestQuoteCachePtr>::const_iterator it = best_quotes->begin();
    for ( ; it != best_quotes->end(); ++it) {
        BondBestQuoteCache* best_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondBestQuoteMap(best_quote, mp);		
    }

    reply.SetInt32(FID_ROW_COUNT, row_cnt);
}       

void BondBestQuoteService::GetBondBestQuoteByID(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Get Bond Best Quote by ID");

    BondBestQuoteBusiness business(this);
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
    msg->GetString(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market);

    // Get bond quotes based on bond_key_listed_market
    BondBestQuoteCacheVecPtr best_quotes = business.FetchBondBestQuoteById(bond_key_listed_market);

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondBestQuoteCachePtr>::const_iterator it = best_quotes->begin();
    for ( ; it != best_quotes->end(); ++it) {
        BondBestQuoteCache* best_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondBestQuoteMap(best_quote, mp);
    }
}

void BondBestQuoteService::GetBondBestQuoteIdList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBestQuoteBusiness business(this);
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
    std::vector<std::string> quotes = business.GetBondBestQuoteIdList(cond, sortby);

    LOGGER_DEBUG("[Id List Size]: " << quotes.size())

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<std::string>::const_iterator it = quotes.begin();
    for ( ; it != quotes.end(); ++it) {
        sdbus::Variant* var = list->AddVariant();
        var->SetString(*it);
    }
}

void BondBestQuoteService::CopyBestQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    sdbus::string bondtype;
    msg->GetList(FID_LIST, list);
    msg->GetString(FID_BOND_TYPE, bondtype);
    BondBestQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString;
    if (bondtype == "ALL")
        exportedString = business.CopyBestQuoteAll(list);
    else
        exportedString = business.CopyBestQuote(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}

void BondBestQuoteService::CopyInternalBestQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);

    BondBestQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString = business.CopyInternalBestQuote(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}