#include "DCSConnectionService.h"
#include "config.h"
#include "include/common.h"
#include "business/dcsbonddeal_business.h"
#include "service/service_manager.h"
#include "qpid/messaging/Session.h"
#include "qpid/messaging/Receiver.h"
#include "qpid/messaging/Sender.h"
#include "qpid/messaging/Address.h"
#include "qpid/messaging/Message.h"	
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time.hpp> 
#include <boost/regex.hpp>

using namespace boost::posix_time;

std::map<std::string, std::string> DCSConnectionService::_operate_map;
std::map<std::string, std::string> DCSConnectionService::_update_content_map;
DCSConnectionService* DCSConnectionService::singleton_ = NULL;

DCSConnectionService* DCSConnectionService::singleton()
{
	if (singleton_ == NULL) 
	{
		singleton_ = new DCSConnectionService();
	}

	return singleton_;
}

DCSConnectionService::DCSConnectionService(void)
	: _service_name("")
	, _service_name_for_recieve("")
	, _product_name("BOND")
	, _cache_data_months(6)
	, _date_type("")
	, _is_open(false)
	, _connection(NULL)
{
	initOperateMap();
	initUpdateContentMap();
}

DCSConnectionService::~DCSConnectionService(void)
{
	Close();
}

void DCSConnectionService::InitDcsParams()
{
	std::string send_service = Config::singleton()->getValue("DCS.SendQueue", "bond.toDcs.messageSend");
	setServiceName(send_service);

	std::string recv_service = Config::singleton()->getValue("DCS.ReceiveQueue", "dcs.toBond.messageSend");
	setServiceNameForReciever(recv_service);

	std::string product_name = Config::singleton()->getValue("DCS.ProductName", "BOND");
	setProductName(product_name);

	std::string cache_data_months = Config::singleton()->getValue("DCS.CacheDataMonths", "6");
	setCacheDataMonths(cache_data_months);

	std::string date_type = Config::singleton()->getValue("DCS.DateType", "createTime");
	setDateType(date_type);

	std::string page_size = Config::singleton()->getValue("DCS.PageSize", "5000");
	kDcsPageSize = boost::lexical_cast<int>(page_size);

    timeout = Config::singleton()->getIntValue("DCS.Timeout", 60) * 1000;
}

bool DCSConnectionService::Open(const std::string& url)
{
	LOGGER_DEBUG("Open DCS connection Service url[ " << url << " ]");

	try
	{
		_is_open = true;
		kDcsEnable = true;

		_connection = new Connection(url);
		_connection->setOption("reconnect", true);
		_connection->setOption("heartbeat", Config::singleton()->getIntValue("DCS.HeartBeat", 5));
		_connection->open();
	}
	catch (const std::exception& error)
	{
		LOGGER_ERROR("exception raise, when Open DCS connection, errmsg[ " << error.what() << " ]");
	}

	return _connection->isOpen();
}

void DCSConnectionService::Close()
{
	try
	{
		_is_open = false;

		if (_connection != NULL)
		{
			_connection->close();

			delete _connection;
			_connection = NULL;
		}
	}
	catch (const std::exception& error)
	{
		LOGGER_ERROR("exception raise, when Close DCS connection, errmsg[ " << error.what() << " ]");
	}
}

void DCSConnectionService::StartReceiveThread()
{
	boost::function0<void> f1 = boost::bind(&DCSConnectionService::threadDoRecieveMsgs, this);
	boost::thread thrd1(f1);
}

void DCSConnectionService::threadDoRecieveMsgs()
{
	while (_is_open)
	{
		recieve();
	}
}

bool DCSConnectionService::recieve() 
{
	if (_connection == NULL || !_connection->isOpen()) return false;

	Session session = _connection->createSession();
	std::string addr = _service_name_for_recieve + "; {create:always , node:{type:queue}}";
	Receiver receiver = session.createReceiver(addr);

	bool success = true;
	try
	{
		Message recieveMsg;
		while (receiver.fetch(recieveMsg, Duration::SECOND))
		{
			ptree pt;
			parseJson(recieveMsg.getContent(), pt);
			std::string method = "";
			if (!pt.empty() && pt.find("method") != pt.not_found())
			{
				method = pt.get<std::string>("method");
			}

			ptime bt1(microsec_clock::local_time());
			LOGGER_INFO("recieve Start processing message...... [Type]: " << method << " begin:[" << to_simple_string(microsec_clock::local_time().time_of_day()) << "]");

			if ("fail" == method)
			{
				handleFailureMsg(pt);
			}
			else if ("update" == method)
			{
				handleUpdateMsg(pt);
			}
			else if ("add" == method)
			{
				//handleAddMsg(pt); //录入成交单时，不处理DCS推送的成交单
			}
			else if ("delete" == method)
			{
				
			}
			else if ("switch" == method)
			{
				handleSwitchMsg(pt);
			}

			session.acknowledge();

			ptime bt2(microsec_clock::local_time());
			LOGGER_INFO("recieve End  processing message. [Type]: " << method << " [Time costing]: " <<
				"[" << to_simple_string(bt2.time_of_day()) << "]-[" << to_simple_string(bt1.time_of_day()) << "]= " << (bt2.time_of_day() - bt1.time_of_day()).total_milliseconds() << " ms");
		}
	}
	catch (const std::exception& error) 
	{
		success = false;
		LOGGER_ERROR("recieve exception raise , when fetch message , errmsg[ " << error.what() << " ]");
	}

	session.sync();
	session.close();

	return success;
}

void DCSConnectionService::handleFailureMsg(const ptree& response)
{
	if (!response.empty() && response.find("result") != response.not_found())
	{
		std::string result = response.get<std::string>("result");

		std::string dcsId = response.get<std::string>("dcsId");
		std::string message = response.get<std::string>("message");
		std::string operateType = response.get<std::string>("operateType");
		std::string operatorName = response.get<std::string>("operator");

		DcsBondDealService *bondDealService = (DcsBondDealService*)ServiceManager::singleton()->getService("DcsBondDeal");
		if (NULL == bondDealService) return;
		DcsBondDealBusiness bonddeal_business(bondDealService);
		bonddeal_business.DcsUpdateFailMsg(dcsId, message, operateType, operatorName);
	}
}

void DCSConnectionService::handleAddMsg(const ptree& response)
{
	if (!response.empty() && (response.find("result") != response.not_found()))
	{
		DcsBondDealCacheVecPtr deal_vec(new std::vector<DcsBondDealCachePtr>());
		dcsToDeal(response, deal_vec);
		if (deal_vec->size() > 0)
		{
			DcsBondDealService *bondDealService = (DcsBondDealService*)ServiceManager::singleton()->getService("DcsBondDeal");
			if (NULL == bondDealService) return;
			DcsBondDealBusiness bonddeal_business(bondDealService);
			bonddeal_business.DcsAddBondDeal(deal_vec->at(0));
		}
	}
}

void DCSConnectionService::handleUpdateMsg(const ptree& response)
{
	if (!response.empty() && (response.find("result") != response.not_found()))
	{
		DcsBondDealCacheVecPtr deal_vec(new std::vector<DcsBondDealCachePtr>());
		dcsToDeal(response, deal_vec);
		LOGGER_DEBUG("------ handleUpdateMsg deal_vec->size() = " << deal_vec->size());
		if (deal_vec->size() > 0)
		{
			DcsBondDealService *bondDealService = (DcsBondDealService*)ServiceManager::singleton()->getService("DcsBondDeal");
			if (NULL == bondDealService) return;
			DcsBondDealBusiness bonddeal_business(bondDealService);
			bonddeal_business.DcsUpdateBondDeal(deal_vec);
		}
	}
}

void DCSConnectionService::handleSwitchMsg(const ptree& response)
{
	if (!response.empty() && (response.find("result") != response.not_found()))
	{
		std::string result = response.get<std::string>("result");
		LOGGER_DEBUG("------ handleSwitchMsg result = " << result);
		kDcsSwitchStatus = ("true" == result) ? true : false;
	}
}

bool DCSConnectionService::send(const ptree& request, ptree& response, Duration timeout)
{
	if (_connection == NULL || !_connection->isOpen() || request.empty()) return false;

	Session session = _connection->createSession();
	std::string sendAddr = _service_name + "; {create:always , node:{type:queue}}";
	Sender sender = session.createSender(sendAddr);
	Address responseAddr = _service_name + GetNewGuid() + "; {create:always,  delete:always, node:{type:queue, x-declare : {auto-delete: true} }}";
	Receiver receiver = session.createReceiver(responseAddr);

	bool success = true;
	try
	{
		Message requestMsg;
		requestMsg.setReplyTo(responseAddr);
		requestMsg.setContent(GenerateJson(request));
		sender.send(requestMsg);
		LOGGER_DEBUG("------ send msg: " << requestMsg.getContent() << ", addr: " << requestMsg.getReplyTo());

		Message responseMsg = receiver.fetch(timeout);
		parseJson(responseMsg.getContent(), response);
	}
	catch (const std::exception& error)
	{
		success = false;
		LOGGER_ERROR("exception raise , when send message , errmsg[ " << error.what() << " ]");
	}

	session.sync();
	session.close();

	return success;
}

bool DCSConnectionService::parseJson(const std::string& json_str, ptree& pt)
{
	if (json_str.empty()) return false;

	//boost::regex expression("\\{.*\\}"); //检查json串是否合法
	//if (!boost::regex_match(json_str, expression)) return false;

	bool success = true;
	try
	{
		std::stringstream istream(json_str);
		read_json(istream, pt);
	}
	catch (const json_parser_error& e)
	{
		success = false;
		LOGGER_ERROR("exception raise , json parser error, errmsg[ " << e.what() << " ]");
	}
	catch (const ptree_error& e)
	{
		success = false;
		LOGGER_ERROR("exception raise , ptree error, errmsg[ " << e.what() << " ]");
	}
	catch (const std::runtime_error& e)
	{
		success = false;
		LOGGER_ERROR("exception raise , runtime error, errmsg[ " << e.what() << " ]");
	}

	return success;
}

std::string DCSConnectionService::GenerateJson(const ptree& pt, bool isSubList)
{
	if (pt.empty()) return "";

	bool success = true;
	std::string json_str = "";
	try
	{
		std::ostringstream ostream;
		this->write_json(ostream, pt, false);
		json_str = ostream.str();
		boost::erase_all(json_str, "    ");
		if (isSubList && (json_str.find("[") != std::string::npos) && (json_str.find("]") != std::string::npos))
		{
			json_str = json_str.substr(json_str.find_first_of("["), json_str.find_last_of("]") - json_str.find_first_of("[") + 1);
		}
	}
	catch (ptree_error& e)
	{
		LOGGER_ERROR("exception raise , when generate json, errmsg[ " << e.what() << " ]");
	}

	return json_str;
}

bool DCSConnectionService::queryContractList(DcsBondDealCacheVecPtr& deal_vec, int& total, int page_num, int page_size)
{
	if (!_is_open) return false;

	LOGGER_DEBUG("------ queryContractList page_num: " << page_num << ", page_size: " << page_size);
	ptree request, response, params;
	ptree startDateArray, endDateArray, dateTypeArray;

	ptree startDate, endDate, dateType;
	std::string beforeDateString = GetBeforeDateString("%Y-%m-%d", _cache_data_months) + " 00:00:00";
	LOGGER_DEBUG("------ queryContractList beforeDateString: " << beforeDateString);
	time_t start_date_milli_sec = ParseTimeString(beforeDateString) * 1000;
	startDate.put("", start_date_milli_sec);
	startDateArray.push_back(std::make_pair("", startDate));

	time_t end_date_milli_sec = ParseTimeString("2099-12-30 23:59:59") * 1000;
	endDate.put("", end_date_milli_sec);
	endDateArray.push_back(std::make_pair("", endDate));

	dateType.put("", _date_type);
	dateTypeArray.push_back(std::make_pair("", dateType));

	params.add_child("startDate", startDateArray);
	params.add_child("endDate", endDateArray);
	params.add_child("dateType", dateTypeArray);

	request.put("method", "QUERYCONTRACTLIST");
	request.put("productName", _product_name);
	request.add_child("params", params);
	request.put("pageNum", page_num);
	request.put("pageSize", page_size);
	request.put("sortedField", "");
	request.put("strategy", "");

	if (send(request, response, Duration(timeout)))
	{
		if (response.empty() || (response.find("result") == response.not_found()) || (response.find("length") == response.not_found())) return false;
		
		dcsToDeal(response, deal_vec);

		std::string length = response.get<std::string>("length");
		if ("" == length) length = "0";
		total = boost::lexical_cast<int>(length);
		LOGGER_DEBUG("------ queryContractList result size: " << deal_vec->size() << ", total size: " << total);
	}
	else
	{
		return false;
	}

	return true;
}

bool DCSConnectionService::addContractList(const DcsBondDealCache* cache, std::string account_name)
{
	if (!_is_open) return false;
	if (NULL == cache) return false;

	ptree request, list, response;
	ptree pt;
	dealToDcs(cache, pt);
	list.push_back(std::make_pair("", pt));

	request.put("method", "ADDCONTRACTLIST");
	request.put("productName", _product_name);
	request.add_child("contractList", list);
	request.put("oldContractId", cache->oldContractId);
	request.put("operator", account_name);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ addContractList result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::updateContractList(const DcsBondDealCache* cache, std::string account_name)
{
	if (!_is_open) return false;
	if (NULL == cache) return false;

	ptree request, list, response;
	ptree pt;
	dealToDcs(cache, pt);
	list.push_back(std::make_pair("", pt));

	request.put("method", "UPDATECONTRACTLIST");
	request.put("productName", _product_name);
	request.add_child("contractList", list);
	request.put("operator", account_name);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ updateContractList result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::updateContractListSilent(const DcsBondDealCache* cache, std::string account_name)
{
	if (!_is_open) return false;
	if (NULL == cache) return false;

	ptree request, list, response;
	ptree pt;
	dealToDcs(cache, pt);
	list.push_back(std::make_pair("", pt));

	request.put("method", "UPDATECONTRACTLISTSILENT");
	request.put("productName", _product_name);
	request.add_child("contractList", list);
	request.put("operator", account_name);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ updateContractListSilent result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::deleteContractList(std::string id_str, std::string account_name)
{
	if (!_is_open) return false;

	ptree request, list, response;
	list.put("", id_str);

	request.put("method", "SOFTDELETECONTRACTLIST");
	request.put("productName", _product_name);
	request.add_child("contractIdList", list);
	request.put("operator", account_name);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ softDeleteContractList result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::updateContractConfirmStatus(std::string id, int bid_ofr, bool status, std::string& deal_status, std::string account_name)
{
	if (!_is_open) return false;

	ptree request, response;
	request.put("method", "UPDATECONTRACTCONFIRMSTATUS");
	request.put("productName", _product_name);
	request.put("contractId", id);
	request.put("confirmIndex", bid_ofr);
	request.put("confirmStatus", status);
	request.put("operator", account_name);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()) && (response.find("dealStatus") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			deal_status = response.get<std::string>("dealStatus");
			LOGGER_DEBUG("------ updateContractConfirmStatus result: " << result << ", deal_status: " << deal_status);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::submitContractList(std::string id_str, std::string account_name)
{
	if (!_is_open) return false;
	if (!kDcsSwitchStatus) return false;

	ptree request, list, response;
	list.put("", id_str);

	request.put("method", "SUBMITCONTRACTLIST");
	request.put("productName", _product_name);
	request.add_child("contractIdList", list);
	request.put("operator", account_name);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ submitContractList result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::relateSubmitContractList(std::string id_str, std::string comments, std::string account_name)
{
	if (!_is_open) return false;
	if (!kDcsSwitchStatus) return false;

	ptree request, list, response;
	list.put("", id_str);

	request.put("method", "RELATESUBMITCONTRACTLIST");
	request.put("productName", _product_name);
	request.add_child("contractIdList", list);
	request.put("operator", account_name);
	request.put("comments", comments);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ relateSubmitContractList result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::destroyContract(std::string id, std::string account_name, const std::string& reason)
{
	if (!_is_open) return false;
	if (!kDcsSwitchStatus) return false;

	ptree request, list, response;
	list.put("", id);
    
	request.put("method", "DESTROYCONTRACT");
	request.put("productName", _product_name);
	request.add_child("contractIdList", list);
	request.put("operator", account_name);
    request.put("destroyReason", reason);
    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ destroyContract result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::setContractListUrgentStatus(std::string id_str, std::string urgent_status, std::string account_name)
{
	if (!_is_open) return false;

	ptree request, list, response;
	list.put("", id_str);

	request.put("method", "SETCONTRACTLISTURGENTSTATUS");
	request.put("productName", _product_name);
	request.add_child("contractIdList", list);
	request.put("urgentStatus", urgent_status);
	request.put("operator", account_name);

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ setContractListUrgentStatus result: " << result);
			return ("true" == result) ? true : false;
		}
	}

	return false;
}

bool DCSConnectionService::getSwitch()
{
	if (!_is_open) return false;

	ptree request, response;
	request.put("method", "GETSWITCH");

    if (send(request, response, Duration(timeout)))
	{
		if (!response.empty() && (response.find("result") != response.not_found()))
		{
			std::string result = response.get<std::string>("result");
			LOGGER_DEBUG("------ getSwitch result: " << result);
			kDcsSwitchStatus = ("true" == result) ? true : false;
		}
	}

	return kDcsSwitchStatus;
}

bool DCSConnectionService::getDcsDealLog(DcsHistoryDetailVecPtr& detail_vec, std::string id)
{
	if (!_is_open) return false;

	LOGGER_DEBUG("------ getDcsDealLog id: " << id);
	ptree request, response;
	request.put("method", "GETDCSDEALLOG");
	request.put("productName", _product_name);
	request.put("contractId", id);

    if (send(request, response, Duration(timeout)))
	{
		if (response.empty() || (response.find("result") == response.not_found())) return false;

		dcsToHistoryDetail(response, detail_vec);
		LOGGER_DEBUG("------ getDcsDealLog result size: " << detail_vec->size());
	}
	else
	{
		return false;
	}

	return true;
}

void DCSConnectionService::dcsToDeal(const ptree& pt, DcsBondDealCacheVecPtr& deal_vec)
{
	if (pt.empty()) return;

	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child("result"))
	{
		DcsBondDealCachePtr deal(new DcsBondDealCache);
		deal->id = v.second.find("contractId") == v.second.not_found() ? "" : v.second.get<std::string>("contractId");
		deal->company_id = v.second.find("companyId") == v.second.not_found() ? "" : v.second.get<std::string>("companyId");
		deal->bond_key_listed_market = v.second.find("bondKeyListedMarket") == v.second.not_found() ? "" : v.second.get<std::string>("bondKeyListedMarket");
		deal->create_time = v.second.find("createTime") == v.second.not_found() ? "" : v.second.get<std::string>("createTime");
		deal->updateTime = v.second.find("updateTime") == v.second.not_found() ? "" : v.second.get<std::string>("updateTime");
		deal->urgent = v.second.find("urgent") == v.second.not_found() ? "false" : v.second.get<std::string>("urgent");
		deal->questionMark = v.second.find("questionMark") == v.second.not_found() ? "false" : v.second.get<std::string>("questionMark");

		deal->deal_type = v.second.find("direction") == v.second.not_found() ? "" : v.second.get<std::string>("direction");
		deal->deal_time = v.second.find("dealTime") == v.second.not_found() ? "" : v.second.get<std::string>("dealTime");
		deal->odd_numbers = v.second.find("orderNo") == v.second.not_found() ? "" : v.second.get<std::string>("orderNo");
		std::string sequence_number = v.second.find("sequenceNumber") == v.second.not_found() ? "0" : v.second.get<std::string>("sequenceNumber");
		deal->sequence_number = boost::lexical_cast<int>(sequence_number);

		deal->goods_id = v.second.find("goodsId") == v.second.not_found() ? "" : v.second.get<std::string>("goodsId");
		deal->bond_key = v.second.find("bondKey") == v.second.not_found() ? "" : v.second.get<std::string>("bondKey");
		deal->listed_market = v.second.find("listedMarket") == v.second.not_found() ? "" : v.second.get<std::string>("listedMarket");
		deal->goods_code = v.second.find("bondCode") == v.second.not_found() ? "" : v.second.get<std::string>("bondCode");
		deal->goods_short_name = v.second.find("bondShortName") == v.second.not_found() ? "" : v.second.get<std::string>("bondShortName");
		
		std::string tmp_maturity_date = v.second.find("maturityDate") == v.second.not_found() ? "" : v.second.get<std::string>("maturityDate");;
		if (tmp_maturity_date.length() > 0 && (-1 != tmp_maturity_date.find("-")))
		{
			time_t tmp_time_t = ParseMaturityDateString(tmp_maturity_date, "%4d-%2d-%2d");
			tmp_maturity_date = GetTDateString(tmp_time_t, "%Y%m%d");
		}
		deal->maturity_date = tmp_maturity_date;

		std::string price = v.second.find("price") == v.second.not_found() ? "0" : v.second.get<std::string>("price");
		if ("" != price) deal->price = boost::lexical_cast<real8>(price);
		deal->rebate = v.second.find("rebate") == v.second.not_found() ? "" : v.second.get<std::string>("rebate");
		std::string returnPoint = v.second.find("returnPoint") == v.second.not_found() ? "0" : v.second.get<std::string>("returnPoint");
		if ("" != returnPoint) deal->return_point = boost::lexical_cast<real8>(returnPoint);
		std::string volume = v.second.find("volume") == v.second.not_found() ? "0" : v.second.get<std::string>("volume");
		if ("" != volume) deal->volume = boost::lexical_cast<real8>(volume);
		deal->currency = v.second.find("currency") == v.second.not_found() ? "" : v.second.get<std::string>("currency");
		deal->exercise = v.second.find("exercise") == v.second.not_found() ? "" : v.second.get<std::string>("exercise");
		deal->time_style = v.second.find("settlementType") == v.second.not_found() ? "" : v.second.get<std::string>("settlementType");
		deal->settlement_date = v.second.find("settlementDate") == v.second.not_found() ? "" : v.second.get<std::string>("settlementDate");

		std::string yield = v.second.find("yield") == v.second.not_found() ? "0" : v.second.get<std::string>("yield");
		if ("" != yield) deal->yield = boost::lexical_cast<real8>(yield);
		std::string spread = v.second.find("spread") == v.second.not_found() ? "0" : v.second.get<std::string>("spread");
		if ("" != spread) deal->spread = boost::lexical_cast<real8>(spread);
		std::string cleanPrice = v.second.find("cleanPrice") == v.second.not_found() ? "0" : v.second.get<std::string>("cleanPrice");
		if ("" != cleanPrice) deal->net_price = boost::lexical_cast<real8>(cleanPrice);
		std::string dirtyPrice = v.second.find("dirtyPrice") == v.second.not_found() ? "0" : v.second.get<std::string>("dirtyPrice");
		if ("" != dirtyPrice) deal->full_price = boost::lexical_cast<real8>(dirtyPrice);

		deal->bid_bank_id = v.second.find("bidBankId") == v.second.not_found() ? "" : v.second.get<std::string>("bidBankId");
		deal->bid_bank_code = v.second.find("bidBankCode") == v.second.not_found() ? "" : v.second.get<std::string>("bidBankCode");
		deal->bid_bank_city = v.second.find("bidBankCity") == v.second.not_found() ? "" : v.second.get<std::string>("bidBankCity");
		deal->bid_agent_id = v.second.find("bidBankAgentId") == v.second.not_found() ? "" : v.second.get<std::string>("bidBankAgentId");
		std::string bidBankAgentCode = v.second.find("bidBankAgentCode") == v.second.not_found() ? "" : v.second.get<std::string>("bidBankAgentCode");
		deal->bid_trader_account = "";
		int start_pos = bidBankAgentCode.find("(");
		int end_pos = bidBankAgentCode.find(")");
		if ((-1 != start_pos) && (-1 != end_pos))
		{
			deal->bid_trader_account = bidBankAgentCode.substr(start_pos + 1, end_pos - start_pos - 1);
		}
		deal->bid_broker_id = v.second.find("bidBrokerIdA") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerIdA");
		deal->bidBrokerIdB = v.second.find("bidBrokerIdB") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerIdB");
		deal->bidBrokerIdC = v.second.find("bidBrokerIdC") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerIdC");
		deal->bidBrokerIdD = v.second.find("bidBrokerIdD") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerIdD");
		deal->bidBrokerCodeA = v.second.find("bidBrokerCodeA") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerCodeA");
		deal->bidBrokerCodeB = v.second.find("bidBrokerCodeB") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerCodeB");
		deal->bidBrokerCodeC = v.second.find("bidBrokerCodeC") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerCodeC");
		deal->bidBrokerCodeD = v.second.find("bidBrokerCodeD") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerCodeD");
		deal->bidPercentA = v.second.find("bidPercentA") == v.second.not_found() ? "" : v.second.get<std::string>("bidPercentA");
		deal->bidPercentB = v.second.find("bidPercentB") == v.second.not_found() ? "" : v.second.get<std::string>("bidPercentB");
		deal->bidPercentC = v.second.find("bidPercentC") == v.second.not_found() ? "" : v.second.get<std::string>("bidPercentC");
		deal->bidPercentD = v.second.find("bidPercentD") == v.second.not_found() ? "" : v.second.get<std::string>("bidPercentD");
		deal->bid_charge = v.second.find("bidCharge") == v.second.not_found() ? "" : v.second.get<std::string>("bidCharge");
		deal->bidIsNoBrokerage = v.second.find("bidIsNoBrokerage") == v.second.not_found() ? "" : v.second.get<std::string>("bidIsNoBrokerage");
		deal->bidBrokerrageRemark = v.second.find("bidBrokerage") == v.second.not_found() ? "" : v.second.get<std::string>("bidBrokerage");
		deal->bidTradeMode = v.second.find("bidChannel") == v.second.not_found() ? "" : v.second.get<std::string>("bidChannel");
		deal->bidNoConfirm = v.second.find("bidNoConfirm") == v.second.not_found() ? "" : v.second.get<std::string>("bidNoConfirm");
		deal->bidNoComments = v.second.find("bidNoComments") == v.second.not_found() ? "" : v.second.get<std::string>("bidNoComments");
		deal->bid_bridge = v.second.find("bidBridge") == v.second.not_found() ? "" : v.second.get<std::string>("bidBridge");

		deal->ofr_bank_id = v.second.find("ofrBankId") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBankId");
		deal->ofr_bank_code = v.second.find("ofrBankCode") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBankCode");
		deal->ofr_bank_city = v.second.find("ofrBankCity") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBankCity");
		deal->ofr_agent_id = v.second.find("ofrBankAgentId") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBankAgentId");
		std::string ofrBankAgentCode = v.second.find("ofrBankAgentCode") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBankAgentCode");
		deal->ofr_trader_account = "";
		start_pos = ofrBankAgentCode.find("(");
		end_pos = ofrBankAgentCode.find(")");
		if ((-1 != start_pos) && (-1 != end_pos))
		{
			deal->ofr_trader_account = ofrBankAgentCode.substr(start_pos + 1, end_pos - start_pos - 1);
		}

		deal->ofr_broker_id = v.second.find("ofrBrokerIdA") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerIdA");
		deal->ofrBrokerIdB = v.second.find("ofrBrokerIdB") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerIdB");
		deal->ofrBrokerIdC = v.second.find("ofrBrokerIdC") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerIdC");
		deal->ofrBrokerIdD = v.second.find("ofrBrokerIdD") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerIdD");
		deal->ofrBrokerCodeA = v.second.find("ofrBrokerCodeA") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerCodeA");
		deal->ofrBrokerCodeB = v.second.find("ofrBrokerCodeB") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerCodeB");
		deal->ofrBrokerCodeC = v.second.find("ofrBrokerCodeC") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerCodeC");
		deal->ofrBrokerCodeD = v.second.find("ofrBrokerCodeD") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerCodeD");
		deal->ofrPercentA = v.second.find("ofrPercentA") == v.second.not_found() ? "" : v.second.get<std::string>("ofrPercentA");
		deal->ofrPercentB = v.second.find("ofrPercentB") == v.second.not_found() ? "" : v.second.get<std::string>("ofrPercentB");
		deal->ofrPercentC = v.second.find("ofrPercentC") == v.second.not_found() ? "" : v.second.get<std::string>("ofrPercentC");
		deal->ofrPercentD = v.second.find("ofrPercentD") == v.second.not_found() ? "" : v.second.get<std::string>("ofrPercentD");
		deal->ofr_charge = v.second.find("ofrCharge") == v.second.not_found() ? "" : v.second.get<std::string>("ofrCharge");
		deal->ofrIsNoBrokerage = v.second.find("ofrIsNoBrokerage") == v.second.not_found() ? "" : v.second.get<std::string>("ofrIsNoBrokerage");
		deal->ofrBrokerrageRemark = v.second.find("ofrBrokerage") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBrokerage");
		deal->ofrTradeMode = v.second.find("ofrChannel") == v.second.not_found() ? "" : v.second.get<std::string>("ofrChannel");
		deal->ofrNoConfirm = v.second.find("ofrNoConfirm") == v.second.not_found() ? "" : v.second.get<std::string>("ofrNoConfirm");
		deal->ofrNoComments = v.second.find("ofrNoComments") == v.second.not_found() ? "" : v.second.get<std::string>("ofrNoComments");
		deal->ofr_bridge = v.second.find("ofrBridge") == v.second.not_found() ? "" : v.second.get<std::string>("ofrBridge");

		deal->bid_remarks = v.second.find("paymentBid") == v.second.not_found() ? "" : v.second.get<std::string>("paymentBid");
		deal->remarks = v.second.find("infoBackOffice") == v.second.not_found() ? "" : v.second.get<std::string>("infoBackOffice");
		deal->ofr_remarks = v.second.find("paymentOfr") == v.second.not_found() ? "" : v.second.get<std::string>("paymentOfr");
		deal->specialPartyBid = v.second.find("specialPartyBid") == v.second.not_found() ? "" : v.second.get<std::string>("specialPartyBid");
		deal->specialPartyOfr = v.second.find("specialPartyOfr") == v.second.not_found() ? "" : v.second.get<std::string>("specialPartyOfr");
		deal->specialTerms = v.second.find("specialTerms") == v.second.not_found() ? "" : v.second.get<std::string>("specialTerms");
		deal->feedback = v.second.find("feedBack") == v.second.not_found() ? "" : v.second.get<std::string>("feedBack");

		deal->delivery_type = v.second.find("settlementMode") == v.second.not_found() ? "" : v.second.get<std::string>("settlementMode");
		deal->total_price = v.second.find("totalPrice") == v.second.not_found() ? "" : v.second.get<std::string>("totalPrice");
		deal->internally = v.second.find("flagInner") == v.second.not_found() ? "" : v.second.get<std::string>("flagInner");

		std::string bid_check = v.second.find("bidCheck") == v.second.not_found() ? "false" : v.second.get<std::string>("bidCheck");
		std::istringstream(bid_check) >> std::boolalpha >> deal->bid_check;
		std::string ofr_check = v.second.find("ofrCheck") == v.second.not_found() ? "false" : v.second.get<std::string>("ofrCheck");
		std::istringstream(ofr_check) >> std::boolalpha >> deal->ofr_check;
		deal->deal_status = v.second.find("dealStatus") == v.second.not_found() ? "" : v.second.get<std::string>("dealStatus");
		std::string inHand = v.second.find("inHand") == v.second.not_found() ? "false" : v.second.get<std::string>("inHand");
		std::istringstream(inHand) >> std::boolalpha >> deal->inHand;

		deal->issue_institution = v.second.find("issueDept") == v.second.not_found() ? "" : v.second.get<std::string>("issueDept");

		std::string backOperated = v.second.find("backOperated") == v.second.not_found() ? "false" : v.second.get<std::string>("backOperated");
		std::istringstream(backOperated) >> std::boolalpha >> deal->backOperated;
		deal->checkLevel = v.second.find("checkLevel") == v.second.not_found() ? "" : v.second.get<std::string>("checkLevel");
		deal->currLevel = v.second.find("currLevel") == v.second.not_found() ? "" : v.second.get<std::string>("currLevel");
		deal->specialFlag = v.second.find("specialFlag") == v.second.not_found() ? "" : v.second.get<std::string>("specialFlag");

		deal->bond_category = v.second.find("bondCategory") == v.second.not_found() ? "" : v.second.get<std::string>("bondCategory");
		deal->bond_deal_type = v.second.find("bondDealType") == v.second.not_found() ? "" : v.second.get<std::string>("bondDealType");
		deal->quote_type = v.second.find("quoteType") == v.second.not_found() ? "" : v.second.get<std::string>("quoteType");
		deal->description = v.second.find("description") == v.second.not_found() ? "" : v.second.get<std::string>("description");
		deal->status = v.second.find("status") == v.second.not_found() ? "" : v.second.get<std::string>("status");
		deal->send_status = v.second.find("sendStatus") == v.second.not_found() ? "" : v.second.get<std::string>("sendStatus");

		deal->deskName = v.second.find("deskName") == v.second.not_found() ? "" : v.second.get<std::string>("deskName");
		deal->versionNo = v.second.find("versionNo") == v.second.not_found() ? "" : v.second.get<std::string>("versionNo");
		deal->mbsNo = v.second.find("mbsNo") == v.second.not_found() ? "" : v.second.get<std::string>("mbsNo");

		deal->oldContractId = v.second.find("oldContractId") == v.second.not_found() ? "" : v.second.get<std::string>("oldContractId");
		deal->relatedMsg = v.second.find("relatedMsg") == v.second.not_found() ? "" : v.second.get<std::string>("relatedMsg");

        deal->destroy_reason = v.second.find("destroyReason") == v.second.not_found() ? "" : v.second.get<std::string>("destroyReason");

		deal_vec->push_back(deal);
	}
}

void DCSConnectionService::dealToDcs(const DcsBondDealCache* cache, ptree& pt)
{
	if (NULL == cache) return;

	pt.put("contractId", cache->id);
	pt.put("contractIdProName", cache->id + "+BOND");
	pt.put("companyId", cache->company_id);
	pt.put("bondKeyListedMarket", cache->bond_key_listed_market);
	pt.put("createTime", cache->create_time);
	pt.put("updateTime", cache->updateTime);
	pt.put("urgent", cache->urgent); //加急标志
	pt.put("questionMark", cache->questionMark); //问号标记

	pt.put("direction", cache->deal_type); //买卖方向, GVN/TKN／TRD
	pt.put("dealTime", cache->deal_time);
	pt.put("orderNo", cache->odd_numbers); //单号
	pt.put("sequenceNumber", cache->sequence_number); //序列号

	pt.put("goodsId", cache->goods_id);
	pt.put("bondKey", cache->bond_key);
	pt.put("listedMarket", cache->listed_market);
	pt.put("bondCode", cache->goods_code);
	pt.put("bondShortName", cache->goods_short_name);

	std::string tmp_maturity_date = cache->maturity_date;
	if (tmp_maturity_date.length() > 0)
	{
		time_t tmp_time_t = ParseMaturityDateString(tmp_maturity_date, "%4d%2d%2d");
		tmp_maturity_date = GetTDateString(tmp_time_t, "%Y-%m-%d");
	}
	pt.put("maturityDate", tmp_maturity_date); //债券到期日

	pt.put("price", real8_format(cache->price));
	pt.put("rebate", cache->rebate);  //是否返点 0, 1
	pt.put("returnPoint", real8_format(cache->return_point)); //返点数值, 比如返0.12
	pt.put("volume", truncateDouble(cache->volume, DOUBLE_DEFAULT_PRECISION));
	pt.put("currency", cache->currency);
	pt.put("exercise", cache->exercise); //0-行权 1-到期
	pt.put("settlementType", cache->time_style); //结算方式：T+0 T+1
	pt.put("settlementDate", cache->settlement_date); //交割日

	pt.put("yield", truncateDouble(cache->yield, DOUBLE_MAX_PRECISION)); //收益率
	pt.put("spread", truncateDouble(cache->spread, DOUBLE_DEFAULT_PRECISION)); //利差
	pt.put("cleanPrice", truncateDouble(cache->net_price, DOUBLE_MAX_PRECISION)); //净价 
	pt.put("dirtyPrice", truncateDouble(cache->full_price, DOUBLE_MAX_PRECISION)); //全价 
	
	pt.put("bidBankId", cache->bid_bank_id);
	pt.put("bidBankCode", cache->bid_bank_code);
	pt.put("bidBankCity", cache->bid_bank_city);
	pt.put("bidBankAgentId", cache->bid_agent_id);
	
	std::string tmpBidBankAgentName = cache->bidBankAgentName;
	if (cache->bid_trader_account.length() > 0) tmpBidBankAgentName += "(" + cache->bid_trader_account + ")";
	pt.put("bidBankAgentCode", tmpBidBankAgentName);

	pt.put("bidBankAgentName", cache->bidBankAgentName);
	pt.put("bidBrokerIdA", cache->bid_broker_id);
	pt.put("bidBrokerIdB", cache->bidBrokerIdB);
	pt.put("bidBrokerIdC", cache->bidBrokerIdC);
	pt.put("bidBrokerIdD", cache->bidBrokerIdD);
	pt.put("bidBrokerCodeA", cache->bidBrokerCodeA);
	pt.put("bidBrokerCodeB", cache->bidBrokerCodeB);
	pt.put("bidBrokerCodeC", cache->bidBrokerCodeC);
	pt.put("bidBrokerCodeD", cache->bidBrokerCodeD);
	pt.put("bidPercentA", cache->bidPercentA);
	pt.put("bidPercentB", cache->bidPercentB);
	pt.put("bidPercentC", cache->bidPercentC);
	pt.put("bidPercentD", cache->bidPercentD);
	pt.put("bidCharge", cache->bid_charge); //bid是否收费
	pt.put("bidIsNoBrokerage", cache->bidIsNoBrokerage); //bid是否免佣
	pt.put("bidBrokerage", cache->bidBrokerrageRemark); //bid佣金备注
	pt.put("bidChannel", cache->bidTradeMode); //bid方交易方式
	pt.put("bidNoComments", cache->bidNoComments); //Bid方NC备注
	pt.put("bidNoConfirm", cache->bidNoConfirm); // Bid方NC
	pt.put("bidBridge", cache->bid_bridge); // Bid方是否为过桥机构

	pt.put("ofrBankId", cache->ofr_bank_id);
	pt.put("ofrBankCode", cache->ofr_bank_code);
	pt.put("ofrBankCity", cache->ofr_bank_city);
	pt.put("ofrBankAgentId", cache->ofr_agent_id);

	std::string tmpOfrBankAgentName = cache->ofrBankAgentName;
	if (cache->ofr_trader_account.length() > 0) tmpOfrBankAgentName += "(" + cache->ofr_trader_account + ")";
	pt.put("ofrBankAgentCode", tmpOfrBankAgentName);

	pt.put("ofrBankAgentName", cache->ofrBankAgentName);
	pt.put("ofrBrokerIdA", cache->ofr_broker_id);
	pt.put("ofrBrokerIdB", cache->ofrBrokerIdB);
	pt.put("ofrBrokerIdC", cache->ofrBrokerIdC);
	pt.put("ofrBrokerIdD", cache->ofrBrokerIdD);
	pt.put("ofrBrokerCodeA", cache->ofrBrokerCodeA);
	pt.put("ofrBrokerCodeB", cache->ofrBrokerCodeB);
	pt.put("ofrBrokerCodeC", cache->ofrBrokerCodeC);
	pt.put("ofrBrokerCodeD", cache->ofrBrokerCodeD);
	pt.put("ofrPercentA", cache->ofrPercentA);
	pt.put("ofrPercentB", cache->ofrPercentB);
	pt.put("ofrPercentC", cache->ofrPercentC);
	pt.put("ofrPercentD", cache->ofrPercentD);
	pt.put("ofrCharge", cache->ofr_charge); //ofr是否收费
	pt.put("ofrIsNoBrokerage", cache->ofrIsNoBrokerage); //ofr是否免佣
	pt.put("ofrBrokerage", cache->ofrBrokerrageRemark); //ofr佣金备注
	pt.put("ofrChannel", cache->ofrTradeMode); //ofr方交易方式
	pt.put("ofrNoComments", cache->ofrNoComments); // Ofr方NC备注
	pt.put("ofrNoConfirm", cache->ofrNoConfirm); // Ofr方NC
	pt.put("ofrBridge", cache->ofr_bridge); // Ofr方是否为过桥机构

	pt.put("paymentBid", cache->bid_remarks);
	pt.put("infoBackOffice", cache->remarks);
	pt.put("paymentOfr", cache->ofr_remarks);
	pt.put("specialPartyBid", cache->specialPartyBid);
	pt.put("specialPartyOfr", cache->specialPartyOfr);
	pt.put("specialTerms", cache->specialTerms);
	pt.put("feedBack", cache->feedback);

	pt.put("settlementMode", cache->delivery_type.substr(0, 3)); //结算模式：DVP(券款对付)
	pt.put("totalPrice", cache->total_price); //结算金额
	pt.put("flagInner", cache->internally); //内部成交, "1"-非内部成交，“2”-内部成交

	pt.put("bidCheck", cache->bid_check);
	pt.put("ofrCheck", cache->ofr_check);
	pt.put("dealStatus", cache->deal_status);
	pt.put("inHand", cache->inHand); //正在处理中

	pt.put("issueDept", cache->issue_institution); //发行人

	//高级审核相关字段
	pt.put("backOperated", cache->backOperated); //成交单是否已经过审核操作
	pt.put("checkLevel", cache->checkLevel);
	pt.put("currLevel", cache->currLevel);
	pt.put("specialFlag", cache->specialFlag);

	pt.put("bondCategory", cache->bond_category); //信用、利率
	pt.put("bondDealType", cache->bond_deal_type); //BCO = 银行间的信用债 BNC = 银行间的利率债 BBE=交易所中的信用债 BBN=交易所中的利率债
	pt.put("quoteType", cache->quote_type); //报价类型: 收益率/净价/全价/利差
	pt.put("status", cache->status);
	pt.put("sendStatus", cache->send_status);
	pt.put("description", cache->description);

	pt.put("deskName", cache->deskName);
	pt.put("versionNo", cache->versionNo);
	pt.put("mbsNo", cache->mbsNo);

	pt.put("oldContractId", cache->oldContractId);
	pt.put("relatedMsg", cache->relatedMsg);

    pt.put("destroyReason", cache->destroy_reason);
}

std::string DCSConnectionService::real8_format(real8 value, std::string percision)
{
	char result[20] = "";
	std::string format = "%." + percision + "f";
	if (DOUBLE_NULL != value) sprintf(result, format.c_str(), value);

	return result;
}

std::string DCSConnectionService::truncateDouble(real8 value, std::string percision)
{
	int default_precision = boost::lexical_cast<int>(DOUBLE_DEFAULT_PRECISION);
	int int_precision = boost::lexical_cast<int>(percision);
	int delta_precision = default_precision;
	if (int_precision < default_precision) delta_precision = int_precision;

	std::string double_str = real8_format(value, percision);
	if (double_str.empty() || ("" == double_str)) return double_str;

	std::string ret = double_str;
	if (boost::algorithm::contains(double_str, ".")) {
		int length = double_str.length();
		int valid_len = -1;
		int start = length - 1, end = length - delta_precision;
		for (int i = start; i >= end; --i) {
			if ('0' != double_str.at(i)) {
				valid_len = i + 1;
				break;
			}
		}
		if (-1 == valid_len) valid_len = length - delta_precision;
		ret = double_str.length() > 1 && valid_len < (double_str.length() - 1) ? double_str.substr(0, valid_len) : double_str;
	}
	if (boost::algorithm::ends_with(ret, ".")) {
		ret = ret.substr(0, ret.length() - 1);
	}
	return ret;
}

void DCSConnectionService::dcsToHistoryDetail(const ptree& pt, DcsHistoryDetailVecPtr& detail_vec)
{
	if (pt.empty()) return;

	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child("result"))
	{
		DcsHistoryDetailPtr detail(new DcsHistoryDetail);
		std::string operate = v.second.find("operate") == v.second.not_found() ? "" : v.second.get<std::string>("operate");
		detail->operate = _operate_map[operate];

		detail->history_operator = v.second.find("operator") == v.second.not_found() ? "" : v.second.get<std::string>("operator");

		std::string update_time_str = v.second.find("update_time") == v.second.not_found() ? "" : v.second.get<std::string>("update_time");
		time_t update_time_t;
		if ("" != update_time_str) update_time_t = boost::lexical_cast<time_t>(update_time_str);
		detail->update_time = GetTimeString(update_time_t / 1000);

		detail->comment = v.second.find("comment") == v.second.not_found() ? "" : v.second.get<std::string>("comment");

		std::string update_content = "";
		if (v.second.find("update_content") != v.second.not_found())
		{
			ptree content_pt;
			parseJson(v.second.get<std::string>("update_content"), content_pt);
			translateUpdateContent(content_pt, update_content);
		}
		detail->update_content = update_content;

		detail_vec->push_back(detail);
	}
}

void DCSConnectionService::translateUpdateContent(const ptree& pt, std::string &update_content)
{
	if (pt.empty()) return;

	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt)
	{
		std::string new_content = v.second.find("new") == v.second.not_found() ? "" : v.second.get<std::string>("new");
		std::string old_content = v.second.find("old") == v.second.not_found() ? "" : v.second.get<std::string>("old");
		std::string name = v.second.find("name") == v.second.not_found() ? "" : v.second.get<std::string>("name");
		if ("" != update_content) update_content += ",";
		update_content += _update_content_map[name] + GBKToUTF8("从") + old_content + GBKToUTF8("变为") + new_content;
	}
}

void DCSConnectionService::initOperateMap()
{
	_operate_map["add"] = GBKToUTF8("创建");
	_operate_map["update"] = GBKToUTF8("编辑");
	_operate_map["delete"] = GBKToUTF8("删除");
	_operate_map["bidCheck"] = GBKToUTF8("bid确认");
	_operate_map["unBidCheck"] = GBKToUTF8("取消bid确认");
	_operate_map["ofrCheck"] = GBKToUTF8("ofr确认");
	_operate_map["unOfrCheck"] = GBKToUTF8("取消ofr确认");
	_operate_map["submit"] = GBKToUTF8("提交");
	_operate_map["sendCheck"] = GBKToUTF8("送审");
	_operate_map["Verify"] = GBKToUTF8("通过");
	_operate_map["Approve"] = GBKToUTF8("确认");
	_operate_map["Reject"] = GBKToUTF8("退回");
	_operate_map["destroy"] = GBKToUTF8("毁单");
	_operate_map["urgent"] = GBKToUTF8("加急");
	_operate_map["unUrgent"] = GBKToUTF8("取消加急");
	_operate_map["Revise"] = GBKToUTF8("校正");
}

void DCSConnectionService::initUpdateContentMap()
{
	_update_content_map["specialTerms"] = GBKToUTF8("Special Terms");
	_update_content_map["specialPartyOfr"] = GBKToUTF8("Special Party Ofr");
	_update_content_map["specialPartyBid"] = GBKToUTF8("Special Party Bid");
	_update_content_map["paymentOfr"] = GBKToUTF8("Payment Ofr");
	_update_content_map["paymentBid"] = GBKToUTF8("Payment Bid");
	_update_content_map["infoBackOffice"] = GBKToUTF8("Info Back Office");
	_update_content_map["bidBrokerage"] = GBKToUTF8("Bid佣金");
	_update_content_map["ofrBrokerage"] = GBKToUTF8("Ofr佣金");
	_update_content_map["bidBrokerCodeA"] = GBKToUTF8("第一个Bid经纪人");
	_update_content_map["bidBrokerCodeB"] = GBKToUTF8("第二个Bid经纪人");
	_update_content_map["bidBrokerCodeC"] = GBKToUTF8("第三个Bid经纪人");
	_update_content_map["bidBrokerCodeD"] = GBKToUTF8("第四个Bid经纪人");
	_update_content_map["ofrBrokerCodeA"] = GBKToUTF8("第一个Ofr经纪人");
	_update_content_map["ofrBrokerCodeB"] = GBKToUTF8("第二个Ofr经纪人");
	_update_content_map["ofrBrokerCodeC"] = GBKToUTF8("第三个Ofr经纪人");
	_update_content_map["ofrBrokerCodeD"] = GBKToUTF8("第四个Ofr经纪人");
	_update_content_map["bidPercentA"] = GBKToUTF8("第一个Bid经纪人佣金比例");
	_update_content_map["bidPercentB"] = GBKToUTF8("第二个Bid经纪人佣金比例");
	_update_content_map["bidPercentC"] = GBKToUTF8("第三个Bid经纪人佣金比例");
	_update_content_map["bidPercentD"] = GBKToUTF8("第四个Bid经纪人佣金比例");
	_update_content_map["ofrPercentA"] = GBKToUTF8("第一个Ofr经纪人佣金比例");
	_update_content_map["ofrPercentB"] = GBKToUTF8("第二个Ofr经纪人佣金比例");
	_update_content_map["ofrPercentC"] = GBKToUTF8("第三个Ofr经纪人佣金比例");
	_update_content_map["ofrPercentD"] = GBKToUTF8("第四个Ofr经纪人佣金比例");
	_update_content_map["ofrBankCode"] = GBKToUTF8("Ofr机构");
	_update_content_map["bidBankCode"] = GBKToUTF8("Bid机构");
	_update_content_map["tradeDate"] = GBKToUTF8("交易日期");
	_update_content_map["tradeTime"] = GBKToUTF8("交易时间");
	_update_content_map["bidVia"] = GBKToUTF8("Bid Via");
	_update_content_map["ofrVia"] = GBKToUTF8("Ofr Via");
	_update_content_map["bidChannel"] = GBKToUTF8("Bid Channel");
	_update_content_map["ofrChannel"] = GBKToUTF8("Ofr Channel");
	_update_content_map["bidBankAgentCode"] = GBKToUTF8("Bid交易员");
	_update_content_map["ofrBankAgentCode"] = GBKToUTF8("Ofr交易员");
	_update_content_map["dcFix"] = GBKToUTF8("Day Count Fix");
	_update_content_map["dcFloating"] = GBKToUTF8("Day Count Floating");
	_update_content_map["paymentCycle"] = GBKToUTF8("付息周期");
	_update_content_map["valueDate"] = GBKToUTF8("Value Date");
	_update_content_map["endDate"] = GBKToUTF8("End Date");
	_update_content_map["firstFloatDate"] = GBKToUTF8("First Floating Date");
	_update_content_map["deal"] = GBKToUTF8("交易币种");
	_update_content_map["floatingRateRef"] = GBKToUTF8("Floating Rate Reference");
	_update_content_map["term"] = GBKToUTF8("期限");
	_update_content_map["price"] = GBKToUTF8("价格");
	_update_content_map["volume"] = GBKToUTF8("数量");
	_update_content_map["category"] = GBKToUTF8("产品");
	_update_content_map["spotRate"] = GBKToUTF8("Spot Rate");
	_update_content_map["currency"] = GBKToUTF8("币种");
	_update_content_map["createTime"] = GBKToUTF8("交易时间");
	_update_content_map["tradeDay"] = GBKToUTF8("T+0");
	_update_content_map["productName"] = GBKToUTF8("利率类型");
	_update_content_map["interestDate"] = GBKToUTF8("interestDate");
	_update_content_map["maturityDate"] = GBKToUTF8("maturityDate");
	_update_content_map["bondShortName"] = GBKToUTF8("债券简称");
	_update_content_map["bondCode"] = GBKToUTF8("债券代码");
	_update_content_map["settlementDate"] = GBKToUTF8("交割日");
	_update_content_map["dirtyPrice"] = GBKToUTF8("全价");
	_update_content_map["yield"] = GBKToUTF8("Yield");
	_update_content_map["cleanPrice"] = GBKToUTF8("Net Price");
	_update_content_map["bidBankCity"] = GBKToUTF8("bidBankCity");
	_update_content_map["ofrBankCity"] = GBKToUTF8("ofrBankCity");
	_update_content_map["dealTime"] = GBKToUTF8("Trade Date");
}

//--------------------------------------------------------------------------------------------------------------------------------------------
//重写boost write_json接口，修复UTF-8中文处理问题
void DCSConnectionService::write_json(std::basic_ostream<ptree::key_type::value_type> &stream, const ptree &pt, bool pretty)
{
	write_json_helper(stream, pt, 0, pretty);
}

//from boost source code
void DCSConnectionService::write_json_helper(std::basic_ostream<ptree::key_type::value_type> &stream, const ptree &pt, int indent, bool pretty)
{

	typedef ptree::key_type::value_type Ch;
	typedef std::basic_string<Ch> Str;

	// Value or object or array
	if (indent > 0 && pt.empty())
	{
		// Write value
		Str data = create_escapes(pt.get_value<Str>()); 
		
		stream << Ch('"') << data << Ch('"');

	}
	else if (indent > 0 && pt.count(Str()) == pt.size())
	{
		// Write array
		stream << Ch('[');
		if (pretty) stream << Ch('\n');
		ptree::const_iterator it = pt.begin();
		for (; it != pt.end(); ++it)
		{
			if (pretty) stream << Str(4 * (indent + 1), Ch(' '));
			write_json_helper(stream, it->second, indent + 1, pretty);
			if (boost::next(it) != pt.end())
				stream << Ch(',');
			if (pretty) stream << Ch('\n');
		}
		stream << Str(4 * indent, Ch(' ')) << Ch(']');

	}
	else
	{
		// Write object
		stream << Ch('{');
		if (pretty) stream << Ch('\n');
		ptree::const_iterator it = pt.begin();
		for (; it != pt.end(); ++it)
		{
			if (pretty) stream << Str(4 * (indent + 1), Ch(' '));
			stream << Ch('"') << create_escapes(it->first) << Ch('"') << Ch(':');
			if (pretty) {
				if (it->second.empty())
					stream << Ch(' ');
				else
					stream << Ch('\n') << Str(4 * (indent + 1), Ch(' '));
			}
			write_json_helper(stream, it->second, indent + 1, pretty);
			if (boost::next(it) != pt.end())
				stream << Ch(',');
			if (pretty) stream << Ch('\n');
		}
		if (pretty) stream << Str(4 * indent, Ch(' '));
		stream << Ch('}');
	}

}

//from boost source code
std::basic_string<char> DCSConnectionService::create_escapes(const std::basic_string<char> &s)
{
	std::basic_string<char> result;
	std::basic_string<char>::const_iterator b = s.begin();
	std::basic_string<char>::const_iterator e = s.end();
	while (b != e)
	{
		// This assumes an ASCII superset. But so does everything in PTree.
		// We escape everything outside ASCII, because this code can't
		// handle high unicode characters.
		if (*b == 0x20 || *b == 0x21 || (*b >= 0x23 && *b <= 0x2E) ||
			(*b >= 0x30 && *b <= 0x5B) || (*b >= 0x5D && *b <= 0xFF)  //it fails here because char are signed
			|| (*b >= -0x80 && *b < 0)) // 增加此条件用于处理UTF8中文
			result += *b;
		else if (*b == char('\b')) result += char('\\'), result += char('b');
		else if (*b == char('\f')) result += char('\\'), result += char('f');
		else if (*b == char('\n')) result += char('\\'), result += char('n');
		else if (*b == char('\r')) result += char('\\'), result += char('r');
		else if (*b == char('/')) result += char('\\'), result += char('/');
		else if (*b == char('"'))  result += char('\\'), result += char('"');
		else if (*b == char('\\')) result += char('\\'), result += char('\\');
		else
		{
			const char *hexdigits = "0123456789ABCDEF";
			unsigned long u = (std::min)(static_cast<unsigned long>(
				static_cast<unsigned char>(*b)),
				0xFFFFul);
			int d1 = u / 4096; u -= d1 * 4096;
			int d2 = u / 256; u -= d2 * 256;
			int d3 = u / 16; u -= d3 * 16;
			int d4 = u;
			result += char('\\'); result += char('u');
			result += char(hexdigits[d1]); result += char(hexdigits[d2]);
			result += char(hexdigits[d3]); result += char(hexdigits[d4]);
		}
		++b;
	}
	return result;
}
