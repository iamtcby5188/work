#ifndef SS_SINGLE_CALENDAR_H
#define SS_SINGLE_CALENDAR_H

#include "SSCalculatorError.h"
#include <string>

using std::string;

namespace ssCalculator {

    class SSSingleCalendar{
    public:
        
        SSSingleCalendar(){
            country_ = "";
            market_ = "";
            date_ = "";
            isBusinessDay_ = true;
        }

        string country_;
        string market_;
        string date_;
        bool isBusinessDay_;

    };
}

#endif

