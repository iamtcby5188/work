#ifndef _MANAGEMENT_SERVICE_H_
#define _MANAGEMENT_SERVICE_H_

#include "service/service.h"
#include "object/management.h"

class ManagementService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

protected:
    virtual bool tokenVerification(const sdbus::Message* msg);    // 重载Verify Token方法

private:
    friend class ServiceManager;
    ManagementService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    // 机构设置同步
    void syncTradingInstitution();

    // 交易员设置同步
    void syncBondTrader();

    // 交易员Broker绑定同步
    void syncBrokerBindingTrader();

    // 用户设置同步
    void syncAccount();

    // 用户部门同步
    void syncAccountDepartment();

    // 用户权限同步
    void syncAccountPermission();

    // 中债估值同步
    //void syncChinaBondValuation();

    // 中债估值偏移值同步
    void syncChinaBondDeviatedValue();

    void resetCache(const sdbus::Message* msg, sdbus::Message& reply);

	void syncConstants();
};

#endif // _MANAGEMENT_SERVICE_H_