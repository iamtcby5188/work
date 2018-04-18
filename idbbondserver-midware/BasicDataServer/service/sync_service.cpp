#include "sync_service.h"
#include "sdbus/connection.h"
#include "include/fid.h"
#include "cache/cache_center.h"
#include "cache/cache_manager.h"
#include "sync/bondsmallview_sync.h"
#include "sync/account_sync.h"
#include "service/service_manager.h"
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include "sync/institutionrating_sync.h" //Add by Young 20-Apr-2016
#include "increment/increment_cache_manager.h"
void SyncService::resetCache(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

    // 每天01:00-01:30分自动重新加载所有缓存数据
    if (now.time_of_day().hours() == 1 && now.time_of_day().minutes() < 30) {
        LOGGER_INFO("Start Reset Cache...");
        CacheCenter::sharedInstance()->clearCache();
        CacheManager::singleton()->InitCache();
        LOGGER_INFO("End Reset Cache...");
    }

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1800));
    t->async_wait(boost::bind(resetCache, boost::asio::placeholders::error, t, service));
}

void SyncService::syncBond(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
    LOGGER_INFO("Start Sync Bond...")        
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();    

    if (now.time_of_day().hours() > 5) {
        BondSmallViewSync bondsmallview_sync;
        bondsmallview_sync.sync();
    }
            
    LOGGER_INFO("End Sync Bond...")

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1200));
    t->async_wait(boost::bind(syncBond, boost::asio::placeholders::error, t, service));
}

void SyncService::syncAccount(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
    LOGGER_INFO("Start Sync Account...")
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();    
    // 0点resetCache，故不同步
    if (now.time_of_day().hours() > 5) {
        AccountSync account_sync;
        account_sync.sync();
    }

    LOGGER_INFO("End Sync Account...")

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1200));
    t->async_wait(boost::bind(syncAccount, boost::asio::placeholders::error, t, service));
}
/*
void SyncService::syncChindBondValuation(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
	LOGGER_INFO("Start Sync Chind Bond Valuation...")           

	ChinaBondValuationSync chinabondvaluation_sync;
	chinabondvaluation_sync.sync();

	LOGGER_INFO("End Sync China Bond Valuation...")

	t->expires_at(t->expires_at() + boost::posix_time::seconds(300));
	t->async_wait(boost::bind(syncChindBondValuation, boost::asio::placeholders::error, t, service));
}*/

/* Add by Young 20-Apr-2016
 * 发行人评级同步 
*/
void SyncService::syncInstitutionRating(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
    LOGGER_INFO("Start Sync Institution Rating...");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

    if (now.time_of_day().hours() > 5) {
        InstitutionRatingSync institutionRating_sync;
        institutionRating_sync.sync();
    }

	LOGGER_INFO("End Sync Institution Rating...")

	t->expires_at(t->expires_at() + boost::posix_time::seconds(1200));
	t->async_wait(boost::bind(syncInstitutionRating, boost::asio::placeholders::error, t, service));
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

    // reset cache
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(585));
    t.async_wait(boost::bind(resetCache, boost::asio::placeholders::error, &t, this));

    // 债券同步
    boost::asio::deadline_timer t2(io, boost::posix_time::seconds(240));
    t2.async_wait(boost::bind(syncBond, boost::asio::placeholders::error, &t2, this));

    // Account同步
    boost::asio::deadline_timer t3(io, boost::posix_time::seconds(120));
    t3.async_wait(boost::bind(syncAccount, boost::asio::placeholders::error, &t3, this));

	// 中债估值同步
	//boost::asio::deadline_timer t4(io, boost::posix_time::seconds(120));
	//t4.async_wait(boost::bind(syncChindBondValuation, boost::asio::placeholders::error, &t4, this));

	//Add by Young 20-Apr-2016
	//发行人评级同步 
	boost::asio::deadline_timer t5(io, boost::posix_time::seconds(160));
	t5.async_wait(boost::bind(syncInstitutionRating, boost::asio::placeholders::error, &t5, this));

    io.run();
}
