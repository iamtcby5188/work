/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    FID definitions
====================================================================================================================
   Date       Name                                  Description of Change
04-Jun-2014   Huameng Jia                           Initial version
05-Jun-2014   James Xu                              Add FID definitions for Public and Bond Deal
12-Jun-2014   James Xu                              Add FID definitions for Quote
17-Jun-2014   James Xu                              Add FID field name strings
20-Jun-2014   James Xu                              Add FID definitions related to caches.
25-Aug-2014   James Xu                              Add FID_BID/OFR_PRICE_STRING and FID_BID/OFR_FAN_DIAN_STRING
25-Sep-2014   James Xu                              Add FID_BID_OFR_SPREAD and FID_CDC_OFFSET
09-Apr-2015   James Xu                              Add FID_YIELD_TO_EXECUTION
$HISTORY$
===================================================================================================================*/
#ifndef _FID_H_
#define _FID_H_

//----------------------  Account  -----------------------------
const int FID_ACCOUNT_BASE                    = 1000;
const int FID_ACCOUNT_ID                      = FID_ACCOUNT_BASE + 0;      // "AccountID"
const int FID_ACCOUNT_NAME                    = FID_ACCOUNT_BASE + 1;      // "AccountName"
const int FID_ACCOUNT_PASSWORD                = FID_ACCOUNT_BASE + 2;      // "AccountPassword"
const int FID_ACCOUNT_COMPANY_ID              = FID_ACCOUNT_BASE + 3;      // "AccountCompanyID"
const int FID_ACCOUNT_TOKEN                   = FID_ACCOUNT_BASE + 4;      // "AccountToken"
const int FID_ACCOUNT_DEPARTMENT              = FID_ACCOUNT_BASE + 5;      // "AccountDepartment": BCO - ���ã�BNC - ����
const int FID_ACCOUNT_COMPANY_NAME            = FID_ACCOUNT_BASE + 6;      // "AccountCompanyName"
const int FID_ACCOUNT_ROLE                    = FID_ACCOUNT_BASE + 7;      // "AccountRole": ���磺BCO-DI, BNC-BROKER
const int FID_ACCOUNT_PERMISSION              = FID_ACCOUNT_BASE + 8;      // "AccountPermission" �û�ӵ�е�����Ȩ�ޣ���','Ϊ���
const int FID_ACCOUNT_CODE                    = FID_ACCOUNT_BASE + 9;      // "AccountCode"
const int FID_ACCOUNT_TELEPHONE               = FID_ACCOUNT_BASE + 10;     // "AccountTelephone"
const int FID_ACCOUNT_PHONE		           = FID_ACCOUNT_BASE + 11;     // "AccountPhone"
const int FID_ACCOUNT_EMAIL				   = FID_ACCOUNT_BASE + 12;     // "AccountEmail"
const int FID_ACCOUNT_RM		               = FID_ACCOUNT_BASE + 13;     // "AccountRM"
const int FID_ACCOUNT_MSN		               = FID_ACCOUNT_BASE + 14;     // "AccountMSN"
const int FID_ACCOUNT_QQ		               = FID_ACCOUNT_BASE + 15;     // "AccountQQ"
const int FID_ACCOUNT_ADDRESS	               = FID_ACCOUNT_BASE + 16;     // "AccountAddress"
const int FID_ACCOUNT_PASSWORD_ORG            = FID_ACCOUNT_BASE + 17;      // "Original AccountPassword"

//----------------------  Login Info ----------------------------- 
const int FID_LOGIN_BASE                      = 1100;
const int FID_LOGIN_RET_CODE                  = FID_LOGIN_BASE + 1;         // "LoginRetCode":��¼������ 0 - �ɹ��� ���� - ������
const int FID_LOGIN_RET_MSG                   = FID_LOGIN_BASE + 2;         // "LoginRetMsg"��¼������Ϣ
const int FID_LOGIN_TOKEN_VERIFY_RET          = FID_LOGIN_BASE + 3;         // "LoginTokenVerifyRet"Token��֤������Ϣ true - �ɹ�,  false - ʧ��
const int FID_CLIENT_VERSION				  = FID_LOGIN_BASE + 4;			// "�ͻ��˰汾��"

//----------------------  System Info ----------------------------
const int FID_SYSTEM_BASE					  = 1200;
const int FID_SERVER_TIME					  = FID_SYSTEM_BASE + 1;	     //"ServerTime": ������ʱ��
const int FID_MAC_ADDRESS                     = FID_SYSTEM_BASE + 2;
const int FID_IPV4_ADDRESS                    = FID_SYSTEM_BASE + 3;
const int FID_USER_INFO_USER_NAME             = FID_SYSTEM_BASE + 4;
const int FID_USER_INFO_TYPE_ATTRIBUTE        = FID_SYSTEM_BASE + 5;
const int FID_USER_INFO_SOFTWARE_NAME         = FID_SYSTEM_BASE + 6;
const int FID_USER_INFO_SOFTWARE_VERSION      = FID_SYSTEM_BASE + 7;
const int FID_USER_INFO_LANGUAGE              = FID_SYSTEM_BASE + 8;
const int FID_USER_INFO_BITWIDTH              = FID_SYSTEM_BASE + 9;
const int FID_USER_INFO_MAC_ADDRESS           = FID_SYSTEM_BASE + 10;
const int FID_USER_INFO_IP_ADDRESS            = FID_SYSTEM_BASE + 11;

//----------------------  Public --------------------------------
const int FID_BOND_PUBLIC_BASE                = 2000;
const int FID_BOND_KEY                        = FID_BOND_PUBLIC_BASE + 0;   // "BondKey":Bond Key
const int FID_BOND_CODE                       = FID_BOND_PUBLIC_BASE + 1;   // "BondCode":Bond Code
const int FID_BOND_TYPE                       = FID_BOND_PUBLIC_BASE + 2;   // "BondType":ծȯ���͡����ڣ���Ʊ...��, ����ծ����Ʊ...��
const int FID_BOND_SHORT_NAME                 = FID_BOND_PUBLIC_BASE + 3;   // "BondShortName":ծȯ��ƣ����磺10����09 
const int FID_BOND_CATEGORY                   = FID_BOND_PUBLIC_BASE + 4;   // "BondCategory":ծȯ���࣬BCO- ���ã�  BNC-����
const int FID_PRICE                           = FID_BOND_PUBLIC_BASE + 5;   // "Price":�۸�
const int FID_QUOTE_TYPE                      = FID_BOND_PUBLIC_BASE + 6;   // "QuoteType ":��������: ������/����/ȫ��/����
const int FID_YIELD                           = FID_BOND_PUBLIC_BASE + 7;   // "Yield":������
const int FID_YIELD_TYPE                      = FID_BOND_PUBLIC_BASE + 8;   // "YieldType":����������0-��Ȩ�����ʣ�1-����������
const int FID_MATURITY                        = FID_BOND_PUBLIC_BASE + 9;   // "Maturity":������, ���磺20150329
const int FID_RATE_TYPE                       = FID_BOND_PUBLIC_BASE + 10;  // "RateType":�������ͣ���Shibor��Depo����Ϣ��
const int FID_OPTION_TYPE                     = FID_BOND_PUBLIC_BASE + 11;  // "OptionType":��Ȩ����, ���� PUT, CAL
const int FID_CLEAN_PRICE                     = FID_BOND_PUBLIC_BASE + 12;  // "CleanPrice":���ۣ�103.0941
const int FID_DIRTY_PRICE                     = FID_BOND_PUBLIC_BASE + 13;  // "DirtyPrice":ȫ�ۣ�108.0941
const int FID_SPREAD                          = FID_BOND_PUBLIC_BASE + 14;  // "Spread":����
const int FID_ISSUER_RATING                   = FID_BOND_PUBLIC_BASE + 15;  // "IssuerRating":ծȯ��������, ����AAA, AA, A+...
const int FID_LISTED_MARKET                   = FID_BOND_PUBLIC_BASE + 16;  // "ListedMarket":�����������Ͻ�������������м䡿��CIB - ���м䣬SSE-�Ͻ�����SZE-���
const int FID_DEL_FLAG                        = FID_BOND_PUBLIC_BASE + 17;  // "DelFlag":ɾ����ǣ�1-������2-��ɾ��
const int FID_INSTITUTION_NAME                = FID_BOND_PUBLIC_BASE + 18;  // "InstitutionName":��������
const int FID_INSTITUTION_ID                  = FID_BOND_PUBLIC_BASE + 19;  // "InstitutionID":����ID
const int FID_TRADER_ID                       = FID_BOND_PUBLIC_BASE + 20;  // "TraderID":����ԱID
const int FID_TRADER_CODE                     = FID_BOND_PUBLIC_BASE + 21;  // "TraderCode":����ԱCode, ���н���Ա�����磺���·桱
const int FID_VOLUME                          = FID_BOND_PUBLIC_BASE + 22;  // "Volume":��
const int FID_SETTLEMENT_TYPE                 = FID_BOND_PUBLIC_BASE + 23;  // "SettlementType":�������ͣ�T+0,T+1
const int FID_DELIVERY_DATE                   = FID_BOND_PUBLIC_BASE + 24;  // "DeliveryDate":������
const int FID_GUARENTEED                      = FID_BOND_PUBLIC_BASE + 25;  // "Guarenteed":���޵���
const int FID_START_DATE                      = FID_BOND_PUBLIC_BASE + 26;  // "StartDate":��ʼ����
const int FID_END_DATE                        = FID_BOND_PUBLIC_BASE + 27;  // "EndDate":��������
const int FID_SHOW_ALL                        = FID_BOND_PUBLIC_BASE + 28;  // "ShowAll":��ʾ����
const int FID_INTELLIGENT_SORTING             = FID_BOND_PUBLIC_BASE + 29;  // "IntelligentSorting":��������, �����ֶΣ�ѡ�С��������򡱺�sortField = ��goodsLevel+goodsTerm��
const int FID_SORT_SEQUENCE                   = FID_BOND_PUBLIC_BASE + 30;  // "SortSequence":˳��/����, ����˳��1-˳�� 0-����
const int FID_FILTER_QUOTE                    = FID_BOND_PUBLIC_BASE + 31;  // "FilterQuote":�Ƿ���˶Լ�, ���ű������棬ѡ�С��Լۡ�checkbox
const int FID_BONDGROUP_ID                    = FID_BOND_PUBLIC_BASE + 32;  // "BondGroupId":ծȯ����ID
const int FID_BROKER_ID                       = FID_BOND_PUBLIC_BASE + 33;  // "BrokerID":������ID
const int FID_ON_THE_RUN                      = FID_BOND_PUBLIC_BASE + 34;  // "OnTheRun":������, ���5�����еģ���Listed_Dat =  [Date - 4, Date]
const int FID_MARKET_TYPE                     = FID_BOND_PUBLIC_BASE + 35;  // "MarketType":�г�����, ��һ���г�������г���1-����  2-����
const int FID_PLEDGED                         = FID_BOND_PUBLIC_BASE + 36;  // "Pledged":�Ƿ����Ѻ��Y-����Ѻ��N-������Ѻ
const int FID_ACROSS_MARKET                   = FID_BOND_PUBLIC_BASE + 37;  // "AcrossMarket":�Ƿ���г���Y-���г���N-�����г�
const int FID_ENTERPRISE_TYPE                 = FID_BOND_PUBLIC_BASE + 38;  // "EnterpriseType":��ҵ����, ���������󣬷ǹ���SOE-����NSO-�ǹ���
const int FID_POLICY_BANK_TYPE                = FID_BOND_PUBLIC_BASE + 39;  // "PolicyBankType":����-Short_Name������������������-Short_Name��������������ũ��-Short_Name������ũ����
const int FID_KEYWORDS                        = FID_BOND_PUBLIC_BASE + 40;  // "Keywords":�����ؼ���
const int FID_MUNICIPAL                       = FID_BOND_PUBLIC_BASE + 41;  // "Municipal":�Ƿ��Ͷծ, �Ƿ��Ͷծ��Y-��Ͷծ��N-�ǳ�Ͷծ
const int FID_INDEX                           = FID_BOND_PUBLIC_BASE + 42;  // "Index":����, ���緭ҳ����
const int FID_ROW_COUNT                       = FID_BOND_PUBLIC_BASE + 43;  // "RowCount":����
const int FID_BROKER_NAME                     = FID_BOND_PUBLIC_BASE + 44;  // "BrokerName":����������
const int FID_PAGE_INFOR                      = FID_BOND_PUBLIC_BASE + 45;  // "PageInfor":��ҳ��Ϣ
const int FID_COUPON_RATE                     = FID_BOND_PUBLIC_BASE + 46;  // "CouponRate":Ʊ������
const int FID_MODIFY_TIME                     = FID_BOND_PUBLIC_BASE + 47;  // "ModifyTime":�޸�ʱ��
const int FID_MATURITY_RANGE_START            = FID_BOND_PUBLIC_BASE + 48;  // "MaturityRangeStart":�����շ�Χ����ʼ��
const int FID_MATURITY_RANGE_END              = FID_BOND_PUBLIC_BASE + 49;  // "MaturityRangeEnd":�����շ�Χ����ֹ��
const int FID_MATURITY_DATE_TYPE              = FID_BOND_PUBLIC_BASE + 50;  // "MaturityDateType":����������ĩ���ǽڼ��ջ�������������
const int FID_LOGIN_ACCOUNT                   = FID_BOND_PUBLIC_BASE + 51;  // "LoginAccount":��¼��Broker ID
const int FID_BOND_KEY_LISTED_MARKET          = FID_BOND_PUBLIC_BASE + 52;  // "BondKeyListedMarket":����ֵ+�г��� Ψһ��ʾһ��ծȯ
const int FID_PIN_YIN                         = FID_BOND_PUBLIC_BASE + 53;  // "PinYin":ƴ��
const int FID_PIN_YIN_FULL                    = FID_BOND_PUBLIC_BASE + 54;  // "PinYinFull":ƴ��ȫ��
const int FID_INSTITUTION_CODE                = FID_BOND_PUBLIC_BASE + 55;  // "InstitutionCode":��������
const int FID_DESCRIPTION                     = FID_BOND_PUBLIC_BASE + 56;  // "Description":����,�籨���������ɽ�����
const int FID_INSTITUTION_FULL_NAME           = FID_BOND_PUBLIC_BASE + 57;  // "InstitutionFullName":����ȫ��
const int FID_TRADER_NAME                     = FID_BOND_PUBLIC_BASE + 59;  // "TraderName":����Ա����
const int FID_USER_NAME                       = FID_BOND_PUBLIC_BASE + 60;  // "UserName":�û���
const int FID_LOGIN_NAME                      = FID_BOND_PUBLIC_BASE + 61;  // "LoginName":��½��
const int FID_FAN_DIAN                        = FID_BOND_PUBLIC_BASE + 62;  // "FanDian":������ֵ, ���緵0.12
const int FID_FAN_DIAN_FLAG                   = FID_BOND_PUBLIC_BASE + 63;  // "FanDianFlag":�Ƿ񷵵�
const int FID_DATE_TYPE                       = FID_BOND_PUBLIC_BASE + 64;  // "DateType":ʱ������:�ɽ���/������/�����ա�"createTime" , "settlementDate", "tradedDate"��
const int FID_PAGE_INDEX                      = FID_BOND_PUBLIC_BASE + 65;  // "PageIndex":ҳ�룬Table�ĵڼ�ҳ
const int FID_PAGE_SIZE                       = FID_BOND_PUBLIC_BASE + 66;  // "PageSize":ÿҳ��ʾ������
const int FID_PAGE_COUNT                      = FID_BOND_PUBLIC_BASE + 67;  // "PageCount":��ҳ�����ܹ��ж���ҳ
const int FID_TOTAL_RECORDS                   = FID_BOND_PUBLIC_BASE + 68;  // "TotalRecords":���������ܹ��ж�������¼
const int FID_USING_PAGINATION                = FID_BOND_PUBLIC_BASE + 69;  // "UsingPagination":�Ƿ��ҳ��ʾ
const int FID_HAS_PREVIOUS                    = FID_BOND_PUBLIC_BASE + 70;  // "HasPrevious":ǰһҳ�Ƿ�����
const int FID_HAS_NEXT                        = FID_BOND_PUBLIC_BASE + 71;  // "HasNext":��һҳ�Ƿ�����
const int FID_FIRST                           = FID_BOND_PUBLIC_BASE + 73;  // "FIRST":������ֵΪ1��������ʾ��һҳ
const int FID_DEFAULT_PAGE_SIZE               = FID_BOND_PUBLIC_BASE + 74;  // "DefaultPageSize":������ֵΪ10��������ʾĬ��ҳ��С
const int FID_RETURN_CODE                     = FID_BOND_PUBLIC_BASE + 75;  // "ReturnCode":0: success, 1: failed
const int FID_ERROR_MESSAGE                   = FID_BOND_PUBLIC_BASE + 76;  // "ErrorMessage":������Ϣ
const int FID_CREATE_TIME                     = FID_BOND_PUBLIC_BASE + 77;  // "CreateTime":����ʱ��
const int FID_LISTED_DATE                     = FID_BOND_PUBLIC_BASE + 78;  // "ListedDate": ������
const int FID_ID                              = FID_BOND_PUBLIC_BASE + 79;  // "ID":��ƷID
const int FID_UNDER_WRITER_NAME               = FID_BOND_PUBLIC_BASE + 81;  // "UnderWriterName":��������
const int FID_ISSUE_AMOUNT                    = FID_BOND_PUBLIC_BASE + 82;  // "IssueAmount":������
const int FID_BOND_SUBTYPE                    = FID_BOND_PUBLIC_BASE + 83;  // "BondSubType":ծȯ����
const int FID_TIME_TO_MATURITY                = FID_BOND_PUBLIC_BASE + 84;  // "TimeToMaturity":ʣ�����ޣ���7.5Y
const int FID_PRICE_MAP                       = FID_BOND_PUBLIC_BASE + 85;  // "PriceMap":�۸������
const int FID_CDC_VALUATIONS_YIELD            = FID_BOND_PUBLIC_BASE + 86;  // "CDCValuationsYield":��ծ��ֵ������
const int FID_CDC_VALUATIONS_CLEAN_PRICE      = FID_BOND_PUBLIC_BASE + 87;  // "CDCValuationsCleanPrice":��ծ��ֵ����
const int FID_ISSUER_RATING_NPY               = FID_BOND_PUBLIC_BASE + 88;  // "IssuerRatingNPY":��������������Ԫ��
const int FID_BOND_RATING                     = FID_BOND_PUBLIC_BASE + 89;  // "BondRating":ծȯ����
const int FID_BOND_RATING_NPY                 = FID_BOND_PUBLIC_BASE + 90;  // "BondRatingNPY":ծȯ����������Ԫ��
const int FID_BID_PRICE_MAP                   = FID_BOND_PUBLIC_BASE + 91;  // "BidPriceMap": BID Price�ṹ��
const int FID_OFR_PRICE_MAP                   = FID_BOND_PUBLIC_BASE + 92;  // "OfrPriceMap": OFR Price�ṹ��
const int FID_INSTITUTION_TRADER_NAMES        = FID_BOND_PUBLIC_BASE + 93;  // "InstitutionTraderNames":����(����Ա)�����磺ũ�б�����������"
const int FID_RATING_DATE                     = FID_BOND_PUBLIC_BASE + 94;  // "RatingDate":�������ڣ�����20140619
const int FID_ISSUER_CODE                     = FID_BOND_PUBLIC_BASE + 95;  // "IssuerCode":���л�������, Z000506
const int FID_NEXT_COUPON_DATE                = FID_BOND_PUBLIC_BASE + 96;  // "NextCouponDate":��һ����Ϣ�գ�����20140619    
const int FID_OPTION_DATE                     = FID_BOND_PUBLIC_BASE + 97;  // "OptionDate":��Ȩ�գ�����20140619    
const int FID_UNDERWRITER_CODE                = FID_BOND_PUBLIC_BASE + 98;  // "UnderwriterCode":������������    
const int FID_ISSUER_RATING_INSTITUTION_CODE  = FID_BOND_PUBLIC_BASE + 99;  // "IssuerRatingInstitutionCode":����������������    
const int FID_BOND_RATING_INSTITUTION_CODE    = FID_BOND_PUBLIC_BASE + 100; // "BondRatingInstitutionCode":ծ��������������
const int FID_COUPON_RATE_SPREAD_BY_PERCENT   = FID_BOND_PUBLIC_BASE + 101; // "CouponRateSpreadByPercent":�����%��ʽ
const int FID_TIME_TO_MATURITY_WITH_OPTION_TERM = FID_BOND_PUBLIC_BASE + 102; // "TimeToMaturityWithOptionTerm":���� 360 + 300����ʾ360D + 300D
const int FID_REDEMPTION_NO_DESCRIPTION       = FID_BOND_PUBLIC_BASE + 103; // "RedemptionNODescription":���硰��ǰ��������������һ�λ�����

const int FID_SORTING_DATE                    = FID_BOND_PUBLIC_BASE + 105; // "SortingDate":�������ջ�����Ȩ������
const int FID_INSTITUTION_TYPE                = FID_BOND_PUBLIC_BASE + 106; // "InstitutionType":�����У����չ�˾��ծȯ��˾��������
const int FID_CAPITAL_TYPE                    = FID_BOND_PUBLIC_BASE + 107; // "CapitalType":������,���ʣ����ʣ�������
const int FID_INTERNAL_INSTITUTION_TYPE       = FID_BOND_PUBLIC_BASE + 108; // "InternalInstitutionType":���ǣ����ڲ�����
const int FID_INSTITUTION_CREDIT_TYPE         = FID_BOND_PUBLIC_BASE + 109; // "InstitutionCreditType": �������ԣ��������ֺͻ�����
const int FID_INSTITUTION_CITY_CODE           = FID_BOND_PUBLIC_BASE + 110; // "InstitutionCityCode": �������ڳ��д���
const int FID_INSTITUTION_CITY_NAME           = FID_BOND_PUBLIC_BASE + 111; // "InstitutionCityName": �������ڳ�������
const int FID_INSTITUTION_NAME_EN             = FID_BOND_PUBLIC_BASE + 112; // "InstitutionNameEN": ����Ӣ�ļ��
const int FID_DICTIONARY_CODE                 = FID_BOND_PUBLIC_BASE + 113; // "DictionaryCode": ��ѯ�ֵ����
const int FID_DICTIONARY_CONTENT              = FID_BOND_PUBLIC_BASE + 114; // "DictionaryContent": ��ѯ�ֵ�����Ӧ������
const int FID_DICTIONARY_GROUP_NAME           = FID_BOND_PUBLIC_BASE + 115; // "DictionaryGroupName": ��ѯ�ֵ������������
const int FID_TRADER_FCODE                    = FID_BOND_PUBLIC_BASE + 116; // "TraderFCode": ����ԱF����
const int FID_TRADABLE_BOND_CATEGORY          = FID_BOND_PUBLIC_BASE + 117; // "TradableBondCategory": ����Ա�ɽ���ծȯ���࣬���ʻ����ã����确BCO������BCO,BNC��
const int FID_CUSTOMER_ID                     = FID_BOND_PUBLIC_BASE + 118; // "CustomerID": ?
const int FID_CDC_VALUATIONS_DATE             = FID_BOND_PUBLIC_BASE + 119; // "CDCValuationsDate": String    ��ծ��ֵ����
const int FID_MATURITY_TERM                   = FID_BOND_PUBLIC_BASE + 120; // "MaturityTerm": ʣ�����޵����ֲ��֣�����3.5Y,�� MaturityTerm = 3.5
const int FID_TERM_UNIT                       = FID_BOND_PUBLIC_BASE + 121; // "TermUnit": ʣ�����޵ĵ�λ���֣�����3.5Y,�� TermUnit = Y
const int FID_GUARANTEE_INSTITUTE_CODE        = FID_BOND_PUBLIC_BASE + 122; // "GuaranteeInstituteCode": �����������룬Null��ʾ���ݲ�����������û�е�������
const int FID_BID_TRADER_IS_VIP               = FID_BOND_PUBLIC_BASE + 123; // "BidTraderIsVIP": �򷽽���Ա�Ƿ���VIP
const int FID_OFR_TRADER_IS_VIP               = FID_BOND_PUBLIC_BASE + 124; // "OfrTraderIsVIP": ��������Ա�Ƿ���VIP
const int FID_BID_PRICE                       = FID_BOND_PUBLIC_BASE + 125; // "BidPrice": Bid�۸�
const int FID_BID_FAN_DIAN                    = FID_BOND_PUBLIC_BASE + 126; // "BidFanDian": Bid ������
const int FID_BID_FAN_DIAN_FLAG               = FID_BOND_PUBLIC_BASE + 127; // "BidFanDianFlag": Bid �����ʶ
const int FID_BID_INTERNAL_FLAG               = FID_BOND_PUBLIC_BASE + 128; // "BidInternalFlag": Bid �ڲ���ʶ
const int FID_BID_DESCRIPTION                 = FID_BOND_PUBLIC_BASE + 129; // "BidDescription": Bid ��������
const int FID_BID_BARGAIN_FLAG                = FID_BOND_PUBLIC_BASE + 130; // "BidBargainFlag": Bid ��۱�ʶ
const int FID_BID_OCO_FLAG                    = FID_BOND_PUBLIC_BASE + 131; // "BidOCOFlag": Bid OCO��ʶ, 0-������ʾ��1-��ʾoco  2-oco���
const int FID_OFR_PRICE                       = FID_BOND_PUBLIC_BASE + 132; // "OfrPrice": Ofr �۸�
const int FID_OFR_FAN_DIAN                    = FID_BOND_PUBLIC_BASE + 133; // "OfrFanDian": Ofr ������
const int FID_OFR_FAN_DIAN_FLAG               = FID_BOND_PUBLIC_BASE + 134; // "OfrFanDianFlag": Ofr �����ʶ
const int FID_OFR_INTERNAL_FLAG               = FID_BOND_PUBLIC_BASE + 135; // "OfrInternalFlag": Ofr �ڲ���ʶ
const int FID_OFR_DESCRIPTION                 = FID_BOND_PUBLIC_BASE + 136; // "OfrDescription": Ofr ��������
const int FID_OFR_BARGAIN_FLAG                = FID_BOND_PUBLIC_BASE + 137; // "OfrBargainFlag": Ofr ��۱�ʶ
const int FID_OFR_OCO_FLAG                    = FID_BOND_PUBLIC_BASE + 138; // "OfrOCOFlag": Ofr OCO��ʶ, 0-������ʾ��1-��ʾoco  2-oco���
const int FID_BID_BAD_FLAG                    = FID_BOND_PUBLIC_BASE + 139; // "BidBadFlag": Bid����Bad
const int FID_OFR_BAD_FLAG                    = FID_BOND_PUBLIC_BASE + 140; // "OfrBadFlag": Ofr����Bad
const int FID_BID_IS_OWN_PRICE                = FID_BOND_PUBLIC_BASE + 141; // "BidIsOwnPrice": Bid�ҵı���
const int FID_OFR_IS_OWN_PRICE                = FID_BOND_PUBLIC_BASE + 142; // "OfrIsOwnPrice": Ofr�ҵı���
const int FID_HAS_OPTION                      = FID_BOND_PUBLIC_BASE + 143; // "HasOption" : �Ƿ�Ȩ
const int FID_BID_VOLUME                      = FID_BOND_PUBLIC_BASE + 144; // "BidVolume": BID��
const int FID_OFR_VOLUME                      = FID_BOND_PUBLIC_BASE + 145; // "OfrVolume": OFR��
const int FID_BID_QUOTE_ID                    = FID_BOND_PUBLIC_BASE + 146; // "BidQuoteID": Bid����ID
const int FID_OFR_QUOTE_ID                    = FID_BOND_PUBLIC_BASE + 147; // "OfrQuoteID": Ofr����ID
const int FID_BID_PRICE_STRING                = FID_BOND_PUBLIC_BASE + 148; // "BidPriceString"
const int FID_BID_FAN_DIAN_STRING             = FID_BOND_PUBLIC_BASE + 149; // "BidFanDianString"
const int FID_OFR_PRICE_STRING                = FID_BOND_PUBLIC_BASE + 150; // "OfrPriceString"
const int FID_OFR_FAN_DIAN_STRING             = FID_BOND_PUBLIC_BASE + 151; // "OfrFanDianString"
const int FID_PRICE_STRING                    = FID_BOND_PUBLIC_BASE + 152; // "PriceString"
const int FID_FAN_DIAN_STRING                 = FID_BOND_PUBLIC_BASE + 153; // "FanDianString"
const int FID_BID_OFR_SPREAD                  = FID_BOND_PUBLIC_BASE + 154; // "BidOfrSpread"
const int FID_CDC_OFFSET                      = FID_BOND_PUBLIC_BASE + 155; // "CDCOffset"
const int FID_TRADER_BARGAIN                  = FID_BOND_PUBLIC_BASE + 156; // "Trader: Bargain"
const int FID_ACROSS_MARKET_BOND_CODES        = FID_BOND_PUBLIC_BASE + 157; // "AcrossMarketBondCodes": ���г�����Bond Codes
const int FID_BOND_IS_EXPIRED                 = FID_BOND_PUBLIC_BASE + 160; // "IsExpired"
const int FID_MATURITY_HOLIDAYS               = FID_BOND_PUBLIC_BASE + 161; // "maturity_holidays"

const int FID_ACTION_TYPE                     = FID_BOND_PUBLIC_BASE + 200;  // "ActionType":������Ϣ���ͣ�Add��Update, Delete
const int FID_LIST                            = FID_BOND_PUBLIC_BASE + 201;  // "List":������ʶList
const int FID_MAP                             = FID_BOND_PUBLIC_BASE + 202;  // "Map":������ʶMap
const int FID_ZIP_DATA                        = FID_BOND_PUBLIC_BASE + 203;  // "ZipData":������ʶѹ������
const int FID_ZIP_LEN                         = FID_BOND_PUBLIC_BASE + 204;  // "ZipLen":������ʶѹ�����ݳ���
const int FID_UNZIP_LEN                       = FID_BOND_PUBLIC_BASE + 205;  // "ZipLen":������ʶѹ�����ݳ���

const int FID_CONDITIONS                      = FID_BOND_PUBLIC_BASE + 210;  // "Conditions":����, ���� ��FID1 = value1 and FID2 < value2��
const int FID_SORT_BY                         = FID_BOND_PUBLIC_BASE + 211;  // "SortBy":����, "FID1 asc, FID2 desc"
const int FID_RANGE                           = FID_BOND_PUBLIC_BASE + 212;  // "Range":��Χ, "1, 1000"

const int FID_TOTAL_PACKAGES                  = FID_BOND_PUBLIC_BASE + 220;  // "TotalPackages":�ܰ���
const int FID_EXECUTIME                       = FID_BOND_PUBLIC_BASE + 221;  // "Executime":ִ��ʱ��
const int FID_RATE_INDEX					  = FID_BOND_PUBLIC_BASE + 222;	 // "RateIndex":�����洢��DEPO������REPO����
const int FID_GUARANTEE_TYPE                  = FID_BOND_PUBLIC_BASE + 223;  // "GuaranteeType":������ʽ
const int FID_ISSUE_INSTITUTION               = FID_BOND_PUBLIC_BASE + 224;  // "IssueInstitution":���л����������ˣ�
const int FID_RATING_INSTITUTION              = FID_BOND_PUBLIC_BASE + 225;  // "RatingInstitution":ծȯ��������

const int FID_ISSUER_RATING_FILTER            = FID_BOND_PUBLIC_BASE + 230;  // "IssuerRating":ծȯ��������, ���ڹ��ˣ�����AAA, AA, ����...
const int FID_TIME_TO_MATURITY_REAL           = FID_BOND_PUBLIC_BASE + 231;  // "TimeToMaturityReal":ʣ�����ޣ�����������λyear
const int FID_EXPORTED_TEXT                   = FID_BOND_PUBLIC_BASE + 232;  // "ExportedText" : ���ֵ����ı�

const int FID_BOND_TYPE_ID                    = FID_BOND_PUBLIC_BASE + 233;  // "BondTypeId": ծȯ����id
const int FID_ISSUE_INSTITUTION_PINYIN        = FID_BOND_PUBLIC_BASE + 234;  // �����˵�pinyin
const int FID_ISSUE_INSTITUTION_PINYINFULL    = FID_BOND_PUBLIC_BASE + 235;  // �����˵�pinyinfull
const int FID_ISSUER_RATING_ORDER             = FID_BOND_PUBLIC_BASE + 240;
const int FID_ISSUER_RATING_NPY_ORDER         = FID_BOND_PUBLIC_BASE + 241;
const int FID_BOND_RATING_ORDER               = FID_BOND_PUBLIC_BASE + 242;
const int FID_BOND_RATING_NPY_ORDER           = FID_BOND_PUBLIC_BASE + 243;

const int FID_BOND_TYPE_STATUS                = FID_BOND_PUBLIC_BASE + 244;
const int FID_BOND_TYPE_MODIFY_TIME           = FID_BOND_PUBLIC_BASE + 245;
const int FID_FILTER_MATURITY                 = FID_BOND_PUBLIC_BASE + 246;   // �����ֶΣ���Ȩծ����option_date���Ǻ�Ȩծ����maturity_date
const int FID_OPERATION_TYPE                  = FID_BOND_PUBLIC_BASE + 247;   // ������������ͣ�Add Update Delete
const int FID_OPTION_TYPE_CLIENT              = FID_BOND_PUBLIC_BASE + 248;   // ��Ȩ���ʣ�������Ȩ����+����ծȯ����
const int FID_MAX_ISSUE_YEAR				  = FID_BOND_PUBLIC_BASE + 249;	  //bond��������
const int FID_MIN_ISSUE_YEAR				  = FID_BOND_PUBLIC_BASE + 250;	  //bond��С�������
const int FID_ISSUE_YEAR					  = FID_BOND_PUBLIC_BASE + 251;   //bond�������
const int FID_SW_SECTOR_LEVEL				  = FID_BOND_PUBLIC_BASE + 252;   //��ҵ����
const int FID_SW_SECTOR_NAME				  = FID_BOND_PUBLIC_BASE + 253;   //��ҵ����
const int FID_SW_SECTOR_CODE				  = FID_BOND_PUBLIC_BASE + 254;   //��ҵ����
const int FID_SW_SECTOR_UPPER_CODE			  = FID_BOND_PUBLIC_BASE + 255;   //������ҵ��������һ����ҵ����
const int FID_ISSUE_PROVINCE    			  = FID_BOND_PUBLIC_BASE + 256;   //bond����ʡ��
const int FID_ISSUE_PROVINCE_CODE  			  = FID_BOND_PUBLIC_BASE + 257;   //bond����ʡ�ݴ���
const int FID_ISSUING_INSTITUTION  			  = FID_BOND_PUBLIC_BASE + 258;   //bond������
const int FID_CITY_NAME                       = FID_BOND_PUBLIC_BASE + 260;  // ����
const int FID_FINANCING_PLATFORM              = FID_BOND_PUBLIC_BASE + 261;  // ƽ̨ծ
const int FID_COMPANY_BOND		              = FID_BOND_PUBLIC_BASE + 262;  // ��˾ծ
const int FID_DURATION_FILTER	              = FID_BOND_PUBLIC_BASE + 263;  // ���ڹ���
const int FID_VAL_CONVEXITY			          = FID_BOND_PUBLIC_BASE + 264;  // ͹��
const int FID_VAL_BASIC_POINT_VALUE	          = FID_BOND_PUBLIC_BASE + 265;  // PVBP
const int FID_ISSUE_INSTITUTION_RATING        = FID_BOND_PUBLIC_BASE + 266;  // �����˼���
const int FID_OUTLOOK                         = FID_BOND_PUBLIC_BASE + 267;  // չ�� Add by Young 20-Apr-2016

//----------------------  Bond Quote -----------------------------
const int FID_BOND_QUOTE_BASE                 = 3000;
const int FID_QUOTE_SIDE                      = FID_BOND_QUOTE_BASE + 1;  // "QuoteSide":���۷���, -1-ofr��1-bid
const int FID_QUOTE_STATUS                    = FID_BOND_QUOTE_BASE + 2;  // "QuoteStatus":����״̬. 0 - �������ۣ� 2 - referred�ı���
const int FID_OCO_FLAG                        = FID_BOND_QUOTE_BASE + 3;  // "OCOFlag":OCO��ʶ, 0-������ʾ��1-��ʾoco  2-oco���
const int FID_VIP_FLAG                        = FID_BOND_QUOTE_BASE + 4;  // "VIPFlag":VIP��ʶ, �Ƿ�VIP��0-�� 1-��
const int FID_BAD_FLAG                        = FID_BOND_QUOTE_BASE + 5;  // "BadFlag":��/�� ���� 
const int FID_CALCULATOR_ON                   = FID_BOND_QUOTE_BASE + 6;  // "CalculatorOn":�Ƿ���������, false-��������true-����������
const int FID_COMPARISON_SYMBOL               = FID_BOND_QUOTE_BASE + 7;  // "ComparisonSymbol":����ծ��ֵ�ıȽϷ��ţ�����>=,  <=
const int FID_HAVING_QUOTE                    = FID_BOND_QUOTE_BASE + 8;  // "HavingQuote":���Ƿ��б���
const int FID_TWO_SIDED                       = FID_BOND_QUOTE_BASE + 9;  // "TwoSided":�Ƿ�˫��
const int FID_AVG_PRICE                       = FID_BOND_QUOTE_BASE + 10; // "AvgPrice":���׾���
const int FID_CHECK_VALUATION                 = FID_BOND_QUOTE_BASE + 11; // "CheckValuation":����ծ��ֵ�Ƚ�
const int FID_DIFF_WITH_VALUATION             = FID_BOND_QUOTE_BASE + 12; // "DiffWithValuation":����ծ��ֵ��ƫ��
const int FID_QUOTE_INTERNAL_FLAG             = FID_BOND_QUOTE_BASE + 13; // "QuoteInternalFlag":1-�������ۣ�2-�ڲ�����
const int FID_QUOTE_ID                        = FID_BOND_QUOTE_BASE + 14; // "QuoteID": ����ID
const int FID_BARGAIN_FLAG                    = FID_BOND_QUOTE_BASE + 15; // "BargainFlag":��۱�ʶ
const int FID_REFER_TIME                      = FID_BOND_QUOTE_BASE + 16; // "ReferTime":Refer Time
const int FID_PRICE_DESCRIPTION               = FID_BOND_QUOTE_BASE + 17; // "PriceDescription":�۸�ע

//----------------------  Oper log -----------------------------
const int FID_OPER_LOG_BASE = 3200;
const int FID_OPER_TIME = FID_OPER_LOG_BASE + 1;						  // "OperTime":����ʱ��
const int FID_OPER_USERNAME = FID_OPER_LOG_BASE + 2;					  // "OperUsername":������
const int FID_OPER_TYPE = FID_OPER_LOG_BASE + 3;						  // "OperType":��������

//----------------------  Bond Deal -----------------------------
const int FID_BOND_DEAL_BASE                  = 4000;
const int FID_DEAL_ID                         = FID_BOND_DEAL_BASE + 0;   // "DealID":�ɽ�ID
const int FID_DEAL_NO                         = FID_BOND_DEAL_BASE + 1;   // "DealNo":�ɽ�����
const int FID_DIRECTION                       = FID_BOND_DEAL_BASE + 2;   // "Direction":��������, ��������, GVN/TKN��TRD
const int FID_DEAL_INTERNAL_FLAG              = FID_BOND_DEAL_BASE + 3;   // "DealInternalFlag":1-���ڲ��ɽ���2-�ڲ��ɽ�
const int FID_DEAL_STATUS                     = FID_BOND_DEAL_BASE + 4;   // "DealStatus":�ɽ���״̬, 0-ȷ�ϣ�1-���ύ��2-���ύ��3-��ͨ����5-δͨ����7-�ѻٵ���13-�����У�-1-��ɾ��

const int FID_BID_INSTITUTION_ID              = FID_BOND_DEAL_BASE + 5;   // "BidInstitutionId":Bid������ID
const int FID_BID_INSTITUTION_NAME            = FID_BOND_DEAL_BASE + 6;   // "BidInstitutionName":Bid����������
const int FID_BID_INSTITUTION_CITY            = FID_BOND_DEAL_BASE + 7;   // "BidInstitutionCity":Bid���������ڳ���
const int FID_BID_TRADER_ID                   = FID_BOND_DEAL_BASE + 8;   // "BidTraderID":Bid������ԱID
const int FID_BID_TRADER_NAME                 = FID_BOND_DEAL_BASE + 9;   // "BidTraderName":Bid������Ա����
const int FID_BID_BROKER_ID                   = FID_BOND_DEAL_BASE + 10;  // "BidBrokerId":Bid��������ID
const int FID_BID_BROKER_NAME                 = FID_BOND_DEAL_BASE + 11;  // "BidBrokerName":Bid������������
const int FID_BID_BROKERAGE                   = FID_BOND_DEAL_BASE + 12;  // "BidBrokerage":Bid��Ӷ��
const int FID_BID_IS_BRIDGE                   = FID_BOND_DEAL_BASE + 13;  // "BidIsBridge":Bid���Ƿ�Ϊ���Ż���
const int FID_BID_IS_CHARGED                  = FID_BOND_DEAL_BASE + 14;  // "BidIsCharged":Bid���Ƿ�Ӷ��
const int FID_BID_BROKER_CHECKED              = FID_BOND_DEAL_BASE + 15;  // "BidBrokerChecked":Bid���������Ƿ��Ѿ�ȷ��
const int FID_BID_BROKER_CHECK_DATE           = FID_BOND_DEAL_BASE + 16;  // "BidBrokerCheckDate":Bid��������ȷ��ʱ��
const int FID_BID_TRADER_CHECKED              = FID_BOND_DEAL_BASE + 17;  // "BidTraderChecked":Bid������Ա�Ƿ��Ѿ�ȷ��
const int FID_BID_TRADER_CHECKDATE            = FID_BOND_DEAL_BASE + 18;  // "BidTraderCheckDate":Bid������Աȷ��ʱ��

const int FID_BRIDGE_INSTITUTION_ID           = FID_BOND_DEAL_BASE + 19;  // "BridgeInstitutionId":���Ż���ID
const int FID_BRIDGE_INSTITUTION_NAME         = FID_BOND_DEAL_BASE + 20;  // "BridgeInstitutionName":���Ż�������
const int FID_BRIDGE_TRADER_ID                = FID_BOND_DEAL_BASE + 21;  // "BridgeTraderId":���Ż�������ԱID
const int FID_BRIDGE_TRADER_NAME              = FID_BOND_DEAL_BASE + 22;  // "BridgeTraderName":���Ż�������Ա����
const int FID_BRIDGE_IS_CHARGED               = FID_BOND_DEAL_BASE + 23;  // "BridgeIsCharged":���Ż����Ƿ�Ӷ��

const int FID_OFR_INSTITUTION_ID              = FID_BOND_DEAL_BASE + 24;  // "OfrInstitutionId":Ofr������ID
const int FID_OFR_INSTITUTION_NAME            = FID_BOND_DEAL_BASE + 25;  // "OfrInstitutionName":Ofr����������
const int FID_OFR_INSTITUTION_CITY            = FID_BOND_DEAL_BASE + 26;  // "OfrInstitutionCity":Ofr���������ڳ���
const int FID_OFR_TRADER_ID                   = FID_BOND_DEAL_BASE + 27;  // "OfrTraderID":Ofr������ԱID
const int FID_OFR_TRADER_NAME                 = FID_BOND_DEAL_BASE + 28;  // "OfrTraderName":OFR������Ա����
const int FID_OFR_BROKER_ID                   = FID_BOND_DEAL_BASE + 29;  // "OfrBrokerId":Ofr��������ID
const int FID_OFR_BROKER_NAME                 = FID_BOND_DEAL_BASE + 30;  // "OfrBrokerName":Ofr������������
const int FID_OFR_BROKERAGE                   = FID_BOND_DEAL_BASE + 31;  // "OfrBrokerage":Ofr��Ӷ��
const int FID_OFR_IS_BRIDGE                   = FID_BOND_DEAL_BASE + 32;  // "OfrIsBridge":Ofr���Ƿ�Ϊ���Ż���
const int FID_OFR_IS_CHARGED                  = FID_BOND_DEAL_BASE + 33;  // "OfrIsCharged":Ofr���Ƿ�Ӷ��
const int FID_OFR_BROKER_CHECKED              = FID_BOND_DEAL_BASE + 34;  // "OfrBrokerChecked":Ofr���������Ƿ��Ѿ�ȷ��
const int FID_OFR_BROKER_CHECK_DATE           = FID_BOND_DEAL_BASE + 35;  // "OfrBrokerCheckDate":Ofr��������ȷ��ʱ��
const int FID_OFR_TRADER_CHECKED              = FID_BOND_DEAL_BASE + 36;  // "OfrTraderChecked":Ofr������Ա�Ƿ��Ѿ�ȷ��
const int FID_OFR_TRADER_CHECK_DATE           = FID_BOND_DEAL_BASE + 37;  // "OfrTraderCheckDate":Ofr������Աȷ��ʱ��

const int FID_CURRENCY                        = FID_BOND_DEAL_BASE + 38;  // "Currency":����, ����, USD/CNY��
const int FID_SETTLEMENT_DATE                 = FID_BOND_DEAL_BASE + 39;  // "SettlementDate":������
const int FID_SETTLEMENT_MODE                 = FID_BOND_DEAL_BASE + 40;  // "SettlementMode":����ģʽ, ��DVP
const int FID_SETTLEMENT_AMOUNT               = FID_BOND_DEAL_BASE + 41;  // "SettlementAmount":������

const int FID_DEAL_TIME                       = FID_BOND_DEAL_BASE + 42;  // "DealTime":�ɽ�ʱ��
const int FID_CONFIRM_DATE                    = FID_BOND_DEAL_BASE + 43;  // "ConfirmDate"�����գ�����ȷ������
const int FID_CONFIRM_ACCOUNT                 = FID_BOND_DEAL_BASE + 44;  // "ConfirmAccount":���գ�����ȷ������
const int FID_BID_INSTITUTION_TRADER_NAMES    = FID_BOND_DEAL_BASE + 45;  // "BidInstitutionTraderNames":�򷽻���(����Ա)�����磺ũ�б�����������
const int FID_IS_PRINT                        = FID_BOND_DEAL_BASE + 46;  // "IsPrint":�Ƿ��Ѿ���ӡ, 1-�Ѿ���ӡ
const int FID_GROUP_FLAG                      = FID_BOND_DEAL_BASE + 47;  // "GroupFlag":���ŵ������ɽ����ı�ʶ, ���û�й��ţ���ΪNULL
const int FID_TRADED_DATE                     = FID_BOND_DEAL_BASE + 48;  // "TradedDate":��������
const int FID_REMARKS                         = FID_BOND_DEAL_BASE + 49;  // "Remarks":����
const int FID_SEND_STATUS                     = FID_BOND_DEAL_BASE + 50;  // "SendStatus":��Ϣ���͵�״̬, 2��ֹͣ��3�����¼�ʱ��1����������
const int FID_OFR_INSTITUTION_TRADER_NAMES    = FID_BOND_DEAL_BASE + 51;  // "OfrInstitutionTraderNames":��������(����Ա)�����磺ũ�б�����������
const int FID_BID_INSTITUTION_CODE            = FID_BOND_DEAL_BASE + 52;  // "BidInstitutionCode":Bid����������
const int FID_OFR_INSTITUTION_CODE            = FID_BOND_DEAL_BASE + 53;  // "OfrInstitutionCode":Ofr����������
const int FID_BID_CHECK						  = FID_BOND_DEAL_BASE + 54;  // "BidCheck":Bid��ȷ�ϱ�ʶ
const int FID_OFR_CHECK						  = FID_BOND_DEAL_BASE + 55;  // "OfrCheck":Ofr��ȷ�ϱ�ʶ
const int FID_BID_CHECK_DATE			      = FID_BOND_DEAL_BASE + 56;  // "BidCheckDate":Bid��ȷ��ʱ��
const int FID_OFR_CHECK_DATE			      = FID_BOND_DEAL_BASE + 57;  // "OfrCheckDate":Ofr��ȷ��ʱ��
const int FID_DEAL_STATUS_STRING		      = FID_BOND_DEAL_BASE + 58;  // "DealStatusString":�ɽ���ȷ��״̬
const int FID_SERIAL_NUMBER                   = FID_BOND_DEAL_BASE + 59;  // "SerialNumber":�ɽ������к�
const int FID_BID_REMARKS                     = FID_BOND_DEAL_BASE + 60;  // "BidRemarks": Bid��ע
const int FID_OFR_REMARKS                     = FID_BOND_DEAL_BASE + 61;  // "OfrRemarks": Ofr��ע
const int FID_BID_BROKERAGE_RATE              = FID_BOND_DEAL_BASE + 62;  // "BidBrokerageRate": BidӶ�����
const int FID_OFR_BROKERAGE_RATE              = FID_BOND_DEAL_BASE + 63;  // "OfrBrokerageRate": OfrӶ�����
const int FID_BID_DISCOUNT                    = FID_BOND_DEAL_BASE + 64;  // "BidDiscount": BidӶ���ۿ�
const int FID_OFR_DISCOUNT                    = FID_BOND_DEAL_BASE + 65;  // "OfrDiscount": OfrӶ���ۿ�

const int FID_BID_BROKER_CODE				  = FID_BOND_DEAL_BASE + 66;  // bid broker code
const int FID_OFR_BROKER_CODE				  = FID_BOND_DEAL_BASE + 67;  // bid broker code

const int FID_BID_BROKER_ID_A                 = FID_BOND_DEAL_BASE + 68;  // Bid��������ID
const int FID_BID_BROKER_ID_B                 = FID_BOND_DEAL_BASE + 69;
const int FID_BID_BROKER_ID_C                 = FID_BOND_DEAL_BASE + 70;
const int FID_BID_BROKER_ID_D                 = FID_BOND_DEAL_BASE + 71;
const int FID_BID_BROKER_NAME_A               = FID_BOND_DEAL_BASE + 72;  // Bid������������
const int FID_BID_BROKER_NAME_B               = FID_BOND_DEAL_BASE + 73;
const int FID_BID_BROKER_NAME_C               = FID_BOND_DEAL_BASE + 74;
const int FID_BID_BROKER_NAME_D               = FID_BOND_DEAL_BASE + 75;
const int FID_BID_PERCENT_A                   = FID_BOND_DEAL_BASE + 76;  // Bid��Ӷ�����
const int FID_BID_PERCENT_B                   = FID_BOND_DEAL_BASE + 77;
const int FID_BID_PERCENT_C                   = FID_BOND_DEAL_BASE + 78;
const int FID_BID_PERCENT_D                   = FID_BOND_DEAL_BASE + 79;
const int FID_OFR_BROKER_ID_A                 = FID_BOND_DEAL_BASE + 80;  // ofr��������ID
const int FID_OFR_BROKER_ID_B                 = FID_BOND_DEAL_BASE + 81;
const int FID_OFR_BROKER_ID_C                 = FID_BOND_DEAL_BASE + 82;
const int FID_OFR_BROKER_ID_D                 = FID_BOND_DEAL_BASE + 83;
const int FID_OFR_BROKER_NAME_A               = FID_BOND_DEAL_BASE + 84;  // ofr������������
const int FID_OFR_BROKER_NAME_B               = FID_BOND_DEAL_BASE + 85;
const int FID_OFR_BROKER_NAME_C               = FID_BOND_DEAL_BASE + 86;
const int FID_OFR_BROKER_NAME_D               = FID_BOND_DEAL_BASE + 87;
const int FID_OFR_PERCENT_A                   = FID_BOND_DEAL_BASE + 88;  // ofr��Ӷ�����
const int FID_OFR_PERCENT_B                   = FID_BOND_DEAL_BASE + 89;
const int FID_OFR_PERCENT_C                   = FID_BOND_DEAL_BASE + 90;
const int FID_OFR_PERCENT_D                   = FID_BOND_DEAL_BASE + 91;

//----------------------  DCS BOND DEAL ����ֶ� -----------------------------
const int FID_DCS_RESULT                      = FID_BOND_DEAL_BASE + 200;	// dcs �������
const int FID_DCS_IN_HAND                     = FID_BOND_DEAL_BASE + 201;	// true:���ڴ����� false:����״̬
const int FID_BID_TRADE_MODE                  = FID_BOND_DEAL_BASE + 202;	// dcs bid�����׷�ʽ
const int FID_BID_IS_NO_BROKERAGE             = FID_BOND_DEAL_BASE + 203;	// Bid���Ƿ���Ӷ
const int FID_BID_NO_CONFIRM                  = FID_BOND_DEAL_BASE + 204;	// Bid��NC
const int FID_BID_NO_COMMENTS                 = FID_BOND_DEAL_BASE + 205;	// Bid��NC��ע
const int FID_OFR_TRADE_MODE                  = FID_BOND_DEAL_BASE + 206;	// dcs ofr�����׷�ʽ
const int FID_OFR_IS_NO_BROKERAGE             = FID_BOND_DEAL_BASE + 207;	// Ofr���Ƿ���Ӷ
const int FID_OFR_NO_CONFIRM                  = FID_BOND_DEAL_BASE + 208;	// Ofr��NC
const int FID_OFR_NO_COMMENTS                 = FID_BOND_DEAL_BASE + 209;	// Ofr��NC��ע
const int FID_BID_SPECIAL_PARTY               = FID_BOND_DEAL_BASE + 210;	// bid���ر�ϸ��
const int FID_OFR_SPECIAL_PARTY               = FID_BOND_DEAL_BASE + 211;	// bid���ر�ϸ��
const int FID_DCS_SPECIAL_TERMS               = FID_BOND_DEAL_BASE + 212;	// ����ϸ��
const int FID_FEEDBACK                        = FID_BOND_DEAL_BASE + 213;	// dcs ��̨����
const int FID_SWITCH_STATUS                   = FID_BOND_DEAL_BASE + 214;	// dcs switch status

// dcs �ύʧ�ܻ�ٵ�ʧ����Ϣ����
const int FID_DCS_FAIL_DCS_ID                 = FID_BOND_DEAL_BASE + 215;	// �ɽ���ID
const int FID_DCS_FAIL_MSG                    = FID_BOND_DEAL_BASE + 216;	// ������Ϣ
const int FID_DCS_FAIL_OPERATE_TYPE           = FID_BOND_DEAL_BASE + 217;	// �������� submit/destroy
const int FID_DCS_FAIL_OPERATE_NAME           = FID_BOND_DEAL_BASE + 218;	// ������

//dcs�ɽ�����ʷ��������
const int FID_DCS_HISTORY_OPERATE             = FID_BOND_DEAL_BASE + 219;	// ����
const int FID_DCS_HISTORY_OPERATOR            = FID_BOND_DEAL_BASE + 220;	// ������
const int FID_DCS_HISTORY_UPDATE_TIME         = FID_BOND_DEAL_BASE + 221;	// ����ʱ��
const int FID_DCS_HISTORY_COMMENT             = FID_BOND_DEAL_BASE + 222;	// ��ע
const int FID_DCS_HISTORY_UPDATE_CONTENT      = FID_BOND_DEAL_BASE + 223;	// ��������

const int FID_RELATE_SUBMIT_COMMENTS          = FID_BOND_DEAL_BASE + 224;	// �����ύ��ע
const int FID_URGENT_STATUS                   = FID_BOND_DEAL_BASE + 225;	// �Ӽ���ʶ
const int FID_OLD_CONTRACT_ID                 = FID_BOND_DEAL_BASE + 226;	// �༭���Ų�ʱ���ϳɽ�����ID
const int FID_RELATED_MSG                     = FID_BOND_DEAL_BASE + 227;	// ���Ź����ֶ�
const int FID_BID_TRADER_ACCOUNT              = FID_BOND_DEAL_BASE + 228;	// Bid������Ա�˻���
const int FID_OFR_TRADER_ACCOUNT              = FID_BOND_DEAL_BASE + 229;	// Ofr������Ա�˻���
const int FID_DCS_QUESTION_MARK               = FID_BOND_DEAL_BASE + 230;	// �ʺű��

const int FID_DCS_DESTROY_REASON              = FID_BOND_DEAL_BASE + 231;	// �ٵ�ԭ��

//----------------------  Product Favor -----------------------------
const int FID_BOND_GROUP_BASE                 = 5000;
const int FID_GROUP_NAME                      = FID_BOND_GROUP_BASE + 0;  // "GroupName":������

const int FID_GROUP_ID                        = FID_BOND_GROUP_BASE + 3;  // "GroupId":����ID
const int FID_USER_ACCOUNT_ID                 = FID_BOND_GROUP_BASE + 4;  // "UserAccountId":�ͻ�ID
const int FID_GROUP_FILTER                    = FID_BOND_GROUP_BASE + 5;  // "GroupFilter": �����������

//----------------------  Calculator  -----------------------------
const int FID_CALCULATOR_BASE                 = 6000;
const int FID_INTEREST_START_DATE             = FID_CALCULATOR_BASE + 0;   // "InterestStartDate":��Ϣ��
const int FID_FIRST_COUPON_DATE               = FID_CALCULATOR_BASE + 1;   // "FirstCouponDate":��һ�θ�Ϣ��
const int FID_COUPON_DAY_ADJUST               = FID_CALCULATOR_BASE + 2;   // "CouponDayAdjust":��Ϣ���Ƿ����
const int FID_COUPON_DAY_DMC                  = FID_CALCULATOR_BASE + 3;   // "CouponDayDMC":��Ϣ�յ�������
const int FID_COUPON_CALENDAR_KEY             = FID_CALCULATOR_BASE + 4;   // "CouponCalendarKey":��Ϣ�յ�������
const int FID_MATURITY_ADJUST                 = FID_CALCULATOR_BASE + 5;   // "MaturityAdjust:"�������Ƿ����
const int FID_MATURITY_DMC                    = FID_CALCULATOR_BASE + 6;   // "MaturityDMC":�����յ�������
const int FID_MATURITY_CALENDAR_KEY           = FID_CALCULATOR_BASE + 7;   // "MaturityCalendarKey":�����յ�������
const int FID_COUPON_TYPE                     = FID_CALCULATOR_BASE + 8;   // "CouponType":ծȯ��Ϣ����
const int FID_COUPON_FREQUENCY                = FID_CALCULATOR_BASE + 9;   // "CouponFrequency":��ϢƵ��
const int FID_COUPON_RATE_SPREAD              = FID_CALCULATOR_BASE + 10;  // "CouponRateSpread":Ʊ������/����
const int FID_ISSUE_PRICE                     = FID_CALCULATOR_BASE + 11;  // "IssuePrice":���м�
const int FID_INTEREST_BASIS                  = FID_CALCULATOR_BASE + 12;  // "InterestBasis":��Ϣ����
const int FID_SIMPLE_COMPOUND                 = FID_CALCULATOR_BASE + 13;  // "SimpleCompound":����/����
const int FID_COMPOUND_FREQUENCY              = FID_CALCULATOR_BASE + 14;  // "CompoundFrequency":����Ƶ��
const int FID_FRN_MULTIPLIER                  = FID_CALCULATOR_BASE + 15;  // "FRNMultiplier":��׼���ʱ���
const int FID_FRN_INDEX_ID                    = FID_CALCULATOR_BASE + 16;  // "FRNIndexID":��׼����ID
const int FID_FIRST_INDEX_RATE                = FID_CALCULATOR_BASE + 17;  // "FirstIndexRate":�״θ�Ϣ����
const int FID_FIXING_FREQUENCY                = FID_CALCULATOR_BASE + 18;  // "FixingFrequency":��׼���ʹ̶�Ƶ��
const int FID_FIXING_MA_DAYS                  = FID_CALCULATOR_BASE + 19;  // "FixingMADays":��׼�����ƶ�ƽ������
const int FID_FIXING_PRECEDS                  = FID_CALCULATOR_BASE + 20;  // "FixingPreceds":��׼���ʹ̶���ǰ����
const int FID_FIXING_CALENDAR_KEY             = FID_CALCULATOR_BASE + 21;  // "FixingCalendarKey":��׼���ʹ̶�����
const int FID_FIXING_DIGIT                    = FID_CALCULATOR_BASE + 22;  // "FixingDigit":��׼���ʱ���С��λ��
const int FID_RESET_EFFECTIVE                 = FID_CALCULATOR_BASE + 23;  // "ResetEffective":������Ч
const int FID_REDEMPTION_NO                   = FID_CALCULATOR_BASE + 24;  // "RedemptionNo":��ǰ��������
const int FID_REDEMPTION_STR                  = FID_CALCULATOR_BASE + 25;  // "RedemptionStr":��ǰ�������ڼ����
const int FID_COMPENSATE_RATE                 = FID_CALCULATOR_BASE + 26;  // "CompensateRate":�������ʷ�Χ
const int FID_COMPENSATE_FROM                 = FID_CALCULATOR_BASE + 27;  // "CompensateFrom":����������ʼ����
const int FID_OPTION_STYLE                    = FID_CALCULATOR_BASE + 28;  // "OptionStyle":ŷʽ/��ʽ
const int FID_CALL_NO                         = FID_CALCULATOR_BASE + 29;  // "CallNo":��ش���
const int FID_CALL_STR                        = FID_CALCULATOR_BASE + 30;  // "CallStr":������ڼ���ؼ۸�
const int FID_PUT_NO                          = FID_CALCULATOR_BASE + 31;  // "PutNo":���۴���
const int FID_PUT_STR                         = FID_CALCULATOR_BASE + 32;  // "PutStr":�������ڼ����ۼ۸�
const int FID_ASSIGN_TRANSSTR                 = FID_CALCULATOR_BASE + 33;  // "AssignTransStr":����ת�����ڼ��۸�
const int FID_CAP                             = FID_CALCULATOR_BASE + 34;  // "Cap":������������
const int FID_FLOOR                           = FID_CALCULATOR_BASE + 35;  // "Floor":������������

const int FID_IS_FRN_BOND                     = FID_CALCULATOR_BASE + 36;  // "IsFRNBond": �Ƿ�Ϊ����ծȯ
const int FID_IS_YTC_AVAILABLE                = FID_CALCULATOR_BASE + 37;  // "IsYTCAvailable": �Ƿ�����
const int FID_IS_YTP_AVAILABLE                = FID_CALCULATOR_BASE + 38;  // "IsYTPAvailable": �Ƿ�ɻ���
const int FID_INDEX_RATE                      = FID_CALCULATOR_BASE + 39;  // "IndexRate":��׼����ֵ
const int FID_INDEX_DATE                      = FID_CALCULATOR_BASE + 40;  // "IndexDate":��׼����ȡֵ����
const int FID_YIELD_TO_MATURITY               = FID_CALCULATOR_BASE + 41;  // "YieldToMaturity":����������
const int FID_YIELD_TO_CALL                   = FID_CALCULATOR_BASE + 42;  // "YieldToCall":��Ȩ�����ʣ���أ�
const int FID_YIELD_TO_PUT                    = FID_CALCULATOR_BASE + 43;  // "YieldToPut":��Ȩ�����ʣ����ۣ�
const int FID_ACCRUED_DAYS                    = FID_CALCULATOR_BASE + 44;  // "AccruedDays":��Ϣ����
const int FID_ACCRUED_AMOUNT                  = FID_CALCULATOR_BASE + 45;  // "AccruedAmount":Ӧ����Ϣ
const int FID_EXECUTION_DATE                  = FID_CALCULATOR_BASE + 46;  // "ExecutionDate":��Ȩ����
const int FID_MACAULAY_DURATION               = FID_CALCULATOR_BASE + 47;  // "MacaulayDuration":Macauly����
const int FID_MODIFIED_DURATION               = FID_CALCULATOR_BASE + 48;  // "ModifiedDuration":��������
const int FID_PVBP                            = FID_CALCULATOR_BASE + 49;  // "PVBP":PVBP
const int FID_CONVEXITY                       = FID_CALCULATOR_BASE + 50;  // "Convexity":͹��
const int FID_CALCULATOR_ERROR_MESSAGE        = FID_CALCULATOR_BASE + 51;  // "CalculatorErrorMessage":������Ϣ
const int FID_CALCULATE_ACTION_TYPE           = FID_CALCULATOR_BASE + 52;  // "CalculateActionType":���㶯������
const int FID_CALENDAR_MARKET_KEY             = FID_CALCULATOR_BASE + 53;  // "CalendarMarketKey":�����г�
const int FID_DATE                            = FID_CALCULATOR_BASE + 54;  // "Date" ����
const int FID_PERIOD_NUM                      = FID_CALCULATOR_BASE + 55;  // "PeriodNum" ���ڵ�������
const int FID_PERIOD_UNIT                     = FID_CALCULATOR_BASE + 56;  // "PeriodNum" ���ڵ�����λ
const int FID_BUSINESS_DAY_CONVENTION         = FID_CALCULATOR_BASE + 57;  // "BusinessDayConvention" ���ڵ�������
const int FID_YIELD_TO_EXECUTION              = FID_CALCULATOR_BASE + 58;  // "YieldToExecution":��Ȩ������
const int FID_TENOR                           = FID_CALCULATOR_BASE + 59;  // "Tenor":"1D"
const int FID_CALCULATOR_ERROR_CODE		   = FID_CALCULATOR_BASE + 60;  // "CalculatorErrorMessage":������Ϣ

//----------------------  Best Quote -----------------------------
const int FID_BOND_BEST_QUOTE_BASE            = 7000;
const int FID_BEST_BID_VOLUME                 = FID_BOND_BEST_QUOTE_BASE + 0;  // "BestBidVolume":Bid��������λ��Ԫ��
const int FID_BEST_OFR_VOLUME                 = FID_BOND_BEST_QUOTE_BASE + 1;  // "BestOfrVolume":Ofr��������λ��Ԫ��
const int FID_BEST_BID_PRICE                  = FID_BOND_BEST_QUOTE_BASE + 2;  // "BestBidPrice":����Bid����
const int FID_BEST_OFR_PRICE                  = FID_BOND_BEST_QUOTE_BASE + 3;  // "BestOfrPrice":����Ofr����
const int FID_BEST_BID_QUOTE_COUNT            = FID_BOND_BEST_QUOTE_BASE + 4;  // "BestBidQuoteCount":Bid���ű��۸���
const int FID_BEST_OFR_QUOTE_COUNT            = FID_BOND_BEST_QUOTE_BASE + 5;  // "BestOfrQuoteCount":Ofr���ű��۸���
const int FID_BEST_BID_DIRTY_PRICE            = FID_BOND_BEST_QUOTE_BASE + 6;  // "BestBidDirtyPrice":Bidȫ��
const int FID_BEST_OFR_DIRTY_PRICE            = FID_BOND_BEST_QUOTE_BASE + 7;  // "BestOfrDirtyPrice":Ofrȫ��
const int FID_BEST_BID_CLEAN_PRICE            = FID_BOND_BEST_QUOTE_BASE + 8;  // "BestBidCleanPrice":Bid����
const int FID_BEST_OFR_CLEAN_PRICE            = FID_BOND_BEST_QUOTE_BASE + 9;  // "BestOfrCleanPrice":Ofr����
const int FID_BEST_BID_SPREAD                 = FID_BOND_BEST_QUOTE_BASE + 10; // "BestBidSpread":Bid����
const int FID_BEST_OFR_SPREAD                 = FID_BOND_BEST_QUOTE_BASE + 11; // "BestOfrSpread":Ofr����
const int FID_BID_QUOTE_COUNT                 = FID_BOND_BEST_QUOTE_BASE + 12; // "BidQuoteCount":Bid���۸���
const int FID_BEST_INTERNAL_BID_PRICE         = FID_BOND_BEST_QUOTE_BASE + 13; // "InternalBidPrice": ���ű����е��ڲ�Bid����
const int FID_BEST_INTERNAL_OFR_PRICE         = FID_BOND_BEST_QUOTE_BASE + 14; // "InternalOfrPrice": ���ű����е��ڲ�Ofr����
const int FID_BEST_INTERNAL_BID_FANDIAN       = FID_BOND_BEST_QUOTE_BASE + 15; // "InternalBidFanDian": ���ű����е��ڲ�Bid����
const int FID_BEST_INTERNAL_OFR_FANDIAN       = FID_BOND_BEST_QUOTE_BASE + 16; // "InternalOfrFanDian": ���ű����е��ڲ�Ofr����
const int FID_BEST_INTERNAL_BID_FANDIAN_FLAG  = FID_BOND_BEST_QUOTE_BASE + 17; // "InternalBidFanDianFlag": ���ű����е��ڲ�Bid�Ƿ񷵵�
const int FID_BEST_INTERNAL_OFR_FANDIAN_FLAG  = FID_BOND_BEST_QUOTE_BASE + 18; // "InternalOfrFanDianFlag": ���ű����е��ڲ�Ofr�Ƿ񷵵�
const int FID_COMPLETED_QUOTE_SORT            = FID_BOND_BEST_QUOTE_BASE + 19; // "CompletedQuoteSort": ���������������ֶ�
const int FID_COMPLETED_BOND_TYPE             = FID_BOND_BEST_QUOTE_BASE + 20; // "CompletedBondType": ����������ծȯ����
const int FID_OFR_QUOTE_COUNT				  = FID_BOND_BEST_QUOTE_BASE + 21; // "OfrQuoteCount": Ofr���ű��۸���

const int FID_BID_QUOTE_TYPE                  = FID_BOND_BEST_QUOTE_BASE + 24; // "BidQuoteType":Bid��������
const int FID_OFR_QUOTE_TYPE                  = FID_BOND_BEST_QUOTE_BASE + 25; // "OfrQuoteType": Ofr��������
const int FID_BID_YIELD                       = FID_BOND_BEST_QUOTE_BASE + 26; // "BidYield": Bid������
const int FID_OFR_YIELD                       = FID_BOND_BEST_QUOTE_BASE + 27; // "OfrYield": Ofr������
const int FID_BID_SETTLEMENT_TYPE             = FID_BOND_BEST_QUOTE_BASE + 28; // "BidSettlementType": Bid�������ͣ�T+0,T+1
const int FID_OFR_SETTLEMENT_TYPE             = FID_BOND_BEST_QUOTE_BASE + 29; // "OfrSettlementType": OFR�������ͣ�T+0,T+1
const int FID_BID_DELIVERY_DATE               = FID_BOND_BEST_QUOTE_BASE + 30; // "BidDeliveryDate": BID������
const int FID_OFR_DELIVERY_DATE               = FID_BOND_BEST_QUOTE_BASE + 31; // "BidDeliveryDate": OFR������
const int FID_BID_YIELD_TYPE                  = FID_BOND_BEST_QUOTE_BASE + 32; // "BidYieldType": BID����������0-��Ȩ�����ʣ�1-����������
const int FID_OFR_YIELD_TYPE                  = FID_BOND_BEST_QUOTE_BASE + 33; // "OfrYieldType": OFR����������0-��Ȩ�����ʣ�1-����������
const int FID_INTERNAL_BID_QUOTE_ID           = FID_BOND_BEST_QUOTE_BASE + 34; // "InternalBidQuoteID": �ڲ�Bid����ID
const int FID_INTERNAL_OFR_QUOTE_ID           = FID_BOND_BEST_QUOTE_BASE + 35; // "InternalOfrQuoteID": �ڲ�Ofr����ID
const int FID_BID_BEST_QUOTE_ID               = FID_BOND_BEST_QUOTE_BASE + 36; // "BidBestQuoteId": Bid���ű���ID
const int FID_OFR_BEST_QUOTE_ID               = FID_BOND_BEST_QUOTE_BASE + 37; // "OfrBestQuoteId": Ofr���ű���ID
const int FID_IS_BEST_QUOTE                   = FID_BOND_BEST_QUOTE_BASE + 38; // "IsBestQuote": ���ű��� true ���ű��� false
const int FID_INTERNAL_BID_PRICE_STRING       = FID_BOND_BEST_QUOTE_BASE + 39; // "InternalBidPriceString"
const int FID_INTERNAL_BID_FAN_DIAN_STRING    = FID_BOND_BEST_QUOTE_BASE + 40; // "InternalBidFanDianString"
const int FID_INTERNAL_OFR_PRICE_STRING       = FID_BOND_BEST_QUOTE_BASE + 41; // "InternalOfrPriceString"
const int FID_INTERNAL_OFR_FAN_DIAN_STRING    = FID_BOND_BEST_QUOTE_BASE + 42; // "InternalOfrFanDianString"
const int FID_BEST_QUOTE_ID                   = FID_BOND_BEST_QUOTE_BASE + 43; // "BestQuoteId" ���ű���id
const int FID_BEST_BID_VOLUME_CLIENT          = FID_BOND_BEST_QUOTE_BASE + 44;  // "BestBidVolumeClient": �������ű��������ۼ�ֵ
const int FID_BEST_OFR_VOLUME_CLIENT          = FID_BOND_BEST_QUOTE_BASE + 45;  // "BestOfrVolumeClient": �������ű��������ۼ�ֵ

const int FID_HAVING_EXTERNAL_QUOTE           = FID_BOND_BEST_QUOTE_BASE + 50;
const int FID_HAVING_INTERNAL_QUOTE           = FID_BOND_BEST_QUOTE_BASE + 51;
const int FID_HAVING_BID_QUOTE                = FID_BOND_BEST_QUOTE_BASE + 52;
const int FID_HAVING_OFR_QUOTE                = FID_BOND_BEST_QUOTE_BASE + 53;
const int FID_BEST_BID_VOLUME_TOTAL			  = FID_BOND_BEST_QUOTE_BASE + 54;
const int FID_BEST_OFR_VOLUME_TOTAL			  = FID_BOND_BEST_QUOTE_BASE + 55;
const int FID_BEST_BID_DESCRIPTION			  = FID_BOND_BEST_QUOTE_BASE + 56;
const int FID_BEST_OFR_DESCRIPTION			  = FID_BOND_BEST_QUOTE_BASE + 57;


//---------------------  Hot Key ----------------------------------
const int FID_HOT_KEY_BASE                    = 8000;
const int FID_HOT_KEY_ID                      = FID_HOT_KEY_BASE + 0;          //"HotKeyId": ��ݼ�ID
const int FID_HOT_KEY_CODE                    = FID_HOT_KEY_BASE + 1;          //"HotKeyCode":   ��ݼ�Code����"Ctrl+0"
const int FID_HOT_KEY_FUNCTION_NAME           = FID_HOT_KEY_BASE + 2;          //"HotKeyFunctionName": ��ݼ��������ܿ�����
const int FID_HOT_KEY_DESCRIPTION             = FID_HOT_KEY_BASE + 3;          //"HotKeyDescription": ��ϸ��Ϣ
const int FID_HOT_KEY_FUNCTION                = FID_HOT_KEY_BASE + 4;          //"HotKeyFunction": ��ݼ��������ܿ����

//--------------------- Export ------------------------------------
const int FID_EXPORT_BASE                     = 8500;
const int FID_EXPORT_FILE_PATH                = FID_EXPORT_BASE + 0;           //"ExportPath": ���������صı���·��
const int FID_EXPORT_FILE_NAME                = FID_EXPORT_BASE + 1;           //"ExportName": ���������ص��ļ�����
const int FID_EXPORT_IS_INTERNAL              = FID_EXPORT_BASE + 2;           //"ExportIsInternal": �������Ƿ�Ϊ�ڲ����� 

#define SET_FID_MAP(fid, column) fid_mp_[fid] = #column;					   //���ַ���ת��FastDB��SQL���


//----------------------System Management--------------------------
const int FID_CONSTANTS_BASE                 = 9000;
const int FID_CONSTANTS_ID                   = FID_CONSTANTS_BASE + 0;        //"ConstantsId": ����id
const int FID_CONSTANTS_NAME                 = FID_CONSTANTS_BASE + 1;        //"ConstantsName": ��������
const int FID_CONSTANTS_VALUE                = FID_CONSTANTS_BASE + 2;        //"ConstantsValue": ����ֵ
const int FID_CONSTANTS_KEY                  = FID_CONSTANTS_BASE + 3;        //"ConstantsKey": ������ֵ

//------------------------Push Message------------------------------
const int FID_PUSH_MESSAGE_BASE              = 10000;
const int FID_PUSH_MESSAGE_VERSION           = FID_PUSH_MESSAGE_BASE + 0;          //"version"
const int FID_PUSH_MESSAGE_LAST_VERSION      = FID_PUSH_MESSAGE_BASE + 1;          //"lastVersion"
const int FID_PUSH_MESSAGE_COMPANY_ID        = FID_PUSH_MESSAGE_BASE + 2;          //"companyId"
const int FID_PUSH_MESSAGE_COMPANY_NAME      = FID_PUSH_MESSAGE_BASE + 3;          //"companyName"

const int FID_PUSH_MESSAGE_BOND_QUOTE        = FID_PUSH_MESSAGE_BASE + 10;         //"bondOfferMessage"            
const int FID_PUSH_MESSAGE_BOND_DEAL		 = FID_PUSH_MESSAGE_BASE + 11;         //"bondDealMessage"
const int FID_PUSH_MESSAGE_BOND_BEST_QUOTE	 = FID_PUSH_MESSAGE_BASE + 12;         //"bondBestOfferMessage"

const int FID_PUSH_MESSAGE_QUOTE_LIST        = FID_PUSH_MESSAGE_BASE + 20;         //"bondOffer"
const int FID_PUSH_MESSAGE_DEAL_LIST         = FID_PUSH_MESSAGE_BASE + 21;         //"bondDeal"
const int FID_PUSH_MESSAGE_BEST_QUOTE_LIST	 = FID_PUSH_MESSAGE_BASE + 22;         //"bondBestOffer"

const int FID_PUSH_MESSAGE_METHOD_ADD        = FID_PUSH_MESSAGE_BASE + 30;         //"methodAdd"
const int FID_PUSH_MESSAGE_METHOD_UPDATE     = FID_PUSH_MESSAGE_BASE + 31;         //"methodUpdate"
const int FID_PUSH_MESSAGE_METHOD_DELETE     = FID_PUSH_MESSAGE_BASE + 32;         //"methodDelete"
const int FID_PUSH_MESSAGE_METHOD_REFER      = FID_PUSH_MESSAGE_BASE + 33;         //"methodRefer"
const int FID_PUSH_MESSAGE_METHOD_UNREFER    = FID_PUSH_MESSAGE_BASE + 34;         //"methodUnRefer"


//------------------------Undo operate------------------------------
const int FID_UNDO_OPERATE					  = 20000;
const int FID_UNDO_SERVICE					  = FID_UNDO_OPERATE + 0;         //not for table field
const int FID_UNDO_METHOD				      = FID_UNDO_OPERATE + 1;         //not for table field
const int FID_UNDO_METHOD_TYPE               = FID_UNDO_OPERATE + 2;
const int FID_UNDO_LIST				  = FID_UNDO_OPERATE + 3;

//------------------------For QQ2IDB------------------------------
const int FID_QQ2IDB_PARSEDMSG					  = 30000;
const int FID_QQ2IDB_PARSER_RESULT  			  = FID_QQ2IDB_PARSEDMSG + 1;
const int FID_QQ2IDB_ERROR_MAP                    = FID_QQ2IDB_PARSEDMSG + 2;
const int FID_QQ2IDB_ERROR_LIST                   = FID_QQ2IDB_PARSEDMSG + 3;

//------------------------For QQ2IDB ERROR MSG------------------------------
const int FID_QQ2IDB_IS_SUCCESS					  = 31000;                     // bool, create quote success or not
const int FID_QQ2IDB_ERROR_CODE  			      = FID_QQ2IDB_IS_SUCCESS + 1;  // pre-defined error code
const int FID_QQ2IDB_ERROR_MSG                    = FID_QQ2IDB_IS_SUCCESS + 2;  // error msg

const int FID_ACK_MSG_HEAD         = 7777;
const int FID_ACK_MSG_BODY         = 7778;
const int FID_BOND_CALCULATOR         = 9898;
#endif // _FID_H_