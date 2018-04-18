/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Common Functions
====================================================================================================================
   Date       Name                                  Description of Change
13-Jun-2014   Huameng Jia                           Initial version
19-Aug-2014   James Xu                              Add CountChineseWords and rename Process to Round
25-Aug-2014   James Xu                              Add GetPriceStrings
26-Sep-2014   James Xu                              Add Split
$HISTORY$
===================================================================================================================*/
#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <map>
#include <vector>
#include "sdbus/string.h"
#include "sdbus/message.h"
#include <boost/date_time/posix_time/posix_time.hpp>  
#define INVALIDVALUE -999

class BondFilter;

std::string GetNewGuid();
std::string& Trim(std::string &s);
void split(const std::string& s, const std::string& delim, std::vector< std::string >* ret);  
std::string ParseCondition(const sdbus::string& cond, int cond_fid);
std::string ParseCondition(const sdbus::string& cond, const std::map<int, std::string>& fid_mp);
std::string ParseSortBy(const sdbus::string& sortby, const std::map<int, std::string>& fid_mp, const std::string& sortby_default);
std::string ParseRange(const sdbus::string& range);
void ParseRange(const sdbus::string& range, int& start, int& cnt);
std::string ParseParam(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range, 
                        const std::map<int, std::string>& fid_mp, const std::string& sortby_default = "");

int ParseRatingStr(const std::string& rating_str);

std::string	ParseRateType(std::string &FRN_Index_ID, int Next_Coupon_Date, int MaturityDate);

std::string GetTimeToMaturity(int maturity_date, int interest_start_date, const std::string& option_type_client);
double GetTimeToMaturityReal(int maturity_date, int interest_start_date, const std::string& option_type_client);
double GetTimeToMaturity(int maturity_date, int interest_start_date);
std::string CheckOnTheRun(int listed_date);
int GetSortIndexByBondType(const std::string &bondtype);
std::string GBKToUTF8(const std::string& strGBK);
std::string Utf8ToAnsi(const sdbus::string &strUTF8);
std::string GetCompletedBondType(const std::string &bondtype, const std::string &frnindexid, const std::string &optiontype);
bool hasOptionType(const std::string &optiontype);

time_t ParseTimeString(const std::string &time_str);
time_t ParseMaturityDateString(const std::string &date_str, const char* format);
std::string GetTimeString(time_t t);
time_t GetCurrentTimeMilliSec();     // 获取毫秒级时间
time_t GetCurrentTimeMicroSec();     // 获取微秒级时间
std::string GetCurrentTimeString();
std::string GetCurrentMinuteString();
std::string GetCurrentHMSString();
std::string GetTDateString(const char* format);
std::string GetTDateString(time_t t, const char* format);
std::string GetT1DateString(const char* format);
std::string GetBeforeDateString(const char* format, int months);

void GetPriceStrings(double price, const std::string& fan_dian_flag, double fan_dian, int symbol, int price_type, std::string& price_string, std::string& fan_dian_strin); 
void GetBestPriceStrings(double price, const std::string &fan_dian_flag, double fan_dian, const std::string &quote_id, int quote_side, std::string &price_string, std::string &fan_dian_string);

int CountChineseWords(const std::string& str);

std::string GetDealStatusString(const std::string& deal_status, const bool bid_checked, const bool ofr_checked);

std::string GetDcsDealStatusString(const std::string& deal_status, bool in_hand);

double Round(double t, int n);
std::string Format(double t, int min, int max = -1);
std::string truncateDouble(double value, const int& percision);

std::string IntToString(int value);
std::string FloatToString(float value);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::string join(std::vector<std::string>& str_arr, std::string delim);
std::string joinSql(std::vector<std::string>& str_arr);

char* allocString(std::string value);
void stringSetter(const char** property, std::string value);
char const* stringGetter(char const* property);
void clearStringProperty(const char** property);

// add by lsq 判断某一天是星期几
size_t getWeekDay(size_t year, size_t month, size_t day);
int GetBondSmallTermDays(int maturity_date, int interest_start_date);
int GetDayDifference(int from_date, int to_date);

int GetMaturityTermInDays(int end_date, std::pair<std::string, std::string>& maturity_term_pair);
std::pair<std::string, std::string> CalculateMaturityTerm(int maturity_date, int option_date);
std::string GetIpAddressFromString(const std::string &msg_string);

// zip sdbus msg
bool zipMessage(const sdbus::Message &msg, sdbus::Message &zip_msg);
bool unzipMessage(const sdbus::Message &zip_msg, sdbus::Message &msg);

bool isBCOCategoryType(const std::string& bond_category);

enum QQ2IDB_ERROR_CODE
{
    QQ2IDB_ERROR_BOND_CODE_IS_EMPTY = 1,
    QQ2IDB_ERROR_BOND_CODE_NOT_FOUND_IN_IDB,
    QQ2IDB_ERROR_BOND_INSTITUTION_IS_EMPTY,
    QQ2IDB_ERROR_BOND_INSTITUTION_NOT_FOUND_IN_IDB,
    QQ2IDB_ERROR_BOND_LISTMARKET_NOT_CORRECT,
    QQ2IDB_ERROR_BOND_KEY_LISTED_MARKET_IS_EMPTY,
    QQ2IDB_ERROR_BOND_TRADER_IS_EMPTY,
    QQ2IDB_ERROR_BOND_TRADER_NOT_FOUND_IN_IDB,
    QQ2IDB_ERROR_BOND_PRICE_IS_NEGATIVE,
    QQ2IDB_ERROR_BOND_VOLUME_IS_NEGATIVE,
    QQ2IDB_ERROR_BOND_CODE_WITH_INVALID_FORMAT,
    QQ2IDB_ERROR_BOND_NOT_FOUND_WITH_MARKET_AND_SHORTNAME_IN_IDB,
    QQ2IDB_ERROR_FOUND_MULTI_BOND_IN_IDB,
    QQ2IDB_ERROR_FOUND_EMPTY_BOND_CODE_WITH_MARKET_AND_SHORTNAME_IN_IDB,
    QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_INSTITUTION,
    QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_TRADER,
    QQ2IDB_ERROR_COULD_NOT_GET_BOND_OR_INSTITUTION_OR_TRADER
};
#define QQ2IDB_ERROR_NAME(CODE) #CODE
std::string getIntToStringWithBracket(int i);

/* 20170101 -> 2017-01-01*/
std::string dateShortToLong(const std::string& shortDate);
/*return yyyy-MM-dd*/
std::string dateToStr(const boost::gregorian::date& date);
/* yyyy-MM-dd*/
boost::gregorian::date strToDate(const std::string& dateStr);
boost::gregorian::date currentDate();
boost::gregorian::date_duration dateDuration(const boost::gregorian::date& start, const boost::gregorian::date& end);
boost::gregorian::date getNextWorkDate(std::vector<std::string> holiday_date_vecs, const boost::gregorian::date& date);
bool lessThan1Year(const std::string& endDate);

void initHolidayCache();
const std::vector<std::string>& getHolidayCache();
void clearHolidayCache();

bool strContains(const std::string& source, const std::string& str);

std::string getDBAccountSchema();
std::string getDBBondSchema();

std::string GetFastdbQuery(const std::string &key, const std::string &val);
std::string GetFastdbQueryByBondKeyListedMarket(const std::string &val, const std::string &companyId);
std::string GetFastdbQueryById(const std::string &val, const std::string &companyId);

void trimSql(std::string& sql);

#endif