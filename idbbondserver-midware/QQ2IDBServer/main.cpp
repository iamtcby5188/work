#include <iostream>

#include "sdbus/connection.h"
#include "sdbus/message.h"
#include "db/dbconnector.h"
#include "transform_service.h"
#include "engine/qpid_engine.h"

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "connection_manager.h"
#include "config.h"
#include <boost/thread.hpp>
#include "service/send_service.h"

using bcserver_service_transform::Transform_service;

int main(int argc, char * argv[]) {
    
    std::cout << "starting server..." << std::endl;

    engine::qpid::EngineInitializer qpid_engine ;

    log4cxx::PropertyConfigurator::configure("log4cxx.properties");

    LOGGER_INITIALIZATION(QQ2IDBServer);

    Config::singleton()->config("QQ2IDBServerCfg.xml");
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
        LOGGER_ERROR("create send connection failed!");
        return -1;
    }

    // 每个应用send service ret queue必须保证不一样
    Config::singleton()->load("Queue");
    std::string main_queue = Config::singleton()->getValue("Queue.Main", "Queue.Bond.QQ2IDB");
    LOGGER_INFO("Send Service return queue: " << main_queue);   
    if (!SendService::singleton()->init(send_conn, main_queue)) {
        LOGGER_ERROR("init send service failed!");
        return -1;
    }

    Transform_service *t_service = new Transform_service(conn);

    LOGGER_INFO("********************************************************")
    LOGGER_INFO("----start QQ2IDB service succeed. ----")
    LOGGER_INFO("********************************************************")

    while (1) {
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }

    conn->DelHandler(t_service);
    delete t_service;
    delete conn;

    return 0;
}