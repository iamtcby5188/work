
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
	std::string bond_key_listed_market;                   // ��ֵ���г�Ψһ��ʾһ��ծȯ
	std::string create_time;                              // �ɽ�ʱ��
	std::string updateTime;                               // ����ʱ��
	std::string urgent;                                   // �Ӽ���־ "true"-�Ӽ���"false"-�ǼӼ�
	std::string questionMark;                             // �ʺű�� "true"-������"false"-������

	std::string deal_type;                                // ��������, GVN/TKN/TRD
    std::string deal_time;                                // ����ʱ��
	std::string odd_numbers;                              // ����
	int sequence_number;                                  // ���к�

	std::string goods_id;
	std::string goods_code;
	std::string goods_short_name;
	std::string bond_key;
	std::string listed_market;
	std::string maturity_date;                            // ծȯ������

	real8 price;                                          // �۸�
	real8 return_point;                                   // ������ֵ, ���緵0.12  
	std::string rebate;                                   // �Ƿ񷵵� 0, 1
	real8 volume;                                          // ���� ȯ���ܶ�
	std::string currency;                                 // �������� CNY
	std::string exercise;                                 // ��Ȩ���� 0-��Ȩ 1-����
	std::string time_style;                               // ���㷽ʽ T+0 T+1
	std::string settlement_date;                          // ������

	real8 yield;                                          // ������
	real8 spread;                                         // ����
	real8 net_price;                                      // ����
	real8 full_price;                                     // ȫ��
	
    std::string bid_bank_id;                              // Bid������id
    std::string bid_bank_code;                            // Bid����������
    std::string bid_bank_city;                            // Bid���������ڵ�
    std::string bid_agent_id;                             // Bid������Աid
    std::string bid_agent_code;                           // Bid������Ա����
	std::string bidBankAgentName;                         // Bid������Ա����
    std::string bid_broker_id;                            // Bid��������id A
	std::string bidBrokerIdB;                             // Bid��������id B
	std::string bidBrokerIdC;                             // Bid��������id C
	std::string bidBrokerIdD;                             // Bid��������id D
    std::string bid_broker_name;                          // Bid������������ A
	std::string bidBrokerCodeA;                           // Bid�������˴��� A
	std::string bidBrokerCodeB;                           // Bid�������˴��� B
	std::string bidBrokerCodeC;                           // Bid�������˴��� C
	std::string bidBrokerCodeD;                           // Bid�������˴��� D
	std::string bidPercentA;                              // Bid�������˱��� A
	std::string bidPercentB;                              // Bid�������˱��� B
	std::string bidPercentC;                              // Bid�������˱��� C
	std::string bidPercentD;                              // Bid�������˱��� D
	std::string bid_charge;                               // Bid���Ƿ��շ� "2"-�շѣ�"1"-���շ�
	std::string bidIsNoBrokerage;                         // Bid���Ƿ���Ӷ "2"-��Ӷ��"1"-����Ӷ
    real8 bid_brokerrage;                                 // Bid��Ӷ��
	std::string bidBrokerrageRemark;                      // Bid��Ӷ��ע
	std::string bidTradeMode;                             // Bid�����׷�ʽ
	std::string bidNoConfirm;                             // Bid��NC "1"-NC��"0"-��NC
	std::string bidNoComments;                            // Bid��NC��ע
	std::string bid_bridge;                               // Bid���Ƿ�Ϊ���Ż��� "2"-���Ż����� "1"-�ǹ��Ż���

    std::string ofr_bank_id;                              // Ofr������id
	std::string ofr_bank_code;                            // Ofr����������
	std::string ofr_bank_city;                            // Ofr���������ڵ�
	std::string ofr_agent_id;                             // Ofr������Աid
	std::string ofr_agent_code;                           // Ofr������Ա����
	std::string ofrBankAgentName;                         // Ofr������Ա����
	std::string ofr_broker_id;                            // Ofr��������id A
	std::string ofrBrokerIdB;                             // Ofr��������id B
	std::string ofrBrokerIdC;                             // Ofr��������id C
	std::string ofrBrokerIdD;                             // Ofr��������id D
	std::string ofr_broker_name;                          // Ofr������������ A
	std::string ofrBrokerCodeA;                           // Ofr�������˴��� A
	std::string ofrBrokerCodeB;                           // Ofr�������˴��� B
	std::string ofrBrokerCodeC;                           // Ofr�������˴��� C
	std::string ofrBrokerCodeD;                           // Ofr�������˴��� D
	std::string ofrPercentA;                              // Ofr�������˱��� A
	std::string ofrPercentB;                              // Ofr�������˱��� B
	std::string ofrPercentC;                              // Ofr�������˱��� C
	std::string ofrPercentD;                              // Ofr�������˱��� D
	std::string ofr_charge;                               // Ofr���Ƿ��շ� "2"-�շѣ�"1"-���շ�
	std::string ofrIsNoBrokerage;                         // Ofr���Ƿ���Ӷ "2"-��Ӷ��"1"-����Ӷ
    real8 ofr_brokerrage;                                 // Ofr��Ӷ��
	std::string ofrBrokerrageRemark;                      // Ofr��Ӷ��ע
	std::string ofrTradeMode;                             // ofr�����׷�ʽ
	std::string ofrNoConfirm;                             // ofr��NC "1"-NC��"0"-��NC
	std::string ofrNoComments;                            // ofr��NC��ע 
	std::string ofr_bridge;                               // Ofr���Ƿ�Ϊ���Ż��� "2"-���Ż����� "1"-�ǹ��Ż���

	std::string bid_remarks;                              // Bid����ע
	std::string remarks;                                  // ��̨��Ϣ
	std::string ofr_remarks;                              // Ofr����ע
	std::string specialPartyBid;                          // Bid���ر�ϸ��
	std::string specialPartyOfr;                          // Ofr���ر�ϸ��
	std::string specialTerms;                             // ����ϸ��
	std::string feedback;                                 // ��̨����

	std::string delivery_type;                            // ����ģʽ DVP(ȯ��Ը�)
	std::string total_price;                              // ������
	std::string internally;                               // �Ƿ��ڲ��ɽ�, "1"-���ڲ��ɽ�����2��-�ڲ��ɽ�

	bool bid_check;                                       // Bid���Ƿ�ȷ��
	bool ofr_check;                                       // Ofr���Ƿ�ȷ��
    std::string deal_status;                              // �ɽ���״̬, 0-ȷ�ϣ�1-���ύ��2-���ύ��3-��ͨ����5-δͨ����7-�ѻٵ���13-�����У�-1-��ɾ��
	
	std::string quote_type;                               // �������� ������/����/ȫ��/����
    std::string description;
    std::string status;
	std::string send_status;
    std::string destroy_reason;                           // �ٵ�ԭ��

	//------------ DCS����Ҫ�ֶ� -------------------
	std::string group_flag;
    std::string confirm_account;
    std::string confirm_date;
	std::string bid_check_date;
	std::string ofr_check_date;
	bool bid_trader_check;
	std::string bid_trader_check_date;
	bool ofr_trader_check;
	std::string ofr_trader_check_date;
	std::string is_print;                                 // 0-δ��ӡ 1-�Ѵ�ӡ
	std::string clear_speed;                              // �����ٶ� ��varchar:  T+0,T+1��
	std::string delivery_date;                            // ��������
	std::string traded_date;
	double bid_brokerage_rate;                            // bidӶ�����
	double ofr_brokerage_rate;                            // ofrӶ�����
	double bid_discount;                                  // bid�ۿ�
	double ofr_discount;                                  // ofr�ۿ�
	//------------ DCS����Ҫ�ֶ� -------------------
	

    // ----------------- Combination Fields ----------------------
	//------------ DCS����Combination�ֶ� -------------------
	bool inHand;                                          // true:���ڴ����� false:����״̬
	bool backOperated;                                    // �ɽ����Ƿ��Ѿ�����˲���  //new Field
	std::string checkLevel;                               // ��4���ַ���ɣ��Ƿ���Ҫ��ˣ����ž����߼������ؼ������Ϲ棩��0Ϊ����Ҫ��1Ϊ��Ҫ������1111  //new Field
	std::string currLevel;                                // ��checkLevel��Ӧ����4���ַ���ɣ��Ƿ���˹������ž����߼������ؼ������Ϲ棩��0Ϊδ��ˣ�1Ϊ����ˣ�����1111  //new Field
	std::string specialFlag;                              // ��3���ַ���ɣ��Ƿ�����Ӷ���Ƿ��̨��˹����Ƿ�no confirmation����0Ϊ��1Ϊ�ǣ�����101  //new Field

	std::string deskName;
	std::string versionNo;
	std::string mbsNo;

	std::string relatedMsg;
	std::string oldContractId;

	std::string bidBrokerNameB;                           // Bid������������ B
	std::string bidBrokerNameC;                           // Bid������������ C
	std::string bidBrokerNameD;                           // Bid������������ D
	std::string ofrBrokerNameB;                           // Ofr������������ B
	std::string ofrBrokerNameC;                           // Ofr������������ C
	std::string ofrBrokerNameD;                           // Ofr������������ D
	//------------ DCS����Combination�ֶ� -------------------

    bool isValidBond;
    std::string maturity_date_type;
    int maturity_holidays; // �����յĽڼ�������

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
	std::string bond_deal_type; //BCO = ���м������ծ BNC = ���м������ծ BBE=�������е�����ծ BBN=�������е�����ծ
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

	int4 listed_date;  //������
	int4 filter_maturity_date;
	std::string redemption_no_client;  //��ǰ����
    // Non-Database Related Fields
    std::string countdown_end_time;
    std::string bond_subtype;
    std::string coupon_type;
    std::string across_market_bond_ids;
  
    time_t create_time_milli_sec;
    time_t deal_time_milli_sec;
	int4 issue_year;
	/*******************��ҵ�͵������ˣ�����Դ����issuer_info_cache**************************/
	//ֻʹ��code���й���
	std::string sw_sector;
	std::string sw_subsector;
	std::string sw_sector_code;
	std::string sw_subsector_code;
	std::string province;
	std::string province_code;
	std::string financing_platform;
	/*************************************************************************************/

    /*******************ID���ˣ�����Դ����issuingInstitution**************************/
    std::string issue_institution_id;
    /*************************************************************************************/

	std::string company_bond; //��˾ծ����
	std::string outlook; //չ�� Add by Young 20-Apr-2016
	

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
