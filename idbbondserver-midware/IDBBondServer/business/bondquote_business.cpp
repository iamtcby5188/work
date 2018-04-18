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
#include "dcsbonddeal_business.h"
#include "bondbestquote_business.h"
#include "calculator_business.h"
#include "db/dbconnector.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_center.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "include/text_export.h"
#include "connection_manager.h"
#include "service/service_manager.h"
#include "service/qb_sync_service.h"
#include "service/bondcompletedquote_service.h"
#include "boost/lexical_cast.hpp"
#include "msg2db/msg_log_service.h"
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
	SET_FID_MAP(FID_ISSUE_YEAR, issue_year)
    SET_FID_MAP(FID_SW_SECTOR_CODE, sw_subsector_code)
    SET_FID_MAP(FID_ISSUE_PROVINCE_CODE, province_code)
	SET_FID_MAP(FID_FINANCING_PLATFORM, financing_platform)
    SET_FID_MAP(FID_ID, issue_institution_id)
	SET_FID_MAP(FID_COMPANY_BOND, company_bond)
	SET_FID_MAP(FID_OUTLOOK, outlook) //Add by Young 20-Apr-2016
    
    SET_FID_MAP(FID_CDC_OFFSET, cdc_offset)
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

void BondQuoteBusiness::AssembleBondQuote(BondQuoteCachePtr& bondquotecache, ptree* bond_quote_msg) {
	if (bond_quote_msg != NULL) {
        bond_quote_msg->put("id", bondquotecache->id);
        bond_quote_msg->put("companyId", bondquotecache->company_id);
		bond_quote_msg->put("goodsId", bondquotecache->goods_id);
		bond_quote_msg->put("bondKey", bondquotecache->bond_key);
		bond_quote_msg->put("listedMarket", bondquotecache->listed_market);
		bond_quote_msg->put("bondKeyListedMarket", bondquotecache->bond_key_listed_market);
		bond_quote_msg->put("goodsCode", bondquotecache->goods_code);
		bond_quote_msg->put("profitCode", bondquotecache->profit_code);
		bond_quote_msg->put("bankName", bondquotecache->bank_name);
		bond_quote_msg->put("bankId", bondquotecache->bank_id);
        bond_quote_msg->put("operatorId", bondquotecache->operator_id);
		bond_quote_msg->put("description", bondquotecache->description);
		bond_quote_msg->put("symbol", bondquotecache->symbol);
        if (bondquotecache->price != DOUBLE_NULL) {
			bond_quote_msg->put("price", bondquotecache->price);
		}
        if (bondquotecache->volume != DOUBLE_NULL) {
            bond_quote_msg->put("volume", bondquotecache->volume);
        }		
		bond_quote_msg->put("dealStatus", bondquotecache->deal_status);
		bond_quote_msg->put("dealTime", bondquotecache->deal_time);
		bond_quote_msg->put("reviewId", bondquotecache->review_id);
		bond_quote_msg->put("createTime", bondquotecache->create_time);
		bond_quote_msg->put("status", bondquotecache->status);
		bond_quote_msg->put("bankAgentId", bondquotecache->bank_agent_id);
		bond_quote_msg->put("agentCode", bondquotecache->agent_code);
		bond_quote_msg->put("priceDescription", bondquotecache->price_description);
		bond_quote_msg->put("flagVip", bondquotecache->trader_is_vip);
		bond_quote_msg->put("flagBad", bondquotecache->trader_is_bad);
		bond_quote_msg->put("flagBargain", bondquotecache->flag_bargain);
		bond_quote_msg->put("goodsType", bondquotecache->goods_type);
		bond_quote_msg->put("goodsShortName", bondquotecache->goods_short_name);
		bond_quote_msg->put("goodsLevel", bondquotecache->goods_level);
		bond_quote_msg->put("goodsTerm", bondquotecache->goods_term);
		bond_quote_msg->put("goodsName", bondquotecache->goods_name);
		bond_quote_msg->put("optionType", bondquotecache->option_type);
		bond_quote_msg->put("rateType", bondquotecache->rate_type);
		bond_quote_msg->put("internally", bondquotecache->internally);
		bond_quote_msg->put("flagRelation", bondquotecache->flag_relation);
		bond_quote_msg->put("traderId", bondquotecache->trader_id);
		bond_quote_msg->put("quoteType", bondquotecache->quote_type);
        if (bondquotecache->return_point != DOUBLE_NULL) {
			bond_quote_msg->put("returnPoint", bondquotecache->return_point);
		}
        if (bondquotecache->net_price != DOUBLE_NULL) {
			bond_quote_msg->put("netPrice", bondquotecache->net_price);
		}
        if (bondquotecache->full_price != DOUBLE_NULL) {
			bond_quote_msg->put("fullPrice", bondquotecache->full_price);
		}
        if (bondquotecache->yield != DOUBLE_NULL) {
			bond_quote_msg->put("yield", bondquotecache->yield);
		}
        if (bondquotecache->spread != DOUBLE_NULL) {
			bond_quote_msg->put("spread", bondquotecache->spread);
		}
		bond_quote_msg->put("clearSpeed", bondquotecache->clear_speed);
		bond_quote_msg->put("deliveryDate", bondquotecache->delivery_date);
		bond_quote_msg->put("exercise", bondquotecache->exercise);
		bond_quote_msg->put("rebate", bondquotecache->rebate);
		bond_quote_msg->put("calculateStatus", bondquotecache->calculate_status);
	}
}

void BondQuoteBusiness::AssembleReferQuote(BondQuoteReferCachePtr& bondquotecache, ptree* bond_quote_msg) {
	if (bond_quote_msg != NULL) {
        bond_quote_msg->put("id", bondquotecache->id);
        bond_quote_msg->put("companyId", bondquotecache->company_id);
		bond_quote_msg->put("goodsId", bondquotecache->goods_id);
		bond_quote_msg->put("bondKey", bondquotecache->bond_key);
		bond_quote_msg->put("listedMarket", bondquotecache->listed_market);
		bond_quote_msg->put("bondKeyListedMarket", bondquotecache->bond_key_listed_market);
		bond_quote_msg->put("goodsCode", bondquotecache->goods_code);
		bond_quote_msg->put("profitCode", bondquotecache->profit_code);
		bond_quote_msg->put("bankName", bondquotecache->bank_name);
		bond_quote_msg->put("bankId", bondquotecache->bank_id);
        bond_quote_msg->put("operatorId", bondquotecache->operator_id);
		bond_quote_msg->put("description", bondquotecache->description);
		bond_quote_msg->put("symbol", bondquotecache->symbol);
        if (bondquotecache->price != DOUBLE_NULL) {
			bond_quote_msg->put("price", bondquotecache->price);
		}
        if (bondquotecache->volume != DOUBLE_NULL) {
            bond_quote_msg->put("volume", bondquotecache->volume);
        }
		bond_quote_msg->put("dealStatus", bondquotecache->deal_status);
		bond_quote_msg->put("dealTime", bondquotecache->deal_time);
		bond_quote_msg->put("reviewId", bondquotecache->review_id);
		bond_quote_msg->put("createTime", bondquotecache->create_time);
		bond_quote_msg->put("status", bondquotecache->status);
		bond_quote_msg->put("bankAgentId", bondquotecache->bank_agent_id);
		bond_quote_msg->put("agentCode", bondquotecache->agent_code);
		bond_quote_msg->put("priceDescription", bondquotecache->price_description);
		bond_quote_msg->put("flagVip", bondquotecache->trader_is_vip);
		bond_quote_msg->put("flagBad", bondquotecache->trader_is_bad);
		bond_quote_msg->put("flagBargain", bondquotecache->flag_bargain);
		bond_quote_msg->put("goodsType", bondquotecache->goods_type);
		bond_quote_msg->put("goodsShortName", bondquotecache->goods_short_name);
		bond_quote_msg->put("goodsLevel", bondquotecache->goods_level);
		bond_quote_msg->put("goodsTerm", bondquotecache->goods_term);
		bond_quote_msg->put("goodsName", bondquotecache->goods_name);
		bond_quote_msg->put("optionType", bondquotecache->option_type);
		bond_quote_msg->put("rateType", bondquotecache->rate_type);
		bond_quote_msg->put("internally", bondquotecache->internally);
		bond_quote_msg->put("flagRelation", bondquotecache->flag_relation);
		bond_quote_msg->put("traderId", bondquotecache->trader_id);
		bond_quote_msg->put("quoteType", bondquotecache->quote_type);
        if (bondquotecache->return_point != DOUBLE_NULL) {
			bond_quote_msg->put("returnPoint", bondquotecache->return_point);
		}
        if (bondquotecache->net_price != DOUBLE_NULL) {
			bond_quote_msg->put("netPrice", bondquotecache->net_price);
		}
        if (bondquotecache->full_price != DOUBLE_NULL) {
			bond_quote_msg->put("fullPrice", bondquotecache->full_price);
		}
        if (bondquotecache->yield != DOUBLE_NULL) {
			bond_quote_msg->put("yield", bondquotecache->yield);
		}
        if (bondquotecache->spread != DOUBLE_NULL) {
			bond_quote_msg->put("spread", bondquotecache->spread);
		}
		bond_quote_msg->put("clearSpeed", bondquotecache->clear_speed);
		bond_quote_msg->put("deliveryDate", bondquotecache->delivery_date);
		bond_quote_msg->put("exercise", bondquotecache->exercise);
		bond_quote_msg->put("rebate", bondquotecache->rebate);
		bond_quote_msg->put("calculateStatus", bondquotecache->calculate_status);
	}
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
    return bondQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteCacheVecPtr BondQuoteBusiness::GetBondQuoteByID(const std::string& bond_quote_id)
{
    std::string str = "id = '" + bond_quote_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteCacheController bondQuoteCC;
    return bondQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteReferCacheVecPtr BondQuoteBusiness::GetBondReferByID(const std::string& bond_quote_id)
{
    std::string str = "id = '" + bond_quote_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteReferCacheController bondReferQuoteCC;
    return bondReferQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteCacheVecPtr BondQuoteBusiness::GetBondQuoteByConditions(const int& side, const std::string& internal_flag, const std::string& bond_key_listed_market, const std::string& institution_id, const std::string& trader_id)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' and bank_id = '" + institution_id + "' and bank_agent_id = '" + trader_id + "' and symbol = " + boost::lexical_cast<std::string>(side) + " and internally = '" + internal_flag + "' and deal_status = '0'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteCacheController bondQuoteCC;
    return bondQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteReferCacheVecPtr BondQuoteBusiness::FetchBondReferQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range)
{
    std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, range, "deal_time desc"); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondQuoteReferCacheController bondReferQuoteCC;
    return bondReferQuoteCC.getCacheListByQueryInThreadSafty(query);
}

BondQuoteCacheVecPtr BondQuoteBusiness::FetchBondQuotesByDate(const std::string& cond, const std::string& bondtype, const std::string& datetime)
{
    std::string str = std::string("") + datetime.c_str() + "%";
    std::string str1 = " bond_category = '" + cond + "'" + GetCompanyConditionStringWithPreAnd() + " and bond_type != '" + bondtype + "' and create_time like '" + str + "' order by bond_type, goods_term asc"; //Modify by Young 05-Apr-2016
    dbQuery query1(str1.c_str());
    std::string str2 = " bond_category = '" + cond + "'" + GetCompanyConditionStringWithPreAnd() + " and bond_type = '" + bondtype + "' and create_time like '" + str + "' order by goods_term asc"; //Modify by Young 05-Apr-2016
    dbQuery query2(str2.c_str());
    BondQuoteCacheController bondQuoteCC;
    BondQuoteCacheVecPtr vec1 = bondQuoteCC.getCacheListByQueryInThreadSafty(query1);
    BondQuoteCacheVecPtr vec2 = bondQuoteCC.getCacheListByQueryInThreadSafty(query2);
    if(vec1->size() == 0)
        return vec2;
    if(vec2->size() == 0)
        return vec1;
    for(unsigned int i = 0; i < vec2->size(); ++i)
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
    std::string str = GetFastdbQueryWithCompanyCondition(cond, "", "");
	BondQuoteCacheController bondQuoteCC;
 	dbQuery query(str.c_str());
    
    return bondQuoteCC.getSizeByQueryInThreadSafty(query);
}

int BondQuoteBusiness::GetBondReferQuotesCount(const sdbus::string& cond)
{
    std::string str = GetFastdbQueryWithCompanyCondition(cond, "", ""); //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());

    BondQuoteReferCacheController bondReferQuoteCC;
    return bondReferQuoteCC.getSizeByQueryInThreadSafty(query);
}

void BondQuoteBusiness::SyncBondQuoteToQB(std::vector<BondQuoteCachePtr>& bond_quote_vec, const std::string& kServiceQBSync, const int method) {
	if (bond_quote_vec.size() == 0) {
		return;
	}
	QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
	if (qb_sync_service == NULL) {
		return;
	}
	std::vector<BondQuoteCachePtr> bond_quote_vec_internal;
	std::vector<BondQuoteCachePtr> bond_quote_vec_non_internal;

	for (int i = 0; i < bond_quote_vec.size(); ++i) {
		if (bond_quote_vec[i]->internally == "1") {
			bond_quote_vec_non_internal.push_back(bond_quote_vec[i]);
		} else if (bond_quote_vec[i]->internally == "2") {
			bond_quote_vec_internal.push_back(bond_quote_vec[i]);
		}
	}

	if (bond_quote_vec_internal.size() > 0) {
		LOGGER_INFO("Synchronize Internal Bond Quote To QB");
		qb_sync_service->PublishBondQuote(bond_quote_vec_internal, FID_PUSH_MESSAGE_METHOD_DELETE);
	}

	if (bond_quote_vec_non_internal.size() > 0) {
		LOGGER_INFO("Synchronize Bond Quote To QB");
		qb_sync_service->PublishBondQuote(bond_quote_vec_non_internal, method);
	}
}

void BondQuoteBusiness::SyncBondBestQuoteToQB(const std::vector<std::string>& bond_key_listed_maret_vec) {
	if (bond_key_listed_maret_vec.empty())
		return;
	QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
	if (qb_sync_service == NULL) {
		return;
	}
	qb_sync_service->PublishBondBestQuote(bond_key_listed_maret_vec, GetCompanyId());
}

bool BondQuoteBusiness::AddQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, bool export_is_internal, ptree* bond_quote_msg /*= NULL*/, bool manually_set_deal_id /*= false*/)
{
    BondQuoteCacheController bondquote_cc;
    std::vector<BondQuoteCachePtr> bond_quote_calc_vec;
    std::vector<BondQuoteCachePtr> bond_quote_vec;

    for (unsigned int i = 0; i < vec->size(); ++i)
    {
        BondQuotePtr bond_quote = vec->at(i);        

        BondQuoteCachePtr bondquotecache(new BondQuoteCache());
		if (manually_set_deal_id) {
			bondquotecache->id = bond_quote->id_;
		}
		else {
			bondquotecache->id = GetNewGuid();
		}
		vec->at(i)->id_ = bondquotecache->id;

        bondquotecache->company_id = GetCompanyId();

        SETIFASSIGNED(bondquotecache->bond_key_listed_market, bond_quote->bond_key_listed_market_)

        SETIFASSIGNED(bondquotecache->bank_id, bond_quote->institution_id_)
        SETIFASSIGNED(bondquotecache->bank_agent_id, bond_quote->trader_id_)
        SETIFASSIGNED(bondquotecache->operator_id, bond_quote->broker_id_)

        SETIFASSIGNED(bondquotecache->symbol, bond_quote->quote_side_)
        SETIFASSIGNED(bondquotecache->price, bond_quote->price_)
        SETIFASSIGNED(bondquotecache->volume, bond_quote->volume_)
        SETIFASSIGNED(bondquotecache->price_description, bond_quote->description_)

        SETIFASSIGNED(bondquotecache->rebate, bond_quote->fan_dian_flag_)
        SETIFASSIGNED(bondquotecache->return_point, bond_quote->fan_dian_)
        SETIFASSIGNED(bondquotecache->flag_bargain, bond_quote->bargain_flag_)
        SETIFASSIGNED(bondquotecache->flag_relation, bond_quote->oco_flag_)
        SETIFASSIGNED(bondquotecache->internally, bond_quote->internal_flag_)

        SETIFASSIGNED(bondquotecache->quote_type, bond_quote->quote_type_)
        SETIFASSIGNED(bondquotecache->delivery_date, bond_quote->delivery_date_)
        SETIFASSIGNED(bondquotecache->exercise, bond_quote->yield_type_)

        bond_quote_calc_vec.push_back(bondquotecache);
    }

    CalculatorBusiness calc_business;
	calc_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
    calc_business.Calc(bond_quote_calc_vec);
    for (unsigned int i = 0; i < bond_quote_calc_vec.size(); ++i) {       
        BondQuoteCachePtr bondquotecache = bond_quote_calc_vec.at(i);
    
        // create_time 由服务端生成
        bondquotecache->create_time_milli_sec = GetCurrentTimeMilliSec();
        bondquotecache->create_time = GetTimeString(bondquotecache->create_time_milli_sec / 1000);      

        CacheCenter::sharedInstance()->AttachCurrentThread();
        bondquotecache->loadCombinationFields();
        CacheCenter::sharedInstance()->DetachCurrentThread();

        BondQuoteCacheController bondquote_cc;
        bool succeed = bondquote_cc.insertCacheInThreadSafty(bondquotecache.get());

        if (succeed) {
            bond_quote_vec.push_back(bondquotecache);
        }
    }

	if (bond_quote_msg != NULL) {
		for (unsigned int i=0; i<bond_quote_vec.size(); ++i) {
			ptree each_bond_quote_msg;
			this->AssembleBondQuote(bond_quote_vec[i], &each_bond_quote_msg);
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}
    
    std::vector<std::string> bond_key_listed_market_vec;
    if (bond_quote_vec.size() > 0) {
        service_->PublishBondQuote(bond_quote_vec, "AddList");

		SyncBondQuoteToQB(bond_quote_vec, kServiceQBSync, FID_PUSH_MESSAGE_METHOD_ADD);

        LOGGER_INFO("bond_key_listed_market_vec.push_back start")
        std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_vec.begin();
        for ( ; it != bond_quote_vec.end(); ++it) {
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), (*it)->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back((*it)->bond_key_listed_market);
            }
        }
        LOGGER_INFO("bond_key_listed_market_vec.push_back end")
    }    
    
    if (bond_key_listed_market_vec.size() > 0) {       
        BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
        BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
        LOGGER_INFO("make company id start")
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016 
        LOGGER_INFO("make company id end")
        LOGGER_INFO("update best quote start")
        bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec, true);
        LOGGER_INFO("update best quote end")
        if (exported_text != NULL) {
            if(export_is_internal) {
                *exported_text = bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec);
            } else {
                *exported_text = bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
            }
        }
        LOGGER_INFO("copy best quote end");
		SyncBondBestQuoteToQB(bond_key_listed_market_vec);
        BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
        bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());
    }
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_vec);
    return true;
}

bool BondQuoteBusiness::UpdateQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, bool export_is_internal, ptree* bond_quote_msg /*= NULL*/)
{
    BondQuoteCacheController bondquote_cc;
    std::vector<BondQuoteCachePtr> bond_quote_calc_vec;
    std::vector<BondQuoteCachePtr> bond_quote_vec;

    for(unsigned int i = 0; i < vec->size(); ++i)
    {
        BondQuotePtr bond_quote = vec->at(i);
        std::string str = "id = '" + bond_quote->id_.ref() + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());

        BondQuoteCachePtr cache = bondquote_cc.getCacheByQueryInThreadSafty(q);
        if (cache) {
            UpdateBondQuoteCallBack(cache.get(), (void*)(bond_quote.get())); 
            bond_quote_calc_vec.push_back(cache);
        }
    }    

    CalculatorBusiness calc_business;
	calc_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
    calc_business.Calc(bond_quote_calc_vec);

    for (unsigned int i = 0; i < bond_quote_calc_vec.size(); ++i) {       
        BondQuoteCachePtr cache = bond_quote_calc_vec.at(i);
        
        CacheCenter::sharedInstance()->AttachCurrentThread();
        cache->loadCombinationFields();    
        CacheCenter::sharedInstance()->DetachCurrentThread();

        std::string str = "id = '" + cache->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());
        bondquote_cc.updateCacheInThreadSafty(q, cache.get());

        bond_quote_vec.push_back(cache);        
    }

	if (bond_quote_msg != NULL) {
		for (unsigned int i=0; i<bond_quote_vec.size(); ++i) {
			ptree each_bond_quote_msg;
			this->AssembleBondQuote(bond_quote_vec[i], &each_bond_quote_msg);
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}
    
    std::vector<std::string> bond_key_listed_market_vec;
    if (bond_quote_vec.size() > 0) {
        service_->PublishBondQuote(bond_quote_vec, "UpdateList");

        SyncBondQuoteToQB(bond_quote_vec, kServiceQBSync, FID_PUSH_MESSAGE_METHOD_UPDATE);

        std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_vec.begin();
        for ( ; it != bond_quote_vec.end(); ++it) {
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), (*it)->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back((*it)->bond_key_listed_market);
            }
        }
    }

    if (bond_key_listed_market_vec.size() > 0) {       
        BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
        BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec, true);        
        if (exported_text != NULL) {
            if(export_is_internal) {
                *exported_text = bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec);
            } else {
                *exported_text = bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
            }
        }
		SyncBondBestQuoteToQB(bond_key_listed_market_vec);
        BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
        bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());
    }
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_vec);
    return true;
}

bool BondQuoteBusiness::UpdateReferQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, bool export_is_internal, ptree* bond_quote_msg /*= NULL*/)
{
	BondQuoteReferCacheController bondquoterefer_cc;
	std::vector<BondQuoteReferCachePtr> bond_quote_vec;

	for(unsigned int i = 0; i < vec->size(); ++i)
	{
		BondQuotePtr bond_quote = vec->at(i);
		std::string str = "id = '" + bond_quote->id_.ref() + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());

        BondQuoteReferCachePtr cache = bondquoterefer_cc.getCacheByQueryInThreadSafty(q);
        if (cache) {
            UpdateBondReferQuoteCallBack(cache.get(), (void*)(bond_quote.get()));          

            CacheCenter::sharedInstance()->AttachCurrentThread();
            cache->loadCombinationFields();    
            CacheCenter::sharedInstance()->DetachCurrentThread();
            bondquoterefer_cc.updateCacheInThreadSafty(q, cache.get());

            bond_quote_vec.push_back(cache);        
        }
	}

	if (bond_quote_msg != NULL) {
		for (unsigned int i=0; i<bond_quote_vec.size(); ++i) {
			ptree each_bond_quote_msg;
			this->AssembleReferQuote(bond_quote_vec[i], &each_bond_quote_msg);
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}

	if (bond_quote_vec.size() > 0) {
		service_->PublishBondReferQuote(bond_quote_vec, "UpdateList");
	}
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_vec);
	return true;
}

bool BondQuoteBusiness::UpdateQuoteInfo(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, bool export_is_internal, ptree* bond_quote_msg /*= NULL*/)
{
	BondQuoteCacheController bondquote_cc;
    std::vector<BondQuoteCachePtr> bond_quote_calc_vec;
	std::vector<BondQuoteCachePtr> bond_quote_vec;

	sdbus::VariantList quote_ids;
	for(unsigned int i = 0; i < vec->size(); ++i)
	{
		quote_ids.PushBack(vec->at(i)->id_.ref().c_str());
		BondQuotePtr bond_quote = vec->at(i);
		std::string str = "id = '" + bond_quote->id_.ref() + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());

        BondQuoteCachePtr cache = bondquote_cc.getCacheByQueryInThreadSafty(q);
        if (cache) {
            UpdateBondQuoteCallBack(cache.get(), (void*)(bond_quote.get()));   
            bond_quote_calc_vec.push_back(cache);
        }
    }
    
    CalculatorBusiness calc_business;
	calc_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
    calc_business.Calc(bond_quote_calc_vec);

    for (unsigned int i = 0; i < bond_quote_calc_vec.size(); ++i) {       
        BondQuoteCachePtr cache = bond_quote_calc_vec.at(i);

        CacheCenter::sharedInstance()->AttachCurrentThread();
        cache->loadCombinationFields();    
        CacheCenter::sharedInstance()->DetachCurrentThread();
      
        std::string str = "id = '" + cache->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());
        bondquote_cc.updateCacheInThreadSafty(q, cache.get());

        bond_quote_vec.push_back(cache);       
	}
	
	if (bond_quote_msg != NULL) {
		for (unsigned int i=0; i<bond_quote_vec.size(); ++i) {
			ptree each_bond_quote_msg;
			this->AssembleBondQuote(bond_quote_vec[i], &each_bond_quote_msg);
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}

	std::vector<std::string> bond_key_listed_market_vec;
	if (bond_quote_vec.size() > 0) {
		service_->PublishBondQuote(bond_quote_vec, "UpdateList");

		SyncBondQuoteToQB(bond_quote_vec, kServiceQBSync, FID_PUSH_MESSAGE_METHOD_UPDATE);

		std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_vec.begin();
		for ( ; it != bond_quote_vec.end(); ++it) {
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), (*it)->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back((*it)->bond_key_listed_market);
            }
		}
	}

	if (bond_key_listed_market_vec.size() > 0) {       
		BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
		BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
		bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec, true);        

		if (exported_text != NULL) {
			if(export_is_internal) {
				*exported_text = bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec);
			} else {
				*exported_text = bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
			}
			if (exported_text->length() == 0)
			{
				*exported_text = this->CopyReferredQuote(quote_ids);
			}
		}
		SyncBondBestQuoteToQB(bond_key_listed_market_vec);
        BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
        bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());
    }
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_vec);
    return true;
}

void BondQuoteBusiness::UpdateBondQuoteCallBack(BondQuoteCache* cache, void* param) {
    if (param == NULL) {
        return;
    }

    BondQuote* bond_quote = (BondQuote*)param;    

    SETIFASSIGNED(cache->bond_key_listed_market, bond_quote->bond_key_listed_market_)

    SETIFASSIGNED(cache->bank_id, bond_quote->institution_id_)
    SETIFASSIGNED(cache->bank_agent_id, bond_quote->trader_id_)
    SETIFASSIGNED(cache->operator_id, bond_quote->broker_id_)

    SETIFASSIGNED(cache->symbol, bond_quote->quote_side_)
    SETIFASSIGNED(cache->price, bond_quote->price_)
    SETIFASSIGNED(cache->volume, bond_quote->volume_)
    SETIFASSIGNED(cache->price_description, bond_quote->description_)

    SETIFASSIGNED(cache->rebate, bond_quote->fan_dian_flag_)
    SETIFASSIGNED(cache->return_point, bond_quote->fan_dian_)
    SETIFASSIGNED(cache->flag_bargain, bond_quote->bargain_flag_)
    SETIFASSIGNED(cache->flag_relation, bond_quote->oco_flag_)
    SETIFASSIGNED(cache->internally, bond_quote->internal_flag_)

    SETIFASSIGNED(cache->quote_type, bond_quote->quote_type_)
    SETIFASSIGNED(cache->delivery_date, bond_quote->delivery_date_)
    SETIFASSIGNED(cache->exercise, bond_quote->yield_type_)

    // create_time 由服务端生成
    cache->create_time_milli_sec = GetCurrentTimeMilliSec();
    cache->create_time = GetTimeString(cache->create_time_milli_sec / 1000);

    //CalculatorBusiness calc_business;
    //calc_business.Calc(cache);
}

void BondQuoteBusiness::UpdateBondReferQuoteCallBack(BondQuoteReferCache* cache, void* param) {
	if (param == NULL) {
		return;
	}

	BondQuote* bond_quote = (BondQuote*)param;    

	SETIFASSIGNED(cache->bond_key_listed_market, bond_quote->bond_key_listed_market_)

    SETIFASSIGNED(cache->bank_id, bond_quote->institution_id_)
    SETIFASSIGNED(cache->bank_agent_id, bond_quote->trader_id_)
    SETIFASSIGNED(cache->operator_id, bond_quote->broker_id_)

    SETIFASSIGNED(cache->symbol, bond_quote->quote_side_)
    SETIFASSIGNED(cache->price, bond_quote->price_)
    SETIFASSIGNED(cache->volume, bond_quote->volume_)
    SETIFASSIGNED(cache->price_description, bond_quote->description_)

    SETIFASSIGNED(cache->rebate, bond_quote->fan_dian_flag_)
    SETIFASSIGNED(cache->return_point, bond_quote->fan_dian_)
    SETIFASSIGNED(cache->flag_bargain, bond_quote->bargain_flag_)
    SETIFASSIGNED(cache->flag_relation, bond_quote->oco_flag_)
    SETIFASSIGNED(cache->internally, bond_quote->internal_flag_)

    SETIFASSIGNED(cache->quote_type, bond_quote->quote_type_)
    SETIFASSIGNED(cache->delivery_date, bond_quote->delivery_date_)
    SETIFASSIGNED(cache->exercise, bond_quote->yield_type_)

	// create_time 由服务端生成
	cache->create_time_milli_sec = GetCurrentTimeMilliSec();
	cache->create_time = GetTimeString(cache->create_time_milli_sec / 1000);
}

bool BondQuoteBusiness::EditReferredQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, bool export_is_internal, ptree* bond_quote_msg /*= NULL*/)
{
    BondQuoteReferCacheController bondquoterefer_cc;
    BondQuoteCacheController bondquote_cc;
    std::vector<BondQuoteCachePtr> bond_quote_vec;
    std::vector<BondQuoteReferCachePtr> bond_quote_refer_vec;

    for(unsigned int i = 0; i < vec->size(); ++i)
    {
        BondQuotePtr bond_quote = vec->at(i);
        std::string str = "id = '" + bond_quote->id_.ref() + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());

        BondQuoteReferCachePtr cache = bondquoterefer_cc.getCacheByQueryInThreadSafty(q);
        if (cache) {
            EditReferredQuoteCallBack(cache.get(), (void*)(bond_quote.get()));       
            CacheCenter::sharedInstance()->AttachCurrentThread();
            cache->loadCombinationFields();    
            CacheCenter::sharedInstance()->DetachCurrentThread();
            bondquoterefer_cc.updateCacheInThreadSafty(q, cache.get());

            bond_quote_refer_vec.push_back(cache);
        }
    }
    
    std::vector<std::string> bond_key_listed_market_vec;
    // 推送Refer Quote -- "Delete"
    if (bond_quote_refer_vec.size() > 0) { 
        std::vector<BondQuoteReferCachePtr>::const_iterator it = bond_quote_refer_vec.begin();
        for ( ; it != bond_quote_refer_vec.end(); ++it) {
            std::string str = "id = '" + (*it)->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
            dbQuery q(str.c_str());
            // Refer Quote Cache里删除此报价
            bondquoterefer_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

            BondQuoteCachePtr bond_quote_cache = BondQuoteCache::fromBondQuoteReferCache((*it).get());            
            // Quote Cache里添加该报价
            bondquote_cc.insertCacheInThreadSaftyWithoutDBSync(bond_quote_cache.get());

            bond_quote_vec.push_back(bond_quote_cache);
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), bond_quote_cache->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back(bond_quote_cache->bond_key_listed_market);
            }
        }

        service_->PublishBondReferQuote(bond_quote_refer_vec, "DeleteList");               
    }

    if (bond_quote_msg != NULL) {
		for (unsigned int i=0; i<bond_quote_vec.size(); ++i) {
			ptree each_bond_quote_msg;
			this->AssembleReferQuote(bond_quote_refer_vec[i], &each_bond_quote_msg);
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}

    // 推送Quote -- "Add"
    if (bond_quote_vec.size() > 0) {
        service_->PublishBondQuote(bond_quote_vec, "AddList");   

        SyncBondQuoteToQB(bond_quote_vec, kServiceQBSync, FID_PUSH_MESSAGE_METHOD_ADD);
    }

    // 更新最优报价缓存并推送
    if (bond_key_listed_market_vec.size() > 0) {
        BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
        BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec, true);        
        if (exported_text != NULL) {
            if(export_is_internal) {
                *exported_text = bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec);
            } else {
                *exported_text = bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
            }
        }
		SyncBondBestQuoteToQB(bond_key_listed_market_vec);
        BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
        bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());
    }
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_vec);
    return true;
}

void BondQuoteBusiness::EditReferredQuoteCallBack(BondQuoteReferCache* cache, void* param) {
    if (param == NULL) {
        return;
    }

    BondQuote* bond_quote = (BondQuote*)param;    

    SETIFASSIGNED(cache->bond_key_listed_market, bond_quote->bond_key_listed_market_)

    SETIFASSIGNED(cache->bank_id, bond_quote->institution_id_)
    SETIFASSIGNED(cache->bank_agent_id, bond_quote->trader_id_)
    SETIFASSIGNED(cache->operator_id, bond_quote->broker_id_)

    SETIFASSIGNED(cache->symbol, bond_quote->quote_side_)
    SETIFASSIGNED(cache->price, bond_quote->price_)
    SETIFASSIGNED(cache->volume, bond_quote->volume_)
    SETIFASSIGNED(cache->price_description, bond_quote->description_)

    SETIFASSIGNED(cache->rebate, bond_quote->fan_dian_flag_)
    SETIFASSIGNED(cache->return_point, bond_quote->fan_dian_)
    SETIFASSIGNED(cache->flag_bargain, bond_quote->bargain_flag_)
    SETIFASSIGNED(cache->flag_relation, bond_quote->oco_flag_)
    SETIFASSIGNED(cache->internally, bond_quote->internal_flag_)

    SETIFASSIGNED(cache->quote_type, bond_quote->quote_type_)
    SETIFASSIGNED(cache->delivery_date, bond_quote->delivery_date_)
    SETIFASSIGNED(cache->exercise, bond_quote->yield_type_)

    // create_time 由服务端生成
    cache->create_time_milli_sec = GetCurrentTimeMilliSec();
    cache->create_time = GetTimeString(cache->create_time_milli_sec / 1000);

    cache->deal_status      = "0"; // 正常报价

    CalculatorBusiness calc_business;
    calc_business.Calc(cache);

}

bool BondQuoteBusiness::DeleteReferredQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, ptree* refer_quote_msg /*= NULL*/)
{
    BondQuoteReferCacheController bondquoterefer_cc;
    std::vector<BondQuoteReferCachePtr> bond_quote_refer_vec;

	if (refer_quote_msg != NULL) {
		for (unsigned int i = 0; i < vec->size(); ++i)
		{
			ptree each_refer_quote_msg;
			each_refer_quote_msg.put("id", vec->at(i)->id_.ref());
			refer_quote_msg->push_back(std::make_pair("", each_refer_quote_msg));
		}
	}

    std::string sql_str = /*GetCompanyConditionStringWithEndAnd() + */" id in "; //Modify by Young 05-Apr-2016
    sql_str += "(";
    for (unsigned int i = 0; i < vec->size(); ++i)
    {
        if (i > 0) {
            sql_str += ", ";
        }
        sql_str += "'" + vec->at(i)->id_.ref() + "'";
    }
    sql_str += ")";
    LOGGER_DEBUG("vec size: " << vec->size() << ", sql length: " << sql_str.length());
    dbQuery q(sql_str.c_str());

    bondquoterefer_cc.updateCacheByQueryInThreadSafty(q, DeleteReferredQuoteCallBack, NULL, DeleteReferredQuoteSuccessed, DeleteReferredQuoteFailed, (void*)(&bond_quote_refer_vec));
    
    // 推送Refer Quote -- "Delete"
    if (bond_quote_refer_vec.size() > 0) {
        std::vector<BondQuoteReferCachePtr>::const_iterator it = bond_quote_refer_vec.begin();
        for ( ; it != bond_quote_refer_vec.end(); ++it) {
            std::string str = "id = '" + (*it)->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
            dbQuery q(str.c_str());
            // Refer Quote Cache里删除此报价
            bondquoterefer_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
        }

        service_->PublishBondReferQuote(bond_quote_refer_vec, "DeleteList");
    }
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_refer_vec);
    return true;
}

void BondQuoteBusiness::DeleteReferredQuoteCallBack(BondQuoteReferCache* cache, void* param)
{
    cache->status = "2";
}

void BondQuoteBusiness::DeleteReferredQuoteSuccessed(BondQuoteReferCache* cache, void* param) {
    std::vector<BondQuoteReferCachePtr>* bond_quote_refer_vec = (std::vector<BondQuoteReferCachePtr>*)param;

    BondQuoteReferCache* bond_quote_refer = new BondQuoteReferCache();
    *bond_quote_refer = *cache;

    bond_quote_refer_vec->push_back(BondQuoteReferCachePtr(bond_quote_refer));
    LOGGER_INFO("DeleteReferredQuote: Success for updating BondQuoteReferCache: " << cache->id)
}

void BondQuoteBusiness::DeleteReferredQuoteFailed(BondQuoteReferCache* cache, void* param) {
    LOGGER_ERROR("DeleteReferredQuote: Failed for updating BondQuoteReferCache: " << cache->id)
}

bool BondQuoteBusiness::DeleteQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, bool export_is_internal, ptree* bond_quote_msg /*= NULL*/)
{
    BondQuoteCacheController bondquote_cc;
    std::vector<BondQuoteCachePtr> bond_quote_vec;

	if (bond_quote_msg != NULL) {
		for (unsigned int i = 0; i < vec->size(); ++i)
		{
			ptree each_bond_quote_msg;
			each_bond_quote_msg.put("id", vec->at(i)->id_.ref());
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}

    std::string sql_str = /*GetCompanyConditionStringWithEndAnd() + */" id in "; //Modify by Young 05-Apr-2016
    sql_str += "(";
    for (unsigned int i = 0; i < vec->size(); ++i)
    {
        if (i > 0) {
            sql_str += ", ";
        }
        sql_str += "'" + vec->at(i)->id_.ref() + "'";
    }
    sql_str += ")";
    LOGGER_DEBUG("vec size: " << vec->size() << ", sql length: " << sql_str.length());
    dbQuery q(sql_str.c_str());

    bondquote_cc.updateCacheByQueryInThreadSafty(q, DeleteQuoteCallBack, NULL, DeleteQuoteSuccessed, DeleteQuoteFailed, (void*)(&bond_quote_vec));
	QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
    std::vector<std::string> bond_key_listed_market_vec;
    // 推送Quote -- "Delete"
    if (bond_quote_vec.size() > 0) {
        std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_vec.begin();
        for ( ; it != bond_quote_vec.end(); ++it) {            
            std::string str = "id = '" + (*it)->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
            dbQuery q(str.c_str());
            // Refer Quote Cache里删除此报价
            bondquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
            
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), (*it)->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back((*it)->bond_key_listed_market);
            }
        }

        service_->PublishBondQuote(bond_quote_vec, "DeleteList");
        
        if (qb_sync_service) {
            qb_sync_service->PublishBondQuote(bond_quote_vec, FID_PUSH_MESSAGE_METHOD_DELETE);
        }
    }
    
    // 更新最优报价缓存并推送
    if (bond_key_listed_market_vec.size() > 0) {       
        BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
        BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec);        
        if (exported_text != NULL) {
            if(export_is_internal) {
                *exported_text = bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec);
            } else {
                *exported_text = bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
            }
        }
		if (qb_sync_service) {
			qb_sync_service->PublishBondBestQuote(bond_key_listed_market_vec, GetCompanyId());
		}		
        BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
        bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());
    }
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_vec);
    return true;
}

void BondQuoteBusiness::DeleteQuoteCallBack(BondQuoteCache* cache, void* param)
{
    cache->status = "2";
}

void BondQuoteBusiness::DeleteQuoteSuccessed(BondQuoteCache* cache, void* param) {
    std::vector<BondQuoteCachePtr>* bond_quote_vec = (std::vector<BondQuoteCachePtr>*)param;

    BondQuoteCache* bond_quote = new BondQuoteCache();
    *bond_quote = *cache;

    bond_quote_vec->push_back(BondQuoteCachePtr(bond_quote));
    LOGGER_INFO("DeleteQuote: Success for updating BondQuoteCache: " << cache->id)
}

void BondQuoteBusiness::DeleteQuoteFailed(BondQuoteCache* cache, void* param) {
    LOGGER_ERROR("DeleteQuote: Failed for updating BondQuoteCache: " << cache->id)
}

bool BondQuoteBusiness::ReferBondQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text, bool export_is_internal, ptree* bond_quote_msg)
{
    BondQuoteCacheController bondquote_cc;
    BondQuoteReferCacheController bondquoterefer_cc;
    std::vector<BondQuoteCachePtr> bond_quote_vec;
    std::vector<BondQuoteReferCachePtr> bond_quote_refer_vec;

    sdbus::VariantList quote_ids;
    std::string sql_str = /*GetCompanyConditionStringWithEndAnd() +*/ " id in "; //Modify by Young 05-Apr-2016
    sql_str += "(";
    for (unsigned int i = 0; i < vec->size(); ++i)
    {
        quote_ids.PushBack(vec->at(i)->id_.ref().c_str());
        if (i > 0) {
            sql_str += ", ";
        }
        sql_str += "'" + vec->at(i)->id_.ref() + "'";
    }
    sql_str += ")";
    LOGGER_DEBUG("vec: " << vec->size() << ", sql length: " << sql_str.length());
    dbQuery q(sql_str.c_str());

    bondquote_cc.updateCacheByQueryInThreadSafty(q, ReferBondQuoteCallBack, NULL, ReferBondQuoteSuccessed, ReferBondQuoteFailed, (void*)(&bond_quote_vec));
	QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
    std::vector<std::string> bond_key_listed_market_vec;
    // 推送Quote -- "Delete"
    if (bond_quote_vec.size() > 0) { 
        std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_vec.begin();
        for ( ; it != bond_quote_vec.end(); ++it) {
            std::string str = "id = '" + (*it)->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
            dbQuery q(str.c_str());
            // Quote Cache里删除此报价
            bondquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

            BondQuoteReferCachePtr bond_quote_refer_cache = BondQuoteReferCache::fromBondQuoteCache((*it).get());            
            // Refer Quote Cache里添加该报价
            bondquoterefer_cc.insertCacheInThreadSaftyWithoutDBSync(bond_quote_refer_cache.get());

            bond_quote_refer_vec.push_back(bond_quote_refer_cache);            
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), bond_quote_refer_cache->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back(bond_quote_refer_cache->bond_key_listed_market);
            }
        }

        service_->PublishBondQuote(bond_quote_vec, "DeleteList");
         
        if (qb_sync_service) {
            qb_sync_service->PublishBondQuote(bond_quote_vec, FID_PUSH_MESSAGE_METHOD_DELETE);
        }
    }
    
    // 推送Refer Quote -- "Add"
    if (bond_quote_refer_vec.size() > 0) {
        service_->PublishBondReferQuote(bond_quote_refer_vec, "AddList");
    }

	if (bond_quote_msg != NULL) {
		for (unsigned int i=0; i<bond_quote_refer_vec.size(); ++i) {
			ptree each_bond_quote_msg;
			this->AssembleReferQuote(bond_quote_refer_vec[i], &each_bond_quote_msg);
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}

    // 更新最优报价缓存并推送
    if (bond_key_listed_market_vec.size() > 0) {
        BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
        BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec);        
        if (exported_text != NULL) {
            if(export_is_internal) {
                *exported_text = bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec);
            } else {
                *exported_text = bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
            }
            if (exported_text->length() == 0)
            {
                *exported_text = this->CopyReferredQuote(quote_ids);
            }
        }
		if (qb_sync_service) {
			qb_sync_service->PublishBondBestQuote(bond_key_listed_market_vec, GetCompanyId());
		}
    }

    BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
    bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_refer_vec);
    return true;
}
/*
bool BondQuoteBusiness::ReferBondQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text, bool export_is_internal, ptree* bond_quote_msg)
{
	int _REFER_QUOTE_SIZE = 1000;
	std::vector<std::string> sql_vec;
	std::vector<sdbus::VariantList> quote_ids_vec;
	int iCnt = 0;
	std::string sql = GetCompanyConditionStringWithEndAnd() + " id in (";
	sdbus::VariantList quote_ids;
	for(size_t i = 0; i < vec->size(); ++i){
		const std::string& id = vec->at(i)->id_;
		if(iCnt > 0){
			sql += ", ";
		}
		sql += "'" + id + "'";
		quote_ids.PushBack(id.c_str());
		iCnt ++;
		if(i == vec->size() - 1){
			sql += ")";
			sql_vec.push_back(sql);
			quote_ids_vec.push_back(quote_ids);
			break;
		}
		if(iCnt >= _REFER_QUOTE_SIZE){
			sql += ")";
			sql_vec.push_back(sql);
			sql.clear();
			sql = GetCompanyConditionStringWithEndAnd() + " id in (";
			quote_ids_vec.push_back(quote_ids);
			quote_ids.Clear();
			iCnt = 0;
		}
	}
	LOGGER_DEBUG("refer quote, vec size[" << vec->size() << "], sql_vec size[" << sql_vec.size() << "], quote_ids_vec size[" << quote_ids_vec.size() << "]");
	
	for(size_t i = 0; i < sql_vec.size(); ++i){
		const std::string& sql_str = sql_vec.at(i);
		const sdbus::VariantList& quote_ids = quote_ids_vec.at(i);
		LOGGER_DEBUG("refer quote, sql length[" << sql.length() << "], ids size[" << quote_ids.Size() << "]");
		
		BondQuoteCacheController bondquote_cc;
		BondQuoteReferCacheController bondquoterefer_cc;
		std::vector<BondQuoteCachePtr> bond_quote_vec;
		std::vector<BondQuoteReferCachePtr> bond_quote_refer_vec;
		dbQuery q(sql_str.c_str());
		bondquote_cc.updateCacheByQueryInThreadSafty(q, ReferBondQuoteCallBack, NULL, ReferBondQuoteSuccessed, ReferBondQuoteFailed, (void*)(&bond_quote_vec));

		std::vector<std::string> bond_key_listed_market_vec;
		// 推送Quote -- "Delete"
		if (bond_quote_vec.size() > 0) { 
			std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_vec.begin();
			std::string sql = GetCompanyConditionStringWithEndAnd() + " id in (";
			int i = 0;
			for ( ; it != bond_quote_vec.end(); ++it) {
				if(i > 0){
					sql += ",";
				}
				sql += "'" + (*it)->id + "'";

				BondQuoteReferCachePtr bond_quote_refer_cache = BondQuoteReferCache::fromBondQuoteCache((*it).get());            
				// Refer Quote Cache里添加该报价
				bondquoterefer_cc.insertCacheInThreadSaftyWithoutDBSync(bond_quote_refer_cache.get());

				bond_quote_refer_vec.push_back(bond_quote_refer_cache);            
				std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), bond_quote_refer_cache->bond_key_listed_market);
				if (iter == bond_key_listed_market_vec.end()) {
					bond_key_listed_market_vec.push_back(bond_quote_refer_cache->bond_key_listed_market);
				}
			}
			sql += ")";
			dbQuery delQ(sql.c_str());
			bondquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(delQ); // Quote Cache里删除此报价

			service_->PublishBondQuote(bond_quote_vec, "DeleteList");

			QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
			if (qb_sync_service) {
				qb_sync_service->PublishBondQuote(bond_quote_vec, FID_PUSH_MESSAGE_METHOD_DELETE);
			}
		}

		// 推送Refer Quote -- "Add"
		if (bond_quote_refer_vec.size() > 0) {
			service_->PublishBondReferQuote(bond_quote_refer_vec, "AddList");
		}

		if (bond_quote_msg != NULL) {
			for (unsigned int i=0; i<bond_quote_refer_vec.size(); ++i) {
				ptree each_bond_quote_msg;
				this->AssembleReferQuote(bond_quote_refer_vec[i], &each_bond_quote_msg);
				bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
			}
		}

		// 更新最优报价缓存并推送
		if (bond_key_listed_market_vec.size() > 0) {
			BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
			BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
			bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
			bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec);        
			if (exported_text != NULL) {
				if(export_is_internal) {
					*exported_text += bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec);
				} else {
					*exported_text += bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
				}
				if (exported_text->length() == 0)
				{
					*exported_text += this->CopyReferredQuote(quote_ids);
				}
			}
		}
	}
	return true;
}*/

bool BondQuoteBusiness::ReferBondQuoteAll(std::string bond_category)
{
	if ("" == bond_category || bond_category.empty()) return true;

    BondQuoteCacheController bondquote_cc;
    BondQuoteReferCacheController bondquoterefer_cc;
    std::vector<BondQuoteCachePtr> bond_quote_vec;
    std::vector<BondQuoteReferCachePtr> bond_quote_refer_vec;

	std::string str;
	if ("all" == bond_category)
	{
		str = GetCompanyConditionString();
	}
	else
	{
		str = "bond_category = '" + bond_category + "'" + GetCompanyConditionStringWithPreAnd();
	}

    dbQuery q(str.c_str());
    bondquote_cc.updateCacheByQueryInThreadSafty(q, ReferBondQuoteCallBack, NULL, ReferBondQuoteSuccessed, ReferBondQuoteFailed, (void*)(&bond_quote_vec));
	QBSyncService *qb_sync_service = (QBSyncService*)ServiceManager::singleton()->getService(kServiceQBSync);
    std::vector<std::string> bond_key_listed_market_vec;
    // 推送Quote -- "Delete"
    if (bond_quote_vec.size() > 0) { 
        std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_vec.begin();
        for ( ; it != bond_quote_vec.end(); ++it) {
            std::string str = "id = '" + (*it)->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/;
            dbQuery q(str.c_str());
            // Quote Cache里删除此报价
            bondquote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);

            BondQuoteReferCachePtr bond_quote_refer_cache = BondQuoteReferCache::fromBondQuoteCache((*it).get());            
            // Refer Quote Cache里添加该报价
            bondquoterefer_cc.insertCacheInThreadSaftyWithoutDBSync(bond_quote_refer_cache.get());

            bond_quote_refer_vec.push_back(bond_quote_refer_cache);
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), bond_quote_refer_cache->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back(bond_quote_refer_cache->bond_key_listed_market);
            }
        }

		service_->PublishBondQuote(bond_quote_vec, "DeleteList", true, bond_category);

        if (qb_sync_service) {
            qb_sync_service->PublishBondQuote(bond_quote_vec, FID_PUSH_MESSAGE_METHOD_DELETE);			
        }
    }

    // 推送Refer Quote -- "Add"
    if (bond_quote_refer_vec.size() > 0) {
        service_->PublishBondReferQuote(bond_quote_refer_vec, "AddList");
    }

    // 更新最优报价缓存并推送
    if (bond_key_listed_market_vec.size() > 0) {
        BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
        BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec);        

        BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
        bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());

		if (qb_sync_service) {
			qb_sync_service->PublishBondBestQuote(bond_quote_vec, GetCompanyId());
		}
    }

    return true;
}

void BondQuoteBusiness::ReferBondQuoteCallBack(BondQuoteCache* cache, void* param) {
    cache->deal_status = "2"; // Referred报价
    cache->deal_time_milli_sec = GetCurrentTimeMilliSec();
    cache->deal_time = GetTimeString(cache->deal_time_milli_sec / 1000);

    cache->create_time_milli_sec= GetCurrentTimeMilliSec();
    cache->create_time = GetTimeString(cache->create_time_milli_sec / 1000);

    // refer不修改create_time  2014.11.22 huameng
    //cache->create_time = GetCurrentTimeString();
}

void BondQuoteBusiness::ReferBondQuoteSuccessed(BondQuoteCache* cache, void* param) {
    std::vector<BondQuoteCachePtr>* bond_quote_vec = (std::vector<BondQuoteCachePtr>*)param;

    BondQuoteCache* bond_quote = new BondQuoteCache();
    *bond_quote = *cache;

    bond_quote_vec->push_back(BondQuoteCachePtr(bond_quote));
    LOGGER_INFO("ReferBondQuote: Success for updating BondQuoteCache: " << cache->id)
}

void BondQuoteBusiness::ReferBondQuoteFailed(BondQuoteCache* cache, void* param) {
    LOGGER_ERROR("ReferBondQuote: Failed for updating BondQuoteCache: " << cache->id)
}

bool BondQuoteBusiness::UnreferBondQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text /* = NULL */, bool export_is_internal, ptree* bond_quote_msg /*= NULL*/)
{
    BondQuoteReferCacheController bondquoterefer_cc;
    BondQuoteCacheController bondquote_cc;
    std::vector<BondQuoteCachePtr> bond_quote_vec;
    std::vector<BondQuoteReferCachePtr> bond_quote_refer_vec;

    std::vector<std::string> id_vec;
    for(unsigned int i = 0; i < vec->size(); ++i)
    {
        BondQuotePtr bond_quote = vec->at(i);
        if(bond_quote.get() != NULL)
            id_vec.push_back("'" + bond_quote->id_.ref() + "'");
    }

    std::string str = "id in (" + join(id_vec, ",") + ")";
    dbQuery refer_q(str.c_str());
    BondQuoteReferCacheVecPtr bond_quote_refer_vec_ptr = bondquoterefer_cc.getCacheListByQueryInThreadSafty(refer_q);
    bond_quote_refer_vec = *bond_quote_refer_vec_ptr.get();
    UnReferBondQuoteCallBackBatch(bond_quote_refer_vec, NULL);       

    std::vector<BondQuoteReferCachePtr>::const_iterator const_refer_it = bond_quote_refer_vec.begin();
    for(; const_refer_it != bond_quote_refer_vec.end() ; ++const_refer_it)
    {
        //BondQuotePtr bond_quote = vec->at(i);

        if (const_refer_it->get() != NULL) {
            CacheCenter::sharedInstance()->AttachCurrentThread();
            (*const_refer_it)->loadCombinationFields();    
            CacheCenter::sharedInstance()->DetachCurrentThread();

            std::string str_update = "id = '" + (*const_refer_it)->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
            dbQuery q(str_update.c_str());
            bondquoterefer_cc.updateCacheInThreadSafty(q, const_refer_it->get());
        }
    }

	if (bond_quote_msg != NULL) {
		for (unsigned int i=0; i<bond_quote_refer_vec.size(); ++i) {
			ptree each_bond_quote_msg;
			this->AssembleReferQuote(bond_quote_refer_vec[i], &each_bond_quote_msg);
			bond_quote_msg->push_back(std::make_pair("", each_bond_quote_msg));
		}
	}

    std::vector<std::string> bond_key_listed_market_vec;
    // 推送Refer Quote -- "Delete"
    if (bond_quote_refer_vec.size() > 0) { 
        // Refer Quote Cache里删除此报价
        bondquoterefer_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(refer_q);

        std::vector<BondQuoteReferCachePtr>::const_iterator it = bond_quote_refer_vec.begin();
        for ( ; it != bond_quote_refer_vec.end(); ++it) {
            //std::string str = "id = '" + (*it)->id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
            //dbQuery q(str.c_str());

            BondQuoteCachePtr bond_quote_cache = BondQuoteCache::fromBondQuoteReferCache((*it).get());            
            // Quote Cache里添加该报价
            bondquote_cc.insertCacheInThreadSaftyWithoutDBSync(bond_quote_cache.get());

            bond_quote_vec.push_back(bond_quote_cache);
            std::vector<std::string>::const_iterator iter = find(bond_key_listed_market_vec.begin(), bond_key_listed_market_vec.end(), bond_quote_cache->bond_key_listed_market);
            if (iter == bond_key_listed_market_vec.end()) {
                bond_key_listed_market_vec.push_back(bond_quote_cache->bond_key_listed_market);
            }
        } 

        service_->PublishBondReferQuote(bond_quote_refer_vec, "DeleteList");             
    }
    
    // 推送Quote -- "Add"
    if (bond_quote_vec.size() > 0) {      
        service_->PublishBondQuote(bond_quote_vec, "AddList"); 

       SyncBondQuoteToQB(bond_quote_vec, kServiceQBSync, FID_PUSH_MESSAGE_METHOD_ADD);
    }

    // 更新最优报价缓存并推送
    if (bond_key_listed_market_vec.size() > 0) {
        BondBestQuoteService *bond_best_quote_service = (BondBestQuoteService*)ServiceManager::singleton()->getService(kServiceBondBestQuote);        
        BondBestQuoteBusiness bond_best_quote_business(bond_best_quote_service);
		bond_best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        bond_best_quote_business.UpdateBestQuote(bond_key_listed_market_vec, true);        
        if (exported_text != NULL) {
            if(export_is_internal) {
                *exported_text = bond_best_quote_business.CopyInternalBestQuote(bond_key_listed_market_vec); 
            } else {
                *exported_text = bond_best_quote_business.CopyBestQuote(bond_key_listed_market_vec);
            }
        }
		SyncBondBestQuoteToQB(bond_key_listed_market_vec);
        BondCompletedQuoteService *bond_completed_quote_service = (BondCompletedQuoteService*)ServiceManager::singleton()->getService(kServiceBondCompletedQuote);
        bond_completed_quote_service->PublishCompletedQuoteDetailList(bond_key_listed_market_vec, "ConditionSync", GetCompanyId());
    }
    MsgLogService::singleton()->writeQuoteLog(getRequestMsg(), bond_quote_vec);
    return true;
}

void BondQuoteBusiness::UnReferBondQuoteCallBack(BondQuoteReferCache* cache, void* param){
    cache->deal_status = "0"; // 正常报价 
    cache->create_time_milli_sec = GetCurrentTimeMilliSec();
    cache->create_time = GetTimeString(cache->create_time_milli_sec / 1000);

    CalculatorBusiness calc_business;
    calc_business.Calc(cache);
}

void BondQuoteBusiness::UnReferBondQuoteCallBackBatch(std::vector<BondQuoteReferCachePtr> &bond_quote_refer_vec, void* param){
    CalculatorBusiness calc_business;
    calc_business.Calc(bond_quote_refer_vec);


    std::vector<BondQuoteReferCachePtr>::iterator refer_it = bond_quote_refer_vec.begin();
    for(; refer_it != bond_quote_refer_vec.end() ; ++refer_it)
    {
        (*refer_it)->deal_status = "0"; // 正常报价 
        (*refer_it)->create_time_milli_sec = GetCurrentTimeMilliSec();
        (*refer_it)->create_time = GetTimeString((*refer_it)->create_time_milli_sec / 1000);
    }
}

bool BondQuoteBusiness::GvnTknQuote(sdbus::VariantList quote_ids, sdbus::string* exported_text /* = NULL */,  sdbus::VariantList *deal_id_list /* = NULL */, ptree* gvntkn_msg /*= NULL*/)
{
    BondDealService *bondDealService = (BondDealService*)ServiceManager::singleton()->getService("BondDeal");
    BondDealBusiness bonddeal_business(bondDealService);
	bonddeal_business.SetCompanyId(GetCompanyId());
    bonddeal_business.setRequestMsg(getRequestMsg());
	bool result = true;
    BondQuoteCacheController bondquote_cc;

    for (unsigned int i = 0; i < quote_ids.Size(); ++i)
    {
		ptree each_deal_msg;
        BondQuoteCacheController bond_quote_cc;
        std::string quote_id = quote_ids.Get(i)->ToString().c_str();
        std::string str = "id = '" + quote_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/;
        dbQuery q(str.c_str());
        BondQuoteCachePtr bond_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        if (bond_quote_cache == NULL) {
            continue;
        }

        BondDeal bond_deal;

        bond_deal.id_ = GetNewGuid();
        bond_deal.company_id = bond_quote_cache->company_id;

        bond_deal.bond_key_listed_market_ = bond_quote_cache->bond_key_listed_market;
        bond_deal.price_ = bond_quote_cache->price;
        bond_deal.fan_dian_flag_ = bond_quote_cache->rebate;
        bond_deal.fan_dian_      = bond_quote_cache->return_point;
        bond_deal.quote_type_    = bond_quote_cache->quote_type;

		bond_deal.deal_time_ = GetTDateString("%Y-%m-%d");
        bond_deal.create_time_ = GetCurrentTimeString();
        bond_deal.volume_ = bond_quote_cache->volume;
        bond_deal.yield_type_ = bond_quote_cache->exercise;
        bond_deal.settlement_type_ = bond_quote_cache->clear_speed;
        bond_deal.traded_date_ = GetTDateString("%Y-%m-%d");
        bond_deal.delivery_date_ = GetT1DateString("%Y-%m-%d");
        bond_deal.settlement_date_ = GetT1DateString("%Y-%m-%d");

		bond_deal.deal_status_ = kDcsDealtoBeConfirm;

        // 内部报价生成内部成交
        bond_deal.deal_internal_flag_ = (bond_quote_cache->internally == kQuoteInternal ? kDealInternal : kDealNotInternal);

        //// 佣金初始化为0
        //bond_deal.bid_brokerage_ = 0;
        //bond_deal.ofr_brokerage_ = 0;

        if (bond_quote_cache->symbol == 1) {

            bond_deal.direction_ = "given";
            bond_deal.bid_institution_id_   = bond_quote_cache->bank_id;
            bond_deal.bid_trader_id_ = bond_quote_cache->bank_agent_id;
            bond_deal.bid_broker_id_ = bond_quote_cache->operator_id;

			bond_deal.ofr_broker_id_ = GetAccountId();

        } else {
            bond_deal.direction_ = "taken";
            bond_deal.ofr_institution_id_   = bond_quote_cache->bank_id;
            bond_deal.ofr_trader_id_ = bond_quote_cache->bank_agent_id;
            bond_deal.ofr_broker_id_ = bond_quote_cache->operator_id;

			bond_deal.bid_broker_id_ = GetAccountId();
        }

        sdbus::string deal_text;
		sdbus::string id;
        bonddeal_business.AddBondDeal(bond_deal, &deal_text, &id, &each_deal_msg, false, true);

		/*if (kDcsEnable)
		{
			DcsBondDealService *dcsBondDealService = (DcsBondDealService*)ServiceManager::singleton()->getService("DcsBondDeal");
			DcsBondDealBusiness dcsbonddeal_business(dcsBondDealService);
			dcsbonddeal_business.SetCompanyId(GetCompanyId());
			dcsbonddeal_business.SetAccountId(GetAccountId());

			bool dcs_res = dcsbonddeal_business.AddBondDeal(bond_deal);
			if (!dcs_res)
			{
				result = false;
			}
		}*/

		if(deal_id_list)
		{
			sdbus::Variant* v = deal_id_list->AddVariant();
			v->SetString(id);
		}
        
		if (exported_text)
		{
			exported_text->append(deal_text);
		}

        if (gvntkn_msg != NULL) {
			gvntkn_msg->push_back(std::make_pair("", each_deal_msg));
		}
    }

	return result;
}

sdbus::string BondQuoteBusiness::CopyQuote(sdbus::VariantList quote_ids) 
{
    /*sdbus::string quoteString;
    if (GetCompanyId() == kCompanyTP)
		quoteString = CopyQuoteForTP(quote_ids);
    else if (GetCompanyId() == kCompanyICAP)
		quoteString = CopyQuoteForICAP(quote_ids);
    else if (GetCompanyId() == kCompanyPATR)
		quoteString = CopyQuoteForPATR(quote_ids);
    else if (GetCompanyId() == kCompanyBGC)
		quoteString = CopyQuoteForBGC(quote_ids);
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
    else if (GetCompanyId() == kCompanyPATR)
		refQuoteString = CopyReferredQuoteForPATR(quote_ids);
    else if (GetCompanyId() == kCompanyBGC)
		refQuoteString = CopyQuoteForBGC(quote_ids);
	else
		refQuoteString = CopyReferredQuoteForTP(quote_ids);

    return refQuoteString;*/
    return CopyReferredQuoteForTP(quote_ids);
}

sdbus::string BondQuoteBusiness::CopyQuoteForTP(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (unsigned int i = 0; i < quote_ids.Size(); ++i)
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

sdbus::string BondQuoteBusiness::CopyQuoteForICAP(sdbus::VariantList quote_ids) 
{
    std::string quotesTexts;

    for (unsigned int i = 0; i < quote_ids.Size(); ++i)
    {
        BondQuoteCacheController bond_quote_cc;
        dbQuery q;
        sdbus::string quote_id = quote_ids.Get(i)->ToString().c_str();
        q = "id = ", quote_id, " and company_id = ", kCompanyICAP; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr bond_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        if (bond_quote_cache == NULL)
        {
            continue;
        }

        // --- 剩余期限 ---
        std::string ttm;
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            ttm = getExpTTM(bond_quote_cache->time_to_maturity, 8);
        }
        else
        {
            ttm = bond_quote_cache->time_to_maturity;
        }

        std::transform (ttm.begin(), ttm.end(), ttm.begin(), ::tolower);
        quotesTexts =  quotesTexts + ttm + " ";

        // --- 浮息 ----
        if (bond_quote_cache->rate_index.length() > 0  &&  bond_quote_cache->rate_index != "FIXED")
        {
            if (isBCOCategoryType(bond_quote_cache->bond_category))
            {
                quotesTexts = quotesTexts + getRateIndex(bond_quote_cache->rate_index, 8, kCompanyICAP) + " ";
            }
            else
            {
                quotesTexts = quotesTexts + getRateIndex(bond_quote_cache->rate_index, 0, kCompanyICAP) + " ";
            }          
        }

        // --- 代码 ---
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id, 10) + " ";
        }
        else
        {
            quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id, 0) + " ";
        }
        
        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(bond_quote_cache->bond_short_name) + " ";
        }

        // --- 评级 ----
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            std::string ratings= getExpRatings(bond_quote_cache->issuer_rating, bond_quote_cache->bond_rating, kCompanyICAP);
            std::transform (ratings.begin(), ratings.end(), ratings.begin(), ::tolower);
            quotesTexts = quotesTexts + ratings + " ";
        }
        
        // --- 价格(备注) ---
        std::string sPrice = getExpPrice(bond_quote_cache->price, bond_quote_cache->rebate, bond_quote_cache->return_point, bond_quote_cache->symbol);
        std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description, kCompanyICAP); 
        std::string priceAndNotes = getExpPriceAndNotes(sPrice, sNotes, bond_quote_cache->symbol, kCompanyICAP);

        quotesTexts = quotesTexts + priceAndNotes + " "; 

        // --- 交易量 ---
        quotesTexts = quotesTexts + getExpVolume(bond_quote_cache->volume, bond_quote_cache->symbol, 0, kCompanyICAP) + " ";
        
        // --- 还本方式 ---
        if (bond_quote_cache->redemption_no > 0)
        {
            quotesTexts = quotesTexts + bond_quote_cache->redemption_no_client;
        }
        
        // --- 担保 ----
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            if (bond_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + GBKToUTF8("有担保");
            }
        }

        quotesTexts = quotesTexts + "\n";

    }

    return quotesTexts;
}

sdbus::string BondQuoteBusiness::CopyQuoteForBGC(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (unsigned int i = 0; i < quote_ids.Size(); ++i)
    {
        BondQuoteCacheController bond_quote_cc;
        dbQuery q;
        sdbus::string quote_id = quote_ids.Get(i)->ToString().c_str();
        q = "id = ", quote_id, " and company_id = ", kCompanyBGC; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr bond_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        if (bond_quote_cache == NULL) {
            continue;
        }

        // --- 新发 ---
        quotesTexts =  quotesTexts + (bond_quote_cache->market_type == "1" ? GBKToUTF8("新发") + " " : ""); 
        
        // --- 剩余期限 ---
        quotesTexts =  quotesTexts + getExpTTM(bond_quote_cache->time_to_maturity, bond_quote_cache->maturity_holidays) + " ";

        // --- 浮息 ----
        if (bond_quote_cache->rate_index.length() > 0  &&  bond_quote_cache->rate_index != "FIXED")
        {
             if ( isBCOCategoryType(bond_quote_cache->bond_category) )
             {
                 quotesTexts = quotesTexts + getRateIndex(bond_quote_cache->rate_index + GBKToUTF8("浮动"), 17, kCompanyBGC) + " ";
             }
             else
             {
                 quotesTexts = quotesTexts + getRateIndex(bond_quote_cache->rate_index, 13, kCompanyBGC) + " ";
             }       
        }
        
        // --- 代码(上市日期) ---
        quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id, 10) + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(bond_quote_cache->bond_short_name, 18) + " ";
        }

        // 意向债
        if (bond_quote_cache->market_type == "1")
        {
            quotesTexts = quotesTexts + "("  + getExpListedDate(bond_quote_cache->listed_date) + ")" + " ";
        }

        // --- 价格(备注) ---
        std::string sPrice = getExpPrice(bond_quote_cache->price, bond_quote_cache->rebate, bond_quote_cache->return_point, bond_quote_cache->symbol);
        std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
        std::string priceAndNotes = getExpPriceAndNotes(sPrice, sNotes, bond_quote_cache->symbol);

        quotesTexts = quotesTexts + priceAndNotes + "  "; // 价格后面空两格

        // --- 交易量 ---
        quotesTexts = quotesTexts + getExpVolume(bond_quote_cache->volume, bond_quote_cache->symbol, 8, kCompanyBGC) + " ";

        // --- 评级 ----
        quotesTexts = quotesTexts + getExpRatings(bond_quote_cache->issuer_rating, bond_quote_cache->bond_rating);

        // --- 担保 ----
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            if (bond_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + ";" + GBKToUTF8("有担保");
            }
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondQuoteBusiness::CopyQuoteForPATR(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (unsigned int i = 0; i < quote_ids.Size(); ++i)
    {
        BondQuoteCacheController bond_quote_cc;
        dbQuery q;
        sdbus::string quote_id = quote_ids.Get(i)->ToString().c_str();
        q = "id = ", quote_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteCachePtr bond_quote_cache = bond_quote_cc.getCacheByQueryInThreadSafty(q);

        if (bond_quote_cache == NULL)
        {
            continue;
        }
        // --- 剩余期限(月) ---
        quotesTexts =  quotesTexts + getExpTTMInMonth(bond_quote_cache->time_to_maturity, 8, bond_quote_cache->has_option == "Y") + " ";

        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(bond_quote_cache->bond_short_name) + " ";
        }

        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id) + " ";

        // --- 评级 ----
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpRatings(bond_quote_cache->issuer_rating, bond_quote_cache->bond_rating, kCompanyPATR) + " ";
        }
        
        // --- 价格(备注) ---
        std::string sPrice = getExpPrice(bond_quote_cache->price, bond_quote_cache->rebate, bond_quote_cache->return_point, bond_quote_cache->symbol);
        std::string sNotes = getExpNotes(bond_quote_cache->flag_bargain, bond_quote_cache->flag_relation, bond_quote_cache->price_description); 
        std::string priceAndNotes = getExpPriceAndNotes(sPrice, sNotes, bond_quote_cache->symbol);

        quotesTexts = quotesTexts + priceAndNotes + "  "; // 价格后面空两格

        // --- 交易量 ---
        quotesTexts = quotesTexts + getExpVolume(bond_quote_cache->volume, bond_quote_cache->symbol, 0, kCompanyPATR) + "    ";


        // --- 担保 ----
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            if (bond_quote_cache->guarenteed == "Y")
            {
                quotesTexts = quotesTexts + GBKToUTF8("有担保");
            }
        }

        // --- 浮息 ----
        if (bond_quote_cache->rate_index.length() > 0  &&  bond_quote_cache->rate_index != "FIXED")
        {        
             if ( isBCOCategoryType(bond_quote_cache->bond_category) )
             {
                quotesTexts = quotesTexts + (bond_quote_cache->guarenteed == "Y" ? ";" : "") + getRateIndex(bond_quote_cache->rate_index, 0, kCompanyPATR) + GBKToUTF8("浮动");
             }
             else
             {
                 quotesTexts = quotesTexts + (bond_quote_cache->guarenteed == "Y" ? ";" : "") + getRateIndex(bond_quote_cache->rate_index, 0, kCompanyPATR);
             }           
        }

        // --- 上市日期 ---
        if (bond_quote_cache->market_type == "1" && bond_quote_cache->listed_date != 0)
        {
            std::string listed_date = IntToString(bond_quote_cache->listed_date);   
            if (listed_date.length() == 8)
            {
                quotesTexts = quotesTexts + "  " + listed_date.substr(4, 4);//只取mmdd
            }
        }

        // --- Callable Bond ---
        if (bond_quote_cache->has_option == "Y")
        {
            dbQuery q;
            q = "bond_key_listed_market = ", bond_quote_cache->bond_key_listed_market, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
            BondSmallViewCacheController bond_small_view_cc;
            BondSmallViewCachePtr small_view_cache = bond_small_view_cc.getCacheByQueryInThreadSafty(q);
            if (small_view_cache && small_view_cache->option_type == "CAL")
            {
                quotesTexts = quotesTexts + " " + GBKToUTF8("赎回权");
            }
            
        }
        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

sdbus::string BondQuoteBusiness::CopyReferredQuoteForTP(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (unsigned int i = 0; i < quote_ids.Size(); ++i)
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
        quotesTexts = quotesTexts + getExpTTM(bond_quote_cache->time_to_maturity, bond_quote_cache->maturity_holidays, 0) + "\t";

        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(bond_quote_cache->bond_short_name, 0) + "\t";
        }

        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id, 0) + "\t";

        // --- 价格(备注) ---
        std::string sPrice = getExpPrice(bond_quote_cache->price, bond_quote_cache->rebate, bond_quote_cache->return_point, bond_quote_cache->symbol, 3);
        quotesTexts = quotesTexts + sPrice + "\t"; 

        // --- 报价类型 ---
        if (bond_quote_cache->symbol == 1) // Bid
        {
            quotesTexts = quotesTexts + "bid" + "\t";
        }
        else
        {
            quotesTexts = quotesTexts + "ofr" + "\t";
        }

        // --- 交易操作 ---
        quotesTexts = quotesTexts + "ref" + "\n";
   
    }

    return quotesTexts;
}

sdbus::string BondQuoteBusiness::CopyReferredQuoteForPATR(sdbus::VariantList quote_ids) 
{
    sdbus::string quotesTexts;

    for (unsigned int i = 0; i < quote_ids.Size(); ++i)
    {
        BondQuoteReferCacheController bondquoterefer_cc;
        dbQuery q;
        sdbus::string quote_id = quote_ids.Get(i)->ToString().c_str();
        q = "id = ", quote_id, " and company_id = ", kCompanyPATR; //Modify by Young 05-Apr-2016
        BondQuoteReferCachePtr bond_quote_cache = bondquoterefer_cc.getCacheByQueryInThreadSafty(q);

        if (bond_quote_cache == NULL) {
            continue;
        }
        // --- 信用债::简称 
        if ( isBCOCategoryType(bond_quote_cache->bond_category) )
        {
            quotesTexts = quotesTexts + getExpBondShortName(bond_quote_cache->bond_short_name, 18) + " ";
        }

        // --- 剩余期限 ---l;
        quotesTexts =  quotesTexts + getExpTTM(bond_quote_cache->time_to_maturity, 7) + " ";


        // --- 代码 ---
        quotesTexts = quotesTexts + getExpBondCode(bond_quote_cache->bond_id, 8) + " ";

        // --- 价格(备注) ---
        std::string sPrice = getExpPrice(bond_quote_cache->price, bond_quote_cache->rebate, bond_quote_cache->return_point, bond_quote_cache->symbol, 2, 9);
        quotesTexts = quotesTexts + sPrice + " "; 

        // --- 报价类型 ---
        if (bond_quote_cache->symbol == 1) // Bid
        {
            quotesTexts = quotesTexts + "bid" + "\t";
        }
        else
        {
            quotesTexts = quotesTexts + "ofr" + "\t";
        }

        // --- 交易操作 ---
        quotesTexts = quotesTexts + "ref" + "\t";

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}

bool BondQuoteBusiness::UpdateYiJiBanRate(const YiJiBanBondPtr &yijiban_bond_ptr)
{
    std::ostringstream oss;
    oss << "select coupon_rate_tmp as CouponRate, rate_tmp as AvgPrice, rebate_tmp as FanDian from bond_temporary_rate where bond_key = "
        << "\'" << yijiban_bond_ptr->bond_key_ << "\' and "
        << "status = 1 and create_time > \'" << GetTDateString("%Y-%m-%d") << "\'";    

    sql::ResultSet* res = DBConnector::getInstance()->executeQuery(oss.str(), SCHEMA_IDB_BOND);
    if (res == NULL) {
        return false;
    }

    // 数据库字段映射 rate-avgprice rebate-fandian couponrate-couponrate
    if (res->next())
    {
        oss.str("");
        oss << "update bond_temporary_rate set rate_tmp = "
            << "\'" << yijiban_bond_ptr->avg_price_ << "\'" << ", "
            << "coupon_rate_tmp = " << "\'" << yijiban_bond_ptr->coupon_rate_ << "\'" << ", "
            << "rebate_tmp = " << "\'" << yijiban_bond_ptr->fandian_ << "\'" << ", "
            << "modify_time = " << "\'" << GetCurrentTimeString() << "\' "
            << "where bond_key = " << "\'" << yijiban_bond_ptr->bond_key_ << "\' and "
            << "status = 1";

        DBConnector::getInstance()->executeUpdate(oss.str(), SCHEMA_IDB_BOND);
    }
    else
    {
        oss.str("");
        oss << "insert into bond_temporary_rate ( Id, bond_key, status, create_time, coupon_rate_tmp, rate_tmp, rebate_tmp ) values ("
            << "\'" << GetNewGuid() << "\'" << ", "
            << "\'" << yijiban_bond_ptr->bond_key_ << "\'" << ", " 
            << 1 << ", "
            << "\'" << GetCurrentTimeString() << "\'" << ", "
            << "\'" << yijiban_bond_ptr->coupon_rate_ << "\'" << ", "
            << "\'" << yijiban_bond_ptr->avg_price_ << "\'" << ", "
            << "\'" << yijiban_bond_ptr->fandian_ << "\'" << ")" << "\0";     
        
        DBConnector::getInstance()->executeUpdate(oss.str(), SCHEMA_IDB_BOND);     
    }

    delete res;
    return true;
}

YiJiBanBondPtr BondQuoteBusiness::FetchYiJiBanBond(const sdbus::string &bond_key)
{
    std::ostringstream oss;
    oss << "select coupon_rate_tmp as CouponRate, rate_tmp as AvgPrice, rebate_tmp as FanDian from bond_temporary_rate where bond_key = "
        << "\'" << bond_key << "\' and "
        << "status = 1 and create_time > \'" << GetTDateString("%Y-%m-%d") << "\'";    

    sql::ResultSet* res = DBConnector::getInstance()->executeQuery(oss.str(), SCHEMA_IDB_BOND);   
    if (res == NULL) {
        return YiJiBanBondPtr();
    }

    YiJiBanBondPtr yijibanbond(new YiJiBanBond());    
    if (res->next()) {        
        yijibanbond->avg_price_ = res->getString("AvgPrice");
        yijibanbond->coupon_rate_ = res->getString("CouponRate");
        yijibanbond->fandian_ = res->getString("FanDian");
    }

    delete res;
    return yijibanbond;
}