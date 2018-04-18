/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Deal Business
====================================================================================================================
   Date       Name                                  Description of Change
05-Jun-2014   James Xu                              Initial version
13-Jun-2014   James Xu                              Add AddBondDeal
20-Aug-2014   James Xu                              Add CopyDeal and  CopyDetailedDeal
05-Sep-2014   James Xu                              Generate create_time at server side and deal_time at client side
$HISTORY$
===================================================================================================================*/
#include <sstream>
#include "connection_manager.h"
#include "bonddeal_business.h"
#include "bondquote_business.h"
#include "calculator_business.h"
#include "db/dbconnector.h"
#include "include/common.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/cache_center.h"
#include "include/fid.h"
#include "service/service_manager.h"
#include "service/qb_sync_service.h"
#include "msg2db/msg_log_service.h"
#include <iomanip>

#include "include/text_export.h"
#include <boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;

boost::mutex BondDealBusiness::add_deal_mutex_;

BondDealBusiness::BondDealBusiness(BondDealService* service)
    : service_(service)
{
    Init();
}

void BondDealBusiness::Init()
{
    // 设置FID与数据库表字段名对应关系
    SET_FID_MAP(FID_BOND_SHORT_NAME, bond_short_name)
    SET_FID_MAP(FID_PIN_YIN, pin_yin)
    SET_FID_MAP(FID_PIN_YIN_FULL, pin_yin_full)
    SET_FID_MAP(FID_BOND_CATEGORY, bond_category)
    SET_FID_MAP(FID_BOND_TYPE, bond_type)
    SET_FID_MAP(FID_ENTERPRISE_TYPE, enterprise_type)
    SET_FID_MAP(FID_LISTED_MARKET, listed_market)
    SET_FID_MAP(FID_PLEDGED, pledged)
    SET_FID_MAP(FID_ACROSS_MARKET, across_market)
    SET_FID_MAP(FID_ISSUER_RATING_FILTER, issuer_rating_filter)
    SET_FID_MAP(FID_MARKET_TYPE, market_type)
    SET_FID_MAP(FID_MUNICIPAL, municipal)
    SET_FID_MAP(FID_GUARENTEED, guarenteed)
    SET_FID_MAP(FID_HAS_OPTION, has_option)
    SET_FID_MAP(FID_ON_THE_RUN, on_the_run)
    SET_FID_MAP(FID_RATE_INDEX, rate_index)
    SET_FID_MAP(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market)
    SET_FID_MAP(FID_MATURITY, maturity_date)
	SET_FID_MAP(FID_FILTER_MATURITY, filter_maturity_date)
	SET_FID_MAP(FID_LISTED_DATE, listed_date)
	SET_FID_MAP(FID_REDEMPTION_NO_DESCRIPTION, redemption_no_client)
    SET_FID_MAP(FID_DEAL_INTERNAL_FLAG, internally)
    SET_FID_MAP(FID_DEAL_STATUS, deal_status)
    SET_FID_MAP(FID_BROKER_ID, broker_ids)
	SET_FID_MAP(FID_ISSUE_YEAR, issue_year)
    SET_FID_MAP(FID_MATURITY_HOLIDAYS, maturity_holidays);

    // 全局搜索过滤
    SET_FID_MAP(FID_BID_INSTITUTION_ID, bid_bank_id)
    SET_FID_MAP(FID_OFR_INSTITUTION_ID, ofr_bank_id)
    SET_FID_MAP(FID_BID_TRADER_ID, bid_agent_id)
	SET_FID_MAP(FID_OFR_TRADER_ID, ofr_agent_id)
    SET_FID_MAP(FID_BID_BROKER_ID_A, bid_broker_id)
	SET_FID_MAP(FID_BID_BROKER_ID_B, bidBrokerIdB)
	SET_FID_MAP(FID_BID_BROKER_ID_C, bidBrokerIdC)
	SET_FID_MAP(FID_BID_BROKER_ID_D, bidBrokerIdD)
    SET_FID_MAP(FID_OFR_BROKER_ID_A, ofr_broker_id)
	SET_FID_MAP(FID_OFR_BROKER_ID_B, ofrBrokerIdB)
	SET_FID_MAP(FID_OFR_BROKER_ID_C, ofrBrokerIdC)
	SET_FID_MAP(FID_OFR_BROKER_ID_D, ofrBrokerIdD)
    SET_FID_MAP(FID_ACROSS_MARKET_BOND_CODES, across_market_bond_ids)

    // 排序
    SET_FID_MAP(FID_TIME_TO_MATURITY_REAL, time_to_maturity_real)
    SET_FID_MAP(FID_BOND_CODE, bond_id)
    SET_FID_MAP(FID_PRICE, price)
    SET_FID_MAP(FID_VOLUME, volume)
    SET_FID_MAP(FID_BROKER_NAME, broker_name)
    SET_FID_MAP(FID_ISSUER_RATING_ORDER, issuer_rating_order)
    SET_FID_MAP(FID_ISSUER_RATING_NPY_ORDER, issuer_rating_npy_order)
    SET_FID_MAP(FID_BOND_RATING_ORDER, bond_rating_order)
    SET_FID_MAP(FID_BOND_RATING_NPY_ORDER, bond_rating_npy_order)
    SET_FID_MAP(FID_CREATE_TIME, create_time)
    SET_FID_MAP(FID_DEAL_TIME, deal_time)
    SET_FID_MAP(FID_SETTLEMENT_DATE, settlement_date)
    SET_FID_MAP(FID_CDC_VALUATIONS_YIELD, cdc_valuation_yield)
    SET_FID_MAP(FID_CDC_VALUATIONS_CLEAN_PRICE, cdc_valuation_net_price)
    SET_FID_MAP(FID_BID_INSTITUTION_NAME, bid_institution_name)
    SET_FID_MAP(FID_OFR_INSTITUTION_NAME, ofr_institution_name)
    SET_FID_MAP(FID_BID_TRADER_NAME, bid_trader_name)
    SET_FID_MAP(FID_OFR_TRADER_NAME, ofr_trader_name)
    SET_FID_MAP(FID_OPTION_TYPE, option_client)
}

//------------------------------------------------------------------------------------------------------------------------
BondDealCacheVecPtr BondDealBusiness::FetchBondDeals(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range)
{
    std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, range, "create_time desc");
    dbQuery query(str.c_str());
    BondDealCacheController bondDealCC;
    return bondDealCC.getCacheListByQueryInThreadSafty(query);
}

//------------------------------------------------------------------------------------------------------------------------
BondDealCacheVecPtr BondDealBusiness::GetBondDealByID(const std::string& bond_deal_id)
{
    std::string str = "id = '" + bond_deal_id + "'" + GetCompanyConditionStringWithPreAnd();
    dbQuery query(str.c_str());
    BondDealCacheController bondDealCC;
    return bondDealCC.getCacheListByQueryInThreadSafty(query);
}

//------------------------------------------------------------------------------------------------------------------------
BondDealCacheVecPtr BondDealBusiness::FetchBondDealsByDate(const std::string& cond, const std::string& datetime)
{
    std::string str = datetime + "%";
    std::string qStr = " bond_category = '" + cond + "'" + GetCompanyConditionStringWithPreAnd() + " and create_time like '" + str + "' order by bond_subtype,coupon_type desc";
	dbQuery query(qStr.c_str());
    BondDealCacheController bondDealCC;
    return bondDealCC.getCacheListByQueryInThreadSafty(query);
}

//------------------------------------------------------------------------------------------------------------------------
int BondDealBusiness::GetBondDealCount(const sdbus::string& cond)
{
    std::string str = GetFastdbQueryWithCompanyCondition(cond, "", "");
    dbQuery query(str.c_str());

    BondDealCacheController bondDealCC;
    return bondDealCC.getSizeByQueryInThreadSafty(query);
}

//------------------------------------------------------------------------------------------------------------------------
int BondDealBusiness::GetDealSequenceNumber(const BondDealCachePtr& cache)
{
	std::string day = cache->create_time.substr(0, 10);
	if (0 == day.length()) day = GetTDateString("%Y-%m-%d");

	std::string low = day + " 00:00:00";
	std::string high = day + " 23:59:59";
    std::string qStr = " create_time >= '" + low + "' and create_time <= '" + high + "' and sequence_number > 0 " \
		+ " and bond_category = '" + cache->bond_category + "'" + GetCompanyConditionStringWithPreAnd() + "order by sequence_number desc";
    dbQuery query(qStr.c_str());

    LOGGER_INFO("get deal sequence number start...")	
    BondDealCacheController bondDealCC;
    BondDealCachePtr deal_cache = bondDealCC.getCacheByQueryInThreadSafty(query);
    LOGGER_INFO("get deal sequence number end...")	
    if (deal_cache) {
        return deal_cache->sequence_number;
    }    

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------
bool BondDealBusiness::AddDeal(const BondDeal& bond_deal, 
    sdbus::string* exported_text /* = NULL */, 
    sdbus::string* bond_id /* = NULL */, 
    ptree* bond_deal_msg /* = NULL */, 
    bool manually_set_deal_id /*= false*/,
    bool ptree_is_not_list /*= false*/)
{
	boost::mutex::scoped_lock lock(add_deal_mutex_);

    std::vector<BondDealCachePtr> bond_deal_vec;
    BondDealCachePtr bonddealcache(new BondDealCache());
   
	if (manually_set_deal_id) {
		bonddealcache->id = bond_deal.id_;
	}
	else {
		bonddealcache->id = GetNewGuid();
	}

    bonddealcache->company_id = GetCompanyId();

	SETIFASSIGNED(bonddealcache->bond_key_listed_market, bond_deal.bond_key_listed_market_);

	SETIFASSIGNED(bonddealcache->deal_type, bond_deal.direction_);

	// create time 由客户端生成
	SETIFASSIGNED(bonddealcache->create_time, bond_deal.create_time_);
	bonddealcache->create_time_milli_sec = ParseTimeString(bonddealcache->create_time) * 1000;
	bonddealcache->updateTime = bonddealcache->create_time;

	SETIFASSIGNED(bonddealcache->odd_numbers, bond_deal.deal_no_);

	SETIFASSIGNED(bonddealcache->price, bond_deal.price_);
	SETIFASSIGNED(bonddealcache->return_point, bond_deal.fan_dian_);
	SETIFASSIGNED(bonddealcache->rebate, bond_deal.fan_dian_flag_);
	SETIFASSIGNED(bonddealcache->quote_type, bond_deal.quote_type_);
	SETIFASSIGNED(bonddealcache->volume, bond_deal.volume_);

	SETIFASSIGNED(bonddealcache->currency, bond_deal.currency_);
	SETIFASSIGNED(bonddealcache->exercise, bond_deal.yield_type_);
	SETIFASSIGNED(bonddealcache->time_style, bond_deal.settlement_type_);

	SETIFASSIGNED(bonddealcache->deal_time, bond_deal.deal_time_);
	if (bond_deal.deal_time_.assigned())
		bonddealcache->deal_time += " 00:00:00";
	bonddealcache->deal_time_milli_sec = ParseTimeString(bonddealcache->deal_time) * 1000;
	SETIFASSIGNED(bonddealcache->delivery_date, bond_deal.delivery_date_);
	SETIFASSIGNED(bonddealcache->settlement_date, bond_deal.settlement_date_);

	SETIFASSIGNED(bonddealcache->bid_bridge, bond_deal.bid_is_bridge_);
	SETIFASSIGNED(bonddealcache->bid_charge, bond_deal.bid_is_charged_);
	SETIFASSIGNED(bonddealcache->bid_bank_id, bond_deal.bid_institution_id_);
	SETIFASSIGNED(bonddealcache->bid_bank_city, bond_deal.bid_inst_city_);
    SETIFASSIGNED(bonddealcache->bid_agent_id, bond_deal.bid_trader_id_);
	SETIFASSIGNED(bonddealcache->bid_trader_account, bond_deal.bid_trader_account_);
    SETIFASSIGNED(bonddealcache->bid_broker_id, bond_deal.bid_broker_id_);
	SETIFASSIGNED(bonddealcache->bidBrokerIdB, bond_deal.bid_broker_id_B_);
	SETIFASSIGNED(bonddealcache->bidBrokerIdC, bond_deal.bid_broker_id_C_);
	SETIFASSIGNED(bonddealcache->bidBrokerIdD, bond_deal.bid_broker_id_D_);
	SETIFASSIGNED(bonddealcache->bidPercentA, bond_deal.bid_percent_A_);
	SETIFASSIGNED(bonddealcache->bidPercentB, bond_deal.bid_percent_B_);
	SETIFASSIGNED(bonddealcache->bidPercentC, bond_deal.bid_percent_C_);
	SETIFASSIGNED(bonddealcache->bidPercentD, bond_deal.bid_percent_D_);
	bonddealcache->bid_brokerrage = DOUBLE_NULL;
	SETIFASSIGNED(bonddealcache->bid_check, bond_deal.bid_checked_);
	if (bond_deal.bid_checked_) {
		bonddealcache->bid_check_date = GetTDateString("%Y-%m-%d");
	}
	else {
		bonddealcache->bid_check_date = "";
	}
	SETIFASSIGNED(bonddealcache->bid_brokerage_rate, bond_deal.bid_brokerage_rate_);
	SETIFASSIGNED(bonddealcache->bid_discount, bond_deal.bid_discount_);

	SETIFASSIGNED(bonddealcache->ofr_bridge, bond_deal.ofr_is_bridge_);
	SETIFASSIGNED(bonddealcache->ofr_charge, bond_deal.ofr_is_charged_);
	SETIFASSIGNED(bonddealcache->ofr_bank_id, bond_deal.ofr_institution_id_);
	SETIFASSIGNED(bonddealcache->ofr_bank_city, bond_deal.ofr_inst_city_);
    SETIFASSIGNED(bonddealcache->ofr_agent_id, bond_deal.ofr_trader_id_);
	SETIFASSIGNED(bonddealcache->ofr_trader_account, bond_deal.ofr_trader_account_);
    SETIFASSIGNED(bonddealcache->ofr_broker_id, bond_deal.ofr_broker_id_);
	SETIFASSIGNED(bonddealcache->ofrBrokerIdB, bond_deal.ofr_broker_id_B_);
	SETIFASSIGNED(bonddealcache->ofrBrokerIdC, bond_deal.ofr_broker_id_C_);
	SETIFASSIGNED(bonddealcache->ofrBrokerIdD, bond_deal.ofr_broker_id_D_);
	SETIFASSIGNED(bonddealcache->ofrPercentA, bond_deal.ofr_percent_A_);
	SETIFASSIGNED(bonddealcache->ofrPercentB, bond_deal.ofr_percent_B_);
	SETIFASSIGNED(bonddealcache->ofrPercentC, bond_deal.ofr_percent_C_);
	SETIFASSIGNED(bonddealcache->ofrPercentD, bond_deal.ofr_percent_D_);
	bonddealcache->ofr_brokerrage = DOUBLE_NULL;
	SETIFASSIGNED(bonddealcache->ofr_check, bond_deal.ofr_checked_);
	if (bond_deal.ofr_checked_) {
		bonddealcache->ofr_check_date = GetTDateString("%Y-%m-%d");
	}
	else {
		bonddealcache->ofr_check_date = "";
	}
	SETIFASSIGNED(bonddealcache->ofr_brokerage_rate, bond_deal.ofr_brokerage_rate_);
	SETIFASSIGNED(bonddealcache->ofr_discount, bond_deal.ofr_discount_);

	SETIFASSIGNED(bonddealcache->bidIsNoBrokerage, bond_deal.bid_is_no_brokerage_);
	SETIFASSIGNED(bonddealcache->bidBrokerrageRemark, bond_deal.bid_brokerage_remark_);
	SETIFASSIGNED(bonddealcache->bidTradeMode, bond_deal.bid_trade_mode_);
	SETIFASSIGNED(bonddealcache->bidNoConfirm, bond_deal.bid_no_confirm_);
	SETIFASSIGNED(bonddealcache->bidNoComments, bond_deal.bid_no_comments_);
	SETIFASSIGNED(bonddealcache->ofrIsNoBrokerage, bond_deal.ofr_is_no_brokerage_);
	SETIFASSIGNED(bonddealcache->ofrBrokerrageRemark, bond_deal.ofr_brokerage_remark_);
	SETIFASSIGNED(bonddealcache->ofrTradeMode, bond_deal.ofr_trade_mode_);
	SETIFASSIGNED(bonddealcache->ofrNoConfirm, bond_deal.ofr_no_confirm_);
	SETIFASSIGNED(bonddealcache->ofrNoComments, bond_deal.ofr_no_comments_);

	SETIFASSIGNED(bonddealcache->bid_remarks, bond_deal.bid_remarks_);
	SETIFASSIGNED(bonddealcache->ofr_remarks, bond_deal.ofr_remarks_);
	SETIFASSIGNED(bonddealcache->remarks, bond_deal.remarks_);
	SETIFASSIGNED(bonddealcache->specialPartyBid, bond_deal.special_party_bid_);
	SETIFASSIGNED(bonddealcache->specialPartyOfr, bond_deal.special_party_ofr_);
	SETIFASSIGNED(bonddealcache->specialTerms, bond_deal.special_terms_);
	SETIFASSIGNED(bonddealcache->feedback, bond_deal.feedback_);

	SETIFASSIGNED(bonddealcache->deal_status, bond_deal.deal_status_);
	SETIFASSIGNED(bonddealcache->inHand, bond_deal.in_hand_);
	SETIFASSIGNED(bonddealcache->urgent, bond_deal.urgent_status_);

	SETIFASSIGNED(bonddealcache->delivery_type, bond_deal.settlement_mode_);
	SETIFASSIGNED(bonddealcache->total_price, bond_deal.settlement_amount_);
	SETIFASSIGNED(bonddealcache->description, bond_deal.description_);

	SETIFASSIGNED(bonddealcache->internally, bond_deal.deal_internal_flag_);
    
    LOGGER_INFO("[id]: " << bonddealcache->id)
    LOGGER_INFO("[bond_key_listed_market]: " << bonddealcache->bond_key_listed_market)
    LOGGER_INFO("[price]: " << bonddealcache->price)

    CalculatorBusiness calc_business;
    calc_business.Calc(bonddealcache.get());

    // 成交表单里可手动修改保存 clean_price, dirty_price, spread, yield
    if (bond_deal.clean_price_.assigned() && bond_deal.clean_price_ != DOUBLE_NULL) {
        bonddealcache->net_price = bond_deal.clean_price_;
    }
    if (bond_deal.dirty_price_.assigned() && bond_deal.dirty_price_ != DOUBLE_NULL) {
        bonddealcache->full_price = bond_deal.dirty_price_;
    }
    if (bond_deal.spread_.assigned() && bond_deal.spread_ != DOUBLE_NULL) {
        bonddealcache->spread = bond_deal.spread_;
    }
    if (bond_deal.yield_.assigned() && bond_deal.yield_ != DOUBLE_NULL) {
        bonddealcache->yield = bond_deal.yield_;
    }

    CacheCenter::sharedInstance()->AttachCurrentThread();
    bonddealcache->loadCombinationFields();
    CacheCenter::sharedInstance()->DetachCurrentThread();

	//序列号信用、利率分开处理
	int sequence_number = GetDealSequenceNumber(bonddealcache) + 1;
	bonddealcache->sequence_number = sequence_number;

    BondDealCacheController bonddeal_cc;
    bool succeed = bonddeal_cc.insertCacheInThreadSafty(bonddealcache.get());
    
    LOGGER_DEBUG("Insert Deal:"<< succeed)

    // Publish
    if (succeed) {
        bond_deal_vec.push_back(bonddealcache);        
    }

    if (bond_deal_vec.size() > 0) {
        service_->PublishBondDeal(bond_deal_vec, "AddList");

        QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
        if (qb_sync_service) {
            qb_sync_service->PublishBondDeal(bond_deal_vec, FID_PUSH_MESSAGE_METHOD_ADD);
        }
    }

    if (exported_text != NULL)
    {
        sdbus::VariantList deal_ids;
        deal_ids.PushBack(bonddealcache->id.c_str());
        *exported_text = this->CopyDeal(deal_ids);
    }

	if (bond_id != NULL)
	{
		*bond_id = bonddealcache->id;
	}
	
	if (bond_deal_msg != NULL) {
        ptree each_bond_deal_msg;
		this->AssembleBondDeal(bonddealcache, &each_bond_deal_msg);
        if (ptree_is_not_list) {
            *bond_deal_msg = each_bond_deal_msg;
        }
        else {  
		    bond_deal_msg->push_back(std::make_pair("", each_bond_deal_msg));
        }
	}
    MsgLogService::singleton()->writeDealLog(getRequestMsg(), bond_deal_vec);
	return succeed;
}

//------------------------------------------------------------------------------------------------------------------------
bool BondDealBusiness::AddBondDeal(const BondDeal& bond_deal, 
    sdbus::string* exported_text /* = NULL */, 
    sdbus::string* bond_id /* = NULL */, 
    ptree* bond_deal_msg /* = NULL */, 
    bool manually_set_deal_id /*= false*/,
    bool ptree_is_not_list /*= false*/)
{
    LOGGER_DEBUG("AddBondDeal: " << bond_deal.bond_key_listed_market_.ref())

    BondQuoteCacheController bond_quote_cc;
    dbQuery q;

    std::string bidStr = " bond_key_listed_market = '" + bond_deal.bond_key_listed_market_.ref() + "' and bank_agent_id = '" + bond_deal.bid_trader_id_.ref() + "' and symbol = 1" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    q = bidStr.c_str();
    BondQuoteCacheVecPtr bid_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    std::string ofrStr = " bond_key_listed_market = '" + bond_deal.bond_key_listed_market_.ref() + "' and bank_agent_id = '" + bond_deal.ofr_trader_id_.ref() + "' and symbol = -1" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    q = ofrStr.c_str();
    BondQuoteCacheVecPtr ofr_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    BondQuoteService *bondQuoteService = (BondQuoteService*)ServiceManager::singleton()->getService("BondQuote");
    BondQuoteBusiness bondquote_business(bondQuoteService);
	bondquote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
    bondquote_business.setRequestMsg(getRequestMsg());
    if (bid_quote_cache_vec != NULL)
    {
        std::vector<BondQuoteCachePtr>::const_iterator it = bid_quote_cache_vec->begin();
        for ( ; it != bid_quote_cache_vec->end(); ++it) 
        {
            BondQuoteCache* bid_quote_cache = it->get();

            BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
            BondQuotePtr bond_quote(new BondQuote());
            bond_quote->id_ = bid_quote_cache->id;

            if (bond_deal.volume_ < bid_quote_cache->volume) 
            {    
                bond_quote->volume_ = bid_quote_cache->volume - bond_deal.volume_;       
                vec->push_back(bond_quote);
                bondquote_business.UpdateQuote(vec);                  
            } 
            else
            {           
                vec->push_back(bond_quote);
                bondquote_business.ReferBondQuote(vec);                                  
            } 
        }
    } 

    if (ofr_quote_cache_vec != NULL)
    {
        std::vector<BondQuoteCachePtr>::const_iterator it = ofr_quote_cache_vec->begin();
        for ( ; it != ofr_quote_cache_vec->end(); ++it) 
        {
            BondQuoteCache* ofr_quote_cache = it->get();

            BondQuoteBusiness::BondQuoteVecPtr vec(new std::vector<BondQuotePtr>());
            BondQuotePtr bond_quote(new BondQuote());
            bond_quote->id_ = ofr_quote_cache->id;

            if (bond_deal.volume_ < ofr_quote_cache->volume) 
            {    
                bond_quote->volume_ = ofr_quote_cache->volume - bond_deal.volume_;       
                vec->push_back(bond_quote);
                bondquote_business.UpdateQuote(vec);                  
            } 
            else
            {           
                vec->push_back(bond_quote);
                bondquote_business.ReferBondQuote(vec);                                  
            } 
        }
    } 

    AddDeal(bond_deal, exported_text, bond_id, bond_deal_msg, manually_set_deal_id, ptree_is_not_list);

    return true;
}

void BondDealBusiness::AssembleBondDeal(BondDealCachePtr& bonddealcache, ptree* bond_deal_msg) {
	if (bond_deal_msg != NULL) {
        bond_deal_msg->put("id", bonddealcache->id);
        bond_deal_msg->put("company_id", bonddealcache->company_id);
		bond_deal_msg->put("dealTime", bonddealcache->deal_time);
		bond_deal_msg->put("bidBankId", bonddealcache->bid_bank_id);
		bond_deal_msg->put("bidBankCode", bonddealcache->bid_bank_code);
		bond_deal_msg->put("bidBankCity", bonddealcache->bid_bank_city);
		bond_deal_msg->put("bidAgentId", bonddealcache->bid_agent_id);
		bond_deal_msg->put("bidAgentCode", bonddealcache->bid_agent_code);
		bond_deal_msg->put("bidBrokerId", bonddealcache->bid_broker_id);
		bond_deal_msg->put("bidBrokerName", bonddealcache->bid_broker_name);
		bond_deal_msg->put("bidBrokerrage", bonddealcache->bid_brokerrage);
		bond_deal_msg->put("bidCheck", bonddealcache->bid_check);
		bond_deal_msg->put("ofrBankId", bonddealcache->ofr_bank_id);
		bond_deal_msg->put("ofrBankCode", bonddealcache->ofr_bank_code);
		bond_deal_msg->put("ofrBankCity", bonddealcache->ofr_bank_city);
		bond_deal_msg->put("ofrAgentId", bonddealcache->ofr_agent_id);
		bond_deal_msg->put("ofrAgentCode", bonddealcache->ofr_agent_code);
		bond_deal_msg->put("ofrBrokerId", bonddealcache->ofr_broker_id);
		bond_deal_msg->put("ofrBrokerName", bonddealcache->ofr_broker_name);
		bond_deal_msg->put("ofrBrokerrage", bonddealcache->ofr_brokerrage);
		bond_deal_msg->put("ofrCheck", bonddealcache->ofr_check);
        if (bonddealcache->price != DOUBLE_NULL) {
			bond_deal_msg->put("price", Format(bonddealcache->price, 2, 4));
		}
        if (bonddealcache->volume != DOUBLE_NULL) {
            bond_deal_msg->put("volume", Format(bonddealcache->volume, 0, 4));
        }
		bond_deal_msg->put("currency", bonddealcache->currency);
		if (bonddealcache->full_price != DOUBLE_NULL) {
			bond_deal_msg->put("fullPrice", Format(bonddealcache->full_price, 2, 4));
		}
		if (bonddealcache->net_price != DOUBLE_NULL) {
			bond_deal_msg->put("netPrice", Format(bonddealcache->net_price, 2, 4));
		}
		bond_deal_msg->put("settlementDate", bonddealcache->settlement_date);
		bond_deal_msg->put("deliveryType", bonddealcache->delivery_type);
		bond_deal_msg->put("goodsId", bonddealcache->goods_id);
		bond_deal_msg->put("bondKey", bonddealcache->bond_key);
		bond_deal_msg->put("listedMarket", bonddealcache->listed_market);
		bond_deal_msg->put("bondKeyListedMarket", bonddealcache->bond_key_listed_market);
		bond_deal_msg->put("goodsCode", bonddealcache->goods_code);
		bond_deal_msg->put("goodsShortName", bonddealcache->goods_short_name);
		bond_deal_msg->put("maturityDate", bonddealcache->maturity_date);
		bond_deal_msg->put("description", bonddealcache->description);
		bond_deal_msg->put("dealStatus", bonddealcache->deal_status);
		bond_deal_msg->put("status", bonddealcache->status);
		bond_deal_msg->put("createTime", bonddealcache->create_time);
		bond_deal_msg->put("bidBridge", bonddealcache->bid_bridge);
		bond_deal_msg->put("bidCharge", bonddealcache->bid_charge);
		bond_deal_msg->put("ofrBridge", bonddealcache->ofr_bridge);
		bond_deal_msg->put("dealType", bonddealcache->deal_type);
		bond_deal_msg->put("ofrCharge", bonddealcache->ofr_charge);
		bond_deal_msg->put("groupFlag", bonddealcache->group_flag);
		bond_deal_msg->put("timeStyle", bonddealcache->time_style);
		bond_deal_msg->put("confirmAccount", bonddealcache->confirm_account);
		bond_deal_msg->put("confirmDate", bonddealcache->confirm_date);
		bond_deal_msg->put("bidCheckDate", bonddealcache->bid_check_date);
		bond_deal_msg->put("ofrCheckDate", bonddealcache->ofr_check_date);
		bond_deal_msg->put("bidTraderCheck", bonddealcache->bid_trader_check);
		bond_deal_msg->put("bidTraderCheckDate", bonddealcache->bid_trader_check_date);
		bond_deal_msg->put("ofrTraderCheck", bonddealcache->ofr_trader_check);
		bond_deal_msg->put("ofrTraderCheckDate", bonddealcache->ofr_trader_check_date);
		bond_deal_msg->put("remarks", bonddealcache->remarks);
		bond_deal_msg->put("oddNumbers", bonddealcache->odd_numbers);
		bond_deal_msg->put("isPrint", bonddealcache->is_print);
		bond_deal_msg->put("exercise", bonddealcache->exercise);
		bond_deal_msg->put("internally", bonddealcache->internally);

        if (bonddealcache->return_point != DOUBLE_NULL) {
			bond_deal_msg->put("returnPoint", bonddealcache->return_point);
		}
        if (bonddealcache->yield != DOUBLE_NULL) {
			bond_deal_msg->put("yield", bonddealcache->yield);
		}
        if (bonddealcache->spread != DOUBLE_NULL) {
			bond_deal_msg->put("spread", bonddealcache->spread);
		}
		bond_deal_msg->put("clearSpeed", bonddealcache->clear_speed);
		bond_deal_msg->put("deliveryDate", bonddealcache->delivery_date);
		bond_deal_msg->put("rebate", bonddealcache->rebate);
		bond_deal_msg->put("quoteType", bonddealcache->quote_type);
		bond_deal_msg->put("tradedDate", bonddealcache->traded_date);
		bond_deal_msg->put("sequenceNumber", bonddealcache->sequence_number);
		bond_deal_msg->put("totalPrice", bonddealcache->total_price);
	}
}

//------------------------------------------------------------------------------------------------------------------------
bool BondDealBusiness::UpdateBondDeal(const BondDeal& bond_deal, sdbus::string* exported_text /* = NULL */, ptree* bond_deal_msg /* = NULL */)
{
    BondDealCacheController bonddeal_cc;
    std::vector<BondDealCachePtr> bond_deal_vec;

    std::string str = "id = '" + bond_deal.id_.ref() + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondDealCachePtr cache = bonddeal_cc.getCacheByQueryInThreadSafty(q);
    if (cache) {
        UpdateBondDealCallBack(cache.get(), (void*)(&bond_deal));       
        CacheCenter::sharedInstance()->AttachCurrentThread();
        cache->loadCombinationFields();    
        CacheCenter::sharedInstance()->DetachCurrentThread();
        bonddeal_cc.updateCacheInThreadSafty(q, cache.get());

        bond_deal_vec.push_back(cache);
    }

    // Publish
    if (bond_deal_vec.size() > 0) {
        service_->PublishBondDeal(bond_deal_vec, "UpdateList");

        QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
        if (qb_sync_service) {
            qb_sync_service->PublishBondDeal(bond_deal_vec, FID_PUSH_MESSAGE_METHOD_UPDATE);
        }
    }

    if (exported_text != NULL)
    {
        sdbus::VariantList deal_ids;
        deal_ids.PushBack(bond_deal.id_.ref().c_str());
        *exported_text = this->CopyDeal(deal_ids);
    }

    if (cache && bond_deal_msg != NULL) {
        ptree each_bond_deal_msg;
        this->AssembleBondDeal(cache, &each_bond_deal_msg);
        bond_deal_msg->push_back(std::make_pair("", each_bond_deal_msg));
    }
    MsgLogService::singleton()->writeDealLog(getRequestMsg(), bond_deal_vec);
    return true;
}

//------------------------------------------------------------------------------------------------------------------------
bool BondDealBusiness::UpdateBondDeal(const BondDealVecPtr& vec, sdbus::string* exported_text /* = NULL */, ptree* bond_deal_info_msg)
{
    BondDealCacheController bonddeal_cc;
    std::vector<BondDealCachePtr> bond_deal_vec;

    for(int i = 0; i < vec->size(); ++i)
    {
        BondDealPtr bond_deal = vec->at(i);
        std::string str = "id = '" + bond_deal->id_.ref() + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());

        BondDealCachePtr cache = bonddeal_cc.getCacheByQueryInThreadSafty(q);
        if (cache) {
            UpdateBondDealCallBack(cache.get(), (void*)(bond_deal.get()));       
            CacheCenter::sharedInstance()->AttachCurrentThread();
            cache->loadCombinationFields();    
            CacheCenter::sharedInstance()->DetachCurrentThread();
            bonddeal_cc.updateCacheInThreadSafty(q, cache.get());

            bond_deal_vec.push_back(cache);
			ptree each_bond_deal_info_msg;
			this->AssembleBondDeal(cache, &each_bond_deal_info_msg);
			bond_deal_info_msg->push_back(std::make_pair("", each_bond_deal_info_msg));
        }
    }


    // Publish
    if (bond_deal_vec.size() > 0)
    {
        service_->PublishBondDeal(bond_deal_vec, "UpdateList");

        QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
        if (qb_sync_service) {
            qb_sync_service->PublishBondDeal(bond_deal_vec, FID_PUSH_MESSAGE_METHOD_UPDATE);
        }
    }

    if (exported_text != NULL)
    {
        sdbus::VariantList deal_ids;
        for(int i = 0; i < vec->size(); ++i)
        {
            BondDealPtr bond_deal = vec->at(i);
            deal_ids.PushBack(bond_deal->id_.ref().c_str());
        }
        
        *exported_text = this->CopyDeal(deal_ids);
    }
    MsgLogService::singleton()->writeDealLog(getRequestMsg(), bond_deal_vec);
    return true;
}
void BondDealBusiness::UpdateBondDealCallBack(BondDealCache* cache, void* param) {
    if (param == NULL) {
        return;
    }

    BondDeal* bonddeal = (BondDeal*)param;

	SETIFASSIGNED(cache->deal_type, bonddeal->direction_);
	SETIFASSIGNED(cache->odd_numbers, bonddeal->deal_no_);
	SETIFASSIGNED(cache->create_time, bonddeal->create_time_);
	cache->create_time_milli_sec = ParseTimeString(cache->create_time) * 1000;

	SETIFASSIGNED(cache->price, bonddeal->price_);
	SETIFASSIGNED(cache->rebate, bonddeal->fan_dian_flag_);
	SETIFASSIGNED(cache->return_point, bonddeal->fan_dian_);
	SETIFASSIGNED(cache->quote_type, bonddeal->quote_type_);
	SETIFASSIGNED(cache->volume, bonddeal->volume_);
	SETIFASSIGNED(cache->currency, bonddeal->currency_);

	SETIFASSIGNED(cache->exercise, bonddeal->yield_type_);
	SETIFASSIGNED(cache->time_style, bonddeal->settlement_type_);
	SETIFASSIGNED(cache->total_price, bonddeal->settlement_amount_);
	SETIFASSIGNED(cache->deal_time, bonddeal->deal_time_);
	if (bonddeal->deal_time_.assigned())
		cache->deal_time += " 00:00:00";
	cache->deal_time_milli_sec = ParseTimeString(cache->deal_time) * 1000;
	SETIFASSIGNED(cache->settlement_date, bonddeal->settlement_date_);
	SETIFASSIGNED(cache->delivery_date, bonddeal->delivery_date_);

	SETIFASSIGNED(cache->bid_bridge, bonddeal->bid_is_bridge_);
	SETIFASSIGNED(cache->bid_charge, bonddeal->bid_is_charged_);
	SETIFASSIGNED(cache->bid_bank_id, bonddeal->bid_institution_id_);
	SETIFASSIGNED(cache->bid_bank_city, bonddeal->bid_inst_city_);
    SETIFASSIGNED(cache->bid_agent_id, bonddeal->bid_trader_id_);
	SETIFASSIGNED(cache->bid_trader_account, bonddeal->bid_trader_account_);
	cache->bid_brokerrage = DOUBLE_NULL;
    SETIFASSIGNED(cache->bid_broker_id, bonddeal->bid_broker_id_);
	SETIFASSIGNED(cache->bidBrokerIdB, bonddeal->bid_broker_id_B_);
	SETIFASSIGNED(cache->bidBrokerIdC, bonddeal->bid_broker_id_C_);
	SETIFASSIGNED(cache->bidBrokerIdD, bonddeal->bid_broker_id_D_);
	SETIFASSIGNED(cache->bidPercentA, bonddeal->bid_percent_A_);
	SETIFASSIGNED(cache->bidPercentB, bonddeal->bid_percent_B_);
	SETIFASSIGNED(cache->bidPercentC, bonddeal->bid_percent_C_);
	SETIFASSIGNED(cache->bidPercentD, bonddeal->bid_percent_D_);
    SETIFASSIGNED(cache->bid_check, bonddeal->bid_checked_);
    if (bonddeal->bid_checked_) {
        cache->bid_check_date = GetTDateString("%Y-%m-%d");
    } else {
        cache->bid_check_date = "";
    }
	SETIFASSIGNED(cache->bid_brokerage_rate, bonddeal->bid_brokerage_rate_);
	SETIFASSIGNED(cache->bid_discount, bonddeal->bid_discount_);


	SETIFASSIGNED(cache->ofr_bridge, bonddeal->ofr_is_bridge_);
	SETIFASSIGNED(cache->ofr_charge, bonddeal->ofr_is_charged_);
	SETIFASSIGNED(cache->ofr_bank_id, bonddeal->ofr_institution_id_);
	SETIFASSIGNED(cache->ofr_bank_city, bonddeal->ofr_inst_city_);
    SETIFASSIGNED(cache->ofr_agent_id, bonddeal->ofr_trader_id_);
	SETIFASSIGNED(cache->ofr_trader_account, bonddeal->ofr_trader_account_);
	cache->ofr_brokerrage = DOUBLE_NULL;
    SETIFASSIGNED(cache->ofr_broker_id, bonddeal->ofr_broker_id_);
	SETIFASSIGNED(cache->ofrBrokerIdB, bonddeal->ofr_broker_id_B_);
	SETIFASSIGNED(cache->ofrBrokerIdC, bonddeal->ofr_broker_id_C_);
	SETIFASSIGNED(cache->ofrBrokerIdD, bonddeal->ofr_broker_id_D_);
	SETIFASSIGNED(cache->ofrPercentA, bonddeal->ofr_percent_A_);
	SETIFASSIGNED(cache->ofrPercentB, bonddeal->ofr_percent_B_);
	SETIFASSIGNED(cache->ofrPercentC, bonddeal->ofr_percent_C_);
	SETIFASSIGNED(cache->ofrPercentD, bonddeal->ofr_percent_D_);
    SETIFASSIGNED(cache->ofr_check, bonddeal->ofr_checked_);
    if (bonddeal->ofr_checked_) {
        cache->ofr_check_date = GetTDateString("%Y-%m-%d");
    } else {
        cache->ofr_check_date = "";
    }
	SETIFASSIGNED(cache->ofr_brokerage_rate, bonddeal->ofr_brokerage_rate_);
	SETIFASSIGNED(cache->ofr_discount, bonddeal->ofr_discount_);

	SETIFASSIGNED(cache->delivery_type, bonddeal->settlement_mode_);
	SETIFASSIGNED(cache->description, bonddeal->description_);

	SETIFASSIGNED(cache->bidIsNoBrokerage, bonddeal->bid_is_no_brokerage_);
	SETIFASSIGNED(cache->bidBrokerrageRemark, bonddeal->bid_brokerage_remark_);
	SETIFASSIGNED(cache->bidTradeMode, bonddeal->bid_trade_mode_);
	SETIFASSIGNED(cache->bidNoConfirm, bonddeal->bid_no_confirm_);
	SETIFASSIGNED(cache->bidNoComments, bonddeal->bid_no_comments_);
	SETIFASSIGNED(cache->ofrIsNoBrokerage, bonddeal->ofr_is_no_brokerage_);
	SETIFASSIGNED(cache->ofrBrokerrageRemark, bonddeal->ofr_brokerage_remark_);
	SETIFASSIGNED(cache->ofrTradeMode, bonddeal->ofr_trade_mode_);
	SETIFASSIGNED(cache->ofrNoConfirm, bonddeal->ofr_no_confirm_);
	SETIFASSIGNED(cache->ofrNoComments, bonddeal->ofr_no_comments_);

	SETIFASSIGNED(cache->bid_remarks, bonddeal->bid_remarks_);
	SETIFASSIGNED(cache->ofr_remarks, bonddeal->ofr_remarks_);
	SETIFASSIGNED(cache->remarks, bonddeal->remarks_);
	SETIFASSIGNED(cache->specialPartyBid, bonddeal->special_party_bid_);
	SETIFASSIGNED(cache->specialPartyOfr, bonddeal->special_party_ofr_);
	SETIFASSIGNED(cache->specialTerms, bonddeal->special_terms_);
	SETIFASSIGNED(cache->feedback, bonddeal->feedback_);

	SETIFASSIGNED(cache->deal_status, bonddeal->deal_status_);
	SETIFASSIGNED(cache->inHand, bonddeal->in_hand_);
	SETIFASSIGNED(cache->urgent, bonddeal->urgent_status_);

	if (bonddeal->deal_status_ == kDcsDealPassed) {
        cache->confirm_date = GetTDateString("%Y-%m-%d");
    } else {
        cache->confirm_date = "";
    }

    SETIFASSIGNED(cache->internally, bonddeal->deal_internal_flag_); 

    CalculatorBusiness calc_business;
    calc_business.Calc(cache);

    // 成交表单里可手动修改保存 clean_price, dirty_price, spread, yield
    if (bonddeal->clean_price_.assigned() && bonddeal->clean_price_ != DOUBLE_NULL) {
        cache->net_price = bonddeal->clean_price_;
    }
    if (bonddeal->dirty_price_.assigned() && bonddeal->dirty_price_ != DOUBLE_NULL) {
        cache->full_price = bonddeal->dirty_price_;
    }
    if (bonddeal->spread_.assigned() && bonddeal->spread_ != DOUBLE_NULL) {
        cache->spread = bonddeal->spread_;
    }
    if (bonddeal->yield_.assigned() && bonddeal->yield_ != DOUBLE_NULL) {
        cache->yield = bonddeal->yield_;
    }
}

bool BondDealBusiness::DeleteBondDeal(const BondDealVecPtr& vec, ptree* delete_deal_msg_data /*= NULL*/)
{
    BondDealCacheController bonddeal_cc;
    std::vector<BondDealCachePtr> bond_deal_vec;
	
	if (delete_deal_msg_data != NULL) {
		for (int i = 0; i < vec->size(); ++i)
		{
			ptree each_delete_deal_msg_data;
			each_delete_deal_msg_data.put("id", vec->at(i)->id_.ref());
			delete_deal_msg_data->push_back(std::make_pair("", each_delete_deal_msg_data));
		}
	}

    std::string sql_str = /*GetCompanyConditionStringWithEndAnd() + */" id in "; //Modify by Young 05-Apr-2016
    sql_str += "(";
    for (int i = 0; i < vec->size(); ++i)
    {
        if (i > 0) {
            sql_str += ", ";
        }
        sql_str += "'" + vec->at(i)->id_.ref() + "'";
    }
    sql_str += ")";
    LOGGER_DEBUG("vec: " << vec->size() << ", sql length: " << sql_str.length());
    dbQuery q(sql_str.c_str());

    bonddeal_cc.updateCacheByQueryInThreadSafty(q, DeleteBondDealCallBack, NULL, DeleteBondDealSuccessed, DeleteBondDealFailed, (void*)(&bond_deal_vec));

    // 推送Deal -- "Delete"
    if (bond_deal_vec.size() > 0) {
        std::vector<BondDealCachePtr>::const_iterator it = bond_deal_vec.begin();
        for ( ; it != bond_deal_vec.end(); ++it) {
            dbQuery q;
            std::string str = "id = '" + (*it)->id + "'"/* + GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
            q = str.c_str();
            // Deal Cache里删除此成交
            bonddeal_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
        }

        service_->PublishBondDeal(bond_deal_vec, "DeleteList");

        QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
        if (qb_sync_service) {
            qb_sync_service->PublishBondDeal(bond_deal_vec, FID_PUSH_MESSAGE_METHOD_DELETE);
        }
    }
    MsgLogService::singleton()->writeDealLog(getRequestMsg(), bond_deal_vec);
    return true;
}

void BondDealBusiness::DeleteBondDealCallBack(BondDealCache* cache, void* param) {
    cache->status = "2"; // "2": 表示已删除
}

void BondDealBusiness::DeleteBondDealSuccessed(BondDealCache* cache, void* param) {
    std::vector<BondDealCachePtr>* bond_deal_vec = (std::vector<BondDealCachePtr>*)param;

    BondDealCache* bond_deal = new BondDealCache();
    *bond_deal = *cache;

    bond_deal_vec->push_back(BondDealCachePtr(bond_deal));
    LOGGER_INFO("DeleteBondDeal: Success for updating BondDealCache: " << cache->id)
}

void BondDealBusiness::DeleteBondDealFailed(BondDealCache* cache, void* param) {
    LOGGER_ERROR("DeleteBondDeal: Failed for updating BondDealCache: " << cache->id)
}

sdbus::string BondDealBusiness::CopyDeal(sdbus::VariantList deal_ids) 
{
    /*sdbus::string dealString;
    if (GetCompanyId() == kCompanyTP)
        dealString = CopyDealForTP(deal_ids);
    else if(GetCompanyId() == kCompanyICAP)
        dealString = CopyDealForICAP(deal_ids);
    else if(GetCompanyId() == kCompanyBGC)
        dealString = CopyDealForBGC(deal_ids);
    else if(GetCompanyId() == kCompanyPATR)
        dealString = CopyDealForPATR(deal_ids);
	else
		dealString = CopyDealForTP(deal_ids);

    return dealString;*/
    return CopyDealForTP(deal_ids);
}

sdbus::string BondDealBusiness::CopyDealForTP(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", GetCompanyId(); //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL) {
            continue;
        }

        // --- 剩余期限 ---
        dealTexts = dealTexts + getExpTTM(bond_deal_cache->time_to_maturity, bond_deal_cache->maturity_holidays, 0) + "\t";

        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 0) + "\t";
        }

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 0) + "\t";

        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, 2);
       
        dealTexts = dealTexts + sPrice + " \t"; 

        // --- 交易操作 ---
        dealTexts = dealTexts + getExpDealDirection(bond_deal_cache->deal_type) + "\t";

        // --- 主体评级 ----
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + bond_deal_cache->issuer_rating;
        }
        
        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}

sdbus::string BondDealBusiness::CopyDealForBGC(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL) {
            continue;
        }

        // --- 剩余期限 ---
        dealTexts =  dealTexts + getExpTTM(bond_deal_cache->time_to_maturity, 0) + "\t";

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 0) + "\t";

        // --- 浮息 ---
        if ( bond_deal_cache->bond_category == "BNC" )
        {
            if (bond_deal_cache->rate_index.length() >= 4  &&  bond_deal_cache->rate_index != "FIXED")
            {
                std::string rateIndex = bond_deal_cache->rate_index.substr(0, 4);
                std::transform (rateIndex.begin(), rateIndex.end(), rateIndex.begin(), ::tolower);

                dealTexts = dealTexts + rateIndex + "\t";
            }
        }
     
        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 0) + "\t";
        }

        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, 2);

        dealTexts = dealTexts + sPrice + " \t"; 

        // --- 主体评级 ----
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + bond_deal_cache->issuer_rating + "\t";
        }
   
        // --- 交易操作 ---
        dealTexts = dealTexts + getExpDealDirection(bond_deal_cache->deal_type);

        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}

sdbus::string BondDealBusiness::CopyDealForPATR(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL) {
            continue;
        }

        // --- 剩余期限 ---
        dealTexts =  dealTexts + getExpTTM(bond_deal_cache->time_to_maturity, 8) + " ";

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 10) + " ";
        
        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 21) + " ";
        }
        
        // --- 主体评级 ----
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpRatings(bond_deal_cache->issuer_rating, 8) + " ";
        }
        
        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, 2, 10);

        dealTexts = dealTexts + sPrice + " "; 

        // --- 交易操作 ---
        dealTexts = dealTexts + getExpDealDirection(bond_deal_cache->deal_type);

        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}

sdbus::string BondDealBusiness::CopyDealForICAP(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL) {
            continue;
        }

        // --- 剩余期限 ---
        std::string ttm = bond_deal_cache->time_to_maturity; 
        std::transform (ttm.begin(), ttm.end(), ttm.begin(), ::tolower);
        dealTexts =  dealTexts + ttm + "\t";    
        
        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 0) + "\t";
        
        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 0) + "\t";
        }

        // --- 评级 ----
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            std::string ratings= getExpRatings(bond_deal_cache->issuer_rating, bond_deal_cache->bond_rating);
            std::transform (ratings.begin(), ratings.end(), ratings.begin(), ::tolower);
            dealTexts = dealTexts + ratings + "\t";
        }

        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, 2);

        dealTexts = dealTexts + sPrice + " \t"; 

        // --- 交易操作 ---
        dealTexts = dealTexts + getExpDealDirection(bond_deal_cache->deal_type) + "\n";
    }

    return dealTexts;
}

sdbus::string BondDealBusiness::CopyDetailedDeal(sdbus::VariantList deal_ids) 
{
    /*sdbus::string dealString;
    if (GetCompanyId() == kCompanyTP)
        dealString =  CopyDetailedDealForTP(deal_ids);
    else if(GetCompanyId() == kCompanyICAP)
        dealString = CopyDetailedDealForICAP(deal_ids);
    else if(GetCompanyId() == kCompanyBGC)
        dealString = CopyDetailedDealForBGC(deal_ids);
    else if(GetCompanyId() == kCompanyPATR)
        dealString = CopyDetailedDealForPATR(deal_ids);
	else
		dealString = CopyDetailedDealForTP(deal_ids);

    return dealString;*/
    return CopyDetailedDealForTP(deal_ids);
}

sdbus::string BondDealBusiness::CopyDetailedDealForTP(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", GetCompanyId(); //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL)
        {
            continue;
        }

        // --- 剩余期限 ---
        dealTexts = dealTexts + getExpTTM(bond_deal_cache->time_to_maturity, bond_deal_cache->maturity_holidays, 0) + "\t";

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 0) + "\t";
        
        // --- 简称 --- 
        dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 0) + "\t";

        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, false);

        dealTexts = dealTexts + sPrice + " \t"; 

        // --- 量 ---
        dealTexts = dealTexts + getExpVolume(bond_deal_cache->volume) + "\t";

        // --- 交割日 ---
        dealTexts = dealTexts + bond_deal_cache->settlement_date + "\t";

        // --- 结算方式(DVP) -- 
        dealTexts = dealTexts + bond_deal_cache->delivery_type + "\t";

        // --- Ofr机构（ofr交易员）---
        if (!bond_deal_cache->ofr_trader_name.empty())
        {
            if ( isBCOCategoryType(bond_deal_cache->bond_category) )
            {
                dealTexts = dealTexts + bond_deal_cache->ofr_institution_name + "(" + bond_deal_cache->ofr_trader_name + ")" + "\t";
            }
            else
            {
                dealTexts = dealTexts + bond_deal_cache->ofr_institution_name + bond_deal_cache->ofr_trader_name + "\t";
            }
        }
        
        dealTexts = dealTexts + GBKToUTF8("出给") + "\t";

        // --- Bid机构（bid交易员）---
        if (!bond_deal_cache->bid_trader_name.empty())
        {
            if ( isBCOCategoryType(bond_deal_cache->bond_category) )
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name + "(" + bond_deal_cache->bid_trader_name + ")" + "\t";
            }
            else
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name  + bond_deal_cache->bid_trader_name + "\t";
            }           
        }
        
        // --- Ofr经纪人 ---
        dealTexts = dealTexts + bond_deal_cache->ofr_broker_name + "\t";

        // --- Bid经纪人 ---
        dealTexts = dealTexts + bond_deal_cache->bid_broker_name;

        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}

sdbus::string BondDealBusiness::CopyDetailedDealForBGC(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL)
        {
            continue;
        }

        // --- 交易日 ---
        dealTexts =  dealTexts + bond_deal_cache->traded_date + "\t";

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 0) + "\t";

        // --- 简称 --- 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 0) + "\t";
        }
        
        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, false);

        dealTexts = dealTexts + sPrice + " \t"; 

        // --- 量 ---
        dealTexts = dealTexts + getExpVolume(bond_deal_cache->volume) + "\t";

        // --- 剩余期限 ---
        dealTexts =  dealTexts + getExpTTM(bond_deal_cache->time_to_maturity, 0) + "\t";

        // --- Bid机构（bid交易员）Bid经纪人---
        if (!bond_deal_cache->bid_trader_name.empty())
        {
            if ( isBCOCategoryType(bond_deal_cache->bond_category) )
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name + "\t" + "(" + bond_deal_cache->bid_trader_name + ")" + "\t" + bond_deal_cache->bid_broker_name + "\t";
            }
            else
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name + "\t" + bond_deal_cache->bid_trader_name + "\t" + bond_deal_cache->bid_broker_name + "\t";

            }
        }

        // --- Ofr机构（ofr交易员）Ofr经纪人---
        if (!bond_deal_cache->ofr_trader_name.empty())
        {
            if ( isBCOCategoryType(bond_deal_cache->bond_category) )
            {
                dealTexts = dealTexts + bond_deal_cache->ofr_institution_name + "\t" + "(" + bond_deal_cache->ofr_trader_name + ")" + "\t" + bond_deal_cache->ofr_broker_name + "\t";
            }
            else
            {
                dealTexts = dealTexts + bond_deal_cache->ofr_institution_name + "\t"  + bond_deal_cache->ofr_trader_name + "\t" + bond_deal_cache->ofr_broker_name + "\t";
            }
        }

        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}

sdbus::string BondDealBusiness::CopyDetailedDealForPATR(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL)
        {
            continue;
        }

        // --- 简称 --- 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 21) + " ";
        }
        
        // --- 剩余期限 ---
        dealTexts =  dealTexts + getExpTTM(bond_deal_cache->time_to_maturity, 8) + " ";

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 10) + " ";

        // --- 价格 ---
        dealTexts = dealTexts + getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, 1, 10) + " "; 

        // --- 量 ---
        std::string sVolume = getExpVolume(bond_deal_cache->volume);
        if (sVolume.size() < 6)
        {
            std::ostringstream oss;
            oss << std::setw(6) << setiosflags(std::ios::left) << sVolume;
            sVolume = oss.str();
        }

        dealTexts = dealTexts + sVolume + " ";

        // --- 清算方式 ---
        dealTexts = dealTexts + bond_deal_cache->time_style + "  ";

        // --- 结算方式(DVP) -- 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + bond_deal_cache->delivery_type + "  ";
        }
        else if (bond_deal_cache->delivery_type.length() >= 3)
        {
            dealTexts = dealTexts + bond_deal_cache->delivery_type.substr(0, 3) + "  ";
        }
        
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            // --- Ofr机构（ofr交易员）---
            if (!bond_deal_cache->ofr_trader_name.empty())
            {
                dealTexts = dealTexts + bond_deal_cache->ofr_institution_name + "(" + bond_deal_cache->ofr_trader_name + ")" + " ";
            }

            dealTexts = dealTexts + GBKToUTF8("出给") + " ";

            // --- Bid机构（bid交易员）---
            if (!bond_deal_cache->bid_trader_name.empty())
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name + "(" + bond_deal_cache->bid_trader_name + ")";
            }
        }

        else
        {
            // --- Bid机构（bid交易员）---
            if (!bond_deal_cache->bid_trader_name.empty())
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name + " " + bond_deal_cache->bid_trader_name;
            }

            // --- Ofr机构（ofr交易员）---
            if (!bond_deal_cache->ofr_trader_name.empty())
            {
                dealTexts = dealTexts +  "/" + bond_deal_cache->ofr_institution_name + " " + bond_deal_cache->ofr_trader_name;
            }
        }
  
        // --- 过桥信息 ---
         if ( bond_deal_cache->bond_category == "BNC" )
         {
             if (bond_deal_cache->bid_bridge == "2" || bond_deal_cache->ofr_bridge == "2")
             {
                 dealTexts = dealTexts + "  " + "(" + GBKToUTF8("桥") + ")";
             }
         }

        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}


sdbus::string BondDealBusiness::CopyDetailedDealForICAP(sdbus::VariantList deal_ids) 
{
    sdbus::string dealTexts;

    for (int i = 0; i < deal_ids.Size(); ++i)
    {
        BondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL)
        {
            continue;
        }

        // --- 剩余期限 ---
        std::string ttm = bond_deal_cache->time_to_maturity; 
        std::transform (ttm.begin(), ttm.end(), ttm.begin(), ::tolower);
        dealTexts =  dealTexts + ttm + "\t";  

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 0) + "\t";

        // --- 简称 --- 
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 0) + "\t";
        }

        // --- 评级 ----
        if ( isBCOCategoryType(bond_deal_cache->bond_category) )
        {
            std::string ratings= getExpRatings(bond_deal_cache->issuer_rating, bond_deal_cache->bond_rating);
            std::transform (ratings.begin(), ratings.end(), ratings.begin(), ::tolower);
            dealTexts = dealTexts + ratings + "\t";
        }
        
        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, false);

        dealTexts = dealTexts + sPrice + " \t"; 

        // --- 量 ---
        dealTexts = dealTexts + getExpVolume(bond_deal_cache->volume) + "\t";

        // --- 交割日 ---
        dealTexts = dealTexts + bond_deal_cache->time_style + "\t";

        // --- 结算方式(DVP) -- 
        dealTexts = dealTexts + bond_deal_cache->delivery_type + "\t";

        // --- "麻烦"---
        dealTexts = dealTexts + GBKToUTF8("麻烦") + "\t";

        // --- Ofr机构（ofr交易员）---
        if (!bond_deal_cache->ofr_trader_name.empty())
        {
            dealTexts = dealTexts + bond_deal_cache->ofr_institution_name + " ";
        }

        dealTexts = dealTexts + GBKToUTF8("出给") + " ";

        // --- Bid机构（bid交易员）---
        if (!bond_deal_cache->bid_trader_name.empty())
        {
            if ( isBCOCategoryType(bond_deal_cache->bond_category) )
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name + "(" + bond_deal_cache->bid_trader_name + ")";
            }
            else
            {
                dealTexts = dealTexts + bond_deal_cache->bid_institution_name + bond_deal_cache->bid_trader_name;
            }
        }

        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}
