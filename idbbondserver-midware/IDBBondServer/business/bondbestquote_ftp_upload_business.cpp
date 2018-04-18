#include "bondbestquote_ftp_upload_business.h"
#include "db/dbconnector.h"
#include "common.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <logger.h>
#ifdef _WIN32
#include <direct.h>
#endif


BondBestQuoteFTPUploadBusiness::BondBestQuoteFTPUploadBusiness(FTP& ftp)
{
	_ftp = ftp;
	int status;
	_csv_file_name = "XQBJ.csv";

#ifdef _WIN32
	mkdir("ftp_upload");
	_path = std::string("ftp_upload\\") + _ftp.ftpId;
	_csv_file_path = _path + "\\" + _csv_file_name;
	status = mkdir(_path.c_str());
#else
	mkdir("ftp_upload", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	_path = std::string("ftp_upload/") + _ftp.ftpId;
	_csv_file_path = _path + "/" + _csv_file_name;
	status = mkdir(_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

BondBestQuoteFTPUploadBusiness::~BondBestQuoteFTPUploadBusiness()
{

}

void BondBestQuoteFTPUploadBusiness::prepareBondBestQuote(FTP_RULES ftpRules, std::ofstream& csvfile) {
	// hard coded properties
	std::string listed_market = "CIB";
	std::string status = "1";
	std::string having_external_quote = "Y";

	BondBestQuoteCacheController bondBestQuoteCacheController;

	std::string today = GetTDateString("%Y-%m-%d");

	dbQuery q;
	q = "bond_type = ", ftpRules.goodsTypeCode,
		" and listed_market = ", listed_market,
		" and status = ", status,
		" and having_external_quote = ", having_external_quote,
		" and (ofr_trader_id != ", "",
		" or bid_trader_id != ", "",
		" ) and create_time > ", today , 
		" order by bond_type";

	BondBestQuoteCacheVecPtr bbqCacheVec = bondBestQuoteCacheController.getCacheListByQueryInThreadSafty(q);
	LOGGER_INFO(ftpRules.goodsTypeCode << "  " << ftpRules.goodsTypeId << "  " << bbqCacheVec->size());
    for (std::vector<BondBestQuoteCachePtr>::const_iterator iter = bbqCacheVec->begin(); iter != bbqCacheVec->end(); ++iter) {

		std::ostringstream bid_price;
		std::ostringstream bid_yield;
		if (iter->get()->bid_price >= 30) {
			bid_price << iter->get()->bid_price - ftpRules.bidFloat;
		}
		else if (iter->get()->bid_price != DOUBLE_NULL) {
			bid_yield << iter->get()->bid_price + ftpRules.bidFloat;
		}

		std::ostringstream ofr_price;
		std::ostringstream ofr_yield;
		if (iter->get()->ofr_price >= 30) {
			ofr_price << iter->get()->ofr_price + ftpRules.ofrFloat;
		}
		else if (iter->get()->ofr_price != DOUBLE_NULL) {
			ofr_yield << iter->get()->ofr_price - ftpRules.ofrFloat;
		}
		
		std::string bid_volume = "";
		if (iter->get()->bid_volume.find("--") == -1) {
			bid_volume = iter->get()->bid_volume;
		}

		std::string ofr_volume = "";
		if (iter->get()->ofr_volume.find("--") == -1) {
			ofr_volume = iter->get()->ofr_volume;
		}

		std::pair<std::string, std::string> maturity_term_pair = CalculateMaturityTerm(iter->get()->maturity_date, iter->get()->option_date);

		csvfile << today << ", "								//报价日期
				<< iter->get()->create_time << ", "				//买入报价时间
				<< iter->get()->create_time << ", "				//卖出报价时间
				<< iter->get()->goods_code << ", "				//债券代码
				<< maturity_term_pair.first << ", "				//待偿期
				<< maturity_term_pair.second << ", "			//期限单位
				<< "10" << ", "									//编码机构代码
				<< "2" << ", "									//清算速度
				<< bid_price.str() << ", "						//买入净价(元)
				<< bid_yield.str() << ", "						//买入净价收益率(%)
				<< ofr_price.str() << ", "						//卖出净价(元)
				<< ofr_yield.str() << ", "						//卖出净价收益率(％)
				<< bid_volume << ", "							//买入券面总额(万元)
				<< ofr_volume << ", "							//卖出券面总额(万元)
				<< "0" << ", "									//收益率类别
				<< std::endl;									
	}
}

bool BondBestQuoteFTPUploadBusiness::generateCSV() {
	LOGGER_INFO("generate best quote csv file ...")

	sql::ResultSet* res = DBConnector::getInstance()->executeQuery("select goodsTypeId, bidFloat, ofrFloat, code from idb_ftprules inner join bond_goods_type on (idb_ftprules.goodsTypeId = bond_goods_type.id and bond_goods_type.status = 1) where ftpid = \"" + this->_ftp.ftpId + "\"", "idb_bond");
	if (res == NULL) {
		return false;
	}

	// csv writter
	std::ofstream csvfile(_csv_file_path);
	if (!csvfile.is_open()) {
		LOGGER_ERROR("Fail to open create csvfile");
	}
	csvfile << "报价日期,买入报价时间,卖出报价时间,债券代码,待偿期,期限单位,编码机构代码,清算速度,买入净价(元),买入净价收益率(%),卖出净价(元),卖出净价收益率(％),买入券面总额(万元),卖出券面总额(万元),收益率类别" << std::endl;

	while (res->next()) {
		FTP_RULES ftpRules;
		ftpRules.goodsTypeId = res->getString("goodsTypeId");
		ftpRules.bidFloat = res->getInt("bidFloat");
		ftpRules.ofrFloat = res->getInt("ofrFloat");
		ftpRules.goodsTypeCode = res->getString("code");
		BondTypeSelectiveCacheController bondTypeSelectiveCacheController;
		dbQuery q;
		q = "type_id = ", ftpRules.goodsTypeId;
		int count = bondTypeSelectiveCacheController.getSizeByQueryInThreadSafty(q);
		if (count > 0) {
			this->prepareBondBestQuote(ftpRules, csvfile);
		}
	}

	csvfile.close();

	delete res;

	LOGGER_INFO("generate best quote csv file complete.");
	return true;
}

void BondBestQuoteFTPUploadBusiness::startUpload(FTP ftp)
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
			if(strstr(_csv_file_path.c_str(), "ftp_upload") == NULL)
			{
                LOGGER_ERROR("file is invalid!");
				return;
			}

			ftpclient.UploadFile(this->_csv_file_path);
		}
	}
	else
	{
		LOGGER_INFO("generateCSV failed");
	}

}
