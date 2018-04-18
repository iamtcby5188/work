#include "heartbeat_service.h"
#include "include/fid.h"
#include "sdbus/connection.h"
#include <time.h>
#include "cache/cache_center.h"
#include "cache/cache_manager.h"
#include "datamanager/data_manager.h"

HeartBeatService::HeartBeatService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{

}

void HeartBeatService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if(method == "HeartBeat.SendHeartBeat"){
        ProcessHeartBeat(msg, reply);
    } else if (method == "Cache.ResetCache") {
        resetCache(msg, reply);
    }
    
    if (msg->GetReplyTo().length() > 0) {
        bool flag = conn_->Send(msg->GetReplyTo(), reply);
        if (flag) {
            LOGGER_INFO("Reply success: " << msg->GetType())
        } else {
            LOGGER_ERROR("Reply failed: " << msg->GetType())
        }
    }
}

void HeartBeatService::ProcessHeartBeat(const sdbus::Message* msg, sdbus::Message& reply)
{

	reply.SetString(FID_SERVER_TIME, currentDateTime());
}

const std::string HeartBeatService::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
	//"2015-12-02 21;47:59"
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

void HeartBeatService::resetCache(const sdbus::Message* msg, sdbus::Message& reply) {
    LOGGER_INFO("Start Reset Cache...");
    CacheCenter::sharedInstance()->clearCache();
    CacheManager::singleton()->InitCache();
    DataManager::singleton()->reloadData();
    LOGGER_INFO("End Reset Cache...");
}