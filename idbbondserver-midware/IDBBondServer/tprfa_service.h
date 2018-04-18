#ifndef _TPRFA_SERVICE_H_
#define _TPRFA_SERVICE_H_

#include "logger.h"
#include "qpid/messaging/Connection.h"
#include "qpid/messaging/Session.h"
#include "qpid/messaging/Receiver.h"
#include "qpid/messaging/Sender.h"
#include "qpid/messaging/Address.h"
#include "qpid/messaging/Message.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>

class TPRFAService
{
public:
    static TPRFAService* singleton();

	~TPRFAService();

	bool Open(const std::string& url) ;
	void Close() ;

	inline const std::string& ServiceName() const {return _service_name;}
	inline void setServiceName(const std::string& name) { _service_name = name;}

    void PushMsg(const std::string& text);

    // 消息发送线程
    void threadSendMsg();

private:
    TPRFAService();
	void SendRequest(const std::string& text, const std::string subject);


	std::string _service_name;
	std::string _type;
    
    std::queue<std::string> msg_queue_;
    boost::mutex queue_mutex_;
    boost::condition_variable_any queue_cond_var_;

	::qpid::messaging::Connection* _connection;
	bool _finaled;

    static TPRFAService* singleton_;

};

#endif

