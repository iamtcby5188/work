#ifndef _SEND_SERVICE_H_
#define _SEND_SERVICE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif
#include "sdbus/callback.h"
#include "logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>

class ReqMsg{
public:
    bool sync;                                  // 同步异步调用标志 false - 异步; true - 同步

    void(*callback)(void*);                     // 异步调用回调函数

    boost::system_time request_time;            // 同步调用请求发起时间
    boost::posix_time::time_duration timeout;   // 同步调用超时时间
    sdbus::Message *response;                   // 同步调用返回数据指针
    bool ret;                                   // 同步调用是否已返回标志
}; 
typedef std::tr1::shared_ptr<ReqMsg> ReqMsgPtr;

class SendService : public sdbus::Callback {
public:
    static SendService* singleton();

    ~SendService();

    bool init(sdbus::Connection *conn, const std::string& queue_name);
    void release();

    virtual void OnMsgs(const sdbus::Message** msg, int count);

    // 返回消息处理线程
    void threadRetMsg();

    bool call(const sdbus::string& name, sdbus::Message& request, void(*callback)(void*) =NULL);
    bool call(const sdbus::string& name, sdbus::Message& request, sdbus::Message& response, int timeout = 1000);

protected:
    friend class ServiceManager;
    SendService();

    // 处理返回消息函数
    void processRetMsg(const sdbus::Message* ret_msg);

    // 自身发送消息mp
    std::map<std::string, ReqMsgPtr> req_msg_mp_;
    boost::mutex mutex_;
    boost::condition_variable_any cond_var_;

    // 返回消息队列
    std::queue<sdbus::Message> ret_msg_queue_;
    boost::mutex ret_queue_mutex_;
    boost::condition_variable_any ret_queue_cond_var_;

    sdbus::Connection *conn_;
    std::string queue_name_;    // 接收队列

    static SendService* singleton_;

};

#endif // _SEND_SERVICE_H_