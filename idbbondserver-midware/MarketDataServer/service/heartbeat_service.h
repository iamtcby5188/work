#ifndef _HEARTBEAT_SERVICE_H_
#define _HEARTBEAT_SERVICE_H_

#include "service/service.h"

class HeartBeatService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

private:
    friend class ServiceManager;
    HeartBeatService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void ProcessHeartBeat(const sdbus::Message* msg, sdbus::Message& reply);
	const std::string currentDateTime();
    void resetCache(const sdbus::Message* msg, sdbus::Message& reply);
};



#endif // _HEARTBEAT_SERVICE_H_