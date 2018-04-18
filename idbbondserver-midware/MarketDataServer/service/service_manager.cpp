#include "service_manager.h"
#include "service/service.h"
#include "sdbus/connection.h"
#include "include/constants.h"
#include "bondbestquote_service.h"
#include "bondcompletedquote_service.h"
#include "bonddeal_service.h"
#include "dcsbonddeal_service.h"
#include "bondquote_service.h"
#include "heartbeat_service.h"
#include "sync_service.h"
#include "connection_manager.h"

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
    if (getService(conn, kServiceBondDeal) == NULL) {
        LOGGER_ERROR("create service failed: " << kServiceBondDeal);
        return false;
    }
	if (getService(conn, kServiceDcsBondDeal) == NULL) {
		LOGGER_ERROR("create service failed: " << kServiceDcsBondDeal);
		return false;
	}
    if (getService(conn, kServiceBondQuote) == NULL) {
        LOGGER_ERROR("create service failed: " << kServiceBondQuote);
        return false;
    }
    if (getService(conn, kServiceBondBestQuote) == NULL) {
        LOGGER_ERROR("create service failed: " << kServiceBondBestQuote);
        return false;
    }
    if (getService(conn, kServiceBondCompletedQuote) == NULL) {
        LOGGER_ERROR("create service failed: " << kServiceBondCompletedQuote);
        return false;
    }
    if (getService(conn, kServiceSync) == NULL) {
        LOGGER_ERROR("create service faield in basic data server: " << kServiceSync);
        return false;
    }
    if (getService(conn, kServiceHeartBeat) == NULL) {
        LOGGER_ERROR("create service failed: " << kServiceHeartBeat);
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
        int thread_num = 1;

        if (service_name == kServiceBondBestQuote) {
            service = new BondBestQuoteService(conn, "Queue.MarketData.Bond.BestQuote", kTopicBondBestQuote);
            thread_num = 10;
        } else if (service_name == kServiceBondCompletedQuote) {
            service = new BondCompletedQuoteService(conn, "Queue.MarketData.Bond.CompletedQuote", kTopicBondCompletedQuote);
            thread_num = 10;
        } else if (service_name == kServiceBondDeal) {
            service = new BondDealService(conn, "Queue.MarketData.Bond.Deal", kTopicBondDeal);
            thread_num = 10;
		}
		else if (service_name == kServiceDcsBondDeal) {
			service = new DcsBondDealService(conn, "Queue.MarketData.Dcs.Bond.Deal", kTopicDcsBondDeal);
			thread_num = 10;
        } else if (service_name == kServiceBondQuote) {
            service = new BondQuoteService(conn, "Queue.MarketData.Bond.Quote", kTopicBondQuote);
            thread_num = 10;
        } else if (service_name == kServiceSync) {
            service = new SyncService(conn, kServiceQueueSync, kTopicSync);
        }  else if (service_name == kServiceHeartBeat) {
            service = new HeartBeatService(conn, kServiceQueueHeartBeat, kTopicHeartBeat);
        } 

        if (service == NULL) {
            return NULL;
        }

        if (!service->init(thread_num)) {

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

    for ( ; it != service_mp_.end(); ++it) {
        Service *service = (*it).second;

        if (service) {
            service->ready();
        }
    }
}
