#include "calculator_business.h"

CalculatorBusiness::CalculatorBusiness()
{

}

//bool CalculatorBusiness::Calc(BondQuoteCache* cache)
//{
//    time_t lt = time(NULL);
//    struct tm *local = localtime(&lt);
//    char timeChars[16];              
//    strftime(timeChars, 16, "%Y%m%d", local);
//
//
//    CalcData calc_data;
//
//    calc_data.bond_key = cache->bond_key_listed_market.length() > 3 ? cache->bond_key_listed_market.substr(0, cache->bond_key_listed_market.length() - 3) : "";
//    calc_data.listed_market = getListedMarket(cache->bond_key_listed_market);
//    calc_data.quote_type = cache->quote_type;
//    calc_data.yield_type = atoi(cache->exercise.c_str());
//    calc_data.fandian_flag = cache->rebate == "1";
//    calc_data.fandian = cache->return_point;
//    calc_data.price = cache->price;
//    calc_data.trade_date = timeChars;
//
//    calc_data.coupon_rate = DOUBLE_NULL;
//
//    calc_data.clean_price = DOUBLE_NULL;
//    calc_data.dirty_price = DOUBLE_NULL;
//    calc_data.yield = DOUBLE_NULL;
//    calc_data.spread = DOUBLE_NULL;
//
//    // huameng jia  2015.04.24
//    //意向债(一级半)的券在调用计算器规则调整如下:
//    //1. 调用计算器接口需要额外带上票面利率(临时)参数 FID_COUPON_RATE.  
//    //2. 报价类型为收益率时,   如果用户输入价格为空, 取交易均价作为价格;  非空则取用户输入价格
//    //3. 返点按钮激活时,  如果用户输入返点为空, 取返费作为返点;  非空则取用户输入返点
//    if ("1" == cache->market_type ) {
//        BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);        
//        BondQuoteBusiness bond_quote_business(bond_quote_service);
//
//        YiJiBanBondPtr yijiban_bond = bond_quote_business.FetchYiJiBanBond(calc_data.bond_key);
//        if (yijiban_bond) {
//            if (yijiban_bond->coupon_rate_.length() > 0) {
//                calc_data.coupon_rate = atof(yijiban_bond->coupon_rate_.c_str()) * 0.01;
//            }
//
//            if (yijiban_bond->avg_price_.length() > 0 && calc_data.price == DOUBLE_NULL && (calc_data.quote_type == "3" || calc_data.quote_type == "0" || calc_data.quote_type == "8")) {                
//                calc_data.price = atof(yijiban_bond->avg_price_.c_str()) * 0.01;
//            }
//
//            if (yijiban_bond->fandian_.length() > 0 && calc_data.fandian == DOUBLE_NULL && calc_data.fandian_flag) {
//                calc_data.fandian = atof(yijiban_bond->fandian_.c_str());
//            }
//        }
//    }
//
//    if (cache->price == DOUBLE_NULL) {
//        cache->net_price = DOUBLE_NULL;
//        cache->full_price = DOUBLE_NULL;
//        cache->yield = DOUBLE_NULL;
//        cache->spread = DOUBLE_NULL;
//        return true;
//    }
//
//    if (!DoCalculation(&calc_data)) {
//        cache->net_price = DOUBLE_NULL;
//        cache->full_price = DOUBLE_NULL;
//        cache->yield = DOUBLE_NULL;
//        cache->spread = DOUBLE_NULL;
//        return false;
//    }
//
//    cache->net_price = Round(calc_data.clean_price, 4);
//    cache->full_price = Round(calc_data.dirty_price, 4);
//    cache->yield = Round(calc_data.yield, 4);
//    cache->spread = Round(calc_data.spread, 4);
//
//    return true;
//}


//bool CalculatorBusiness::Calc(std::vector<BondQuoteCachePtr> &cache_vec)
//{
//    time_t lt = time(NULL);
//    struct tm *local = localtime(&lt);
//    char timeChars[16];              
//    strftime(timeChars, 16, "%Y%m%d", local);
//
//    BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);        
//    BondQuoteBusiness bond_quote_business(bond_quote_service);
//
//    std::vector<CalcDataPtr> calc_data_vec;
//    std::vector<BondQuoteCachePtr>::iterator it = cache_vec.begin();
//    int size = 0;
//    for ( ; it != cache_vec.end(); ++it) {
//        BondQuoteCachePtr cache = *it;
//       
//        CalcDataPtr calc_data(new CalcData);
//
//        calc_data->id = cache->id;
//        calc_data->bond_key = cache->bond_key_listed_market.length() > 3 ? cache->bond_key_listed_market.substr(0, cache->bond_key_listed_market.length() - 3) : "";
//        calc_data->listed_market = getListedMarket(cache->bond_key_listed_market);
//        calc_data->quote_type = cache->quote_type;
//        calc_data->yield_type = atoi(cache->exercise.c_str());
//        calc_data->fandian_flag = cache->rebate == "1";
//        calc_data->fandian = cache->return_point;
//        calc_data->price = cache->price;
//        calc_data->trade_date = timeChars;
//
//        calc_data->coupon_rate = DOUBLE_NULL;
//
//        calc_data->clean_price = DOUBLE_NULL;
//        calc_data->dirty_price = DOUBLE_NULL;
//        calc_data->yield = DOUBLE_NULL;
//        calc_data->spread = DOUBLE_NULL;
//
//        // huameng jia  2015.04.24
//        //意向债(一级半)的券在调用计算器规则调整
//        if ("1" == cache->market_type ) {
//            YiJiBanBondPtr yijiban_bond = bond_quote_business.FetchYiJiBanBond(calc_data->bond_key);
//            if (yijiban_bond) {
//                if (yijiban_bond->coupon_rate_.length() > 0) {
//                    calc_data->coupon_rate = atof(yijiban_bond->coupon_rate_.c_str()) * 0.01;
//                }
//
//                if (yijiban_bond->avg_price_.length() > 0 && calc_data->price == DOUBLE_NULL && (calc_data->quote_type == "3" || calc_data->quote_type == "0" || calc_data->quote_type == "8")) {                
//                    calc_data->price = atof(yijiban_bond->avg_price_.c_str()) * 0.01;
//                }
//
//                if (yijiban_bond->fandian_.length() > 0 && calc_data->fandian == DOUBLE_NULL && calc_data->fandian_flag) {
//                    calc_data->fandian = atof(yijiban_bond->fandian_.c_str());
//                }
//            }
//        }
//
//        if (cache->price == DOUBLE_NULL) {
//            cache->net_price = DOUBLE_NULL;
//            cache->full_price = DOUBLE_NULL;
//            cache->yield = DOUBLE_NULL;
//            cache->spread = DOUBLE_NULL;
//            continue;
//        }
//
//        calc_data_vec.push_back(calc_data);
//    }
//
//    if (calc_data_vec.size() == 0) {
//        return true;
//    }
//
//    if (!DoBatchCalculation(calc_data_vec)) {        
//        for (it = cache_vec.begin(); it != cache_vec.end(); ++it) {            
//            BondQuoteCachePtr cache = *it;        
//            cache->net_price = DOUBLE_NULL;
//            cache->full_price = DOUBLE_NULL;
//            cache->yield = DOUBLE_NULL;
//            cache->spread = DOUBLE_NULL;
//        }
//
//        return false;
//    }
//
//    for (unsigned int i = 0; i < calc_data_vec.size(); ++i) {
//        CalcDataPtr calc_data = calc_data_vec.at(i);
//        if (calc_data != NULL) {
//            for (it = cache_vec.begin(); it != cache_vec.end(); ++it) {
//                BondQuoteCachePtr cache = *it;
//                if (cache->id == calc_data->id) {
//                    cache->net_price = Round(calc_data->clean_price, 4);
//                    cache->full_price = Round(calc_data->dirty_price, 4);
//                    cache->yield = Round(calc_data->yield, 4);
//                    cache->spread = Round(calc_data->spread, 4);
//                    break;
//                }
//            }
//        }
//
//    }
//
//    return true;
//}

bool CalculatorBusiness::Calc(BondQuoteReferCache* cache)
{
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);
    char timeChars[16];              
    strftime(timeChars, 16, "%Y%m%d", local);


    CalcData calc_data;

    calc_data.bond_key = cache->bond_key_listed_market.length() > 3 ? cache->bond_key_listed_market.substr(0, cache->bond_key_listed_market.length() - 3) : "";
    calc_data.listed_market = getListedMarket(cache->bond_key_listed_market);
    calc_data.quote_type = cache->quote_type;
    calc_data.yield_type = atoi(cache->exercise.c_str());
    calc_data.fandian_flag = cache->rebate == "1";
    calc_data.fandian = cache->return_point;
    calc_data.price = cache->price;
    calc_data.trade_date = timeChars;

    calc_data.coupon_rate = DOUBLE_NULL;

    calc_data.clean_price = DOUBLE_NULL;
    calc_data.dirty_price = DOUBLE_NULL;
    calc_data.yield = DOUBLE_NULL;
    calc_data.spread = DOUBLE_NULL;

    if (cache->price == DOUBLE_NULL) {
        cache->net_price = DOUBLE_NULL;
        cache->full_price = DOUBLE_NULL;
        cache->yield = DOUBLE_NULL;
        cache->spread = DOUBLE_NULL;
        return true;
    }
    
    // huameng jia  2015.04.24
    //意向债(一级半)的券在调用计算器规则调整
    if ("1" == cache->market_type ) {
        BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);        
        BondQuoteBusiness bond_quote_business(bond_quote_service);

        YiJiBanBondPtr yijiban_bond = bond_quote_business.FetchYiJiBanBond(calc_data.bond_key);
        if (yijiban_bond) {
            if (yijiban_bond->coupon_rate_.length() > 0) {
                calc_data.coupon_rate = atof(yijiban_bond->coupon_rate_.c_str()) * 0.01;
            }

            if (yijiban_bond->avg_price_.length() > 0 && calc_data.price == DOUBLE_NULL && (calc_data.quote_type == "3" || calc_data.quote_type == "0" || calc_data.quote_type == "8")) {                
                calc_data.price = atof(yijiban_bond->avg_price_.c_str()) * 0.01;
            }

            if (yijiban_bond->fandian_.length() > 0 && calc_data.fandian == DOUBLE_NULL && calc_data.fandian_flag) {
                calc_data.fandian = atof(yijiban_bond->fandian_.c_str());
            }
        }
    }

    if (!DoCalculation(&calc_data)) {
        cache->net_price = DOUBLE_NULL;
        cache->full_price = DOUBLE_NULL;
        cache->yield = DOUBLE_NULL;
        cache->spread = DOUBLE_NULL;
        return false;
    }

    cache->net_price = Round(calc_data.clean_price, 4);
    cache->full_price = Round(calc_data.dirty_price, 4);
    cache->yield = Round(calc_data.yield, 4);
    cache->spread = Round(calc_data.spread, 4);

    return true;
}

bool CalculatorBusiness::Calc(DcsBondDealCache* cache)
{
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);
    char timeChars[16];              
    strftime(timeChars, 16, "%Y%m%d", local);

    CalcData calc_data;

    calc_data.bond_key = cache->bond_key_listed_market.length() > 3 ? cache->bond_key_listed_market.substr(0, cache->bond_key_listed_market.length() - 3) : "";
    calc_data.listed_market = getListedMarket(cache->bond_key_listed_market);
    calc_data.quote_type = cache->quote_type;
    calc_data.yield_type = atoi(cache->exercise.c_str());
    calc_data.fandian_flag = cache->rebate == "1";
    calc_data.fandian = cache->return_point;
    calc_data.price = cache->price;
    calc_data.trade_date = timeChars;

    calc_data.coupon_rate = DOUBLE_NULL;

    calc_data.clean_price = DOUBLE_NULL;
    calc_data.dirty_price = DOUBLE_NULL;
    calc_data.yield = DOUBLE_NULL;
    calc_data.spread = DOUBLE_NULL;

    if (cache->price == DOUBLE_NULL) {
        cache->net_price = DOUBLE_NULL;
        cache->full_price = DOUBLE_NULL;
        cache->yield = DOUBLE_NULL;
        cache->spread = DOUBLE_NULL;
        return true;
    }
    
    // huameng jia  2015.04.24
    //意向债(一级半)的券在调用计算器规则调整
    if ("1" == cache->market_type ) {
        BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);        
        BondQuoteBusiness bond_quote_business(bond_quote_service);

        YiJiBanBondPtr yijiban_bond = bond_quote_business.FetchYiJiBanBond(calc_data.bond_key);
        if (yijiban_bond) {
            if (yijiban_bond->coupon_rate_.length() > 0) {
                calc_data.coupon_rate = atof(yijiban_bond->coupon_rate_.c_str()) * 0.01;
            }

            if (yijiban_bond->avg_price_.length() > 0 && calc_data.price == DOUBLE_NULL && (calc_data.quote_type == "3" || calc_data.quote_type == "0" || calc_data.quote_type == "8")) {                
                calc_data.price = atof(yijiban_bond->avg_price_.c_str()) * 0.01;
            }

            if (yijiban_bond->fandian_.length() > 0 && calc_data.fandian == DOUBLE_NULL && calc_data.fandian_flag) {
                calc_data.fandian = atof(yijiban_bond->fandian_.c_str());
            }
        }
    }

    if (!DoCalculation(&calc_data)) {
        cache->net_price = DOUBLE_NULL;
        cache->full_price = DOUBLE_NULL;
        cache->yield = DOUBLE_NULL;
        cache->spread = DOUBLE_NULL;
        return false;
    }

    cache->net_price = Round(calc_data.clean_price, 4);
    cache->full_price = Round(calc_data.dirty_price, 4);
    cache->yield = Round(calc_data.yield, 4);
    cache->spread = Round(calc_data.spread, 4);

    return true;
}

bool CalculatorBusiness::DoCalculation(CalcData *calc_data)
{
    sdbus::Message request, response;
    std::string quote_type = "";
    double price = DOUBLE_NULL;

    request.SetType("50189");
    sdbus::VariantList *list = request.AddList(FID_BOND_CALCULATOR);

    sdbus::VariantMap *mp = list->AddMap();

    //mp->SetString(FID_CALENDAR_MARKET_KEY, calc_data->listed_market);
	mp->SetString(FID_LISTED_MARKET, calc_data->listed_market);
    mp->SetString(FID_TRADED_DATE, calc_data->trade_date);
    mp->SetString(FID_TENOR, "1D");
    mp->SetString(FID_BUSINESS_DAY_CONVENTION, "ModifiedFollowing");

    mp->SetString(FID_BOND_KEY, calc_data->bond_key);

    if (calc_data->quote_type == "3" || calc_data->quote_type == "0") {
        if (calc_data->yield_type == 0) {
            mp->SetDouble(FID_YIELD_TO_EXECUTION, calc_data->price * 0.01);
        } else if (calc_data->yield_type == 1) {
            mp->SetDouble(FID_YIELD_TO_MATURITY, calc_data->price * 0.01);
        }
    } else if (calc_data->quote_type == "1") {
        mp->SetDouble(FID_CLEAN_PRICE, calc_data->price);
    } else if (calc_data->quote_type == "2") {
        mp->SetDouble(FID_DIRTY_PRICE, calc_data->price);
    } else if (calc_data->quote_type == "4") {
        mp->SetDouble(FID_SPREAD, calc_data->price * 0.01);
    } else if (calc_data->quote_type == "8") {
        mp->SetDouble(FID_YIELD_TO_MATURITY, calc_data->price * 0.01);
    }

    if (calc_data->fandian != DOUBLE_NULL) {
        mp->SetDouble(FID_FAN_DIAN, calc_data->fandian);
    }

    // huameng jia  2015.04.24
    //意向债(一级半)的券在调用计算器规则调整
    if (calc_data->coupon_rate != DOUBLE_NULL) {
        mp->SetDouble(FID_COUPON_RATE, calc_data->coupon_rate);
    }

    LOGGER_DEBUG("DoCalculation...")

    if (SendService::singleton()->call(kServiceQueueCalculator, request, response, 300)) {
        sdbus::VariantMap *body;
        if (response.RefMap(FID_ACK_MSG_BODY, body)) {
            sdbus::VariantList *list;
            if (body->RefList(FID_BOND_CALCULATOR, list)) {
                const sdbus::VariantMap &calc_ret_map = list->Get(0)->ToMap();

                calc_ret_map.GetDouble(FID_CLEAN_PRICE, calc_data->clean_price);
                calc_ret_map.GetDouble(FID_DIRTY_PRICE, calc_data->dirty_price);

                if (0 == calc_data->yield_type) {
                    calc_ret_map.GetDouble(FID_YIELD_TO_EXECUTION, calc_data->yield);                   
                } else {
                    calc_ret_map.GetDouble(FID_YIELD_TO_MATURITY, calc_data->yield);
                }

                calc_ret_map.GetDouble(FID_SPREAD, calc_data->spread);
            }
        }

        if (calc_data->spread != DOUBLE_NULL) {
            calc_data->spread *= 100;
        }
        if (calc_data->yield != DOUBLE_NULL) {
            calc_data->yield *= 100;
        }

        LOGGER_DEBUG("DoCalculation result: ")
        LOGGER_DEBUG("[clean price]: " << calc_data->clean_price)
        LOGGER_DEBUG("[dirty_price]: " << calc_data->dirty_price)
        LOGGER_DEBUG("[yield]: " << calc_data->yield)
        LOGGER_DEBUG("[spread]: " << calc_data->spread)
		MsgLogService::singleton()->FilterAndSaveMsg(request,response);
		return true;
    } else	{
		sdbus::VariantMap *mp_reply = response.AddMap(FID_ACK_MSG_HEAD);
		mp_reply->SetString(FID_CALCULATOR_ERROR_MESSAGE, "request timeout(300ms)");
	}
	MsgLogService::singleton()->FilterAndSaveMsg(request,response);
    LOGGER_ERROR("DoCalculation Failed.");
    return false;
}

bool CalculatorBusiness::DoBatchCalculation(std::vector<CalcDataPtr> &calc_data_vec)
{
    sdbus::Message request, response;
    std::string quote_type = "";
    double price = DOUBLE_NULL;

    request.SetType("50189");
    sdbus::VariantList *list = request.AddList(FID_BOND_CALCULATOR);

    for (size_t index = 0; index != calc_data_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();

        CalcDataPtr calc_data = calc_data_vec.at(index);

        //mp->SetString(FID_CALENDAR_MARKET_KEY, calc_data->listed_market);
		mp->SetString(FID_LISTED_MARKET, calc_data->listed_market);
        mp->SetString(FID_TRADED_DATE, calc_data->trade_date);
        mp->SetString(FID_TENOR, "1D");
        mp->SetString(FID_BUSINESS_DAY_CONVENTION, "ModifiedFollowing");

        mp->SetString(FID_BOND_KEY, calc_data->bond_key);

        if (calc_data->quote_type == "3" || calc_data->quote_type == "0") {
            if (calc_data->yield_type == 0) {
                mp->SetDouble(FID_YIELD_TO_EXECUTION, calc_data->price * 0.01);
            } else if (calc_data->yield_type == 1) {
                mp->SetDouble(FID_YIELD_TO_MATURITY, calc_data->price * 0.01);
            }
        } else if (calc_data->quote_type == "1") {
            mp->SetDouble(FID_CLEAN_PRICE, calc_data->price);
        } else if (calc_data->quote_type == "2") {
            mp->SetDouble(FID_DIRTY_PRICE, calc_data->price);
        } else if (calc_data->quote_type == "4") {
            mp->SetDouble(FID_SPREAD, calc_data->price * 0.01);
        } else if (calc_data->quote_type == "8") {
            mp->SetDouble(FID_YIELD_TO_MATURITY, calc_data->price * 0.01);
        }

        if (calc_data->fandian != DOUBLE_NULL) {
            mp->SetDouble(FID_FAN_DIAN, calc_data->fandian);
        }

        // huameng jia  2015.04.24
        //意向债(一级半)的券在调用计算器规则调整
        if (calc_data->coupon_rate != DOUBLE_NULL) {
            mp->SetDouble(FID_COUPON_RATE, calc_data->coupon_rate);
        }
    }    

    LOGGER_DEBUG("DoBatchCalculation...")
    LOGGER_DEBUG("Size: " << calc_data_vec.size())
    int timeout = 5 * calc_data_vec.size();
    if (timeout < 300) timeout = 300;

    if (SendService::singleton()->call(kServiceQueueCalculator, request, response, timeout)) {
        sdbus::VariantMap *body;
        if (response.RefMap(FID_ACK_MSG_BODY, body)) {
            sdbus::VariantList *list;
            if (body->RefList(FID_BOND_CALCULATOR, list)) {
                if (list->Size() == calc_data_vec.size()) {                    
                    for (size_t i = 0; i < list->Size(); ++i) {
                        const sdbus::VariantMap &calc_ret_map = list->Get(i)->ToMap();                        
                        CalcDataPtr calc_data = calc_data_vec.at(i);

                        calc_ret_map.GetDouble(FID_CLEAN_PRICE, calc_data->clean_price);
                        calc_ret_map.GetDouble(FID_DIRTY_PRICE, calc_data->dirty_price);

                        if (0 == calc_data->yield_type) {
                            calc_ret_map.GetDouble(FID_YIELD_TO_EXECUTION, calc_data->yield);                   
                        } else {
                            calc_ret_map.GetDouble(FID_YIELD_TO_MATURITY, calc_data->yield);
                        }

                        calc_ret_map.GetDouble(FID_SPREAD, calc_data->spread);
                   
                        if (calc_data->spread != DOUBLE_NULL) {
                            calc_data->spread *= 100;
                        }
                        if (calc_data->yield != DOUBLE_NULL) {
                            calc_data->yield *= 100;
                        }
                    }
					MsgLogService::singleton()->FilterAndSaveMsg(request,response);
                    return true;
                }
            }
        }
		
    } else {
		sdbus::VariantMap *mp_reply = response.AddMap(FID_ACK_MSG_HEAD);
		mp_reply->SetString(FID_CALCULATOR_ERROR_MESSAGE, "request timeout(300ms)");
	}
	MsgLogService::singleton()->FilterAndSaveMsg(request,response);
    LOGGER_ERROR("DoCalculation Failed.");
    return false;
}

std::string CalculatorBusiness::getListedMarket(const std::string& bond_key_listed_market){
	if(bond_key_listed_market.empty() || bond_key_listed_market.length() < 3){
		return "CIB";
	}
	return bond_key_listed_market.substr(bond_key_listed_market.size() - 3);
}