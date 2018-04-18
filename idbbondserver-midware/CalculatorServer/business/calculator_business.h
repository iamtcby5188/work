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

#ifndef BCSERVER_BUSINESS_CALCULATOR_BUSINESS_H_
#define BCSERVER_BUSINESS_CALCULATOR_BUSINESS_H_

#include "include/SSCalculator/SSBasicCalculationPage.h"

#include "business/calculator_data_loader.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

typedef boost::mutex                   Uni_Mutex;  
typedef boost::shared_mutex            WR_Mutex;  
typedef boost::unique_lock<WR_Mutex>   writeLock;  
typedef boost::shared_lock<WR_Mutex>   readLock;  

using ssCalculator::SSBasicCalculationPage;

namespace bcserver_business_calculator {

class CalculatorBusiness {
public:
    CalculatorBusiness() {}

    static void LoadCalculatorData(CalculatorDataLoader::DataSource data_source);
    bool IsFRNBond(const string& bond_key) const;
    bool IsYTCAvailable(const string& bond_Key, const string& settlement_date) const;
    bool IsYTPAvailable(const string& bond_Key, const string& settlement_date) const;
    SSBasicCalculationPage PriceYieldConversion(
        const string& calculate_action_type, 
        bool fandian_flag, 
        const string& quote_type, 
        const string& bond_key, 
        const string& settlement_date, 
        double price, 
        double index_rate, 
        double fandian) const;
    string DateAdvance(
        const string& market, 
        const string& date_str, 
        int period_num,
        const string& period_unit,
        const string& convention) const;

private:
    static WR_Mutex wr_mutex;

};

}  // namespace bcserver_business_calculator

#endif