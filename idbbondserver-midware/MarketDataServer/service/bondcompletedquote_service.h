#ifndef _BONDCOMPLETEDQUOTE_SERVICE_H_
#define _BONDCOMPLETEDQUOTE_SERVICE_H_

#include "service/service.h"
#include "object/completedquote.h"

class BondCompletedQuoteService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

private:
    friend class ServiceManager;
    BondCompletedQuoteService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void CopyCompletedQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void CopyInternalCompletedQuote(const sdbus::Message* msg, sdbus::Message& reply);
    void GetCompletedQuoteDetailList(const sdbus::Message *msg, sdbus::Message &reply);

};

#endif // _BONDCOMPLETEDQUOTE_SERVICE_H_