#ifndef _SERVICE_H_
#define _SERVICE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif
#include "sdbus/callback.h"
#include "logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>
#include <boost/thread/thread.hpp>

class Service : public sdbus::Callback {
public:
    bool init(int thread_num = 1);
    void ready() {is_ready_ = true;}
    ~Service();
    
    virtual void OnMsgs(const sdbus::Message** msg, int count);
    
    // �ⲿ������Ϣ�����߳�
    void threadReqMsg();
      
    // �����յ���������Ϣ����
    virtual void processReqMsg(const sdbus::Message* msg) = 0;


    void BeforeProcessReqMsg(const sdbus::Message*, std::vector<std::string>&, std::string&);
    void AfterProcessReqMsg(const std::vector<std::string>&, std::string&);

private:
    bool AttachService();

protected:
    Service(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    // ��֤������Ϣ��Token
    virtual bool tokenVerification(const sdbus::Message* msg);

    // �ⲿ������Ϣ����
    std::queue<sdbus::Message> req_msg_queue_;
    boost::mutex req_queue_mutex_;
    boost::condition_variable_any req_queue_cond_var_;

    sdbus::Connection *conn_;
    std::string queue_name_;    // ���ն���
    std::string topic_name_;    // ���Ͷ���

    bool is_ready_;
    
    std::vector<boost::thread*> thrd_vec_;
};

#endif // _SERVICE_H_