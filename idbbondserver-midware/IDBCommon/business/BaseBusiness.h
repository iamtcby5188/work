/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    BaseBusiness 提供获取当前用户的CompanyId接口
====================================================================================================================
   Date       Name                                  Description of Change
05-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _BASE_BUSINESS_H_
#define _BASE_BUSINESS_H_

#include <string>
#include "sync/thread_sync.h"
#include "sdbus/string.h"
#include "sdbus/message.h"
#include <map>

class BaseBusiness
{
public:
	BaseBusiness(void);
	virtual ~BaseBusiness(void);

    std::string GetCompanyId();
    std::string GetCompanyConditionString();
    std::string GetCompanyConditionStringWithPreAnd();
    std::string GetCompanyConditionStringWithEndAnd();
    std::string GetFastdbQueryWithCompanyCondition(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range, 
        const std::string& sortby_default = "");
	void SetCompanyId(std::string company_id);

	std::string GetAccountId();
	void SetAccountId(std::string account_id);

    void setRequestMsg(const sdbus::Message* request);
    const sdbus::Message* getRequestMsg() const;
private:
	std::string mCompanyId;
	std::string mAccountId;
    const sdbus::Message* request_;

protected:
    std::map<int, std::string> fid_mp_;
};

#endif
