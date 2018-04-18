#ifndef _LOGIN_BUSINESS_H
#define _LOGIN_BUSINESS_H

#include <vector>
#include "service/login_service.h"
#include "object/accountrole.h"
#include "logger.h"

class LoginBusiness
{
public:
    LoginBusiness(LoginService* service);

    void BondLogin(AccountRole& account_role);
	void SaveLoginToken(const AccountRole &account_role);
	void DeleteLoginToken(const AccountRole &account_role);
	void SaveLoginLog(const AccountRole &account_role);
	void DeleteLoginLog(const AccountRole &account_role);
	bool VerifyToken(const std::string &token, const std::string &account_id);
	void deleteCacheWhenOffLine();
private:
    void Init();

    LoginService* service_;

};

#endif