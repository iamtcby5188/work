#include <iostream>

#include "sdbus/connection.h"
#include "sdbus/dict.h"
#include "msg2db/msg_log_service.h"
#include "db/dbconnector.h"
#include "service/productfavor_service.h"
#include "engine/qpid_engine.h"
#include "cache/cache_manager.h"
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

    LOGGER_INITIALIZATION(ProductFavorServer);

    Config::singleton()->config("ProductFavorServerCfg.xml");
    if (!Config::singleton()->load()) {
        LOGGER_ERROR("failed to load config file!")
            return -1;
    }

    sdbus::Connection *conn = ConnectionManager::singleton()->getConnection();
    if (conn == NULL) {
        LOGGER_ERROR("failed to create connection!")
        return -1;
    }

    sdbus::Connection *send_conn = ConnectionManager::singleton()->getConnection("Send");
    if (send_conn == NULL) {
        LOGGER_ERROR("failed to create connection sender!")
        return -1;
    }

    // 每个应用send service ret queue必须保证不一样
    if (!SendService::singleton()->init(send_conn, kServiceRetQueueProductFavor)) {
        LOGGER_ERROR("failed to send init service failure!")
        return -1;
    }

	ServiceManager* service_manager = ServiceManager::singleton();  

	if (service_manager->getService(conn, kServiceProductFavor) == NULL) {
		LOGGER_ERROR("failed to create service: " << kServiceProductFavor)
			return -2;
	}
	MsgLogService::singleton()->init(argv[0]);
	CacheManager::singleton()->InitCache();
    
    service_manager->ready();

	LOGGER_INFO("productfavor server start success")

    while (1) {
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }

    return 0;
}