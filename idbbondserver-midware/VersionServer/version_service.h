#ifndef BCSERVER_SERVICE_VERSION_SERVICE_H_
#define BCSERVER_SERVICE_VERSION_SERVICE_H_

#include "sdbus/connection.h"
#include "../IDBCommon/util/pugixml/pugixml.hpp"

namespace bcserver_service_version {

class version_service : public sdbus::Callback {
public:
    version_service(sdbus::Connection* conn);
    virtual void OnMsgs(const sdbus::Message ** msgs, int count);
    void SaveUserInfo(const sdbus::Message *);
private:
    void remove_nodes(const pugi::xml_document&, const char *);

    sdbus::Connection* conn_;
};

}  // namespace bcserver_service_version

#endif