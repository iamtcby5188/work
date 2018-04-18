#include "sdbus/connection.h"
#include "sdbus/dict.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "db/connection_pool.h"
#include "cache/cache_manager.h"
#include "cache/cache_center.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "msg2db/msg_log_service.h"
#include "connection_manager.h"
#include "service/send_service.h"
#include "service/service_manager.h"
#include "datamanager/data_manager.h"
#include "engine/qpid_engine.h"
#include "config.h"
#include <boost/thread.hpp> 

#include "cache/cache_center.h"
#include "cache/cache_monitor.h"

bool init(int argc, char * argv[]) 
{
    ConnPool::getInstance()->init();
	Config::singleton()->load("SDBus");
	sdbus::Connection *conn_recv_publish = ConnectionManager::singleton()->getConnection("ReceivePublish");
	if (conn_recv_publish == NULL) {
		LOGGER_ERROR("create receive publish connection failed!");
		return false;
	}

	sdbus::Connection *conn_process_request = ConnectionManager::singleton()->getConnection();
	if (conn_process_request == NULL) {
		LOGGER_ERROR("create process request connection failed!");
		return false;
	}

	sdbus::Connection *send_conn = ConnectionManager::singleton()->getConnection("Send");
	if (send_conn == NULL) {
		LOGGER_ERROR("create send connection failed!");
		return false;
	}

	// 每个应用send service ret queue必须保证不一样
	Config::singleton()->load("Queue");
	std::string main_queue = Config::singleton()->getValue("Queue.Main", "Queue.Bond.MarketData");
	kServiceSyncLimit = Config::singleton()->getIntValue("Queue.PageLimit", 5000);
	LOGGER_INFO("Send Service return queue: " << main_queue << ", pageLimit:" << kServiceSyncLimit);
	if (!SendService::singleton()->init(send_conn, main_queue)) {
		LOGGER_ERROR("init send service failed!");
		return false;
	}

	/*Config::singleton()->load("FastDB");
	std::string cache_name = Config::singleton()->getValue("FastDB.CacheName", "MarketDataCache");
	std::string cache_size = Config::singleton()->getValue("FastDB.CacheSize", "512");
	std::string cache_lockTimeout = Config::singleton()->getValue("FastDB.waitLockTimeoutMsec", "100");
	std::string cache_threads = Config::singleton()->getValue("FastDB.nThreads", "1");
	std::string cache_scanSize = Config::singleton()->getValue("FastDB.parallelScanThreshold", "1000");
	LOGGER_INFO("FastDB Cache Name: " << cache_name << ", Cache Size: " << cache_size << "M, LockTimeout: "
		<< cache_lockTimeout << "ms, WorkThreads: " << cache_threads << ", parallelScanThreshold: " << cache_scanSize);
	if (!CacheCenter::sharedInstance()->Init(cache_name, atoi(cache_size.c_str()),
		atoi(cache_lockTimeout.c_str()), atoi(cache_threads.c_str()), atoi(cache_scanSize.c_str()))) {
		LOGGER_ERROR("init fastdb failed!");
		return -2;
	}*/

	MsgLogService::singleton()->init(argv[0]);

	CacheManager::singleton()->InitCompanyCache();
	if (!DataManager::singleton()->init(conn_recv_publish)) {
		return false;
	}
    bool isInitData = CacheCenter::sharedInstance()->isInitData(argc, argv);
	if (isInitData)	{
		CacheManager::singleton()->InitCache();
		DataManager::singleton()->reloadData();
	}

	ServiceManager* service_manager = ServiceManager::singleton();
	if (!service_manager->init(conn_process_request)) {
		return false;
	}
	//service_manager->ready(); 

	LOGGER_INFO("********************************************************")
	LOGGER_INFO("----market server start success----")
	LOGGER_INFO("********************************************************")

	return true;
}

int main( int argc, char * argv[] )
{    
    engine::qpid::EngineInitializer qpid_engine ;
    
    engine::qpid::LogLevel(5);

    log4cxx::PropertyConfigurator::configure("log4cxx.properties");

    LOGGER_INITIALIZATION(MarketDataServer);

    Config::singleton()->config("MarketDataServerCfg.xml");
    if (!Config::singleton()->load()) {
        LOGGER_ERROR("read config file failed!");
        return -1;
    }
    
	if (!Config::singleton()->load("FastDB")) {
		LOGGER_ERROR("read fastdb config error!!!");
		return -1;
	}

	if (!CacheCenter::sharedInstance()->Init()) {
		LOGGER_ERROR("init fastdb error!!!");
		return -1;
	}

	CacheCenter::sharedInstance()->Start();
	CacheMonitor::getInstance()->setDataBase(CacheCenter::sharedInstance()->getFastDB());
	CacheMonitor::getInstance()->setSaveFileName(CacheCenter::sharedInstance()->getLogFile());
	CacheMonitor::getInstance()->start();

	boost::this_thread::sleep(boost::posix_time::seconds(1));
	bool isServerable = false;
	if (CacheMonitor::getInstance()->isFastDBEnable()) {
		if (!init(argc, argv)) {
			LOGGER_ERROR("start service error!!!");
			return -1;
		}
		isServerable = true;
	}

	while (1) {
		boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
		if (!isServerable && CacheMonitor::getInstance()->isFastDBEnable()) {
            int sleepTime = Config::singleton()->getIntValue("FastDB.ChangeSleepTime", 0);
            if (sleepTime > 0)
                boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(sleepTime));
            LOGGER_INFO("fastdb status change to active. " << CacheMonitor::getInstance()->getFastDBInfo());
			if (!init(argc, argv)) {
				LOGGER_ERROR("start service error!!!");
				return -1;
			}
			isServerable = true;
		}
	}

    return 0;
}
