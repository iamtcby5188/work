#ifndef SS_SINGLE_CASHFLOW_H
#define SS_SINGLE_CASHFLOW_H

#include "SSCalculatorError.h"
#include <string>

using std::string;

namespace ssCalculator {
    class SSSingleCashflow{
    public:

        SSSingleCashflow(){
            date_ = "";
            couponRate_ = INVALID_VALUE;
            spread_ = INVALID_VALUE;
            indexRate_ = INVALID_VALUE;
            coupon_ = INVALID_VALUE;
            redemption_ = INVALID_VALUE;
            cashflow_ = INVALID_VALUE;
            notional_ = INVALID_VALUE;
        }
        string date_;
        double couponRate_;
        double spread_;
        double indexRate_;
        double coupon_;
        double redemption_;
        double cashflow_;
        double notional_;
    };
}

#endif
