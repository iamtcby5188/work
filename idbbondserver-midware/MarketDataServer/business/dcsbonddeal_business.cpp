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
#include "dcsbonddeal_business.h"
#include "bondquote_business.h"
#include "db/dbconnector.h"
#include "include/common.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "cache/cache_center.h"
#include "include/fid.h"
#include "service/service_manager.h"

#include <iomanip>

#include "include/text_export.h"

DcsBondDealBusiness::DcsBondDealBusiness(DcsBondDealService* service)
    : service_(service)
{
    Init();
}

void DcsBondDealBusiness::Init()
{
    // 设置FID与数据库表字段名对应关系
    SET_FID_MAP(FID_BOND_SHORT_NAME, bond_short_name)
    SET_FID_MAP(FID_PIN_YIN, pin_yin)
    SET_FID_MAP(FID_PIN_YIN_FULL, pin_yin_full)
    SET_FID_MAP(FID_ISSUE_INSTITUTION_RATING, institution_rating)
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
	SET_FID_MAP(FID_ISSUE_YEAR, issue_year) //必须添加过滤条件
	SET_FID_MAP(FID_SW_SECTOR_CODE, sw_subsector_code)
    SET_FID_MAP(FID_ISSUE_PROVINCE_CODE, province_code)
	SET_FID_MAP(FID_FINANCING_PLATFORM, financing_platform)
    SET_FID_MAP(FID_ISSUING_INSTITUTION, issue_institution_id) // issuing institution
	SET_FID_MAP(FID_COMPANY_BOND, company_bond)
	SET_FID_MAP(FID_OUTLOOK, outlook) 

	SET_FID_MAP(FID_BOND_SUBTYPE, bond_subtype);
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
	SET_FID_MAP(FID_DURATION_FILTER, val_modified_duration)
    SET_FID_MAP(FID_BID_INSTITUTION_NAME, bid_institution_name)
    SET_FID_MAP(FID_OFR_INSTITUTION_NAME, ofr_institution_name)
    SET_FID_MAP(FID_BID_TRADER_NAME, bid_trader_name)
    SET_FID_MAP(FID_OFR_TRADER_NAME, ofr_trader_name)
	SET_FID_MAP(FID_OPTION_TYPE, option_client)

	SET_FID_MAP(FID_DCS_QUESTION_MARK, questionMark)
	SET_FID_MAP(FID_SERIAL_NUMBER, sequence_number)
	SET_FID_MAP(FID_DEAL_NO, odd_numbers)
	SET_FID_MAP(FID_DEAL_STATUS_STRING, deal_status)
	SET_FID_MAP(FID_RELATED_MSG, relatedMsg)
	SET_FID_MAP(FID_BID_INSTITUTION_TRADER_NAMES, bid_trader_name)
	SET_FID_MAP(FID_OFR_INSTITUTION_TRADER_NAMES, ofr_trader_name)
}

//------------------------------------------------------------------------------------------------------------------------
DcsBondDealCacheVecPtr DcsBondDealBusiness::FetchBondDeals(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range)
{
	std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, range, "create_time desc");
    dbQuery query(str.c_str());
    DcsBondDealCacheController bondDealCC;
    DcsBondDealCacheVecPtr deals = bondDealCC.getCacheListByQueryInThreadSafty(query);
    std::vector<DcsBondDealCachePtr>::iterator it = deals->begin();
    for ( ; it != deals->end(); ++it) {
        DcsBondDealCachePtr bond_deal = *it;
        if (bond_deal) {
            ChinaBondValuationCacheController china_bond_valuation_cc;
            ChinaBondValuationCachePtr china_bond_valuation_cache = china_bond_valuation_cc.getValuation(bond_deal->bond_id);

            if (china_bond_valuation_cache) {
                bond_deal->cdc_valuation_yield = china_bond_valuation_cache->valuation_yield;
                bond_deal->cdc_valuation_net_price = china_bond_valuation_cache->valuation_net_price;
            } 
        }
    }

    return deals;
}

//------------------------------------------------------------------------------------------------------------------------
DcsBondDealCacheVecPtr DcsBondDealBusiness::GetBondDealByID(const std::string& bond_deal_id)
{
    std::string str = "id = '" + bond_deal_id + "'" + GetCompanyConditionStringWithPreAnd();
    dbQuery query(str.c_str());
    DcsBondDealCacheController bondDealCC;
    return bondDealCC.getCacheListByQueryInThreadSafty(query);
}

//------------------------------------------------------------------------------------------------------------------------
DcsBondDealCacheVecPtr DcsBondDealBusiness::FetchBondDealsByDate(const std::string& cond, const std::string& datetime)
{
    std::string str = datetime + "%";
    std::string qStr = " bond_category = '" + cond + "'" + GetCompanyConditionStringWithPreAnd() + " and create_time like '" + str + "' order by bond_subtype,coupon_type desc";
    dbQuery query(qStr.c_str());
    DcsBondDealCacheController bondDealCC;
    return bondDealCC.getCacheListByQueryInThreadSafty(query);
}

//------------------------------------------------------------------------------------------------------------------------
int DcsBondDealBusiness::GetBondDealCount(const sdbus::string& cond)
{
	std::string str = GetFastdbQueryWithCompanyCondition(cond, "", "");
    dbQuery query(str.c_str());

    DcsBondDealCacheController bondDealCC;
    return bondDealCC.getSizeByQueryInThreadSafty(query);
}

//------------------------------------------------------------------------------------------------------------------------
int DcsBondDealBusiness::GetDealSequenceNumber()
{
    std::string today = GetTDateString("%Y-%m-%d");
    std::string low = today + " 00:00:00";
    std::string high = today + " 23:59:59";
    std::string str = " create_time >= '" + low + "' and create_time <= '" + high + "' and sequence_number > 0 " + GetCompanyConditionStringWithPreAnd() + " order by sequence_number desc"; 
    dbQuery query(str.c_str());

    DcsBondDealCacheController bondDealCC;
    DcsBondDealCacheVecPtr deal_cache_vec = bondDealCC.getCacheListByQueryInThreadSafty(query);
    if (deal_cache_vec->size() > 0) {
        return deal_cache_vec->at(0)->sequence_number;
    }

    return 0;
}

sdbus::string DcsBondDealBusiness::CopyDeal(sdbus::VariantList deal_ids)
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

sdbus::string DcsBondDealBusiness::CopyDealForTP(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", GetCompanyId();
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

        if (bond_deal_cache == NULL) {
            continue;
        }

        if (isBCOCategoryType(bond_deal_cache->bond_category)) { // 信用债
            // --- 剩余期限 ---
            dealTexts = dealTexts + getExpTTM(bond_deal_cache->time_to_maturity, bond_deal_cache->maturity_holidays, 0) + "\t";
        }        

        // --- 简称 
        dealTexts = dealTexts + getExpBondShortName(bond_deal_cache->bond_short_name, 0) + "\t";

        // --- 代码 ---
        dealTexts = dealTexts + getExpBondCode(bond_deal_cache->bond_id, 0) + "\t";

        // --- 价格 ---
        std::string sPrice = getExpPrice(bond_deal_cache->price, bond_deal_cache->rebate, bond_deal_cache->return_point, 0, 2);
		if ((isBCOCategoryType(bond_deal_cache->bond_category)) && ("Y" == bond_deal_cache->has_option)) { // 信用债且含权
            if (bond_deal_cache->exercise == "0") {
                sPrice += GBKToUTF8("行权");
            } else if (bond_deal_cache->exercise == "1") {
				sPrice += GBKToUTF8("到期");
            }
        }
        dealTexts = dealTexts + sPrice + " \t"; 

        // --- 量 ---
        dealTexts = dealTexts + getExpVolume(bond_deal_cache->volume) + "\t";

        // --- 交割日+结算方式 ---
        std::string sDate = bond_deal_cache->settlement_date;
        if (!sDate.empty() && "T+1" == bond_deal_cache->time_style)
            sDate = sDate + "+1";
        dealTexts = dealTexts + sDate + "\t";
        
		// --- 主体评级 ----
		if (isBCOCategoryType(bond_deal_cache->bond_category)) {
			dealTexts = dealTexts + bond_deal_cache->issuer_rating + "\t";
		}

        // --- 交易双方机构 ---
        if (!bond_deal_cache->bid_institution_name.empty() && !bond_deal_cache->ofr_institution_name.empty()) {
			dealTexts += bond_deal_cache->ofr_institution_name + GBKToUTF8("出给") + bond_deal_cache->bid_institution_name;
        }
        
        dealTexts = dealTexts + "\n";
    }

    return dealTexts;
}

sdbus::string DcsBondDealBusiness::CopyDealForBGC(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

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

sdbus::string DcsBondDealBusiness::CopyDealForPATR(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

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

sdbus::string DcsBondDealBusiness::CopyDealForICAP(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

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

sdbus::string DcsBondDealBusiness::CopyDetailedDeal(sdbus::VariantList deal_ids)
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

sdbus::string DcsBondDealBusiness::CopyDetailedDealForTP(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", GetCompanyId(); //Modify by Young 05-Apr-2016
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

		if (bond_deal_cache == NULL || bond_deal_cache->deal_status != kDcsDealPassed)
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

sdbus::string DcsBondDealBusiness::CopyDetailedDealForBGC(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

		if (bond_deal_cache == NULL || bond_deal_cache->deal_status != kDcsDealPassed)
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

sdbus::string DcsBondDealBusiness::CopyDetailedDealForPATR(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

		if (bond_deal_cache == NULL || bond_deal_cache->deal_status != kDcsDealPassed)
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


sdbus::string DcsBondDealBusiness::CopyDetailedDealForICAP(sdbus::VariantList deal_ids)
{
    sdbus::string dealTexts;

    for (size_t i = 0; i < deal_ids.Size(); ++i)
    {
        DcsBondDealCacheController bond_deal_cc;
        dbQuery q;
        sdbus::string deal_id = deal_ids.Get(i)->ToString().c_str();
        q = "id = ", deal_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        DcsBondDealCachePtr bond_deal_cache = bond_deal_cc.getCacheByQueryInThreadSafty(q);

		if (bond_deal_cache == NULL || bond_deal_cache->deal_status != kDcsDealPassed)
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
