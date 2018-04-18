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
#include "msg2db/msg_log_service.h"
#include "bonddeal_service.h"
#include "business/bonddeal_business.h"
#include "business/oper_log.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "excel/generateexcel.h"
#include "sdbus/codec.h"
#include "service/publish_manager.h"
#include "config.h"
#include "increment/increment_cache_manager.h"
#include "MessageResendService.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options/detail/convert.hpp>
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/locale/encoding.hpp>
#include<boost/property_tree/xml_parser.hpp>
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using boost::property_tree::write_xml;

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
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
        } else if (subject == "increment") {
            GetBondDealListInc(msg, reply);
        }
    } else if (method == "BondDeal.AddBondDeal") {
        AddBondDeal(msg, reply); 
    } else if (method == "BondDeal.UpdateBondDeal") {
        UpdateBondDeal(msg, reply);
	} else if (method == "BondDeal.UpdateDealInfo") {
		UpdateDealInfo(msg, reply);
    } else if (method == "BondDeal.UpdateDealInfoUndo") {
		UpdateDealInfoUndo(msg, reply);
	} else if (method == "BondDeal.DeleteBondDeal") {
        DeleteBondDeal(msg, reply);
    } else if (method == "BondDeal.GetBondDealByID") {
        GetBondDealByID(msg, reply);
    } else if(method =="BondDeal.CopyDeal"){
        CopyDeal(msg, reply);
    } else if(method =="BondDeal.CopyDetailedDeal"){
        CopyDetailedDeal(msg, reply);
    } else if(method == "BondDeal.ExportData") {
        ExportDealData(msg, reply);
	} else if (method == "BondDeal.DealOperLog") {
		OperLog operlog;
		operlog.GetDealOperLog(msg, reply);
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
	mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);

	mp->SetString(FID_DEL_FLAG, cache->status);

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
	business.SetCompanyId(company_id.c_str());
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

    std::vector<BondDealCachePtr>::const_iterator it = deals->begin();
    for ( ; it != deals->end(); ++it) {
        BondDealCache* bond_deal = it->get();
        
        sdbus::VariantMap *mp = list->AddMap();

        SetBondDealMap(bond_deal, mp);
    }

    reply.SetInt32(FID_ROW_COUNT, row_cnt);
}

void BondDealService::GetBondDealListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
    int version; 
    msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
    IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("BondDeal.DealList." + company_id);
    if (cache) {
        cache->Get(version, reply);
	}
}

void BondDealService::AddBondDeal(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Add Bond Deal");
      
	BondDealBusiness business(this);
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

    const sdbus::VariantMap& mp = msg->MapValue();
    BondDealPtr bond_deal(new BondDeal());

	GetFieldString(mp, FID_BOND_KEY_LISTED_MARKET, bond_deal->bond_key_listed_market_);
	GetFieldInt(mp, FID_QUOTE_SIDE, bond_deal->quote_side_);

	GetFieldString(mp, FID_DIRECTION, bond_deal->direction_);
	GetFieldString(mp, FID_CREATE_TIME, bond_deal->create_time_);
	GetFieldString(mp, FID_DEAL_NO, bond_deal->deal_no_);

	GetFieldDouble(mp, FID_PRICE, bond_deal->price_);
	GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_deal->fan_dian_flag_);
	GetFieldDouble(mp, FID_FAN_DIAN, bond_deal->fan_dian_);
	GetFieldString(mp, FID_QUOTE_TYPE, bond_deal->quote_type_);

	GetFieldDouble(mp, FID_VOLUME, bond_deal->volume_);
	GetFieldString(mp, FID_CURRENCY, bond_deal->currency_);

	GetFieldDouble(mp, FID_CLEAN_PRICE, bond_deal->clean_price_);
	GetFieldDouble(mp, FID_DIRTY_PRICE, bond_deal->dirty_price_);
	GetFieldDouble(mp, FID_SPREAD, bond_deal->spread_);
	GetFieldDouble(mp, FID_YIELD, bond_deal->yield_);

	GetFieldString(mp, FID_YIELD_TYPE, bond_deal->yield_type_);
	GetFieldString(mp, FID_SETTLEMENT_TYPE, bond_deal->settlement_type_);

	GetFieldString(mp, FID_DEAL_TIME, bond_deal->deal_time_);
	GetFieldString(mp, FID_DELIVERY_DATE, bond_deal->delivery_date_);
	GetFieldString(mp, FID_SETTLEMENT_DATE, bond_deal->settlement_date_);

	GetFieldString(mp, FID_BID_IS_BRIDGE, bond_deal->bid_is_bridge_);
	GetFieldString(mp, FID_BID_IS_CHARGED, bond_deal->bid_is_charged_);
	GetFieldString(mp, FID_BID_INSTITUTION_ID, bond_deal->bid_institution_id_);
	GetFieldString(mp, FID_BID_INSTITUTION_CITY, bond_deal->bid_inst_city_);
    GetFieldString(mp, FID_BID_TRADER_ID, bond_deal->bid_trader_id_);
	GetFieldString(mp, FID_BID_TRADER_ACCOUNT, bond_deal->bid_trader_account_);
    GetFieldBool(mp, FID_BID_CHECK, bond_deal->bid_checked_);
	GetFieldString(mp, FID_BID_BROKER_ID_A, bond_deal->bid_broker_id_);
	GetFieldString(mp, FID_BID_BROKER_ID_B, bond_deal->bid_broker_id_B_);
	GetFieldString(mp, FID_BID_BROKER_ID_C, bond_deal->bid_broker_id_C_);
	GetFieldString(mp, FID_BID_BROKER_ID_D, bond_deal->bid_broker_id_D_);
	GetFieldString(mp, FID_BID_PERCENT_A, bond_deal->bid_percent_A_);
	GetFieldString(mp, FID_BID_PERCENT_B, bond_deal->bid_percent_B_);
	GetFieldString(mp, FID_BID_PERCENT_C, bond_deal->bid_percent_C_);
	GetFieldString(mp, FID_BID_PERCENT_D, bond_deal->bid_percent_D_);
	GetFieldDouble(mp, FID_BID_BROKERAGE_RATE, bond_deal->bid_brokerage_rate_);	
	GetFieldDouble(mp, FID_BID_DISCOUNT, bond_deal->bid_discount_);

	GetFieldString(mp, FID_OFR_IS_BRIDGE, bond_deal->ofr_is_bridge_);
	GetFieldString(mp, FID_OFR_IS_CHARGED, bond_deal->ofr_is_charged_);
	GetFieldString(mp, FID_OFR_INSTITUTION_ID, bond_deal->ofr_institution_id_);
	GetFieldString(mp, FID_OFR_INSTITUTION_CITY, bond_deal->ofr_inst_city_);
    GetFieldString(mp, FID_OFR_TRADER_ID, bond_deal->ofr_trader_id_);
	GetFieldString(mp, FID_OFR_TRADER_ACCOUNT, bond_deal->ofr_trader_account_);
    GetFieldString(mp, FID_OFR_BROKER_ID_A, bond_deal->ofr_broker_id_);
	GetFieldString(mp, FID_OFR_BROKER_ID_B, bond_deal->ofr_broker_id_B_);
	GetFieldString(mp, FID_OFR_BROKER_ID_C, bond_deal->ofr_broker_id_C_);
	GetFieldString(mp, FID_OFR_BROKER_ID_D, bond_deal->ofr_broker_id_D_);
	GetFieldString(mp, FID_OFR_PERCENT_A, bond_deal->ofr_percent_A_);
	GetFieldString(mp, FID_OFR_PERCENT_B, bond_deal->ofr_percent_B_);
	GetFieldString(mp, FID_OFR_PERCENT_C, bond_deal->ofr_percent_C_);
	GetFieldString(mp, FID_OFR_PERCENT_D, bond_deal->ofr_percent_D_);
    GetFieldBool(mp, FID_OFR_CHECK, bond_deal->ofr_checked_);
	GetFieldDouble(mp, FID_OFR_BROKERAGE_RATE, bond_deal->ofr_brokerage_rate_);
	GetFieldDouble(mp, FID_OFR_DISCOUNT, bond_deal->ofr_discount_);

	GetFieldString(mp, FID_BID_IS_NO_BROKERAGE, bond_deal->bid_is_no_brokerage_);
	GetFieldString(mp, FID_BID_BROKERAGE, bond_deal->bid_brokerage_remark_);
	GetFieldString(mp, FID_BID_TRADE_MODE, bond_deal->bid_trade_mode_);
	GetFieldString(mp, FID_BID_NO_CONFIRM, bond_deal->bid_no_confirm_);
	GetFieldString(mp, FID_BID_NO_COMMENTS, bond_deal->bid_no_comments_);
	GetFieldString(mp, FID_OFR_IS_NO_BROKERAGE, bond_deal->ofr_is_no_brokerage_);
	GetFieldString(mp, FID_OFR_BROKERAGE, bond_deal->ofr_brokerage_remark_);
	GetFieldString(mp, FID_OFR_TRADE_MODE, bond_deal->ofr_trade_mode_);
	GetFieldString(mp, FID_OFR_NO_CONFIRM, bond_deal->ofr_no_confirm_);
	GetFieldString(mp, FID_OFR_NO_COMMENTS, bond_deal->ofr_no_comments_);

	GetFieldString(mp, FID_SETTLEMENT_MODE, bond_deal->settlement_mode_);
	GetFieldString(mp, FID_SETTLEMENT_AMOUNT, bond_deal->settlement_amount_);
	GetFieldString(mp, FID_DESCRIPTION, bond_deal->description_);

	GetFieldString(mp, FID_DEAL_STATUS, bond_deal->deal_status_);
	GetFieldBool(mp, FID_DCS_IN_HAND, bond_deal->in_hand_);
	GetFieldString(mp, FID_URGENT_STATUS, bond_deal->urgent_status_);

	GetFieldString(mp, FID_BID_REMARKS, bond_deal->bid_remarks_);
	GetFieldString(mp, FID_OFR_REMARKS, bond_deal->ofr_remarks_);
	GetFieldString(mp, FID_REMARKS, bond_deal->remarks_);
	GetFieldString(mp, FID_BID_SPECIAL_PARTY, bond_deal->special_party_bid_);
	GetFieldString(mp, FID_OFR_SPECIAL_PARTY, bond_deal->special_party_ofr_);
	GetFieldString(mp, FID_DCS_SPECIAL_TERMS, bond_deal->special_terms_);
	GetFieldString(mp, FID_FEEDBACK, bond_deal->feedback_);

	GetFieldString(mp, FID_DEAL_INTERNAL_FLAG, bond_deal->deal_internal_flag_);

    sdbus::string exported_text;
	sdbus::string bond_id;

	ptree add_bond_deal_msg_data;
    business.AddBondDeal(*bond_deal, &exported_text, &bond_id, &add_bond_deal_msg_data);
	
	sdbus::VariantList *list = reply.AddList(FID_LIST);
	sdbus::Variant* var = list->AddVariant();
	var->SetString(bond_id);

	reply.SetString(FID_EXPORTED_TEXT, exported_text);


	ptree add_bond_deal_msg;
	add_bond_deal_msg.put("method", msg->GetType());
	add_bond_deal_msg.add_child("data", add_bond_deal_msg_data);
	std::ostringstream buf; 
	write_json(buf, add_bond_deal_msg);
	//write_xml(buf, add_bond_deal_msg);
	std::string outputText = buf.str(); 
	MessageResendService::singleton()->PushMsg(outputText);
}

void BondDealService::UpdateBondDeal(const sdbus::Message* msg, sdbus::Message& reply)
{
    LOGGER_INFO("Update Bond Deal");

    BondDealBusiness business(this);
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
    
    const sdbus::VariantMap& mp = msg->MapValue();
    BondDealPtr bond_deal(new BondDeal());

	GetFieldString(mp, FID_ID, bond_deal->id_);
	GetFieldString(mp, FID_BOND_KEY_LISTED_MARKET, bond_deal->bond_key_listed_market_);

	GetFieldString(mp, FID_DIRECTION, bond_deal->direction_);
	GetFieldString(mp, FID_CREATE_TIME, bond_deal->create_time_);
	GetFieldString(mp, FID_DEAL_NO, bond_deal->deal_no_);

	GetFieldDouble(mp, FID_PRICE, bond_deal->price_);
	GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_deal->fan_dian_flag_);
	GetFieldDouble(mp, FID_FAN_DIAN, bond_deal->fan_dian_);
	GetFieldString(mp, FID_QUOTE_TYPE, bond_deal->quote_type_);

	GetFieldDouble(mp, FID_VOLUME, bond_deal->volume_);
	GetFieldString(mp, FID_CURRENCY, bond_deal->currency_);

	GetFieldDouble(mp, FID_CLEAN_PRICE, bond_deal->clean_price_);
	GetFieldDouble(mp, FID_DIRTY_PRICE, bond_deal->dirty_price_);
	GetFieldDouble(mp, FID_SPREAD, bond_deal->spread_);
	GetFieldDouble(mp, FID_YIELD, bond_deal->yield_);

	GetFieldString(mp, FID_YIELD_TYPE, bond_deal->yield_type_);
	GetFieldString(mp, FID_SETTLEMENT_TYPE, bond_deal->settlement_type_);

	GetFieldString(mp, FID_DEAL_TIME, bond_deal->deal_time_);
	GetFieldString(mp, FID_DELIVERY_DATE, bond_deal->delivery_date_);
	GetFieldString(mp, FID_SETTLEMENT_DATE, bond_deal->settlement_date_);

	GetFieldString(mp, FID_BID_IS_BRIDGE, bond_deal->bid_is_bridge_);
	GetFieldString(mp, FID_BID_IS_CHARGED, bond_deal->bid_is_charged_);
	GetFieldString(mp, FID_BID_INSTITUTION_ID, bond_deal->bid_institution_id_);
	GetFieldString(mp, FID_BID_INSTITUTION_CITY, bond_deal->bid_inst_city_);
	GetFieldString(mp, FID_BID_TRADER_ID, bond_deal->bid_trader_id_);
	GetFieldString(mp, FID_BID_TRADER_ACCOUNT, bond_deal->bid_trader_account_);
	GetFieldString(mp, FID_BID_BROKER_ID_A, bond_deal->bid_broker_id_);
	GetFieldString(mp, FID_BID_BROKER_ID_B, bond_deal->bid_broker_id_B_);
	GetFieldString(mp, FID_BID_BROKER_ID_C, bond_deal->bid_broker_id_C_);
	GetFieldString(mp, FID_BID_BROKER_ID_D, bond_deal->bid_broker_id_D_);
	GetFieldString(mp, FID_BID_PERCENT_A, bond_deal->bid_percent_A_);
	GetFieldString(mp, FID_BID_PERCENT_B, bond_deal->bid_percent_B_);
	GetFieldString(mp, FID_BID_PERCENT_C, bond_deal->bid_percent_C_);
	GetFieldString(mp, FID_BID_PERCENT_D, bond_deal->bid_percent_D_);
	GetFieldBool(mp, FID_BID_CHECK, bond_deal->bid_checked_);
	GetFieldDouble(mp, FID_BID_BROKERAGE_RATE, bond_deal->bid_brokerage_rate_);
	GetFieldDouble(mp, FID_BID_DISCOUNT, bond_deal->bid_discount_);

	GetFieldString(mp, FID_OFR_IS_BRIDGE, bond_deal->ofr_is_bridge_);
	GetFieldString(mp, FID_OFR_IS_CHARGED, bond_deal->ofr_is_charged_);
	GetFieldString(mp, FID_OFR_INSTITUTION_ID, bond_deal->ofr_institution_id_);
	GetFieldString(mp, FID_OFR_INSTITUTION_CITY, bond_deal->ofr_inst_city_);
	GetFieldString(mp, FID_OFR_TRADER_ID, bond_deal->ofr_trader_id_);
	GetFieldString(mp, FID_OFR_TRADER_ACCOUNT, bond_deal->ofr_trader_account_);
	GetFieldString(mp, FID_OFR_BROKER_ID_A, bond_deal->ofr_broker_id_);
	GetFieldString(mp, FID_OFR_BROKER_ID_B, bond_deal->ofr_broker_id_B_);
	GetFieldString(mp, FID_OFR_BROKER_ID_C, bond_deal->ofr_broker_id_C_);
	GetFieldString(mp, FID_OFR_BROKER_ID_D, bond_deal->ofr_broker_id_D_);
	GetFieldString(mp, FID_OFR_PERCENT_A, bond_deal->ofr_percent_A_);
	GetFieldString(mp, FID_OFR_PERCENT_B, bond_deal->ofr_percent_B_);
	GetFieldString(mp, FID_OFR_PERCENT_C, bond_deal->ofr_percent_C_);
	GetFieldString(mp, FID_OFR_PERCENT_D, bond_deal->ofr_percent_D_);
	GetFieldBool(mp, FID_OFR_CHECK, bond_deal->ofr_checked_);
	GetFieldDouble(mp, FID_OFR_BROKERAGE_RATE, bond_deal->ofr_brokerage_rate_);
	GetFieldDouble(mp, FID_OFR_DISCOUNT, bond_deal->ofr_discount_);

	GetFieldString(mp, FID_BID_IS_NO_BROKERAGE, bond_deal->bid_is_no_brokerage_);
	GetFieldString(mp, FID_BID_BROKERAGE, bond_deal->bid_brokerage_remark_);
	GetFieldString(mp, FID_BID_TRADE_MODE, bond_deal->bid_trade_mode_);
	GetFieldString(mp, FID_BID_NO_CONFIRM, bond_deal->bid_no_confirm_);
	GetFieldString(mp, FID_BID_NO_COMMENTS, bond_deal->bid_no_comments_);
	GetFieldString(mp, FID_OFR_IS_NO_BROKERAGE, bond_deal->ofr_is_no_brokerage_);
	GetFieldString(mp, FID_OFR_BROKERAGE, bond_deal->ofr_brokerage_remark_);
	GetFieldString(mp, FID_OFR_TRADE_MODE, bond_deal->ofr_trade_mode_);
	GetFieldString(mp, FID_OFR_NO_CONFIRM, bond_deal->ofr_no_confirm_);
	GetFieldString(mp, FID_OFR_NO_COMMENTS, bond_deal->ofr_no_comments_);

	GetFieldString(mp, FID_SETTLEMENT_MODE, bond_deal->settlement_mode_);
	GetFieldString(mp, FID_SETTLEMENT_AMOUNT, bond_deal->settlement_amount_);
	GetFieldString(mp, FID_DESCRIPTION, bond_deal->description_);

	GetFieldString(mp, FID_DEAL_STATUS, bond_deal->deal_status_);
	GetFieldBool(mp, FID_DCS_IN_HAND, bond_deal->in_hand_);
	GetFieldString(mp, FID_URGENT_STATUS, bond_deal->urgent_status_);

	GetFieldString(mp, FID_BID_REMARKS, bond_deal->bid_remarks_);
	GetFieldString(mp, FID_OFR_REMARKS, bond_deal->ofr_remarks_);
	GetFieldString(mp, FID_REMARKS, bond_deal->remarks_);
	GetFieldString(mp, FID_BID_SPECIAL_PARTY, bond_deal->special_party_bid_);
	GetFieldString(mp, FID_OFR_SPECIAL_PARTY, bond_deal->special_party_ofr_);
	GetFieldString(mp, FID_DCS_SPECIAL_TERMS, bond_deal->special_terms_);
	GetFieldString(mp, FID_FEEDBACK, bond_deal->feedback_);

	GetFieldString(mp, FID_DEAL_INTERNAL_FLAG, bond_deal->deal_internal_flag_);

    sdbus::string exported_text;

	ptree update_bond_deal_msg_data;
    business.UpdateBondDeal(*bond_deal, &exported_text, &update_bond_deal_msg_data);

	reply.SetString(FID_EXPORTED_TEXT, exported_text);

	ptree update_bond_deal_msg;
	update_bond_deal_msg.put("method", msg->GetType());
	update_bond_deal_msg.add_child("data", update_bond_deal_msg_data);
	std::ostringstream buf; 
	write_json(buf, update_bond_deal_msg);
	//write_xml(buf, add_bond_deal_msg);
	std::string outputText = buf.str(); 
	MessageResendService::singleton()->PushMsg(outputText);
}

//------------------------------------------------------------------------------------------------------------------------
void BondDealService::UpdateDealInfoUndo(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondDealBusiness business(this);
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

    BondDealVecPtr vec(new std::vector<BondDealPtr>());
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        const sdbus::VariantMap& mp = list.Get(i)->ToMap();
      
        BondDealPtr bond_deal(new BondDeal());

		GetFieldString(mp, FID_ID, bond_deal->id_);
		GetFieldString(mp, FID_BOND_KEY_LISTED_MARKET, bond_deal->bond_key_listed_market_);

		GetFieldString(mp, FID_DIRECTION, bond_deal->direction_);
		GetFieldString(mp, FID_CREATE_TIME, bond_deal->create_time_);
		GetFieldString(mp, FID_DEAL_NO, bond_deal->deal_no_);

		GetFieldDouble(mp, FID_PRICE, bond_deal->price_);
		GetFieldString(mp, FID_FAN_DIAN_FLAG, bond_deal->fan_dian_flag_);
		GetFieldDouble(mp, FID_FAN_DIAN, bond_deal->fan_dian_);
		GetFieldString(mp, FID_QUOTE_TYPE, bond_deal->quote_type_);

		GetFieldDouble(mp, FID_VOLUME, bond_deal->volume_);
		GetFieldString(mp, FID_CURRENCY, bond_deal->currency_);

		GetFieldDouble(mp, FID_CLEAN_PRICE, bond_deal->clean_price_);
		GetFieldDouble(mp, FID_DIRTY_PRICE, bond_deal->dirty_price_);
		GetFieldDouble(mp, FID_SPREAD, bond_deal->spread_);
		GetFieldDouble(mp, FID_YIELD, bond_deal->yield_);

		GetFieldString(mp, FID_YIELD_TYPE, bond_deal->yield_type_);
		GetFieldString(mp, FID_SETTLEMENT_TYPE, bond_deal->settlement_type_);

		GetFieldString(mp, FID_DEAL_TIME, bond_deal->deal_time_);
		GetFieldString(mp, FID_DELIVERY_DATE, bond_deal->delivery_date_);
		GetFieldString(mp, FID_SETTLEMENT_DATE, bond_deal->settlement_date_);

		GetFieldString(mp, FID_BID_IS_BRIDGE, bond_deal->bid_is_bridge_);
		GetFieldString(mp, FID_BID_IS_CHARGED, bond_deal->bid_is_charged_);
		GetFieldString(mp, FID_BID_INSTITUTION_ID, bond_deal->bid_institution_id_);
		GetFieldString(mp, FID_BID_INSTITUTION_CITY, bond_deal->bid_inst_city_);
		GetFieldString(mp, FID_BID_TRADER_ID, bond_deal->bid_trader_id_);
		GetFieldString(mp, FID_BID_BROKER_ID, bond_deal->bid_broker_id_);
		GetFieldBool(mp, FID_BID_CHECK, bond_deal->bid_checked_);

		GetFieldString(mp, FID_OFR_IS_BRIDGE, bond_deal->ofr_is_bridge_);
		GetFieldString(mp, FID_OFR_IS_CHARGED, bond_deal->ofr_is_charged_);
		GetFieldString(mp, FID_OFR_INSTITUTION_ID, bond_deal->ofr_institution_id_);
		GetFieldString(mp, FID_OFR_INSTITUTION_CITY, bond_deal->ofr_inst_city_);
		GetFieldString(mp, FID_OFR_TRADER_ID, bond_deal->ofr_trader_id_);
		GetFieldString(mp, FID_OFR_BROKER_ID, bond_deal->ofr_broker_id_);
		GetFieldBool(mp, FID_OFR_CHECK, bond_deal->ofr_checked_);

		GetFieldString(mp, FID_BID_IS_NO_BROKERAGE, bond_deal->bid_is_no_brokerage_);
		GetFieldString(mp, FID_BID_BROKERAGE, bond_deal->bid_brokerage_remark_);
		GetFieldString(mp, FID_BID_TRADE_MODE, bond_deal->bid_trade_mode_);
		GetFieldString(mp, FID_BID_NO_CONFIRM, bond_deal->bid_no_confirm_);
		GetFieldString(mp, FID_BID_NO_COMMENTS, bond_deal->bid_no_comments_);
		GetFieldString(mp, FID_OFR_IS_NO_BROKERAGE, bond_deal->ofr_is_no_brokerage_);
		GetFieldString(mp, FID_OFR_BROKERAGE, bond_deal->ofr_brokerage_remark_);
		GetFieldString(mp, FID_OFR_TRADE_MODE, bond_deal->ofr_trade_mode_);
		GetFieldString(mp, FID_OFR_NO_CONFIRM, bond_deal->ofr_no_confirm_);
		GetFieldString(mp, FID_OFR_NO_COMMENTS, bond_deal->ofr_no_comments_);

		GetFieldString(mp, FID_SETTLEMENT_MODE, bond_deal->settlement_mode_);
		GetFieldString(mp, FID_SETTLEMENT_AMOUNT, bond_deal->settlement_amount_);
		GetFieldString(mp, FID_DESCRIPTION, bond_deal->description_);

		GetFieldString(mp, FID_DEAL_STATUS, bond_deal->deal_status_);
		GetFieldBool(mp, FID_DCS_IN_HAND, bond_deal->in_hand_);
		GetFieldString(mp, FID_URGENT_STATUS, bond_deal->urgent_status_);

		GetFieldString(mp, FID_BID_REMARKS, bond_deal->bid_remarks_);
		GetFieldString(mp, FID_OFR_REMARKS, bond_deal->ofr_remarks_);
		GetFieldString(mp, FID_REMARKS, bond_deal->remarks_);
		GetFieldString(mp, FID_BID_SPECIAL_PARTY, bond_deal->special_party_bid_);
		GetFieldString(mp, FID_OFR_SPECIAL_PARTY, bond_deal->special_party_ofr_);
		GetFieldString(mp, FID_DCS_SPECIAL_TERMS, bond_deal->special_terms_);
		GetFieldString(mp, FID_FEEDBACK, bond_deal->feedback_);

		GetFieldString(mp, FID_DEAL_INTERNAL_FLAG, bond_deal->deal_internal_flag_);
        vec->push_back(bond_deal);
    }

    sdbus::string exported_text; 
	ptree update_deal_info_msg_data;
	business.UpdateBondDeal(vec, &exported_text, &update_deal_info_msg_data);

    reply.SetString(FID_EXPORTED_TEXT, exported_text);

	ptree update_bond_deal_info_undo_msg;
	update_bond_deal_info_undo_msg.put("method", msg->GetType());
	update_bond_deal_info_undo_msg.add_child("data", update_deal_info_msg_data);
	std::ostringstream buf; 
	write_json(buf, update_bond_deal_info_undo_msg);
	//write_xml(buf, add_bond_deal_msg);
	std::string outputText = buf.str(); 
	MessageResendService::singleton()->PushMsg(outputText);
}

//------------------------------------------------------------------------------------------------------------------------
void BondDealService::UpdateDealInfo(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondDealBusiness business(this);
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

	BondDealVecPtr vec(new std::vector<BondDealPtr>());
	sdbus::VariantList list;
	msg->GetList(FID_LIST, list);

	for (unsigned int i = 0; i < list.Size(); ++i)
	{
		const sdbus::VariantMap& mp = list.Get(i)->ToMap();

		BondDealPtr bond_deal(new BondDeal());

		GetFieldString(mp, FID_ID, bond_deal->id_);

		GetFieldString(mp, FID_DEAL_INTERNAL_FLAG, bond_deal->deal_internal_flag_);
		GetFieldDouble(mp, FID_CLEAN_PRICE, bond_deal->clean_price_);
		GetFieldDouble(mp, FID_DIRTY_PRICE, bond_deal->dirty_price_);
		GetFieldDouble(mp, FID_SPREAD, bond_deal->spread_);
		GetFieldDouble(mp, FID_YIELD, bond_deal->yield_);

		vec->push_back(bond_deal);
	}

	sdbus::string exported_text;    

	ptree update_deal_info_msg_data;
	business.UpdateBondDeal(vec, &exported_text, &update_deal_info_msg_data);

	reply.SetString(FID_EXPORTED_TEXT, exported_text);

	ptree update_bond_deal_info_msg;
	update_bond_deal_info_msg.put("method", msg->GetType());
	update_bond_deal_info_msg.add_child("data", update_deal_info_msg_data);
	std::ostringstream buf; 
	write_json(buf, update_bond_deal_info_msg);
	//write_xml(buf, add_bond_deal_msg);
	std::string outputText = buf.str(); 
	MessageResendService::singleton()->PushMsg(outputText);
}

void BondDealService::ExportDealData(const sdbus::Message* msg, sdbus::Message& reply)
{
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

    std::string cond, date;
    date = GetTDateString("%Y-%m-%d");
    msg->GetString(FID_CONDITIONS, cond);
    BondDealCacheVecPtr deals = business.FetchBondDealsByDate(cond, date);

    sdbus::string fileName = cond + date + "Deal.xls";
    GenerateExcel * excelEngine = new GenerateExcel(fileName.c_str());    
    if(excelEngine != NULL)
    {
        bool saveData = false;

        if (company_id == kCompanyTP) {
            if(isBCOCategoryType(cond)) {
                saveData = excelEngine->SaveBondDealDataTPBCO(deals);
            } else if(cond == "BNC") {
                saveData = excelEngine->SaveBondDealDataTPBNC(deals);
            }
        }
        else if(company_id == kCompanyICAP) {
            saveData = excelEngine->SaveBondDealDataICAP(deals);
        } else if(company_id == kCompanyBGC) {
            saveData = excelEngine->SaveBondDealDataBGC(deals);
        } else if(company_id == kCompanyPATR) {
            saveData = excelEngine->SaveBondDealDataPATR(deals);
        }

        if(saveData)
        {
            std::string ftp_url = Config::singleton()->getValue("Ftp.Url", "ftp://192.168.1.143/");
            reply.SetString(FID_EXPORT_FILE_PATH, ftp_url.c_str() + fileName);  
            reply.SetString(FID_EXPORT_FILE_NAME, cond + date);
        }
        delete excelEngine;
        excelEngine = NULL;
    }
}

void BondDealService::DeleteBondDeal(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondDealBusiness business(this);
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

    BondDealVecPtr vec(new std::vector<BondDealPtr>());
    sdbus::VariantList list;
    msg->GetList(FID_LIST, list);
    for (unsigned int i = 0; i < list.Size(); ++i)
    {
        BondDealPtr bond_deal(new BondDeal());
        bond_deal->id_ = list.Get(i)->ToString().c_str();
        vec->push_back(bond_deal);
    }

	ptree delete_deal_msg_data;
    business.DeleteBondDeal(vec, &delete_deal_msg_data);

	ptree delete_bond_deal_info_msg;
	delete_bond_deal_info_msg.put("method", msg->GetType());
	delete_bond_deal_info_msg.add_child("data", delete_deal_msg_data);
	std::ostringstream buf; 
	write_json(buf, delete_bond_deal_info_msg);
	//write_xml(buf, add_bond_deal_msg);
	std::string outputText = buf.str(); 
	MessageResendService::singleton()->PushMsg(outputText);
}

void BondDealService::PublishBondDeal(const std::vector<BondDealCachePtr> &bond_deal_vec, const char* action_type, bool without_data /* = false */)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    
    
    without_data = without_data && (bond_deal_vec.size() >= 1000);

    for (size_t index = 0; index != bond_deal_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBondDealMap(bond_deal_vec.at(index).get(), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(bond_deal_vec.size() > 0)
        companyId = bond_deal_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    PublishManager::singleton()->publish(topic_name_, "BondDeal.DealList", msg, without_data);*/
	std::map<std::string, sdbus::Message> tMap;
	std::map<std::string, sdbus::VariantList*> tListMap;
	for(size_t index = 0; index < bond_deal_vec.size(); ++index){
		BondDealCache* cache = bond_deal_vec.at(index).get();
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
		SetBondDealMap(cache, mp);
	}
	std::map<std::string, sdbus::Message>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message& msg = iter->second;
		sdbus::VariantList* list = tListMap[iter->first];
		bool tFlag = without_data && (list->Size() >= 1000);
		PublishManager::singleton()->publish(topic_name_, "BondDeal.DealList", msg, tFlag);
	}
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
	business.SetCompanyId(company_id.c_str());
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
	business.SetCompanyId(company_id.c_str());
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
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string exportedString = business.CopyDetailedDeal(list);

    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}

