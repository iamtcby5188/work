/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Service
====================================================================================================================
   Date       Name                                  Description of Change
05-Jun-2014   Huameng Jia                           Initial version
20-Aug-2014   James Xu                              Add GetBondSmallViewListByID, GetInstitutionInfoListByID, 
$HISTORY$
===================================================================================================================*/
#ifndef _BONDSERVICE_H_
#define _BONDSERVICE_H_

#include "service/service.h"
#include "object/bond.h"
#include "cache/controller/bondsmallview_cachecontroller.h"

class BondSmallViewCache;
class TradingInstitutionCache;
class BondTraderCache;
class AccountCache;
class BrokerBindingTraderCache;
class ChinaBondDeviatedValueCache;
//class ChinaBondValuationCache;
class BondService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

    static void SetBondSmallViewMap(const BondSmallViewCache* cache, sdbus::VariantMap *mp);
    static void SetBondInfoMap(const BondSmallViewCache* cache, sdbus::VariantMap *mp);
    static void SetInstitutionInfoMap(const TradingInstitutionCache* cache, sdbus::VariantMap *mp);
    static void SetTraderInfoMap(const BondTraderCache* cache, sdbus::VariantMap *mp);
    static void SetAccountInfoMap(const AccountCache* cache, sdbus::VariantMap *mp);
    static void SetBrokerBindingTraderMap(const BrokerBindingTraderCache* cache, sdbus::VariantMap *mp);
    static void SetBondDeviatedValueMap(const ChinaBondDeviatedValueCache* cache, sdbus::VariantMap *mp);
    //static void SetBondValuationMap(const ChinaBondValuationCache* cache, sdbus::VariantMap *mp);

    static void SetBondSmallViewDetailMap(const BondSmallViewCache* cache, sdbus::VariantMap *mp);

    void PublishBondSmallView(const std::vector<BondSmallViewCache*> &bond_vec, const char* action_type);

    void PublishBondSmallViewDetail(const std::vector<BondSmallViewCache*> &bond_vec, const char* action_type);

    void PublishBondInfo(const std::vector<BondSmallViewCache*> &bond_vec, const char* action_type);

    void PublishInstitutionInfo(const std::vector<TradingInstitutionCache*> &institution_vec, const char* action_type);

    void PublishTraderInfo(const std::vector<BondTraderCache*> &trader_vec, const char* action_type);

    void PublishAccountInfo(const std::vector<AccountCache*> &account_vec, const char* action_type);

    void PublishBrokerBindingTrader(const std::vector<BrokerBindingTraderCache*> &binding_vec, const char* action_type);

    void PublishBondDeviatedValue(const std::vector<ChinaBondDeviatedValueCache*> &deviatedvalue_vec, const char* action_type);

    //void PublishBondValuation();

private:
    friend class ServiceManager;
    BondService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

	void GetBondSmallViewList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondSmallViewIncList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondInfoByCondition(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondInfoByID(const sdbus::Message* msg, sdbus::Message& reply);
    bool GetBondInfoByCode(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondSmallViewDetailList(const sdbus::Message* msg, sdbus::Message& reply);

    void GetInstitutionInfoList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetInstitutionInfoByID(const sdbus::Message* msg, sdbus::Message& reply);

    void GetTraderInfoList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetTraderInfoByID(const sdbus::Message* msg, sdbus::Message& reply);

    void GetAccountInfoList(const sdbus::Message* msg, sdbus::Message& reply);
	void UpdateAccountInfoByID(const sdbus::Message* msg, sdbus::Message& reply);
	void UpdateAccountPasswordByID(const sdbus::Message* msg, sdbus::Message& reply);
    void GetAccountInfoByID(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBrokerBindingTraderList(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondDeviatedValueList(const sdbus::Message* msg, sdbus::Message& reply);

    //void GetBondValuationList(const sdbus::Message* msg, sdbus::Message& reply);

	void GetBondIssueYearBound(const sdbus::Message* msg, sdbus::Message& reply);
    void GetProvinceInfoList(const sdbus::Message* msg, sdbus::Message& reply);
	void GetSwSectorInfoList(const sdbus::Message* msg, sdbus::Message& reply); 
    void GetIssuingInstitutionList(const sdbus::Message* msg, sdbus::Message& reply);
	void GetBondValuation(const sdbus::Message* msg, sdbus::Message& reply);

	void GetTreasuryFuture(const sdbus::Message* msg, sdbus::Message& reply);

private:
	void GetBondSmallViewInc(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondSmallViewDetailInc(const sdbus::Message* msg, sdbus::Message& reply);
	void GetBondInfoByConditionInc(const sdbus::Message* msg, sdbus::Message& reply);
	void GetInstitutionInfoListInc(const sdbus::Message* msg, sdbus::Message& reply);
	void GetTraderInfoListInc(const sdbus::Message* msg, sdbus::Message& reply);
	void GetAccountInfoListInc(const sdbus::Message* msg, sdbus::Message& reply);
	void GetBrokerBindingTraderListInc(const sdbus::Message* msg, sdbus::Message& reply);
	void GetBondDeviatedValueListInc(const sdbus::Message* msg, sdbus::Message& reply);

    bool createOrGetAndPublishInstitutionTraderInfo(const sdbus::Message* msg, sdbus::Message& reply);
};

#endif // _BONDSERVICE_H_