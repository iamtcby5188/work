#ifndef _BOND_H_
#define _BOND_H_

#include "sdbus/string.h"
#include <string>
#include <memory>

typedef struct 
{
    sdbus::string bond_id;
    sdbus::string short_name;
    sdbus::string type;
    sdbus::string rating;
    sdbus::string maturity;
    sdbus::string coupon_rate;
}Bond;

/*typedef struct 
{
    sdbus::string bond_code_;
    sdbus::string bond_key_;
    sdbus::string bond_short_name_;
    sdbus::string listed_market_;
    sdbus::string bond_key_listed_market_;
    sdbus::string pin_yin_;
    sdbus::string pin_yin_full_;
    sdbus::string isser_rating_;
    int maturity_date_;
    sdbus::string time_to_maturity_;
    sdbus::int32_t listed_date_;
    float issue_amount_;
    sdbus::string under_writer_name_;
    float coupon_rate_;
	sdbus::string rate_type;
    sdbus::string bond_type_;
    sdbus::string option_type_;
    sdbus::string issue_institution_;  //发行机构
    sdbus::string issue_institution_pinyin_;
    sdbus::string issue_institution_pinyinfull_;
    sdbus::string bond_category_;  //债券类型
    sdbus::string option_type_client_;
    sdbus::string mkt_type_;
    sdbus::string valuation_yield_; //债券估值收益
    sdbus::string valuation_net_price_;
}BondSmallView;*/
class BondSmallView {
public:
    BondSmallView():maturity_date_(0),listed_date_(0),issue_amount_(0),coupon_rate_(0) {}
    sdbus::string bond_code_;
    sdbus::string bond_key_;
    sdbus::string bond_short_name_;
    sdbus::string listed_market_;
    sdbus::string bond_key_listed_market_;
    sdbus::string pin_yin_;
    sdbus::string pin_yin_full_;
    sdbus::string isser_rating_;
    int maturity_date_;
    sdbus::string time_to_maturity_;
    sdbus::int32_t listed_date_;
    float issue_amount_;
    sdbus::string under_writer_name_;
    float coupon_rate_;
    sdbus::string rate_type;
    sdbus::string bond_type_;
    sdbus::string option_type_;
    sdbus::string issue_institution_;  //发行机构
    sdbus::string issue_institution_pinyin_;
    sdbus::string issue_institution_pinyinfull_;
    sdbus::string bond_category_;  //债券类型
    sdbus::string option_type_client_;
    sdbus::string mkt_type_;
    sdbus::string valuation_yield_; //债券估值收益
    sdbus::string valuation_net_price_;
};

typedef struct  
{
    sdbus::string institution_id_;
    sdbus::string institution_code_;
    sdbus::string institution_name_;
    sdbus::string pin_yin_;
    sdbus::string pin_yin_full_;
    sdbus::string institution_full_name_;
}InstitutionInfo;

/*typedef struct  
{
    sdbus::string trader_id_;
    sdbus::string institution_code_;
    sdbus::string trader_code_;
    sdbus::string pin_yin_;
    sdbus::string pin_yin_full_;
    sdbus::string trader_name_;
    bool vip_flag_;
    bool bad_flag_;
    sdbus::string trade_able_bond_category_;   //交易员可交易债券种类，利率或信用，比如‘BCO’，‘BCO,BNC’
}TraderInfo;*/
class TraderInfo {
public:
    TraderInfo() :vip_flag_(false), bad_flag_(false){}
    sdbus::string trader_id_;
    sdbus::string institution_code_;
    sdbus::string trader_code_;
    sdbus::string pin_yin_;
    sdbus::string pin_yin_full_;
    sdbus::string trader_name_;
    bool vip_flag_;
    bool bad_flag_;
    sdbus::string trade_able_bond_category_;   //交易员可交易债券种类，利率或信用，比如‘BCO’，‘BCO,BNC’
};

typedef struct  
{
    sdbus::string id_;
    sdbus::string username_;
    sdbus::string display_name_;
    sdbus::string password_;
    sdbus::string company_id_;
    sdbus::string company_name_;
    sdbus::string department_;
}AccountInfo;

typedef struct  
{
    sdbus::string broker_id_;
    sdbus::string trader_id_;
}BrokerBindingTrader;

typedef std::tr1::shared_ptr<Bond> BondPtr;
typedef std::tr1::shared_ptr<BondSmallView> BondSmallViewPtr;
typedef std::tr1::shared_ptr<InstitutionInfo> InstitutionInfoPtr;
typedef std::tr1::shared_ptr<TraderInfo> TraderInfoPtr;
typedef std::tr1::shared_ptr<AccountInfo> AccountInfoPtr;
typedef std::tr1::shared_ptr<BrokerBindingTrader> BrokerBindingTraderPtr;
#endif // _BOND_H_
