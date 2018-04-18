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
#include "msg2db/msg_log_service.h"
#include "bondquote_service.h"
#include "business/bondquote_business.h"
#include "business/bonddeal_business.h"
#include "business/bondbestquotecalc_business.h"
#include "business/oper_log.h"
#include "excel/generateexcel.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "service/publish_manager.h"
#include "config.h"
#include "increment/increment_cache_manager.h"
#include "MessageResendService.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options/detail/convert.hpp>
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/foreach.hpp>
#include<boost/property_tree/xml_parser.hpp>
#include "service/send_service.h"

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using boost::property_tree::write_xml;

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
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
        } else if (subject == "model.bid") {
            GetBondBidQuoteList(msg, reply);
        } else if (subject == "model.ofr") {
            GetBondOfrQuoteList(msg, reply);
        } else if (subject == "ids") {
            GetBondQuoteIdList(msg, reply);
        } else if (subject == "increment") {
            GetBondQuoteListInc(msg, reply);
        }
    } else if (method == "BondQuote.ReferQuoteList") {
        if (subject == "model") {
            GetBondReferQuoteList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
        } else if (subject == "increment") {
            GetBondReferQuoteListInc(msg, reply);
        }
    } else if (method == "BondQuote.Add") {
        AddQuote(msg, reply);
    } else if (method == "BondQuote.Update") {
        UpdateQuote(msg, reply);
    } else if (method == "BondQuote.DeleteReferredQuote") {
        DeleteReferredQuote(msg, reply);
    } else if (method == "BondQuote.Refer") {
        ReferQuote(msg, reply);
	} else if (method == "BondQuote.ReferAll") {
		ReferAllQuote(msg, reply);
	} else if (method == "BondQuote.DeleteQuote") {
        DeleteQuote(msg, reply);
    } else if (method == "BondQuote.UnRefer") {
        UnReferQuote(msg, reply);
    } else if (method == "BondQuote.UpdateInfo") {
        UpdateQuoteInfo(msg, reply);
    } else if (method == "BondQuote.UpdateInternalInfo") {
        UpdateInternalInfo(msg, reply);
    } else if (method == "BondQuote.GvnTknQuote") {
        GvnTknQuote(msg, reply);
	} else if (method == "BondQuote.EditReferredQuote") {
        EditReferredQuote(msg, reply);
	} else if (method == "BondQuote.UpdateReferredQuote") {
		UpdateReferredQuote(msg, reply);
    } else if (method == "BondQuote.BondExport") {
        ExportBondQuote(msg, reply);
    } else if (method == "BondQuote.CopyQuote") {
        CopyQuote(msg, reply);
    } else if (method == "BondQuote.CopyReferredQuote") {
        CopyReferredQuote(msg, reply);
    } else if (method == "BondQuote.GetBondQuoteByID") {
        GetBondQuoteByID(msg, reply);
    } else if (method == "BondQuote.UpdateYiJiBanRate") {
        UpdateYiJiBanRate(msg, reply);
    } else if (method == "BondQuote.GetYiJiBanRate") {
        GetYiJiBanRate(msg, reply);
    } else if (method == "BondQuote.GetBondQuoteByConditions") {
        GetBondQuoteByConditions(msg, reply);
	} else if (method = "BondQuote.QuoteOperLog") {
		OperLog operlog;
		operlog.GetQuoteOperLog(msg, reply);
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

	//发行年份、行业、地区过滤
	mp->SetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->SetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code); // 使用二级行业进行过滤
	mp->SetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code); //一级行业暂时没有使用到，预留
    mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);

    mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
	mp->SetString(FID_COMPANY_BOND, cache->company_bond);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016

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

	mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);

    // 跨市场所有Code
    mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
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
    mp->SetInt32(FID_MATURITY, cache->maturity_date);
    mp->SetString(FID_QUOTE_INTERNAL_FLAG, cache->internally);     // for filter

	mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);

    // 跨市场所有Code
    mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
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
	business.SetCompanyId(company_id.c_str());
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

    std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteCache* bond_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondQuoteMap(bond_quote, mp);
    }
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
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);

    // Get bond quotes based on CONDITION, SORTBY & RANGE
    BondQuoteCacheVecPtr quotes = business.FetchBondQuotes(cond, "", "");

    std::sort(quotes->begin(), quotes->end(), better_bid());

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteCache* bond_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondQuoteMap(bond_quote, mp);
    }

    int row_cnt = business.GetBondQuotesCount(cond);

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
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);

    // Get bond quotes based on CONDITION, SORTBY & RANGE
    BondQuoteCacheVecPtr quotes = business.FetchBondQuotes(cond, "", "");

    std::sort(quotes->begin(), quotes->end(), better_ofr());

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteCache* bond_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondQuoteMap(bond_quote, mp);
    }

    int row_cnt = business.GetBondQuotesCount(cond);

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
	business.SetCompanyId(company_id.c_str());
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
	business.SetCompanyId(company_id.c_str());
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
	business.SetCompanyId(company_id.c_str());
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
	business.SetCompanyId(company_id.c_str());
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

    std::vector<BondQuoteReferCachePtr>::const_iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteReferCache* bond_refer_quote = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondReferQuoteMap(bond_refer_quote, mp);
    }

    reply.SetInt32(FID_ROW_COUNT, row_cnt);
}
#include "msg2db/msg_helper.h"
void BondQuoteService::GetBondQuoteListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
    int version; 
    msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
    IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("BondQuote.QuoteList." + company_id);
    if (cache) {
        cache->Get(version, reply);
	}
}

void BondQuoteService::GetBondReferQuoteListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
    int version; 
    msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
    IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("BondQuote.ReferQuoteList." + company_id);
    if (cache) {
        cache->Get(version, reply);
	}
}

void BondQuoteService::AddQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
    BondQuoteBusiness::BondQuoteVecPtr vec_update(new std::vector<BondQuotePtr>());
    sdbus::VariantList list;    
    bool export_is_internal = false;
    msg->GetList(FID_LIST, list);
    msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        BondQuotePtr bond_quote(new BondQuote());

        GetFieldString(mp, FID_BOND_KEY_LISTED_MARKET, bond_quote->bond_key_listed_market_); 
        GetFieldString(mp, FID_INSTITUTION_ID, bond_quote->institution_id_);
        GetFieldString(mp, FID_TRADER_ID, bond_quote->trader_id_);
        GetFieldString(mp, FID_BROKER_ID, bond_quote->broker_id_);

        sdbus::VariantMap *sdMap = reply.AddMap(FID_QQ2IDB_ERROR_MAP);
        sdbus::VariantList *sdList = sdMap->AddList(FID_QQ2IDB_ERROR_LIST);
        // 6th Co., Ltd createOrGetAndPublishInstitutionTraderInfo
        if(bond_quote->bond_key_listed_market_.ref().empty() &&
            bond_quote->institution_id_.ref().empty() &&
            bond_quote->trader_id_.ref().empty())
        {
            std::string bondCode, bondShortName, bondKeyListedMarket, institutionCode, traderCode, institutionId, traderId;
            mp.GetString(FID_BOND_CODE, bondCode);
            mp.GetString(FID_BOND_SHORT_NAME, bondShortName);
            mp.GetString(FID_INSTITUTION_CODE, institutionCode);
            mp.GetString(FID_TRADER_CODE, traderCode);
            LOGGER_INFO("bondCode in AddQuote in IDB:[" << bondCode << "]")
            LOGGER_INFO("bondShortName in AddQuote in IDB:[" << bondShortName << "]")
            LOGGER_INFO("institutionCode in AddQuote in IDB:[" << institutionCode << "]")
            LOGGER_INFO("traderCode in AddQuote in IDB:[" << traderCode << "]")

            sdbus::Message request, response;
            request.SetType("Bond.createOrGetAndPublishInstitutionTraderInfo");
            request.SetString(FID_BOND_CODE, bondCode);
            request.SetString(FID_BOND_SHORT_NAME, bondShortName);
            request.SetString(FID_INSTITUTION_CODE, institutionCode);
            request.SetString(FID_TRADER_CODE, traderCode);
            bool ret = SendService::singleton()->call(kServiceQueueBondInfo, request, response, 1000);
            sdbus::Message unzipMsg;
            if(!unzipMessage(response, unzipMsg))
                unzipMsg = response;
            bool isSuccess = false;
            unzipMsg.GetBool(FID_QQ2IDB_IS_SUCCESS, isSuccess);

            sdbus::VariantMap *errMap = sdList->AddMap();
            if(!isSuccess)
            {
                errMap->SetBool(FID_QQ2IDB_IS_SUCCESS, isSuccess);
                int code;
                std::string errMsg;
                unzipMsg.GetInt32(FID_QQ2IDB_ERROR_CODE, code);
                unzipMsg.GetString(FID_QQ2IDB_ERROR_MSG, errMsg);
                errMap->SetInt32(FID_QQ2IDB_ERROR_CODE, code);
                errMap->SetString(FID_QQ2IDB_ERROR_MSG, errMsg);
                continue;
            }

            unzipMsg.GetString(FID_BOND_KEY_LISTED_MARKET, bondKeyListedMarket);
            unzipMsg.GetString(FID_INSTITUTION_ID, institutionId);
            unzipMsg.GetString(FID_TRADER_ID, traderId);
            LOGGER_INFO("bondKeyListedMarket in AddQuote in IDB:[" << bondKeyListedMarket << "]")
            LOGGER_INFO("institutionId in AddQuote in IDB:[" << institutionId << "]")
            LOGGER_INFO("traderId in AddQuote in IDB:[" << traderId << "]")

            if(bondKeyListedMarket.empty() || institutionId.empty() || traderId.empty())
            {
                std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_COULD_NOT_GET_BOND_OR_INSTITUTION_OR_TRADER) + getIntToStringWithBracket(QQ2IDB_ERROR_COULD_NOT_GET_BOND_OR_INSTITUTION_OR_TRADER);
                errMap->SetString(FID_QQ2IDB_IS_SUCCESS, false);
                errMap->SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_COULD_NOT_GET_BOND_OR_INSTITUTION_OR_TRADER);
                errMap->SetString(FID_QQ2IDB_ERROR_MSG, err);
                LOGGER_ERROR(err)
                continue;
            }
            bond_quote->bond_key_listed_market_ = bondKeyListedMarket;
            bond_quote->institution_id_ = institutionId;
            bond_quote->trader_id_ = traderId;

            errMap->SetBool(FID_QQ2IDB_IS_SUCCESS, true);
        }

        GetFieldInt(mp, FID_QUOTE_SIDE, bond_quote->quote_side_);
        GetFieldDouble(mp, FID_PRICE, bond_quote->price_);
        GetFieldDouble(mp, FID_VOLUME, bond_quote->volume_);
        GetFieldString(mp, FID_DESCRIPTION, bond_quote->description_);

        GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_quote->fan_dian_flag_);
        GetFieldDouble(mp, FID_FAN_DIAN, bond_quote->fan_dian_);
        GetFieldString(mp, FID_BARGAIN_FLAG, bond_quote->bargain_flag_);
        GetFieldString(mp, FID_OCO_FLAG, bond_quote->oco_flag_);
        GetFieldString(mp, FID_QUOTE_INTERNAL_FLAG, bond_quote->internal_flag_);

        GetFieldString(mp, FID_QUOTE_TYPE, bond_quote->quote_type_);
        GetFieldString(mp, FID_DELIVERY_DATE, bond_quote->delivery_date_);
        GetFieldString(mp, FID_YIELD_TYPE, bond_quote->yield_type_);
        BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions(bond_quote->quote_side_.ref(), bond_quote->internal_flag_.ref(), 
            bond_quote->bond_key_listed_market_.ref(), bond_quote->institution_id_.ref(), bond_quote->trader_id_.ref());
        if(quotes->size() > 0) {
            std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
            bond_quote->id_ = (*it)->id;
            vec_update->push_back(bond_quote);
        } else {
            vec_normal->push_back(bond_quote);
        }
    }

    sdbus::string exported_normal_text, exported_update_text;

	ptree bond_quote_msg_data;

    if(vec_normal->size() > 0) {
        business.AddQuote(vec_normal, &exported_normal_text, export_is_internal, &bond_quote_msg_data);
    }
    if(vec_update->size() > 0) {
        business.UpdateQuote(vec_update, &exported_update_text, export_is_internal, &bond_quote_msg_data);
    } 

    sdbus::VariantList *replay_list = reply.AddList(FID_LIST);
    std::vector<BondQuotePtr>::const_iterator it = vec_normal->begin();
    for ( ; it != vec_normal->end(); ++it) {
        BondQuote* bond_quote = it->get();
        if(bond_quote != NULL) {
            sdbus::VariantMap *mp = replay_list->AddMap();
            mp->SetString(FID_ID, bond_quote->id_);
        }
    }
    it = vec_update->begin();
    for ( ; it != vec_update->end(); ++it) {
        BondQuote* bond_quote = it->get();
        if(bond_quote != NULL) {
            sdbus::VariantMap *mp = replay_list->AddMap();
            mp->SetString(FID_ID, bond_quote->id_);
        }
    }

    reply.SetString(FID_EXPORTED_TEXT, exported_normal_text+exported_update_text);

    if (!bond_quote_msg_data.empty()) {
	    ptree add_bond_quote_msg;
	    add_bond_quote_msg.put("method", msg->GetType());
	    add_bond_quote_msg.add_child("data", bond_quote_msg_data);
	    std::ostringstream buf; 
	    write_json(buf, add_bond_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::UpdateQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
    BondQuoteBusiness::BondQuoteVecPtr vec_delete(new std::vector<BondQuotePtr>());
    sdbus::VariantList list;
    bool export_is_internal = false;
    msg->GetList(FID_LIST, list);
    msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();
        BondQuotePtr bond_quote(new BondQuote());

        GetFieldString(mp, FID_ID, bond_quote->id_);

        GetFieldString(mp, FID_BOND_KEY_LISTED_MARKET, bond_quote->bond_key_listed_market_);

        GetFieldString(mp, FID_INSTITUTION_ID, bond_quote->institution_id_);
        GetFieldString(mp, FID_TRADER_ID, bond_quote->trader_id_);
        GetFieldString(mp, FID_BROKER_ID, bond_quote->broker_id_);

        GetFieldInt(mp, FID_QUOTE_SIDE, bond_quote->quote_side_);
        GetFieldDouble(mp, FID_PRICE, bond_quote->price_);
        GetFieldDouble(mp, FID_VOLUME, bond_quote->volume_);
        GetFieldString(mp, FID_DESCRIPTION, bond_quote->description_);

        GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_quote->fan_dian_flag_);
        GetFieldDouble(mp, FID_FAN_DIAN, bond_quote->fan_dian_);
        GetFieldString(mp, FID_BARGAIN_FLAG, bond_quote->bargain_flag_);
        GetFieldString(mp, FID_OCO_FLAG, bond_quote->oco_flag_);
        GetFieldString(mp, FID_QUOTE_INTERNAL_FLAG, bond_quote->internal_flag_);

        GetFieldString(mp, FID_QUOTE_TYPE, bond_quote->quote_type_);
        GetFieldString(mp, FID_DELIVERY_DATE, bond_quote->delivery_date_);
        GetFieldString(mp, FID_YIELD_TYPE, bond_quote->yield_type_);

        BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions(bond_quote->quote_side_.ref(), bond_quote->internal_flag_.ref(), 
            bond_quote->bond_key_listed_market_.ref(), bond_quote->institution_id_.ref(), bond_quote->trader_id_.ref());

        if(quotes && quotes->size() > 0) {
            BondQuotePtr delete_quote(new BondQuote());
            std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
            if((*it)->id != bond_quote->id_.ref()) {
                delete_quote->id_ = (*it)->id;
                vec_delete->push_back(delete_quote);
            }
        }

        vec_normal->push_back(bond_quote);
    }

    sdbus::string exported_text;
	ptree bond_quote_msg_data;
    business.UpdateQuote(vec_normal, &exported_text, export_is_internal, &bond_quote_msg_data);

    if(vec_delete->size() > 0) {
        business.DeleteQuote(vec_delete);
    }
    
    reply.SetString(FID_EXPORTED_TEXT, exported_text);

    if (!bond_quote_msg_data.empty()) {
	    ptree update_bond_quote_msg;
	    update_bond_quote_msg.put("method", msg->GetType());
	    update_bond_quote_msg.add_child("data", bond_quote_msg_data);
	    std::ostringstream buf; 
	    write_json(buf, update_bond_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::UpdateQuoteInfo(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
    sdbus::VariantList list;
    bool export_is_internal = false;
    msg->GetList(FID_LIST, list);
    msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();
      
        BondQuotePtr bond_quote(new BondQuote());

        GetFieldString(mp, FID_ID, bond_quote->id_);

        GetFieldDouble(mp, FID_PRICE, bond_quote->price_);
        GetFieldDouble(mp, FID_VOLUME, bond_quote->volume_);
        GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_quote->fan_dian_flag_);
        GetFieldDouble(mp, FID_FAN_DIAN, bond_quote->fan_dian_);
        GetFieldString(mp, FID_BARGAIN_FLAG, bond_quote->bargain_flag_);
        GetFieldString(mp, FID_OCO_FLAG, bond_quote->oco_flag_);
        GetFieldString(mp, FID_DESCRIPTION, bond_quote->description_);

        vec->push_back(bond_quote);
    }

    sdbus::string exported_text;    
	ptree bond_quote_msg_data;
    business.UpdateQuoteInfo(vec, &exported_text, export_is_internal, &bond_quote_msg_data);

    reply.SetString(FID_EXPORTED_TEXT, exported_text);

    if (!bond_quote_msg_data.empty()) {
	    ptree update_bond_quote_msg;
	    update_bond_quote_msg.put("method", msg->GetType());
	    update_bond_quote_msg.add_child("data", bond_quote_msg_data);
	    std::ostringstream buf; 
	    write_json(buf, update_bond_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::UpdateReferredQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

	BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
	sdbus::VariantList list;
	msg->GetList(FID_LIST, list);
	for (unsigned int i = 0; i < list.Size(); ++i)
	{
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
		BondQuotePtr bond_quote(new BondQuote());

		GetFieldString(mp, FID_ID, bond_quote->id_);

		GetFieldString(mp, FID_BOND_KEY_LISTED_MARKET, bond_quote->bond_key_listed_market_);

		GetFieldString(mp, FID_INSTITUTION_ID, bond_quote->institution_id_);
		GetFieldString(mp, FID_TRADER_ID, bond_quote->trader_id_);
		GetFieldString(mp, FID_BROKER_ID, bond_quote->broker_id_);

		GetFieldInt(mp, FID_QUOTE_SIDE, bond_quote->quote_side_);
		GetFieldDouble(mp, FID_PRICE, bond_quote->price_);
		GetFieldDouble(mp, FID_VOLUME, bond_quote->volume_);
		GetFieldString(mp, FID_DESCRIPTION, bond_quote->description_);

		GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_quote->fan_dian_flag_);
		GetFieldDouble(mp, FID_FAN_DIAN, bond_quote->fan_dian_);
		GetFieldString(mp, FID_BARGAIN_FLAG, bond_quote->bargain_flag_);
		GetFieldString(mp, FID_OCO_FLAG, bond_quote->oco_flag_);
		GetFieldString(mp, FID_QUOTE_INTERNAL_FLAG, bond_quote->internal_flag_);

		GetFieldString(mp, FID_QUOTE_TYPE, bond_quote->quote_type_);
		GetFieldString(mp, FID_DELIVERY_DATE, bond_quote->delivery_date_);
		GetFieldString(mp, FID_YIELD_TYPE, bond_quote->yield_type_);

		vec_normal->push_back(bond_quote);
	}

	ptree refer_quote_msg_data;
	business.UpdateReferQuote(vec_normal, NULL, false, &refer_quote_msg_data);

    if (!refer_quote_msg_data.empty()) {
	    // For old bond, just referred
	    ptree update_refer_quote_msg;
	    update_refer_quote_msg.put("method", msg->GetType());
	    update_refer_quote_msg.add_child("data", refer_quote_msg_data);
	    std::ostringstream buf; 
	    write_json(buf, update_refer_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::UpdateInternalInfo(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
    BondQuoteBusiness::BondQuoteVecPtr vec_delete(new std::vector<BondQuotePtr>());

    sdbus::VariantList list;
    bool export_is_internal = false;
    msg->GetList(FID_LIST, list);
    msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();      
        BondQuotePtr bond_quote(new BondQuote());

        GetFieldString(mp, FID_ID, bond_quote->id_);
        GetFieldString(mp, FID_QUOTE_INTERNAL_FLAG, bond_quote->internal_flag_);
        GetFieldString(mp, FID_BARGAIN_FLAG, bond_quote->bargain_flag_);

        BondQuoteCacheVecPtr quote_update = business.GetBondQuoteByID(bond_quote->id_.ref());
        if(quote_update && quote_update->size() > 0) {
            std::vector<BondQuoteCachePtr>::const_iterator update_it = quote_update->begin();

            BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions((*update_it)->symbol, bond_quote->internal_flag_.ref(), 
                (*update_it)->bond_key_listed_market, (*update_it)->bank_id, (*update_it)->bank_agent_id);
            if(quotes && quotes->size() > 0) {
                BondQuotePtr delete_quote(new BondQuote());
                std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();                
                if((*it)->id != bond_quote->id_.ref()) {
                    delete_quote->id_ = (*it)->id;
                    vec_delete->push_back(delete_quote);
                }
            }
        }

        vec_normal->push_back(bond_quote);
    }

    sdbus::string exported_text;
	ptree bond_quote_msg;
    business.UpdateQuoteInfo(vec_normal, &exported_text, export_is_internal, &bond_quote_msg);

    if(vec_delete->size() > 0) {
        business.DeleteQuote(vec_delete);
    }

    reply.SetString(FID_EXPORTED_TEXT, exported_text);

    if (!bond_quote_msg.empty()) {
	    ptree update_interal_info_msg;
	    update_interal_info_msg.put("method", msg->GetType());
	    update_interal_info_msg.add_child("data",bond_quote_msg);
	    std::ostringstream buf; 
	    write_json(buf, update_interal_info_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::DeleteReferredQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        BondQuotePtr bond_quote(new BondQuote());

        bond_quote->id_ = list.Get(i)->ToString().c_str();

        vec->push_back(bond_quote);
    }

	ptree refer_quote_msg_data;
    business.DeleteReferredQuote(vec, NULL, &refer_quote_msg_data);

    if (!refer_quote_msg_data.empty()) {
	    ptree delete_refer_quote_msg;
	    delete_refer_quote_msg.put("method", msg->GetType());
	    delete_refer_quote_msg.add_child("data", refer_quote_msg_data);
	    std::ostringstream buf; 
	    write_json(buf, delete_refer_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::ReferQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Refer Bond Quote");

    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
    sdbus::VariantList list;    
    bool export_is_internal = false;
    msg->GetList(FID_LIST, list);
    msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        BondQuotePtr bond_quote(new BondQuote());

        bond_quote->id_ = list.Get(i)->ToString().c_str();

        vec->push_back(bond_quote);
    }

    sdbus::string exported_text;   
	ptree refer_quote_msg_data;
    business.ReferBondQuote(vec, &exported_text, export_is_internal, &refer_quote_msg_data);  

    reply.SetString(FID_EXPORTED_TEXT, exported_text);

    if (!refer_quote_msg_data.empty()) {
	    ptree refer_quote_msg;
	    refer_quote_msg.put("method", msg->GetType());
	    refer_quote_msg.add_child("data", refer_quote_msg_data);
	    std::ostringstream buf; 
	    write_json(buf, refer_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::ReferAllQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
	LOGGER_INFO("Refer All Bond Quote");

	BondQuoteBusiness business(this);
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if (company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);

	std::string bond_category = "";
	msg->GetString(FID_BOND_CATEGORY, bond_category);

    business.ReferBondQuoteAll(bond_category);
}

void BondQuoteService::DeleteQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Delete Bond Quote");
    
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
    sdbus::VariantList list;
    bool export_is_internal = false;
    msg->GetList(FID_LIST, list);
    msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        BondQuotePtr bond_quote(new BondQuote());

        bond_quote->id_ = list.Get(i)->ToString().c_str();

        vec->push_back(bond_quote);
    }

    sdbus::string exported_text;    
	ptree bond_quote_msg;
    business.DeleteQuote(vec, &exported_text, export_is_internal, &bond_quote_msg);

    reply.SetString(FID_EXPORTED_TEXT, exported_text);

    if (!bond_quote_msg.empty()) {
	    ptree delete_bond_quote_msg;
	    delete_bond_quote_msg.put("method", msg->GetType());
	    delete_bond_quote_msg.add_child("data", bond_quote_msg);
	    std::ostringstream buf; 
	    write_json(buf, delete_bond_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::UnReferQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Unrefer Bond Quote");
    
    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

    BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
    BondQuoteBusiness::BondQuoteVecPtr vec_delete(new std::vector<BondQuotePtr>());

    sdbus::VariantList list;
    bool export_is_internal = false;
    msg->GetList(FID_LIST, list);
    msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
    for (unsigned int i = 0; i < list.Size(); ++i)
    { 
        BondQuotePtr bond_quote(new BondQuote());

        bond_quote->id_ = list.Get(i)->ToString().c_str();

        BondQuoteReferCacheVecPtr quote_refer = business.GetBondReferByID(bond_quote->id_.ref());
        if(quote_refer && quote_refer->size() > 0) {
            std::vector<BondQuoteReferCachePtr>::const_iterator refer_it = quote_refer->begin();

            BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions((*refer_it)->symbol, (*refer_it)->internally, 
                (*refer_it)->bond_key_listed_market, (*refer_it)->bank_id, (*refer_it)->bank_agent_id);
            if(quotes && quotes->size() > 0) {
                BondQuotePtr delete_quote(new BondQuote());
                std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
                if((*it)->id != bond_quote->id_.ref()) {
                    delete_quote->id_ = (*it)->id;
                    vec_delete->push_back(delete_quote);
                }
            } 
        }

        vec_normal->push_back(bond_quote);
    }
    sdbus::string exported_text;
	ptree bond_quote_msg;
    business.UnreferBondQuote(vec_normal, &exported_text, export_is_internal, &bond_quote_msg);

    if(vec_delete->size() > 0) {
        business.DeleteQuote(vec_delete);
    }

    reply.SetString(FID_EXPORTED_TEXT, exported_text);

    if (!bond_quote_msg.empty()) {
	    ptree unrefer_quote_msg;
	    unrefer_quote_msg.put("method", msg->GetType());
	    unrefer_quote_msg.add_child("data", bond_quote_msg);
	    std::ostringstream buf; 
	    write_json(buf, unrefer_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::GvnTknQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);
    BondQuoteBusiness business(this);
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	business.SetAccountId(account_id);
    business.setRequestMsg(msg);

    sdbus::string exported_text; 
	sdbus::VariantList *deal_id_list = reply.AddList(FID_LIST);
	ptree gvntkn_msg_data;
    bool gvn_tkn_success = business.GvnTknQuote(list, &exported_text, deal_id_list, &gvntkn_msg_data);
	reply.SetBool(FID_DCS_RESULT, gvn_tkn_success);

	BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
	for (unsigned int i = 0; i < list.Size(); ++i)
	{
		BondQuotePtr bond_quote(new BondQuote());

		bond_quote->id_ = list.Get(i)->ToString().c_str();

		vec->push_back(bond_quote);
	}
	business.ReferBondQuote(vec);
	reply.SetString(FID_EXPORTED_TEXT, exported_text);

	if (!gvntkn_msg_data.empty()) {
		ptree gvntkn_quote_msg;
		gvntkn_quote_msg.put("method", msg->GetType());
		gvntkn_quote_msg.add_child("data", gvntkn_msg_data);
		std::ostringstream buf;
		write_json(buf, gvntkn_quote_msg);
		//write_xml(buf, add_bond_deal_msg);
		std::string outputText = buf.str();
		MessageResendService::singleton()->PushMsg(outputText);
	}
}

void BondQuoteService::EditReferredQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
    business.setRequestMsg(msg);
	/* END Add by Young 05-Apr-2016 */

	BondQuoteBusiness::BondQuoteVecPtr vec_normal(new std::vector<BondQuotePtr>());
	BondQuoteBusiness::BondQuoteVecPtr vec_delete(new std::vector<BondQuotePtr>());

	sdbus::VariantList list;
	bool export_is_internal = false;
	msg->GetList(FID_LIST, list);
	msg->GetBool(FID_EXPORT_IS_INTERNAL, export_is_internal);
	for (unsigned int i = 0; i < list.Size(); ++i)
	{
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();
		BondQuotePtr bond_quote(new BondQuote());

		GetFieldString(mp, FID_ID, bond_quote->id_);

		GetFieldString(mp, FID_BOND_KEY_LISTED_MARKET, bond_quote->bond_key_listed_market_);

		GetFieldString(mp, FID_INSTITUTION_ID, bond_quote->institution_id_);
		GetFieldString(mp, FID_TRADER_ID, bond_quote->trader_id_);
		GetFieldString(mp, FID_BROKER_ID, bond_quote->broker_id_);

		GetFieldInt(mp, FID_QUOTE_SIDE, bond_quote->quote_side_);
		GetFieldDouble(mp, FID_PRICE, bond_quote->price_);
		GetFieldDouble(mp, FID_VOLUME, bond_quote->volume_);
		GetFieldString(mp, FID_DESCRIPTION, bond_quote->description_);

		GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_quote->fan_dian_flag_);
		GetFieldDouble(mp, FID_FAN_DIAN, bond_quote->fan_dian_);
		GetFieldString(mp, FID_BARGAIN_FLAG, bond_quote->bargain_flag_);
		GetFieldString(mp, FID_OCO_FLAG, bond_quote->oco_flag_);
		GetFieldString(mp, FID_QUOTE_INTERNAL_FLAG, bond_quote->internal_flag_);

		GetFieldString(mp, FID_QUOTE_TYPE, bond_quote->quote_type_);
		GetFieldString(mp, FID_DELIVERY_DATE, bond_quote->delivery_date_);
		GetFieldString(mp, FID_YIELD_TYPE, bond_quote->yield_type_);

		BondQuoteCacheVecPtr quotes = business.GetBondQuoteByConditions(bond_quote->quote_side_.ref(), bond_quote->internal_flag_.ref(), 
			bond_quote->bond_key_listed_market_.ref(), bond_quote->institution_id_.ref(), bond_quote->trader_id_.ref());
		if(quotes->size() > 0) {
			BondQuotePtr delete_quote(new BondQuote());
			std::vector<BondQuoteCachePtr>::const_iterator it = quotes->begin();
            if((*it)->id != bond_quote->id_.ref()) {
			    delete_quote->id_ = (*it)->id;
			    vec_delete->push_back(delete_quote);
            }
		}
		vec_normal->push_back(bond_quote);
	}

	sdbus::string exported_text;
	ptree refer_quote_msg_data;
	business.EditReferredQuote(vec_normal, &exported_text, export_is_internal, &refer_quote_msg_data);

	if(vec_delete->size() > 0) {
		business.DeleteQuote(vec_delete);
	}

	reply.SetString(FID_EXPORTED_TEXT, exported_text);
	
    if (!refer_quote_msg_data.empty()) {
	    ptree update_refer_quote_msg;
	    update_refer_quote_msg.put("method", msg->GetType());
	    update_refer_quote_msg.add_child("data", refer_quote_msg_data);
	    std::ostringstream buf; 
	    write_json(buf, update_refer_quote_msg);
	    //write_xml(buf, add_bond_deal_msg);
	    std::string outputText = buf.str(); 
	    MessageResendService::singleton()->PushMsg(outputText);
    }
}

void BondQuoteService::PublishBondQuote(const std::vector<BondQuoteCachePtr> &bond_quote_vec, const char *action_type, bool without_data/* = true*/, const std::string bond_category/* = "NONE"*/)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    
    
    without_data = without_data && (bond_quote_vec.size() >= 1000);

    for (size_t index = 0; index != bond_quote_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBondQuoteMap(bond_quote_vec.at(index).get(), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(bond_quote_vec.size() > 0)
        companyId = bond_quote_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    PublishManager::singleton()->publish(topic_name_, "BondQuote.QuoteList", msg, without_data);*/
	std::map<std::string, sdbus::Message> tMap;
	std::map<std::string, sdbus::VariantList*> tListMap;
	for(size_t index = 0; index < bond_quote_vec.size(); ++index){
		BondQuoteCache* cache = bond_quote_vec.at(index).get();
		if(cache->company_id.length() < 1){
			continue;
		}
		if(tMap.count(cache->company_id) < 1){
			sdbus::Message msg;
			tMap[cache->company_id] = msg;
			tMap[cache->company_id].SetString(FID_ACTION_TYPE, action_type);
			tMap[cache->company_id].SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
			sdbus::VariantList* list = tMap[cache->company_id].AddList(FID_LIST);
			tListMap[cache->company_id] = list;
		}
		if(tListMap[cache->company_id]->Size() > 1000){
			LOGGER_INFO("too much data. ignore!!!");
			break;
		}
		sdbus::VariantMap *mp = tListMap[cache->company_id]->AddMap();
		SetBondQuoteMap(cache, mp);
	}
	std::map<std::string, sdbus::Message>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message& msg = iter->second;
		sdbus::VariantList* list = tListMap[iter->first];
		bool tFlag = without_data && (list->Size() >= 1000);
		PublishManager::singleton()->publish(topic_name_, "BondQuote.QuoteList", msg, tFlag);

		if ("NONE" != bond_category)
		{
			if ("all" == bond_category)
			{
				std::vector<std::string> bond_categories;
				bond_categories.push_back("BCO");
				bond_categories.push_back("BNC");
				bond_categories.push_back("ABS");
				bond_categories.push_back("NCD");
				BOOST_FOREACH(const std::string& category, bond_categories) {
					PublishManager::singleton()->publish(topic_name_, "BondQuote.QuoteList", msg, tFlag, "SyncClear" + category);
				}
			}
			else
			{
				PublishManager::singleton()->publish(topic_name_, "BondQuote.QuoteList", msg, tFlag, "SyncClear" + bond_category);
			}
		}
	}
}

void BondQuoteService::PublishBondReferQuote(const std::vector<BondQuoteReferCachePtr> &bond_quote_refer_vec, const char *action_type, bool without_data)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    
    
    without_data = without_data && (bond_quote_refer_vec.size() >= 1000);

    for (size_t index = 0; index != bond_quote_refer_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBondReferQuoteMap(bond_quote_refer_vec.at(index).get(), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(bond_quote_refer_vec.size() > 0)
        companyId = bond_quote_refer_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    PublishManager::singleton()->publish(topic_name_, "BondQuote.ReferQuoteList", msg, without_data);*/
	std::map<std::string, sdbus::Message> tMap;
	std::map<std::string, sdbus::VariantList*> tListMap;
	for(size_t index = 0; index < bond_quote_refer_vec.size(); ++index){
		BondQuoteReferCache* cache = bond_quote_refer_vec.at(index).get();
		if(cache->company_id.length() < 1){
			continue;
		}
		if(tMap.count(cache->company_id) < 1){
			sdbus::Message msg;
			tMap[cache->company_id] = msg;
			tMap[cache->company_id].SetString(FID_ACTION_TYPE, action_type);
			tMap[cache->company_id].SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
			sdbus::VariantList* list = tMap[cache->company_id].AddList(FID_LIST);
			tListMap[cache->company_id] = list;
		}
		if(tListMap[cache->company_id]->Size() > 1000){
			LOGGER_INFO("too much data. ignore!!!");
			break;
		}
		sdbus::VariantMap *mp = tListMap[cache->company_id]->AddMap();
		SetBondReferQuoteMap(cache, mp);
	}
	std::map<std::string, sdbus::Message>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message& msg = iter->second;
		sdbus::VariantList* list = tListMap[iter->first];
		bool tFlag = without_data && (list->Size() >= 1000);
		PublishManager::singleton()->publish(topic_name_, "BondQuote.ReferQuoteList", msg, tFlag);
	}
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
	business.SetCompanyId(company_id.c_str());
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
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString = business.CopyReferredQuote(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}

void BondQuoteService::ExportBondQuote(const sdbus::Message* msg, sdbus::Message& reply)
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

    std::string cond, bondtype, date;
    date = GetTDateString("%Y-%m-%d");
    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_BOND_TYPE, bondtype);

    // Get bond quotes based on CONDITION, SORTBY & RANGE
    BondQuoteCacheVecPtr quotes = business.FetchBondQuotesByDate(cond, bondtype, date);
    sdbus::string fileName = cond + date + "Quote.xls";
    
    GenerateExcel * excelEngine = new GenerateExcel(fileName.c_str());
    if(excelEngine != NULL){
        bool saveData = false;

        if (company_id == kCompanyTP 
            || company_id == kCompanyBGC 
            || company_id == kCompanyPATR)
            saveData = excelEngine->SaveBondDataBase(quotes);
        else if(company_id == kCompanyICAP)
            saveData = excelEngine->SaveBondDataICAP(quotes);

        if(saveData)
        {
            std::string ftp_url = Config::singleton()->getValue("Ftp.Url", "ftp://192.168.1.143/");
            reply.SetString(FID_EXPORT_FILE_PATH, ftp_url.c_str() + Utf8ToAnsi(fileName));
            reply.SetString(FID_EXPORT_FILE_NAME, cond + date);
        }

        delete excelEngine;
        excelEngine = NULL;
    }
}

void BondQuoteService::UpdateYiJiBanRate(const sdbus::Message* msg, sdbus::Message& reply)
{
   LOGGER_INFO("UpdateYiJiBanRate");

    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    YiJiBanBondPtr yijiban_bond(new YiJiBanBond());
    msg->GetString(FID_BOND_KEY, yijiban_bond->bond_key_);
    msg->GetString(FID_AVG_PRICE, yijiban_bond->avg_price_);
    msg->GetString(FID_COUPON_RATE, yijiban_bond->coupon_rate_);
    msg->GetString(FID_FAN_DIAN, yijiban_bond->fandian_);
    msg->GetString(FID_DEL_FLAG, yijiban_bond->delflag_);

    business.UpdateYiJiBanRate(yijiban_bond);
}

void BondQuoteService::GetYiJiBanRate(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("GetYiJiBanRate");

    BondQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string bond_key;
    msg->GetString(FID_BOND_KEY, bond_key);
    YiJiBanBondPtr yijiban_bond = business.FetchYiJiBanBond(bond_key);
    if (yijiban_bond) {
        reply.SetString(FID_AVG_PRICE, yijiban_bond->avg_price_);
        reply.SetString(FID_COUPON_RATE, yijiban_bond->coupon_rate_);
        reply.SetString(FID_FAN_DIAN, yijiban_bond->fandian_);
    }
}
