#ifndef _BONDBESTQUOTE_SHCH_FTP_UPLOAD_SERVICE_H_
#define _BONDBESTQUOTE_SHCH_FTP_UPLOAD_SERVICE_H_

#include "logger.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include "business/bondbestquote_shch_ftp_upload_business.h"

class BondBestQuoteSHCHFTPUploadService
{
public:
	BondBestQuoteSHCHFTPUploadService();
	~BondBestQuoteSHCHFTPUploadService();

	void startService();
	void runBusiness();
	void dispatchUploadCommand(const boost::system::error_code& e, boost::asio::deadline_timer* t);
	void uploadThread(FTP ftp);
};

#endif