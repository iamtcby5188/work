#ifndef _BONDBESTQUOTE_FTP_UPLOAD_BUSINESS_H_
#define _BONDBESTQUOTE_FTP_UPLOAD_BUSINESS_H_

#include "ftpClient.h"
#include <string>
#include "cache/controller/bondbestquote_cachecontroller.h"

class BondBestQuoteFTPUploadBusiness {
public:
    BondBestQuoteFTPUploadBusiness(FTP& ftp);
    ~BondBestQuoteFTPUploadBusiness();
    void startUpload(FTP ftp);
    bool generateCSV();
    void prepareBondBestQuote(FTP_RULES ftpRules, std::ofstream& csvfile);
private:
    FTP _ftp;
    std::string _path;
    std::string _csv_file_path;
    std::string _csv_file_name;

};

#endif
