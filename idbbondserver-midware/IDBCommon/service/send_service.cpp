#include "send_service.h"
#include "connection_manager.h"
#include "sdbus/connection.h"
#include "include/common.h"
#include "include/fid.h"
#include "include/constants.h"
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>

SendService* SendService::singleton_ = NULL;

SendService* SendService::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new SendService();
    } 

    return singleton_;
}

SendService::SendService()   
    : conn_(NULL)
{

}

SendService::~SendService()
{
    
}

void SendService::release() 
{
    if (conn_) {
        conn_->DelHandler(this);
    }
}

bool SendService::init(sdbus::Connection *conn, const std::string& queue_name)
{
    conn_ = conn;
    queue_name_ = queue_name;

    if (!Init(queue_name_, sdbus::DeliverDirect)) {
        return false;
    }
    if (!conn_->AddHandler(this)) {
        return false;
    }

    boost::function0<void> f = boost::bind(&SendService::threadRetMsg, this);
    boost::thread thrd(f); 

    return true;
}

void SendService::OnMsgs(const sdbus::Message ** msgs, int count)
{

    for(int i =0; i<count; ++i)
    {
        const sdbus::Message* msg = *(msgs + i);

        LOGGER_INFO("Get return message. [Type]: " << msg->GetType())
        boost::mutex::scoped_lock lock(ret_queue_mutex_);
        ret_msg_queue_.push(*msg);
        ret_queue_cond_var_.notify_one();       
    }
}

void SendService::threadRetMsg()
{    
    LOGGER_DEBUG("Start thread to process returned msg - " << queue_name_)

    while(true) {
        sdbus::Message msg;

        {
            boost::mutex::scoped_lock lock(ret_queue_mutex_);

            while(ret_msg_queue_.empty()) {        
                ret_queue_cond_var_.wait(lock);
            }

            msg = ret_msg_queue_.front();
            ret_msg_queue_.pop();
        }

        // 处理返回消息 
        processRetMsg(&msg);
    }
}

bool SendService::call(const sdbus::string& name, sdbus::Message& request, void(*callback)(void*) /*=NULL*/)
{
    if (conn_ == NULL) {
        return false;
    }

    std::string msg_id = GetNewGuid();
    request.SetMessageID(msg_id);
    request.SetReplyTo(queue_name_);

    if (callback) {
        ReqMsgPtr req_msg(new ReqMsg());
        req_msg->sync = false;
        req_msg->callback = callback;
        {
            boost::mutex::scoped_lock lock(mutex_);

            req_msg_mp_[msg_id] = req_msg;
        }
    }

    return conn_->Send(name, request);
}

bool SendService::call(const sdbus::string& name, sdbus::Message& request, sdbus::Message& response, int timeout /* = 1000 */)
{   
    if (conn_ == NULL) {
        return false;
    }

    std::string msg_id = GetNewGuid();
    request.SetMessageID(msg_id);
    request.SetReplyTo(queue_name_);

    ReqMsgPtr req_msg(new ReqMsg());
    req_msg->sync = true;
    req_msg->request_time = boost::get_system_time();
    req_msg->timeout = boost::posix_time::milliseconds(timeout);
    req_msg->ret = false;
    req_msg->response = &response;
	
	{
		boost::mutex::scoped_lock lock(mutex_);
		req_msg_mp_[msg_id] = req_msg;
	}

    conn_->Send(name, request);

    {
        boost::mutex::scoped_lock lock(mutex_);

        //req_msg_mp_[msg_id] = req_msg;

        while(!req_msg->ret && req_msg->request_time + req_msg->timeout > boost::get_system_time()) {
            cond_var_.timed_wait(lock, boost::posix_time::milliseconds(100));
        }

        req_msg_mp_.erase(msg_id);
    }

    return req_msg->ret;
}

void SendService::processRetMsg(const sdbus::Message* ret_msg)
{    
    boost::mutex::scoped_lock lock(mutex_);

    std::string msg_id = ret_msg->GetMessageID().c_str();
    std::map<std::string, ReqMsgPtr>::iterator it = req_msg_mp_.find(msg_id);
    if (it == req_msg_mp_.end()) {
        return;
    }

    ReqMsgPtr req_msg = (*it).second;   
    req_msg_mp_.erase(it);

    if (req_msg->sync) {    // 同步调用处理        
        req_msg->ret = true;
        if (req_msg->response) {
            req_msg->response->operator=(*ret_msg);
        }

        cond_var_.notify_all();
    } else {    // 异步调用处理
        if (req_msg->callback) {
            sdbus::Message* msg = new sdbus::Message(*ret_msg);
            req_msg->callback((void*)msg);
            delete msg;
        }
    }
}
