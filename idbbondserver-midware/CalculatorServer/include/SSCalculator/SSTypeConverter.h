#ifndef SS_TYPE_CONVERTER_H
#define SS_TYPE_CONVERTER_H

#include <ql/quantlib.hpp>
#include <string>

using namespace QuantLib;
using std::string;

namespace ssCalculator {
    
    class SSTypeConverter{
    public:
        SSTypeConverter(){}
        Date YYYYMMDD2qldate(const string& str) const;
        string qldate2YYYYMMDD(const Date& date) const;
        double replaceToNull(const double x) const;
        double replaceNull(const double x) const;
        TimeUnit str2TimeUnit (const string& str) const;
        
    };

}

#endif

