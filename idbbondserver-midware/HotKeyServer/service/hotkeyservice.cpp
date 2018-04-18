#include "hotkeyservice.h"
#include "business/hotkeybusiness.h"
#include "include/fid.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"


HotKeyService::HotKeyService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{

}

void HotKeyService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "HotKey.HotKey") {
        if (subject == "model") {
            GetHotKeyList(msg, reply);
        }
    }
    
    if (msg->GetReplyTo().length() > 0) {   
        size_t len = -1;
        VariantMapCalcSize(reply.MapValue(), len);
        LOGGER_INFO("Message Size: " << len)

        bool flag = conn_->Send(msg->GetReplyTo(), reply);
        if (flag) {
            LOGGER_INFO("Reply success: " << msg->GetType())
        } else {
            LOGGER_ERROR("Reply failed: " << msg->GetType())
        }
    }
}

void HotKeyService::GetHotKeyList(const sdbus::Message* msg, sdbus::Message& reply)
{
    HotKeyBusiness business(this);

    HotKeyCacheVecPtr bonds = business.GetHotKeyList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<HotKeyCachePtr>::const_iterator it = bonds->begin();
    
    for ( ; it != bonds->end(); it++) {
        HotKeyCache* cache = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        mp->SetString(FID_HOT_KEY_CODE, cache->HotKey);
        mp->SetString(FID_HOT_KEY_FUNCTION_NAME, cache->FunctionName);
        mp->SetString(FID_HOT_KEY_DESCRIPTION, cache->Description);
        mp->SetString(FID_HOT_KEY_FUNCTION, cache->Function);
    }
}

