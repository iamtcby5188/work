#ifndef _MANAGEMENT_H_
#define _MANAGEMENT_H_

#include "sdbus/string.h"
#include <string>

//产品管理--常量设置
/*typedef struct
{
    sdbus::string constants_id_;
    sdbus::string constants_name_;
    double constants_value_;
    sdbus::string constants_key_;
    sdbus::string create_time_;
    bool succeed_;                          // 更新成功标记
}Constants;*/
class Constants {
public:
    Constants():constants_value_(0), succeed_(false) {}

    sdbus::string constants_id_;
    sdbus::string constants_name_;
    double constants_value_;
    sdbus::string constants_key_;
    sdbus::string create_time_;
    bool succeed_;                          // 更新成功标记
};

//产品管理--中债估值偏离值设置
/*typedef struct
{
    sdbus::string id_;
    sdbus::string bond_type_id_;
    sdbus::string bond_type_;
    double deviated_value_;
    sdbus::string modify_time_;
    bool succeed_;                          
}ChinaBondDeviatedValue;*/
class ChinaBondDeviatedValue {
public:
    ChinaBondDeviatedValue() :deviated_value_(0), succeed_(false){}
    sdbus::string id_;
    sdbus::string bond_type_id_;
    sdbus::string bond_type_;
    double deviated_value_;
    sdbus::string modify_time_;
    bool succeed_;
};

typedef std::tr1::shared_ptr<Constants> ConstantsPtr;
typedef std::tr1::shared_ptr<ChinaBondDeviatedValue> ChinaBondDeviatedValuePtr;
#endif // _MANAGEMENT_H_
