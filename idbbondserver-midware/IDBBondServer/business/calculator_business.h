#ifndef _CALCULATOR_BUSINESS_H_
#define _CALCULATOR_BUSINESS_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif
#include "logger.h"
#include "include/common.h"
#include "cache/model/bondquote_cache.h"
#include "cache/model/bondquoterefer_cache.h"
#include "cache/model/dcsbonddeal_cache.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016

#include "include/fid.h"
#include "include/constants.h"
#include "sdbus/connection.h"
#include "cache/cache_center.h"
#include "service/send_service.h"
#include "service/service_manager.h"
#include "business/bondquote_business.h"
#include "msg2db/msg_log_service.h"

typedef struct {
    // in
    std::string id;
    std::string quote_type;
    std::string bond_key;
    std::string listed_market;
    std::string trade_date;
    int yield_type;
    double price;
    double fandian;
    bool fandian_flag;

    double coupon_rate; // 意向债临时票面利率

    // out    
    double clean_price;
    double dirty_price;
    double yield;
    double spread;
} CalcData;
typedef std::tr1::shared_ptr<CalcData> CalcDataPtr;

namespace sdbus {
    class VariantMap;
}

class CalculatorBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    CalculatorBusiness();

    template<class T> bool Calc(T* cache)
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

        // huameng jia  2015.04.24
        //意向债(一级半)的券在调用计算器规则调整如下:
        //1. 调用计算器接口需要额外带上票面利率(临时)参数 FID_COUPON_RATE.  
        //2. 报价类型为收益率时,   如果用户输入价格为空, 取交易均价作为价格;  非空则取用户输入价格
        //3. 返点按钮激活时,  如果用户输入返点为空, 取返费作为返点;  非空则取用户输入返点
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

        if (cache->price == DOUBLE_NULL) {
            cache->net_price = DOUBLE_NULL;
            cache->full_price = DOUBLE_NULL;
            cache->yield = DOUBLE_NULL;
            cache->spread = DOUBLE_NULL;
            return true;
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
    template<class T> bool Calc(std::vector<T> &cache_vec)
    {
        time_t lt = time(NULL);
        struct tm *local = localtime(&lt);
        char timeChars[16];              
        strftime(timeChars, 16, "%Y%m%d", local);

        BondQuoteService *bond_quote_service = (BondQuoteService*)ServiceManager::singleton()->getService(kServiceBondQuote);        
        BondQuoteBusiness bond_quote_business(bond_quote_service);

        std::vector<CalcDataPtr> calc_data_vec;
        //std::vector<T>::iterator it = cache_vec.begin();
        //int size = 0;
        //for ( ; it != cache_vec.end(); ++it) {
        for (int i = 0; i < cache_vec.size(); ++i) {
            T cache = cache_vec[i];

            CalcDataPtr calc_data(new CalcData);

            calc_data->id = cache->id;
            calc_data->bond_key = cache->bond_key_listed_market.length() > 3 ? cache->bond_key_listed_market.substr(0, cache->bond_key_listed_market.length() - 3) : "";
            calc_data->listed_market = getListedMarket(cache->bond_key_listed_market);
            calc_data->quote_type = cache->quote_type;
            calc_data->yield_type = atoi(cache->exercise.c_str());
            calc_data->fandian_flag = cache->rebate == "1";
            calc_data->fandian = cache->return_point;
            calc_data->price = cache->price;
            calc_data->trade_date = timeChars;

            calc_data->coupon_rate = DOUBLE_NULL;

            calc_data->clean_price = DOUBLE_NULL;
            calc_data->dirty_price = DOUBLE_NULL;
            calc_data->yield = DOUBLE_NULL;
            calc_data->spread = DOUBLE_NULL;

            // huameng jia  2015.04.24
            //意向债(一级半)的券在调用计算器规则调整
            if ("1" == cache->market_type ) {
                YiJiBanBondPtr yijiban_bond = bond_quote_business.FetchYiJiBanBond(calc_data->bond_key);
                if (yijiban_bond) {
                    if (yijiban_bond->coupon_rate_.length() > 0) {
                        calc_data->coupon_rate = atof(yijiban_bond->coupon_rate_.c_str()) * 0.01;
                    }

                    if (yijiban_bond->avg_price_.length() > 0 && calc_data->price == DOUBLE_NULL && (calc_data->quote_type == "3" || calc_data->quote_type == "0" || calc_data->quote_type == "8")) {                
                        calc_data->price = atof(yijiban_bond->avg_price_.c_str()) * 0.01;
                    }

                    if (yijiban_bond->fandian_.length() > 0 && calc_data->fandian == DOUBLE_NULL && calc_data->fandian_flag) {
                        calc_data->fandian = atof(yijiban_bond->fandian_.c_str());
                    }
                }
            }

            if (cache->price == DOUBLE_NULL) {
                cache->net_price = DOUBLE_NULL;
                cache->full_price = DOUBLE_NULL;
                cache->yield = DOUBLE_NULL;
                cache->spread = DOUBLE_NULL;
                continue;
            }

            calc_data_vec.push_back(calc_data);
        }

        if (calc_data_vec.size() == 0) {
            return true;
        }

        if (!DoBatchCalculation(calc_data_vec)) {        
            //for (it = cache_vec.begin(); it != cache_vec.end(); ++it) {            
            for (size_t i = 0; i < cache_vec.size(); ++i) {            
                T cache = cache_vec[i];        
                cache->net_price = DOUBLE_NULL;
                cache->full_price = DOUBLE_NULL;
                cache->yield = DOUBLE_NULL;
                cache->spread = DOUBLE_NULL;
            }

            return false;
        }

        for (unsigned int i = 0; i < calc_data_vec.size(); ++i) {
            CalcDataPtr calc_data = calc_data_vec.at(i);
            if (calc_data != NULL) {
                for (size_t i = 0; i < cache_vec.size(); ++i) {
                    T cache = cache_vec[i];
                    if (cache->id == calc_data->id) {
                        cache->net_price = Round(calc_data->clean_price, 4);
                        cache->full_price = Round(calc_data->dirty_price, 4);
                        cache->yield = Round(calc_data->yield, 4);
                        cache->spread = Round(calc_data->spread, 4);
                        break;
                    }
                }
            }

        }

        return true;
    }
    bool Calc(BondQuoteReferCache* cache);
    bool Calc(DcsBondDealCache* cache);

private:
    bool DoCalculation(CalcData *calc_data);
    bool DoBatchCalculation(std::vector<CalcDataPtr> &calc_data_vec);

	std::string getListedMarket(const std::string& bond_key_listed_market);

};

//template<T> bool CalculatorBusiness::Calc(T* cache)
//{
//}
//
//template<class T> bool CalculatorBusiness::Calc(std::vector<T> &cache_vec)
//{
//}


#endif // _CALCULATOR_BUSINESS_H_