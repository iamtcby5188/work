#include "CRMConnectionService.h"
#include "config.h"
#include "include/common.h"
#include "qpid/messaging/Session.h"
#include "qpid/messaging/Receiver.h"
#include "qpid/messaging/Sender.h"
#include "qpid/messaging/Address.h"
#include "qpid/messaging/Message.h"	
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp> 
#include "cache/cache_controller_declear.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "sync/tradinginstitution_sync.h"
#include "sync/bondtrader_sync.h"
#include "sync/brokerbindingtrader_sync.h"
#include "service/service_manager.h"
#include "service/bondservice.h"
#include "pinyin.h"

using namespace boost::posix_time;

CRMConnectionService* CRMConnectionService::singleton_ = NULL;

CRMConnectionService* CRMConnectionService::singleton()
{
	if (singleton_ == NULL) 
	{
		singleton_ = new CRMConnectionService();
	}

	return singleton_;
}

CRMConnectionService::CRMConnectionService(void)
	: _request_service_name(""), _response_service_name("")
	, _service_name_for_institution_recieve(""), _service_name_for_contact_recieve(""), _service_name_for_manager_recieve("")
	, _user_name(""), _password(""), _account_id(""), _crm_id(1), _crm_app_type(""), _crm_page_size(100), _crm_page_num(0)
	, _min_records_num(0)
	, _institution_request_id(""), _contact_request_id("")
	, _is_init(false)
	, _is_open(false)
	, _connection(NULL)
{
}

CRMConnectionService::~CRMConnectionService(void)
{
	Close();
}

void CRMConnectionService::InitCrmParams()
{
	std::string crm_request = Config::singleton()->getValue("CRM.RequestQueue", "UCP_CRM.IDB.REQUEST");
	setRequestServiceName(crm_request);

	std::string crm_response = Config::singleton()->getValue("CRM.ResponseQueue", "UCP_CRM.IDB.RESPONSE");
	setResponseServiceName(crm_response);

	std::string crm_institution_receive = Config::singleton()->getValue("CRM.InstitutionTopic", "UCP_CRM.UPDATE.INSTITUTION");
	setServiceNameForInstitutionReciever(crm_institution_receive);

	std::string crm_contact_receive = Config::singleton()->getValue("CRM.ContactTopic", "UCP_CRM.UPDATE.CONTACT");
	setServiceNameForContactReciever(crm_contact_receive);

	std::string crm_manager_receive = Config::singleton()->getValue("CRM.ManagerTopic", "UCP_CRM.UPDATE.MANAGER");
	setServiceNameForManagerReciever(crm_manager_receive);

	std::string crm_user_name = Config::singleton()->getValue("CRM.UserName", "IDB@xx.cn");
	setUserName(crm_user_name);

	std::string crm_password = Config::singleton()->getValue("CRM.Password", "e10adc3949ba59abbe56e057f20f883e");
	setPassword(crm_password);

	std::string crm_account_id = Config::singleton()->getValue("CRM.AccountId", "460");
	setAccountId(crm_account_id);

	std::string crm_id = Config::singleton()->getValue("CRM.CrmId", "1");
	setCrmId(crm_id);

	std::string crm_app_type = Config::singleton()->getValue("CRM.CrmAppType", "IDB");
	setCrmAppType(crm_app_type);

	std::string crm_page_size = Config::singleton()->getValue("CRM.CrmPageSize", "100");
	setCrmPageSize(crm_page_size);

	std::string crm_page_num = Config::singleton()->getValue("CRM.CrmPageNum", "0");
	setCrmPageNum(crm_page_num);

	std::string min_records_num = Config::singleton()->getValue("CRM.MinRecordsNum", "1000");
	setMinRecordsNum(min_records_num);
}

bool CRMConnectionService::Open(const std::string& url)
{
	LOGGER_DEBUG("Open CRM connection Service url[ " << url << " ]");

	try
	{
		_is_open = true;

		_connection = new Connection(url);
		_connection->setOption("reconnect", true);
		_connection->open();

		boost::function0<void> f1 = boost::bind(&CRMConnectionService::threadResponseDoRecieveMsgs, this);
		boost::thread thrd1(f1);

		boost::function0<void> f2 = boost::bind(&CRMConnectionService::threadInstitutionDoRecieveMsgs, this);
		boost::thread thrd2(f2);

		boost::function0<void> f3 = boost::bind(&CRMConnectionService::threadContactDoRecieveMsgs, this);
		boost::thread thrd3(f3);

		boost::function0<void> f4 = boost::bind(&CRMConnectionService::threadManagerDoRecieveMsgs, this);
		boost::thread thrd4(f4);
	}
	catch (const std::exception& error)
	{
		LOGGER_ERROR("exception raise, when Open CRM connection, errmsg[ " << error.what() << " ]");
	}

	return _connection->isOpen();
}

void CRMConnectionService::Close()
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
		LOGGER_ERROR("exception raise, when Close CRM connection, errmsg[ " << error.what() << " ]");
	}
}

void CRMConnectionService::threadResponseDoRecieveMsgs()
{
	while (_is_open)
	{
		responseRecieve();
	}
}

void CRMConnectionService::threadInstitutionDoRecieveMsgs()
{
	while (_is_open)
	{
		institutionRecieve();
	}
}

void CRMConnectionService::threadContactDoRecieveMsgs()
{
	while (_is_open)
	{
		contactRecieve();
	}
}

void CRMConnectionService::threadManagerDoRecieveMsgs()
{
	while (_is_open)
	{
		managerRecieve();
	}
}

bool CRMConnectionService::responseRecieve()
{
	if (_connection == NULL || !_connection->isOpen()) return false;

	Session session = _connection->createSession();
	std::string addr = _response_service_name + "; {create:always , node:{type:queue}}";
	Receiver receiver = session.createReceiver(addr);

	bool success = true;
	try
	{
		Message recieveMsg;
		while (receiver.fetch(recieveMsg, Duration::SECOND))
		{
			session.acknowledge();
			
			std::string type_name = recieveMsg.getProperties()["TypeName"].asString();

			ptime bt1(microsec_clock::local_time());
			LOGGER_INFO("responseRecieve Start processing message...... [Type]: " << type_name << " begin:[" << to_simple_string(microsec_clock::local_time().time_of_day()) << "]");

			if ("GetInstListResponse" == type_name)
			{
				_inst_list_res.ParseFromString(recieveMsg.getContent());

				CommonResponseFields common_res_fields = _inst_list_res.common_fields();
				const std::string request_id = common_res_fields.request_id();
				LOGGER_DEBUG("--------- responseRecieve _institution_request_id: " << _institution_request_id << ", request_id: " << request_id);

				if (_institution_request_id == request_id)
				{
					_institution_request_id = "";

					if ("0" == common_res_fields.return_code())
					{
						TradingInstitutionCacheVecPtr trading_inst_vec(new std::vector<TradingInstitutionCachePtr>());
						if (institutionListTransfer(_inst_list_res, trading_inst_vec))
						{
							TradingInstitutionSync inst_sync;
							inst_sync.sync(trading_inst_vec, _is_init);

							//成功同步机构数据之后，再请求同步联系人数据
							queryContactList();
						}
					}
				}
			}
			else if ("GetCtctListResponse" == type_name)
			{
				_ctct_list_res.ParseFromString(recieveMsg.getContent());

				CommonResponseFields common_res_fields = _ctct_list_res.common_fields();
				const std::string request_id = common_res_fields.request_id();
				LOGGER_DEBUG("--------- responseRecieve _contact_request_id: " << _contact_request_id << ", request_id: " << request_id);

				if (_contact_request_id == request_id)
				{
					_contact_request_id = "";
					if ("0" == common_res_fields.return_code())
					{
						BondTraderCacheVecPtr bond_trader_vec(new std::vector<BondTraderCachePtr>());
						BrokerBindingTraderCacheVecPtr broker_bind_trader_vec(new std::vector<BrokerBindingTraderCachePtr>());
						if (contactListTransfer(_ctct_list_res, bond_trader_vec, broker_bind_trader_vec))
						{
							BondTraderSync bond_trader_sync;
							bond_trader_sync.sync(bond_trader_vec, _is_init);
							BrokerBindingTraderSync broker_bind_trader_sync;
							broker_bind_trader_sync.sync(broker_bind_trader_vec, _is_init);

							_is_init = false;
						}
					}
				}
			}

			ptime bt2(microsec_clock::local_time());
			LOGGER_INFO("responseRecieve End  processing message. [Type]: " << type_name << " [Time costing]: " <<
				"[" << to_simple_string(bt2.time_of_day()) << "]-[" << to_simple_string(bt1.time_of_day()) << "]= " << (bt2.time_of_day() - bt1.time_of_day()).total_milliseconds() << " ms");
		}
	}
	catch (const std::exception& error)
	{
		success = false;
		_institution_request_id = "";
		_contact_request_id = "";

		LOGGER_ERROR("exception raise , when fetch message , errmsg[ " << error.what() << " ]");
	}

	session.sync();
	session.close();

	return success;
}

bool CRMConnectionService::institutionRecieve()
{
	if (_connection == NULL || !_connection->isOpen()) return false;

	Session session = _connection->createSession();
	std::string addr = _service_name_for_institution_recieve + "; {create:always , node:{type:topic}}";
	Receiver receiver = session.createReceiver(addr);

	BondService* bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);

	bool success = true;
	try
	{
		Message recieveMsg;
		while (receiver.fetch(recieveMsg, Duration::SECOND))
		{
			session.acknowledge();

			InstitutionUpdateEvent event;
			event.ParseFromString(recieveMsg.getContent());
			std::string type = event.eventtype();
			InstitutionItem item = event.institution();
			
			ptime bt1(microsec_clock::local_time());
			LOGGER_INFO("institutionRecieve Start processing message...... [Type]: " << type << " begin:[" << to_simple_string(microsec_clock::local_time().time_of_day()) << "]");
			LOGGER_DEBUG("--------- institutionRecieve inst_id: " << item.inst_id() << ", idb_key: " << item.idb_key());

			if (item.idb_key().empty())
			{
				continue;
			}

			TradingInstitutionCachePtr cache(new TradingInstitutionCache);
			institutionTransfer(item, cache);

			TradingInstitutionCacheController cc;
			std::string str = "id = '" + cache->id + "'";
			dbQuery q(str.c_str());

			std::vector<TradingInstitutionCache*> update_vec;
			std::vector<TradingInstitutionCache*> add_vec;
			std::vector<TradingInstitutionCache*> delete_vec;

			TradingInstitutionCachePtr tmp_cache = cc.getCacheByQueryInThreadSafty(q);
			if ("A" == type)
			{
				if (!tmp_cache) //用于处理Cache重复插入问题
				{
					cc.insertCacheInThreadSafty(cache.get());
					add_vec.push_back(cache.get()); //Publish
				}
			}
			else if ("U" == type)
			{
				if (tmp_cache) //若机构存在，则更新
				{
					cc.updateCacheInThreadSafty(q, cache.get());
					update_vec.push_back(cache.get()); //Publish
				}
				else //若机构不存在，则插入
				{
					cc.insertCacheInThreadSafty(cache.get());
					add_vec.push_back(cache.get()); //Publish
				}
			}
			else if ("D" == type)
			{
				cc.deleteCacheByQueryInThreadSafty(q);
				delete_vec.push_back(cache.get()); //Publish
			}

			//Publish
			if (update_vec.size() > 0) {
				bond_service->PublishInstitutionInfo(update_vec, "UpdateList");
			}

			if (add_vec.size() > 0) {
				bond_service->PublishInstitutionInfo(add_vec, "AddList");
			}

			if (delete_vec.size() > 0) {
				bond_service->PublishInstitutionInfo(delete_vec, "DeleteList");
			}

			ptime bt2(microsec_clock::local_time());
			LOGGER_INFO("institutionRecieve End  processing message. [Type]: " << type << " [Time costing]: " <<
				"[" << to_simple_string(bt2.time_of_day()) << "]-[" << to_simple_string(bt1.time_of_day()) << "]= " << (bt2.time_of_day() - bt1.time_of_day()).total_milliseconds() << " ms");
		}
	}
	catch (const std::exception& error) 
	{
		success = false;
		LOGGER_ERROR("exception raise , when fetch message , errmsg[ " << error.what() << " ]");
	}

	session.sync();
	session.close();

	return success;
}

bool CRMConnectionService::contactRecieve()
{
	if (_connection == NULL || !_connection->isOpen()) return false;

	Session session = _connection->createSession();
	std::string addr = _service_name_for_contact_recieve + "; {create:always , node:{type:topic}}";
	Receiver receiver = session.createReceiver(addr);

	BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);

	bool success = true;
	try
	{
		Message recieveMsg;
		while (receiver.fetch(recieveMsg, Duration::SECOND))
		{
			session.acknowledge();

			ContactUpdateEvent event;
			event.ParseFromString(recieveMsg.getContent());
			std::string type = event.eventtype();
			ContactItem ctct_item = event.contact();
			
			ptime bt1(microsec_clock::local_time());
			LOGGER_INFO("contactRecieve Start processing message...... [Type]: " << type << " begin:[" << to_simple_string(microsec_clock::local_time().time_of_day()) << "]");
			LOGGER_DEBUG("--------- contactRecieve ctct_id: " << ctct_item.ctct_id() << ", idb_key: " << ctct_item.idb_key() << ", biz_type: " << ctct_item.biz_type());

			if (ctct_item.idb_key().empty())
			{
				continue;
			}

			BondTraderCachePtr bond_trader_cache(new BondTraderCache);
			contactTransfer(ctct_item, bond_trader_cache);

			BrokerBindingTraderCacheController broker_bind_trader_cc;
			int accout_map_size = ctct_item.account_mapping_size();

			BondTraderCacheController bond_trader_cc;
			std::string str = "id = '" + bond_trader_cache->id + "'";
			dbQuery bond_trader_q(str.c_str());

			std::vector<BondTraderCache*> trader_update_vec;
			std::vector<BondTraderCache*> trader_add_vec;
			std::vector<BondTraderCache*> trader_delete_vec;

			//std::vector<BrokerBindingTraderCache*> binding_update_vec;
			std::vector<BrokerBindingTraderCache*> binding_add_vec;
			std::vector<BrokerBindingTraderCache*> binding_delete_vec;

			BrokerBindingTraderCacheVecPtr binding_tmp_vec(new std::vector<BrokerBindingTraderCachePtr>());

			BondTraderCachePtr tmp_cache = bond_trader_cc.getCacheByQueryInThreadSafty(bond_trader_q);
			if ("A" == type)
			{
				if (!tmp_cache) //用于处理Cache重复插入问题
				{
					//------add BondTrader
					bond_trader_cc.insertCacheInThreadSafty(bond_trader_cache.get());
					trader_add_vec.push_back(bond_trader_cache.get()); //Publish

					//------add BrokerBindingTrader
					for (int i = 0; i < accout_map_size; ++i)
					{
						AccountMappingItem item = ctct_item.account_mapping(i);
						LOGGER_DEBUG("--------- contactRecieve account_id: " << item.account_id() << ", idb_key: " << item.idb_key());
						if (item.idb_key().empty())
						{
							continue;
						}

						BrokerBindingTraderCachePtr cache(new BrokerBindingTraderCache);
						accountMappingTransfer(item, cache);
						binding_tmp_vec->push_back(cache); //用于保存BrokerBindingTraderCachePtr智能指针，防止crash
						broker_bind_trader_cc.insertCacheInThreadSafty(cache.get());
						binding_add_vec.push_back(cache.get()); //Publish
					}
				}
			}
			else if ("U" == type)
			{
				//------update BondTrader
				if (tmp_cache) //若交易员存在，则更新
				{
					bond_trader_cc.updateCacheInThreadSafty(bond_trader_q, bond_trader_cache.get());
					trader_update_vec.push_back(bond_trader_cache.get()); //Publish
				}
				else //若交易员不存在，则插入
				{
					bond_trader_cc.insertCacheInThreadSafty(bond_trader_cache.get());
					trader_add_vec.push_back(bond_trader_cache.get()); //Publish
				}

				//------update BrokerBindingTrader
				//for (int i = 0; i < accout_map_size; ++i)
				//{
				//	AccountMappingItem item = ctct_item.account_mapping(i);
				//	if (item.idb_key().empty())
				//	{
				//		continue;
				//	}

				//	BrokerBindingTraderCachePtr cache(new BrokerBindingTraderCache);
				//	accountMappingTransfer(item, cache);
				//	binding_tmp_vec->push_back(cache); //用于保存BrokerBindingTraderCachePtr智能指针，防止crash

				//	std::string str = "id = '" + cache->id + "'";
				//	dbQuery q(str.c_str());

				//	broker_bind_trader_cc.updateCacheInThreadSafty(q, cache.get());
				//	binding_update_vec.push_back(cache.get()); //Publish
				//}
			}
			else if ("D" == type)
			{
				//------delete BondTrader
				bond_trader_cc.deleteCacheByQueryInThreadSafty(bond_trader_q);
				trader_delete_vec.push_back(bond_trader_cache.get()); //Publish

				//------delete BrokerBindingTrader
				for (int i = 0; i < accout_map_size; ++i)
				{
					AccountMappingItem item = ctct_item.account_mapping(i);
					LOGGER_DEBUG("--------- contactRecieve account_id: " << item.account_id() << ", idb_key: " << item.idb_key());
					if (item.idb_key().empty())
					{
						continue;
					}

					BrokerBindingTraderCachePtr cache(new BrokerBindingTraderCache);
					accountMappingTransfer(item, cache);
					binding_tmp_vec->push_back(cache); //用于保存BrokerBindingTraderCachePtr智能指针，防止crash

					std::string str = "id = '" + cache->id + "'";
					dbQuery q(str.c_str());

					broker_bind_trader_cc.deleteCacheByQueryInThreadSafty(q);
					binding_delete_vec.push_back(cache.get()); //Publish
				}
			}

			//Publish
			if (trader_update_vec.size() > 0) {
				bond_service->PublishTraderInfo(trader_update_vec, "UpdateList");
			}

			if (trader_add_vec.size() > 0) {
				bond_service->PublishTraderInfo(trader_add_vec, "AddList");
			}

			if (trader_delete_vec.size() > 0) {
				bond_service->PublishTraderInfo(trader_delete_vec, "DeleteList");
			}

			/*if (binding_update_vec.size() > 0) {
				bond_service->PublishBrokerBindingTrader(binding_update_vec, "UpdateList");
			}*/

			if (binding_add_vec.size() > 0) {
				bond_service->PublishBrokerBindingTrader(binding_add_vec, "AddList");
			}

			if (binding_delete_vec.size() > 0) {
				bond_service->PublishBrokerBindingTrader(binding_delete_vec, "DeleteList");
			}

			ptime bt2(microsec_clock::local_time());
			LOGGER_INFO("contactRecieve End  processing message. [Type]: " << type << " [Time costing]: " <<
				"[" << to_simple_string(bt2.time_of_day()) << "]-[" << to_simple_string(bt1.time_of_day()) << "]= " << (bt2.time_of_day() - bt1.time_of_day()).total_milliseconds() << " ms");
		}
	}
	catch (const std::exception& error)
	{
		success = false;
		LOGGER_ERROR("exception raise , when fetch message , errmsg[ " << error.what() << " ]");
	}

	session.sync();
	session.close();

	return success;
}

bool CRMConnectionService::managerRecieve()
{
	if (_connection == NULL || !_connection->isOpen()) return false;

	Session session = _connection->createSession();
	std::string addr = _service_name_for_manager_recieve + "; {create:always , node:{type:topic}}";
	Receiver receiver = session.createReceiver(addr);

	BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);

	bool success = true;
	try
	{
		Message recieveMsg;
		while (receiver.fetch(recieveMsg, Duration::SECOND))
		{
			session.acknowledge();

			ManagerUpdateEvent event;
			event.ParseFromString(recieveMsg.getContent());
			std::string type = event.eventtype();
			int map_size = event.mappings_size();

			ptime bt1(microsec_clock::local_time());
			LOGGER_INFO("managerRecieve Start processing message...... [Type]: " << type << " begin:[" << to_simple_string(microsec_clock::local_time().time_of_day()) << "]");
			LOGGER_DEBUG("--------- managerRecieve map_size: " << map_size);

			BrokerBindingTraderCacheController broker_bind_trader_cc;
			std::vector<BrokerBindingTraderCache*> binding_update_vec;
			std::vector<BrokerBindingTraderCache*> binding_add_vec;
			std::vector<BrokerBindingTraderCache*> binding_delete_vec;

			BrokerBindingTraderCacheVecPtr binding_tmp_vec(new std::vector<BrokerBindingTraderCachePtr>());

			if ("A" == type)
			{
				//------add BrokerBindingTrader
				for (int i = 0; i < map_size; ++i)
				{
					AccountMappingItem item = event.mappings(i);
					LOGGER_DEBUG("--------- managerRecieve account_id: " << item.account_id() << ", idb_key: " << item.idb_key());
					if (item.idb_key().empty())
					{
						continue;
					}

					BrokerBindingTraderCachePtr cache(new BrokerBindingTraderCache);
					accountMappingTransfer(item, cache);
					binding_tmp_vec->push_back(cache); //用于保存BrokerBindingTraderCachePtr智能指针，防止crash

					std::string str = "id = '" + cache->id + "'";
					dbQuery q(str.c_str());
					BrokerBindingTraderCachePtr tmp_cache = broker_bind_trader_cc.getCacheByQueryInThreadSafty(q);
					if (!tmp_cache) //用于处理Cache重复插入问题
					{
						broker_bind_trader_cc.insertCacheInThreadSafty(cache.get());
						binding_add_vec.push_back(cache.get()); //Publish
					}
				}
			}
			else if ("U" == type)
			{
				//------update BrokerBindingTrader
				for (int i = 0; i < map_size; ++i)
				{
					AccountMappingItem item = event.mappings(i);
					LOGGER_DEBUG("--------- managerRecieve account_id: " << item.account_id() << ", idb_key: " << item.idb_key());
					if (item.idb_key().empty())
					{
						continue;
					}

					BrokerBindingTraderCachePtr cache(new BrokerBindingTraderCache);
					accountMappingTransfer(item, cache);
					binding_tmp_vec->push_back(cache); //用于保存BrokerBindingTraderCachePtr智能指针，防止crash

					std::string str = "id = '" + cache->id + "'";
					dbQuery q(str.c_str());
					BrokerBindingTraderCachePtr tmp_cache = broker_bind_trader_cc.getCacheByQueryInThreadSafty(q);
					if (tmp_cache) //若绑定关系存在，则更新
					{
						broker_bind_trader_cc.updateCacheInThreadSafty(q, cache.get());
						binding_update_vec.push_back(cache.get()); //Publish
					}
					else //若绑定关系不存在，则插入
					{
						broker_bind_trader_cc.insertCacheInThreadSafty(cache.get());
						binding_add_vec.push_back(cache.get()); //Publish
					}
				}
			}
			else if ("D" == type)
			{
				//------delete BrokerBindingTrader
				for (int i = 0; i < map_size; ++i)
				{
					AccountMappingItem item = event.mappings(i);
					LOGGER_DEBUG("--------- managerRecieve account_id: " << item.account_id() << ", idb_key: " << item.idb_key());
					if (item.idb_key().empty())
					{
						continue;
					}

					BrokerBindingTraderCachePtr cache(new BrokerBindingTraderCache);
					accountMappingTransfer(item, cache);
					binding_tmp_vec->push_back(cache); //用于保存BrokerBindingTraderCachePtr智能指针，防止crash

					std::string str = "id = '" + cache->id + "'";
					dbQuery q(str.c_str());

					broker_bind_trader_cc.deleteCacheByQueryInThreadSafty(q);
					binding_delete_vec.push_back(cache.get()); //Publish
				}
			}

			//Publish
			if (binding_update_vec.size() > 0) {
				bond_service->PublishBrokerBindingTrader(binding_update_vec, "UpdateList");
			}

			if (binding_add_vec.size() > 0) {
				bond_service->PublishBrokerBindingTrader(binding_add_vec, "AddList");
			}

			if (binding_delete_vec.size() > 0) {
				bond_service->PublishBrokerBindingTrader(binding_delete_vec, "DeleteList");
			}

			ptime bt2(microsec_clock::local_time());
			LOGGER_INFO("managerRecieve End  processing message. [Type]: " << type << " [Time costing]: " <<
				"[" << to_simple_string(bt2.time_of_day()) << "]-[" << to_simple_string(bt1.time_of_day()) << "]= " << (bt2.time_of_day() - bt1.time_of_day()).total_milliseconds() << " ms");
		}
	}
	catch (const std::exception& error)
	{
		success = false;
		LOGGER_ERROR("exception raise , when fetch message , errmsg[ " << error.what() << " ]");
	}

	session.sync();
	session.close();

	return success;
}

void CRMConnectionService::Send(const std::string& request, std::string type_name)
{
	if (_connection == NULL || !_connection->isOpen() || request.empty()) return;

	Session session = _connection->createSession();
	std::string sendAddr = _request_service_name + "; {create:always , node:{type:queue}}";
	Sender sender = session.createSender(sendAddr);

	try
	{
		Message requestMsg;
		requestMsg.setProperty("TypeName", type_name);
		requestMsg.setContent(request);
		sender.send(requestMsg);
	}
	catch (const std::exception& error)
	{
		LOGGER_ERROR("exception raise , when send message , errmsg[ " << error.what() << " ]");
	}

	session.sync();
	session.close();
}

void CRMConnectionService::queryInstitutionList(bool is_init)
{
	if (!_is_open) return;

	_is_init = is_init;

	std::string request;
	GetInstListRequest inst_list_req;

	CommonRequestFields* common_req = new CommonRequestFields();
	_institution_request_id = GetNewGuid();
	common_req->set_request_id(_institution_request_id);
	common_req->set_username(_user_name);
	common_req->set_password(_password);
	common_req->set_currentaccountid(_account_id);
	common_req->set_crm_id(_crm_id);
	common_req->set_application_type(_crm_app_type);
	common_req->set_page_size(_crm_page_size);
	common_req->set_page_number(_crm_page_num);

	inst_list_req.set_allocated_common_fields(common_req);

	inst_list_req.SerializeToString(&request);

	LOGGER_DEBUG("--------- queryInstitutionList type: GetInstListRequest, request_id: " << _institution_request_id);

	Send(request, "GetInstListRequest");
}

void CRMConnectionService::queryContactList()
{
	if (!_is_open) return;

	std::string request;
	GetCtctListRequest ctct_list_req;
	CommonRequestFields* common_req = new CommonRequestFields();
	_contact_request_id = GetNewGuid();
	common_req->set_request_id(_contact_request_id);
	common_req->set_username(_user_name);
	common_req->set_password(_password);
	common_req->set_currentaccountid(_account_id);
	common_req->set_crm_id(_crm_id);
	common_req->set_application_type(_crm_app_type);
	common_req->set_page_size(_crm_page_size);
	common_req->set_page_number(_crm_page_num);

	ctct_list_req.set_allocated_common_fields(common_req);
	
	ctct_list_req.SerializeToString(&request);

	LOGGER_DEBUG("--------- queryContactList type: GetCtctListRequest, request_id: " << _contact_request_id);

	Send(request, "GetCtctListRequest"); 
}

void CRMConnectionService::institutionTransfer(const InstitutionItem& item, TradingInstitutionCachePtr& cache)
{
	cache->crm_id = boost::lexical_cast<int>(item.inst_id());
	cache->id = item.idb_key();
	cache->company_id = item.company_id();
	if (cache->company_id.empty() || ("" == cache->company_id))
	{
		cache->company_id = "1";
	}

	cache->name = item.inst_short_name_cn();
	cache->name_en = item.inst_short_name_en();
	cache->full_name = item.inst_full_name_cn();
	cache->full_name_en = item.inst_full_name_en();
	cache->pinyin = item.pinyin();
	if (cache->pinyin.empty() || ("" == cache->pinyin))
	{
		cache->pinyin = getInitialWithSeperator(item.inst_short_name_cn(), "_");
	}

	/*if (cache->pinyin.length() > 255)
	{
		cache->pinyin = cache->pinyin.substr(0, 255);
	}*/

	cache->pinyin_full = item.pinyin_full();
	if (cache->pinyin_full.empty() || ("" == cache->pinyin_full))
	{
		cache->pinyin_full = getFullWithSeperator(item.inst_short_name_cn(), "_");
	}

	cache->code = item.inst_code();
	cache->city = item.inst_city_code();
	cache->city_name = item.inst_city_value();
	cache->description = item.desc();
	cache->modify_time = item.update_time();
	cache->create_time = item.update_time();
	cache->attribute = item.inst_type();
	cache->attribute2 = item.is_internal();

	cache->status = "1";
}

bool CRMConnectionService::institutionListTransfer(const GetInstListResponse& inst_response, TradingInstitutionCacheVecPtr& trading_inst_vec)
{
	int size = inst_response.result_size();
	LOGGER_DEBUG("------------------- institutionListTransfer size: " << size);
	//if (0 == size) return false; 
	if (size <= _min_records_num) return false;

	for (int i = 0; i < size; ++i)
	{
		InstitutionItem inst_item = inst_response.result(i);
		if (!inst_item.idb_key().empty())
		{
			TradingInstitutionCachePtr trading_inst(new TradingInstitutionCache);
			institutionTransfer(inst_item, trading_inst);
			trading_inst_vec->push_back(trading_inst);
		}
		else
		{
			LOGGER_ERROR("--------------- error InstitutionItem inst_id: " << inst_item.inst_id() << ", idb_key: " << inst_item.idb_key());
		}
	}

	if (trading_inst_vec->size() <= 0)
	{
		return false;
	}

	return true;
}

void CRMConnectionService::contactTransfer(const ContactItem& item, BondTraderCachePtr& cache)
{
	cache->crm_id = boost::lexical_cast<int>(item.ctct_id());
	cache->id = item.idb_key();
	cache->company_id = item.company_id();
	if (cache->company_id.empty() || ("" == cache->company_id))
	{
		cache->company_id = "1";
	}

	cache->name = item.ctct_name();
	cache->pinyin = item.pinyin();
	if (cache->pinyin.empty() || ("" == cache->pinyin))
	{
		cache->pinyin = getInitialWithSeperator(item.ctct_name(), "_");
	}

	cache->pinyin_full = item.pinyin_full();
	if (cache->pinyin_full.empty() || ("" == cache->pinyin_full))
	{
		cache->pinyin_full = getFullWithSeperator(item.ctct_name(), "_");
	}

	cache->bank_id = item.inst_id();
	cache->code = item.inst_code();
	cache->f_code = item.ctct_code();
	cache->flag_vip = item.is_vip();
	cache->flag_bad = item.is_danger();
	cache->attribute4 = item.biz_type();
	cache->modify_time = item.update_time();
	cache->create_time = item.update_time();

	cache->status = "1";
}

void CRMConnectionService::accountMappingTransfer(const AccountMappingItem& item, BrokerBindingTraderCachePtr& cache)
{
	cache->account_id = item.account_id();
	cache->bank_agent_id = item.ctct_id();

	cache->id = item.idb_key();
	cache->status = "1";
	cache->sort = 1;
	cache->create_time = GetCurrentTimeString();
	CacheCenter::sharedInstance()->AttachCurrentThread();
	cache->loadCombinationFields();
	CacheCenter::sharedInstance()->DetachCurrentThread();
}

bool CRMConnectionService::contactListTransfer(const GetCtctListResponse& ctct_response, BondTraderCacheVecPtr& bond_trader_vec, BrokerBindingTraderCacheVecPtr& broker_bind_trader_vec)
{
	int ctct_size = ctct_response.result_size();
	LOGGER_DEBUG("------------------- contactListTransfer ctct_size: " << ctct_size)
	//if (0 == ctct_size) return false;
	if (ctct_size <= _min_records_num) return false;

	for (int ctct_index = 0; ctct_index < ctct_size; ++ctct_index)
	{
		//BondTraderCache
		ContactItem ctct_item = ctct_response.result(ctct_index);
		if (!ctct_item.idb_key().empty())
		{
			BondTraderCachePtr bond_trader(new BondTraderCache);
			contactTransfer(ctct_item, bond_trader);
			bond_trader_vec->push_back(bond_trader);

			//BrokerBindingTraderCache
			int accout_map_size = ctct_item.account_mapping_size();
			for (int account_index = 0; account_index < accout_map_size; ++account_index)
			{
				AccountMappingItem account_map_item = ctct_item.account_mapping(account_index);
				if (!account_map_item.idb_key().empty())
				{
					BrokerBindingTraderCachePtr broker_bind_trader(new BrokerBindingTraderCache);
					accountMappingTransfer(account_map_item, broker_bind_trader);
					broker_bind_trader_vec->push_back(broker_bind_trader);
				}
			}
		}
		else
		{
			LOGGER_ERROR("---------------- error ContactItem ctct_id: " << ctct_item.ctct_id() << ", idb_key: " << ctct_item.idb_key());
		}
	}

	if (bond_trader_vec->size() <= 0)
	{
		return false;
	}

	return true;
}
