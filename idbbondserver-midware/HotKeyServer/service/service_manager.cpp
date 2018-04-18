#include "service_manager.h"
#include "service/service.h"
#include "sdbus/connection.h"
#include "include/constants.h"
#include "hotkeyservice.h"

ServiceManager* ServiceManager::singleton_ = NULL;

ServiceManager::ServiceManager()
{

}

ServiceManager::~ServiceManager()
{
    std::map<std::string, Service*>::const_iterator it = service_mp_.begin();

    for ( ; it != service_mp_.end(); it++) {
        delete (*it).second;
    }

    service_mp_.clear();
}

ServiceManager* ServiceManager::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new ServiceManager();
    }

    return singleton_;
}

Service* ServiceManager::getService(sdbus::Connection *conn, const std::string& service_name)
{
    std::map<std::string, Service*>::const_iterator it = service_mp_.find(service_name);

    if (it == service_mp_.end()) { 
        // not find, new a service
        Service *service = NULL;

        if (service_name == kServiceHotKey) {
            service = new HotKeyService(conn, kServiceQueueHotKey, kTopicHotKey);
        }

        if (service == NULL) {
            return NULL;
        }

        if (!service->init()) {
            delete service;
            return NULL;
        }

        service_mp_[service_name] = service;
    }

    return service_mp_[service_name];
}

Service* ServiceManager::getService(const std::string& service_name)
{ 
    if (service_mp_.find(service_name) == service_mp_.end()) { 
        return NULL;
    }

    return service_mp_[service_name];
}

void ServiceManager::ready()
{
    std::map<std::string, Service*>::const_iterator it = service_mp_.begin();

    for ( ; it != service_mp_.end(); it++) {
        Service *service = (*it).second;

        if (service) {
            service->ready();
        }
    }
}