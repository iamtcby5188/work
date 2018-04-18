#ifndef SS_HOLDING_PERIOD_RESULT_H
#define SS_HOLDING_PERIOD_RESULT_H

#include "SSCalculatorError.h"
#include <string>

using std::string;

namespace ssCalculator {
    class SSHoldingPeriodResult{
    public:

        SSHoldingPeriodResult(){
            // start
            startCleanAmount_ = INVALID_VALUE;
            startInterest_ = INVALID_VALUE;
            startTotal_ = INVALID_VALUE;

            // end
            endCleanAmount_ = INVALID_VALUE;
            endInterest_ = INVALID_VALUE;
            endTotal_ = INVALID_VALUE;

            // between
            principalRepaymentBetween_ = INVALID_VALUE;
            interestBetween_ = INVALID_VALUE;

            // pre tax
            cleanAmountPnLPreTax_ = INVALID_VALUE;
            interestPnLPreTax_ = INVALID_VALUE;
            frontDeskCommissionPreTax_ = INVALID_VALUE;
            backStageCommissionPreTax_ = INVALID_VALUE;
            PnLPretax_ = INVALID_VALUE;
            holdingPeriodYieldPreTax_ = INVALID_VALUE;
            annulizedHoldingPeriodYieldPreTax_ = INVALID_VALUE;

            // tax & cost
            businessTax_ = INVALID_VALUE;
            interestTax_ = INVALID_VALUE;
            totalTax_ = INVALID_VALUE;
            holdingCost_ = INVALID_VALUE;

            // after tax
            cleanAmountPnLAT_ = INVALID_VALUE;
            interestPnLAT_ = INVALID_VALUE;
            frontDeskCommissionAT_ = INVALID_VALUE;
            backStageCommissionAT_ = INVALID_VALUE;
            PnLAT_ = INVALID_VALUE;
            holdingPeriodYieldAT_ = INVALID_VALUE;
            annulizedHoldingPeriodYieldAT_ = INVALID_VALUE;
            netPnL_ = INVALID_VALUE;
            netAnnualizedYield_ = INVALID_VALUE;
        }

        // start
        double startCleanAmount_;
        double startInterest_;
        double startTotal_;

        // end
        double endCleanAmount_;
        double endInterest_;
        double endTotal_;

        // between
        double principalRepaymentBetween_;
        double interestBetween_;

        // pre tax
        double cleanAmountPnLPreTax_;
        double interestPnLPreTax_;
        double frontDeskCommissionPreTax_;
        double backStageCommissionPreTax_;
        double PnLPretax_;
        double holdingPeriodYieldPreTax_;
        double annulizedHoldingPeriodYieldPreTax_;

        // tax & cost
        double businessTax_;
        double interestTax_;
        double totalTax_;
        double holdingCost_;

        // after tax
        double cleanAmountPnLAT_;
        double interestPnLAT_;
        double frontDeskCommissionAT_;
        double backStageCommissionAT_;
        double PnLAT_;
        double holdingPeriodYieldAT_;
        double annulizedHoldingPeriodYieldAT_;
        double netPnL_;
        double netAnnualizedYield_;
    };
}

#endif
