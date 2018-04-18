#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "login_business.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/controller/logintoken_cachecontroller.h"
#include "db/dbconnector.h"
#include "include/common.h"

LoginBusiness::LoginBusiness(LoginService* service)
    : service_(service)
{
    Init();
}

void LoginBusiness::Init()
{

}

void LoginBusiness::BondLogin(AccountRole& account_role)
{
    AccountCacheController account_cc;   
    dbQuery q;
	q = "username = ", account_role.name; //Modify by Young 05-Apr-2016
    LOGGER_INFO(account_role.name);
    AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);

    if (!cache) {
        account_role.ret_code = 1;
        account_role.ret_msg = GBKToUTF8("该用户不存在！");
		LOGGER_INFO("The user is not exist!")
        return;
    }

    if (cache->password != account_role.password) {
        account_role.ret_code = 2;
        account_role.ret_msg = GBKToUTF8("密码错误！");
		LOGGER_INFO("The password is incorrect!")
        return;
    }

    account_role.ret_code = 0;
    account_role.ret_msg = "";
    account_role.account_id = cache->id;
    account_role.company_name = cache->company_name;
    account_role.token = GetNewGuid();
    account_role.department = cache->department_codes;
    account_role.role = cache->role_codes;
    account_role.permission = cache->permission_codes;
	account_role.display_name = cache->display_name;
	account_role.company_id = cache->company_id; //Modify by Young 05-Apr-2016

//    SaveLoginToken(account_role);
    SaveLoginLog(account_role);

    return;
}

void LoginBusiness::SaveLoginToken(const AccountRole &account_role)
{
	LoginTokenCachePtr logintoken_cache(new LoginTokenCache());

	logintoken_cache->id = account_role.token;
	logintoken_cache->account_id = account_role.account_id;
	logintoken_cache->token = account_role.token;
	logintoken_cache->activation = "1";
	logintoken_cache->client_type = "1";
	logintoken_cache->client_version = account_role.client_version;
	logintoken_cache->ip = account_role.ip;
	logintoken_cache->mac = account_role.mac;
	logintoken_cache->server = "IDB LOGIN SYSTEM";
	logintoken_cache->status = "1";
	logintoken_cache->last_update_time = GetCurrentTimeString();

	bool succeed = LoginTokenCacheController::singleton()->insertCache(logintoken_cache->id, logintoken_cache);
	if (!succeed){
		LOGGER_ERROR("InsertLoginToken: Failed for add login token: " << logintoken_cache->id)
	}
}

void LoginBusiness::DeleteLoginToken(const AccountRole &account_role)
{
	std::vector<std::string> del_list;
	del_list.push_back(account_role.token.c_str());

	bool succeed = LoginTokenCacheController::singleton()->deleteCache(del_list);
	if (!succeed){
		LOGGER_ERROR("DeleteLoginToken: Failed for delete login_status: " << account_role.token)
	}
}

void LoginBusiness::SaveLoginLog(const AccountRole &account_role)
{
	std::string sqlStr = "insert into idb_login_log(id,login_status_id,account_id,username,display_name,client_type,client_version,mac,ip,start_time,end_time,status) values('" + GetNewGuid() +
		                 "','" + account_role.token.c_str() +
						 "','" + account_role.account_id.c_str() +
						 "','" + account_role.name.c_str() +
						 "','" + account_role.display_name.c_str() + 
						 "','1','" + account_role.client_version.c_str() +
						 "','" + account_role.mac.c_str() + 
						 "','" + account_role.ip.c_str() +
						 "','" + GetCurrentTimeString() + "',NULL,'1')";
	DBConnector::getInstance()->executeUpdate(sqlStr, SCHEMA_IDB_ACCOUNT);
}

void LoginBusiness::DeleteLoginLog(const AccountRole &account_role)
{
	std::string sqlStr = "update idb_login_log set END_TIME=\'" +
		                 GetCurrentTimeString() + "\' where LOGIN_STATUS_ID=\'" +
						 account_role.token.c_str() + "\' and ACCOUNT_ID=\'" +
						 account_role.account_id.c_str() + "\'";
	DBConnector::getInstance()->executeUpdate(sqlStr, SCHEMA_IDB_ACCOUNT);
}

bool LoginBusiness::VerifyToken(const std::string &token, const std::string &account_id)
{
	LoginTokenCachePtr cache = LoginTokenCacheController::singleton()->get(token);
	if (cache && cache->account_id == account_id){
		cache->last_update_time = GetCurrentTimeString();
		LoginTokenCacheController::singleton()->updateCacheWithoutDBSync(cache->id,cache);
		return true;
	}else{
		return false;
	}
}

void LoginBusiness::deleteCacheWhenOffLine()
{	
	std::vector<std::string> del_list;
	LoginTokenCacheVecPtr cache_vec = LoginTokenCacheController::singleton()->getVec();

	std::vector<LoginTokenCachePtr>::const_iterator it = cache_vec->begin();
	for( ; it != cache_vec->end(); ++it) {
		LoginTokenCachePtr cache = *it;

		//超过3分钟做两件事：删token cache，update login log
		//boost::posix_time::ptime last_time(boost::posix_time::time_from_string(cache->last_update_time));
		//boost::posix_time::ptime now_time(boost::posix_time::second_clock::local_time());
		//if (now_time - last_time > boost::posix_time::minutes(1)){
		//	del_list.push_back(cache->id);
		//	AccountRole account_role;
		//	account_role.token = cache->token;
		//	account_role.account_id = cache->account_id;
		//	DeleteLoginLog(account_role);
		//}
		time_t last_time = ParseTimeString(cache->last_update_time);
		time_t now_time = time(NULL);
		if (difftime(now_time, last_time)>60*3){
			del_list.push_back(cache->id);
			AccountRole account_role;
			account_role.token = cache->token;
			account_role.account_id = cache->account_id;
			DeleteLoginLog(account_role);		
		}
	}
	if (del_list.size()>0){
		bool succeed = LoginTokenCacheController::singleton()->deleteCache(del_list);
		if (succeed){
			LOGGER_INFO("DeleteCacheWhenOffLine: Succcess for delete offline data: " << del_list.size())
		}else{
			LOGGER_ERROR("DeleteCacheWhenOffLine: Failed for delete offline data: " << del_list.size())
		}
	}
	
}
