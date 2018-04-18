#ifndef SS_HOLDING_PERIOD_INTEREST_H
#define SS_HOLDING_PERIOD_INTEREST_H

#include "SSCalculatorError.h"
#include <string>

using std::string;

namespace ssCalculator {
    class SSHoldingPeriodInterest{
    public:

        SSHoldingPeriodInterest(){
            startDate_ = "";
            endDate_ = "";
            accruedDays_ = INVALID_VALUE;
            indexRate_ = INVALID_VALUE;
            accruedAmount_ = INVALID_VALUE;
        }

        string startDate_;
        string endDate_;
        int accruedDays_;
        double indexRate_;
        double accruedAmount_;
    };
}

#endif
