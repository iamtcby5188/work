/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Calculator Business
====================================================================================================================
   Date       Name                                  Description of Change
13-June-2014  Michael Chen                              Initial version
$HISTORY$
===================================================================================================================*/

#include "calculator_business.h"

#include "include/SSCalculator/SSCalculatorIF.h"

#include "business/calculator_data_loader.h"

using ssCalculator::SSCalculatorIF;

namespace bcserver_business_calculator {

WR_Mutex CalculatorBusiness::wr_mutex;
void CalculatorBusiness::LoadCalculatorData(CalculatorDataLoader::DataSource data_source) {
    CalculatorDataLoader dataLoader = CalculatorDataLoader(data_source);

    vector<SSSingleRawBondFundamentalInfo> fundmentalInfoList = dataLoader.GetBondFundamentalInfoList();
    vector<SSSingleInterestRate> rates = dataLoader.GetInterestRates();
    vector<SSSingleCalendar> calendarInfoList = dataLoader.GetCalendar();

    {
        writeLock lock(wr_mutex);
        SSCalculatorIF::instance()->setBondFundamentalInfo(fundmentalInfoList);
        SSCalculatorIF::instance()->setInterestRate(rates);
        SSCalculatorIF::instance()->setCalendar(calendarInfoList);
    }
}

bool CalculatorBusiness::IsFRNBond(const string& bond_key) const {
    readLock lock(wr_mutex);
    return SSCalculatorIF::instance()->isFRNBond(bond_key);
}

bool CalculatorBusiness::IsYTCAvailable(const string& bond_Key, const string& settlementDate) const {
    readLock lock(wr_mutex);
    return SSCalculatorIF::instance()->isYTCAvailable(bond_Key, settlementDate);
}

bool CalculatorBusiness::IsYTPAvailable(const string& bond_Key, const string& settlementDate) const {
    readLock lock(wr_mutex);
    return SSCalculatorIF::instance()->isYTPAvailable(bond_Key, settlementDate);
}

SSBasicCalculationPage CalculatorBusiness::PriceYieldConversion(
    const string& calculate_action_type, 
    bool fandian_flag, 
    const string& quote_type, 
    const string& bond_key, 
    const string& settlement_date, 
    double price, 
    double index_rate, 
    double fandian) const 
{
    readLock lock(wr_mutex);
    SSBasicCalculationPage basicPage;
    double coupon_rate_spread = INVALID_VALUE;
    if (calculate_action_type == "Initialization") {
        basicPage = SSCalculatorIF::instance()->basicPageInitialize(bond_key, settlement_date);
    } else if (calculate_action_type == "DoCalculation") {
        if (!fandian_flag) {
            if (quote_type=="CP") {
                basicPage = SSCalculatorIF::instance()->cleanPriceToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price);
            } else if (quote_type=="DP") {
                basicPage = SSCalculatorIF::instance()->dirtyPriceToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price);
            } else if (quote_type=="YTM") {
                basicPage = SSCalculatorIF::instance()->ytmToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price);
            } else if (quote_type=="MS") {
                basicPage = SSCalculatorIF::instance()->marketSpreadToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price);
            } else if (quote_type=="YTCP") {
                if (SSCalculatorIF::instance()->isYTCAvailable(bond_key, settlement_date)) {
                    basicPage = SSCalculatorIF::instance()->ytcToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price);
                } else if (SSCalculatorIF::instance()->isYTPAvailable(bond_key, settlement_date)) {
                    basicPage = SSCalculatorIF::instance()->ytpToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price);
                }
            } else {
                basicPage = SSCalculatorIF::instance()->basicPageInitialize(bond_key, settlement_date);
            }
        } else {
            if (quote_type=="CP") {
                basicPage = SSCalculatorIF::instance()->rebateCleanPriceToAll(
                    bond_key, settlement_date, coupon_rate_spread, index_rate, price, fandian);
            } else if (quote_type=="DP") {
                basicPage = SSCalculatorIF::instance()->rebateDirtyPriceToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price, fandian);
            } else if (quote_type=="YTM") {
                basicPage = SSCalculatorIF::instance()->rebateToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price, fandian);
            } else if (quote_type=="MS") {
                basicPage = SSCalculatorIF::instance()->rebateMarketSpreadToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price, fandian);
            } else if (quote_type=="YTCP") {
                if (SSCalculatorIF::instance()->isYTCAvailable(bond_key, settlement_date)) {
                    basicPage = SSCalculatorIF::instance()->rebateYTCToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price, fandian);
                } else if (SSCalculatorIF::instance()->isYTPAvailable(bond_key, settlement_date)) {
                    basicPage = SSCalculatorIF::instance()->rebateYTPToAll(bond_key, settlement_date, coupon_rate_spread, index_rate, price, fandian);
                }
            } else {
                basicPage = SSCalculatorIF::instance()->basicPageInitialize(bond_key, settlement_date);
            }
        }
    } else if (calculate_action_type == "ClickPrevailingRate") {
        basicPage = SSCalculatorIF::instance()->clickPrevailingRate(bond_key, settlement_date, coupon_rate_spread);
    }

    return basicPage;
}

string CalculatorBusiness::DateAdvance(const string& market, const string& date_str, int period_num, const string& period_unit, const string& convention) const {
    readLock lock(wr_mutex);
    std::cout << "Calling DateAdvance method..." << std::endl;
    std::cout << "market:" << market << std::endl;
    std::cout << "date:" << date_str << std::endl;
    std::cout << "period num:" << period_num << std::endl;
    std::cout << "period unit:" << period_unit << std::endl;
    std::cout << "convention:" << convention << std::endl;
    string result = SSCalculatorIF::instance()->dateAdvance(market, date_str, period_num, period_unit, convention);
    std::cout << "exceptions: " << SSCalculatorIF::instance()->exceptions() << std::endl;
    return result;
}

}  // namespace bcserver_business_calculator