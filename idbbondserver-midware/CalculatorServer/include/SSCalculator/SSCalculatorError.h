#ifndef SS_CALCULATOR_ERROR_H
#define SS_CALCULATOR_ERROR_H

#include <stdio.h>
#include <map>
#include <string>

using std::map;
using std::string;

namespace ssCalculator{

#define INVALID_VALUE -999
#define CALCULATOR_ERROR_BASE 1000

#define DATE_FORMAT_ERROR                                 CALCULATOR_ERROR_BASE+1
#define DATE_ERROR                                        CALCULATOR_ERROR_BASE+2
#define TIME_UNIT_ERROR                                   CALCULATOR_ERROR_BASE+3
#define COUPON_TYPE_ERROR                                 CALCULATOR_ERROR_BASE+4
#define FREQUENCY_ERROR                                   CALCULATOR_ERROR_BASE+5
#define INTEREST_BASIS_ERROR                              CALCULATOR_ERROR_BASE+6
#define COMPOUNDING_TYPE_ERROR                            CALCULATOR_ERROR_BASE+7
#define DMC_ERROR                                         CALCULATOR_ERROR_BASE+8
#define CALENDAR_MARKET_ERROR                             CALCULATOR_ERROR_BASE+9
#define FIXING_PRECEDS_ERROR                              CALCULATOR_ERROR_BASE+10
#define OPTION_TYPE_ERROR                                 CALCULATOR_ERROR_BASE+11
#define OPTION_STYLE_ERROR                                CALCULATOR_ERROR_BASE+12
#define BOND_INFO_ERROR                                   CALCULATOR_ERROR_BASE+13
#define NO_BOND_INFO_ERROR                                CALCULATOR_ERROR_BASE+14
#define CALENDAR_INFO_ERROR                               CALCULATOR_ERROR_BASE+15
#define NULL_DATE_IS_BUSINESSDAY_ERROR                    CALCULATOR_ERROR_BASE+16
#define NO_CALENDAR_INFO_ERROR                            CALCULATOR_ERROR_BASE+17
#define NULL_RATE_ERROR                                   CALCULATOR_ERROR_BASE+18
#define RATE_INFO_ERROR                                   CALCULATOR_ERROR_BASE+19
#define NULL_DATE_GET_RATE_ERROR                          CALCULATOR_ERROR_BASE+20
#define NO_RATE_INFO_ERROR                                CALCULATOR_ERROR_BASE+21
#define NO_RATE_INFO_OR_BEFORE_ERROR                      CALCULATOR_ERROR_BASE+22
#define NULL_SETTLEMENT_DATE_BOND_ERROR                   CALCULATOR_ERROR_BASE+23
#define NULL_INTEREST_START_DATE_ERROR                    CALCULATOR_ERROR_BASE+24
#define NULL_MATURITY_DATE_ERROR                          CALCULATOR_ERROR_BASE+25
#define SETTLEMENT_DATE_OUTBOUND_ERROR                    CALCULATOR_ERROR_BASE+26
#define FRB_NO_FREQUENCY_ERROR                            CALCULATOR_ERROR_BASE+27
#define FRB_NO_COUPON_RATE_ERROR                          CALCULATOR_ERROR_BASE+28
#define PAM_NO_COUPON_RATE_ERROR                          CALCULATOR_ERROR_BASE+29
#define DSC_NO_ISSUE_PRICE_ERROR                          CALCULATOR_ERROR_BASE+30
#define FRN_NO_FREQUENCY_ERROR                            CALCULATOR_ERROR_BASE+31
#define FRN_NO_COUPON_SPREAD_ERROR                        CALCULATOR_ERROR_BASE+32
#define FRN_CONVENTION_ERROR                              CALCULATOR_ERROR_BASE+33
#define COUPON_TYPE_BOND_ERROR                            CALCULATOR_ERROR_BASE+34
#define EMPTY_CASHFLOW_ERROR                              CALCULATOR_ERROR_BASE+35
#define EMPTY_OPTION_CASHFLOW_ERROR                       CALCULATOR_ERROR_BASE+36
#define SETTLEMENT_DATE_OUTBOUND_EXECUTION_ERROR          CALCULATOR_ERROR_BASE+37
#define OPTION_STYLE_SETUP_CALL_LEG_ERROR                 CALCULATOR_ERROR_BASE+38
#define OPTION_TYPE_SETUP_CALL_LEG_ERROR                  CALCULATOR_ERROR_BASE+39
#define NULL_CALL_DATE_ERROR                              CALCULATOR_ERROR_BASE+40
#define NULL_CALL_STRIKE_ERROR                            CALCULATOR_ERROR_BASE+41
#define OPTION_STYLE_SETUP_PUT_LEG_ERROR                  CALCULATOR_ERROR_BASE+42
#define OPTION_TYPE_SETUP_PUT_LEG_ERROR                   CALCULATOR_ERROR_BASE+43
#define NULL_PUT_DATE_ERROR                               CALCULATOR_ERROR_BASE+44
#define NULL_ASS_DATE_ERROR                               CALCULATOR_ERROR_BASE+45
#define NULL_PUT_STRIKE_ERROR                             CALCULATOR_ERROR_BASE+46
#define DSC_CASHFLOW_ERROR                                CALCULATOR_ERROR_BASE+47
#define DSC_NULL_CALL_DATE_ERROR                          CALCULATOR_ERROR_BASE+48
#define DSC_SETTLEMENT_DATE_OUTBOUND_CALL_ERROR           CALCULATOR_ERROR_BASE+49
#define DSC_NULL_PUT_DATE_ERROR                           CALCULATOR_ERROR_BASE+50
#define DSC_SETTLEMENT_DATE_OUTBOUND_PUT_ERROR            CALCULATOR_ERROR_BASE+51
#define FRN_NULL_HISTORICAL_RATE_ERROR                    CALCULATOR_ERROR_BASE+52
#define FRN_PBC_FORECAST_ERROR                            CALCULATOR_ERROR_BASE+53
#define FRN_CFETS_FORECAST_ERROR                          CALCULATOR_ERROR_BASE+54
#define FRN_INCOMPLETE_RATE_ERROR                         CALCULATOR_ERROR_BASE+55
#define FRN_NOT_PAYDAY_ERROR                              CALCULATOR_ERROR_BASE+56
#define NOT_FRN_ERROR                                     CALCULATOR_ERROR_BASE+57
#define YTC_UNAVAILABLE_ERROR                             CALCULATOR_ERROR_BASE+58
#define YTP_UNAVAILABLE_ERROR                             CALCULATOR_ERROR_BASE+59
#define INVALID_START_END_DATES_CASHFLOW_ERROR            CALCULATOR_ERROR_BASE+60
#define DATE_ORDER_ERROR                                  CALCULATOR_ERROR_BASE+61
#define NULL_REBATE_ERROR                                 CALCULATOR_ERROR_BASE+62
#define CAL_DIVERGENT_ERROR                               CALCULATOR_ERROR_BASE+63


    const map<int, string>::value_type errorInitEN[] = {
        map<int, string>::value_type( DATE_FORMAT_ERROR                         , "Invalid date format"),
        map<int, string>::value_type( DATE_ERROR                                , "Invalid date"),
        map<int, string>::value_type( TIME_UNIT_ERROR                           , "Invalid time unit"),
        map<int, string>::value_type( COUPON_TYPE_ERROR                         , "Invalid coupon type"),
        map<int, string>::value_type( FREQUENCY_ERROR                           , "Invalid frequency"),
        map<int, string>::value_type( INTEREST_BASIS_ERROR                      , "Invalid interest basis"),
        map<int, string>::value_type( COMPOUNDING_TYPE_ERROR                    , "Invalid compounding type"),
        map<int, string>::value_type( DMC_ERROR                                 , "Invalid Date moving convention"),
        map<int, string>::value_type( CALENDAR_MARKET_ERROR                     , "Invalid calendar market"),
        map<int, string>::value_type( FIXING_PRECEDS_ERROR                      , "Invalid fixing days"),
        map<int, string>::value_type( OPTION_TYPE_ERROR                         , "Invalid option type"),
        map<int, string>::value_type( OPTION_STYLE_ERROR                        , "Invalid option style"),
        map<int, string>::value_type( BOND_INFO_ERROR                           , "Invalid bond info input"),
        map<int, string>::value_type( NO_BOND_INFO_ERROR                        , "Bond info not found"),
        map<int, string>::value_type( CALENDAR_INFO_ERROR                       , "Invalid calendar info input"),
        map<int, string>::value_type( NULL_DATE_IS_BUSINESSDAY_ERROR            , "Try to determine whether null date is business day"),
        map<int, string>::value_type( NO_CALENDAR_INFO_ERROR                    , "Calendar info not found"),
        map<int, string>::value_type( NULL_RATE_ERROR                           , "Null interest rate input"),
        map<int, string>::value_type( RATE_INFO_ERROR                           , "Invalid interest rate info"),
        map<int, string>::value_type( NULL_DATE_GET_RATE_ERROR                  , "Null date when trying to get interest rate info"),
        map<int, string>::value_type( NO_RATE_INFO_ERROR                        , "Interest rate info not found"),
        map<int, string>::value_type( NO_RATE_INFO_OR_BEFORE_ERROR              , "Interest rate info not found, at or prior to this date"),
        map<int, string>::value_type( NULL_SETTLEMENT_DATE_BOND_ERROR           , "Null settlement date"),
        map<int, string>::value_type( NULL_INTEREST_START_DATE_ERROR            , "Null interest start date"),
        map<int, string>::value_type( NULL_MATURITY_DATE_ERROR                  , "Null maturity date"),
        map<int, string>::value_type( SETTLEMENT_DATE_OUTBOUND_ERROR            , "Settlement date out of bound, please choose settlement date between "),
        map<int, string>::value_type( FRB_NO_FREQUENCY_ERROR                    , "Lack frequency info"),
        map<int, string>::value_type( FRB_NO_COUPON_RATE_ERROR                  , "Lack coupon rate info"),
        map<int, string>::value_type( PAM_NO_COUPON_RATE_ERROR                  , "Lack coupon rate info"),
        map<int, string>::value_type( DSC_NO_ISSUE_PRICE_ERROR                  , "Lack issue price info"),
        map<int, string>::value_type( FRN_NO_FREQUENCY_ERROR                    , "Lack frequency info"),
        map<int, string>::value_type( FRN_NO_COUPON_SPREAD_ERROR                , "Lack coupon rate/spread info"),
        map<int, string>::value_type( FRN_CONVENTION_ERROR                      , "Invalid FRN convention"),
        map<int, string>::value_type( COUPON_TYPE_BOND_ERROR                    , "Invalid coupon type"),
        map<int, string>::value_type( EMPTY_CASHFLOW_ERROR                      , "No cashflow"),
        map<int, string>::value_type( EMPTY_OPTION_CASHFLOW_ERROR               , "No cashflow when execute option"),
        map<int, string>::value_type( SETTLEMENT_DATE_OUTBOUND_EXECUTION_ERROR  , "Settlement date later than option execution date, please choose settlement date between "),
        map<int, string>::value_type( OPTION_STYLE_SETUP_CALL_LEG_ERROR         , "Invalid option style"),
        map<int, string>::value_type( OPTION_TYPE_SETUP_CALL_LEG_ERROR          , "Invalid option type"),
        map<int, string>::value_type( NULL_CALL_DATE_ERROR                      , "Null call date"),
        map<int, string>::value_type( NULL_CALL_STRIKE_ERROR                    , "Null strike price"),
        map<int, string>::value_type( OPTION_STYLE_SETUP_PUT_LEG_ERROR          , "Invalid option style"),
        map<int, string>::value_type( OPTION_TYPE_SETUP_PUT_LEG_ERROR           , "Invalid option type"),
        map<int, string>::value_type( NULL_PUT_DATE_ERROR                       , "Null put date"),
        map<int, string>::value_type( NULL_ASS_DATE_ERROR                       , "Null put date"),
        map<int, string>::value_type( NULL_PUT_STRIKE_ERROR                     , "Null strike price"),
        map<int, string>::value_type( DSC_CASHFLOW_ERROR                        , "Unexpected DSC cashflow"),
        map<int, string>::value_type( DSC_NULL_CALL_DATE_ERROR                  , "Null call date"),
        map<int, string>::value_type( DSC_SETTLEMENT_DATE_OUTBOUND_CALL_ERROR   , "Settlement date later than option execution date, please choose settlement date between "),
        map<int, string>::value_type( DSC_NULL_PUT_DATE_ERROR                   , "Null put date"),
        map<int, string>::value_type( DSC_SETTLEMENT_DATE_OUTBOUND_PUT_ERROR    , "Settlement date later than option execution date, please choose settlement date between "),
        map<int, string>::value_type( FRN_NULL_HISTORICAL_RATE_ERROR            , "Lack historical interest rate info"),
        map<int, string>::value_type( FRN_PBC_FORECAST_ERROR                    , "No default forecast rate can be found, you can make your own forecast by specifying in 'index rate' field"),
        map<int, string>::value_type( FRN_CFETS_FORECAST_ERROR                  , "No default forecast rate can be found, you can make your own forecast by specifying in 'index rate' field"),
        map<int, string>::value_type( FRN_INCOMPLETE_RATE_ERROR                 , "Incomplete interest rate info"),
        map<int, string>::value_type( FRN_NOT_PAYDAY_ERROR                      , "Not coupon pay date"),
        map<int, string>::value_type( NOT_FRN_ERROR                             , "Not FRN"),
        map<int, string>::value_type( YTC_UNAVAILABLE_ERROR                     , "YTC is not available"),
        map<int, string>::value_type( YTP_UNAVAILABLE_ERROR                     , "YTP is not available"),
        map<int, string>::value_type( INVALID_START_END_DATES_CASHFLOW_ERROR    , "Null interest start date or maturity date"),
        map<int, string>::value_type( DATE_ORDER_ERROR                          , "Start date is later than end date"),
        map<int, string>::value_type( NULL_REBATE_ERROR                         , "Null rebate"),
        map<int, string>::value_type( CAL_DIVERGENT_ERROR                       , "calculation divergent")
    };

    const map<int, string>::value_type errorInitCN[] = {
        map<int, string>::value_type( DATE_FORMAT_ERROR                         , "内部日期格式错误，请使用YYYYMMDD作日期输入格式"),
        map<int, string>::value_type( DATE_ERROR                                , "无效的日期"),
        map<int, string>::value_type( TIME_UNIT_ERROR                           , "无法识别的时间单位"),
        map<int, string>::value_type( COUPON_TYPE_ERROR                         , "无法识别的债券类型"),
        map<int, string>::value_type( FREQUENCY_ERROR                           , "无法识别的频率"),
        map<int, string>::value_type( INTEREST_BASIS_ERROR                      , "无法识别的计息基准"),
        map<int, string>::value_type( COMPOUNDING_TYPE_ERROR                    , "无法识别的单利/复利方式"),
        map<int, string>::value_type( DMC_ERROR                                 , "无法识别的节假日调整方式"),
        map<int, string>::value_type( CALENDAR_MARKET_ERROR                     , "无法识别的日历"),
        map<int, string>::value_type( FIXING_PRECEDS_ERROR                      , "无法识别的 T+x 日数"),
        map<int, string>::value_type( OPTION_TYPE_ERROR                         , "无法识别的选择权类别"),
        map<int, string>::value_type( OPTION_STYLE_ERROR                        , "无法识别的选择权类型"),
        map<int, string>::value_type( BOND_INFO_ERROR                           , "输入的债券信息含错"),
        map<int, string>::value_type( NO_BOND_INFO_ERROR                        , "无法找到债券信息"),
        map<int, string>::value_type( CALENDAR_INFO_ERROR                       , "输入的日历信息含错"),
        map<int, string>::value_type( NULL_DATE_IS_BUSINESSDAY_ERROR            , "判断日期是否是工作日时，日期为空"),
        map<int, string>::value_type( NO_CALENDAR_INFO_ERROR                    , "无法找到日历信息"),
        map<int, string>::value_type( NULL_RATE_ERROR                           , "输入的利率值为空"),
        map<int, string>::value_type( RATE_INFO_ERROR                           , "输入的利率信息含错"),
        map<int, string>::value_type( NULL_DATE_GET_RATE_ERROR                  , "尝试获取某日期的利率信息时，日期为空"),
        map<int, string>::value_type( NO_RATE_INFO_ERROR                        , "无法找到指定日期的利率信息"),
        map<int, string>::value_type( NO_RATE_INFO_OR_BEFORE_ERROR              , "无法找到指定日期或之前的利率信息"),
        map<int, string>::value_type( NULL_SETTLEMENT_DATE_BOND_ERROR           , "结算日为空"),
        map<int, string>::value_type( NULL_INTEREST_START_DATE_ERROR            , "起息日为空"),
        map<int, string>::value_type( NULL_MATURITY_DATE_ERROR                  , "到期日为空"),
        map<int, string>::value_type( SETTLEMENT_DATE_OUTBOUND_ERROR            , "结算日超出起息日或到期日，请将结算日选在如下范围内："),
        map<int, string>::value_type( FRB_NO_FREQUENCY_ERROR                    , "该券缺少付息频率信息，无法进行计算"),
        map<int, string>::value_type( FRB_NO_COUPON_RATE_ERROR                  , "该券缺少票面利率信息，无法进行计算"),
        map<int, string>::value_type( PAM_NO_COUPON_RATE_ERROR                  , "该券缺少票面利率信息，无法进行计算"),
        map<int, string>::value_type( DSC_NO_ISSUE_PRICE_ERROR                  , "该券缺少发行价格信息，无法计算应计利息及其他"),
        map<int, string>::value_type( FRN_NO_FREQUENCY_ERROR                    , "该券缺少付息频率信息，无法进行计算"),
        map<int, string>::value_type( FRN_NO_COUPON_SPREAD_ERROR                , "该券缺少票面利差信息，无法进行计算"),
        map<int, string>::value_type( FRN_CONVENTION_ERROR                      , "无效的浮动债券计算规范"),
        map<int, string>::value_type( COUPON_TYPE_BOND_ERROR                    , "无法识别的债券类型（固息债/浮息债/到期一次还本/贴现券），可能是内部数据错误"),
        map<int, string>::value_type( EMPTY_CASHFLOW_ERROR                      , "现金流为空"),
        map<int, string>::value_type( EMPTY_OPTION_CASHFLOW_ERROR               , "行权时现金流为空"),
        map<int, string>::value_type( SETTLEMENT_DATE_OUTBOUND_EXECUTION_ERROR  , "结算日超出行权日，请将结算日选在如下范围内："),
        map<int, string>::value_type( OPTION_STYLE_SETUP_CALL_LEG_ERROR         , "无法计算的选择权类型(非欧式或美式)"),
        map<int, string>::value_type( OPTION_TYPE_SETUP_CALL_LEG_ERROR          , "无法计算的选择权类型(非赎回或回售)"),
        map<int, string>::value_type( NULL_CALL_DATE_ERROR                      , "赎回日期为空"),
        map<int, string>::value_type( NULL_CALL_STRIKE_ERROR                    , "行权价格为空"),
        map<int, string>::value_type( OPTION_STYLE_SETUP_PUT_LEG_ERROR          , "无法计算的选择权类型(非欧式或美式)"),
        map<int, string>::value_type( OPTION_TYPE_SETUP_PUT_LEG_ERROR           , "无法计算的选择权类型(非赎回或回售)"),
        map<int, string>::value_type( NULL_PUT_DATE_ERROR                       , "回售日期为空"),
        map<int, string>::value_type( NULL_ASS_DATE_ERROR                       , "定向转让日期为空"),
        map<int, string>::value_type( NULL_PUT_STRIKE_ERROR                     , "行权价格为空"),
        map<int, string>::value_type( DSC_CASHFLOW_ERROR                        , "意外的贴现债券现金流"),
        map<int, string>::value_type( DSC_NULL_CALL_DATE_ERROR                  , "赎回日期为空"),
        map<int, string>::value_type( DSC_SETTLEMENT_DATE_OUTBOUND_CALL_ERROR   , "结算日超出行权日，请将结算日选在如下范围内："),
        map<int, string>::value_type( DSC_NULL_PUT_DATE_ERROR                   , "回售日期为空"),
        map<int, string>::value_type( DSC_SETTLEMENT_DATE_OUTBOUND_PUT_ERROR    , "结算日超出行权日，请将结算日选在如下范围内："),
        map<int, string>::value_type( FRN_NULL_HISTORICAL_RATE_ERROR            , "缺少利率历史数据"),
        map<int, string>::value_type( FRN_PBC_FORECAST_ERROR                    , "无法获得默认的利率预期，您可以在‘基准利率’栏中输入您的预期"),
        map<int, string>::value_type( FRN_CFETS_FORECAST_ERROR                  , "无法获得默认的利率预期，您可以在‘基准利率’栏中输入您的预期"),
        map<int, string>::value_type( FRN_INCOMPLETE_RATE_ERROR                 , "历史或预期利率信息不完整"),
        map<int, string>::value_type( FRN_NOT_PAYDAY_ERROR                      , "不是付息日"),
        map<int, string>::value_type( NOT_FRN_ERROR                             , "不是浮动利率债券"),
        map<int, string>::value_type( YTC_UNAVAILABLE_ERROR                     , "该券不含选择权，或所含选择权类型无法计算，或结算日晚于行权日"),
        map<int, string>::value_type( YTP_UNAVAILABLE_ERROR                     , "该券不含选择权，或所含选择权类型无法计算，或结算日晚于行权日"),
        map<int, string>::value_type( INVALID_START_END_DATES_CASHFLOW_ERROR    , "起息日或到期日为空"),
        map<int, string>::value_type( DATE_ORDER_ERROR                          , "期初日期晚于期末日期"),
        map<int, string>::value_type( NULL_REBATE_ERROR                         , "返点为空"),
        map<int, string>::value_type( CAL_DIVERGENT_ERROR                       , "计算结果发散")
    };

    const map<int, string> ERRORCODE(errorInitCN,errorInitCN+63);

    struct ErrorCode {

        static string errorCodeMessage (int errorCode, string str){
            char errorCodeStr[5];
            sprintf(errorCodeStr,"%4d",errorCode);
            string errorMessage = "";
            errorMessage = errorMessage + "Calculator Error Code : " + errorCodeStr + " " + str + ( str=="" ? "" : " ") + ERRORCODE.find(errorCode)->second + "\n";
            return errorMessage;
        }

        static string outBoundErrorMessage (int errorCode, string errorValue, string lowerBound, string upperBound){
            char errorCodeStr[5];
            sprintf(errorCodeStr,"%4d",errorCode);
            string errorMessage = "";
            errorMessage = errorMessage + "Calculator Error Code : " + errorCodeStr + " " + errorValue + " " + ERRORCODE.find(errorCode)->second + "[" + lowerBound + "," + upperBound + "]\n";
            return errorMessage;
        }
    };
    


}

#endif
