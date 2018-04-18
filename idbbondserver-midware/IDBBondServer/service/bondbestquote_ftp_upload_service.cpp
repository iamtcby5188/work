#include "bondbestquote_ftp_upload_service.h"
#include "business/bondbestquote_ftp_upload_business.h"
#include "config.h"
#include "db/dbconnector.h"
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>

BondBestQuoteFTPUploadService::BondBestQuoteFTPUploadService()
{

}

BondBestQuoteFTPUploadService::~BondBestQuoteFTPUploadService()
{

}

void BondBestQuoteFTPUploadService::startService()
{
	LOGGER_INFO("Start Bond Best Quote FTP Upload Service...");
	boost::function0<void> f = boost::bind(&BondBestQuoteFTPUploadService::runBusiness, this);  
	boost::thread thrd(f); 
}

void BondBestQuoteFTPUploadService::runBusiness()
{
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(0));
	t.async_wait(boost::bind(&BondBestQuoteFTPUploadService::dispatchUploadCommand, this, boost::asio::placeholders::error, &t));
	io.run();
}

void BondBestQuoteFTPUploadService::dispatchUploadCommand(const boost::system::error_code& e, boost::asio::deadline_timer* t) {
	LOGGER_INFO("Check china bond ftp upload ...")
    Config::singleton()->load("ChinaBond");
    std::string enable = Config::singleton()->getValue("ChinaBond.Enable", "false");       
    std::transform(enable.begin(), enable.end(), enable.begin(), ::tolower);

    if (enable == "true") {
	    sql::ResultSet* res = DBConnector::getInstance()->executeQuery("select * from idb_ftp where startTime >=0 and endTime >=0 and gapTime >=0 and ftpIp is not null and ftpPort is not null and userStatus = 1", "idb_bond");
	    if (res == NULL) {
		    return;
	    }

	    while (res->next()) {
		    FTP ftp;
		    ftp.ftpId = res->getString("id");
		    ftp.ftpIp = res->getString("ftpIp");
		    ftp.ftpPort = res->getString("ftpPort");
		    ftp.ftpUsername = res->getString("userName");
		    ftp.ftpPassword = res->getString("passWord");
		    ftp.startTime = res->getInt("startTime");
		    ftp.endTime = res->getInt("endTime");
		    ftp.interval = res->getInt("gapTime");

		    boost::function0<void> f = boost::bind(&BondBestQuoteFTPUploadService::uploadThread, this, ftp);  
		    boost::thread thrd(f); 

		    LOGGER_INFO("End dispatchUploadCommand ...")
        }
	    delete res;
    }

	t->expires_at(t->expires_at() + boost::posix_time::seconds(60));
	t->async_wait(boost::bind(&BondBestQuoteFTPUploadService::dispatchUploadCommand, this, boost::asio::placeholders::error, t));
}


void BondBestQuoteFTPUploadService::uploadThread(FTP ftp) {

	
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

	BondBestQuoteFTPUploadBusiness ftpUploadBusiness(ftp);

	// 当天
	if (ftp.startTime < ftp.endTime) {
		int from_minutes = ftp.startTime * 60;
		int to_minutes = ftp.endTime * 60;

		int time_point = now.time_of_day().hours() * 60 + now.time_of_day().minutes();
		if ((time_point - from_minutes) % ftp.interval == 0) {
			LOGGER_DEBUG("Execute Upload");
			ftpUploadBusiness.startUpload(ftp);
		}
	}
	// 跨天 ftp.startTime >= ftp.endTime
	else {
		int from_minutes = ftp.startTime * 60;
		int to_minutes = ftp.endTime * 60 + 24 * 60; // 加一天的时间

		int time_point = now.time_of_day().hours() * 60 + now.time_of_day().minutes();
		if ( time_point < from_minutes) { // 如果小于开始时间，则说明是明天的时间
			time_point += 24 * 60;
		}
		if (from_minutes <= time_point && time_point <= to_minutes) {
			if ((time_point - from_minutes) % ftp.interval == 0) {
				LOGGER_DEBUG("Execute Upload");
				ftpUploadBusiness.startUpload(ftp);
			}
		}
	}
}

