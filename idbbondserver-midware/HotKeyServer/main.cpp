#include <iostream>

#include "sdbus/connection.h"
#include "sdbus/dict.h"

#include "db/dbconnector.h"
#include "service/hotkeyservice.h"
#include "engine/qpid_engine.h"
#include "cache/cache_manager.h"
#include "cache/cache_center.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "connection_manager.h"
#include "service/send_service.h"
#include "service/service_manager.h"
#include "config.h"
#include "include/constants.h"
#include <boost/thread.hpp>

int main(int argc, char * argv[]) {
    std::cout << "starting server..." << std::endl;  
    engine::qpid::EngineInitializer qpid_engine ;

    log4cxx::PropertyConfigurator::configure("log4cxx.properties");

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("BCBond");

    Config::singleton()->config("HotKeyServerCfg.xml");
    if (!Config::singleton()->load()) {
        LOG4CXX_ERROR(logger, "failed to read config file in HotKeyServer!")
            return -1;
    }
    sdbus::Connection *conn = ConnectionManager::singleton()->getConnection();
    if (conn == NULL) {
        LOG4CXX_ERROR(logger, "failed to create main connection in HotKeyServer!")
        return -1;
    }

    sdbus::Connection *send_conn = ConnectionManager::singleton()->getConnection("Send");
    if (send_conn == NULL) {
        LOG4CXX_ERROR(logger, "failed to create send connection in HotKeyServer!")
        return -1;
    }

    // 每个应用send service ret queue必须保证不一样
    if (!SendService::singleton()->init(send_conn, kServiceRetQueueHotKey)) {
        LOG4CXX_ERROR(logger, "failed to init send service!")
        return -1;
    }

	ServiceManager* service_manager = ServiceManager::singleton();  

    if (service_manager->getService(conn, kServiceHotKey) == NULL) {
        LOG4CXX_ERROR(logger, "failed to create service: " << kServiceHotKey)
        return -2;
    }

	CacheManager::singleton()->InitCache();

    service_manager->ready();

	LOG4CXX_INFO(logger, "start service success!")

    while (1) {
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }

    return 0;
}