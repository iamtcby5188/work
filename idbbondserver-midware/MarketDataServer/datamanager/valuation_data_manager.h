#ifndef _VALUATION_DATA_MANAGER_H_
#define _VALUATION_DATA_MANAGER_H_

#include "sdbus/callback.h"
#include "sdbus/connection.h"
#include "logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include "cache/model/bondquote_cache.h"
#include "cache/model/bondquoterefer_cache.h"
#include "cache/model/bondbestquote_cache.h"
#include "cache/model/bonddeal_cache.h"
#include "cache/model/dcsbonddeal_cache.h"
#include <queue>
#include <boost/thread/thread.hpp>

class ValuationDataManager : public sdbus::Callback {

public:   
    ValuationDataManager(sdbus::Connection *conn);
    ~ValuationDataManager();

    bool init();

    virtual void OnMsgs(const sdbus::Message** msgs, int count);

    // 订阅消息处理线程
    void threadSubMsg(); 
    void processSubMsg(const sdbus::Message* msg);

    static int GetVersion() { return version_; }
    bool FetchValuationRemote();
	void FetchValuationLocal(BondQuoteCache& cache);
	void FetchValuationLocal(BondQuoteReferCache& cache);
	void FetchValuationLocal(BondBestQuoteCache &);
	void FetchValuationLocal(BondDealCache &);
    void FetchValuationLocal(DcsBondDealCache &);

private:
    sdbus::Connection *conn_;
    static int version_;

    //订阅消息队列
    std::queue<sdbus::Message> sub_msg_queue_;
    boost::mutex sub_queue_mutex_;
    boost::condition_variable_any sub_queue_cond_var_;

    boost::thread thrd;
};

#endif // _VALUATION_DATA_MANAGER_H_