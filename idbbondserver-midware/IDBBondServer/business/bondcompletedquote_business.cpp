#include "bondcompletedquote_business.h"
#include "bondbestquotecalc_business.h"
#include "bondbestquote_business.h"
#include "include/text_export.h"
#include "include/constants.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "service/service_manager.h"
#include "service/bondbestquote_service.h"

BondCompletedQuoteBusiness::BondCompletedQuoteBusiness()
{
    Init();
}

BondCompletedQuoteBusiness::BondCompletedQuoteBusiness(BondCompletedQuoteService* service)
{
    Init();
}

void BondCompletedQuoteBusiness::Init()
{
    // 设置FID与数据库表字段名对应关系
    SET_FID_MAP(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market)
}

void BondCompletedQuoteBusiness::FetchBidQuote(BondQuoteCache *cache, CompletedQuoteDetail* detail)
{
    detail->bid_quote_id = cache->id;
    detail->bond_key_listed_market = cache->bond_key_listed_market;
    detail->bid_issue_institution_id = cache->bank_id;
    detail->bid_trader_id = cache->bank_agent_id;
    detail->bid_bargain_flag_ = cache->flag_bargain;
    detail->bid_broker_name_ = cache->broker_name;
    detail->bid_description_ = cache->price_description;
    detail->bid_fan_dian_ = cache->return_point;
    detail->bid_fan_dian_flag_ = cache->rebate;
    if (!cache->institution_name.empty() && !cache->trader_name.empty())
        detail->bid_institution_trader_name_ = cache->institution_name + "(" + cache->trader_name + ")";
    detail->bid_internal_flag_ = cache->internally;
    detail->bid_oco_flag_ = cache->flag_relation;
    detail->bid_price_ = cache->price;
    detail->bid_volume_ = boost::numeric_cast<double>(cache->volume);

    // 计算价格字段和返点字段
    std::string price_string, fan_dian_string;
    GetPriceStrings(cache->price, cache->rebate, cache->return_point, cache->symbol, 0, price_string, fan_dian_string);
    detail->bid_price_string_ = price_string;
    detail->bid_fan_dian_string_ = fan_dian_string;
}

void BondCompletedQuoteBusiness::FetchOfrQuote(BondQuoteCache *cache, CompletedQuoteDetail* detail)
{
    detail->ofr_quote_id = cache->id;
    detail->bond_key_listed_market = cache->bond_key_listed_market;
    detail->ofr_issue_institution_id = cache->bank_id;
    detail->ofr_trader_id = cache->bank_agent_id;
    detail->ofr_bargain_flag_ = cache->flag_bargain;
    detail->ofr_broker_name_ = cache->broker_name;
    detail->ofr_description_ = cache->price_description;
    detail->ofr_fan_dian_ = cache->return_point;
    detail->ofr_fan_dian_flag_ = cache->rebate;
    if (!cache->institution_name.empty() && !cache->trader_name.empty())
        detail->ofr_institution_trader_name_ = cache->institution_name + "(" + cache->trader_name + ")";
    detail->ofr_internal_flag_ = cache->internally;
    detail->ofr_oco_flag_ = cache->flag_relation;
    detail->ofr_price_ = cache->price;
    detail->ofr_volume_ = boost::numeric_cast<double>(cache->volume);

    // 计算价格字段和返点字段
    std::string price_string, fan_dian_string;
    GetPriceStrings(cache->price, cache->rebate, cache->return_point, cache->symbol, 0, price_string, fan_dian_string);
    detail->ofr_price_string_ = price_string;
    detail->ofr_fan_dian_string_ = fan_dian_string;
}

CompletedQuoteDetailVecPtr BondCompletedQuoteBusiness::GetCompletedQuoteDetailList(const std::string& cond)
{
    CompletedQuoteDetailVecPtr vec(new std::vector<CompletedQuoteDetailPtr>());

    std::string str = ParseCondition(cond, fid_mp_);
    std::string bond_key_listed_market = ParseCondition(cond, FID_BOND_KEY_LISTED_MARKET);

    if (bond_key_listed_market.length() > 0) {
        std::vector<std::pair<BondQuoteCachePtr, BondQuoteCachePtr> > result;

        BondBestQuoteCalcBusiness bond_best_quote_calc_business;
		bond_best_quote_calc_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016
        int best_price_size = bond_best_quote_calc_business.GetBestQuoteSortWithInternal(bond_key_listed_market, result);

        for (int i = 0; i < result.size(); ++i) {
            BondQuoteCachePtr bid_quote_cache = result[i].first;
            BondQuoteCachePtr ofr_quote_cache = result[i].second;

            CompletedQuoteDetail* detail = new CompletedQuoteDetail(); 
            detail->bid_price_ = INVALIDVALUE; //qml端只要价格小于0,就显示--，可忽略其他字段,故这里只初始化这两字段
            detail->ofr_price_ = INVALIDVALUE;
            detail->bid_volume_ = DOUBLE_NULL;
            detail->ofr_volume_ = DOUBLE_NULL;
            detail->is_best_quote_ = i < best_price_size ? true : false;

            if (bid_quote_cache) {
                FetchBidQuote(bid_quote_cache.get(), detail); 
            }
            if (ofr_quote_cache) {
                FetchOfrQuote(ofr_quote_cache.get(), detail); 
            }

            vec->push_back(CompletedQuoteDetailPtr(detail));
        }
    }

    return vec;
}

ExpTypeVecPtr BondCompletedQuoteBusiness::GetCompanyConfig(const std::string& companyId)
{
    ExpTypeVecPtr vec(new std::vector<int>());
    if (companyId == kCompanyTP){
        vec->push_back(TTM);
        vec->push_back(BOND_CODE);
        vec->push_back(PRICE_NOTES);
        vec->push_back(VOLUMN);
        vec->push_back(RATINGS);
        vec->push_back(FLOATING_RATE);
        vec->push_back(LISTED_DATE);
    }else if(companyId == kCompanyICAP){
        vec->push_back(TTM);
        vec->push_back(FLOATING_RATE);
        vec->push_back(BOND_CODE);
        vec->push_back(PRICE_NOTES);
        vec->push_back(VOLUMN);
        vec->push_back(REPAYMENT);
    }else if(companyId == kCompanyBGC){
        vec->push_back(NEW_ISSUE);
        vec->push_back(TTM);
        vec->push_back(FLOATING_RATE);
        vec->push_back(BONDCODE_AND_LISTEDDATE);
        vec->push_back(PRICE_NOTES);
        vec->push_back(VOLUMN);
        vec->push_back(RATINGS);
    }else if(companyId == kCompanyPATR){
        vec->push_back(TTM_MONTH);
        vec->push_back(BOND_CODE);
        vec->push_back(RATINGS);
        vec->push_back(PRICE_NOTES);
        vec->push_back(VOLUMN);
        vec->push_back(FLOATING_RATE);
        vec->push_back(LISTED_DATE);
    }
    return vec;
}

sdbus::string BondCompletedQuoteBusiness::GetPriceAndNotes(BondBestQuoteCachePtr best_quote_cache, bool isInternalQuote)
{
    std::string bidPrice,bidNotes,ofrPrice,ofrNotes;

    // 逻辑为有内部报价则用内部报价, 否则用外部报价
    if (isInternalQuote && best_quote_cache->internally_bid_offer_id != "--"){
        bidPrice = getExpPrice(best_quote_cache->internally_bid_price, best_quote_cache->internally_bid_rebate, best_quote_cache->internally_bid_return_point, 1);
        bidNotes = getExpNotes(best_quote_cache->bid_flag_bargain, best_quote_cache->bid_flag_relation, best_quote_cache->bid_price_description); 
    }else if (best_quote_cache->bid_offer_id != "--") {
        bidPrice = getExpPrice(best_quote_cache->bid_price, best_quote_cache->bid_rebate, best_quote_cache->bid_return_point, 1);
        bidNotes = getExpNotes(best_quote_cache->bid_flag_bargain, best_quote_cache->bid_flag_relation, best_quote_cache->bid_price_description); 
    }

    if (isInternalQuote && best_quote_cache->internally_ofr_offer_id != "--"){
        ofrPrice = getExpPrice(best_quote_cache->internally_ofr_price, best_quote_cache->internally_ofr_rebate, best_quote_cache->internally_ofr_return_point, 1);
        ofrNotes = getExpNotes(best_quote_cache->ofr_flag_bargain, best_quote_cache->ofr_flag_relation, best_quote_cache->ofr_price_description); 
    }else if (best_quote_cache->ofr_offer_id != "--") {
        ofrPrice = getExpPrice(best_quote_cache->ofr_price, best_quote_cache->ofr_rebate, best_quote_cache->ofr_return_point, 1);
        ofrNotes = getExpNotes(best_quote_cache->ofr_flag_bargain, best_quote_cache->ofr_flag_relation, best_quote_cache->ofr_price_description); 
    }
    return getExpPriceAndNotes(bidPrice, bidNotes, ofrPrice, ofrNotes);
}

sdbus::string BondCompletedQuoteBusiness::GetFieldContent(BondBestQuoteCachePtr best_quote_cache,int fieldType, bool isInternalQuote)
{
    sdbus::string content;
    std::string bidPrice,bidNotes, ofrPrice, ofrNotes;
    std::stringstream ssMaturity;
    double dMaturity;
    switch(fieldType) {
    case NEW_ISSUE:
        content = best_quote_cache->market_type == "1" ? GBKToUTF8("新发") : " ";
        break;
    case TTM:
        content = getExpTTM(best_quote_cache->time_to_maturity, best_quote_cache->maturity_holidays);
        break;
    case BOND_CODE:
        content = getExpBondCode(best_quote_cache->bond_id, 8);
        break;
    case PRICE_NOTES:           
        content = GetPriceAndNotes(best_quote_cache,isInternalQuote);
        break;
    case VOLUMN:
        if (best_quote_cache->bid_offer_id != "--" && best_quote_cache->ofr_offer_id != "--"){
            content = getExpBestVolume(atof(best_quote_cache->bid_volume.c_str()), atof(best_quote_cache->ofr_volume.c_str())) + " "; // 量后面空1格
        }
        else if (best_quote_cache->ofr_offer_id == "--"){
            content = getExpVolume(atof(best_quote_cache->bid_volume.c_str()), 2) + " ";
        }
        else if (best_quote_cache->bid_offer_id == "--"){
            content = getExpVolume(atof(best_quote_cache->ofr_volume.c_str()), 1) + " ";
        }
        break;
    case REPAYMENT:
        content = best_quote_cache->redemption_no_client;
        break;
    case RATINGS:
        content = getExpRatings(best_quote_cache->issuer_rating, best_quote_cache->bond_rating);
        break;
    case FLOATING_RATE:
        if (best_quote_cache->rate_index.length() > 0  &&  best_quote_cache->rate_index != "FIXED"){
            content = best_quote_cache->rate_index;
        }
        break;
    case BONDCODE_AND_LISTEDDATE:
        if (best_quote_cache->market_type == "1"){
            content = getExpBondCode(best_quote_cache->bond_id) + "("  + getExpListedDate(best_quote_cache->listed_date) + ")";
        }else{
            content = getExpBondCode(best_quote_cache->bond_id);
        }
        break;
    case LISTED_DATE:
        if (best_quote_cache->market_type == "1") { // 针对一级半导出上市日期
            content = ";" + getExpListedDate(best_quote_cache->listed_date);
        }
        break;
    case TTM_MONTH:
        dMaturity = Round(best_quote_cache->time_to_maturity_real * 12, 1);
        ssMaturity << dMaturity;
        content =  getExpTTM(ssMaturity.str() + "M", best_quote_cache->maturity_holidays);
    default:
        break;
    }
    return content;
}

sdbus::string BondCompletedQuoteBusiness::CopyCompletedQuote(sdbus::VariantList quote_ids, const sdbus::string& title, const std::string& companyId, bool isInternalQuote) 
{
    sdbus::string quotesTexts;
    int best_quote_count = quote_ids.Size(); 
    BondBestQuoteCacheController best_quote_cc;
    dbQuery q;
    sdbus::string lastTitle = "";
    bool isAllType = false;
    sdbus::string internalTitle = GBKToUTF8("暗盘");

    ExpTypeVecPtr companyConfigVec = GetCompanyConfig(companyId);

    if (companyId == kCompanyICAP) 
        quotesTexts = GBKToUTF8(kCompanyICAPName) + GetCurrentMinuteString() + GBKToUTF8("利率报价") + "\n";
    
    if (title != "ALL") 
        quotesTexts = GetTitle(title) + "\n";       
    else
        isAllType = true;

    for (int i = 0; i < best_quote_count; ++i){            
        std::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        std::string str = "bond_key_listed_market = '" + bond_key_listed_market + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
        q = str.c_str();
        BondBestQuoteCachePtr best_quote_cache = best_quote_cc.getCacheByQueryInThreadSafty(q);

        if (isAllType && lastTitle != best_quote_cache->completed_bond_type){
            quotesTexts = quotesTexts + GetTitle(best_quote_cache->completed_bond_type) + "\n";
            lastTitle = best_quote_cache->completed_bond_type;
        }

        if (isInternalQuote)
            quotesTexts = quotesTexts + internalTitle + " ";

        for (int j = 0; j<companyConfigVec->size(); ++j) {
            quotesTexts = quotesTexts + GetFieldContent(best_quote_cache, companyConfigVec->at(j),isInternalQuote) + " ";
        }

        quotesTexts = quotesTexts + "\n";
    }

    return quotesTexts;
}


sdbus::string BondCompletedQuoteBusiness::CopyCompletedQuote(sdbus::VariantList quote_ids) 
{
	/* Modify by Young 05-Apr-2016
	 * 将kCurrentCompanyId替换为GetCompanyId()
	*/
    sdbus::string quotes_texts;

    if (GetCompanyId() == kCompanyICAP) 
    {
        quotes_texts = GBKToUTF8(kCompanyICAPName) + GetCurrentMinuteString() + GBKToUTF8("利率报价") + "\n";
    }

    BondBestQuoteService *best_quote_service = dynamic_cast<BondBestQuoteService*>(ServiceManager::singleton()->getService("BondBestQuote"));
    if (best_quote_service == NULL)
    {
        return "";
    }

    BondBestQuoteBusiness best_quote_business(best_quote_service);
	best_quote_business.SetCompanyId(GetCompanyId()); //Add by Young 05-Apr-2016

    BondBestQuoteCacheController best_quote_cc;
    sdbus::VariantList list;
    sdbus::string last_title;
    for (int i = 0; i < quote_ids.Size(); ++i)
    {                       
        std::string bond_key_listed_market = quote_ids.Get(i)->ToString().c_str();
        std::string str = "bond_key_listed_market = '" + bond_key_listed_market + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
        dbQuery q(str.c_str());
        BondBestQuoteCachePtr best_quote_cache = best_quote_cc.getCacheByQueryInThreadSafty(q);
        if (best_quote_cache == NULL)
        {
            continue;
        }

        // 不同的金融债，Title不同,并且只导出非内部报价
        if ((last_title.empty() || best_quote_cache->completed_bond_type != last_title)
            && (!best_quote_cache->bid_offer_id.empty() || !best_quote_cache->ofr_offer_id.empty())) 
        {
            quotes_texts = quotes_texts + GetTitle(best_quote_cache->completed_bond_type) + "\n";
            last_title = best_quote_cache->completed_bond_type;
        }

        list.PushBack(quote_ids.Get(i)->ToString().c_str());

        quotes_texts = quotes_texts + best_quote_business.CopyBestQuote(list);
        list.Clear();
    }

    return quotes_texts;
}

sdbus::string BondCompletedQuoteBusiness::GetTitle(const sdbus::string& bond_type)
{
    if (bond_type == GBKToUTF8("金融债")) {
        return GBKToUTF8("金融债 固息");
    } else if (bond_type == GBKToUTF8("金融债(含权)")) {
        return GBKToUTF8("金融债 固息 含权");
    } else if (bond_type == GBKToUTF8("金融债(Shibor)")) {
        return GBKToUTF8("金融债 Shibor");
    } else if (bond_type == GBKToUTF8("金融债(Shibor含权)")) {
        return GBKToUTF8("金融债 Shibor 含权");
    } else if (bond_type == GBKToUTF8("金融债(Depo)")) {
        return GBKToUTF8("金融债 Depo");
    } else if (bond_type == GBKToUTF8("金融债(Depo含权)")) {
        return GBKToUTF8("金融债 Depo 含权");
    } else {
        return bond_type;
    }
}