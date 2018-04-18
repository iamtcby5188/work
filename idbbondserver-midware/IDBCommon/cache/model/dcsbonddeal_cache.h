
#ifndef DCSBONDDEAL_CACHE_H_
#define DCSBONDDEAL_CACHE_H_

#include "bondsmallview_cache.h"

class DcsBondDealCache;
typedef std::tr1::shared_ptr<DcsBondDealCache> DcsBondDealCachePtr;
typedef std::tr1::shared_ptr<std::vector<DcsBondDealCachePtr> > DcsBondDealCacheVecPtr;

class DcsBondDealCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
	static std::string SQL_TAG;

public:
	DcsBondDealCache();
	virtual ~DcsBondDealCache();
	std::string getPrimaryKey();

	bool operator ==(const DcsBondDealCache &rhs);
	bool operator !=(const DcsBondDealCache &rhs);

    void loadCombinationFields();
    void loadBondSmallViewFields();
    void loadTradingInstitutionFields();
    void loadTraderFields();
    void loadBrokerFields();
	void loadCdcValuationFields();

public:
    std::string id;
    std::string company_id;
	std::string bond_key_listed_market;                   // 键值加市场唯一标示一个债券
	std::string create_time;                              // 成交时间
	std::string updateTime;                               // 更新时间
	std::string urgent;                                   // 加急标志 "true"-加急，"false"-非加急
	std::string questionMark;                             // 问号标记 "true"-点亮，"false"-不点亮

	std::string deal_type;                                // 买卖方向, GVN/TKN/TRD
    std::string deal_time;                                // 交易时间
	std::string odd_numbers;                              // 单号
	int sequence_number;                                  // 序列号

	std::string goods_id;
	std::string goods_code;
	std::string goods_short_name;
	std::string bond_key;
	std::string listed_market;
	std::string maturity_date;                            // 债券到期日

	real8 price;                                          // 价格
	real8 return_point;                                   // 返点数值, 比如返0.12  
	std::string rebate;                                   // 是否返点 0, 1
	real8 volume;                                          // 数量 券面总额
	std::string currency;                                 // 货币类型 CNY
	std::string exercise;                                 // 行权到期 0-行权 1-到期
	std::string time_style;                               // 结算方式 T+0 T+1
	std::string settlement_date;                          // 交割日

	real8 yield;                                          // 收益率
	real8 spread;                                         // 利差
	real8 net_price;                                      // 净价
	real8 full_price;                                     // 全价
	
    std::string bid_bank_id;                              // Bid方机构id
    std::string bid_bank_code;                            // Bid方机构代码
    std::string bid_bank_city;                            // Bid方机构所在地
    std::string bid_agent_id;                             // Bid方交易员id
    std::string bid_agent_code;                           // Bid方交易员代码
	std::string bidBankAgentName;                         // Bid方交易员名字
    std::string bid_broker_id;                            // Bid方经纪人id A
	std::string bidBrokerIdB;                             // Bid方经纪人id B
	std::string bidBrokerIdC;                             // Bid方经纪人id C
	std::string bidBrokerIdD;                             // Bid方经纪人id D
    std::string bid_broker_name;                          // Bid方经纪人名字 A
	std::string bidBrokerCodeA;                           // Bid方经纪人代码 A
	std::string bidBrokerCodeB;                           // Bid方经纪人代码 B
	std::string bidBrokerCodeC;                           // Bid方经纪人代码 C
	std::string bidBrokerCodeD;                           // Bid方经纪人代码 D
	std::string bidPercentA;                              // Bid方经纪人比例 A
	std::string bidPercentB;                              // Bid方经纪人比例 B
	std::string bidPercentC;                              // Bid方经纪人比例 C
	std::string bidPercentD;                              // Bid方经纪人比例 D
	std::string bid_charge;                               // Bid方是否收费 "2"-收费，"1"-不收费
	std::string bidIsNoBrokerage;                         // Bid方是否免佣 "2"-免佣，"1"-不免佣
    real8 bid_brokerrage;                                 // Bid方佣金
	std::string bidBrokerrageRemark;                      // Bid方佣金备注
	std::string bidTradeMode;                             // Bid方交易方式
	std::string bidNoConfirm;                             // Bid方NC "1"-NC，"0"-非NC
	std::string bidNoComments;                            // Bid方NC备注
	std::string bid_bridge;                               // Bid方是否为过桥机构 "2"-过桥机构， "1"-非过桥机构

    std::string ofr_bank_id;                              // Ofr方机构id
	std::string ofr_bank_code;                            // Ofr方机构代码
	std::string ofr_bank_city;                            // Ofr方机构所在地
	std::string ofr_agent_id;                             // Ofr方交易员id
	std::string ofr_agent_code;                           // Ofr方交易员代码
	std::string ofrBankAgentName;                         // Ofr方交易员名字
	std::string ofr_broker_id;                            // Ofr方经纪人id A
	std::string ofrBrokerIdB;                             // Ofr方经纪人id B
	std::string ofrBrokerIdC;                             // Ofr方经纪人id C
	std::string ofrBrokerIdD;                             // Ofr方经纪人id D
	std::string ofr_broker_name;                          // Ofr方经纪人名字 A
	std::string ofrBrokerCodeA;                           // Ofr方经纪人代码 A
	std::string ofrBrokerCodeB;                           // Ofr方经纪人代码 B
	std::string ofrBrokerCodeC;                           // Ofr方经纪人代码 C
	std::string ofrBrokerCodeD;                           // Ofr方经纪人代码 D
	std::string ofrPercentA;                              // Ofr方经纪人比例 A
	std::string ofrPercentB;                              // Ofr方经纪人比例 B
	std::string ofrPercentC;                              // Ofr方经纪人比例 C
	std::string ofrPercentD;                              // Ofr方经纪人比例 D
	std::string ofr_charge;                               // Ofr方是否收费 "2"-收费，"1"-不收费
	std::string ofrIsNoBrokerage;                         // Ofr方是否免佣 "2"-免佣，"1"-不免佣
    real8 ofr_brokerrage;                                 // Ofr方佣金
	std::string ofrBrokerrageRemark;                      // Ofr方佣金备注
	std::string ofrTradeMode;                             // ofr方交易方式
	std::string ofrNoConfirm;                             // ofr方NC "1"-NC，"0"-非NC
	std::string ofrNoComments;                            // ofr方NC备注 
	std::string ofr_bridge;                               // Ofr方是否为过桥机构 "2"-过桥机构， "1"-非过桥机构

	std::string bid_remarks;                              // Bid方备注
	std::string remarks;                                  // 后台信息
	std::string ofr_remarks;                              // Ofr方备注
	std::string specialPartyBid;                          // Bid方特别细节
	std::string specialPartyOfr;                          // Ofr方特别细节
	std::string specialTerms;                             // 其他细节
	std::string feedback;                                 // 后台反馈

	std::string delivery_type;                            // 结算模式 DVP(券款对付)
	std::string total_price;                              // 结算金额
	std::string internally;                               // 是否内部成交, "1"-非内部成交，“2”-内部成交

	bool bid_check;                                       // Bid方是否确认
	bool ofr_check;                                       // Ofr方是否确认
    std::string deal_status;                              // 成交单状态, 0-确认，1-待提交，2-已提交，3-已通过，5-未通过，7-已毁单，13-送审中，-1-已删除
	
	std::string quote_type;                               // 报价类型 收益率/净价/全价/利差
    std::string description;
    std::string status;
	std::string send_status;
    std::string destroy_reason;                           // 毁单原因

	//------------ DCS不需要字段 -------------------
	std::string group_flag;
    std::string confirm_account;
    std::string confirm_date;
	std::string bid_check_date;
	std::string ofr_check_date;
	bool bid_trader_check;
	std::string bid_trader_check_date;
	bool ofr_trader_check;
	std::string ofr_trader_check_date;
	std::string is_print;                                 // 0-未打印 1-已打印
	std::string clear_speed;                              // 清算速度 （varchar:  T+0,T+1）
	std::string delivery_date;                            // 结算日期
	std::string traded_date;
	double bid_brokerage_rate;                            // bid佣金比例
	double ofr_brokerage_rate;                            // ofr佣金比例
	double bid_discount;                                  // bid折扣
	double ofr_discount;                                  // ofr折扣
	//------------ DCS不需要字段 -------------------
	

    // ----------------- Combination Fields ----------------------
	//------------ DCS新增Combination字段 -------------------
	bool inHand;                                          // true:正在处理中 false:正常状态
	bool backOperated;                                    // 成交单是否已经过审核操作  //new Field
	std::string checkLevel;                               // 由4个字符组成，是否需要审核（部门经理，高级经理，特级经理，合规），0为不需要，1为需要，例：1111  //new Field
	std::string currLevel;                                // 与checkLevel对应，由4个字符组成，是否审核过（部门经理，高级经理，特级经理，合规），0为未审核，1为已审核，例：1111  //new Field
	std::string specialFlag;                              // 由3个字符组成（是否特殊佣金，是否后台审核过，是否no confirmation），0为否，1为是，例：101  //new Field

	std::string deskName;
	std::string versionNo;
	std::string mbsNo;

	std::string relatedMsg;
	std::string oldContractId;

	std::string bidBrokerNameB;                           // Bid方经纪人名字 B
	std::string bidBrokerNameC;                           // Bid方经纪人名字 C
	std::string bidBrokerNameD;                           // Bid方经纪人名字 D
	std::string ofrBrokerNameB;                           // Ofr方经纪人名字 B
	std::string ofrBrokerNameC;                           // Ofr方经纪人名字 C
	std::string ofrBrokerNameD;                           // Ofr方经纪人名字 D
	//------------ DCS新增Combination字段 -------------------

    bool isValidBond;
    std::string maturity_date_type;
    int maturity_holidays; // 到期日的节假日天数

    std::string bid_institution_name;
    std::string ofr_institution_name;
    std::string bid_trader_name;
	std::string bid_trader_account;
    std::string ofr_trader_name;
	std::string ofr_trader_account;
    bool   bid_trader_is_vip;
    bool   ofr_trader_is_vip;
    bool   bid_trader_is_bad;
    bool   ofr_trader_is_bad;
    std::string broker_ids;
    std::string rate_index;
    std::string rate_type_client;
    double coupon_rate;
    std::string option_type;

    std::string bond_id;
    std::string bond_short_name;
    std::string pin_yin;
	std::string pin_yin_full;
    std::string institution_rating;
    std::string bond_category;
	std::string bond_deal_type; //BCO = 银行间的信用债 BNC = 银行间的利率债 BBE=交易所中的信用债 BBN=交易所中的利率债
    std::string bond_type;
    std::string enterprise_type;
    std::string pledged;
    std::string across_market;
    std::string time_to_maturity;
    double time_to_maturity_real;
    std::string issuer_rating_filter;
    std::string issuer_rating;
    std::string issuer_rating_npy;
    std::string bond_rating;
    std::string bond_rating_npy;
    int issuer_rating_order;
    int issuer_rating_npy_order;
    int bond_rating_order;
    int bond_rating_npy_order;
    std::string market_type;
    std::string municipal;
    std::string guarenteed;
    std::string has_option;
    std::string option_client;
    std::string on_the_run;

    std::string cdc_valuation_yield;
    std::string cdc_valuation_net_price;
	double val_modified_duration;
	double val_convexity;
	double val_basis_point_value;

	std::string guarantee_type;
	std::string issue_institution;
	std::string rating_institution;		 

	int4 listed_date;  //上市日
	int4 filter_maturity_date;
	std::string redemption_no_client;  //提前还本
    // Non-Database Related Fields
    std::string countdown_end_time;
    std::string bond_subtype;
    std::string coupon_type;
    std::string across_market_bond_ids;
  
    time_t create_time_milli_sec;
    time_t deal_time_milli_sec;
	int4 issue_year;
	/*******************行业和地区过滤，数据源来自issuer_info_cache**************************/
	//只使用code进行过滤
	std::string sw_sector;
	std::string sw_subsector;
	std::string sw_sector_code;
	std::string sw_subsector_code;
	std::string province;
	std::string province_code;
	std::string financing_platform;
	/*************************************************************************************/

    /*******************ID过滤，数据源来自issuingInstitution**************************/
    std::string issue_institution_id;
    /*************************************************************************************/

	std::string company_bond; //公司债过滤
	std::string outlook; //展望 Add by Young 20-Apr-2016
	

public:
    TYPE_DESCRIPTOR(
            (KEY(id, HASHED /*| INDEXED*/),
             KEY(company_id, 0/*HASHED | INDEXED*/),
			 FIELD(bond_key_listed_market),
			 KEY(create_time, INDEXED), 
			 FIELD(updateTime),
			 FIELD(urgent),
			 FIELD(deal_type),
			 KEY(deal_time, INDEXED),
			 FIELD(odd_numbers),
			 KEY(sequence_number, INDEXED),
			 FIELD(goods_id),
			 FIELD(bond_key),
			 FIELD(listed_market),
			 FIELD(goods_code),
			 FIELD(goods_short_name),
			 FIELD(maturity_date),
			 FIELD(price),
			 FIELD(return_point),
			 FIELD(rebate),
			 FIELD(volume),
			 FIELD(currency),
			 FIELD(exercise),
			 FIELD(time_style),
			 FIELD(settlement_date),
			 FIELD(yield),
			 FIELD(spread),
			 FIELD(net_price),
			 FIELD(full_price),
             FIELD(bid_bank_id),
             FIELD(bid_bank_code),
             FIELD(bid_bank_city),
             FIELD(bid_agent_id),
             FIELD(bid_agent_code),
			 FIELD(bidBankAgentName),
             FIELD(bid_broker_id),
			 FIELD(bidBrokerIdB),
			 FIELD(bidBrokerIdC),
			 FIELD(bidBrokerIdD),
             FIELD(bid_broker_name),
			 FIELD(bidBrokerCodeA),
			 FIELD(bidBrokerCodeB),
			 FIELD(bidBrokerCodeC),
			 FIELD(bidBrokerCodeD),
			 FIELD(bidPercentA),
			 FIELD(bidPercentB),
			 FIELD(bidPercentC),
			 FIELD(bidPercentD),
			 FIELD(bid_charge),
			 FIELD(bidIsNoBrokerage),
             FIELD(bid_brokerrage),
			 FIELD(bidBrokerrageRemark),
			 FIELD(bidTradeMode),
			 FIELD(bidNoConfirm),
			 FIELD(bidNoComments),
			 FIELD(bid_bridge),
             FIELD(ofr_bank_id),
             FIELD(ofr_bank_code),
             FIELD(ofr_bank_city),
             FIELD(ofr_agent_id),
             FIELD(ofr_agent_code),
			 FIELD(ofrBankAgentName),
             FIELD(ofr_broker_id),
			 FIELD(ofrBrokerIdB),
			 FIELD(ofrBrokerIdC),
			 FIELD(ofrBrokerIdD),
			 FIELD(ofr_broker_name),
			 FIELD(ofrBrokerCodeA),
			 FIELD(ofrBrokerCodeB),
			 FIELD(ofrBrokerCodeC),
			 FIELD(ofrBrokerCodeD),
			 FIELD(ofrPercentA),
			 FIELD(ofrPercentB),
			 FIELD(ofrPercentC),
			 FIELD(ofrPercentD),
			 FIELD(ofr_charge),
			 FIELD(ofrIsNoBrokerage),
             FIELD(ofr_brokerrage),
			 FIELD(ofrBrokerrageRemark),
			 FIELD(ofrTradeMode),
			 FIELD(ofrNoConfirm),
			 FIELD(ofrNoComments),
			 FIELD(ofr_bridge),
			 FIELD(bid_remarks),
			 FIELD(remarks),
			 FIELD(ofr_remarks),
			 FIELD(specialPartyBid),
			 FIELD(specialPartyOfr),
			 FIELD(specialTerms),
			 FIELD(feedback),
			 FIELD(delivery_type),
			 FIELD(total_price),
			 FIELD(internally),
			 FIELD(bid_check),
             FIELD(ofr_check),
			 FIELD(deal_status),
			 FIELD(quote_type),
             FIELD(description),
             FIELD(status),
			 FIELD(send_status),
             FIELD(destroy_reason),
             FIELD(group_flag),
             FIELD(confirm_account),
             FIELD(confirm_date),
			 FIELD(bid_check_date),
			 FIELD(ofr_check_date),
			 FIELD(bid_trader_check),
			 FIELD(bid_trader_check_date),
			 FIELD(ofr_trader_check),
			 FIELD(ofr_trader_check_date),
			 FIELD(is_print),
			 FIELD(clear_speed),
			 FIELD(delivery_date),
			 FIELD(traded_date),
             FIELD(bid_brokerage_rate),
             FIELD(ofr_brokerage_rate),
             FIELD(bid_discount),
             FIELD(ofr_discount),


			 COMBINED(inHand), 
			 COMBINED(questionMark),
			 COMBINED(backOperated),
			 COMBINED(checkLevel),
			 COMBINED(currLevel),
			 COMBINED(specialFlag),
			 COMBINED(deskName),
			 COMBINED(versionNo),
			 COMBINED(mbsNo),
			 COMBINED(relatedMsg),
			 COMBINED(oldContractId),
			 COMBINED(bidBrokerNameB),
			 COMBINED(bidBrokerNameC),
			 COMBINED(bidBrokerNameD),
			 COMBINED(ofrBrokerNameB),
			 COMBINED(ofrBrokerNameC),
			 COMBINED(ofrBrokerNameD),
			 COMBINED(listed_date),
			 COMBINED(filter_maturity_date),
			 COMBINED(redemption_no_client), 
             COMBINED(bid_institution_name),
             COMBINED(ofr_institution_name),
             COMBINED(bid_trader_name),
			 COMBINED(bid_trader_account),
             COMBINED(ofr_trader_name),
			 COMBINED(ofr_trader_account),
             COMBINED(bid_trader_is_vip),
             COMBINED(ofr_trader_is_vip),
             COMBINED(bid_trader_is_bad),
             COMBINED(ofr_trader_is_bad),
             COMBINED(broker_ids),
             COMBINED(rate_index),
             COMBINED(rate_type_client),
             COMBINED(coupon_rate),
             COMBINED(option_type),
             COMBINED(bond_id),
             COMBINED(bond_short_name),
			 COMBINED(pin_yin),
			 COMBINED(pin_yin_full),
             COMBINED(institution_rating),
             COMBINED(bond_category),
			 COMBINED(bond_deal_type),
             COMBINED(bond_type),
             COMBINED(enterprise_type),
             COMBINED(pledged),
             COMBINED(across_market),
             COMBINED(time_to_maturity),
             COMBINED(time_to_maturity_real),
             COMBINED(issuer_rating_filter),
             COMBINED(issuer_rating),
             COMBINED(issuer_rating_npy),
             COMBINED(bond_rating),
             COMBINED(bond_rating_npy),
             COMBINED(issuer_rating_order),
             COMBINED(issuer_rating_npy_order),
             COMBINED(bond_rating_order),
             COMBINED(bond_rating_npy_order),
             COMBINED(market_type),
             COMBINED(municipal),
             COMBINED(guarenteed),
             COMBINED(has_option),
             COMBINED(option_client),
             COMBINED(on_the_run),
             COMBINED(cdc_valuation_yield),
             COMBINED(cdc_valuation_net_price),
			 COMBINED(val_modified_duration),
			 COMBINED(val_convexity),
			 COMBINED(val_basis_point_value),
			 COMBINED(guarantee_type),
			 COMBINED(issue_institution),
			 COMBINED(rating_institution),
             COMBINED(bond_subtype),
             COMBINED(coupon_type),
             COMBINED(across_market_bond_ids),
             COMBINED(create_time_milli_sec),
             COMBINED(deal_time_milli_sec),
			 COMBINED(issue_year),
			 COMBINED(sw_sector),
			 COMBINED(sw_sector_code),
			 COMBINED(sw_subsector),
			 COMBINED(sw_subsector_code),
             COMBINED(province),
             COMBINED(province_code),
			 COMBINED(financing_platform),
             COMBINED(issue_institution_id),
             COMBINED(maturity_date_type),
             COMBINED(maturity_holidays),
			 COMBINED(company_bond),
			 COMBINED(outlook)
            )
    );

};

#endif /* DCSBONDDEAL_CACHE_H_ */
