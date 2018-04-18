/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Quote Business
====================================================================================================================
   Date       Name                                  Description of Change
20-Jun-2014   Huameng Jia                           Initial version
19-Aug-2014   James Xu                              Add CopyQuote
21-Aug-2014   James Xu                              Add CopyReferredQuote  
04-Sep-2014   James Xu                              Generate create_time at server side and only update deal_time when a quote is referred
03-Mar-2015   James Xu                              TP: 信用ref 报价格式调整
25-Mar-2015   James Xu                              取消TP信用ref报价格式调整
$HISTORY$
===================================================================================================================*/
#include <sstream>
#include "bondquote_business.h"
#include "bonddeal_business.h"
#include "bondbestquote_business.h"
#include "db/dbconnector.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "cache/cache_center.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "include/text_export.h"
#include "connection_manager.h"
#include "service/service_manager.h"
#include "boost/lexical_cast.hpp"

#include <unordered_set>
#include <iomanip>

BondQuoteBusiness::BondQuoteBusiness(BondQuoteService* service)
    : service_(service)
{
    Init();
}

void BondQuoteBusiness::Init()
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
    SET_FID_MAP(FID_QUOTE_INTERNAL_FLAG, internally)
    SET_FID_MAP(FID_BROKER_ID, operator_id)
    SET_FID_MAP(FID_QUOTE_SIDE, symbol)
	SET_FID_MAP(FID_ISSUE_YEAR, issue_year) //必须添加过滤条件
	SET_FID_MAP(FID_SW_SECTOR_CODE, sw_subsector_code)
    SET_FID_MAP(FID_ISSUE_PROVINCE_CODE, province_code)
	SET_FID_MAP(FID_FINANCING_PLATFORM, financing_platform)
    SET_FID_MAP(FID_ISSUING_INSTITUTION, issue_institution_id) // issuing institution
    SET_FID_MAP(FID_COMPANY_BOND, company_bond)
	SET_FID_MAP(FID_OUTLOOK, outlook) //Add by Young 20-Apr-2016

    SET_FID_MAP(FID_CDC_OFFSET, cdc_offset)
	SET_FID_MAP(FID_DURATION_FILTER, val_modified_duration)

	SET_FID_MAP(FID_BOND_SUBTYPE, bond_subtype);
    SET_FID_MAP(FID_MATURITY_HOLIDAYS, maturity_holidays);

    // 全局搜索过滤
    SET_FID_MAP(FID_INSTITUTION_ID, bank_id)
    SET_FID_MAP(FID_TRADER_ID, bank_agent_id)
    SET_FID_MAP(FID_BROKER_ID, operator_id)
    SET_FID_MAP(FID_ACROSS_MARKET_BOND_CODES, across_market_bond_ids)

    // 排序
    SET_FID_MAP(FID_TIME_TO_MATURITY_REAL, time_to_maturity_real)
    SET_FID_MAP(FID_BOND_CODE, bond_id)
    SET_FID_MAP(FID_PRICE, price)
    SET_FID_MAP(FID_BID_PRICE, price)
    SET_FID_MAP(FID_OFR_PRICE, price)
    SET_FID_MAP(FID_VOLUME, volume)
    SET_FID_MAP(FID_BROKER_NAME, broker_name)
    SET_FID_MAP(FID_ISSUER_RATING_ORDER, issuer_rating_order)
    SET_FID_MAP(FID_ISSUER_RATING_NPY_ORDER, issuer_rating_npy_order)
    SET_FID_MAP(FID_BOND_RATING_ORDER, bond_rating_order)
    SET_FID_MAP(FID_BOND_RATING_NPY_ORDER, bond_rating_npy_order)
    SET_FID_MAP(FID_CREATE_TIME, create_time)
    SET_FID_MAP(FID_REFER_TIME, deal_time)
    SET_FID_MAP(FID_CDC_VALUATIONS_YIELD, cdc_valuation_yield)
    SET_FID_MAP(FID_CDC_VALUATIONS_CLEAN_PRICE, cdc_valuation_net_price)
    SET_FID_MAP(FID_BID_INSTITUTION_NAME, bid_institution_name)
    SET_FID_MAP(FID_OFR_INSTITUTION_NAME, ofr_institution_name)
    SET_FID_MAP(FID_BID_TRADER_NAME, bid_trader_name)
    SET_FID_MAP(FID_OFR_TRADER_NAME, ofr_trader_name)
    SET_FID_MAP(FID_OPTION_TYPE, option_client)
    SET_FID_MAP(FID_CLEAN_PRICE, net_price)
    SET_FID_MAP(FID_DIRTY_PRICE, full_price)
    SET_FID_MAP(FID_SPREAD, spread)
}

void BondQuoteBusiness::FetchBondQuoteId(BondQuoteCache* cache, void* param)
{
    if (param == NULL) {
        return;
    }

    std::vector<std::string>* vec = (std::vector<std::string>*)param;
    vec->push_back(cache->id);
}

BondQuoteCacheVecPtr BondQuoteBusiness::FetchBondQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range)
{    
	std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, range, "create_time desc"); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteCacheController bondQuoteCC;
    BondQuoteCacheVecPtr quotes = bondQuoteCC.getCacheListByQueryInThreadSafty(query);
    std::vector<BondQuoteCachePtr>::iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteCachePtr bond_quote = *it;
        if (bond_quote) {
            ChinaBondValuationCacheController china_bond_valuation_cc;
            ChinaBondValuationCachePtr china_bond_valuation_cache = china_bond_valuation_cc.getValuation(bond_quote->bond_id);

            if (china_bond_valuation_cache) {
                bond_quote->cdc_valuation_yield = china_bond_valuation_cache->valuation_yield;
                bond_quote->cdc_valuation_net_price = china_bond_valuation_cache->valuation_net_price;
                if (bond_quote->symbol == kOfrQuote && bond_quote->cdc_valuation_yield.length() > 0) {
                    if (bond_quote->yield != DOUBLE_NULL) {
                        bond_quote->cdc_offset = (bond_quote->yield - atof(bond_quote->cdc_valuation_yield.c_str())) * 100;
                    } else if (bond_quote->price != DOUBLE_NULL && bond_quote->quote_type == "0") {
                        bond_quote->cdc_offset = (bond_quote->price - atof(bond_quote->cdc_valuation_yield.c_str())) * 100;
                    }
                } 
            }
        }
    }

    return quotes;
}

BondQuoteCacheVecPtr BondQuoteBusiness::GetBondQuoteByID(const std::string& bond_quote_id)
{
    std::string str = "id = '" + bond_quote_id + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteCacheController bondQuoteCC;
    return bondQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteReferCacheVecPtr BondQuoteBusiness::GetBondReferByID(const std::string& bond_quote_id)
{
    std::string str = "id = '" + bond_quote_id + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteReferCacheController bondReferQuoteCC;
    return bondReferQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteCacheVecPtr BondQuoteBusiness::GetBondQuoteByConditions(const int& side, const std::string& internal_flag, const std::string& bond_key_listed_market, const std::string& institution_id, const std::string& trader_id)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' and bank_id = '" + institution_id + "' and bank_agent_id = '" + trader_id + "' and symbol = " + boost::lexical_cast<std::string>(side) + " and internally = '" + internal_flag + "' and deal_status = '0' " + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteCacheController bondQuoteCC;
    return bondQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteReferCacheVecPtr BondQuoteBusiness::FetchBondReferQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range)
{
	std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, range, "deal_time desc"); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteReferCacheController bondReferQuoteCC;
    BondQuoteReferCacheVecPtr quotes = bondReferQuoteCC.getCacheListByQueryInThreadSafty(query);

    std::vector<BondQuoteReferCachePtr>::iterator it = quotes->begin();
    for ( ; it != quotes->end(); ++it) {
        BondQuoteReferCachePtr bond_quote = *it;
        if (bond_quote) {
            ChinaBondValuationCacheController china_bond_valuation_cc;
            ChinaBondValuationCachePtr china_bond_valuation_cache = china_bond_valuation_cc.getValuation(bond_quote->bond_id);

            if (china_bond_valuation_cache) {
                bond_quote->cdc_valuation_yield = china_bond_valuation_cache->valuation_yield;
                bond_quote->cdc_valuation_net_price = china_bond_valuation_cache->valuation_net_price;
                if (bond_quote->symbol == kOfrQuote && bond_quote->cdc_valuation_yield.length() > 0) {
                    if (bond_quote->yield != DOUBLE_NULL) {
                        bond_quote->cdc_offset = (bond_quote->yield - atof(bond_quote->cdc_valuation_yield.c_str())) * 100;
                    } else if (bond_quote->price != DOUBLE_NULL && bond_quote->quote_type == "0") {
                        bond_quote->cdc_offset = (bond_quote->price - atof(bond_quote->cdc_valuation_yield.c_str())) * 100;
                    }
                } 
            } 
        }
    }

    return quotes;
}

BondQuoteCacheVecPtr BondQuoteBusiness::FetchBondQuotesByDate(const std::string& cond, const std::string& bondtype, const std::string& datetime)
{
    std::string str = datetime + "%";
    std::string str1 = " bond_category = '" + cond + "' and bond_type != '" + bondtype + "' " + GetCompanyConditionStringWithPreAnd() + " and create_time like '" + str + "' order by bond_type, goods_term asc"; //Modify by Young 05-Apr-2016
    dbQuery query1(str1.c_str());
    std::string str2 = " bond_category = '" + cond + "' and bond_type = '"  + bondtype + "' " + GetCompanyConditionStringWithPreAnd() + " and create_time like '" + str + "' order by goods_term asc"; //Modify by Young 05-Apr-2016
    dbQuery query2(str2.c_str());
    BondQuoteCacheController bondQuoteCC;
    BondQuoteCacheVecPtr vec1 = bondQuoteCC.getCacheListByQueryInThreadSafty(query1);
    BondQuoteCacheVecPtr vec2 = bondQuoteCC.getCacheListByQueryInThreadSafty(query2);
    if(vec1->size() == 0)
        return vec2;
    if(vec2->size() == 0)
        return vec1;
    for(size_t i = 0; i < vec2->size(); ++i)
    {   
        vec1->push_back(vec2->at(i));
    }

    return vec1;
}

std::vector<std::string> BondQuoteBusiness::GetBondQuoteIdList(const sdbus::string& cond, const sdbus::string& sortby)
{
    std::vector<std::string> vec;

	std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, ""); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());

    BondQuoteCacheController bondQuoteCC;

    bondQuoteCC.traverseCacheByQueryInThreadSafty(query, FetchBondQuoteId, &vec);

    return vec;
}

int BondQuoteBusiness::GetBondQuotesCount(const sdbus::string& cond)
{
	std::string str = GetFastdbQueryWithCompanyCondition(cond, "", ""); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());

    BondQuoteCacheController bondQuoteCC;
    return bondQuoteCC.getSizeByQueryInThreadSafty(query);
}

int BondQuoteBusiness::GetBondReferQuotesCount(const sdbus::string& cond)
{
	std::string str = GetFastdbQueryWithCompanyCondition(cond, "", ""); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());

    BondQuoteReferCacheController bondReferQuoteCC;
    return bondReferQuoteCC.getSizeByQueryInThreadSafty(query);
}

sdbus::string BondQuoteBusiness::CopyQuote(sdbus::VariantList quote_ids) 
{
    /*sdbus::string quoteString;
    if (GetCompanyId() == kCompanyTP)
		quoteString = CopyQuoteForTP(quote_ids);
	else
		quoteString = CopyQuoteForTP(quote_ids);

    return quoteString;*/
    return CopyQuoteForTP(quote_ids);
}

sdbus::string BondQuoteBusiness::CopyReferredQuote(sdbus::VariantList quote_ids) 
{
    /*sdbus::string refQuoteString;
    if (GetCompanyId() == kCompanyTP)
		refQuoteString = CopyReferredQuoteForTP(quote_ids);
	else
		refQuoteString = CopyReferredQuoteForTP(quote_ids);

    return refQuoteString;*/
    return CopyReferredQuoteForTP(quote_ids);
}

sdbus::string BondQuoteBusiness::CopyQuoteForTP(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (size_t i = 0; i < quote_ids.Size(); ++i)
    {
        BondQuoteCacheController bond_quote_cc;
        dbQuery q;
        sdbus::string quote_id = quote_ids.Get(i)->ToString().c_str();
        q = "id = ", quote_id, " and company_id = ", GetCompanyId(); //Modify by Young 05-Apr-2016
        BondQuoteCachePtr bond_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        if (bond_quote_cache == NULL)
        {
            continue;
        }
        // --- 剩余期限 ---
        quotesTexts = quotesTexts + getExpTTM(bond_quote_cache->time_to_maturity, bond_quote_cache->maturity_holidays) + " ";
                    
        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(bond_quote_cache->bond_short_name) + " ";
        }
            
        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id) + " ";
              
        // --- 价格(备注) ---
        std::string sPrice = getExpPrice(bond_quote_cache->price, bond_quote_cache->rebate, bond_quote_cache->return_point, bond_quote_cache->symbol);
        std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
        std::string priceAndNotes = getExpPriceAndNotes(sPrice, sNotes, bond_quote_cache->symbol);
        
        quotesTexts = quotesTexts + priceAndNotes + "  "; // 价格后面空两格

        // --- 交易量 ---
        quotesTexts = quotesTexts + getExpVolume(bond_quote_cache->volume, bond_quote_cache->symbol) + "    "; // 量后面空4个

        if ( bond_quote_cache->bond_category == "BNC" )
        {
            if (bond_quote_cache->bond_type == GBKToUTF8("地方债")) {
                quotesTexts = quotesTexts + bond_quote_cache->bond_short_name + "; ";
				quotesTexts = quotesTexts + getCouponRate(bond_quote_cache->coupon_rate)+" ";
            }
        }    

        // --- 评级 ----
        bool hasRatings = false;
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            hasRatings = true;
            quotesTexts = quotesTexts + getExpRatings(bond_quote_cache->issuer_rating, bond_quote_cache->bond_rating);
        }
          
        // --- 担保 ----
        bool isGuarenteed = false;
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            if (bond_quote_cache->guarenteed == "Y")
            {
                isGuarenteed = true;
                quotesTexts = quotesTexts + (hasRatings ? ";" : "") + GBKToUTF8("有担保");
            }
        }


        // --- 浮息 ----
        bool isFloating = false;
        if (bond_quote_cache->rate_index.length() > 0  &&  bond_quote_cache->rate_index != "FIXED")
        {
            isFloating = true;
            if ( isBCOCategoryType(bond_quote_cache->bond_category) )
            {
                quotesTexts = quotesTexts + (hasRatings || isGuarenteed ? ";" : "") + getRateIndex(bond_quote_cache->rate_index, 0, GetCompanyId()) + GBKToUTF8("浮动");
            }
            else
            {
                quotesTexts = quotesTexts + (hasRatings || isGuarenteed ? ";" : "") + getRateIndex(bond_quote_cache->rate_index, 0, GetCompanyId());
            }
            
        }

        // --- 上市日期 ---
        if (bond_quote_cache->market_type == "1") // 针对一级半导出上市日期, TP只导出MMDD
        {
            std::string listed_date = IntToString(bond_quote_cache->listed_date);  
            if (listed_date.length() == 8)
            {
                quotesTexts = quotesTexts + (hasRatings || isGuarenteed || isFloating ? ";" : "") + listed_date.substr(4, 4);
            }        
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondQuoteBusiness::CopyReferredQuoteForTP(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (int i = 0; i < quote_ids.Size(); ++i)
    {
        BondQuoteReferCacheController bondquoterefer_cc;
        dbQuery q;
        sdbus::string quote_id = quote_ids.Get(i)->ToString().c_str();
        q = "id = ", quote_id, " and company_id = ", GetCompanyId(); //Modify by Young 05-Apr-2016
        BondQuoteReferCachePtr bond_quote_cache = bondquoterefer_cc.getCacheByQueryInThreadSafty(q);

        if (bond_quote_cache == NULL)
        {
            BondQuoteCacheController bondquote_cc;
            BondQuoteCachePtr quote_cache = bondquote_cc.getCacheByQueryInThreadSafty(q);
            if (quote_cache != NULL) {
                bond_quote_cache = BondQuoteReferCache::fromBondQuoteCache(quote_cache.get());
            }

            if (bond_quote_cache == NULL || bond_quote_cache->internally != kQuoteInternal)
            {
                continue;
            }      
        }

        // --- 剩余期限 ---
        quotesTexts = quotesTexts + getExpTTM(bond_quote_cache->time_to_maturity, bond_quote_cache->maturity_holidays, 0) + "    ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(bond_quote_cache->bond_short_name, 0) + "  ";
        }

        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id, 0) + "  ";

        // --- 价格(备注) ---
        std::string sPrice = getExpPrice(bond_quote_cache->price, bond_quote_cache->rebate, bond_quote_cache->return_point, bond_quote_cache->symbol, 3);
        quotesTexts = quotesTexts + sPrice + "  "; 

        // --- 报价类型 ---
        if (bond_quote_cache->symbol == 1) // Bid
        {
            quotesTexts = quotesTexts + "bid" + "  ";
        }
        else
        {
            quotesTexts = quotesTexts + "ofr" + "  ";
        }
   
        if ( bond_quote_cache->bond_category == "BNC" )
        {
            if (bond_quote_cache->bond_type == GBKToUTF8("地方债")) {
                quotesTexts = quotesTexts + bond_quote_cache->bond_short_name + "; ";
                quotesTexts = quotesTexts + getCouponRate(bond_quote_cache->coupon_rate)+"  ";
            }
        }    

        // --- 交易操作 ---
        quotesTexts = quotesTexts + "ref" + "\n";

    }

    return quotesTexts;
}
