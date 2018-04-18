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
#ifndef _DCSBONDDEAL_SERVICE_H_
#define _DCSBONDDEAL_SERVICE_H_

#include "service/service.h"
#include "object/bonddeal.h"
#include "cache/controller/dcsbonddeal_cachecontroller.h"

class DcsBondDealService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);
   
    static void SetBondDealMap(const DcsBondDealCache *cache, sdbus::VariantMap *mp);
    
    static void SetBondDealCache(const sdbus::VariantMap *mp, DcsBondDealCache *cache);
private:
    friend class ServiceManager;
    DcsBondDealService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void GetBondDealByID(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondDealList(const sdbus::Message* msg, sdbus::Message& reply);

    void CopyDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void CopyDetailedDeal(const sdbus::Message* msg, sdbus::Message& reply);

};

#endif // _DCSBONDDEAL_SERVICE_H_