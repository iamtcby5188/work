
#ifndef _BOND_DEAL_H_
#define _BOND_DEAL_H_

#include "sdbus/string.h"
#include "include/common.h"
#include "include/constants.h"
#include "field.h"
#include <string>

/*typedef struct
{
    Field<std::string> id_;						    // �ɽ�ID
    Field<std::string> company_id;
    Field<std::string> bond_key_listed_market_;     // ��ֵ���г�Ψһ��ʾһ��ծȯ

    Field<std::string> direction_;                  // ��������, GVN/TKN��TRD
    Field<std::string> create_time_;                // ����ʱ�� DateTime
    Field<std::string> deal_time_;                  // �ɽ�ʱ��, �ɽ�¼��ʱ�ɱ༭,DateTime
    Field<std::string> deal_no_;                    // �ɽ�����
   
    Field<double> price_;                           // ծȯ������    
	Field<double> volume_;                             // ������/�ɽ���/ȯ���ܶ�
	Field<std::string> currency_;                   // ����, USD/CNY��
	Field<std::string> traded_date_;                // ������
	Field<std::string> yield_type_;                 // ����������0-��Ȩ�����ʣ�1-����������
	Field<std::string> settlement_type_;            // ���㷽ʽ��T+0,T+1
	Field<std::string> settlement_date_;            // ������

	Field<double> yield_;                           // ������
	Field<double> spread_;                          // ����
	Field<double> clean_price_;                     // ����
	Field<double> dirty_price_;                     // ȫ��

	Field<std::string> bid_institution_id_;         // Bid������ID
	Field<std::string> bid_inst_city_;				// Bid������
	Field<std::string> bid_trader_id_;              // Bid������ԱID
	Field<std::string> bid_trader_account_;         // Bid������Ա�˻���
	Field<std::string> bid_broker_id_;              // Bid��������ID 
	Field<std::string> bid_broker_id_B_; 
	Field<std::string> bid_broker_id_C_;
	Field<std::string> bid_broker_id_D_;
	Field<std::string> bid_percent_A_;              // Bid��Ӷ�����
	Field<std::string> bid_percent_B_;
	Field<std::string> bid_percent_C_;
	Field<std::string> bid_percent_D_;
	Field<std::string> bid_is_charged_;             // Bid���Ƿ��շ� "2"-�շѣ�"1"-���շ�
	Field<std::string> bid_is_no_brokerage_;        // Bid���Ƿ���Ӷ "2"-��Ӷ��"1"-����Ӷ
	Field<double> bid_brokerage_;                   // Bid��Ӷ��
	Field<std::string> bid_brokerage_remark_;       // Bid��Ӷ��ע
	Field<std::string> bid_trade_mode_;             // Bid�����׷�ʽ
	Field<std::string> bid_no_confirm_;             // Bid��NC "1"-NC��"0"-��NC
	Field<std::string> bid_no_comments_;            // Bid��NC��ע
	Field<std::string> bid_is_bridge_;              // Bid���Ƿ�Ϊ���Ż��� "2"-���Ż�����"1"-�ǹ��Ż���

	Field<std::string> ofr_institution_id_;         // Ofr������ID
	Field<std::string> ofr_inst_city_;				// Ofr������
	Field<std::string> ofr_trader_id_;              // Ofr������ԱID
	Field<std::string> ofr_trader_account_;         // Ofr������Ա�˻���
	Field<std::string> ofr_broker_id_;              // Ofr��������ID
	Field<std::string> ofr_broker_id_B_;
	Field<std::string> ofr_broker_id_C_;
	Field<std::string> ofr_broker_id_D_;
	Field<std::string> ofr_percent_A_;              // Ofr��Ӷ�����
	Field<std::string> ofr_percent_B_;
	Field<std::string> ofr_percent_C_;
	Field<std::string> ofr_percent_D_;
	Field<std::string> ofr_is_charged_;             // Ofr���Ƿ��շ�. ���ݿ���Ϊstring�ͣ�1,NULL
	Field<std::string> ofr_is_no_brokerage_;        // ofr���Ƿ���Ӷ
	Field<double> ofr_brokerage_;                   // Ofr��Ӷ��
	Field<std::string> ofr_brokerage_remark_;       // Ofr��Ӷ��ע
	Field<std::string> ofr_trade_mode_;             // Ofr�����׷�ʽ
	Field<std::string> ofr_no_confirm_;             // Ofr��NC "1"-NC��"0"-��NC
	Field<std::string> ofr_no_comments_;            // Ofr��NC��ע
	Field<std::string> ofr_is_bridge_;              // Ofr���Ƿ�Ϊ���Ż���, ���ݿ���Ϊstring�ͣ�1,NULL

	Field<std::string> bid_remarks_;                // Bid����ע
	Field<std::string> remarks_;                    // ��̨��Ϣ
	Field<std::string> ofr_remarks_;                // Ofr����ע
	Field<std::string> special_party_bid_;          // bid���ر�ϸ��
	Field<std::string> special_party_ofr_;           // ofr���ر�ϸ��
	Field<std::string> special_terms_;               // ����ϸ��
	Field<std::string> feedback_;                   // ��̨����

	Field<std::string> settlement_mode_;            // ����ģʽ����DVP.
	Field<std::string> settlement_amount_;          // ������.
	Field<std::string> deal_internal_flag_;         // �ڲ��ɽ�, "1"-���ڲ��ɽ�����2��-�ڲ��ɽ�

	Field<bool> bid_checked_;                       // Bid���������Ƿ��Ѿ�ȷ��
	Field<bool> ofr_checked_;                       // Ofr���������Ƿ��Ѿ�ȷ��
	Field<std::string> deal_status_;                // �ɽ���״̬, 0-ȷ�ϣ�1-���ύ��2-���ύ��3-��ͨ����5-δͨ����7-�ѻٵ���13-�����У�-1-��ɾ��
	Field<bool> in_hand_;                           //true:���ڴ����� false:����״̬
	Field<std::string> urgent_status_;              //true:�Ӽ� false:�ǼӼ�
	Field<std::string> question_mark_;              //true:�����ʺű�� false:�������ʺű��

    Field<double> fan_dian_;                        // ������ֵ, ���緵0.12  
    Field<std::string> fan_dian_flag_;              // �Ƿ񷵵� 0, 1
    Field<std::string> quote_type_;                 // ��������: ������/����/ȫ��/����
    Field<std::string> description_;                // ����
    Field<std::string> delivery_date_;              // ��������, DateTime
	Field<double> bid_brokerage_rate_;              // Bid��Ӷ�����
	Field<double> bid_discount_;                    // Bid���ۿ�
	Field<double> ofr_brokerage_rate_;              // Ofr��Ӷ�����
	Field<double> ofr_discount_;                    // Ofr���ۿ�
    Field<int> quote_side_;                         // �óɽ�����Ӧ����bid����Ofr��-1-ofr��1-bid
    Field<std::string> quote_id_;                   // �ɽ�����Ӧ��quote id   

	Field<std::string> related_msg_;                // �༭���Ų�ʱ���ϳɽ�����ID   
	Field<std::string> old_contract_id_;             // ���Ź����ֶ�  

} BondDeal;*/

class BondDeal {
public:
    BondDeal() : price_(DECIMAL_NULL), volume_(0), yield_(DECIMAL_NULL), spread_(DECIMAL_NULL), clean_price_(DECIMAL_NULL), dirty_price_(DECIMAL_NULL), 
        bid_brokerage_(DECIMAL_NULL), ofr_brokerage_(DECIMAL_NULL), bid_checked_(false), ofr_checked_(false), in_hand_(false), fan_dian_(false), 
        bid_brokerage_rate_(DECIMAL_NULL), bid_discount_(DECIMAL_NULL), ofr_brokerage_rate_(DECIMAL_NULL), ofr_discount_(false), quote_side_(0){}
    Field<std::string> id_;						    // �ɽ�ID
    Field<std::string> company_id;
    Field<std::string> bond_key_listed_market_;     // ��ֵ���г�Ψһ��ʾһ��ծȯ

    Field<std::string> direction_;                  // ��������, GVN/TKN��TRD
    Field<std::string> create_time_;                // ����ʱ�� DateTime
    Field<std::string> deal_time_;                  // �ɽ�ʱ��, �ɽ�¼��ʱ�ɱ༭,DateTime
    Field<std::string> deal_no_;                    // �ɽ�����

    Field<double> price_;                           // ծȯ������    
    Field<double> volume_;                             // ������/�ɽ���/ȯ���ܶ�
    Field<std::string> currency_;                   // ����, USD/CNY��
    Field<std::string> traded_date_;                // ������
    Field<std::string> yield_type_;                 // ����������0-��Ȩ�����ʣ�1-����������
    Field<std::string> settlement_type_;            // ���㷽ʽ��T+0,T+1
    Field<std::string> settlement_date_;            // ������

    Field<double> yield_;                           // ������
    Field<double> spread_;                          // ����
    Field<double> clean_price_;                     // ����
    Field<double> dirty_price_;                     // ȫ��

    Field<std::string> bid_institution_id_;         // Bid������ID
    Field<std::string> bid_inst_city_;				// Bid������
    Field<std::string> bid_trader_id_;              // Bid������ԱID
    Field<std::string> bid_trader_account_;         // Bid������Ա�˻���
    Field<std::string> bid_broker_id_;              // Bid��������ID 
    Field<std::string> bid_broker_id_B_;
    Field<std::string> bid_broker_id_C_;
    Field<std::string> bid_broker_id_D_;
    Field<std::string> bid_percent_A_;              // Bid��Ӷ�����
    Field<std::string> bid_percent_B_;
    Field<std::string> bid_percent_C_;
    Field<std::string> bid_percent_D_;
    Field<std::string> bid_is_charged_;             // Bid���Ƿ��շ� "2"-�շѣ�"1"-���շ�
    Field<std::string> bid_is_no_brokerage_;        // Bid���Ƿ���Ӷ "2"-��Ӷ��"1"-����Ӷ
    Field<double> bid_brokerage_;                   // Bid��Ӷ��
    Field<std::string> bid_brokerage_remark_;       // Bid��Ӷ��ע
    Field<std::string> bid_trade_mode_;             // Bid�����׷�ʽ
    Field<std::string> bid_no_confirm_;             // Bid��NC "1"-NC��"0"-��NC
    Field<std::string> bid_no_comments_;            // Bid��NC��ע
    Field<std::string> bid_is_bridge_;              // Bid���Ƿ�Ϊ���Ż��� "2"-���Ż�����"1"-�ǹ��Ż���

    Field<std::string> ofr_institution_id_;         // Ofr������ID
    Field<std::string> ofr_inst_city_;				// Ofr������
    Field<std::string> ofr_trader_id_;              // Ofr������ԱID
    Field<std::string> ofr_trader_account_;         // Ofr������Ա�˻���
    Field<std::string> ofr_broker_id_;              // Ofr��������ID
    Field<std::string> ofr_broker_id_B_;
    Field<std::string> ofr_broker_id_C_;
    Field<std::string> ofr_broker_id_D_;
    Field<std::string> ofr_percent_A_;              // Ofr��Ӷ�����
    Field<std::string> ofr_percent_B_;
    Field<std::string> ofr_percent_C_;
    Field<std::string> ofr_percent_D_;
    Field<std::string> ofr_is_charged_;             // Ofr���Ƿ��շ�. ���ݿ���Ϊstring�ͣ�1,NULL
    Field<std::string> ofr_is_no_brokerage_;        // ofr���Ƿ���Ӷ
    Field<double> ofr_brokerage_;                   // Ofr��Ӷ��
    Field<std::string> ofr_brokerage_remark_;       // Ofr��Ӷ��ע
    Field<std::string> ofr_trade_mode_;             // Ofr�����׷�ʽ
    Field<std::string> ofr_no_confirm_;             // Ofr��NC "1"-NC��"0"-��NC
    Field<std::string> ofr_no_comments_;            // Ofr��NC��ע
    Field<std::string> ofr_is_bridge_;              // Ofr���Ƿ�Ϊ���Ż���, ���ݿ���Ϊstring�ͣ�1,NULL

    Field<std::string> bid_remarks_;                // Bid����ע
    Field<std::string> remarks_;                    // ��̨��Ϣ
    Field<std::string> ofr_remarks_;                // Ofr����ע
    Field<std::string> special_party_bid_;          // bid���ر�ϸ��
    Field<std::string> special_party_ofr_;           // ofr���ر�ϸ��
    Field<std::string> special_terms_;               // ����ϸ��
    Field<std::string> feedback_;                   // ��̨����

    Field<std::string> settlement_mode_;            // ����ģʽ����DVP.
    Field<std::string> settlement_amount_;          // ������.
    Field<std::string> deal_internal_flag_;         // �ڲ��ɽ�, "1"-���ڲ��ɽ�����2��-�ڲ��ɽ�

    Field<bool> bid_checked_;                       // Bid���������Ƿ��Ѿ�ȷ��
    Field<bool> ofr_checked_;                       // Ofr���������Ƿ��Ѿ�ȷ��
    Field<std::string> deal_status_;                // �ɽ���״̬, 0-ȷ�ϣ�1-���ύ��2-���ύ��3-��ͨ����5-δͨ����7-�ѻٵ���13-�����У�-1-��ɾ��
    Field<bool> in_hand_;                           //true:���ڴ����� false:����״̬
    Field<std::string> urgent_status_;              //true:�Ӽ� false:�ǼӼ�
    Field<std::string> question_mark_;              //true:�����ʺű�� false:�������ʺű��

    Field<double> fan_dian_;                        // ������ֵ, ���緵0.12  
    Field<std::string> fan_dian_flag_;              // �Ƿ񷵵� 0, 1
    Field<std::string> quote_type_;                 // ��������: ������/����/ȫ��/����
    Field<std::string> description_;                // ����
    Field<std::string> delivery_date_;              // ��������, DateTime
    Field<double> bid_brokerage_rate_;              // Bid��Ӷ�����
    Field<double> bid_discount_;                    // Bid���ۿ�
    Field<double> ofr_brokerage_rate_;              // Ofr��Ӷ�����
    Field<double> ofr_discount_;                    // Ofr���ۿ�
    Field<int> quote_side_;                         // �óɽ�����Ӧ����bid����Ofr��-1-ofr��1-bid
    Field<std::string> quote_id_;                   // �ɽ�����Ӧ��quote id   

    Field<std::string> related_msg_;                // �༭���Ų�ʱ���ϳɽ�����ID   
    Field<std::string> old_contract_id_;             // ���Ź����ֶ�  
};

typedef std::tr1::shared_ptr<BondDeal> BondDealPtr;
typedef std::tr1::shared_ptr<std::vector<BondDealPtr> > BondDealVecPtr;

#endif // _BOND_DEAL_H_
