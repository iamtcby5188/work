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
    bool sync;                                  // ͬ���첽���ñ�־ false - �첽; true - ͬ��

    void(*callback)(void*);                     // �첽���ûص�����

    boost::system_time request_time;            // ͬ������������ʱ��
    boost::posix_time::time_duration timeout;   // ͬ�����ó�ʱʱ��
    sdbus::Message *response;                   // ͬ�����÷�������ָ��
    bool ret;                                   // ͬ�������Ƿ��ѷ��ر�־
}; 
typedef std::tr1::shared_ptr<ReqMsg> ReqMsgPtr;

class SendService : public sdbus::Callback {
public:
    static SendService* singleton();

    ~SendService();

    bool init(sdbus::Connection *conn, const std::string& queue_name);
    void release();

    virtual void OnMsgs(const sdbus::Message** msg, int count);

    // ������Ϣ�����߳�
    void threadRetMsg();

    bool call(const sdbus::string& name, sdbus::Message& request, void(*callback)(void*) =NULL);
    bool call(const sdbus::string& name, sdbus::Message& request, sdbus::Message& response, int timeout = 1000);

protected:
    friend class ServiceManager;
    SendService();

    // ��������Ϣ����
    void processRetMsg(const sdbus::Message* ret_msg);

    // ��������Ϣmp
    std::map<std::string, ReqMsgPtr> req_msg_mp_;
    boost::mutex mutex_;
    boost::condition_variable_any cond_var_;

    // ������Ϣ����
    std::queue<sdbus::Message> ret_msg_queue_;
    boost::mutex ret_queue_mutex_;
    boost::condition_variable_any ret_queue_cond_var_;

    sdbus::Connection *conn_;
    std::string queue_name_;    // ���ն���

    static SendService* singleton_;

};

#endif // _SEND_SERVICE_H_