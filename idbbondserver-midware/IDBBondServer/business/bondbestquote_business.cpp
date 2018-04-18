/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Best Quote Business
====================================================================================================================
   Date       Name                                  Description of Change
20-Jun-2014   Huameng Jia                           Initial version
19-Aug-2014   James Xu                              Add CopyBestQuote and CopyBestQuoteForTP
19-Aug-2014   James Xu                              Add CopyInternalBestQuote
03-Sep-2-14   James Xu                              Fix the issue that can't copy "no price" quote in best quote tableview
30-Sep-2014   James Xu                              Add CopyInternalBestQuoteForICAP
04-Dec-2014   James Xu                              Press [Alt] to export normal best quotes including internal best quotes
02-Mar-2015   James Xu                              Fix Issue BOND-613: no price notes when copying best quotes
06-Mar-2015   James Xu                              Fix Issue BOND-609: 最优报价有多个时，导出时，量需要叠加
23-May-2015   Huameng Jia                           Fix Issue: 暗盘导出交易量错误
27-May-2015   Huameng Jia                           导出最优报价里去除BondQuoteCache相关操作, 以免导出最优操作受到报价操作影响
$HISTORY$
===================================================================================================================*/
#include <sstream>
#include "bondbestquote_business.h"
#include "bondbestquotecalc_business.h"
#include "db/dbconnector.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/text_export.h"
#include "include/constants.h"
#include "bondbestquotecalc_business.h"
#include "config.h"

BondBestQuoteBusiness::BondBestQuoteBusiness(BondBestQuoteService* service)
    : service_(service)
{
    Init();
}

void BondBestQuoteBusiness::Init()
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
    SET_FID_MAP(FID_COMPLETED_QUOTE_SORT, bond_type_sort_client)
    SET_FID_MAP(FID_COMPLETED_BOND_TYPE, completed_bond_type)
    SET_FID_MAP(FID_HAVING_QUOTE, having_quote)
    SET_FID_MAP(FID_TWO_SIDED, two_sided)
    SET_FID_MAP(FID_HAVING_EXTERNAL_QUOTE, having_external_quote);
    SET_FID_MAP(FID_HAVING_INTERNAL_QUOTE, have_internal_quote);
    SET_FID_MAP(FID_HAVING_BID_QUOTE, having_bid_quote)
    SET_FID_MAP(FID_HAVING_OFR_QUOTE, having_ofr_quote)
    SET_FID_MAP(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market)
    SET_FID_MAP(FID_MATURITY, maturity_date)
	SET_FID_MAP(FID_FILTER_MATURITY, filter_maturity_date)
	SET_FID_MAP(FID_LISTED_DATE, listed_date)
	SET_FID_MAP(FID_REDEMPTION_NO_DESCRIPTION, redemption_no_client)
    SET_FID_MAP(FID_BID_OFR_SPREAD, bid_ofr_spread)
    SET_FID_MAP(FID_CDC_OFFSET, cdc_offset)

    SET_FID_MAP(FID_BID_QUOTE_TYPE, bid_quote_type);
    SET_FID_MAP(FID_OFR_QUOTE_TYPE, ofr_quote_type);
    SET_FID_MAP(FID_BID_YIELD_TYPE, bid_exercise);
    SET_FID_MAP(FID_OFR_YIELD_TYPE, ofr_exercise);
    SET_FID_MAP(FID_ISSUE_YEAR, issue_year);
    SET_FID_MAP(FID_MATURITY_HOLIDAYS, maturity_holidays);

    // 全局搜索过滤
    SET_FID_MAP(FID_BID_INSTITUTION_ID, bid_institution_id);
    SET_FID_MAP(FID_OFR_INSTITUTION_ID, ofr_institution_id);
    SET_FID_MAP(FID_BID_TRADER_ID, bid_trader_id);
    SET_FID_MAP(FID_OFR_TRADER_ID, ofr_trader_id);
    SET_FID_MAP(FID_BID_BROKER_ID, bid_broker_id);
    SET_FID_MAP(FID_OFR_BROKER_ID, ofr_broker_id);
    SET_FID_MAP(FID_BROKER_ID, broker_id_list);
    SET_FID_MAP(FID_ACROSS_MARKET_BOND_CODES, across_market_bond_ids);
    SET_FID_MAP(FID_TRADER_ID, trader_id_list);

    // 排序
    SET_FID_MAP(FID_TIME_TO_MATURITY_REAL, time_to_maturity_real)
    SET_FID_MAP(FID_BOND_CODE, bond_id)
    SET_FID_MAP(FID_BEST_BID_PRICE, bid_price)
    SET_FID_MAP(FID_BEST_OFR_PRICE, ofr_price)
    SET_FID_MAP(FID_BID_PRICE, bid_price)
    SET_FID_MAP(FID_OFR_PRICE, ofr_price)
    SET_FID_MAP(FID_BEST_BID_VOLUME, bid_volume)
    SET_FID_MAP(FID_BEST_OFR_VOLUME, ofr_volume)
    SET_FID_MAP(FID_BEST_BID_VOLUME_CLIENT, vol_bid_client)
    SET_FID_MAP(FID_BEST_OFR_VOLUME_CLIENT, vol_ofr_client)
    SET_FID_MAP(FID_BID_BROKER_NAME, bid_broker_name)
    SET_FID_MAP(FID_OFR_BROKER_NAME, ofr_broker_name)
    SET_FID_MAP(FID_ISSUER_RATING_ORDER, issuer_rating_order)
    SET_FID_MAP(FID_ISSUER_RATING_NPY_ORDER, issuer_rating_npy_order)
    SET_FID_MAP(FID_BOND_RATING_ORDER, bond_rating_order)
    SET_FID_MAP(FID_BOND_RATING_NPY_ORDER, bond_rating_npy_order)
    SET_FID_MAP(FID_CREATE_TIME, create_time)
    SET_FID_MAP(FID_DEAL_TIME, deal_time)
    SET_FID_MAP(FID_CDC_VALUATIONS_YIELD, cdc_valuation_yield)
    SET_FID_MAP(FID_CDC_VALUATIONS_CLEAN_PRICE, cdc_valuation_net_price)
    SET_FID_MAP(FID_BID_INSTITUTION_NAME, bid_institution_name)
    SET_FID_MAP(FID_OFR_INSTITUTION_NAME, ofr_institution_name)
    SET_FID_MAP(FID_BID_TRADER_NAME, bid_trader_name)
    SET_FID_MAP(FID_OFR_TRADER_NAME, ofr_trader_name)
    SET_FID_MAP(FID_OPTION_TYPE, option_client)
    SET_FID_MAP(FID_BEST_BID_CLEAN_PRICE, bid_net_price)
    SET_FID_MAP(FID_BEST_BID_DIRTY_PRICE, bid_full_price)
    SET_FID_MAP(FID_BEST_BID_SPREAD, bid_spread)
    SET_FID_MAP(FID_BEST_OFR_CLEAN_PRICE, ofr_net_price)
    SET_FID_MAP(FID_BEST_OFR_DIRTY_PRICE, ofr_full_price)
    SET_FID_MAP(FID_BEST_OFR_SPREAD, ofr_spread)
}

BondBestQuoteCacheVecPtr BondBestQuoteBusiness::FetchBondBestQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range)
{
	std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, range, "create_time desc"); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondBestQuoteCacheController bondBestQuoteCC;
    return bondBestQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondBestQuoteCacheVecPtr BondBestQuoteBusiness::FetchBondBestQuoteById(const std::string& bond_key_listed_market)
{
    std::string str = "bond_key_listed_market = '" + bond_key_listed_market + "'" + GetCompanyConditionStringWithPreAnd(); //Add by Young 05-Apr-2016
    dbQuery query(str.c_str());

    BondBestQuoteCacheController bondBestQuoteCC;
    return bondBestQuoteCC.getCacheListByQueryInThreadSafty(query);
}

void BondBestQuoteBusiness::FetchBondBestQuoteId(BondBestQuoteCache* cache, void* param)
{
    if (param == NULL) {
        return;
    }

    std::vector<std::string>* vec = (std::vector<std::string>*)param;
    vec->push_back(cache->bond_key_listed_market);
}

int BondBestQuoteBusiness::GetBondBestQuotesCount(const sdbus::string& cond)
{
	std::string str = GetFastdbQueryWithCompanyCondition(cond, "", ""); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());

    BondBestQuoteCacheController bondBestQuoteCC;
    return bondBestQuoteCC.getSizeByQueryInThreadSafty(query);
}

std::vector<std::string> BondBestQuoteBusiness::GetBondBestQuoteIdList(const sdbus::string& cond, const sdbus::string& sortby)
{
    std::vector<std::string> vec;

	std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, ""); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());

    BondBestQuoteCacheController bondBestQuoteCC;

    bondBestQuoteCC.traverseCacheByQueryInThreadSafty(query, FetchBondBestQuoteId, &vec);

    return vec;
}

bool BondBestQuoteBusiness::UpdateBestQuote(std::vector<std::string>& bond_key_listed_market_vec, bool update_across_market_bond /*= false*/)
{
    //std::vector<std::string> bond_key_vec;
    //for(std::vector<std::string>::iterator it = bond_key_listed_market_vec.begin(); it != bond_key_listed_market_vec.end(); ++it)
    //{
    //    std::string bond_key = it->length() > 3 ? it->substr(0, it->length() - 3) : "";
    //    bond_key_vec.push_back(bond_key);
    //}
    //if(!bond_key_vec.empty())
    //    thread_sync::singleton()->thread_sync_begin_best_quote_calc_and_publish(GetCompanyId(), bond_key_vec);


    BondBestQuoteCalcBusiness best_quote_calc_business;
	best_quote_calc_business.SetCompanyId(GetCompanyId());; //Add by Young 05-Apr-2016
    std::vector<std::string> across_market_bond_key_listed_market_vec;
    std::vector<BondBestQuoteCachePtr> bond_best_quote_vec;
    std::vector<std::string>::const_iterator it = bond_key_listed_market_vec.begin();
    for ( ; it != bond_key_listed_market_vec.end(); ++it) {
        std::string bond_key_listed_market = *it;
        std::string bond_key = bond_key_listed_market.length() > 3 ? bond_key_listed_market.substr(0, bond_key_listed_market.length() - 3) : "";
        if (update_across_market_bond && bond_key.length() > 0) {
            std::vector<BondBestQuoteCachePtr> across_market_vec = best_quote_calc_business.UpdateTime(bond_key, bond_key_listed_market_vec);
            for (std::vector<BondBestQuoteCachePtr>::const_iterator it = across_market_vec.begin(); it != across_market_vec.end(); ++it) {
                bond_best_quote_vec.push_back(*it);
                across_market_bond_key_listed_market_vec.push_back((*it)->bond_key_listed_market);
            }
        }

        BondBestQuoteCachePtr bond_best_quote_cache = best_quote_calc_business.MakeBestPrice(*it);
        bond_best_quote_vec.push_back(bond_best_quote_cache);
    }

    std::vector<std::string>::const_iterator it2 = across_market_bond_key_listed_market_vec.begin();
    for (; it2 != across_market_bond_key_listed_market_vec.end(); ++it2) {
        bond_key_listed_market_vec.push_back((*it2));
    }

    if (bond_best_quote_vec.size() > 0) {        
        service_->PublishBondBestQuote(bond_best_quote_vec, "UpdateList");
        
        Config::singleton()->load("TPRFA");
        std::string enable = Config::singleton()->getValue("TPRFA.Enable", "false");    
        std::transform(enable.begin(), enable.end(), enable.begin(), ::tolower);
    
        if (enable == "true") {  
		    service_->PublishBondBestQuoteToTPRFA(bond_best_quote_vec);
        }
    }

    //if(!bond_key_vec.empty())
    //    thread_sync::singleton()->thread_sync_ennnd_best_quote_calc_and_publish(GetCompanyId(), bond_key_vec);


    if (bond_best_quote_vec.size() > 0) {
        bool ret = InsertUpdateIntoDiskDB(bond_best_quote_vec);

        return ret;
    }

    return false;
}

bool BondBestQuoteBusiness::InsertUpdateIntoDiskDB(const std::vector<BondBestQuoteCachePtr> &bond_best_quote_vec, bool referAllBondByCompany/* = false*/)
{
    if(referAllBondByCompany) {
        std::string sqlStr = "update " + BondBestQuoteCache::TABLE_NAME + " set status = '0' where " + GetCompanyConditionString();
        //LOGGER_INFO(sqlStr);
        DBConnector::getInstance()->executeUpdate(sqlStr, BondBestQuoteCache::SCHEMA_NAME);
    }

    std::vector<BondBestQuoteCachePtr> newBestQuoteVec;
    LOGGER_INFO("Begin update bond_best_quote..");
    for (std::vector<BondBestQuoteCachePtr>::const_iterator it = bond_best_quote_vec.begin(); it != bond_best_quote_vec.end(); ++it) {
        if(!referAllBondByCompany)
        {
            // refer best quote in disk db
            BondBestQuoteCache newCacheForUpdate = *(it->get());
            newCacheForUpdate.status = "0";
            std::vector<std::string> conditionVec;
            conditionVec.push_back("bond_key_listed_market");
            conditionVec.push_back("company_id");
            updateDatabase(BondBestQuoteCache::TABLE_NAME, BondBestQuoteCache::SCHEMA_NAME, &newCacheForUpdate, it->get(), &conditionVec);
        }

        // add new best quote in disk db
        BondBestQuoteCachePtr newCacheForInsert(new BondBestQuoteCache(*(it->get())));
        newCacheForInsert->id = GetNewGuid();
        newBestQuoteVec.push_back(newCacheForInsert);
        //insertDatabase(BondBestQuoteCache::TABLE_NAME, BondBestQuoteCache::SCHEMA_NAME, &newCacheForInsert);
    }
    LOGGER_INFO("End update bond_best_quote");

    bool ret = insertDatabase(newBestQuoteVec, 500);
    return ret;
}

sdbus::string BondBestQuoteBusiness::CopyBestQuote(const std::vector<std::string>& bond_key_listed_market_vec)
{
    sdbus::VariantList list;

    std::vector<std::string>::const_reverse_iterator it = bond_key_listed_market_vec.rbegin();
    for ( ; it != bond_key_listed_market_vec.rend(); ++it) {
        sdbus::Variant* var = list.AddVariant();
        var->SetString(*it);
    }

    return CopyBestQuote(list);
}

sdbus::string BondBestQuoteBusiness::CopyInternalBestQuote(const std::vector<std::string>& bond_key_listed_market_vec)
{
    sdbus::VariantList list;

    std::vector<std::string>::const_reverse_iterator it = bond_key_listed_market_vec.rbegin();
    for ( ; it != bond_key_listed_market_vec.rend(); ++it) {
        sdbus::Variant* var = list.AddVariant();
        var->SetString(*it);
    }

    return CopyInternalBestQuote(list);
}

sdbus::string BondBestQuoteBusiness::CopyBestQuote(sdbus::VariantList quote_ids) 
{
    /*sdbus::string quoteString;
	if (GetCompanyId() == kCompanyTP)
        quoteString =  CopyBestQuoteForTP(quote_ids);
    else if(GetCompanyId() == kCompanyICAP)
        quoteString = CopyBestQuoteForICAP(quote_ids);
    else if(GetCompanyId() == kCompanyBGC)
        quoteString = CopyBestQuoteForBGC(quote_ids);
    else if(GetCompanyId() == kCompanyPATR)
        quoteString = CopyBestQuoteForPATR(quote_ids);
	else
		quoteString = CopyBestQuoteForTP(quote_ids);

    return quoteString;*/
    return CopyBestQuoteForTP(quote_ids);
}

sdbus::string BondBestQuoteBusiness::CopyBestQuoteAll(sdbus::VariantList quote_ids_all) 
{
    //最优报价的id序列转换为基本报价bid ofr交叉序列
    sdbus::VariantList quote_ids;
    BondBestQuoteCacheController best_quote_cc;
    dbQuery q;
    sdbus::Variant bid_var, ofr_var;
    BondBestQuoteCachePtr best_quote_cache;
    for (int i = 0; i < quote_ids_all.Size(); ++i){
        std::string best_quote_id = quote_ids_all.Get(i)->ToString().c_str();
        std::string str = "id = '" + best_quote_id + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016;
        q = str.c_str();
        best_quote_cache = best_quote_cc.getCacheByQueryInThreadSafty(q);
        if (best_quote_cache != NULL) {
            bid_var.SetString(best_quote_cache->bid_offer_id);
            ofr_var.SetString(best_quote_cache->bid_offer_id);
            quote_ids.PushBack(bid_var);
            quote_ids.PushBack(ofr_var);
        }
    }
    return CopyBestQuote(quote_ids);
}


sdbus::string BondBestQuoteBusiness::CopyInternalBestQuote(sdbus::VariantList quote_ids) 
{
    /*sdbus::string quoteString;
    if (GetCompanyId() == kCompanyTP)
        quoteString =  CopyInternalBestQuoteForTP(quote_ids);
    else if(GetCompanyId() == kCompanyICAP)
        quoteString = CopyInternalBestQuoteForICAP(quote_ids);
    else if(GetCompanyId() == kCompanyBGC)
        quoteString = CopyInternalBestQuoteForBGC(quote_ids);
    else if(GetCompanyId() == kCompanyPATR)
        quoteString = CopyInternalBestQuoteForPATR(quote_ids);
	else
		quoteString = CopyInternalBestQuoteForTP(quote_ids);

    return quoteString;*/
    return CopyInternalBestQuoteForTP(quote_ids);
}

// Copy最优（外部）报价
sdbus::string BondBestQuoteBusiness::CopyBestQuoteForTP(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (int i = 0; i < quote_ids.Size(); ++i)
    {
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        std::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", GetCompanyId(); //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }

        // 双边都是内部报价
        if (best_quote_cache->bid_offer_id == "" && best_quote_cache->ofr_offer_id == "") 
        {
            continue;
        }

        // --- 剩余期限 ---
        quotesTexts = quotesTexts + getExpTTM(best_quote_cache->time_to_maturity, best_quote_cache->maturity_holidays) + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id) + " ";

        // --- 价格(备注) ---
        std::string bidPrice;
        std::string bidNotes;
        std::vector<std::string> bid_price_notes;
        std::vector<std::string> bid_price_volumes;
        BondBestQuoteCalcBusiness bond_best_quote_calc_business;
		bond_best_quote_calc_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        std::vector<BondQuoteCachePtr> bid_quotes_list = bond_best_quote_calc_business.GetBestQuoteBid(bond_key_listed_market);  
        if (best_quote_cache->bid_offer_id != "")
        {
            bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, kBidQuote);
       
            if (bid_quotes_list.size() == 1)
            {
                BondQuoteCachePtr bond_quote_cache = bid_quotes_list[0];
                if (bond_quote_cache != NULL)
                {
                    bidNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                }
            }
            else
            {
                for (int i = 0; i < bid_quotes_list.size(); ++i)
                {
                    BondQuoteCachePtr bond_quote_cache = bid_quotes_list[i];
                    if (bond_quote_cache == NULL)
                    {
                        continue;
                    }

                    std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                    bid_price_notes.push_back(sNotes);
					bid_price_volumes.push_back(bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4));
                }
            }

        }

        std::string ofrPrice;
        std::string ofrNotes;
        std::vector<std::string> ofr_price_notes;
        std::vector<std::string> ofr_price_volumes;
        std::vector<BondQuoteCachePtr> ofr_quotes_list = bond_best_quote_calc_business.GetBestQuoteOfr(bond_key_listed_market);  
        if (best_quote_cache->ofr_offer_id != "")
        {
            ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, kOfrQuote);
         
            if (ofr_quotes_list.size() == 1)
            {
                BondQuoteCachePtr bond_quote_cache = ofr_quotes_list[0];
                if (bond_quote_cache != NULL)
                {
                    ofrNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                }
            }
            else
            {
                for (int i = 0; i < ofr_quotes_list.size(); ++i)
                {
                    BondQuoteCachePtr bond_quote_cache = ofr_quotes_list[i];
                    if (bond_quote_cache == NULL)
                    {
                        continue;
                    }
                    std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                    ofr_price_notes.push_back(sNotes);
                    ofr_price_volumes.push_back(bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4));
                }
            }
        }

        std::string priceAndNotes;
        if (bid_quotes_list.size() > 1 && ofr_quotes_list.size() > 1)
        {           
            // --- 价格 ---
            priceAndNotes = getExpPriceAndNotes(bidPrice, "", ofrPrice, "");
            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            std::string bid_volume;
            for (int i = 0; i < bid_price_notes.size(); ++i)
            {
                bid_volume = bid_volume + (bid_volume.empty()? "" : "+") + bid_price_volumes[i] + bid_price_notes[i];
            }

            std::string ofr_volume;
            for (int i = 0; i < ofr_price_notes.size(); ++i)
            {
                ofr_volume = ofr_volume + (ofr_volume.empty() ? "" : "+") + ofr_price_volumes[i] + ofr_price_notes[i];
            }

            quotesTexts = quotesTexts + bid_volume + "/" + ofr_volume + "    "; // 量后面空4个
        }
        else if (bid_quotes_list.size() <= 1 && ofr_quotes_list.size() > 1)
        {
            // --- 价格 ---
            priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, "");
            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            std::string ofr_volume;
            for (int i = 0; i < ofr_price_notes.size(); ++i)
            {
                ofr_volume = ofr_volume + (ofr_volume.empty() ? "" : "+") + ofr_price_volumes[i] + ofr_price_notes[i];
            }

            quotesTexts = quotesTexts + best_quote_cache->bid_volume + "/" + ofr_volume + "    "; // 量后面空4个
        }
        else if (bid_quotes_list.size() > 1 && ofr_quotes_list.size() <= 1)
        {
            // --- 价格 ---
            priceAndNotes = getExpPriceAndNotes(bidPrice, "", ofrPrice, ofrNotes);
            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            std::string bid_volume;
            for (int i = 0; i < bid_price_notes.size(); ++i)
            {
                bid_volume = bid_volume + (bid_volume.empty() ? "" : "+") + bid_price_volumes[i] + bid_price_notes[i];
            }

            quotesTexts = quotesTexts + bid_volume + "/" + best_quote_cache->ofr_volume + "    "; // 量后面空4个
        }

        else
        {
            priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, ofrNotes);

            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            quotesTexts = quotesTexts + best_quote_cache->bid_volume + "/" + best_quote_cache->ofr_volume + "    "; // 量后面空4个
        }
         
		if ( best_quote_cache->bond_category == "BNC" )
		{
			if (best_quote_cache->bond_type == GBKToUTF8("地方债")) {
				quotesTexts = quotesTexts + best_quote_cache->bond_short_name + "; ";
				quotesTexts = quotesTexts + getCouponRate(best_quote_cache->coupon_rate)+" ";
			}
		}

        // --- 评级 ----
        bool hasRatings = false;
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            hasRatings = true;
            quotesTexts = quotesTexts + getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating);
        }
        
        bool isGuarenteed = false;
        // --- 担保 ----
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            if (best_quote_cache->guarenteed == "Y")
            {
                isGuarenteed = true;
                quotesTexts = quotesTexts + (hasRatings ? ";" : "") + GBKToUTF8("有担保");
            }
        }

        bool isFloating = false;
        // --- 浮息 ----
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            isFloating = true;
            quotesTexts = quotesTexts + (hasRatings || isGuarenteed? ";" : "") + getRateIndex(best_quote_cache->rate_index, 0, GetCompanyId()) + GBKToUTF8("浮动");
        }

        // --- 上市日期 ---
        if (best_quote_cache->market_type == "1") // 针对一级半导出上市日期
        {
            std::string listed_date = IntToString(best_quote_cache->listed_date);   
            if (listed_date.length() == 8)
            {
                quotesTexts = quotesTexts + (hasRatings || isGuarenteed || isFloating ? ";" : "") + listed_date.substr(4, 4);
            }          
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondBestQuoteBusiness::CopyBestQuoteForICAP(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (int i = 0; i < quote_ids.Size(); ++i)
    {
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        sdbus::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }

        BondQuoteCacheController bond_quote_cc;
        q = "id = ", best_quote_cache->internally_bid_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->internally_ofr_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->bid_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->ofr_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        // --- 剩余期限 ---
        std::string ttm;
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            ttm = getExpTTM(best_quote_cache->time_to_maturity, 8);
        }
        else
        {
            ttm = best_quote_cache->time_to_maturity;
        }

        std::transform (ttm.begin(), ttm.end(), ttm.begin(), ::tolower);
        quotesTexts =  quotesTexts + ttm + " ";

        // --- 浮息 ----
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            if (isBCOCategoryType(best_quote_cache->bond_category))
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index, 8, kCompanyICAP) + " ";
            }
            else
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index, 0, kCompanyICAP) + " ";
            }          
        }

        // --- 代码 ---
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id, 10) + " ";
        }
        else
        {
            quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id, 0) + " ";
        }

        // --- 信用债::简称 
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        // --- 评级 ----
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            std::string ratings= getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating, kCompanyICAP);
            std::transform (ratings.begin(), ratings.end(), ratings.begin(), ::tolower);
            quotesTexts = quotesTexts + ratings + " ";
        }

        // --- 价格(备注) 交易量---
        std::string bidPrice;
        std::string bidNotes;
        double bidVolume = DOUBLE_NULL;
        if (best_quote_cache->bid_offer_id != "" && ext_bid_quote_cache != NULL)
        {
            bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, kBidQuote);
            bidNotes = getExpNotes(best_quote_cache->bid_flag_bargain, best_quote_cache->bid_flag_relation, ext_bid_quote_cache->price_description, kCompanyICAP); 
            bidVolume = boost::numeric_cast<double>(ext_bid_quote_cache->volume);
        }

        std::string ofrPrice;
        std::string ofrNotes;
        double ofrVolume = DOUBLE_NULL;
        if (best_quote_cache->ofr_offer_id != "" && ext_ofr_quote_cache != NULL)
        {
            ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, kOfrQuote);
            ofrNotes = getExpNotes(best_quote_cache->ofr_flag_bargain, best_quote_cache->ofr_flag_relation, ext_ofr_quote_cache->price_description, kCompanyICAP); 
            ofrVolume = boost::numeric_cast<double>(ext_ofr_quote_cache->volume);
        }

        std::string priceNotesVolume = getExpPriceNotesVolume(bidPrice, bidNotes, bidVolume, ofrPrice, ofrNotes, ofrVolume);

        quotesTexts = quotesTexts + priceNotesVolume + " "; // 最优报价导出，价格后面空1格。

        // --- 还本方式 ---
        if (best_quote_cache->redemption_no > 0)
        {
            quotesTexts = quotesTexts + best_quote_cache->redemption_no_client;
        }
        
        // --- 担保 ----
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            if (best_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + (best_quote_cache->redemption_no > 0 ? ";" : "") + GBKToUTF8("有担保");
            }
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondBestQuoteBusiness::CopyBestQuoteForBGC(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (int i = 0; i < quote_ids.Size(); ++i)
    {
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        sdbus::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }

        BondQuoteCacheController bond_quote_cc;
        q = "id = ", best_quote_cache->internally_bid_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->internally_ofr_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->bid_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->ofr_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);
 
        // --- 新发 ---
        quotesTexts =  quotesTexts + (best_quote_cache->market_type == "1" ? GBKToUTF8("新发") + " " : ""); 

        // --- 剩余期限 ---
        quotesTexts =  quotesTexts + getExpTTM(best_quote_cache->time_to_maturity, best_quote_cache->maturity_holidays) + " ";

        // --- 浮息 ----
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            if (isBCOCategoryType(best_quote_cache->bond_category))
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index + GBKToUTF8("浮动"), 17, kCompanyBGC) + " ";
            }
            else
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index, 13, kCompanyBGC) + " ";
            }       
        }

        // --- 代码(上市日期) ---
        quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id) + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        if (best_quote_cache->market_type == "1")
        {
            quotesTexts = quotesTexts + "("  + getExpListedDate(best_quote_cache->listed_date) + ")" + " ";
        }

        // --- 价格(备注) ---
        std::string bidPrice;
        std::string bidNotes;
        if (best_quote_cache->bid_offer_id != "" && ext_bid_quote_cache != NULL)
        {
            bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, kBidQuote);
            bidNotes = getExpNotes(best_quote_cache->bid_flag_bargain, best_quote_cache->bid_flag_relation, best_quote_cache->bid_price_description); 
        }

        std::string ofrPrice;
        std::string ofrNotes;
        if (best_quote_cache->ofr_offer_id != "" && ext_ofr_quote_cache != NULL)
        {
            ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, kOfrQuote);
            ofrNotes = getExpNotes(best_quote_cache->ofr_flag_bargain, best_quote_cache->ofr_flag_relation, best_quote_cache->ofr_price_description); 
        }

        std::string priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, ofrNotes);

        quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

        // --- 交易量 ---
        quotesTexts = quotesTexts + best_quote_cache->bid_volume + "/" + best_quote_cache->ofr_volume + "    "; // 量后面空4个

        // --- 评级 ----
        quotesTexts = quotesTexts + getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating);

        // --- 担保 ----
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            if (best_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + ";" + GBKToUTF8("有担保");
            }
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondBestQuoteBusiness::CopyBestQuoteForPATR(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (int i = 0; i < quote_ids.Size(); ++i)
    {
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        sdbus::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }

        if (best_quote_cache->bid_offer_id == "" && best_quote_cache->ofr_offer_id == "") 
        {
            continue;
        }

        BondQuoteCacheController bond_quote_cc;
        q = "id = ", best_quote_cache->internally_bid_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->internally_ofr_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->bid_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->ofr_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        // --- 剩余期限 ---
        quotesTexts =  quotesTexts + getExpTTMInMonth(best_quote_cache->time_to_maturity, 8, best_quote_cache->has_option == "Y") + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id) + " ";


        // --- 评级 ----
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating, kCompanyPATR) + " ";
        }

        // --- 价格(备注) ---
        std::string bidPrice;
        std::string bidNotes;
        if (best_quote_cache->bid_offer_id != "" && ext_bid_quote_cache != NULL)
        {
            bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, kBidQuote);
            bidNotes = getExpNotes(best_quote_cache->bid_flag_bargain, best_quote_cache->bid_flag_relation, best_quote_cache->bid_price_description); 
        }

        std::string ofrPrice;
        std::string ofrNotes;
        if (best_quote_cache->ofr_offer_id != "" && ext_ofr_quote_cache != NULL)
        {
            ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, kOfrQuote);
            ofrNotes = getExpNotes(best_quote_cache->ofr_flag_bargain, best_quote_cache->ofr_flag_relation, best_quote_cache->ofr_price_description); 
        }

        std::string priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, ofrNotes);

        quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

        // --- 交易量 ---
        quotesTexts = quotesTexts + best_quote_cache->bid_volume + "/" + best_quote_cache->ofr_volume + "    "; // 量后面空4个

        // --- 担保 ----
        bool isGuarenteed = false;
        if ( isBCOCategoryType(best_quote_cache->bond_category) &&  best_quote_cache->guarenteed == "Y")
        {
            isGuarenteed = true;
            quotesTexts = quotesTexts + GBKToUTF8("有担保");
        }

        // --- 浮息 ----
        bool isFloating = false;
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            isFloating = true;
            quotesTexts = quotesTexts +  (isGuarenteed ? ";" : "") + getRateIndex(best_quote_cache->rate_index, 0, kCompanyPATR) + GBKToUTF8("浮动");
        }

        // --- 上市日期 ---
        if (best_quote_cache->market_type == "1") // 针对一级半导出上市日期
        {
            quotesTexts = quotesTexts + (isGuarenteed || isFloating ? ";" : "") + getExpListedDate(best_quote_cache->listed_date);
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondBestQuoteBusiness::CopyInternalBestQuoteForTP(sdbus::VariantList quote_ids) 
{
    sdbus::string internalTitle = GBKToUTF8("暗盘");
    sdbus::string quotesTexts;

    sdbus::VariantMap mp;
    for (int i = 0; i < quote_ids.Size(); ++i)
    {        
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        std::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", GetCompanyId(); //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }
         
        // [James Xu, 2014-12-19] 按住Alt键, 刷出最优报价（包含内部报价），并标明暗盘    
        // [Huameng, 2015-09-14] 选中导出的最优报价（暗盘）需包含无报价的券。注意此处和MarketDataServer下的导出不一样!!
        if (best_quote_cache->bid_offer_id == "" && best_quote_cache->internally_bid_offer_id == "" && best_quote_cache->ofr_offer_id == "" && best_quote_cache->internally_ofr_offer_id == "") // 外部/外部
        {
            continue;
        }

        // --- 增加暗盘标识 ---
        if (isBCOCategoryType(best_quote_cache->bond_category))
        {
            quotesTexts = quotesTexts +  internalTitle + "  ";
        }
        
        // --- 剩余期限 ---
        quotesTexts = quotesTexts + getExpTTM(best_quote_cache->time_to_maturity, best_quote_cache->maturity_holidays) + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id) + " ";

        //------------------------------------------------------------
        // --- 价格(备注) ---
        std::string bidPrice;
        std::string bidNotes;
        std::string bidVolume = "--";
        std::vector<std::string> bid_price_notes;
        std::vector<std::string> bid_price_volumes;
        BondBestQuoteCalcBusiness bond_best_quote_calc_business;
        bond_best_quote_calc_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        std::vector<BondQuoteCachePtr> bid_quotes_list = bond_best_quote_calc_business.GetBestQuoteBidWithInternal(bond_key_listed_market);  
        if (best_quote_cache->internally_bid_offer_id != "")
        {
            bidPrice = getExpPrice(best_quote_cache->internally_bid_price, best_quote_cache->internally_bid_rebate, best_quote_cache->internally_bid_return_point, kBidQuote);
       
            if (bid_quotes_list.size() == 1)
            {
                BondQuoteCachePtr bond_quote_cache = bid_quotes_list[0];
                bidNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description);  
                bidVolume = bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4);
            }
            else
            {
                for (int i = 0; i < bid_quotes_list.size(); ++i)
                {
                    BondQuoteCachePtr bond_quote_cache = bid_quotes_list[i];
                    if (bond_quote_cache == NULL)
                    {
                        continue;
                    }
                    std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                    bid_price_notes.push_back(sNotes);
                    bid_price_volumes.push_back(bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0,  4));
                }
            }

        }
        else if (best_quote_cache->bid_offer_id != "")
        {
            bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, kBidQuote);
            if (bid_quotes_list.size() == 1)
            {
                BondQuoteCachePtr bond_quote_cache = bid_quotes_list[0];
                bidNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description);
                bidVolume = bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0? "--" : Format(bond_quote_cache->volume, 0, 4);
            }
            else
            {
                for (int i = 0; i < bid_quotes_list.size(); ++i)
                {
                    BondQuoteCachePtr bond_quote_cache = bid_quotes_list[i];
                    if (bond_quote_cache == NULL)
                    {
                        continue;
                    }

                    std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                    bid_price_notes.push_back(sNotes);
                    bid_price_volumes.push_back(bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4));
                }
            }
        }

        std::string ofrPrice;
        std::string ofrNotes;
        std::string ofrVolume = "--";
        std::vector<std::string> ofr_price_notes;
        std::vector<std::string> ofr_price_volumes;
        std::vector<BondQuoteCachePtr> ofr_quotes_list = bond_best_quote_calc_business.GetBestQuoteOfrWithInternal(bond_key_listed_market);  
        if (best_quote_cache->internally_ofr_offer_id != "")
        {
            ofrPrice = getExpPrice(best_quote_cache->internally_ofr_price, best_quote_cache->internally_ofr_rebate, best_quote_cache->internally_ofr_return_point, kOfrQuote);
       
            if (ofr_quotes_list.size() == 1)
            {
                BondQuoteCachePtr bond_quote_cache = ofr_quotes_list[0];
                ofrNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description);   
                ofrVolume = bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4);
            }
            else
            {
                for (int i = 0; i < ofr_quotes_list.size(); ++i)
                {
                    BondQuoteCachePtr bond_quote_cache = ofr_quotes_list[i];
                    if (bond_quote_cache == NULL)
                    {
                        continue;
                    }
                    std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                    ofr_price_notes.push_back(sNotes);
                    ofr_price_volumes.push_back(bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4));
                }
            }

        }
        else if (best_quote_cache->ofr_offer_id != "")
        {
            ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, kOfrQuote);
            if (ofr_quotes_list.size() == 1)
            {
                BondQuoteCachePtr bond_quote_cache = ofr_quotes_list[0];
                ofrNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                ofrVolume = bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4);
            }
            else
            {
                for (int i = 0; i < ofr_quotes_list.size(); ++i)
                {
                    BondQuoteCachePtr bond_quote_cache = ofr_quotes_list[i];
                    if (bond_quote_cache == NULL)
                    {
                        continue;
                    }
                    std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
                    ofr_price_notes.push_back(sNotes);
                    ofr_price_volumes.push_back(bond_quote_cache->volume == DOUBLE_NULL || bond_quote_cache->volume == 0 ? "--" : Format(bond_quote_cache->volume, 0, 4));
                }
            }
        }

        std::string priceAndNotes;
        if (bid_quotes_list.size() > 1 && ofr_quotes_list.size() > 1)
        {           
            // --- 价格 ---
            priceAndNotes = getExpPriceAndNotes(bidPrice, "", ofrPrice, "");
            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            std::string bid_volume;
            for (int i = 0; i < bid_price_notes.size(); ++i)
            {
                bid_volume = bid_volume + (bid_volume.empty() ? "" : "+") + bid_price_volumes[i] + bid_price_notes[i];
            }

            std::string ofr_volume;
            for (int i = 0; i < ofr_price_notes.size(); ++i)
            {
                ofr_volume = ofr_volume + (ofr_volume.empty() ? "" : "+") + ofr_price_volumes[i] + ofr_price_notes[i];
            }

            quotesTexts = quotesTexts + bid_volume + "/" + ofr_volume + "    "; // 量后面空4个
        }
        else if (bid_quotes_list.size() <= 1 && ofr_quotes_list.size() > 1)
        {
            // --- 价格 ---
            priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, "");
            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            std::string ofr_volume;
            for (int i = 0; i < ofr_price_notes.size(); ++i)
            {
                ofr_volume = ofr_volume + (ofr_volume.empty() ? "" : "+") + ofr_price_volumes[i] + ofr_price_notes[i];
            }

            quotesTexts = quotesTexts + bidVolume + "/" + ofr_volume + "    "; // 量后面空4个
        }
        else if (bid_quotes_list.size() > 1 && ofr_quotes_list.size() <= 1)
        {
            // --- 价格 ---
            priceAndNotes = getExpPriceAndNotes(bidPrice, "", ofrPrice, ofrNotes);
            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            std::string bid_volume;
            for (int i = 0; i < bid_price_notes.size(); ++i)
            {
                bid_volume = bid_volume + (bid_volume.empty() ? "" : "+") + bid_price_volumes[i] + bid_price_notes[i];
            }

            quotesTexts = quotesTexts + bid_volume + "/" + ofrVolume + "    "; // 量后面空4个
        }

        else
        {
            priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, ofrNotes);

            quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

            // --- 交易量 ---
            quotesTexts = quotesTexts + bidVolume + "/" + ofrVolume + "    "; // 量后面空4个
        }

		if ( best_quote_cache->bond_category == "BNC" )
		{
			if (best_quote_cache->bond_type == GBKToUTF8("地方债")) {
				quotesTexts = quotesTexts + best_quote_cache->bond_short_name + "; ";
				quotesTexts = quotesTexts + getCouponRate(best_quote_cache->coupon_rate)+" ";
			}
		}  

        // --- 评级 ----
        bool hasRatings = false;
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            hasRatings = true;
            quotesTexts = quotesTexts + getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating);
        }

        // --- 担保 ----
        bool isGuarenteed = false;
        if ( isBCOCategoryType(best_quote_cache->bond_category) && best_quote_cache->guarenteed == "Y" )
        {
            isGuarenteed = true;
            quotesTexts = quotesTexts + (hasRatings ? ";" : "") + GBKToUTF8("有担保");
        }

        // --- 浮息 ----
        bool isFloating = false;
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            isFloating = true;
            quotesTexts = quotesTexts + (hasRatings || isGuarenteed ? ";" : "") + getRateIndex(best_quote_cache->rate_index, 0, GetCompanyId()) + GBKToUTF8("浮动");
        }

        // --- 上市日期 ---
        if (best_quote_cache->market_type == "1") // 针对一级半导出上市日期
        {
            std::string listed_date = IntToString(best_quote_cache->listed_date);   
            if (listed_date.length() == 8)
            {
                quotesTexts = quotesTexts + (hasRatings || isGuarenteed || isFloating ? ";" : "") + listed_date.substr(4, 4);
            }           
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondBestQuoteBusiness::CopyInternalBestQuoteForICAP(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    sdbus::VariantMap mp;
    for (int i = 0; i < quote_ids.Size(); ++i)
    {        
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        sdbus::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }

        if (best_quote_cache->internally_bid_offer_id == "" && best_quote_cache->internally_ofr_offer_id == "") // 外部/外部
        {
            continue;
        }

        BondQuoteCacheController bond_quote_cc;
        q = "id = ", best_quote_cache->internally_bid_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->internally_ofr_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->bid_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->ofr_offer_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        // --- 增加暗盘标识 ---
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + "  ";
        }
        
        // --- 剩余期限 ---
        std::string ttm;
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            ttm = getExpTTM(best_quote_cache->time_to_maturity, 8);
        }
        else
        {
            ttm = best_quote_cache->time_to_maturity;
        }

        std::transform (ttm.begin(), ttm.end(), ttm.begin(), ::tolower);
        quotesTexts =  quotesTexts + ttm + " ";

        // --- 浮息 ----
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            if (isBCOCategoryType(best_quote_cache->bond_category))
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index, 8, kCompanyICAP) + " ";
            }
            else
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index, 0, kCompanyICAP) + " ";
            }          
        }

        // --- 代码 ---
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id, 10) + " ";
        }
        else
        {
            quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id, 0) + " ";
        }

        // --- 信用债::简称 
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        // --- 评级 ----
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            std::string ratings= getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating, kCompanyICAP);
            std::transform (ratings.begin(), ratings.end(), ratings.begin(), ::tolower);
            quotesTexts = quotesTexts + ratings + " ";
        }

        // --- 价格(备注) 交易量---
        std::string bidPrice, ofrPrice;
        std::string bidNotes, ofrNotes;
        double bidVolume = DOUBLE_NULL, ofrVolume = DOUBLE_NULL;

        if (best_quote_cache->internally_bid_offer_id != "" && best_quote_cache->internally_ofr_offer_id != "") // 内部/内部
        {
            bidPrice = getExpPrice(int_bid_quote_cache->price, int_bid_quote_cache->rebate, int_bid_quote_cache->return_point, kBidQuote);
            bidNotes = getExpNotes(int_bid_quote_cache->flag_bargain, int_bid_quote_cache->flag_relation, int_bid_quote_cache->price_description, kCompanyICAP); 
            bidVolume = int_bid_quote_cache->volume;

            ofrPrice = getExpPrice(int_ofr_quote_cache->price, int_ofr_quote_cache->rebate, int_ofr_quote_cache->return_point, kOfrQuote);
            ofrNotes = getExpNotes(int_ofr_quote_cache->flag_bargain, int_ofr_quote_cache->flag_relation, int_ofr_quote_cache->price_description, kCompanyICAP); 
            ofrVolume = int_ofr_quote_cache->volume;
        }
        else if (best_quote_cache->internally_bid_offer_id != "") // 内部/(外部or空)
        {
            bidPrice = getExpPrice(int_bid_quote_cache->price, int_bid_quote_cache->rebate, int_bid_quote_cache->return_point, kBidQuote);
            bidNotes = getExpNotes(int_bid_quote_cache->flag_bargain, int_bid_quote_cache->flag_relation, int_bid_quote_cache->price_description, kCompanyICAP); 
            bidVolume = int_bid_quote_cache->volume;

            if (best_quote_cache->ofr_offer_id != "")
            {
                ofrPrice = getExpPrice(ext_ofr_quote_cache->price, ext_ofr_quote_cache->rebate, ext_ofr_quote_cache->return_point, kOfrQuote);
                ofrNotes = getExpNotes(ext_ofr_quote_cache->flag_bargain, ext_ofr_quote_cache->flag_relation, ext_ofr_quote_cache->price_description, kCompanyICAP); 
                ofrVolume = ext_ofr_quote_cache->volume;
            }
        }
        else if (best_quote_cache->internally_ofr_offer_id != "") // (外部or空)/内部
        {
            if (best_quote_cache->bid_offer_id != "")
            {
                bidPrice = getExpPrice(ext_bid_quote_cache->price, ext_bid_quote_cache->rebate, ext_bid_quote_cache->return_point, kBidQuote);
                bidNotes = getExpNotes(ext_bid_quote_cache->flag_bargain, ext_bid_quote_cache->flag_relation, ext_bid_quote_cache->price_description, kCompanyICAP); 
                bidVolume = ext_bid_quote_cache->volume;
            }
            
            ofrPrice = getExpPrice(int_ofr_quote_cache->price, int_ofr_quote_cache->rebate, int_ofr_quote_cache->return_point, kOfrQuote);
            ofrNotes = getExpNotes(int_ofr_quote_cache->flag_bargain, int_ofr_quote_cache->flag_relation, int_ofr_quote_cache->price_description, kCompanyICAP); 
            ofrVolume = int_ofr_quote_cache->volume;
        }


        std::string priceNotesVolume = getExpPriceNotesVolume(bidPrice, bidNotes, bidVolume, ofrPrice, ofrNotes, ofrVolume);

        quotesTexts = quotesTexts + priceNotesVolume + " "; // 最优报价导出，价格后面空1格。

        // --- 还本方式 ---
        if (best_quote_cache->redemption_no > 0)
        {
            quotesTexts = quotesTexts + best_quote_cache->redemption_no_client;
        }

        // --- 担保 ----
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            if (best_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + (best_quote_cache->redemption_no > 0 ? ";" : "") + GBKToUTF8("有担保");
            }
        }

        // --- 上市日期 ---
        if (best_quote_cache->market_type == "1") // 针对一级半导出上市日期
        {
            quotesTexts = quotesTexts + ";" + getExpListedDate(best_quote_cache->listed_date);
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondBestQuoteBusiness::CopyInternalBestQuoteForPATR(sdbus::VariantList quote_ids) 
{
    sdbus::string internalTitle = GBKToUTF8("暗盘");
    sdbus::string quotesTexts;

    sdbus::VariantMap mp;
    for (int i = 0; i < quote_ids.Size(); ++i)
    {        
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        sdbus::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }

        // [James Xu, 2014-12-4] 按住Alt键, 刷出最优报价（包含内部报价），并标明暗盘
        //if (best_quote_cache->internally_bid_offer_id == "" && best_quote_cache->internally_ofr_offer_id == "") // 外部/外部
        //{
        //    continue;
        //}

        BondQuoteCacheController bond_quote_cc;
        q = "id = ", best_quote_cache->internally_bid_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->internally_ofr_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->bid_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->ofr_offer_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        // --- 增加暗盘标识 ---
        quotesTexts = quotesTexts + internalTitle + "  ";

        // --- 剩余期限 ---
        quotesTexts =  quotesTexts + getExpTTMInMonth(best_quote_cache->time_to_maturity, 8, best_quote_cache->has_option == "Y") + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id) + " ";


        // --- 评级 ----
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating, kCompanyPATR) + " ";
        }

        // --- 价格(备注) ---
        std::string bidPrice;
        std::string bidNotes;
        if (best_quote_cache->internally_bid_offer_id != "" && int_bid_quote_cache != NULL) //
        {
            bidPrice = getExpPrice(best_quote_cache->internally_bid_price, best_quote_cache->internally_bid_rebate, best_quote_cache->internally_bid_return_point, kBidQuote);
            bidNotes = getExpNotes(int_bid_quote_cache->flag_bargain, int_bid_quote_cache->flag_relation, int_bid_quote_cache->price_description); 
        }
        else if (best_quote_cache->bid_offer_id != "")
        {

            bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, kBidQuote);
            bidNotes = getExpNotes(best_quote_cache->bid_flag_bargain, best_quote_cache->bid_flag_relation, best_quote_cache->bid_price_description); 
        }

        std::string ofrPrice;
        std::string ofrNotes;
        if (best_quote_cache->internally_ofr_offer_id != "" && int_ofr_quote_cache != NULL)
        {
            ofrPrice = getExpPrice(best_quote_cache->internally_ofr_price, best_quote_cache->internally_ofr_rebate, best_quote_cache->internally_ofr_return_point, kOfrQuote);
            ofrNotes = getExpNotes(int_ofr_quote_cache->flag_bargain, int_ofr_quote_cache->flag_relation, int_ofr_quote_cache->price_description); 
        }
        else if (best_quote_cache->ofr_offer_id != "")
        {
            ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, kOfrQuote);
            ofrNotes = getExpNotes(best_quote_cache->ofr_flag_bargain, best_quote_cache->ofr_flag_relation, best_quote_cache->ofr_price_description); 
        }

        std::string priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, ofrNotes);

        quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

        // --- 交易量 ---
        if (best_quote_cache->internally_bid_offer_id != "" && best_quote_cache->internally_ofr_offer_id != "") // 内部/内部
        {
            quotesTexts = quotesTexts + getExpBestVolume(int_bid_quote_cache->volume, int_ofr_quote_cache->volume) + "    "; // 量后面空4个
        }
        else if (best_quote_cache->internally_bid_offer_id != "") // 内部/外部
        {
            quotesTexts = quotesTexts + getExpBestVolume(int_bid_quote_cache->volume, ext_ofr_quote_cache == NULL ? 0 : ext_ofr_quote_cache->volume) + "    "; 
        }
        else if (best_quote_cache->internally_ofr_offer_id != "") // 外部/内部
        {
            quotesTexts = quotesTexts + getExpBestVolume(ext_bid_quote_cache == NULL ? 0 : ext_bid_quote_cache->volume, int_ofr_quote_cache->volume) + "    "; 
        }

        // --- 担保 ----
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            if (best_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + GBKToUTF8("有担保");
            }
        }

        // --- 浮息 ----
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            if ( isBCOCategoryType(best_quote_cache->bond_category) )
            {
                quotesTexts = quotesTexts + ";" + getRateIndex(best_quote_cache->rate_index, 0, kCompanyPATR) + GBKToUTF8("浮动");
            }
            else
            {
                quotesTexts = quotesTexts  + getRateIndex(best_quote_cache->rate_index, 0, kCompanyPATR);
            }
        }

        // --- 上市日期 ---
        if (best_quote_cache->market_type == "1") // 针对一级半导出上市日期
        {
            quotesTexts = quotesTexts + ";" + getExpListedDate(best_quote_cache->listed_date);
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondBestQuoteBusiness::CopyInternalBestQuoteForBGC(sdbus::VariantList quote_ids) 
{
    sdbus::string internalTitle = GBKToUTF8("暗盘");
    sdbus::string quotesTexts;

    sdbus::VariantMap mp;
    for (int i = 0; i < quote_ids.Size(); ++i)
    {        
        BondBestQuoteCacheController bond_best_quote_cc;
        dbQuery q;
        sdbus::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        q = "bond_key_listed_market =  ", bond_key_listed_market, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondBestQuoteCachePtr best_quote_cache = bond_best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (best_quote_cache == NULL)
        {
            continue;
        }

        if (best_quote_cache->internally_bid_offer_id == "" && best_quote_cache->internally_ofr_offer_id == "") // 外部/外部
        {
            continue;
        }

        BondQuoteCacheController bond_quote_cc;
        q = "id = ", best_quote_cache->internally_bid_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->internally_ofr_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr int_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->bid_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_bid_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        q = "id = ", best_quote_cache->ofr_offer_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr ext_ofr_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        // --- 增加暗盘标识 ---
        quotesTexts = quotesTexts + internalTitle + "  ";

        // --- 新发 ---
        quotesTexts =  quotesTexts + (best_quote_cache->market_type == "1" ? GBKToUTF8("新发") + " " : ""); 

        // --- 剩余期限 ---
        quotesTexts =  quotesTexts + getExpTTM(best_quote_cache->time_to_maturity, best_quote_cache->maturity_holidays) + " ";

        // --- 浮息 ----
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED")
        {
            if ( isBCOCategoryType(best_quote_cache->bond_category) )
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index + GBKToUTF8("浮动"), 17, kCompanyBGC) + " ";
            }
            else
            {
                quotesTexts = quotesTexts + getRateIndex(best_quote_cache->rate_index, 13, kCompanyBGC) + " ";
            }       
        }

        // --- 代码(上市日期) ---
        quotesTexts = quotesTexts + getExpBondCode(best_quote_cache->bond_id) + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(best_quote_cache->bond_short_name) + " ";
        }

        if (best_quote_cache->market_type == "1")
        {
            quotesTexts = quotesTexts + "("  + getExpListedDate(best_quote_cache->listed_date) + ")" + " ";
        }

        // --- 价格(备注) ---
        std::string bidPrice;
        std::string bidNotes;
        if (best_quote_cache->internally_bid_offer_id != "" && int_bid_quote_cache != NULL) //
        {
            bidPrice = getExpPrice(best_quote_cache->internally_bid_price, best_quote_cache->internally_bid_rebate, best_quote_cache->internally_bid_return_point, kBidQuote);
            bidNotes = getExpNotes(int_bid_quote_cache->flag_bargain, int_bid_quote_cache->flag_relation, int_bid_quote_cache->price_description); 
        }
        else if (best_quote_cache->bid_offer_id != "")
        {

            bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, kBidQuote);
            bidNotes = getExpNotes(best_quote_cache->bid_flag_bargain, best_quote_cache->bid_flag_relation, best_quote_cache->bid_price_description); 
        }

        std::string ofrPrice;
        std::string ofrNotes;
        if (best_quote_cache->internally_ofr_offer_id != "" && int_ofr_quote_cache != NULL)
        {
            ofrPrice = getExpPrice(best_quote_cache->internally_ofr_price, best_quote_cache->internally_ofr_rebate, best_quote_cache->internally_ofr_return_point, kOfrQuote);
            ofrNotes = getExpNotes(int_ofr_quote_cache->flag_bargain, int_ofr_quote_cache->flag_relation, int_ofr_quote_cache->price_description); 
        }
        else if (best_quote_cache->ofr_offer_id != "")
        {
            ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, kOfrQuote);
            ofrNotes = getExpNotes(best_quote_cache->ofr_flag_bargain, best_quote_cache->ofr_flag_relation, best_quote_cache->ofr_price_description); 
        }

        std::string priceAndNotes = getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, ofrNotes);

        quotesTexts = quotesTexts + priceAndNotes + " "; // 最优报价导出，价格后面空1格。

        // --- 交易量 ---
        if (best_quote_cache->internally_bid_offer_id != "" && best_quote_cache->internally_ofr_offer_id != "") // 内部/内部
        {
            quotesTexts = quotesTexts + getExpBestVolume(int_bid_quote_cache->volume, int_ofr_quote_cache->volume) + "    "; // 量后面空4个
        }
        else if (best_quote_cache->internally_bid_offer_id != "") // 内部/外部
        {
            quotesTexts = quotesTexts + getExpBestVolume(int_bid_quote_cache->volume, ext_ofr_quote_cache == NULL ? 0 : ext_ofr_quote_cache->volume) + "    "; 
        }
        else if (best_quote_cache->internally_ofr_offer_id != "") // 外部/内部
        {
            quotesTexts = quotesTexts + getExpBestVolume(ext_bid_quote_cache == NULL ? 0 : ext_bid_quote_cache->volume, int_ofr_quote_cache->volume) + "    "; 
        }

        // --- 评级 ----
        quotesTexts = quotesTexts + getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating);

        // --- 担保 ----
        if ( isBCOCategoryType(best_quote_cache->bond_category) )
        {
            if (best_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + ";" + GBKToUTF8("有担保");
            }
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}