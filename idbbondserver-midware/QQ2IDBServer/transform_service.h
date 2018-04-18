#ifndef TRANSFORM_SERVICE_H_
#define TRANSFORM_SERVICE_H_

#include "sdbus/connection.h"

namespace bcserver_service_transform {

    class VerifyData;
class Transform_service : public sdbus::Callback {
public:
    Transform_service(sdbus::Connection* conn);
    virtual void OnMsgs(const sdbus::Message ** msgs, int count);
    void AddQuote(const sdbus::Message *, sdbus::Message &reply);
    void DeleteQuote(const sdbus::Message *msg, sdbus::Message &reply);

private:
    sdbus::Connection* conn_;
};

}  // namespace bcserver_service_transform

#endif