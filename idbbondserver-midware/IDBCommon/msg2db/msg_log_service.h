#ifndef _MSG_LOG_SERVICE_H_
#define _MSG_LOG_SERVICE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif
#include "sdbus/callback.h"
#include "logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include "msg_cache.h"
#include "sdbus/message.h"
#include <list>

#include "cache/model/bondquote_cache.h"
#include "cache/model/bondquoterefer_cache.h"
#include "cache/model/bonddeal_cache.h"

class MsgLogService{
public:
	static MsgLogService* singleton();
	bool init(const std::string &server_name, int thread_num = 1);
	void FilterAndSaveMsg(const sdbus::Message &req_msg,const sdbus::Message &reply_msg);
	void SaveMsg(const sdbus::Message &req_msg,const sdbus::Message &reply_msg);
	~MsgLogService();

    void writeQuoteLog(const sdbus::Message* request, const std::vector<BondQuoteCachePtr>& vecs);
    void writeQuoteLog(const sdbus::Message* request, const std::vector<BondQuoteReferCachePtr>& vecs);
    void writeDealLog(const sdbus::Message* request, const std::vector<BondDealCachePtr>& vecs);

private:
	MsgLogService();
	void startThread();
	bool processMsg(MsgCachePtr msg);
	bool processMsg4DB(MsgCachePtr msg);
	bool processMsg4File(MsgCachePtr msg);
	bool FilterMsg(const std::string &type);
	bool filterMethod(const std::string &filter_msg, const char **mng_filter_key, int size);

private:
	std::list<MsgCachePtr> msg_list;
	boost::mutex mutex_;
	boost::condition_variable_any cond_var_;
	
	bool is_enable_;
	int thread_num_;
	std::string msg_destination;
	std::string server_name_;

	unsigned long squence_;
	
	static MsgLogService *singleton_;
	static log4cxx::LoggerPtr logger_msg_;
};

#endif // _MSG_LOG_SERVICE_H_