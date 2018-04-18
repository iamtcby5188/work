#include "bondbestquote_shch_ftp_upload_business.h"

BondBestQuoteSHCHFTPUploadBusiness::BondBestQuoteSHCHFTPUploadBusiness(FTP& ftp)
{
	std::string today = GetTDateString("%Y-%m-%d");
	_ftp = ftp;
	_csv_file_name = "Offer" + today + ".csv";
}

BondBestQuoteSHCHFTPUploadBusiness::~BondBestQuoteSHCHFTPUploadBusiness()
{

}

void BondBestQuoteSHCHFTPUploadBusiness::prepareBondBestQuote(std::ofstream& csvfile) {
	// hard coded properties
	std::string listed_market = "CIB";
    std::string market_type = "2";
	std::string status = "1";
	std::string having_external_quote = "Y";

	BondBestQuoteCacheController bondBestQuoteCacheController;

	std::string today = GetTDateString("%Y-%m-%d");

	dbQuery q;
    q = "status = ", status,
		" and having_external_quote = ", having_external_quote,
		" and (ofr_trader_id != ", "",
		" or bid_trader_id != ", "",
		" ) and create_time > ", today , 
		" order by create_time asc";

	BondBestQuoteCacheVecPtr bbqCacheVec = bondBestQuoteCacheController.getCacheListByQueryInThreadSafty(q);
	LOGGER_INFO("shanghai clearing house best qutoe size: " << bbqCacheVec->size());
    for (std::vector<BondBestQuoteCachePtr>::const_iterator iter = bbqCacheVec->begin(); iter != bbqCacheVec->end(); ++iter) {
        if ((*iter)->bid_yield == DOUBLE_NULL && (*iter)->bid_net_price == DOUBLE_NULL && (*iter)->ofr_yield == DOUBLE_NULL && (*iter)->ofr_net_price == DOUBLE_NULL) {
            continue;
        }

		csvfile << "" << ","								    //序号
				<< (*iter)->goods_code << ","				    //代码
				<< Utf8ToAnsi((*iter)->bond_short_name) << ","	//名称
				<< Format((*iter)->ttm, 2, 4) + "Y" << ","		//剩余期限
				<< ((*iter)->create_time.length() >=8 ? (*iter)->create_time.substr((*iter)->create_time.length() - 8, 8) : "") << ","     //报价时间
				<< (*iter)->bid_institution_id << ","			//报价方(bid)
				<< "" << ","									//速度
				<< "" << ","									//券面总额
				<< ((*iter)->bid_net_price != DOUBLE_NULL ? Format((*iter)->bid_net_price, 2, 4) : "") << ","	//净价(bid)
				<< ((*iter)->bid_yield != DOUBLE_NULL ? Format((*iter)->bid_yield, 2, 4) : "") << ","			//收益率(bid)
				<< "" << ","									//净价差额
				<< "" << ","									//收益率差(BP)
				<< ((*iter)->ofr_yield != DOUBLE_NULL ? Format((*iter)->ofr_yield, 2, 4) : "") << ","			//收益率(ofr)
				<< ((*iter)->ofr_net_price != DOUBLE_NULL ? Format((*iter)->ofr_net_price, 2, 4) : "") << ","	//净价(ofr)
				<< "" << ","									//券面总额
				<< "" << ","									//速度
				<< (*iter)->ofr_institution_id << ","			//报价方(bid)
				<< "" << ","									//日期
				<< "" << ","									//净价
				<< "" << ","									//全价
				<< "" << ","									//收益率(%)
				<< "" << ","									//票面利率/参考利率
				<< "" << ","									//债券评级
				<< "" << ","									//主体评级
				<< std::endl;									
	}
}

bool BondBestQuoteSHCHFTPUploadBusiness::generateCSV() {
	LOGGER_INFO("generate shanghai clearing house best quote csv file ...")

	// csv writter
	std::ofstream csvfile(_csv_file_name);
	if (!csvfile.is_open()) {
		LOGGER_ERROR("Fail to open create csvfile");
	}
	csvfile << "序号,代码,名称,剩余年限,报价时间,报价方,速度,券面总额(万元),净价,收益率,净价差额,收益率差(BP),收益率,净价,券面总额(万元),速度,报价方,日期,净价,全价,收益率(%),票面利率/参考利率(%),债券评级,主体评级" << std::endl;

    this->prepareBondBestQuote(csvfile);

	csvfile.close();

	LOGGER_INFO("generate shanghai clearing house best quote csv file complete.");
	return true;
}

void BondBestQuoteSHCHFTPUploadBusiness::startUpload(FTP ftp)
{
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	LOGGER_INFO(ftp.startTime << " - " << ftp.endTime << ": " << ftp.interval);
	LOGGER_INFO("Strating FTP ..." << now.time_of_day().hours() << ":" << now.time_of_day().minutes());

	if (generateCSV()) {
		//std::string system_cmd = "python ftphandler.py [" + ftp.ftpIp + "] [" + ftp.ftpPort + "] [" + ftp.ftpUsername + "] [" + ftp.ftpPassword + "] [" + this->_path + "] [" + this->_csv_file_name + "]";
		//system(system_cmd.c_str());
		
		ftp_client ftpclient;
		if(ftp.ftpUsername == "" || ftp.ftpUsername == NULL){
			ftp.ftpUsername = "anonymous";
			ftp.ftpPassword = "";
		}

		bool bret = ftpclient.connect(ftp.ftpIp,ftp.ftpPort,ftp.ftpUsername,ftp.ftpPassword);

		bool blogin = ftpclient.logined();
		if(blogin){
			ftpclient.UploadFile(this->_csv_file_name);
		}
	}
	else
	{
		LOGGER_INFO("generateCSV failed");
	}

}

