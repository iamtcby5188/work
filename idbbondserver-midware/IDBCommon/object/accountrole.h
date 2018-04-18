#ifndef _ACCOUNT_ROLE_H_
#define _ACCOUNT_ROLE_H_

#include "sdbus/string.h"
#include <string>

/*typedef struct
{
    std::string account_id;
	sdbus::string account_code;
    sdbus::string name;    //username
    sdbus::string password;
	sdbus::string password_org;
    sdbus::string company_id;
    sdbus::string company_name;

    sdbus::string token;
    int           ret_code;
    sdbus::string ret_msg;
    sdbus::string department;
    sdbus::string role;
    sdbus::string permission;

	//保存token和记录日志用
	sdbus::string ip;
	sdbus::string display_name;

	sdbus::string telephone;
	sdbus::string mobile;
	sdbus::string email;
	sdbus::string rm;
	sdbus::string msn;
	sdbus::string qq;
	sdbus::string address;

}AccountRole; */
class AccountRole {
public:
    AccountRole() :ret_code(0){}
    std::string account_id;
    sdbus::string account_code;
    sdbus::string name;    //username
    sdbus::string password;
    sdbus::string password_org;
    sdbus::string company_id;
    sdbus::string company_name;

    sdbus::string token;
    int           ret_code;
    sdbus::string ret_msg;
    sdbus::string department;
    sdbus::string role;
    sdbus::string permission;

    //保存token和记录日志用
    sdbus::string ip;
    sdbus::string display_name;
	sdbus::string mac;
	sdbus::string client_version;

    sdbus::string telephone;
    sdbus::string mobile;
    sdbus::string email;
    sdbus::string rm;
    sdbus::string msn;
    sdbus::string qq;
    sdbus::string address;
};
typedef std::tr1::shared_ptr<AccountRole> AccountRolePtr;

#endif