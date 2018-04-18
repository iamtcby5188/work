#include <iostream>

#include "sdbus/connection.h"
#include "sdbus/dict.h"

#include "db/dbconnector.h"
#include "version_service.h"
#include "engine/qpid_engine.h"

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "connection_manager.h"
#include "config.h"
#include <boost/thread.hpp>

using bcserver_service_version::version_service;

int main(int argc, char * argv[]) {
    std::cout << "starting server..." << std::endl;
    engine::qpid::EngineInitializer qpid_engine ;

    log4cxx::PropertyConfigurator::configure("log4cxx.properties");

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("BCBond");

    Config::singleton()->config("VersionServerCfg.xml");
    if (!Config::singleton()->load()) {
        LOG4CXX_ERROR(logger, "配置文件读取失败！")
            return -1;
    }

    sdbus::Connection *conn = ConnectionManager::singleton()->getConnection();

    if (conn == NULL) {
        LOG4CXX_ERROR(logger, "连接总线失败！")
        return -1;
    }

    version_service *v_service = new version_service(conn);

    while (1) {
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
        //std::cout << boost::get_system_time().time_of_day().total_seconds() << std::endl;
    }

    conn->DelHandler(v_service);
    delete v_service;
    conn->Close();
    delete conn;

    return 0;
}