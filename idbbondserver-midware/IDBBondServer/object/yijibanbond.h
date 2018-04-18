#ifndef _YIJIBAN_BOND_H_
#define _YIJIBAN_BOND_H_

#include "sdbus/string.h"
#include <string>

typedef struct 
{
    sdbus::string bond_key_;
    sdbus::string avg_price_;
    sdbus::string coupon_rate_;
    sdbus::string fandian_;
    sdbus::string delflag_;
 
} YiJiBanBond;

typedef std::tr1::shared_ptr<YiJiBanBond> YiJiBanBondPtr;

#endif