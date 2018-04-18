/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Quote Service
====================================================================================================================
   Date       Name                                  Description of Change
12-Jun-2014   Guoying Feng                          Initial version
19-Aug-2014   James Xu                              Add CopyQuote
21-Aug-2014   James Xu                              Add CopyReferredQuote
$HISTORY$
===================================================================================================================*/
#ifndef _BONDQUOTE_SERVICE_H_
#define _BONDQUOTE_SERVICE_H_

#include "service/service.h"
#include "object/quote.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"

class BondQuoteService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

    static void SetBondQuoteMap(const BondQuoteCache *cache, sdbus::VariantMap *mp);
    static void SetBondReferQuoteMap(const BondQuoteReferCache *cache, sdbus::VariantMap *mp);

	void PublishBondQuote(const std::vector<BondQuoteCachePtr> &bond_quote_vec, const char *action_type, bool without_data = true, const std::string bond_category = "NONE"); // change default to true
	void PublishBondReferQuote(const std::vector<BondQuoteReferCachePtr> &bond_quote_vec, const char *action_type, bool without_data = true); // change default to true

private:
    friend class ServiceManager;
    BondQuoteService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void GetBondQuoteList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondBidQuoteList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondOfrQuoteList(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondQuoteByID(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondReferQuoteList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondQuoteByConditions(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondQuoteIdList(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondQuoteListInc(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondReferQuoteListInc(const sdbus::Message* msg, sdbus::Message& reply);

    void CopyQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void CopyReferredQuote(const sdbus::Message* msg, sdbus::Message& reply);

    void AddQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateQuoteInfo(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateInternalInfo(const sdbus::Message* msg, sdbus::Message& reply);
    void DeleteReferredQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void ReferQuote(const sdbus::Message* msg, sdbus::Message& reply);
	void ReferAllQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void UnReferQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void GvnTknQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void EditReferredQuote(const sdbus::Message* msg, sdbus::Message& reply);
	void UpdateReferredQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void ExportBondQuote(const sdbus::Message* msg, sdbus::Message& reply);

    void DeleteQuote(const sdbus::Message* msg, sdbus::Message& reply);

    void UpdateYiJiBanRate(const sdbus::Message* msg, sdbus::Message& reply);
    void GetYiJiBanRate(const sdbus::Message* msg, sdbus::Message& reply);
};

#endif // _BONDQUOTE_SERVICE_H_