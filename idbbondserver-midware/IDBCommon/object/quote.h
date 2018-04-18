/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Quote Structure
====================================================================================================================
   Date       Name                                  Description of Change
12-Jun-2014   Guoying Feng                          Initial version
25-Aug-2014   James Xu                              Add price_string and fan_dian_string
$HISTORY$
===================================================================================================================*/
#ifndef _QUOTE_H_
#define _QUOTE_H_

#include "sdbus/string.h"
#include "field.h"
#include "include/common.h"
#include <string>

class BondQuote
{
public:
    BondQuote() : quote_side_(0), price_(INVALIDVALUE), volume_(0), fan_dian_(INVALIDVALUE) {}

public:
    Field<std::string> id_;                         // ����ID
	Field<std::string> company_id_;
    Field<std::string> bond_key_listed_market_;    
    Field<std::string> institution_id_;             // ����ID
    Field<std::string> trader_id_;                  // ����ԱID
    Field<std::string> broker_id_;
    
    Field<int> quote_side_;                         // ���۷���bid/ofr
    Field<double> price_;                           // ���ۣ�bid or ofr
    Field<double> volume_;
    Field<std::string> description_;                // ���۱�ע/����

    Field<std::string> institution_name_;             // ��������
    Field<std::string> trader_name_;                  // ������Ա����
    Field<std::string> broker_name_;                  // ����������
    
    Field<std::string> fan_dian_flag_;                // �Ƿ񷵵㣬 ��1��- �з��㣬��0��- �޷���
    Field<double> fan_dian_;                          // ������ֵ, ���緵0.12
    Field<std::string> bargain_flag_;                 // ��۱�ʶ, ȡֵ 0, 1, 2
    Field<std::string> oco_flag_;                     // OCO��ʶ, 0-������ʾ��1-��ʾoco  2-oco���
    Field<std::string> internal_flag_;                // 1-�������ۣ�2-�ڲ�����

    Field<std::string> quote_type_;                   // �۸�����: ������/����/ȫ��/����
    Field<std::string> delivery_date_;                // ������
	Field<std::string> yield_type_;                   // ��������         
};

typedef std::tr1::shared_ptr<BondQuote> BondQuotePtr;

typedef struct  
{
    sdbus::string institution_id_; //bank_id_
    sdbus::string institution_name_;//bank_code_;
    //sdbus::string bank_name_;
    sdbus::string pin_yin_;
    sdbus::string pin_yin_full_;
    //sdbus::string bank_name_full_;
    //sdbus::string attribute4_;
} BankInfo;

typedef std::tr1::shared_ptr<BankInfo> BankInfoPtr;

typedef struct 
{
    sdbus::string institution_id_;//bank_id_;
    sdbus::string trader_code_;
    //sdbus::string f_code_;
    //sdbus::string code_;
    //sdbus::string id_;
    sdbus::string vip_flag_;
    sdbus::string bad_flag_;
    //sdbus::string customer_id_;
}AgentInfo;

typedef std::tr1::shared_ptr<AgentInfo> AgentInfoPtr;

typedef struct 
{
    sdbus::string bond_key_;
    sdbus::string selective_content_;
    sdbus::string selective_code_;
    sdbus::string coupon_type_;
}BondRateType;

typedef std::tr1::shared_ptr<BondRateType> BondRateTypePtr;

#endif // _QUOTE_H_
