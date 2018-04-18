#include "msg_log_service.h"
#include "include/common.h"
#include "include/fid.h"
#include "include/constants.h"
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>
#include "db/dbconnector.h"
#include "../config.h"
#include "msg_helper.h"
#include "include/cache_def.h"
#include <log4cxx/logger.h>

#define INSERT_DATABASE_BEGIN_LOCAL(table_name)\
	std::vector<std::string> fieldList;			 \
	std::vector<std::string> conditionList;			 \
	std::string sqlStr = "insert into " + std::string(table_name) + " set ";\

#define INSERT_STRING_FIELD_LOCAL(field_name, val)							\
{																		\
	std::ostringstream s;												\
	\
	s << #field_name << " = " << "\'" << val << "\'";	\
	fieldList.push_back(s.str());										\
}	
#define INSERT_NULLABLE_STRING_FIELD_LOCAL(field_name, val)						\
{   \
    std::ostringstream ss;  \
    ss << val;  \
    if (!ss.str().empty()){							            \
		INSERT_STRING_FIELD_LOCAL(field_name, val)  \
    }												\
}
#define INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(field_name, val)								\
{																		\
	if(val != DECIMAL_NULL){    \
        std::ostringstream s;												\
	    s << #field_name << " = " << std::setprecision(16) << val;      \
	    fieldList.push_back(s.str());   \
    }   \
}

#define INSERT_NULLABLE_BOOLEAN_LOCAL(field_name, val)								\
{	\
    INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(field_name, val ? 1 : 0)    \
}

#define INSERT_DATABASE_END_LOCAL(table_name, schema)         \
	if (fieldList.size() == 0)                 \
	{                                           \
		LOGGER_DEBUG("All fields are identical, no need to update table - [" << table_name << "]");\
		return 0;                               \
	}                                           \
	std::string fieldString = join(fieldList, ",");	\
	sqlStr += fieldString;						\
	return DBConnector::getInstance()->executeUpdate(sqlStr, schema);						    \

#define INSERT_DATABASE_END_LOCAL_NORETURN(table_name, schema)         \
	std::string fieldString = join(fieldList, ",");	\
	sqlStr += fieldString;						\
	DBConnector::getInstance()->executeUpdate(sqlStr, schema);						    \

static const char * comm_filter_key[]=
{
	"add",
	"edit",
	"update",
	"delete",
};

static const char * quote_filter_key[]=
{
	"add",
	"edit",
	"update",
	"delete",
	"gvntkn",
	"refer",
};

static const char * quote_filter_key_except[]=
{
	"referquotelist", //ReferQuoteList
};

static const char * basic_filter_key[]=
{
	"update",
};

MsgLogService* MsgLogService::singleton_ = NULL;
log4cxx::LoggerPtr MsgLogService::logger_msg_;
MsgLogService* MsgLogService::singleton()
{
	if (singleton_ == NULL) {
		singleton_ = new MsgLogService();
	}

	return singleton_;
}

MsgLogService::MsgLogService()
{
	is_enable_ = false;
	thread_num_ = 1;
	msg_destination = "db";
	squence_ = 0;
}

MsgLogService::~MsgLogService()
{
	if (singleton_) {
		delete singleton_;
	}
}

bool MsgLogService::init(const std::string &server_name, int thread_num)
{
	//init enable flag
	std::string is_enable = Config::singleton()->getValue("LogMsg.Enable", "false");
	transform(is_enable.begin(),is_enable.end(),is_enable.begin(), ::tolower);
	if ("true" == is_enable)
	{
		is_enable_ = true;
	}
	else
	{
		is_enable_ = false;
	}
	if (!is_enable_)
	{
		return false;
	}

	//init server_name_
	server_name_ = server_name;
	std::size_t pos = server_name.find_last_of("/\\");
	if (std::string::npos!=pos)
	{
		server_name_ = server_name_.substr(pos+1);
	}

	//msg destination
	msg_destination = Config::singleton()->getValue("LogMsg.Destination", "db");
	transform(msg_destination.begin(),msg_destination.end(),msg_destination.begin(), ::tolower);
	if (msg_destination == "file")
	{
		logger_msg_ = log4cxx::Logger::getLogger("msg_log");
	}

	//init thread num
	thread_num_ = atoi(Config::singleton()->getValue("LogMsg.ThreadCount", "1").c_str());
	if (thread_num_ < 1) thread_num_ = 1;
	if (thread_num_ > 5) thread_num_ = 5;
	if (msg_destination == "file")
	{
		thread_num_ =1;
	}

	boost::function0<void> f = boost::bind(&MsgLogService::startThread, this);  
	//start thread
	for (int i = 0; i < thread_num_; ++i) {
		boost::thread thrd(f); 
	}

	return true;
}

void MsgLogService::startThread()
{    
	LOGGER_DEBUG("Start message to db thread......");
	MsgCachePtr msg;
	while(true) {
		{
			boost::mutex::scoped_lock lock(mutex_);
			while(msg_list.empty()) {        
				cond_var_.wait(lock);
			}

			msg = msg_list.front();
			msg_list.pop_front();
		}
		processMsg(msg);
	}
}

void MsgLogService::FilterAndSaveMsg(const sdbus::Message &req_msg,const sdbus::Message &reply_msg)
{
	if (!is_enable_)
	{
		return;
	}

	std::string type = req_msg.GetType().c_str();
	std::string service;
	std::string method;
	if ("50189" == type)
	{
		service = "Calculator";
		method = "Calculator";
	}
	else
	{
		if(!FilterMsg(type))
		{
			return;
		}
		//get service name and method
		std::size_t pos = type.find('.');
		if(pos==std::string::npos)
		{
			return;
		}
		service = type.substr(0,pos);
		method = type.substr(pos+1);
	}

	//get operator id
	std::string operator_id;
	req_msg.GetString(FID_ACCOUNT_ID, operator_id);

	//save message information
	MsgCachePtr msg(new MsgCache());
	msg->id = GetNewGuid();
	msg->receive_msg_time = GetCurrentTimeString();
	msg->server_name = server_name_;
	msg->service_name = service;
	msg->method = method;
	msg->operator_id = operator_id;
	MsgHelper::MessageToString(req_msg, msg->request_msg );
	MsgHelper::MessageToString(reply_msg, msg->reply_msg );
	{
		boost::mutex::scoped_lock lock(mutex_);
		msg_list.push_back(msg);
		cond_var_.notify_one();
	}
}

void MsgLogService::SaveMsg(const sdbus::Message &req_msg,const sdbus::Message &reply_msg)
{
	if (!is_enable_)
	{
		return;
	}

	//get service name and method
	std::string type = req_msg.GetType().c_str();
	std::size_t pos = type.find('.');
	if(pos==std::string::npos)
	{
		return;
	}
	std::string service = type.substr(0,pos);
	std::string method = type.substr(pos+1);

	//get operator id
	std::string operator_id;
	req_msg.GetString(FID_ACCOUNT_ID, operator_id);

	//save message information
	MsgCachePtr msg(new MsgCache());
	msg->id = GetNewGuid();
	msg->receive_msg_time = GetCurrentTimeString();
	msg->server_name = server_name_;
	msg->service_name = service;
	msg->method = method;
	msg->operator_id = operator_id;
	MsgHelper::MessageToString(req_msg, msg->request_msg );
	MsgHelper::MessageToString(reply_msg, msg->reply_msg );
	{
		boost::mutex::scoped_lock lock(mutex_);
		msg_list.push_back(msg);
		cond_var_.notify_one();
	}
}

bool MsgLogService::processMsg(MsgCachePtr msg)
{
	if (msg_destination == "file")
	{
		return processMsg4File(msg);
	}
	else
	{
		return processMsg4DB(msg);
	}
	return true;
}

bool MsgLogService::processMsg4DB(MsgCachePtr msg)
{
	INSERT_DATABASE_BEGIN_LOCAL("bond_log");
	INSERT_STRING_FIELD_LOCAL(id, msg->id);
	INSERT_STRING_FIELD_LOCAL(server_name, msg->server_name);
	INSERT_STRING_FIELD_LOCAL(service_name, msg->service_name);	
	INSERT_STRING_FIELD_LOCAL(method, msg->method);
	INSERT_STRING_FIELD_LOCAL(operator_id, msg->operator_id);
	INSERT_STRING_FIELD_LOCAL(receive_msg_time, msg->receive_msg_time);
	INSERT_STRING_FIELD_LOCAL(request_msg, msg->request_msg);
	INSERT_STRING_FIELD_LOCAL(reply_msg,  msg->reply_msg);
	INSERT_STRING_FIELD_LOCAL(create_time, GetCurrentTimeString());
	INSERT_DATABASE_END_LOCAL("bond_log", SCHEMA_IDB_BOND);
}

bool MsgLogService::processMsg4File(MsgCachePtr msg)
{
	LOG4CXX_INFO(logger_msg_, '\n' << "------------message sequence: " << squence_++ <<"------------"<< '\n'  
		<<"[receive time] " <<  msg->receive_msg_time << '\n' 
		<<"[operator id]" <<  msg->operator_id << '\n' 
		<<"[server name]" <<  msg->server_name << '\n' 
		<<"[service name]" <<  msg->service_name << '\n' 
		);
	return true;
}

bool MsgLogService::FilterMsg(const std::string &type)
{
	std::string method = type; 	
	std::size_t pos = method.find('.');
	if(pos==std::string::npos)
	{
		return false;
	}

	std::string prefix = method.substr(0,pos);
	method = method.substr(pos+1);
	
	if ("BondQuote" == prefix)//quote
	{
		if(!filterMethod(method, quote_filter_key, sizeof(quote_filter_key)/sizeof(char *)))
		{
			return false;
		}
		if(filterMethod(method, quote_filter_key_except, sizeof(quote_filter_key_except)/sizeof(char *)))
		{
			return false;
		}
		return true;
	}
	else if ("Bond" == prefix) //basic server
	{
		return filterMethod(method, basic_filter_key, sizeof(basic_filter_key)/sizeof(char *));
	}	
	else //default ---- known: deal/group
	{
		return filterMethod(method, comm_filter_key, sizeof(comm_filter_key)/sizeof(char *));
	}
	
	return false;
}

bool MsgLogService::filterMethod(const std::string &filter_msg, const char **filter_key, int size)
{
	std::string body = filter_msg;
	transform(body.begin(),body.end(),body.begin(), ::tolower);
	for (int i=0; i<size;++i)
	{
		if (body.find(filter_key[i])!= std::string::npos)
		{
			return true;
		}
	}
	return false;
}

void MsgLogService::writeQuoteLog(const sdbus::Message* request, const std::vector<BondQuoteCachePtr>& vecs) {
    if (!request || vecs.empty())
        return;
    std::string accountId, accountToken, method = std::string(request->GetType().c_str()), msgId = std::string(request->GetMessageID().c_str());
    request->GetString(FID_ACCOUNT_ID, accountId);
    request->GetString(FID_ACCOUNT_TOKEN, accountToken);
    BOOST_FOREACH(BondQuoteCachePtr cache, vecs) {
        INSERT_DATABASE_BEGIN("bond_offer_log");
        INSERT_NULLABLE_STRING_FIELD_LOCAL(id, GetNewGuid());
        INSERT_NULLABLE_STRING_FIELD_LOCAL(method, method);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(message_id, msgId);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(login_token, accountToken);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(oper_id, accountId);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(oper_time, GetCurrentTimeString());

        INSERT_NULLABLE_STRING_FIELD_LOCAL(offer_id, cache->id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(company_id, cache->company_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_id, cache->goods_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_code, cache->goods_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(profit_code, cache->profit_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bank_name, cache->bank_name);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bank_id, cache->bank_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(description, cache->description);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(symbol, cache->symbol);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(price, cache->price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(volume, cache->volume);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_status, cache->deal_status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_time, cache->deal_time);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(operator_id, cache->operator_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(review_id, cache->review_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(create_time, cache->create_time);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(status, cache->status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bank_agent_id, cache->bank_agent_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(agent_code, cache->agent_code);
        INSERT_NULLABLE_BOOLEAN_LOCAL(flag_vip, cache->flag_vip);
        INSERT_NULLABLE_BOOLEAN_LOCAL(flag_bad, cache->flag_bad);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(flag_bargain, cache->flag_bargain);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(price_description, cache->price_description);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_type, cache->goods_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_short_name, cache->goods_short_name);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_level, cache->goods_level);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_term, cache->goods_term);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_name, cache->goods_name);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(option_type, cache->option_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(rate_type, cache->rate_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(internally, cache->internally);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(flag_relation, cache->flag_relation);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(trader_id, cache->trader_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(quote_type, cache->quote_type);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(return_point, cache->return_point);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(net_price, cache->net_price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(full_price, cache->full_price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(yield, cache->yield);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(spread, cache->spread);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(clear_speed, cache->clear_speed);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(delivery_date, cache->delivery_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(exercise, cache->exercise);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(rebate, cache->rebate);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(send_status, cache->send_status);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(calculate_status, cache->calculate_status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bond_key, cache->bond_key);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(listed_market, cache->listed_market);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bond_key_listed_market, cache->bond_key_listed_market);

        INSERT_DATABASE_END_LOCAL_NORETURN("bond_offer_log", SCHEMA_IDB_BOND);
    }
}
void MsgLogService::writeQuoteLog(const sdbus::Message* request, const std::vector<BondQuoteReferCachePtr>& vecs) {
    if (!request || vecs.empty())
        return;
    std::string accountId, accountToken, method = std::string(request->GetType().c_str()), msgId = std::string(request->GetMessageID().c_str());
    request->GetString(FID_ACCOUNT_ID, accountId);
    request->GetString(FID_ACCOUNT_TOKEN, accountToken);
    BOOST_FOREACH(BondQuoteReferCachePtr cache, vecs) {
        INSERT_DATABASE_BEGIN("bond_offer_log");
        INSERT_NULLABLE_STRING_FIELD_LOCAL(id, GetNewGuid());
        INSERT_NULLABLE_STRING_FIELD_LOCAL(method, method);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(message_id, msgId);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(login_token, accountToken);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(oper_id, accountId);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(oper_time, GetCurrentTimeString());

        INSERT_NULLABLE_STRING_FIELD_LOCAL(offer_id, cache->id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(company_id, cache->company_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_id, cache->goods_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_code, cache->goods_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(profit_code, cache->profit_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bank_name, cache->bank_name);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bank_id, cache->bank_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(description, cache->description);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(symbol, cache->symbol);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(price, cache->price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(volume, cache->volume);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_status, cache->deal_status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_time, cache->deal_time);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(operator_id, cache->operator_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(review_id, cache->review_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(create_time, cache->create_time);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(status, cache->status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bank_agent_id, cache->bank_agent_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(agent_code, cache->agent_code);
        INSERT_NULLABLE_BOOLEAN_LOCAL(flag_vip, cache->flag_vip);
        INSERT_NULLABLE_BOOLEAN_LOCAL(flag_bad, cache->flag_bad);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(flag_bargain, cache->flag_bargain);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(price_description, cache->price_description);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_type, cache->goods_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_short_name, cache->goods_short_name);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_level, cache->goods_level);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_term, cache->goods_term);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_name, cache->goods_name);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(option_type, cache->option_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(rate_type, cache->rate_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(internally, cache->internally);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(flag_relation, cache->flag_relation);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(trader_id, cache->trader_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(quote_type, cache->quote_type);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(return_point, cache->return_point);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(net_price, cache->net_price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(full_price, cache->full_price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(yield, cache->yield);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(spread, cache->spread);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(clear_speed, cache->clear_speed);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(delivery_date, cache->delivery_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(exercise, cache->exercise);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(rebate, cache->rebate);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(send_status, cache->send_status);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(calculate_status, cache->calculate_status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bond_key, cache->bond_key);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(listed_market, cache->listed_market);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bond_key_listed_market, cache->bond_key_listed_market);

        INSERT_DATABASE_END_LOCAL_NORETURN("bond_offer_log", SCHEMA_IDB_BOND);
    }
}
void MsgLogService::writeDealLog(const sdbus::Message* request, const std::vector<BondDealCachePtr>& vecs) {
    if (!request || vecs.empty())
        return;
    std::string accountId, accountToken, method = std::string(request->GetType().c_str()), msgId = std::string(request->GetMessageID().c_str());
    request->GetString(FID_ACCOUNT_ID, accountId);
    request->GetString(FID_ACCOUNT_TOKEN, accountToken);
    BOOST_FOREACH(BondDealCachePtr cache, vecs) {
        INSERT_DATABASE_BEGIN("bond_deal_log");
        INSERT_NULLABLE_STRING_FIELD_LOCAL(id, GetNewGuid());
        INSERT_NULLABLE_STRING_FIELD_LOCAL(method, method);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(message_id, msgId);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(login_token, accountToken);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(oper_id, accountId);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(oper_time, GetCurrentTimeString());

        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_id, cache->id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(company_id, cache->company_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_time, cache->deal_time);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_bank_id, cache->bid_bank_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_bank_code, cache->bid_bank_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_bank_city, cache->bid_bank_city);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_agent_id, cache->bid_agent_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_agent_code, cache->bid_agent_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_broker_id, cache->bid_broker_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_broker_name, cache->bid_broker_name);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(bid_brokerrage, cache->bid_brokerrage);
        INSERT_NULLABLE_BOOLEAN_LOCAL(bid_check, cache->bid_check);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_bank_id, cache->ofr_bank_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_bank_code, cache->ofr_bank_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_bank_city, cache->ofr_bank_city);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_agent_id, cache->ofr_agent_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_agent_code, cache->ofr_agent_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_broker_id, cache->ofr_broker_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_broker_name, cache->ofr_broker_name);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(ofr_brokerrage, cache->ofr_brokerrage);
        INSERT_NULLABLE_BOOLEAN_LOCAL(ofr_check, cache->ofr_check);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(price, cache->price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(volume, cache->volume);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(currency, cache->currency);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(full_price, cache->full_price);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(net_price, cache->net_price);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(settlement_date, cache->settlement_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(delivery_type, cache->delivery_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_id, cache->goods_id);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_code, cache->goods_code);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(goods_short_name, cache->goods_short_name);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(maturity_date, cache->maturity_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_status, cache->deal_status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(description, cache->description);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(status, cache->status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(create_time, cache->create_time);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_bridge, cache->bid_bridge);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_charge, cache->bid_charge);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_bridge, cache->ofr_bridge);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_charge, cache->ofr_charge);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(deal_type, cache->deal_type);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(group_flag, cache->group_flag);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(time_style, cache->time_style);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(confirm_account, cache->confirm_account);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(confirm_date, cache->confirm_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_check_date, cache->bid_check_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_check_date, cache->ofr_check_date);
        INSERT_NULLABLE_BOOLEAN_LOCAL(bid_trader_check, cache->bid_trader_check);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_trader_check_date, cache->bid_trader_check_date);
        INSERT_NULLABLE_BOOLEAN_LOCAL(ofr_trader_check, cache->ofr_trader_check);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_trader_check_date, cache->ofr_trader_check_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(remarks, cache->remarks);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(odd_numbers, cache->odd_numbers);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(exercise, cache->exercise);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(is_print, cache->is_print);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(return_point, cache->return_point);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(spread, cache->spread);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(clear_speed, cache->clear_speed);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(delivery_date, cache->delivery_date);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(rebate, cache->rebate);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(internally, cache->internally);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(send_status, cache->send_status);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(quote_type, cache->quote_type);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(yield, cache->yield);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bond_key, cache->bond_key);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(listed_market, cache->listed_market);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bond_key_listed_market, cache->bond_key_listed_market);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(traded_date, cache->traded_date);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(sequence_number, cache->sequence_number);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(total_price, cache->total_price);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bid_remarks, cache->bid_remarks);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofr_remarks, cache->ofr_remarks);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(bid_brokerage_rate, cache->bid_brokerage_rate);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(ofr_brokerage_rate, cache->ofr_brokerage_rate);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(bid_discount, cache->bid_discount);
        INSERT_NULLABLE_DECIMAL_FIELD_LOCAL(ofr_discount, cache->ofr_discount);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(updateTime, cache->updateTime);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(urgent, cache->urgent);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBankAgentName, cache->bidBankAgentName);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerIdB, cache->bidBrokerIdB);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerIdC, cache->bidBrokerIdC);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerIdD, cache->bidBrokerIdD);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerCodeA, cache->bidBrokerCodeA);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerCodeB, cache->bidBrokerCodeB);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerCodeC, cache->bidBrokerCodeC);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerCodeD, cache->bidBrokerCodeD);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidPercentA, cache->bidPercentA);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidPercentB, cache->bidPercentB);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidPercentC, cache->bidPercentC);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidPercentD, cache->bidPercentD);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidIsNoBrokerage, cache->bidIsNoBrokerage);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidBrokerrageRemark, cache->bidBrokerrageRemark);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidTradeMode, cache->bidTradeMode);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidNoConfirm, cache->bidNoConfirm);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(bidNoComments, cache->bidNoComments);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBankAgentName, cache->ofrBankAgentName);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerIdB, cache->ofrBrokerIdB);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerIdC, cache->ofrBrokerIdC);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerIdD, cache->ofrBrokerIdD);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerCodeA, cache->ofrBrokerCodeA);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerCodeB, cache->ofrBrokerCodeB);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerCodeC, cache->ofrBrokerCodeC);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerCodeD, cache->ofrBrokerCodeD);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrPercentA, cache->ofrPercentA);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrPercentB, cache->ofrPercentB);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrPercentC, cache->ofrPercentC);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrPercentD, cache->ofrPercentD);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrIsNoBrokerage, cache->ofrIsNoBrokerage);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrBrokerrageRemark, cache->ofrBrokerrageRemark);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrTradeMode, cache->ofrTradeMode);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrNoConfirm, cache->ofrNoConfirm);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(ofrNoComments, cache->ofrNoComments);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(specialPartyBid, cache->specialPartyBid);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(specialPartyOfr, cache->specialPartyOfr);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(specialTerms, cache->specialTerms);
        INSERT_NULLABLE_STRING_FIELD_LOCAL(feedback, cache->feedback);

        INSERT_DATABASE_END_LOCAL_NORETURN("bond_deal_log", SCHEMA_IDB_BOND);

    }
}
