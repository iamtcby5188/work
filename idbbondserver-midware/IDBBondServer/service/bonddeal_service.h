/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Deal Service
====================================================================================================================
   Date       Name                                  Description of Change
05-Jun-2014   James Xu                              Initial version
20-Aug-2014   James Xu                              Add CopyDeal and CopyDetailedDeal
26-Dec-2014   James Xu                              Remove AddDealFromQuote
$HISTORY$
===================================================================================================================*/
#ifndef _BONDDEAL_SERVICE_H_
#define _BONDDEAL_SERVICE_H_

#include "service/service.h"
#include "object/bonddeal.h"
#include "cache/controller/bonddeal_cachecontroller.h"

class BondDealService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);
   
    static void SetBondDealMap(const BondDealCache *cache, sdbus::VariantMap *mp);
    
    void PublishBondDeal(const std::vector<BondDealCachePtr> &bond_deal_vec, const char* action_type, bool without_data = true); // change default to true

private:
    friend class ServiceManager;
    BondDealService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void AddBondDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateBondDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateDealInfo(const sdbus::Message* msg, sdbus::Message& reply);
	void UpdateDealInfoUndo(const sdbus::Message* msg, sdbus::Message& reply);

    void DeleteBondDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondDealByID(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondDealList(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondDealListInc(const sdbus::Message* msg, sdbus::Message& reply);

    void CopyDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void CopyDetailedDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void ExportDealData(const sdbus::Message* msg, sdbus::Message& reply);
};

#endif // _BONDDEAL_SERVICE_H_