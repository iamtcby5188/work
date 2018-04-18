#ifndef _BONDBESTQUOTE_FTP_UPLOAD_SERVICE_H_
#define _BONDBESTQUOTE_FTP_UPLOAD_SERVICE_H_

#include "logger.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include "business/bondbestquote_ftp_upload_business.h"

class BondBestQuoteFTPUploadService
{
public:
	BondBestQuoteFTPUploadService();
	~BondBestQuoteFTPUploadService();

	void startService();
	void runBusiness();
	void dispatchUploadCommand(const boost::system::error_code& e, boost::asio::deadline_timer* t);
	void uploadThread(FTP ftp);
};

#endif