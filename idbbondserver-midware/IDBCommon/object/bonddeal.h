
#ifndef _BOND_DEAL_H_
#define _BOND_DEAL_H_

#include "sdbus/string.h"
#include "include/common.h"
#include "include/constants.h"
#include "field.h"
#include <string>

/*typedef struct
{
    Field<std::string> id_;						    // 成交ID
    Field<std::string> company_id;
    Field<std::string> bond_key_listed_market_;     // 键值加市场唯一标示一个债券

    Field<std::string> direction_;                  // 买卖方向, GVN/TKN／TRD
    Field<std::string> create_time_;                // 创建时间 DateTime
    Field<std::string> deal_time_;                  // 成交时间, 成交录入时可编辑,DateTime
    Field<std::string> deal_no_;                    // 成交单号
   
    Field<double> price_;                           // 债券收益率    
	Field<double> volume_;                             // 报价量/成交量/券面总额
	Field<std::string> currency_;                   // 货币, USD/CNY…
	Field<std::string> traded_date_;                // 交易日
	Field<std::string> yield_type_;                 // 收益率类型0-行权收益率，1-到期收益率
	Field<std::string> settlement_type_;            // 结算方式，T+0,T+1
	Field<std::string> settlement_date_;            // 交割日

	Field<double> yield_;                           // 收益率
	Field<double> spread_;                          // 利差
	Field<double> clean_price_;                     // 净价
	Field<double> dirty_price_;                     // 全价

	Field<std::string> bid_institution_id_;         // Bid方机构ID
	Field<std::string> bid_inst_city_;				// Bid方城市
	Field<std::string> bid_trader_id_;              // Bid方交易员ID
	Field<std::string> bid_trader_account_;         // Bid方交易员账户名
	Field<std::string> bid_broker_id_;              // Bid方经纪人ID 
	Field<std::string> bid_broker_id_B_; 
	Field<std::string> bid_broker_id_C_;
	Field<std::string> bid_broker_id_D_;
	Field<std::string> bid_percent_A_;              // Bid方佣金比例
	Field<std::string> bid_percent_B_;
	Field<std::string> bid_percent_C_;
	Field<std::string> bid_percent_D_;
	Field<std::string> bid_is_charged_;             // Bid方是否收费 "2"-收费，"1"-不收费
	Field<std::string> bid_is_no_brokerage_;        // Bid方是否免佣 "2"-免佣，"1"-不免佣
	Field<double> bid_brokerage_;                   // Bid方佣金
	Field<std::string> bid_brokerage_remark_;       // Bid方佣金备注
	Field<std::string> bid_trade_mode_;             // Bid方交易方式
	Field<std::string> bid_no_confirm_;             // Bid方NC "1"-NC，"0"-非NC
	Field<std::string> bid_no_comments_;            // Bid方NC备注
	Field<std::string> bid_is_bridge_;              // Bid方是否为过桥机构 "2"-过桥机构，"1"-非过桥机构

	Field<std::string> ofr_institution_id_;         // Ofr方机构ID
	Field<std::string> ofr_inst_city_;				// Ofr方城市
	Field<std::string> ofr_trader_id_;              // Ofr方交易员ID
	Field<std::string> ofr_trader_account_;         // Ofr方交易员账户名
	Field<std::string> ofr_broker_id_;              // Ofr方经纪人ID
	Field<std::string> ofr_broker_id_B_;
	Field<std::string> ofr_broker_id_C_;
	Field<std::string> ofr_broker_id_D_;
	Field<std::string> ofr_percent_A_;              // Ofr方佣金比例
	Field<std::string> ofr_percent_B_;
	Field<std::string> ofr_percent_C_;
	Field<std::string> ofr_percent_D_;
	Field<std::string> ofr_is_charged_;             // Ofr方是否收费. 数据库中为string型，1,NULL
	Field<std::string> ofr_is_no_brokerage_;        // ofr方是否免佣
	Field<double> ofr_brokerage_;                   // Ofr方佣金
	Field<std::string> ofr_brokerage_remark_;       // Ofr方佣金备注
	Field<std::string> ofr_trade_mode_;             // Ofr方交易方式
	Field<std::string> ofr_no_confirm_;             // Ofr方NC "1"-NC，"0"-非NC
	Field<std::string> ofr_no_comments_;            // Ofr方NC备注
	Field<std::string> ofr_is_bridge_;              // Ofr方是否为过桥机构, 数据库中为string型，1,NULL

	Field<std::string> bid_remarks_;                // Bid方备注
	Field<std::string> remarks_;                    // 后台信息
	Field<std::string> ofr_remarks_;                // Ofr方备注
	Field<std::string> special_party_bid_;          // bid方特别细节
	Field<std::string> special_party_ofr_;           // ofr方特别细节
	Field<std::string> special_terms_;               // 其他细节
	Field<std::string> feedback_;                   // 后台反馈

	Field<std::string> settlement_mode_;            // 结算模式，如DVP.
	Field<std::string> settlement_amount_;          // 结算金额.
	Field<std::string> deal_internal_flag_;         // 内部成交, "1"-非内部成交，“2”-内部成交

	Field<bool> bid_checked_;                       // Bid方经纪人是否已经确认
	Field<bool> ofr_checked_;                       // Ofr方经纪人是否已经确认
	Field<std::string> deal_status_;                // 成交单状态, 0-确认，1-待提交，2-已提交，3-已通过，5-未通过，7-已毁单，13-送审中，-1-已删除
	Field<bool> in_hand_;                           //true:正在处理中 false:正常状态
	Field<std::string> urgent_status_;              //true:加急 false:非加急
	Field<std::string> question_mark_;              //true:点亮问号标记 false:不点亮问号标记

    Field<double> fan_dian_;                        // 返点数值, 比如返0.12  
    Field<std::string> fan_dian_flag_;              // 是否返点 0, 1
    Field<std::string> quote_type_;                 // 报价类型: 收益率/净价/全价/利差
    Field<std::string> description_;                // 描述
    Field<std::string> delivery_date_;              // 交割日期, DateTime
	Field<double> bid_brokerage_rate_;              // Bid方佣金比例
	Field<double> bid_discount_;                    // Bid方折扣
	Field<double> ofr_brokerage_rate_;              // Ofr方佣金比例
	Field<double> ofr_discount_;                    // Ofr方折扣
    Field<int> quote_side_;                         // 该成交所对应的是bid还是Ofr，-1-ofr，1-bid
    Field<std::string> quote_id_;                   // 成交单对应的quote id   

	Field<std::string> related_msg_;                // 编辑过桥拆单时，老成交单的ID   
	Field<std::string> old_contract_id_;             // 过桥关联字段  

} BondDeal;*/

class BondDeal {
public:
    BondDeal() : price_(DECIMAL_NULL), volume_(0), yield_(DECIMAL_NULL), spread_(DECIMAL_NULL), clean_price_(DECIMAL_NULL), dirty_price_(DECIMAL_NULL), 
        bid_brokerage_(DECIMAL_NULL), ofr_brokerage_(DECIMAL_NULL), bid_checked_(false), ofr_checked_(false), in_hand_(false), fan_dian_(false), 
        bid_brokerage_rate_(DECIMAL_NULL), bid_discount_(DECIMAL_NULL), ofr_brokerage_rate_(DECIMAL_NULL), ofr_discount_(false), quote_side_(0){}
    Field<std::string> id_;						    // 成交ID
    Field<std::string> company_id;
    Field<std::string> bond_key_listed_market_;     // 键值加市场唯一标示一个债券

    Field<std::string> direction_;                  // 买卖方向, GVN/TKN／TRD
    Field<std::string> create_time_;                // 创建时间 DateTime
    Field<std::string> deal_time_;                  // 成交时间, 成交录入时可编辑,DateTime
    Field<std::string> deal_no_;                    // 成交单号

    Field<double> price_;                           // 债券收益率    
    Field<double> volume_;                             // 报价量/成交量/券面总额
    Field<std::string> currency_;                   // 货币, USD/CNY…
    Field<std::string> traded_date_;                // 交易日
    Field<std::string> yield_type_;                 // 收益率类型0-行权收益率，1-到期收益率
    Field<std::string> settlement_type_;            // 结算方式，T+0,T+1
    Field<std::string> settlement_date_;            // 交割日

    Field<double> yield_;                           // 收益率
    Field<double> spread_;                          // 利差
    Field<double> clean_price_;                     // 净价
    Field<double> dirty_price_;                     // 全价

    Field<std::string> bid_institution_id_;         // Bid方机构ID
    Field<std::string> bid_inst_city_;				// Bid方城市
    Field<std::string> bid_trader_id_;              // Bid方交易员ID
    Field<std::string> bid_trader_account_;         // Bid方交易员账户名
    Field<std::string> bid_broker_id_;              // Bid方经纪人ID 
    Field<std::string> bid_broker_id_B_;
    Field<std::string> bid_broker_id_C_;
    Field<std::string> bid_broker_id_D_;
    Field<std::string> bid_percent_A_;              // Bid方佣金比例
    Field<std::string> bid_percent_B_;
    Field<std::string> bid_percent_C_;
    Field<std::string> bid_percent_D_;
    Field<std::string> bid_is_charged_;             // Bid方是否收费 "2"-收费，"1"-不收费
    Field<std::string> bid_is_no_brokerage_;        // Bid方是否免佣 "2"-免佣，"1"-不免佣
    Field<double> bid_brokerage_;                   // Bid方佣金
    Field<std::string> bid_brokerage_remark_;       // Bid方佣金备注
    Field<std::string> bid_trade_mode_;             // Bid方交易方式
    Field<std::string> bid_no_confirm_;             // Bid方NC "1"-NC，"0"-非NC
    Field<std::string> bid_no_comments_;            // Bid方NC备注
    Field<std::string> bid_is_bridge_;              // Bid方是否为过桥机构 "2"-过桥机构，"1"-非过桥机构

    Field<std::string> ofr_institution_id_;         // Ofr方机构ID
    Field<std::string> ofr_inst_city_;				// Ofr方城市
    Field<std::string> ofr_trader_id_;              // Ofr方交易员ID
    Field<std::string> ofr_trader_account_;         // Ofr方交易员账户名
    Field<std::string> ofr_broker_id_;              // Ofr方经纪人ID
    Field<std::string> ofr_broker_id_B_;
    Field<std::string> ofr_broker_id_C_;
    Field<std::string> ofr_broker_id_D_;
    Field<std::string> ofr_percent_A_;              // Ofr方佣金比例
    Field<std::string> ofr_percent_B_;
    Field<std::string> ofr_percent_C_;
    Field<std::string> ofr_percent_D_;
    Field<std::string> ofr_is_charged_;             // Ofr方是否收费. 数据库中为string型，1,NULL
    Field<std::string> ofr_is_no_brokerage_;        // ofr方是否免佣
    Field<double> ofr_brokerage_;                   // Ofr方佣金
    Field<std::string> ofr_brokerage_remark_;       // Ofr方佣金备注
    Field<std::string> ofr_trade_mode_;             // Ofr方交易方式
    Field<std::string> ofr_no_confirm_;             // Ofr方NC "1"-NC，"0"-非NC
    Field<std::string> ofr_no_comments_;            // Ofr方NC备注
    Field<std::string> ofr_is_bridge_;              // Ofr方是否为过桥机构, 数据库中为string型，1,NULL

    Field<std::string> bid_remarks_;                // Bid方备注
    Field<std::string> remarks_;                    // 后台信息
    Field<std::string> ofr_remarks_;                // Ofr方备注
    Field<std::string> special_party_bid_;          // bid方特别细节
    Field<std::string> special_party_ofr_;           // ofr方特别细节
    Field<std::string> special_terms_;               // 其他细节
    Field<std::string> feedback_;                   // 后台反馈

    Field<std::string> settlement_mode_;            // 结算模式，如DVP.
    Field<std::string> settlement_amount_;          // 结算金额.
    Field<std::string> deal_internal_flag_;         // 内部成交, "1"-非内部成交，“2”-内部成交

    Field<bool> bid_checked_;                       // Bid方经纪人是否已经确认
    Field<bool> ofr_checked_;                       // Ofr方经纪人是否已经确认
    Field<std::string> deal_status_;                // 成交单状态, 0-确认，1-待提交，2-已提交，3-已通过，5-未通过，7-已毁单，13-送审中，-1-已删除
    Field<bool> in_hand_;                           //true:正在处理中 false:正常状态
    Field<std::string> urgent_status_;              //true:加急 false:非加急
    Field<std::string> question_mark_;              //true:点亮问号标记 false:不点亮问号标记

    Field<double> fan_dian_;                        // 返点数值, 比如返0.12  
    Field<std::string> fan_dian_flag_;              // 是否返点 0, 1
    Field<std::string> quote_type_;                 // 报价类型: 收益率/净价/全价/利差
    Field<std::string> description_;                // 描述
    Field<std::string> delivery_date_;              // 交割日期, DateTime
    Field<double> bid_brokerage_rate_;              // Bid方佣金比例
    Field<double> bid_discount_;                    // Bid方折扣
    Field<double> ofr_brokerage_rate_;              // Ofr方佣金比例
    Field<double> ofr_discount_;                    // Ofr方折扣
    Field<int> quote_side_;                         // 该成交所对应的是bid还是Ofr，-1-ofr，1-bid
    Field<std::string> quote_id_;                   // 成交单对应的quote id   

    Field<std::string> related_msg_;                // 编辑过桥拆单时，老成交单的ID   
    Field<std::string> old_contract_id_;             // 过桥关联字段  
};

typedef std::tr1::shared_ptr<BondDeal> BondDealPtr;
typedef std::tr1::shared_ptr<std::vector<BondDealPtr> > BondDealVecPtr;

#endif // _BOND_DEAL_H_
