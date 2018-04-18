#ifndef _MSG_CACHE_H_
#define _MSG_CACHE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif

typedef struct 
{
	sdbus::string id;
	std::string server_name;
	std::string service_name;
	std::string method;
	std::string operator_id;
	std::string receive_msg_time; 
	std::string request_msg; 
	std::string reply_msg; 
}MsgCache;	

typedef std::tr1::shared_ptr<MsgCache> MsgCachePtr;

#endif // _MSG_CACHE_H_