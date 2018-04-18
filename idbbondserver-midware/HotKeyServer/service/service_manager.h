#ifndef _SERVICE_MANAGER_H_
#define _SERVICE_MANAGER_H_

#include <map>
#include "sdbus/string.h"

class Service;
namespace sdbus {
    class Connection;
}    

class ServiceManager
{
public:
    static ServiceManager* singleton();

    Service* getService(sdbus::Connection *conn, const std::string& service_name);
    Service* getService(const std::string& service_name);

    void ready();

    ~ServiceManager();
private:
    ServiceManager();

    std::map<std::string, Service*> service_mp_;

    static ServiceManager *singleton_;
};


#endif // _SERVICE_MANAGER_H_