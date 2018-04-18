#ifndef _BOND_DATA_MANAGER_H_
#define _BOND_DATA_MANAGER_H_

#include "sdbus/callback.h"
#include "sdbus/connection.h"
#include "logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>
#include <boost/thread/thread.hpp>
#include "business/VersionBusiness.h"

class BondDataManager : public sdbus::Callback {

public:   
    BondDataManager(sdbus::Connection *conn);
    ~BondDataManager();
    
    bool init();

    virtual void OnMsgs(const sdbus::Message** msgs, int count);

    // 订阅消息处理线程
    void threadSubMsg(); 
    void processSubMsg(const sdbus::Message* msg);

    //static int GetVersion() { return version_; }
    int GetVersion();
    void SetVersion(const int& version);
    bool FetchBondRemote();
    bool FetchIncrementBondRemote(int version);

private:
    void AddList(const sdbus::VariantList &list);
    void UpdateList(const sdbus::VariantList &list);
    void DeleteList(const sdbus::VariantList &list);

    sdbus::Connection *conn_;
    //static int version_;

    //订阅消息队列
    std::queue<sdbus::Message> sub_msg_queue_;
    boost::mutex sub_queue_mutex_;
    boost::condition_variable_any sub_queue_cond_var_;
    
    VersionBusiness versionBusiness;

    boost::thread thrd;
};

#endif // _QUOTE_DATA_MANAGER_H_