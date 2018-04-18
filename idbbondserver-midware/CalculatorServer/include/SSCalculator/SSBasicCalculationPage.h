#ifndef SS_BASIC_CALCULATION_PAGE_H
#define SS_BASIC_CALCULATION_PAGE_H

#include "SSCalculatorError.h"
#include <string>

using std::string;

namespace ssCalculator {
    class SSBasicCalculationPage{
    public:

        SSBasicCalculationPage(){
            cleanPrice_ = INVALID_VALUE;
            dirtyPrice_ = INVALID_VALUE;
            yieldToMaturity_ = INVALID_VALUE;
            spread_ = INVALID_VALUE;
            yieldToCall_ = INVALID_VALUE;
            yieldToPut_ = INVALID_VALUE;
            accruedDays_ = INVALID_VALUE;
            accruedAmount_ = INVALID_VALUE;
            maturityDate_ = "";
            indexRate_ = INVALID_VALUE;
            indexDate_ = "";
            executionDate_ = "";
            MacaulayDuration_ = INVALID_VALUE;
            modifiedDuration_ = INVALID_VALUE;
            pvbp_ = INVALID_VALUE;
            convexity_ = INVALID_VALUE;
            couponRateSpread_ = INVALID_VALUE;
            exceptions_ = "";
        }

        double cleanPrice_;
        double dirtyPrice_;
        double yieldToMaturity_;
        double spread_;
        double yieldToCall_;
        double yieldToPut_;
        int    accruedDays_;
        double accruedAmount_;
        string maturityDate_;
        double indexRate_;
        string indexDate_;
        string executionDate_;
        double MacaulayDuration_;
        double modifiedDuration_;
        double pvbp_;
        double convexity_;
        double couponRateSpread_;
        string exceptions_;
    };
}

#endif
