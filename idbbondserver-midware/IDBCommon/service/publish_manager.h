#ifndef _PUBLISH_MANAGER_H_
#define _PUBLISH_MANAGER_H_

#include "sdbus/connection.h"
#include "logger.h"
#include "fid.h"
#include <map>
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include "boost/shared_ptr.hpp"
#include <queue>

typedef struct {
    std::string topic_name;
    std::string model_name;
    boost::shared_ptr<sdbus::Message> msg;
    bool without_data;
    int version;
    std::string action_type;
	std::string company_id;
} PubMsg;

class PublishManager
{
public:
    static PublishManager* singleton();

    ~PublishManager();

    void init(sdbus::Connection *conn);

    int getVersion(const std::string &model_name, const sdbus::Message* msg);
	bool publish(const std::string &topic_name, const std::string &model_name, sdbus::Message &msg, bool without_data = false, const std::string action_type = "Sync");
    void publish(const PubMsg &pub_msg);
        
    // 消息推送线程
    void threadPubMsg();
private:
    PublishManager();

    std::queue<boost::shared_ptr<PubMsg>> msg_queue_;
    boost::mutex queue_mutex_;
    boost::condition_variable_any queue_cond_var_;

    //std::map<std::string, int> version_mp_;
    sdbus::Connection *conn_;

    static PublishManager* singleton_;

    boost::mutex mutex_;

};

#endif // _PUBLISH_MANAGER_H_