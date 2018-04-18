#ifndef _HOTKEYSERVICE_H_
#define _HOTKEYSERVICE_H_

#include "service/service.h"

class HotKeyService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

private:
    friend class ServiceManager;
    HotKeyService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void GetHotKeyList(const sdbus::Message* msg, sdbus::Message& reply);

};

#endif // _HOTKEYSERVICE_H_