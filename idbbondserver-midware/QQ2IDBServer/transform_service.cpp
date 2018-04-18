#include "transform_service.h"

#include <iostream>
#include <map>
#include "include/fid.h"
#include "include/constants.h"
#include "include/common.h"
#include "service/send_service.h"
#include "message.h"
#include "add_bond_quote.h"

namespace bcserver_service_transform {

    Transform_service::Transform_service(sdbus::Connection* conn) : conn_(conn) {
        Init(kServiceQueueQQ2IDB, sdbus::DeliverDirect);
        conn_->AddHandler(this);

        /*
        sdbus::Message msg;
        sdbus::VariantMap map;
        sdbus::VariantList list;
        sdbus::VariantMap *quoteMap = list.AddMap();
        quoteMap->SetString(FID_BOND_CODE, "1480317");
        quoteMap->SetString(FID_BOND_SHORT_NAME, "1480317");
        quoteMap->SetString(FID_INSTITUTION_CODE, "20160122");
        quoteMap->SetString(FID_TRADER_CODE, "100");
        quoteMap->SetInt32(FID_VOLUME, 5);
        quoteMap->SetInt32(FID_DIRECTION, 1);
        quoteMap->SetDouble(FID_PRICE, 105);

        map.SetList(FID_QQ2IDB_PARSER_RESULT, list);
        msg.SetMap(FID_QQ2IDB_PARSEDMSG, map);

        sdbus::Message rd;
        msg.SetType("QQ2IDB.AddQuote");
        SendService::singleton()->call(kServiceQueueQQ2IDB, msg, rd, 5000);
        */
    }

    void Transform_service::OnMsgs(const sdbus::Message ** msgs, int count) {
        std::cout << "<<--Receive message-->>" << "[" << count << "]" << std::endl;
        for (int i = 0; i < count; i++) {
            sdbus::Message reply;
            const sdbus::Message * msg = *(msgs + i);
            sdbus::string method = msg->GetType();
            LOGGER_INFO("Receive message: " << method)
            if(method == "QQ2IDB.AddQuote")
                AddQuote(msg, reply);
            else if(method == "QQ2IDB.DeleteQuote")
                DeleteQuote(msg, reply);

            if (msg->GetReplyTo().length() > 0) {
                bool flag;
                flag = conn_->Send(msg->GetReplyTo(), reply);

                if (flag) {
                    LOGGER_INFO("Reply success: " << msg->GetType())
                } else {
                    LOGGER_ERROR("Reply failed: " << msg->GetType())
                }
            }
        }
    }

    void Transform_service::AddQuote(const sdbus::Message *msg, sdbus::Message &reply) {

        // Verify data and create data
        AddBondQuote v(msg);
        if(!v.sendquote(reply)) {
            v.processError();
        }
    }

    void Transform_service::DeleteQuote(const sdbus::Message *msg, sdbus::Message &reply) {

    }

}  // namespace bcserver_service_transform
