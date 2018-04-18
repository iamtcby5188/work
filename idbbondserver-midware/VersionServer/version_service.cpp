#include "version_service.h"

#include <iostream>
#include <map>
#include "include/fid.h"
#include "include/constants.h"
#include "include/common.h"

const std::string FILENAME = "userinfo.xml";
namespace bcserver_service_version {

    version_service::version_service(sdbus::Connection* conn) : conn_(conn) {
        Init(kServiceQueueUserInfo, sdbus::DeliverDirect);
        conn_->AddHandler(this);
    }

    void version_service::OnMsgs(const sdbus::Message ** msgs, int count) {
        for (int i = 0; i < count; i++) {
            const sdbus::Message * msg = *(msgs + i);
            sdbus::string method = msg->GetType();
            std::cout << "Receive message: " << method << std::endl;

            if (method == "UserInfo.SaveUserInfo") {
                SaveUserInfo(msg);
            }
        }
    }

    void version_service::SaveUserInfo(const sdbus::Message *msg) {
        std::map<int, std::string> m;

        msg->GetString(FID_USER_INFO_USER_NAME,         m[FID_USER_INFO_USER_NAME]);
        msg->GetString(FID_USER_INFO_TYPE_ATTRIBUTE,    m[FID_USER_INFO_TYPE_ATTRIBUTE]);
        msg->GetString(FID_USER_INFO_SOFTWARE_NAME,     m[FID_USER_INFO_SOFTWARE_NAME]);
        msg->GetString(FID_USER_INFO_SOFTWARE_VERSION,  m[FID_USER_INFO_SOFTWARE_VERSION]);
        msg->GetString(FID_USER_INFO_LANGUAGE,          m[FID_USER_INFO_LANGUAGE]);
        msg->GetString(FID_USER_INFO_BITWIDTH,          m[FID_USER_INFO_BITWIDTH]);
        msg->GetString(FID_USER_INFO_MAC_ADDRESS,       m[FID_USER_INFO_MAC_ADDRESS]);
        msg->GetString(FID_USER_INFO_IP_ADDRESS,        m[FID_USER_INFO_IP_ADDRESS]);

        pugi::xml_document doc;
        doc.load_file(FILENAME.c_str());
        pugi::xml_node node;

        std::string infoString = "/Info";
        std::string userString = infoString + "/User[@name='"+ m[FID_USER_INFO_USER_NAME] +"']";
        std::string selectString = userString + "/Software[@type='" + m[FID_USER_INFO_TYPE_ATTRIBUTE] + "'][Mac='" + m[FID_USER_INFO_MAC_ADDRESS] + "']";

        remove_nodes(doc, "/*[name()!='Info']");
        remove_nodes(doc, "/Info[position()>1]");
        remove_nodes(doc, "/Info/*[name()!='User']");
        remove_nodes(doc, (userString+"[position()>1]").c_str());

        if(doc.select_nodes(selectString.c_str()).size() == 0){
            auto resultInfo = doc.select_nodes(infoString.c_str());
            if(resultInfo.size() == 0) {
                // no node "Info"
                doc.reset();
                node = doc.append_child("Info").append_child("User");
                node.append_attribute("name").set_value(m[FID_USER_INFO_USER_NAME].c_str());
            }else{
                auto resultUser = doc.select_nodes(userString.c_str());
                if(resultUser.size()==0) {
                    // no expected node "User"
                    node = resultInfo[0].node().append_child("User");
                    node.append_attribute("name").set_value(m[FID_USER_INFO_USER_NAME].c_str());
                }else{
                    node = resultUser[0].node();
                }
            }
        }else{
            node = doc.select_nodes(selectString.c_str())[0].parent();
            // remove all the nodes under same user with same software type and same mac address
            remove_nodes(doc, selectString.c_str());
        }

        node = node.append_child("Software");
        node.append_attribute("type").set_value(m[FID_USER_INFO_TYPE_ATTRIBUTE].c_str());

        node.append_child("Name")   .text().set(m[FID_USER_INFO_SOFTWARE_NAME].c_str());
        node.append_child("Version").text().set(m[FID_USER_INFO_SOFTWARE_VERSION].c_str());
        node.append_child("Lang")   .text().set(m[FID_USER_INFO_LANGUAGE].c_str());
        node.append_child("Bit")    .text().set(m[FID_USER_INFO_BITWIDTH].c_str());
        node.append_child("Mac")    .text().set(m[FID_USER_INFO_MAC_ADDRESS].c_str());
        node.append_child("Ip")     .text().set(m[FID_USER_INFO_IP_ADDRESS].c_str());

        doc.save_file(FILENAME.c_str());
    }

    void version_service::remove_nodes(const pugi::xml_document& doc, const char *search_query){
        pugi::xpath_node_set temp_node_set;
        while((temp_node_set = doc.select_nodes(search_query)).size()!=0)
            temp_node_set[0].node().parent().remove_child(temp_node_set[0].node());
    };
}  // namespace bcserver_service_version
