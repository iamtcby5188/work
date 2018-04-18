#include "bondbestquote_shch_ftp_upload_service.h"
#include "business/bondbestquote_shch_ftp_upload_business.h"
#include "db/dbconnector.h"
#include "config.h"
#include "common.h"
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>

BondBestQuoteSHCHFTPUploadService::BondBestQuoteSHCHFTPUploadService()
{

}

BondBestQuoteSHCHFTPUploadService::~BondBestQuoteSHCHFTPUploadService()
{

}

void BondBestQuoteSHCHFTPUploadService::startService()
{
	LOGGER_INFO("Start SH clearing house Bond Best Quote FTP Upload Service...");
	boost::function0<void> f = boost::bind(&BondBestQuoteSHCHFTPUploadService::runBusiness, this);  
	boost::thread thrd(f); 
}

void BondBestQuoteSHCHFTPUploadService::runBusiness()
{
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(30));
	t.async_wait(boost::bind(&BondBestQuoteSHCHFTPUploadService::dispatchUploadCommand, this, boost::asio::placeholders::error, &t));
	io.run();
}

void BondBestQuoteSHCHFTPUploadService::dispatchUploadCommand(const boost::system::error_code& e, boost::asio::deadline_timer* t) {
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

    LOGGER_INFO("Check SH clearing house ftp upload ...")
    Config::singleton()->load("SHCH");
    std::string enable = Config::singleton()->getValue("SHCH.Enable", "false");     
    std::transform(enable.begin(), enable.end(), enable.begin(), ::tolower);  

    if (enable == "true") {
        std::string time_str = Config::singleton()->getValue("SHCH.Time", "16:23");
        std::transform(enable.begin(), enable.end(), enable.begin(), ::tolower);

        std::vector<std::string> time_vec = split(time_str, ';');
        std::vector<std::string>::const_iterator it = time_vec.begin();
        for ( ; it != time_vec.end(); ++it) {
            std::string time = (*it);
            std::vector<std::string> vec = split(time, ':');
            if (vec.size() < 2) {
                continue;
            }
            int hour = atoi(vec[0].c_str());
            int minute = atoi(vec[1].c_str());

            if (now.time_of_day().hours() == hour && now.time_of_day().minutes() == minute) {
                FTP ftp;
                Config::singleton()->load("SHCH.FTP"); 
                ftp.ftpIp = Config::singleton()->getValue("SHCH.FTP.Ip", "1.1.1.1");
                ftp.ftpPort = Config::singleton()->getValue("SHCH.FTP.Port", "21");
                ftp.ftpUsername = Config::singleton()->getValue("SHCH.FTP.UserName", "test");
                ftp.ftpPassword = Config::singleton()->getValue("SHCH.FTP.Password", "test");
                LOGGER_DEBUG(ftp.ftpIp << ":" << ftp.ftpPort << " " << ftp.ftpUsername << "/" << ftp.ftpPassword);

                boost::function0<void> f = boost::bind(&BondBestQuoteSHCHFTPUploadService::uploadThread, this, ftp);  
                boost::thread thrd(f); 
                
            }
        }       
    }

	t->expires_at(t->expires_at() + boost::posix_time::seconds(60));
	t->async_wait(boost::bind(&BondBestQuoteSHCHFTPUploadService::dispatchUploadCommand, this, boost::asio::placeholders::error, t));
}


void BondBestQuoteSHCHFTPUploadService::uploadThread(FTP ftp) {	
	BondBestQuoteSHCHFTPUploadBusiness ftpUploadBusiness(ftp);
    LOGGER_DEBUG("Execute Upload");
	ftpUploadBusiness.startUpload(ftp);
}

