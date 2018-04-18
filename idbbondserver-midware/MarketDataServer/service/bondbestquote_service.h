/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Best Quote Service
====================================================================================================================
   Date       Name                                  Description of Change
20-Jun-2014   Huameng Jia                           Initial version
19-Aug-2014   James Xu                              Add CopyBestQuote and CopyCompletedQuote
21-Aug-2014   James Xu                              Add CopyInternalBestQuote
$HISTORY$ 
===================================================================================================================*/
#ifndef _BONDBESTQUOTE_SERVICE_H_
#define _BONDBESTQUOTE_SERVICE_H_

#include "service/service.h"
#include "cache/controller/bondbestquote_cachecontroller.h"

class BondBestQuoteService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

    static void SetBondBestQuoteMap(const BondBestQuoteCache *cache, sdbus::VariantMap *mp);

    static void SetBondBestQuoteCache(const sdbus::VariantMap *mp, BondBestQuoteCache *cache);

private:
    friend class ServiceManager;
    BondBestQuoteService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void GetBondBestQuoteList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondBestQuoteByID(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondBestQuoteIdList(const sdbus::Message* msg, sdbus::Message& reply);
	void GetCompletedQuoteList(const sdbus::Message* msg, sdbus::Message& reply);

    void CopyBestQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void CopyInternalBestQuote(const sdbus::Message* msg, sdbus::Message& reply);    

};

#endif // _BONDBESTQUOTE_SERVICE_H_