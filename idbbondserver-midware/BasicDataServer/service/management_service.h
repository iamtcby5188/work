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
    virtual bool tokenVerification(const sdbus::Message* msg);    // ����Verify Token����

private:
    friend class ServiceManager;
    ManagementService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    // ��������ͬ��
    void syncTradingInstitution();

    // ����Ա����ͬ��
    void syncBondTrader();

    // ����ԱBroker��ͬ��
    void syncBrokerBindingTrader();

    // �û�����ͬ��
    void syncAccount();

    // �û�����ͬ��
    void syncAccountDepartment();

    // �û�Ȩ��ͬ��
    void syncAccountPermission();

    // ��ծ��ֵͬ��
    //void syncChinaBondValuation();

    // ��ծ��ֵƫ��ֵͬ��
    void syncChinaBondDeviatedValue();

    void resetCache(const sdbus::Message* msg, sdbus::Message& reply);

	void syncConstants();
};

#endif // _MANAGEMENT_SERVICE_H_