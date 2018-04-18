#ifndef SS_SINGLE_INTEREST_RATE_H
#define SS_SINGLE_INTEREST_RATE_H

#include "SSCalculatorError.h"
#include <string>

using std::string;

namespace ssCalculator {

    class SSSingleInterestRate{
    public:

        SSSingleInterestRate(){
            index_id_ = "";
            startDate_ = "";
            endDate_ = "";
            value_ = INVALID_VALUE;
        }
        
        string index_id_;
        string startDate_;
        string endDate_;
        double value_;

    };

}

#endif

