#ifndef _QB_SYNC_SERVICE_H_
#define _QB_SYNC_SERVICE_H_

#include "service/service.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>

class QBSyncService : public Service
{
#define SET_VALUE_NUM(mp, val){}
public:
    QBSyncService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    virtual void processReqMsg(const sdbus::Message* msg);
    
    void PublishBondQuote(const std::vector<BondQuoteCachePtr> &bond_quote_vec, int fid_method);
    void PublishBondDeal(const std::vector<BondDealCachePtr> &bond_deal_vec, int fid_method);
	void PublishBondBestQuote(const std::vector<BondBestQuoteCachePtr>& bond_best_quote_vec, int fid_method);
	void PublishBondBestQuote(const std::vector<BondQuoteCachePtr>& bond_quote_vec, const std::string& companyId);
	void PublishBondBestQuote(const std::vector<std::string>& bond_key_listed_market_vec, const std::string& companyId);

    void Publish(sdbus::Message &msg);
    
    // QB消息推送线程
    void threadPubMsg();

protected:
    virtual bool tokenVerification(const sdbus::Message* msg);    // QBSync Service 重载Verify Token方法

private:
	void SetBondQuoteMap(const BondQuoteCache *cache, sdbus::VariantMap *mp);
	void SetBondDealMap(const BondDealCache *cache, sdbus::VariantMap *mp);
	void setBondBestQuoteMap(const BondBestQuoteCache* cache, sdbus::VariantMap *mp);

    bool getPushMessageVersion(int &version);
    bool savePushMessageData(const sdbus::Message &msg, int version);
    bool loadPushMessageData(sdbus::Message &msg, int version);

	void getPushMessageData(const sdbus::Message *request, sdbus::Message& reply);
	void getPushMessageDataByTime(const sdbus::Message *request, sdbus::Message& reply);

	void procQuery(const std::string sql, sdbus::Message& reply);
	
    std::queue<sdbus::Message> msg_queue_;
    boost::mutex queue_mutex_;
    boost::condition_variable_any queue_cond_var_;

    boost::mutex mutex_;

	std::string mCompanyId; //Add by Young 05-Apr-2016

};

#endif // _QB_SYNC_SERVICE_H_