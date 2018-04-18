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
#include "cache/cache_controller_declear.h"
#include "increment/increment_cache_manager.h"
void SyncService::resetCache(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* service)
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

    // 每天02:00分自动Refer所有报价
    if (now.time_of_day().hours() == 2 && now.time_of_day().minutes() < 20) {
        LOGGER_INFO("Start Refer All Quotes...")

		/* Modify by Young 14-Apr-2016 */
		SystemCompanyCacheController system_company_cc;
		dbQuery q("");
		SystemCompanyCacheVecPtr system_company_cache_vec = system_company_cc.getCacheListByQueryInThreadSafty(q);
		if (NULL != system_company_cache_vec)
		{
			std::vector<SystemCompanyCachePtr>::const_iterator it = system_company_cache_vec->begin();
			for ( ; it != system_company_cache_vec->end(); ++it) 
			{
				SystemCompanyCache* system_company_cache = it->get();

				BondQuoteService *bondQuoteService = (BondQuoteService*)ServiceManager::singleton()->getService("BondQuote");
				BondQuoteBusiness bondquote_business(bondQuoteService);
				bondquote_business.SetCompanyId(system_company_cache->id);
				bondquote_business.ReferBondQuoteAll();
			}
		}
		LOGGER_INFO("End Refer All Quotes...")
    }

    // 每天02:40分自动重新加载所有缓存数据
    if (now.time_of_day().hours() == 2 && now.time_of_day().minutes() >= 40) {
        LOGGER_INFO("Start Reset Cache...");
        CacheCenter::sharedInstance()->clearCache();
        CacheManager::singleton()->InitCompanyCache();
        DataManager::singleton()->reloadData();
        CacheManager::singleton()->InitCache();
        LOGGER_INFO("End Reset Cache...")
    }

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1200));
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

void SyncService::startSync()
{
    boost::function0<void> f = boost::bind(&SyncService::runSync, this);  
    boost::thread thrd(f); 
}

void SyncService::runSync()
{    
    LOGGER_DEBUG("start sync thread")

    boost::asio::io_service io;

    // 缓存重置
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(585));
    t.async_wait(boost::bind(resetCache, boost::asio::placeholders::error, &t, this));

	// 中债估值同步
	boost::asio::deadline_timer t4(io, boost::posix_time::seconds(120));
	t4.async_wait(boost::bind(syncChindBondValuation, boost::asio::placeholders::error, &t4, this));

    io.run();
}