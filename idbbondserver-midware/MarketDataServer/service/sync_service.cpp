#include "sync_service.h"
#include "sdbus/connection.h"
#include "include/fid.h"
#include "cache/cache_center.h"
#include "cache/cache_manager.h"
#include "service/service_manager.h"
#include "datamanager/data_manager.h"
#include "business/bondquote_business.h"
#include "connection_manager.h"
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include "sync/chinabondvaluation_sync.h"

void SyncService::resetCache(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

    // 每天03:00分自动重新加载所有缓存数据
    if (now.time_of_day().hours() == 3 && now.time_of_day().minutes() >= 30) {
        LOGGER_INFO("Start Reset Cache...");
        CacheCenter::sharedInstance()->clearCache();
		CacheManager::singleton()->InitCache();
		DataManager::singleton()->reloadData();
		LOGGER_INFO("End Reset Cache...");
    }

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1800));
    t->async_wait(boost::bind(resetCache, boost::asio::placeholders::error, t, service));
}

void SyncService::syncChindBondValuation(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
	LOGGER_INFO("Start Sync Chind Bond Valuation...");           

    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

    if (now.time_of_day().hours() > 5) {
        ChinaBondValuationSync chinabondvaluation_sync;
        chinabondvaluation_sync.sync();
    }

	LOGGER_INFO("End Sync China Bond Valuation...");

	t->expires_at(t->expires_at() + boost::posix_time::seconds(1200));
	t->async_wait(boost::bind(syncChindBondValuation, boost::asio::placeholders::error, t, service));
}

SyncService::SyncService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{
    startSync();
}

void SyncService::processReqMsg(const sdbus::Message* msg)
{

}

void SyncService::PublishSync(const char* action_type)
{
    sdbus::Message msg;
    
    msg.SetType(action_type);
    bool flag = conn_->Publish(topic_name_, msg);

    if (flag) {
        LOGGER_INFO("Publish Succeed " << action_type)
    } else {
        LOGGER_ERROR("Publish Failed " << action_type)
    }
}

void SyncService::startSync()
{
    boost::function0<void> f = boost::bind(&SyncService::runSync, this);  
    boost::thread thrd(f); 
}

void SyncService::runSync()
{    
    LOGGER_DEBUG("start sync service!")

    boost::asio::io_service io;

    // 缓存重置
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(585));
    t.async_wait(boost::bind(resetCache, boost::asio::placeholders::error, &t, this));

	// 中债估值同步
	boost::asio::deadline_timer t4(io, boost::posix_time::seconds(120));
	t4.async_wait(boost::bind(syncChindBondValuation, boost::asio::placeholders::error, &t4, this));

    io.run();
}