/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Text Export Utility class
====================================================================================================================
   Date       Name                                  Description of Change
13-Aug-2014   James Xu                              Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _TEXT_EXPORT_
#define _TEXT_EXPORT_

#include <string>
#include "sdbus/string.h"
#include "include/constants.h"

enum ExpType{NEW_ISSUE, TTM, BOND_SHORT_NAME, BOND_CODE, PRICE_NOTES, VOLUMN, REPAYMENT,RATINGS, FLOATING_RATE, LISTED_DATE, BONDCODE_AND_LISTEDDATE, TTM_MONTH};

//std::string getExpTTM(const std::string& time_to_maturity, int width = 8);

std::string getExpTTM(const std::string& time_to_maturity, const int& days, int width = 8);

std::string getExpTTMInMonth(const std::string& time_to_maturity, int width, bool has_option);

std::string getExpBondShortName(const std::string& bond_short_name, int width = 18);

std::string getExpBondCode(const std::string& bond_code, int width = 10);

std::string getExpPrice(double price, const std::string& fan_dian_flag, double fan_dian, int symbol, int price_type = 0, int width = 0);

std::string getExpNotes(const std::string& bargain_flag, const std::string& oco_flag, const std::string& price_description, const std::string& company_id = kCompanyTP);

std::string getExpVolume(double volume, int symbol, int width = 0, const std::string& company_id = kCompanyTP);

std::string getExpVolume(double volume);

std::string getExpBestVolume(double bid_volume, double ofr_volume);

std::string getExpRatings(const std::string& issuer_rating, const std::string& bond_rating, const std::string& company_id = kCompanyTP);

std::string getExpRatings(const std::string& issuer_rating, int width = 0);

std::string getExpListedDate(int listed_date);

std::string getExpPriceAndNotes(const std::string& price, const std::string& notes, int symbol, const std::string& company_id = kCompanyTP);

std::string getExpPriceAndNotes(const std::string& bid_price, const std::string& bid_notes, const std::string& ofr_price, const std::string& ofr_notes);

std::string getExpPriceNotesVolume(const std::string& bid_price, const std::string& bid_notes, double bid_volume, const std::string& ofr_price, const std::string& ofr_notes, double ofr_volume);

std::string getExpDealDirection(const std::string& deal_direction);

std::string getRateIndex(const std::string& rate_index, int width, const std::string& company_id = kCompanyTP);

std::string getCouponRate(const double& coupon_rate);

std::string getExpMaturityHolidays(const int& days);

#endif