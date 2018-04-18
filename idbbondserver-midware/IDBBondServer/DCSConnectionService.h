#ifndef _DCS_CONNECTION_SERVICE_H_
#define _DCS_CONNECTION_SERVICE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif

#include "cache/model/dcsbonddeal_cache.h"
#include "object/DcsHistoryDetail.h"
#include "qpid/messaging/Connection.h"
#include <boost/lexical_cast.hpp>

#define BOOST_SPIRIT_THREADSAFE //用于解决read_json崩溃问题
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include "qpid/messaging/Session.h"
#include "qpid/messaging/Message.h"
using namespace qpid::messaging;

static const std::string DOUBLE_DEFAULT_PRECISION = "4";
static const std::string DOUBLE_MAX_PRECISION = "8";

class DCSConnectionService
{
public:
	static DCSConnectionService* singleton();

	~DCSConnectionService();

	void InitDcsParams();
	bool Open(const std::string& url);
	void Close();
	void StartReceiveThread();

	inline const std::string& ServiceName() const { return _service_name; }
	inline void setServiceName(const std::string& name) { _service_name = name; }
	inline const std::string& ServiceNameForReciever() const { return _service_name_for_recieve; }
	inline void setServiceNameForReciever(const std::string& name) { _service_name_for_recieve = name; }
	inline const std::string& ProductName() const { return _product_name; }
	inline void setProductName(const std::string& name) { _product_name = name; }
	inline const int& CacheDataMonths() const { return _cache_data_months; }
	inline void setCacheDataMonths(const std::string& months) { _cache_data_months = boost::lexical_cast<int>(months); }
	inline const std::string& DateType() const { return _date_type; }
	inline void setDateType(const std::string& type) { _date_type = type; }

	bool queryContractList(DcsBondDealCacheVecPtr& deal_vec, int& total, int page_num, int page_size);
	bool addContractList(const DcsBondDealCache* cache, std::string account_name);
	bool updateContractList(const DcsBondDealCache* cache, std::string account_name);
	bool updateContractListSilent(const DcsBondDealCache* cache, std::string account_name);
	bool deleteContractList(std::string id_str, std::string account_name);
	bool updateContractConfirmStatus(std::string id, int bid_ofr, bool status, std::string& deal_status, std::string account_name);
	bool submitContractList(std::string id_str, std::string account_name);
	bool relateSubmitContractList(std::string id_str, std::string comments, std::string account_name);
	bool destroyContract(std::string id, std::string account_name, const std::string& reason);
	bool setContractListUrgentStatus(std::string id_str, std::string urgent_status, std::string account_name);
	bool getSwitch();
	bool getDcsDealLog(DcsHistoryDetailVecPtr& detail_vec, std::string id);

private:
	DCSConnectionService();
	bool parseJson(const std::string& json_str, ptree& pt);
	std::string GenerateJson(const ptree& pt, bool isSubList = false);
	bool send(const ptree& request, ptree& response, Duration timeout = Duration(10 * 1000));
	void threadDoRecieveMsgs();
	bool recieve();
	void handleFailureMsg(const ptree& response);
	void handleAddMsg(const ptree& response);
	void handleUpdateMsg(const ptree& response);
	void handleSwitchMsg(const ptree& response);

	void dcsToDeal(const ptree& pt, DcsBondDealCacheVecPtr& deal_vec);
	void dealToDcs(const DcsBondDealCache* cache, ptree& pt);
	void dcsToHistoryDetail(const ptree& pt, DcsHistoryDetailVecPtr& detail_vec);
	void translateUpdateContent(const ptree& pt, std::string &update_content);
	void initOperateMap();
	void initUpdateContentMap();

	std::string real8_format(real8 value, std::string percision = DOUBLE_DEFAULT_PRECISION);
	std::string truncateDouble(real8 value, std::string percision);

	void write_json(std::basic_ostream<ptree::key_type::value_type> &stream, const ptree &pt, bool pretty);
	void write_json_helper(std::basic_ostream<ptree::key_type::value_type> &stream, const ptree &pt, int indent, bool pretty);
	std::basic_string<char> create_escapes(const std::basic_string<char> &s);

private:
	std::string _service_name;
	std::string _service_name_for_recieve;
	std::string _product_name;
	int _cache_data_months;
	std::string _date_type;
	bool _is_open;
	qpid::messaging::Connection* _connection;
    int timeout;

	static std::map<std::string, std::string> _operate_map;
	static std::map<std::string, std::string> _update_content_map;
	static DCSConnectionService* singleton_;
    
};

#endif
