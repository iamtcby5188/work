#include "tprfa_service.h"
#include <boost/locale/encoding.hpp>	
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include "config.h"
TPRFAService* TPRFAService::singleton_ = NULL;

TPRFAService* TPRFAService::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new TPRFAService();
    }

    return singleton_;
}

TPRFAService::TPRFAService()
{
	_connection = NULL;
	_finaled = false;
}


TPRFAService::~TPRFAService()
{
	Close();
}

bool TPRFAService::Open(const std::string& url)
{
    LOGGER_DEBUG("Open TPRFA Service")

	_connection = new qpid::messaging::Connection(url) ;
	_connection->setOption("reconnect" , true) ;
	_connection->setOption("heartbeat", Config::singleton()->getIntValue("SDBus.Main.HeartBeat", 5));
	_connection->open() ;

    boost::function0<void> f = boost::bind(&TPRFAService::threadSendMsg, this);  
    boost::thread thrd(f); 

	return _connection->isOpen() ;
}

void TPRFAService::Close()
{
	if(_connection != NULL)
	{
		_connection->close() ;

		delete _connection ;
		_connection = NULL ;
	}
}

void TPRFAService::PushMsg(const std::string& text) 
{    
    if (_connection == NULL || !_connection->isOpen()) {
        return;
    }

    boost::mutex::scoped_lock lock(queue_mutex_);
    msg_queue_.push(text);
    queue_cond_var_.notify_one();
}

void TPRFAService::SendRequest(const std::string& text, const std::string subject) {
    if (_connection == NULL || !_connection->isOpen()) {
        return;
    }

	qpid::messaging::Session session = _connection->createSession() ;

	std::string addr = _service_name + "; {create:always , node:{type:queue}}" ;
	qpid::messaging::Sender sender = session.createSender(addr) ;


	qpid::messaging::Message msg ;
	msg.setSubject(subject);
	//msg.setContentType("application/octet-stream");

	std::string sendStr = boost::locale::conv::between(text, "UTF-8", "GBK");
	//std::string sendStr = boost::locale::conv::between(text, "GBK", "UTF-8");
	msg.setContent(text);

	//msg.setContent(text);
	sender.send(msg) ;     

	session.sync() ;
    session.close() ;
    LOGGER_INFO("Publish Succeed Message Resend Service")
}

void TPRFAService::threadSendMsg()
{    
    LOGGER_DEBUG("Send TPRFA Msg")

    while(true) {
        std::string text;

        {
            boost::mutex::scoped_lock lock(queue_mutex_);

            while(msg_queue_.empty()) {        
                queue_cond_var_.wait(lock);
            }

            text = msg_queue_.front();
            msg_queue_.pop();
        }
        
        SendRequest(text, "Non-Subject");

    }
}
