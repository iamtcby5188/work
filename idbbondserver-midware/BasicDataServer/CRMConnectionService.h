#ifndef _CRM_CONNECTION_SERVICE_H_
#define _CRM_CONNECTION_SERVICE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif

#include "cache/model/tradinginstitution_cache.h"
#include "cache/model/bondtrader_cache.h"
#include "cache/model/brokerbindingtrader_cache.h"

#include "qpid/messaging/Connection.h"
#include "qpid/messaging/Session.h"
#include "qpid/messaging/Message.h"
#include "CrmProto.pb.h"

#include <boost/lexical_cast.hpp>

using namespace qpid::messaging;
using namespace Crm;

class CRMConnectionService
{
public:
	static CRMConnectionService* singleton();

	~CRMConnectionService();

	void InitCrmParams();
	bool Open(const std::string& url);
	void Close();

	inline const std::string& RequestServiceName() const { return _request_service_name; }
	inline void setRequestServiceName(const std::string& name) { _request_service_name = name; }
	inline const std::string& ResponseServiceName() const { return _response_service_name; }
	inline void setResponseServiceName(const std::string& name) { _response_service_name = name; }
	inline const std::string& ServiceNameForInstitutionReciever() const { return _service_name_for_institution_recieve; }
	inline void setServiceNameForInstitutionReciever(const std::string& name) { _service_name_for_institution_recieve = name; }
	inline const std::string& ServiceNameForContactReciever() const { return _service_name_for_contact_recieve; }
	inline void setServiceNameForContactReciever(const std::string& name) { _service_name_for_contact_recieve = name; }
	inline const std::string& ServiceNameForManagerReciever() const { return _service_name_for_manager_recieve; }
	inline void setServiceNameForManagerReciever(const std::string& name) { _service_name_for_manager_recieve = name; }
	
	inline const std::string& UserName() const { return _user_name; }
	inline void setUserName(const std::string& name) { _user_name = name; }
	inline const std::string& Password() const { return _password; }
	inline void setPassword(const std::string& pwd) { _password = pwd; }
	inline const std::string& AccountId() const { return _account_id; }
	inline void setAccountId(const std::string& id) { _account_id = id; }
	inline const int& CrmId() const { return _crm_id; }
	inline void setCrmId(const std::string& id) { _crm_id = boost::lexical_cast<int>(id); }
	inline const std::string& CrmAppType() const { return _crm_app_type; }
	inline void setCrmAppType(const std::string& type) { _crm_app_type = type; }
	inline const int& CrmPageSize() const { return _crm_page_size; }
	inline void setCrmPageSize(const std::string& size) { _crm_page_size = boost::lexical_cast<int>(size); }
	inline const int& CrmPageNum() const { return _crm_page_num; }
	inline void setCrmPageNum(const std::string& num) { _crm_page_num = boost::lexical_cast<int>(num); }
	
	inline const int& MinRecordsNum() const { return _min_records_num; }
	inline void setMinRecordsNum(const std::string& num) { _min_records_num = boost::lexical_cast<int>(num); }

	void queryInstitutionList(bool is_init = false);

private:
	CRMConnectionService();
	void queryContactList();
	void Send(const std::string& request, std::string type_name);
	void threadResponseDoRecieveMsgs();
	void threadInstitutionDoRecieveMsgs();
	void threadContactDoRecieveMsgs();
	void threadManagerDoRecieveMsgs();
	bool responseRecieve();
	bool institutionRecieve();
	bool contactRecieve();
	bool managerRecieve();

	void institutionTransfer(const InstitutionItem& item, TradingInstitutionCachePtr& cache);
	bool institutionListTransfer(const GetInstListResponse& inst_response, TradingInstitutionCacheVecPtr& trading_inst_vec);
	void contactTransfer(const ContactItem& item, BondTraderCachePtr& cache);
	void accountMappingTransfer(const AccountMappingItem& item, BrokerBindingTraderCachePtr& cache);
	bool contactListTransfer(const GetCtctListResponse& ctct_response, BondTraderCacheVecPtr& bond_trader_vec, BrokerBindingTraderCacheVecPtr& broker_bind_trader_vec);

private:
	std::string _request_service_name;
	std::string _response_service_name;
	std::string _service_name_for_institution_recieve;
	std::string _service_name_for_contact_recieve; 
	std::string _service_name_for_manager_recieve;

	std::string _user_name;
	std::string _password;
	std::string _account_id;
	int _crm_id;
	std::string _crm_app_type;
	int _crm_page_size;
	int _crm_page_num;

	int _min_records_num;

	std::string _institution_request_id;
	std::string _contact_request_id;
	GetInstListResponse _inst_list_res;
	GetCtctListResponse _ctct_list_res;

	bool _is_open;
	bool _is_init; //是否程序启动时初始化cache标识
	Connection* _connection;

	static CRMConnectionService* singleton_;

};

#endif
