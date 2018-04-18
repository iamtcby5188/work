#include <iostream>

#include "sdbus/connection.h"
#include "sdbus/dict.h"

#include "db/dbconnector.h"
#include "service/calculator_service.h"
#include "engine/qpid_engine.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "connection_manager.h"
#include "config.h"
#include <boost/thread.hpp>

using bcserver_service_calculator::CalculatorService;

int main(int argc, char * argv[]) {
    std::cout << "starting server..." << std::endl;  
    engine::qpid::EngineInitializer qpid_engine ;

    log4cxx::PropertyConfigurator::configure("log4cxx.properties");

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("BCBond");

    Config::singleton()->config("CalculatorServerCfg.xml");
    if (!Config::singleton()->load()) {
        LOG4CXX_ERROR(logger, "failed to read config file!")
            return -1;
    }

    sdbus::Connection *conn = ConnectionManager::singleton()->getConnection();
    if (conn == NULL) {
        LOG4CXX_ERROR(logger, "failed to connect bus!")
        return -1;
    }

    CalculatorService *calculator = new CalculatorService(conn);
    while (1) {
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }

    conn->DelHandler(calculator);
    delete calculator;
    conn->Close();
    delete conn;

    return 0;
}