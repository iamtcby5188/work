#ifndef _LOGIN_SERVICE_H
#define _LOGIN_SERVICE_H

#include "service/service.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

class LoginService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

protected:
    virtual bool tokenVerification(const sdbus::Message* msg);    // Login Service ÷ÿ‘ÿVerify Token∑Ω∑®

private:
    friend class ServiceManager;
    LoginService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void BondLogin(const sdbus::Message* msg, sdbus::Message& reply);
	void BondLogout(const sdbus::Message* msg, sdbus::Message& reply);
    void VerifyToken(const sdbus::Message* msg, sdbus::Message& reply);
	void startOffLineCheck();
	void runCheck();
	void checkOffLine(const boost::system::error_code& e, boost::asio::deadline_timer* t);
    void resetCache(const sdbus::Message* msg, sdbus::Message& reply);
};

#endif