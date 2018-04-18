#ifndef _SYNC_SERVICE_H_
#define _SYNC_SERVICE_H_

#include "service/service.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

class SyncService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

    void startSync();

    // �����߳�
    void runSync();

private:
    friend class ServiceManager;

    SyncService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    // ��������,ÿ���賿ִ��
    static void resetCache(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* servic);

	// ��ծ��ֵͬ��
	static void syncChindBondValuation(const boost::system::error_code& e, boost::asio::deadline_timer* t, SyncService* servic);

};

#endif // _SYNC_SERVICE_H_