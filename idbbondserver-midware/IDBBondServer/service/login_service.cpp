#include "login_service.h"
#include "business/login_business.h"
#include "object/accountrole.h"
#include "include/fid.h"
#include "include/common.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"
#include "cache/cache_center.h"
#include "cache/controller/logintoken_cachecontroller.h"
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include "msg2db/msg_log_service.h"
#include "config.h"
#include "cache/cache_manager.h"
#include "datamanager/data_manager.h"
#include "increment/increment_cache_manager.h"
LoginService::LoginService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{
//	startOffLineCheck();
}

void LoginService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if(method == "Login.BondLogin"){
        BondLogin(msg, reply);
    }else if (method == "Login.BondLogout"){
		BondLogout(msg, reply);
    }else if (method == "Login.VerifyToken"){
        VerifyToken(msg, reply);
    } else if (method == "Cache.ResetCache") {
        resetCache(msg, reply);
    }
    
    if (msg->GetReplyTo().length() > 0) {   
        size_t len = -1;
        VariantMapCalcSize(reply.MapValue(), len);
        LOGGER_INFO("Message Size: " << len)

        bool flag = conn_->Send(msg->GetReplyTo(), reply);
        if (flag) {
            LOGGER_INFO("Reply success: " << msg->GetType())
        } else {
            LOGGER_ERROR("Reply failed: " << msg->GetType())
        }
    }
	MsgLogService::singleton()->SaveMsg(*msg,reply);
}

void LoginService::BondLogin(const sdbus::Message* msg, sdbus::Message& reply)
{
	LoginBusiness business(this);

    AccountRole account_role;
	std::string clientversion;
	msg->GetString(FID_CLIENT_VERSION, clientversion);
	account_role.client_version = clientversion;
	if (strncmp(clientversion.c_str(), Config::singleton()->getValue("ClientVersion").c_str(), 3) != 0) {
		account_role.ret_code = 3;
		account_role.ret_msg = GBKToUTF8("客户端版本错误！");
		LOGGER_INFO("Client Version Error")
	} else {
		msg->GetString(FID_ACCOUNT_NAME, account_role.name);
		msg->GetString(FID_ACCOUNT_PASSWORD, account_role.password);
		//msg->GetString(FID_ACCOUNT_COMPANY_ID, account_role.company_id); //Del by Young 05-Apr-2016

		//account_role.ip = "";//GetIpAddressFromString(msg->GetReplyTo().c_str());
		msg->GetString(FID_IPV4_ADDRESS, account_role.ip);
		msg->GetString(FID_MAC_ADDRESS, account_role.mac);

		business.BondLogin(account_role);
	}

    reply.SetString(FID_ACCOUNT_ID, account_role.account_id);
    reply.SetString(FID_ACCOUNT_NAME, account_role.name);
    reply.SetString(FID_ACCOUNT_PASSWORD, account_role.password);
    reply.SetString(FID_ACCOUNT_COMPANY_ID, account_role.company_id);
    reply.SetString(FID_ACCOUNT_COMPANY_NAME, account_role.company_name);
    reply.SetString(FID_ACCOUNT_TOKEN, account_role.token);
    reply.SetInt32(FID_LOGIN_RET_CODE, account_role.ret_code);
    reply.SetString(FID_LOGIN_RET_MSG, account_role.ret_msg);
    reply.SetString(FID_ACCOUNT_DEPARTMENT, account_role.department);
    reply.SetString(FID_ACCOUNT_ROLE, account_role.role);
    reply.SetString(FID_ACCOUNT_PERMISSION, account_role.permission);
}

void LoginService::BondLogout(const sdbus::Message* msg, sdbus::Message& reply)
{
	LoginBusiness business(this);

	AccountRole account_role;
	msg->GetString(FID_ACCOUNT_TOKEN, account_role.token);
	msg->GetString(FID_ACCOUNT_ID, account_role.account_id);

//	business.DeleteLoginToken(account_role);
	business.DeleteLoginLog(account_role);
}

void LoginService::VerifyToken(const sdbus::Message* msg, sdbus::Message& reply)
{
    LoginBusiness business(this);

    sdbus::string account_id, token;
    msg->GetString(FID_ACCOUNT_TOKEN, token);
    msg->GetString(FID_ACCOUNT_ID, account_id);

    bool ret = business.VerifyToken(token.c_str(), account_id.c_str());

    reply.SetBool(FID_LOGIN_TOKEN_VERIFY_RET, ret);
}

void LoginService::startOffLineCheck()
{
	boost::function0<void> f = boost::bind(&LoginService::runCheck, this);  
	boost::thread thrd(f); 
}

void LoginService::runCheck()
{    
	LOGGER_DEBUG("Start offline check thread")

	boost::asio::io_service io;

	boost::asio::deadline_timer t(io, boost::posix_time::seconds(60));
	t.async_wait(boost::bind(&LoginService::checkOffLine, this, boost::asio::placeholders::error, &t));

	io.run();
}

void LoginService::checkOffLine(const boost::system::error_code& e, boost::asio::deadline_timer* t)
{
	LOGGER_INFO("Start Check Off Line...")        
	LoginBusiness business(this);
	business.deleteCacheWhenOffLine();
	LOGGER_INFO("End Check Off Line...")

	t->expires_at(t->expires_at() + boost::posix_time::seconds(60));
	t->async_wait(boost::bind(&LoginService::checkOffLine, this, boost::asio::placeholders::error, t));
}

// Login Service 重载Verify Token方法
// Login Service 无需验证Token
bool LoginService::tokenVerification(const sdbus::Message* msg)
{   
    return true;
}

void LoginService::resetCache(const sdbus::Message* msg, sdbus::Message& reply) {
    LOGGER_INFO("Start Reset Cache...");
    CacheCenter::sharedInstance()->clearCache();
    CacheManager::singleton()->InitCompanyCache();
    DataManager::singleton()->reloadData();
    CacheManager::singleton()->InitCache();
    LOGGER_INFO("End Reset Cache...");
}