#ifndef _BONDBESTQUOTE_SHCH_FTP_UPLOAD_BUSINESS_H_
#define _BONDBESTQUOTE_SHCH_FTP_UPLOAD_BUSINESS_H_

#include "ftpClient.h"
#include <string>
#include "cache/controller/bondbestquote_cachecontroller.h"

class BondBestQuoteSHCHFTPUploadBusiness {
public:
    BondBestQuoteSHCHFTPUploadBusiness(FTP& ftp);
    ~BondBestQuoteSHCHFTPUploadBusiness();
    void startUpload(FTP ftp);
    bool generateCSV();
    void prepareBondBestQuote(std::ofstream& csvfile);
private:
    FTP _ftp;
    std::string _csv_file_name;

};


#endif // _BONDBESTQUOTE_SHCH_FTP_UPLOAD_BUSINESS_H_