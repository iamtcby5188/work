/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    calculator data loader, have different implementations.
====================================================================================================================
   Date       Name                                  Description of Change
13-June-2014  Michael Chen                              Initial version
$HISTORY$
===================================================================================================================*/

#ifndef BCSERVER_BUSINESS_CALCULATOR_DATA_LOADER_H_
#define BCSERVER_BUSINESS_CALCULATOR_DATA_LOADER_H_

#include <vector>

#include <boost/shared_ptr.hpp>

#include "include/SSCalculator/SSSingleRawBondFundamentalInfo.h"
#include "include/SSCalculator/SSSingleInterestRate.h"
#include "include/SSCalculator/SSSingleCalendar.h"

using std::vector;

using ssCalculator::SSSingleRawBondFundamentalInfo;
using ssCalculator::SSSingleInterestRate;
using ssCalculator::SSSingleCalendar;

namespace bcserver_business_calculator {

// This class is supposed to get initializing data for 
// the singleton instance of SSCalculator(but does not initialize it).
// Different enumerate DataSource in constructor indicates
// different data source.
// Sample usage:
// CalculatorDataLoader dataLoader = CalculatorDataLoader(CalculatorDataLoader::kSSDB);
// vector<SSSingleRawBondFundamentalInfo> fundmentalInfoList = dataLoader.getBondFundamentalInfoList();

// TODO(Michael): add implementation for data source from data bus when available

class CalculatorDataLoader {
 public:
  enum DataSource {
    kSSDB,
    kALLDB
  };
  CalculatorDataLoader(DataSource source = kSSDB);
  vector<SSSingleRawBondFundamentalInfo> GetBondFundamentalInfoList() const;
  vector<SSSingleInterestRate> GetInterestRates() const;
  vector<SSSingleCalendar> GetCalendar() const;

protected:
  class Impl {
   public:
    virtual ~Impl() {}
    virtual vector<SSSingleRawBondFundamentalInfo> GetBondFundamentalInfoList() const = 0;
    virtual vector<SSSingleInterestRate> GetInterestRates() const = 0;
    virtual vector<SSSingleCalendar> GetCalendar() const = 0;
  };
  boost::shared_ptr<Impl> impl_;

private:
  class SsdbImpl : public CalculatorDataLoader::Impl {
   public:
    vector<SSSingleRawBondFundamentalInfo> GetBondFundamentalInfoList() const;
    vector<SSSingleInterestRate> GetInterestRates() const;
    vector<SSSingleCalendar> GetCalendar() const;
  };
	class AllDbImpl : public CalculatorDataLoader::Impl {
   public:
    vector<SSSingleRawBondFundamentalInfo> GetBondFundamentalInfoList() const;
    vector<SSSingleInterestRate> GetInterestRates() const;
    vector<SSSingleCalendar> GetCalendar() const;
  };

};

inline vector<SSSingleRawBondFundamentalInfo> CalculatorDataLoader::GetBondFundamentalInfoList() const {
  return impl_->GetBondFundamentalInfoList();
}

inline vector<SSSingleInterestRate> CalculatorDataLoader::GetInterestRates() const {
  return impl_->GetInterestRates();
}

inline vector<SSSingleCalendar> CalculatorDataLoader::GetCalendar() const {
  return impl_->GetCalendar();
}

}  //namespace bcserver_business_calculator


#endif