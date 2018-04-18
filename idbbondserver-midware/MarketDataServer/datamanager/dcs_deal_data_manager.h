#ifndef _DCS_DEAL_DATA_MANAGER_H_
#define _DCS_DEAL_DATA_MANAGER_H_

#include "sdbus/callback.h"
#include "sdbus/connection.h"
#include "logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>
#include <boost/thread/thread.hpp>


class DcsDealDataManager : public sdbus::Callback {

public:   
    DcsDealDataManager(sdbus::Connection *conn, std::string companyId);
    ~DcsDealDataManager();
    
    bool init();

    virtual void OnMsgs(const sdbus::Message** msgs, int count);
    
    // 订阅消息处理线程
    void threadSubMsg(); 
    void processSubMsg(const sdbus::Message* msg);

    inline static int GetVersion(std::string company) { return version_map_[company]; }
    inline static void SetVersion(std::string company, int version) { version_map_[company] = version; }
    bool FetchDcsDealRemote();
    bool FetchIncrementDcsDealRemote(int version);

private:
    void AddList(const sdbus::VariantList &list);
    void UpdateList(const sdbus::VariantList &list);
    void DeleteList(const sdbus::VariantList &list);

    sdbus::Connection *conn_;
    static std::map<std::string, int> version_map_;
    
    //订阅消息队列
    std::queue<sdbus::Message> sub_msg_queue_;
    boost::mutex sub_queue_mutex_;
    boost::condition_variable_any sub_queue_cond_var_;

    std::string company_id_;
    boost::thread thrd;
};

#endif // _DCS_DEAL_DATA_MANAGER_H_