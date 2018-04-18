#include "management_service.h"
#include "include/fid.h"
#include "include/common.h"
#include "sdbus/connection.h"
#include "sync/tradinginstitution_sync.h"
#include "sync/bondtrader_sync.h"
#include "sync/brokerbindingtrader_sync.h"
#include "sync/account_sync.h"
#include "sync/chinabonddeviatedvalue_sync.h"
#include "cache/cache_manager.h"
#include "cache/cache_center.h"
#include "cache/cache_controller_declear.h"
#include "increment/increment_cache_manager.h"
ManagementService::ManagementService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{

}

void ManagementService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "ADD_INSTITUTION" || method == "UPDATE_INSTITUTION" || method == "DELETE_INSTITUTION") {
        syncTradingInstitution();
    } else if (method == "ADD_TRADER" || method == "UPDATE_TRADER" || method == "DELETE_TRADER") {
        syncBondTrader();
    } else if (method == "BIND_BROKER" || method == "BIND_TRADER" || method == "TRANSFER_TRADER") {
        syncBrokerBindingTrader();
    } else if (method == "ADD_USER" || method == "UPDATE_USER" || method == "DELETE_USER"
        || method == "ADD_ROLE" || method == "UPDATE_ROLE" || method == "DELETE_ROLE"
        || method == "ADD_DEPARTMENT" || method == "UPDATE_DEPARTMENT" || method == "DELETE_DEPARTMENT") {
        syncAccountDepartment();
    } else if (method == "UPDATE_CONSOLE_PERMISSION") {
        syncAccountPermission();
    /*} else if (method == "UPDATE_CHINA_BOND_VALUATION") {
        syncChinaBondValuation();*/
    } else if (method == "UPDATE_CHINA_BOND_DEVIATED_VALUE") {
        syncChinaBondDeviatedValue();
    } else if (method == "Cache.ResetCache") {
        resetCache(msg, reply);
	} else if (method == "ADD_TREASURY_FUTURE" || method == "UPDATE_TREASURY_FUTURE" || method == "DELETE_TREASURY_FUTURE") {
		syncConstants();
	}
    
    //bool flag = conn_->Send(msg->GetReplyTo(), reply);
    //if (flag) {
    //    LOGGER_INFO("Reply success: " << msg->GetType())
    //} else {
    //    LOGGER_ERROR("Reply failed: " << msg->GetType())
    //}
}

// 机构设置同步
void ManagementService::syncTradingInstitution()
{
    LOGGER_INFO("Start Sync Trading Institution...")

    TradingInstitutionSync tradinginstitution_sync;
    tradinginstitution_sync.sync();

    LOGGER_INFO("End Sync Trading Institution...")
}

// 交易员设置同步
void ManagementService::syncBondTrader()
{
    LOGGER_INFO("Start Sync Bond Trader...")

    BondTraderSync bondtrader_sync;
    bondtrader_sync.sync();

    LOGGER_INFO("End Sync Bond Trader...")
}

// 交易员Broker绑定同步
void ManagementService::syncBrokerBindingTrader()
{

    LOGGER_INFO("Start Sync Broker Binding Trader...")

    BrokerBindingTraderSync binding_sync;
    binding_sync.sync();

    LOGGER_INFO("End Sync Broker Binding Trader...")
}

// 用户设置同步
void ManagementService::syncAccount()
{
    LOGGER_INFO("Start Sync Account...")

    AccountSync account_sync;
    account_sync.sync();

    LOGGER_INFO("End Sync Account...")
}

// 用户设置同步
void ManagementService::syncAccountDepartment()
{
    LOGGER_INFO("Start Sync Account Department...")

    AccountSync account_sync;
    account_sync.syncDepartment();

    LOGGER_INFO("End Sync Account Department...")
}

// 用户设置同步
void ManagementService::syncAccountPermission()
{
    LOGGER_INFO("Start Sync Account Permission...")

    AccountSync account_sync;
    account_sync.syncPermission();

    LOGGER_INFO("End Sync Account Permission...")
}

// 中债估值同步
/*void ManagementService::syncChinaBondValuation()
{
    LOGGER_INFO("Start Sync China Bond Valuation...")

    ChinaBondValuationSync chinabondvaluation_sync;
    chinabondvaluation_sync.sync();

    LOGGER_INFO("End Sync China Bond Valuation...")      
}*/

// 中债估值偏移值同步
void ManagementService::syncChinaBondDeviatedValue()
{
    LOGGER_INFO("Start Sync China Bond Deviate Value...")

    ChinaBondDeviatedValueSync chinabonddeviatedvalue_sync;
    chinabonddeviatedvalue_sync.sync();

    LOGGER_INFO("End Sync China Bond Deviate Value...")      
}

// Login Service 重载Verify Token方法
// Login Service 无需验证Token
bool ManagementService::tokenVerification(const sdbus::Message* msg)
{   
    return true;
}

void ManagementService::resetCache(const sdbus::Message* msg, sdbus::Message& reply) {
    LOGGER_INFO("Start Reset Cache...");
    CacheCenter::sharedInstance()->clearCache();
    CacheManager::singleton()->InitCache();
    LOGGER_INFO("End Reset Cache...");
}

void ManagementService::syncConstants() {
	ConstantsCacheController ccc;
	ccc.cacheTable();
	ccc.commit();
}