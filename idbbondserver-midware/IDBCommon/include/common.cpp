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
#include "fid.h"
#include "common.h"
#include "constants.h"
#include "sdbus/expr.h"
#include "sdbus/codec.h"
#include "sdbus/zip.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <vector>
#include <time.h>
/*

#include "boost/locale/encoding.hpp"*/
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp> 
#include <boost/algorithm/string/replace.hpp>
#include <boost/thread/mutex.hpp>
#include "logger.h"

#include "../config.h"

static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
static const std::string _DATE_FORMAT = "%Y-%m-%d";
static boost::uuids::random_generator rg;
static boost::mutex rg_mutex;
std::string GetNewGuid()
{
	boost::mutex::scoped_lock lock(rg_mutex);
    boost::uuids::uuid u = rg();
	std::string result = boost::lexical_cast<std::string>(u);
	result = boost::replace_all_copy(result, "-", "");
	/*std::string result;
    result.reserve(36);
	
    std::size_t i=0;
    for (boost::uuids::uuid::const_iterator it_data = u.begin(); it_data != u.end(); ++it_data) {
        const size_t hi = ((*it_data) >> 4) & 0x0F;
        result += boost::uuids::detail::to_char(hi);

        const size_t lo = (*it_data) & 0x0F;
        result += boost::uuids::detail::to_char(lo);
    }
	*/
    return result;
}

std::string& Trim(std::string &s)
{
    if (s.empty()) 
    {
        return s;
    }
    
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void split(const std::string& s, const std::string& delim, std::vector< std::string >* ret)  
{  
    size_t last = 0;  
    size_t index = s.find_first_of(delim,last);  
    while (index != std::string::npos)  
    {  
        ret->push_back(s.substr(last, index - last));  
        last = index + 1;  
        index = s.find_first_of(delim, last);  
    }  
    if (index - last > 0)  
    {  
        ret->push_back(s.substr(last, index - last));  
    }  
}  

std::string ParseCondition(const sdbus::string& cond, int cond_fid)
{
    if (cond.length() == 0) {
        return "";
    }

    sdbus::Expr expr;
    bool flag = expr.Parse(cond);

    if (!flag) {
        return "";
    }

    std::string cond_str;
    std::string temp_str;

    std::stack<std::string> st;

    const sdbus::ExprNode* node = expr.First();
    for ( ; node != NULL; node = expr.Next(node)) {
        if (node->Type == sdbus::ExprNode::kNodeArith) {
            const sdbus::ArithNode* arithNode = (const sdbus::ArithNode*)node;
            int fid = atoi(arithNode->Name.c_str());
            std::string value = arithNode->Value.c_str();

            if (fid == cond_fid) {
                return value;
            }
        }
    }

    return "";
}

std::string ParseCondition(const sdbus::string& cond, const std::map<int, std::string>& fid_mp)
{
    if (cond.length() == 0) {
        return "";
    }

    sdbus::Expr expr;
    bool flag = expr.Parse(cond);

    if (!flag) {
        return "";
    }

    std::string cond_str;
    std::string temp_str;

    std::stack<std::string> st;

    const sdbus::ExprNode* node = expr.First();
    for ( ; node != NULL; node = expr.Next(node)) {
        if (node->Type == sdbus::ExprNode::kNodeArith) {
            const sdbus::ArithNode* arithNode = (const sdbus::ArithNode*)node;
            int fid = atoi(arithNode->Name.c_str());
            std::string value = arithNode->Value.c_str();

            std::map<int, std::string>::const_iterator it = fid_mp.find(fid);
            if (it != fid_mp.end()) {
                if (fid == FID_BROKER_ID || fid == FID_TRADER_ID || fid == FID_BOND_SHORT_NAME || fid == FID_BOND_CODE || fid == FID_ACROSS_MARKET_BOND_CODES || fid == FID_PIN_YIN || fid == FID_PIN_YIN_FULL || fid == FID_DEAL_NO) {
                    // 处理模糊匹配
					//st.push(std::string("") + it->second + " like '%" + value + "%'");
                    st.push(std::string("") + it->second + " contains '" + value + "'");
                } else if (fid == FID_QUOTE_SIDE || fid == FID_TIME_TO_MATURITY_REAL || fid == FID_MATURITY || fid == FID_FILTER_MATURITY || fid == FID_BID_OFR_SPREAD 
					|| fid == FID_CDC_OFFSET || fid == FID_LISTED_DATE || fid == FID_BOND_IS_EXPIRED || fid == FID_ISSUE_YEAR || fid == FID_DURATION_FILTER || fid == FID_MATURITY_HOLIDAYS) {
					// 处理int bool double
					st.push(std::string("") + it->second + " " + arithNode->Operator.c_str() + value);
				} else {
					// 处理字符串
					st.push(std::string("") + it->second + " " + arithNode->Operator.c_str() + " '" + value + "'");
				}
            } else {
                st.push("");
            }
        } else if (node->Type == sdbus::ExprNode::kNodeDeguacus) {
            if (st.size() >= 1) {
                std::string str = st.top();
                if (str.length() > 0) {
                    st.pop();
                    st.push(std::string("(") + str + ")");
                }
            }
        } else if (node->Type == sdbus::ExprNode::kNodeLogical) {
            const sdbus::LogicalNode* logicalNode = (const sdbus::LogicalNode*)node;
            if (st.size() >= 2){
                std::string str2 = st.top();
                st.pop();
                std::string str1 = st.top();
                st.pop();
                if (logicalNode->Operator == "AND") {
                    if (str1.length() > 0 && str2.length() > 0) {
                        st.push(str1 + " and " + str2);
                    } else if (str1.length() > 0) {
                        st.push(str1);
                    } else if (str2.length() > 0) {
                        st.push(str2);
                    } else {
                        st.push("");
                    }
                } else if (logicalNode->Operator == "OR") {
                    if (str1.length() > 0 && str2.length() > 0) {
                        st.push(str1 + " or " + str2);
                    } else if (str1.length() > 0) {
                        st.push(str1);
                    } else if (str2.length() > 0) {
                        st.push(str2);
                    } else {
                        st.push("");
                    }
                }
            }
        }
    }
    if (st.size() >= 1) {
        temp_str = st.top();
    }

    if (temp_str.length() > 0) {
        if (cond_str.length() > 0) {
            cond_str += " and ";
        }
        cond_str += temp_str;
    }
    return cond_str;
}

std::string ParseSortBy(const sdbus::string& sortby, const std::map<int, std::string>& fid_mp, const std::string& sortby_default)
{
    if (sortby.length() == 0) {
        if (sortby_default.length() > 0) {
            return "order by " + sortby_default;
        } else {
            return "";
        }
    }

    std::string sort_str;
    std::vector<std::string> columns = split(sortby.c_str(), ',');
    int cnt = 0;
    if (columns.size() > 0) {
        for (unsigned int i = 0; i < columns.size(); ++i) {
            std::string column = columns[i];
            Trim(column);
            std::vector<std::string> tokens = split(column, ' ');
            if (tokens.size() == 2) {
                int fid = atoi(tokens[0].c_str());
                std::map<int, std::string>::const_iterator it = fid_mp.find(fid);
                if (it != fid_mp.end()) {
                    if (cnt > 0) {
                        sort_str += ", ";
                    } else {
                        sort_str += "order by ";
                    }
                    sort_str += it->second + " " + tokens[1];
                    cnt++;
                }
            }
        }
    }

    return sort_str;
}

std::string ParseRange(const sdbus::string& range)
{
    if (range.length() == 0) {
        return "";
    }

    std::string range_str = std::string("limit ") + range.c_str();

    return range_str;
}

void ParseRange(const sdbus::string& range, int& start, int& cnt)
{
    std::vector<std::string> vec = split(range.c_str(), ',');

    if (vec.size() >= 2) {
        start = atoi(Trim(vec[0]).c_str());
        cnt = atoi(Trim(vec[1]).c_str());
    } 
}

std::string ParseParam(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range, 
                        const std::map<int, std::string>& fid_mp, const std::string& sortby_default)
{
    std::string cond_str = ParseCondition(cond, fid_mp);
    std::string sort_str = ParseSortBy(sortby, fid_mp, sortby_default);
    std::string range_str = ParseRange(range);    

    std::string str;
    if (cond_str.length() > 0) {
        str += cond_str;
    }
    if (sort_str.length() > 0) {
        if (str.length() > 0) {
            str += " ";
        }
        str += sort_str;
    }
    if (range_str.length() > 0) {
        if (str.length() > 0) {
            str += " ";
        }
        str += range_str;
    }

    if (str.length() > 256) {
        LOGGER_DEBUG("dbQuery: " << str.substr(0, 256))
    } else {
        LOGGER_DEBUG("dbQuery: " << str)
    }

    return str;
}

int ParseRatingStr(const std::string& rating_str)
{
    if (rating_str.length() == 0) {
        return 0;
    }

    int rating = 1;

    const char *p = rating_str.c_str();

    while(*p) {
        char c = *p++;
        switch(c) {
        case 'A':
        case 'B':
        case 'C':
            rating *= 3;
            break;
        case '+':
            rating += 1;
            break;
        case '-':
        case '1':
            rating -= 1;
            break;
        default:
            break;
        }
    }

    switch(rating_str.at(0)) {
    case 'A':
        rating *= 100;
        break;
    case 'B':
        rating *= 10;
        break;
    case 'C':
        rating *= 1;
        break;
    default:
        break;
    }

    return rating;
}

//add by Samuel For Parse rate type
std::string	ParseRateType(std::string &FRN_Index_ID, int Next_Coupon_Date, int MaturityDate)
{
	if ( FRN_Index_ID.empty() || 0 == Next_Coupon_Date || Next_Coupon_Date == MaturityDate)		
	{			
		return "FIXED";	
	}		
	else		
	{		
		std::string rate_type_first;
		std::string	rate_type_second;
		int iPosFirst = FRN_Index_ID.find_first_of("_");
        if (iPosFirst >= 0) {
            rate_type_first = FRN_Index_ID.substr(0, iPosFirst);
            int iPosSecond = FRN_Index_ID.find_first_of("_", iPosFirst + 1);
            if (iPosSecond >= 0) {
                rate_type_second = FRN_Index_ID.substr(iPosFirst + 1, iPosSecond - iPosFirst);
            }
        }
					
		if ( rate_type_first == "B2W" || rate_type_first == "B" )		
		{		
			return "REPO";		
		}
		else if(rate_type_first == "DEPO")		
		{		
			return "DEPO";		
		}
		else if(rate_type_first == "SHIBOR")		
		{		
			return "SHIBOR";		
		}
		else if(rate_type_first == "LIBOR")		
		{		
			return "LIBOR";		
		}
		else if(rate_type_first == "LRB")
		{		
			return "LRB";		
		}
		else if(rate_type_first == "USD" && rate_type_second == "LIBOR")
		{		
			return "USD_LIBOR";		
		}		
	}

	return "";
}

int GetBondSmallTermDays(int maturity_date, int interest_start_date)
{
    int days_now_to_maturity;

    struct tm tm_maturity, tm_interest_start, tm_today;
    time_t maturity, interest_start, today;
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);

    memset(&tm_maturity, 0, sizeof(struct tm));
    memset(&tm_interest_start, 0, sizeof(struct tm));
    memset(&tm_today, 0, sizeof(struct tm));

    tm_maturity.tm_year = maturity_date / 10000 - 1900;
    tm_maturity.tm_mon  = (maturity_date % 10000) / 100 - 1;
    tm_maturity.tm_mday = (maturity_date % 10000) % 100;

    tm_interest_start.tm_year = interest_start_date / 10000 - 1900;
    tm_interest_start.tm_mon  = (interest_start_date % 10000) / 100 - 1;
    tm_interest_start.tm_mday = (interest_start_date % 10000) % 100;

    tm_today.tm_year = local->tm_year;
    tm_today.tm_mon  = local->tm_mon;
    tm_today.tm_mday = local->tm_mday;

    maturity       = mktime(&tm_maturity);
    interest_start = mktime(&tm_interest_start);
    today          = mktime(&tm_today);

    if (difftime(today, interest_start) < 0) {
        days_now_to_maturity = (int)difftime(maturity, interest_start) / (24 * 60 * 60) + 1;
    } else {
        days_now_to_maturity = (int)difftime(maturity, today) / (24 * 60 * 60);
    }
    
    return days_now_to_maturity;
}

int GetDayDifference(int from_date, int to_date)
{
	struct tm tm_from, tm_to;
	time_t date_from, date_to;
	time_t lt = time(NULL);

	memset(&tm_from, 0, sizeof(struct tm));
	memset(&tm_to, 0, sizeof(struct tm));

	tm_from.tm_year = from_date / 10000 - 1900;
	tm_from.tm_mon  = (from_date % 10000) / 100 - 1;
	tm_from.tm_mday = (from_date % 10000) % 100;

	tm_to.tm_year = to_date / 10000 - 1900;
	tm_to.tm_mon  = (to_date % 10000) / 100 - 1;
	tm_to.tm_mday = (to_date % 10000) % 100;

	date_from       = mktime(&tm_from);
	date_to = mktime(&tm_to);

	int days_now_to_maturity = (int)difftime(date_to, date_from) / (24 * 60 * 60) + 1;
    
	return days_now_to_maturity;
}

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

double GetBondSmallTermYears(int maturity_date)
{
    struct tm tm_maturity, tm_today, tm_next;
    time_t maturity, today, next;
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);

    memset(&tm_maturity, 0, sizeof(struct tm));
    memset(&tm_today, 0, sizeof(struct tm));
    memset(&tm_next, 0, sizeof(struct tm));

    tm_maturity.tm_year = maturity_date / 10000 - 1900;
    tm_maturity.tm_mon  = (maturity_date % 10000) / 100 - 1;
    tm_maturity.tm_mday = (maturity_date % 10000) % 100;

    tm_today.tm_year = local->tm_year;
    tm_today.tm_mon  = local->tm_mon;
    tm_today.tm_mday = local->tm_mday;

    // 从到期日依次向前推，直到下一年度
    tm_next.tm_year = (tm_maturity.tm_mon < tm_today.tm_mon || (tm_maturity.tm_mon == tm_today.tm_mon && tm_maturity.tm_mday < tm_today.tm_mday)) ? tm_today.tm_year + 1 : tm_today.tm_year;
    tm_next.tm_mon = tm_maturity.tm_mon;
    tm_next.tm_mday = tm_maturity.tm_mday;

    maturity = mktime(&tm_maturity);
    today    = mktime(&tm_today);
    next     = mktime(&tm_next);

    //if (tm_last.tm_mday == 29 && tm_last.tm_mday == 1 && !isLeapYear(tm_last.tm_year + 1900)) {
    //    tm_last.tm_mday = 28;
    //}
    int n = tm_maturity.tm_year - tm_next.tm_year;
    if (n < 0) n = 0;

    int check_year = tm_next.tm_mon < 2 ? tm_next.tm_year - 1: tm_next.tm_year;   
    if (isLeapYear(check_year + 1900)) {
        return n + difftime(next, today) / (366 * 24 * 60 * 60);
    } else {        
        return n + difftime(next, today) / (365 * 24 * 60 * 60);
    }
}

double n_hundred(int n)
{
    double sum = 1.0;
    for(int i = 1;i <= n;++i)
        sum = sum *10;
    return sum;
}

double Round(double t,int n)
{
    //如果要求保留t小数点后n位数字的话
    int ival = (int)(t * n_hundred(n));
    //看小数点后第n+1位数字是否大于5来进行四舍五入
    int temp = (int)(t * n_hundred(n+1))%10;
    if(temp >= 5)
        ival = ival + 1;
    double dval = ival/n_hundred(n);
    return dval;
}

// 小数点后至少显示min位, 至多max位
std::string Format(double t, int min, int max)
{
    if (t == DOUBLE_NULL) {
        return "";
    }
    int precision = min;
    std::ostringstream ostr;
    if (max != -1) {
        ostr << std::setiosflags(std::ios::fixed) << std::setprecision(max) << t;
        std::string str = ostr.str();
        str = str.substr(0, str.find_last_not_of('0') + 1);
        std::vector<std::string> vec = split(str, '.');

        if (vec.size() >= 2) {
            std::string float_str = vec[1];
            if (float_str.length() > min) {
                precision = float_str.length();
            }
        }
    }

    std::ostringstream oret;
    oret << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << t;
    return oret.str();
}

std::string truncateDouble(double value, const int& percision) {
	int default_precision = 4;
	int int_precision = boost::lexical_cast<int>(percision);
	int delta_precision = default_precision;
	if (int_precision < default_precision) delta_precision = int_precision;

	std::string double_str = Format(value, 0, percision);
	if (double_str.empty() || ("" == double_str)) return double_str;

	std::string ret = double_str;
	if (boost::algorithm::contains(double_str, ".")) {
		int length = double_str.length();
		int valid_len = -1;
		int start = length - 1, end = length - delta_precision;
		for (int i = start; i >= end; --i) {
			if ('0' != double_str.at(i)) {
				valid_len = i + 1;
				break;
			}
		}
		if (-1 == valid_len) valid_len = length - delta_precision;
		ret = double_str.length() > 1 && valid_len < (double_str.length() - 1) ? double_str.substr(0, valid_len) : double_str;
	}
	if (boost::algorithm::ends_with(ret, ".")) {
		ret = ret.substr(0, ret.length() - 1);
	}
	return ret;
}

std::string GetTimeToMaturity(int maturity_date, int interest_start_date, const std::string& option_type_client)
{
    std::vector<std::string> option_array = split(option_type_client.c_str(), '+');
    std::ostringstream ostr;

    if (maturity_date == 0) {
        return "";
    }

    int days_now_to_maturity = GetBondSmallTermDays(maturity_date, interest_start_date);
    double years_to_maturity = GetBondSmallTermYears(maturity_date);
    double year, option_year;

    if (option_array.size() < 2) {
        if (days_now_to_maturity < 365) {
            ostr << days_now_to_maturity << "D";
        } else {
            ostr << Round(years_to_maturity, 2) << "Y";
        }
    } else {
        year = days_now_to_maturity / 365.0;
        option_year = atof(option_array[1].c_str());
        if (year > option_year) {
            year = years_to_maturity - option_year;
            ostr << Round(year, 2) << "Y+" << Round(atof(option_array[1].c_str()), 2) << "Y";
        } else {
            if (days_now_to_maturity < 365) {
                ostr << days_now_to_maturity << "D";
            } else {
                ostr << Round(years_to_maturity, 2) << "Y";
            }
        }
    }

    return ostr.str();
}

double GetTimeToMaturityReal(int maturity_date, int interest_start_date, const std::string& option_type_client)
{
    std::vector<std::string> option_array = split(option_type_client.c_str(), '+');

    int days_now_to_maturity = GetBondSmallTermDays(maturity_date, interest_start_date);
    double years_to_maturity = GetBondSmallTermYears(maturity_date);
    double year, option_year;

    if (option_array.size() < 2) {
        if (days_now_to_maturity < 365) {
            return days_now_to_maturity / 365.0;
        } else {
            return Round(years_to_maturity, 2);
        }
    } else {
        year = days_now_to_maturity / 365.0;
        option_year = atof(option_array[1].c_str());
        if (year > option_year) {
            year = years_to_maturity - option_year;
            return Round(year, 2);
        } else {
            if (days_now_to_maturity < 365) {
                return days_now_to_maturity / 365.0;
            } else {
                return Round(years_to_maturity, 2);
            }
        }
    }
}

double GetTimeToMaturity(int maturity_date, int interest_start_date)
{
    if (maturity_date == 0) {
        return 0;
    }

    int days_now_to_maturity = GetBondSmallTermDays(maturity_date, interest_start_date);

    return days_now_to_maturity / 365.0;
}

int GetMaturityTermInDays(int end_date, std::pair<std::string, std::string>& maturity_term_pair) {
	std::string today = GetTDateString("%Y%m%d");
	int days_to_maturity = GetDayDifference(std::atoi(today.c_str()), end_date);

	std::ostringstream remaining_maturity_term;
	std::ostringstream term_unit;

	if (days_to_maturity > 365) {		   
		char buf[10];
		sprintf(buf, "%2f", days_to_maturity/365.0);
		remaining_maturity_term << std::fixed << std::setprecision(2) << days_to_maturity/365.0;
		term_unit << "Y";
	}
	else {
		remaining_maturity_term << days_to_maturity; 
		term_unit << "D";
	}

	maturity_term_pair.first = remaining_maturity_term.str();
	maturity_term_pair.second = term_unit.str();

	return days_to_maturity;
}

std::pair<std::string, std::string> CalculateMaturityTerm(int maturity_date, int option_date) {

	std::pair<std::string, std::string> maturity_term_pair;
	if (option_date == 0) {
		int days_to_maturity = GetMaturityTermInDays(maturity_date, maturity_term_pair);
	}
	else {
		int days_to_maturity = GetMaturityTermInDays(option_date, maturity_term_pair);
		if (days_to_maturity < 0) {
			GetMaturityTermInDays(maturity_date, maturity_term_pair);
		}
	}

	return maturity_term_pair;
}

std::string CheckOnTheRun(int listed_date)
{
    if (listed_date == 0) {
        return "N";
    }

    struct tm tm_listed, tm_today;
    time_t listed, today;
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);

    memset(&tm_listed, 0, sizeof(struct tm));
    memset(&tm_today, 0, sizeof(struct tm));

    tm_listed.tm_year = listed_date / 10000 - 1900;
    tm_listed.tm_mon  = (listed_date % 10000) / 100 - 1;
    tm_listed.tm_mday = (listed_date % 10000) % 100;

    tm_today.tm_year = local->tm_year;
    tm_today.tm_mon  = local->tm_mon;
    tm_today.tm_mday = local->tm_mday;

    listed = mktime(&tm_listed);
    today  = mktime(&tm_today);

    int diff = boost::numeric_cast<int>(difftime(today, listed));

    if (diff >= 0 && diff < 5 * 24 * 60 * 60) {
        return "Y";
    }

    return "N";
}

time_t ParseTimeString(const std::string &time_str) 
{
    struct tm local;
    memset(&local, 0, sizeof(struct tm));

    if (sscanf(time_str.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &local.tm_year, &local.tm_mon, &local.tm_mday, &local.tm_hour, &local.tm_min, &local.tm_sec) == 6) {
        local.tm_year -= 1900;
        local.tm_mon -= 1;
        return mktime(&local);
    }

    return -1;
}

time_t ParseMaturityDateString(const std::string &date_str, const char* format)
{
	struct tm local;
	memset(&local, 0, sizeof(struct tm));

	if (sscanf(date_str.c_str(), format, &local.tm_year, &local.tm_mon, &local.tm_mday) == 3) {
		local.tm_year -= 1900;
		local.tm_mon -= 1;
		local.tm_hour = 0;
		local.tm_min = 0;
		local.tm_sec = 0;
		return mktime(&local);
	}

	return -1;
}

std::string GetTimeString(time_t t)
{
    if (t == -1) {
        return "";
    }

    struct tm *local = localtime(&t);

    char timeChars[50];              
    strftime(timeChars, 50, "%Y-%m-%d %H:%M:%S", local);

    return std::string(timeChars);
}

time_t GetCurrentTimeMilliSec() 
{
    boost::int64_t time64 = (boost::posix_time::microsec_clock::universal_time() - epoch).total_milliseconds();

    return time64;
}

time_t GetCurrentTimeMicroSec() 
{
    boost::int64_t time64 = (boost::posix_time::microsec_clock::universal_time() - epoch).total_microseconds();

    return time64;
}

std::string GetCurrentTimeString() {
    char   timeChars[50];              
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);

    strftime(timeChars, 50, "%Y-%m-%d %H:%M:%S", local);

    return std::string(timeChars);
}

std::string GetCurrentMinuteString() {
    char   timeChars[50];              
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);

    strftime(timeChars, 50, "%H:%M", local);

    return std::string(timeChars);
}

std::string GetCurrentHMSString() {
	char   timeChars[50];
	time_t lt = time(NULL);
	struct tm *local = localtime(&lt);

	strftime(timeChars, 50, "%H:%M:%S", local);

	return std::string(timeChars);
}

std::string GetTDateString(const char* format) {
    /*char   dayChars[50];              
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);

    strftime(dayChars, 50, format, local);

    return std::string(dayChars);*/
	std::locale _fmt(std::locale::classic(), new boost::gregorian::date_facet(format));
	std::ostringstream os;
	os.imbue(_fmt);
	os << currentDate();
	return os.str();
}

std::string GetTDateString(time_t t, const char* format) {
	char   dayChars[50];
	struct tm *local = localtime(&t);

	strftime(dayChars, 50, format, local);

	return std::string(dayChars);
}

std::string GetT1DateString(const char* format) {
    /*char   dayChars[50];              
    time_t lt = time(NULL);
    struct tm *local = localtime(&lt);
    local->tm_mday = local->tm_mday + 1;

    strftime(dayChars, 50, format, local);

    return std::string(dayChars);*/
	std::locale _fmt(std::locale::classic(), new boost::gregorian::date_facet(format));
	boost::gregorian::date sD = currentDate();
	boost::gregorian::days d(1);
	boost::gregorian::date eD = sD + d;
	std::ostringstream os;
	os.imbue(_fmt);
	os << eD;
	return os.str();
}

std::string GetBeforeDateString(const char* format, int months) {
	char dayChars[50];
	time_t lt = time(NULL);
	struct tm *local = localtime(&lt);
	int delta_year = months / 12;
	int delta_mon = local->tm_mon - months % 12;
	if (delta_mon >= 0)
	{
		local->tm_mon = delta_mon;
	}
	else
	{
		local->tm_mon = 12 - abs(delta_mon);
		delta_year += 1;
	}

	local->tm_year = local->tm_year - delta_year;

	strftime(dayChars, 50, format, local);

	return std::string(dayChars);
}

int GetSortIndexByBondType(const std::string &bondtype)
{
    if (bondtype == GBKToUTF8("国债"))
        return 1;
    else if(bondtype == GBKToUTF8("央票"))
        return 2;
    else if(bondtype == GBKToUTF8("金融债"))
        return 3;
    else if(bondtype == GBKToUTF8("金融债(含权)"))
        return 4;
    else if(bondtype == GBKToUTF8("金融债(Shibor)"))
        return 5;
    else if(bondtype == GBKToUTF8("金融债(Shibor含权)"))
        return 6;
    else if(bondtype == GBKToUTF8("金融债(Depo)"))
        return 7;
    else if(bondtype == GBKToUTF8("金融债(Depo含权)"))
        return 8;
    else if(bondtype == GBKToUTF8("地方债"))
        return 9;
    else if(bondtype == GBKToUTF8("其他"))
        return 10;
    else
        return 10;
}

std::string GBKToUTF8(const std::string& strGBK) {
    return boost::locale::conv::between(strGBK, "UTF-8", "GBK");
}

std::string Utf8ToAnsi(const sdbus::string &strUTF8) {
    return boost::locale::conv::between(strUTF8, "GBK", "UTF-8");
}

std::string GetCompletedBondType(const std::string &bondtype, const std::string &frnindexid, const std::string &optiontype)
{
    if (bondtype != GBKToUTF8("金融债"))
        return bondtype;

    if (-1 != frnindexid.find("DEPO")){
        if (!hasOptionType(optiontype))
            return GBKToUTF8("金融债(Depo)");
        else
            return GBKToUTF8("金融债(Depo含权)");
    }else if (-1 != frnindexid.find("SHIBOR")){
        if (!hasOptionType(optiontype))
            return GBKToUTF8("金融债(Shibor)");
        else
            return GBKToUTF8("金融债(Shibor含权)");
    }else {
        if (!hasOptionType(optiontype))
            return GBKToUTF8("金融债");
        else
            return GBKToUTF8("金融债(含权)");
    }
    return "";
}

bool hasOptionType(const std::string &optiontype)
{
    if (optiontype.empty() || optiontype == "NON")
        return false;
    else
        return true;
}

int CountChineseWords(const std::string& str)
{   
    int count = 0;
    std::string::const_iterator iter = str.begin();

    for (; iter < str.end(); ++iter)
    {
        char ch = *iter;
        bool isAlphabet = ((ch >= 'a' && ch <= 'z') ||  (ch >= 'A' && ch <= 'Z')) ? true : false;
        bool isNumber   = (ch >= '0' && ch <= '9') ? true : false;

        if ( !isAlphabet && !isNumber )
        {
            count++;
        }
    }

    return count / 3;// 中文占用3个字符
}

void GetPriceStrings(double price, const std::string& fan_dian_flag, double fan_dian, int symbol, int price_type, std::string& price_string, std::string& fan_dian_string) 
{
    if (price != INVALIDVALUE) // 价格不为空
    {
        price_string = Format(price, 2, 4);

        if (fan_dian_flag == "1") // 有返点
        {
            if (fan_dian == INVALIDVALUE)
            {
                fan_dian_string = fan_dian_string + "F--";
            } 
            else
            {
                fan_dian_string = fan_dian_string + "F" + Format(fan_dian, 2, 4);
            }
        }
    }

    else // 价格为空
    {
        if (fan_dian_flag == "1") // 有返点
        {
            if (fan_dian == INVALIDVALUE)
            {
                price_string = price_string + GBKToUTF8("平价返");
            } 
            else
            {
                price_string = price_string + "--";
                fan_dian_string = fan_dian_string + "F" +  Format(fan_dian, 2, 4);
            }
        }
        else
        {
            if (price_type == 0)
            {
                price_string =  price_string + (symbol == 1 ? "Bid" : "Ofr");
            }
            else
            {
                price_string = "--";
            }
        }
    }
}

// For the best quote
void GetBestPriceStrings(double price, const std::string &fan_dian_flag, double fan_dian, const std::string &quote_id, int quote_side, std::string &price_string, std::string &fan_dian_string) 
{
    if ( price != INVALIDVALUE) // 价格不为空
    {
        price_string = Format(price, 2, 4);

        if (fan_dian_flag == "1") // 有返点
        {
            if (fan_dian == INVALIDVALUE)
            {
                fan_dian_string = "F--";
            } 
            else
            {
                fan_dian_string = "F" +  Format(fan_dian, 2, 4);
            }
        }
    }
    else // 价格为空
    {
        if (fan_dian_flag == "1") // 有返点
        {
            if (fan_dian == INVALIDVALUE)
            {
                price_string = GBKToUTF8("平价返");
            } 
            else
            {
                price_string = "--";
                fan_dian_string = "F" +  Format(fan_dian, 2, 4);
            }
        }
        else
        {
            //quote_side 1 bid; -1 ofr
            if (quote_id.length() > 0)
            {
                price_string = quote_side == 1 ? "Bid" : "Ofr";
            }
        }
    }
}

std::string GetDcsDealStatusString(const std::string& deal_status, bool in_hand)
{
	std::string status = kDcsDealtoBeConfirmString;

	if (deal_status == kDcsDealtoBeConfirm)
    {
		status = kDcsDealtoBeConfirmString;
    }
	else if (deal_status == kDcsDealtoBeSubmit)
    {
		status = kDcsDealtoBeSubmitString;
    }
	else if (deal_status == kDcsDealSubmitted)
    {
		status = kDcsDealSubmittedString;
    }
	else if (deal_status == kDcsDealPassed)
    {
		status = kDcsDealPassedString;
    }
	else if (deal_status == kDcsDealNoPass)
    {
		status = kDcsDealNoPassString;
    }
	else if (deal_status == kDcsDealDestroyed)
    {
		status = kDcsDealDestroyedString;
    }
	else if (deal_status == kDcsDealIntheReview)
	{
		status = kDcsDealIntheReviewString;
	}
	else if (deal_status == kDcsDealDeleted)
	{
		status = kDcsDealDeletedString;
	}

	if (in_hand)
	{
		status = kDcsDealInHandString;
	}

	return status;
}

std::string IntToString(int value)
{
    std::stringstream ssValue;
    ssValue << value;

    return ssValue.str();
}

// TODO: use template
std::string FloatToString(float value)
{
    std::stringstream ssValue;
    ssValue << value;

    return ssValue.str();
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string join(std::vector<std::string>& str_arr, std::string delim) {
    if (str_arr.size() == 0)
        return "";

    std::ostringstream ss;
    ss << str_arr[0];
    for (size_t i = 1; i < str_arr.size(); ++i)
        ss << delim << str_arr[i];

    return ss.str();
}

std::string joinSql(std::vector<std::string>& str_arr) {
	if (str_arr.empty())
		return "";
	std::ostringstream ss;
	std::string delim = "'";
	ss << delim << str_arr[0] << delim;
	for (size_t i = 1; i < str_arr.size(); ++i)
		ss << "," << delim << str_arr[i] << delim;

	return ss.str();
}

char* allocString(std::string value)
{
    char* str = new char[value.length()+1];
    strcpy(str, value.c_str());
    return str;
}

void stringSetter(const char** property, std::string value)
{
    clearStringProperty(property);
    *property = const_cast<const char*>(allocString(value));
}

char const* stringGetter(char const* property)
{
    return property == NULL ? "" : property;
}

void clearStringProperty(const char** property)
{
    if (*property != NULL)
    {
        delete *property;
    }
}

// 0-星期天 1-6-星期一~星期六 8-无效值
size_t getWeekDay(size_t year, size_t month, size_t day)
{
    if ((year < 1) || (month < 1) || (day < 1))  
    {  
        return 8;  
    }  
    if ((month > 12) || (day > 31))  
    {  
        return 8;  
    }  

    if (month < 3)  
    {  
        month += 12;  
        year--;
    }

    return (day + 1 + 2*month + 3*(month+1)/5 + year + year/4 - year/100 + year/400) % 7;  
}

std::string GetIpAddressFromString(const std::string &msg_string)
{
	std::vector<std::string> ipaddress;
	boost::regex ip_pattern("(\\d{1, 3}.){3}\\d{1, 3}$");  
	find_all_regex(ipaddress, msg_string, ip_pattern); 
	if (ipaddress.size()==0){
		LOGGER_ERROR("BondLogin: Ip Address Error: " << msg_string)
		return "";
	}
	return ipaddress[0];
}

// zip sdbus msg
bool zipMessage(const sdbus::Message &msg, sdbus::Message &zip_msg)
{
    bool ret = false;
    size_t len;
    if (!VariantMapCalcSize(msg.MapValue(), len)) {
        return false;
    }

    char* buffer = new char[len];
    if (buffer == NULL) {
        return false;
    }
    memset(buffer, 0, sizeof(char) * len);

    // serialize
	if (!VariantMapEncode(msg.MapValue(), buffer, sizeof(char) * len, len)) {
        delete [] buffer;
        return false;
    }

    int zip_len = len > 8096 ? len : 8096;
    char* zip_buffer = new char[zip_len];
    if (zip_buffer == NULL) {
        delete [] buffer;
        return false;
    }

    if (!sdbus::zip(buffer, len, zip_buffer, zip_len)) {       
        delete [] buffer;
        delete [] zip_buffer;
        return false;
    }
      
    delete [] buffer;

    zip_msg.SetRaw(FID_ZIP_DATA, zip_buffer, zip_len);
    zip_msg.SetInt32(FID_UNZIP_LEN, len);
    zip_msg.SetInt32(FID_ZIP_LEN, zip_len);
    
    delete [] zip_buffer;
    return true;
}

bool unzipMessage(const sdbus::Message &zip_msg, sdbus::Message &msg)
{
    bool ret = false;

    int len = 0;
    zip_msg.GetInt32(FID_UNZIP_LEN, len);

    if (len <= 0) {
        return false;
    }

    size_t zip_len = 0;
    const void * zip_buffer;
    zip_msg.RefRaw(FID_ZIP_DATA, zip_buffer, zip_len);

    if (zip_len == 0) {
        return false;
    }

    char* buffer = new char[len];
    if (buffer == NULL) {
        return false;
    }
	memset(buffer, 0, sizeof(char) * len);
    if (!sdbus::unzip((const char*)zip_buffer, zip_len, buffer, len)) {
        delete [] buffer;
        return false;
    }

    sdbus::VariantMap &mp = msg.MapValue();
    size_t used = 0;
    if (VariantMapDecode(mp, buffer, sizeof(buffer), used)) {
        msg.SetType(zip_msg.GetType());
        msg.SetSubject(zip_msg.GetSubject());
        msg.SetMessageID(zip_msg.GetMessageID());     
        ret = true;
    }

    delete [] buffer;

    return ret;

}

std::string getIntToStringWithBracket(int i)
{
    return "[" + IntToString(i) + "]";
}

static const std::string _BCO_CATEGORY_TYPES[] = { "BCO", "ABS", "NCD" };
bool isBCOCategoryType(const std::string& bond_category) {
    for (size_t i = 0; i < _BCO_CATEGORY_TYPES->length(); i++){
        if (bond_category == _BCO_CATEGORY_TYPES[i]) {
            return true;
        }
    }
    return false;
}

std::string dateShortToLong(const std::string& shortDate) {
    if (shortDate.empty() || shortDate.length() < 8)
        return shortDate;
    std::string year = shortDate.substr(0, 4);
    std::string month = shortDate.substr(4, 2);
    std::string day = shortDate.substr(6);
    std::stringstream ss;
    ss << year << "-" << month << "-" << day;
    return ss.str();
}
const std::locale fmt(std::locale::classic(), new boost::gregorian::date_facet(_DATE_FORMAT.c_str()));
const std::locale fmt_input(std::locale::classic(), new boost::gregorian::date_facet(_DATE_FORMAT.c_str()));
std::string dateToStr(const boost::gregorian::date& date) {
    if (date.is_not_a_date()) {
        return "";
    }
    //boost::gregorian::date_facet* df(new boost::gregorian::date_facet("%Y-%m-%d"));
    std::ostringstream is;
    //is.imbue(std::locale(is.getloc(), df));
    is.imbue(fmt);
    is << date;
    return is.str();
}
boost::gregorian::date strToDate(const std::string& dateStr) {
    if (dateStr.empty()) {
        return currentDate();
    }
    //std::stringstream ss(dateStr);
    //ss.imbue(fmt_input);
    boost::gregorian::date d = boost::gregorian::from_string(dateStr);
    //ss >> d;
    return d;
}

boost::gregorian::date currentDate() {
    boost::gregorian::date current_date(boost::gregorian::day_clock::local_day());
    return current_date;
}

boost::gregorian::date_duration dateDuration(const boost::gregorian::date& start, const boost::gregorian::date& end) {
    boost::gregorian::date_period dd(start, end);
    return dd.length();
}

boost::gregorian::date getNextWorkDate(std::vector<std::string> holiday_date_vecs, const boost::gregorian::date& date) {
    if (holiday_date_vecs.empty() || date.is_not_a_date())
        return date;
    std::string str = dateToStr(date);
    bool contains = (std::find(holiday_date_vecs.begin(), holiday_date_vecs.end(), str) != holiday_date_vecs.end());
    if (contains) {
        boost::gregorian::days d(1);
        return getNextWorkDate(holiday_date_vecs, date + d);
    } else {
        return date;
    }
}

bool lessThan1Year(const std::string& endDate) {
    using namespace boost::gregorian;
    date d = strToDate(endDate);
    if (d.is_not_a_date())
        return false;
    years y(1);
    date tD = currentDate();
    return d >= tD && d <= tD + y;
}

#include "cache/cache_controller_declear.h"
#include "fastdb/inc/query.h"
static std::vector<std::string> _HOLIDAY_CACHES;
void initHolidayCache() {
    if (!_HOLIDAY_CACHES.empty())
        _HOLIDAY_CACHES.clear();
    HolidayInfoCacheController hicc;
    using namespace boost::gregorian;
    date sD = currentDate();
    years y(2);
    date eD = sD + y;
    std::string start = dateToStr(sD);
    std::string end = dateToStr(eD);
    std::string sql = "holiday_date >= '" + start + "' and holiday_date <= '" + end + "'";
    dbQuery q(sql.c_str());
    HolidayInfoCacheVecPtr vecs = hicc.getCacheListByQueryInThreadSafty(q);
    if (!vecs->empty()) {
        for (std::vector<HolidayInfoCachePtr>::iterator it = vecs->begin(); it != vecs->end(); ++it) {
            if (*it) {
                _HOLIDAY_CACHES.push_back((*it)->holiday_date);
            }
        }
    }
    LOGGER_DEBUG("holiday cache size:" << _HOLIDAY_CACHES.size() << ", cache sql:" << sql);
}

const std::vector<std::string>& getHolidayCache() {
    return _HOLIDAY_CACHES;
}

void clearHolidayCache() {
    _HOLIDAY_CACHES.clear();
}

bool strContains(const std::string& source, const std::string& str) {
    if (source.empty()) {
        return false;
    }
    if (str.empty()) {
        return false;
    }
    if (source.find(str) != std::string::npos) {
        return true;
    }
    return false;
}

std::string getDBAccountSchema() {
	return Config::singleton()->getValue("Connection.DBAccount", "idb_account");
}

std::string getDBBondSchema() {
	return Config::singleton()->getValue("Connection.DataBase", "idb_bond");
}

std::string GetFastdbQuery(const std::string &key, const std::string &val) {
	return key + " = '" + val + "'";
}

std::string GetFastdbQueryByBondKeyListedMarket(const std::string &val, const std::string &companyId) {
	std::string ret = GetFastdbQuery(std::string("bond_key_listed_market"), val);
	if (!companyId.empty())
		ret += " and company_id = '" + companyId + "'";
	return ret;
}

std::string GetFastdbQueryById(const std::string &val, const std::string &companyId) {
	std::string ret = GetFastdbQuery(std::string("id"), val);
	if (!companyId.empty())
		ret += " and company_id = '" + companyId + "'";
	return ret;
}

void trimSql(std::string& sql) {
	if (!sql.empty()) {
		boost::replace_all(sql, "\r\n", "");
		boost::replace_all(sql, "\n", "");
	}
}