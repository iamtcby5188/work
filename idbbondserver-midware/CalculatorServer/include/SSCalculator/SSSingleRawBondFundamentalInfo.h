#ifndef SS_SINGLE_RAW_BOND_FUNDAMENTAL_INFO_H
#define SS_SINGLE_RAW_BOND_FUNDAMENTAL_INFO_H

#include "SSCalculatorError.h"
#include <string>

using std::string;

namespace ssCalculator {

    class SSSingleRawBondFundamentalInfo{
    public:
        
        SSSingleRawBondFundamentalInfo(){
            Bond_Key_ = "";
            Bond_ID_ = "";
            Interest_Start_Date_ = "";
            Maturity_Date_ = "";
            Listed_Date_ = "";
            First_Coupon_Date_ = "";
            Option_Type_ = "";
            Option_Style_ = "";
            Call_No_ = INVALID_VALUE;
            Call_Str_ = "";
            Put_No_ = INVALID_VALUE;
            Put_Str_ = "";
            First_Exchg_Bond_Key_ = "";
            First_Exchg_No_ = INVALID_VALUE;
            First_Exchg_Str_ = "";
            Sec_Exchg_No_ = INVALID_VALUE;
            Sec_Exchg_Year_ = "";
            Assign_Trans_Key_ = "";
            Assign_Trans_No_ = INVALID_VALUE;
            Assign_Trans_Str_ = "";
            Compensate_Rate_ = "";
            Compensate_From_ = INVALID_VALUE;
            Option_Exercise_ = "";
            Option_Exercise_Date_ = "";
            Coupon_Type_ = "";
            Coupon_Rate_Spread_ = INVALID_VALUE;
            Coupon_Frequency_ = "";
            Compound_Frequency_ = "";
            Interest_Basis_ = "";
            Coupon_Dist_ = "";
            FRN_Multiplier_ = INVALID_VALUE;
            FRN_Index_ID_ = "";
            First_Index_Rate_ = INVALID_VALUE;
            Fixing_Frequency_ = "";
            Fixing_MA_Days_ = INVALID_VALUE;
            Fixing_Preceds_ = "";
            Fixing_Calendar_Key_ = "";
            Fixing_Tailing_ = "";
            Fixing_Digit_ = INVALID_VALUE;
            Reset_Effective_ = "";
            Cap_ = INVALID_VALUE;
            Flr_ = INVALID_VALUE;
            Simple_Compound_ = "";
            Variable_Schedule_ = "";
            Coupon_Day_Adjust_ = "";
            Coupon_Day_DMC_ = "";
            Coupon_Calendar_Key_ = "";
            Pricing_Conv_ = "";
            Issue_Price_ = INVALID_VALUE;
            Redemption_No_ = INVALID_VALUE;
            Redemption_Str_ = "";
            Maturity_Adjust_ = "";
            Maturity_DMC_ = "";
            Maturity_Calendar_Key_ = "";

            Issuer_Code_ = "";
            External_Type_ = "";
            Maturity_Term_Y_ = INVALID_VALUE;
            Maturity_Term_ = INVALID_VALUE;
            Term_Unit_ = "";
            Announce_Date_ = "";
            Issue_Start_Date_ = "";
            Issue_End_Date_ = "";
            WI_Start_Date_ = "";
            WI_End_Date_ = "";
            Dist_Date_Start_ = "";
            Dist_Date_End_ = "";
            Auction_Date_Start_ = "";
            Auction_Date_End_ = "";
            Payment_Date_ = "";
            Delisted_Date_ = "";
            Coupon_Rate_Current_ = INVALID_VALUE;
            Issue_Reference_ = "";
            Issue_Reference_Desc_ = "";
            Issue_Rate_ = INVALID_VALUE;
            Planned_Issue_Amount_ = INVALID_VALUE;
            First_Issue_Amount_ = INVALID_VALUE;
            Issue_Amount_ = INVALID_VALUE;
            Rating_Current_ = "";
            Rating_Institution_Code_ = "";
            Rating_Date_ = "";
            Rating_Augment_ = "";
            Warranter_ = "";
            Warrant_Note_ = "";
            Bond_Collateral_ = "";
            Collateral_Value_ = INVALID_VALUE;
            Evaluation_Date_ = "";
            Collateral_Type_ = "";
            Issuer_Rating_Current_ = "";
            Issuer_Rating_Date_ = "";
            Issue_Commission_Rate_ = INVALID_VALUE;
        }

        string    Bond_Key_;
        string    Bond_ID_;
        string    Issuer_Code_;
        string    External_Type_;
        double    Maturity_Term_Y_;
        int       Maturity_Term_;
        string    Term_Unit_;
        string    Interest_Start_Date_;
        string    Maturity_Date_;
        string    First_Coupon_Date_;
        string    Announce_Date_;
        string    Issue_Start_Date_;
        string    Issue_End_Date_;
        string    WI_Start_Date_;
        string    WI_End_Date_;
        string    Dist_Date_Start_;
        string    Dist_Date_End_;
        string    Auction_Date_Start_;
        string    Auction_Date_End_;
        string    Payment_Date_;
        string    Listed_Date_;
        string    Delisted_Date_;
        string    Option_Type_;
        string    Option_Style_;
        int       Call_No_;
        string    Call_Str_;
        int       Put_No_;
        string    Put_Str_;
        string    First_Exchg_Bond_Key_;
        int       First_Exchg_No_;
        string    First_Exchg_Str_;
        int       Sec_Exchg_No_;
        string    Sec_Exchg_Year_;
        string    Assign_Trans_Key_;
        int       Assign_Trans_No_;
        string    Assign_Trans_Str_;
        string    Compensate_Rate_;
        int       Compensate_From_;
        string    Option_Exercise_;
        string    Option_Exercise_Date_;
        string    Coupon_Type_;
        double    Coupon_Rate_Spread_;
        double    Coupon_Rate_Current_;
        string    Coupon_Frequency_;
        string    Compound_Frequency_;
        string    Interest_Basis_;
        string    Coupon_Dist_;
        double    FRN_Multiplier_;
        string    FRN_Index_ID_;
        double    First_Index_Rate_;
        string    Fixing_Frequency_;
        int       Fixing_MA_Days_;
        string    Fixing_Preceds_;
        string    Fixing_Calendar_Key_;
        string    Fixing_Tailing_;
        int       Fixing_Digit_;
        string    Reset_Effective_;
        double    Cap_;
        double    Flr_;
        string    Simple_Compound_;
        string    Issue_Reference_;
        string    Issue_Reference_Desc_;
        string    Variable_Schedule_;
        string    Coupon_Day_Adjust_;
        string    Coupon_Day_DMC_;
        string    Coupon_Calendar_Key_;
        string    Pricing_Conv_;
        double    Issue_Price_;
        double    Issue_Rate_;
        double    Planned_Issue_Amount_;
        double    First_Issue_Amount_;
        double    Issue_Amount_;
        int       Redemption_No_;
        string    Redemption_Str_;
        string    Maturity_Adjust_;
        string    Maturity_DMC_;
        string    Maturity_Calendar_Key_;
        string    Rating_Current_;
        string    Rating_Institution_Code_;
        string    Rating_Date_;
        string    Rating_Augment_;
        string    Warranter_;
        string    Warrant_Note_;
        string    Bond_Collateral_;
        double    Collateral_Value_;
        string    Evaluation_Date_;
        string    Collateral_Type_;
        string    Issuer_Rating_Current_;
        string    Issuer_Rating_Date_;
        double    Issue_Commission_Rate_;


                
    };
}

#endif

