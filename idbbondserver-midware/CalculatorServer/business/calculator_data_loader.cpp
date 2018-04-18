/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    calculator data loader, have different implementations.
====================================================================================================================
   Date       Name                                  Description of Change
13-June-2014  Michael Chen                              Initial version
$HISTORY$
===================================================================================================================*/

#include "calculator_data_loader.h"

#include <fstream>
#include <iostream>

#include "include/SSCalculator/SSTypeConverter.h"
#include "db/dbconnector.h"

using std::fstream;

using ssCalculator::SSTypeConverter;

namespace bcserver_business_calculator {

string formatDBDate(string date_str);

CalculatorDataLoader::CalculatorDataLoader(CalculatorDataLoader::DataSource source) {
  static boost::shared_ptr<CalculatorDataLoader::Impl> ssdbImpl (new CalculatorDataLoader::SsdbImpl);
  static boost::shared_ptr<CalculatorDataLoader::Impl> alldbImpl (new CalculatorDataLoader::AllDbImpl);
  switch(source) {
    case kSSDB:
      impl_ = ssdbImpl;
    break;
    case kALLDB:
      impl_ = alldbImpl;
    break;
    default:
      impl_ = ssdbImpl;
  }
}

vector<SSSingleRawBondFundamentalInfo> CalculatorDataLoader::SsdbImpl::GetBondFundamentalInfoList() const {
  std::cout << "loading bond fundamental info..." << std::endl;
  vector<SSSingleRawBondFundamentalInfo> bond_fundamental_info_list;

    sql::ResultSet* res = DBConnector::getInstance()->executeQuery("select * from bond where delflag = 0", "idb_bond");
    if (res == NULL) {
        return bond_fundamental_info_list;
    }

    while (res->next()) {
        SSSingleRawBondFundamentalInfo info;
        info.Bond_Key_ = res->getString("Bond_Key");
        info.Bond_ID_ = res->getString("Bond_ID");
        info.Interest_Start_Date_ = res->getString("Interest_Start_Date");
        info.Maturity_Date_ = res->getString("Maturity_Date");
        info.Listed_Date_ = res->getString("Listed_Date");
        info.First_Coupon_Date_ = res->getString("First_Coupon_Date");
        info.Option_Type_ = res->getString("Option_Type");
        info.Option_Style_ = res->getString("Option_Style");
        info.Call_No_ = res->getInt("Call_No");
        info.Call_Str_ = res->getString("Call_Str");
        info.Put_No_ = res->getInt("Put_No");
        info.Put_Str_ = res->getString("Put_Str");
        info.First_Exchg_Bond_Key_ = res->getString("First_Exchg_Bond_Key");
        info.First_Exchg_No_ = res->getInt("First_Exchg_No");
        info.First_Exchg_Str_ = res->getString("First_Exchg_Str");
        info.Sec_Exchg_No_ = res->getInt("Sec_Exchg_No");
        info.Sec_Exchg_Year_ = res->getString("Sec_Exchg_Year");
        info.Assign_Trans_Key_ = res->getString("Assign_Trans_Key");
        info.Assign_Trans_No_ = res->getInt("Assign_Trans_No");
        info.Assign_Trans_Str_ = res->getString("Assign_Trans_Str");
        info.Compensate_Rate_ = res->getString("Compensate_Rate");
        info.Compensate_From_ = res->getInt("Compensate_From");
        info.Option_Exercise_ = res->getString("Option_Exercise");
        info.Option_Exercise_Date_ = res->getString("Option_Exercise_Date");
        info.Coupon_Type_ = res->getString("Coupon_Type");
        info.Coupon_Rate_Spread_ = res->getDouble("Coupon_Rate_Spread");
        info.Coupon_Frequency_ = res->getString("Coupon_Frequency").length() > 0 ? res->getString("Coupon_Frequency") : "A";
        info.Compound_Frequency_ = res->getString("Compound_Frequency");
        info.Interest_Basis_ = res->getString("Interest_Basis");
        info.Coupon_Dist_ = res->getString("Coupon_Dist");
        info.FRN_Multiplier_ = res->getDouble("FRN_Multiplier");
        info.FRN_Index_ID_ = res->getString("FRN_Index_ID");
        info.First_Index_Rate_ = res->getDouble("First_Index_Rate");
        info.Fixing_Frequency_ = res->getString("Fixing_Frequency");
        info.Fixing_MA_Days_ = res->getInt("Fixing_MA_Days");
        info.Fixing_Preceds_ = res->getString("Fixing_Preceds");
        info.Fixing_Calendar_Key_ = res->getString("Fixing_Calendar_Key");
        info.Fixing_Tailing_ = res->getString("Fixing_Tailing");
        info.Fixing_Digit_ = res->getInt("Fixing_Digit");
        info.Reset_Effective_ = res->getString("Reset_Effective");
        info.Cap_ = res->getDouble("Cap");
        info.Flr_ = res->getDouble("Flr");
        info.Simple_Compound_ = res->getString("Simple_Compound");
        info.Variable_Schedule_ = res->getString("Variable_Schedule");
        info.Coupon_Day_Adjust_ = res->getString("Coupon_Day_Adjust");
        info.Coupon_Day_DMC_ = res->getString("Coupon_Day_DMC");
        info.Coupon_Calendar_Key_ = res->getString("Coupon_Calendar_Key");
        info.Pricing_Conv_ = res->getString("Pricing_Conv");
        info.Issue_Price_ = res->getDouble("Issue_Price");
        info.Redemption_No_ = res->getInt("Redemption_No");
        info.Redemption_Str_ = res->getString("Redemption_Str");
        info.Maturity_Adjust_ = res->getString("Maturity_Adjust");
        info.Maturity_DMC_ = res->getString("Maturity_DMC");
        info.Maturity_Calendar_Key_ = res->getString("Maturity_Calendar_Key");
        bond_fundamental_info_list.push_back(info);
    }
    
    delete res;

    std::cout << "load bond fundamental info complete." << std::endl;
    return bond_fundamental_info_list;
}

vector<SSSingleInterestRate> CalculatorDataLoader::SsdbImpl::GetInterestRates() const {
    std::cout << "loading interest rates..." << std::endl;
    vector<SSSingleInterestRate> interest_rates;
  
    sql::ResultSet* res = DBConnector::getInstance()->executeQuery("select * from rate_info where delflag = 0", "idb_bond");     
    if (res == NULL) {
        return interest_rates;
    }

    map<string, string> rate_map;
    while (res->next()) {
        rate_map[res->getString("gjk_code")] = res->getString("source_code");
    }
    delete res;

    res = DBConnector::getInstance()->executeQuery("SELECT * FROM `macro_base_index_values` where Index_Value is not NULL and Index_Date >= 20120101", "idb_bond");   
    if (res == NULL) {
        return interest_rates;
    }

    while (res->next()) {
        SSSingleInterestRate rate;
        if(0 == rate_map.count(res->getString("GJK_Code"))){
            continue;
        }
        rate.index_id_ = rate_map[res->getString("GJK_Code")];
        // if the value of Index_Valueis null ,we will abandon this bond 
        if(res->getString("Index_Value") == "") {
            continue; 
        }
        rate.value_ = res->getDouble("Index_Value");
        if(rate.value_ > 10000000){
            continue;
        }
        rate.value_ =  rate.value_/100 ;
        // waiting for remend 
        rate.startDate_ = res->getString("Index_Date");
        rate.endDate_ = rate.startDate_;
        //length of YYYY-MM-DD is 10
        if (rate.startDate_.size() != 10 || rate.endDate_.size() != 10) {
            continue;
        } else {
            //convert YYYY-MM-DD to YYYYMMDD
            rate.startDate_ = rate.startDate_.substr(0,4) + rate.startDate_.substr(5,2) + rate.startDate_.substr(8,2);
            rate.endDate_ = rate.startDate_ ;
        }
        interest_rates.push_back(rate);
    }
    delete res;

    std::cout << "load interest rates complete." << std::endl;
    return interest_rates;
}

typedef struct ST_BOND_HOLIDAY {
    char country[8];
    char market[8];
    int holiday;
    char status[4];
    char flag;
} BOND_HOLIDAY;

vector<SSSingleCalendar> CalculatorDataLoader::SsdbImpl::GetCalendar() const {
  std::cout << "loading calendar info..." << std::endl;

    vector<SSSingleCalendar> calendar_info_list;

    map<string,BOND_HOLIDAY> m_holiday;
    BOND_HOLIDAY item;
    memset(&item,0,sizeof(BOND_HOLIDAY));
    m_holiday.clear();
    char linebuffer[10000]={0};

    //load original bond_holiday file
    std::string fullpath;
    //fullpath.append("..\\data\\Holiday.cod");
    fullpath.append("data\\Holiday.cod");
    fstream file;

    file.open(fullpath.c_str());
    if (file) {
        std::string tmpkey;
        size_t pos = 0;
        while(file.getline(linebuffer,10000)) {
            tmpkey = linebuffer;
            pos = tmpkey.find("|");
            strncpy(item.country, tmpkey.substr(0,pos).c_str(), sizeof(item.country) - 1);
            if(item.country[0] == '\0')
                continue;  //bond_key cannot be null
            tmpkey = tmpkey.substr(pos+1);
            pos = tmpkey.find("|");
            strncpy(item.market, tmpkey.substr(0,pos).c_str(), sizeof(item.market) - 1);
            tmpkey = tmpkey.substr(pos+1);
            pos = tmpkey.find("|");
            item.holiday = atoi(tmpkey.substr(0,pos).c_str());
            tmpkey = tmpkey.substr(pos+1);
            pos = tmpkey.find("|");
            strncpy(item.status, tmpkey.substr(0,pos).c_str(), sizeof(item.status) - 1);
            tmpkey = tmpkey.substr(pos+1);
            pos = tmpkey.find("|");
            item.flag = tmpkey.substr()[0];

            tmpkey = item.country;
            m_holiday[tmpkey] = item;

            string country(item.country);
            string market(item.market);

            char dateStr[9];
            sprintf(dateStr,"%d",item.holiday);
            string date(dateStr);

            string status(item.status);

            SSSingleCalendar holiday;
            holiday.country_ = country;
            holiday.market_ = market;
            holiday.date_ = date;
            holiday.isBusinessDay_ = status == "T" ? true : false;

            calendar_info_list.push_back(holiday);

        }
        file.close();
        std::cout << "load calendar info complete." << std::endl;
    } else {
        std::cout << "load calendar info failed." << std::endl;
    }
    file.clear();
    return calendar_info_list;
}

vector<SSSingleRawBondFundamentalInfo> CalculatorDataLoader::AllDbImpl::GetBondFundamentalInfoList() const {
    boost::shared_ptr<CalculatorDataLoader::Impl> ssdbImpl (new CalculatorDataLoader::SsdbImpl);
    return ssdbImpl->GetBondFundamentalInfoList();
}

vector<SSSingleInterestRate> CalculatorDataLoader::AllDbImpl::GetInterestRates() const {
    boost::shared_ptr<CalculatorDataLoader::Impl> ssdbImpl (new CalculatorDataLoader::SsdbImpl);
    return ssdbImpl->GetInterestRates();
}

vector<SSSingleCalendar> CalculatorDataLoader::AllDbImpl::GetCalendar() const {
    std::cout << "loading calendar info..." << std::endl;
    vector<SSSingleCalendar> calendar_info_list;

    sql::ResultSet* res = DBConnector::getInstance()->executeQuery("select * from holiday_info where delflag = 0", "idb_bond");    
    if (res == NULL) {
        return calendar_info_list;
    }

    SSTypeConverter converter;
    map<string, map<string, map<Date, SSSingleCalendar> > > holiday_map;
    map<string, map<string, Date> > max_date_map;
    map<string, map<string, Date> > min_date_map;

    while (res->next()) {
        SSSingleCalendar holiday;
        holiday.country_ = res->getString("country");
        holiday.market_ = res->getString("market_type");
        string date_str = res->getString("holiday_date");
        holiday.date_ = formatDBDate(date_str);
        Date date = converter.YYYYMMDD2qldate(holiday.date_);
        holiday.isBusinessDay_ = false;
        holiday_map[holiday.country_][holiday.market_][date] = holiday;

        if(max_date_map.count(holiday.country_) == 0) {
            max_date_map[holiday.country_][holiday.market_] = date;
        } else if(max_date_map[holiday.country_].count(holiday.market_) == 0) {
            max_date_map[holiday.country_][holiday.market_] = date;
        }
        if(max_date_map[holiday.country_][holiday.market_] < date) {
            max_date_map[holiday.country_][holiday.market_] = date;
        }

        if(min_date_map.count(holiday.country_) == 0) {
            min_date_map[holiday.country_][holiday.market_] = date;
        } else if(min_date_map[holiday.country_].count(holiday.market_) == 0) {
            min_date_map[holiday.country_][holiday.market_] = date;
        }
        if(min_date_map[holiday.country_][holiday.market_] > date) {
            min_date_map[holiday.country_][holiday.market_] = date;
        }
    }
    delete res;

    for(map<string, map<string, map<Date, SSSingleCalendar> > >::const_iterator it1 = holiday_map.begin(); it1!=holiday_map.end(); it1++) {
        string country = it1->first;
        for(map<string, map<Date, SSSingleCalendar> >::const_iterator it2 = it1->second.begin(); it2!=it1->second.end(); it2++) {
            string market = it2->first;
            for(Date date = min_date_map[country][market]; date <= max_date_map[country][market]; date++) {
                if(holiday_map[country][market].count(date) != 0){
                    calendar_info_list.push_back(holiday_map[country][market][date]);
                } else {
                    SSSingleCalendar holiday;
                    holiday.country_ = country;
                    holiday.market_ = market;
                    string date_str = converter.qldate2YYYYMMDD(date);
                    holiday.date_ = date_str;
                    holiday.isBusinessDay_ = true;
                    calendar_info_list.push_back(holiday);
                }
            }
        }
    }

    std::cout << "load calendar info complete." << std::endl;

    return calendar_info_list;
}

string formatDBDate(string date_str){
    string year = date_str.substr(0, 4);
    string month = date_str.substr(5, 2);
    string day = date_str.substr(8, 2);
    return year + month + day;
}


}  // namespace bcserver_business_calculator
