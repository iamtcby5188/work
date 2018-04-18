#include "service_manager.h"
#include "service/service.h"
#include "sdbus/connection.h"
#include "include/constants.h"
#include "bondservice.h"
#include "management_service.h"
#include "sync_service.h"
#include "productfavor_service.h"

ServiceManager* ServiceManager::singleton_ = NULL;

ServiceManager::ServiceManager()
{

}

ServiceManager::~ServiceManager()
{
    std::map<std::string, Service*>::const_iterator it = service_mp_.begin();

    for ( ; it != service_mp_.end(); ++it) {
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

bool ServiceManager::init(sdbus::Connection *conn)
{
    if (getService(conn, kServiceBondInfo) == NULL) {
        LOGGER_ERROR("create service failed in basic data server: " << kServiceBondInfo);
        return false;
    }
    if (getService(conn, kServiceManagement) == NULL) {
        LOGGER_ERROR("create service failed in basic data server: " << kServiceManagement);
        return false;
    }
    if (getService(conn, kServiceSync) == NULL) {
        LOGGER_ERROR("create service faield in basic data server: " << kServiceSync);
        return false;
    }
    if (getService(conn, kServiceProductFavor) == NULL) {
        LOGGER_ERROR("create service faield in basic data server: " << kServiceProductFavor);
        return false;
    }

    return true;
}

Service* ServiceManager::getService(sdbus::Connection *conn, const std::string& service_name)
{
    std::map<std::string, Service*>::const_iterator it = service_mp_.find(service_name);

    if (it == service_mp_.end()) { 
        // not find, new a service
        Service *service = NULL;
        int thread_num = 5;

		if (service_name == kServiceBondInfo) {
			service = new BondService(conn, kServiceQueueBondInfo, kTopicBondInfo);
        } else if (service_name == kServiceManagement) {
            service = new ManagementService(conn, kServiceQueueManagement, kTopicManagement);
        } else if (service_name == kServiceSync) {
			thread_num = 1;
            service = new SyncService(conn, kServiceQueueSync, kTopicSync);
        } else if (service_name == kServiceProductFavor) {
            service = new ProductFavorService(conn, kServiceQueueProductFavor, kTopicProductFavor);
        }

        if (service == NULL) {
            return NULL;
        }

        if (!service->init(thread_num)) {
			LOGGER_ERROR("init service:" << service_name << ",thread:" << thread_num << " failed!!!");
            delete service;
            return NULL;
		} else {
			LOGGER_DEBUG("init service:" << service_name << ",thread:" << thread_num << " success");
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

    for ( ; it != service_mp_.end(); ++it) {
        Service *service = (*it).second;

        if (service) {
            service->ready();
        }
    }
}