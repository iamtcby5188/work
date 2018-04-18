/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    BaseBusiness 提供获取当前用户的CompanyId接口
====================================================================================================================
   Date       Name                                  Description of Change
18-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _VERSION_BUSINESS_H_
#define _VERSION_BUSINESS_H_

#include <string>

class VersionCache;
class VersionBusiness
{
public:
	VersionBusiness(void);
	virtual ~VersionBusiness(void);

    static int GetVersion(const std::string& method, const std::string& company);
    static void SetVersion(const std::string& method, const std::string& company, const int& version);
    static void IncOneAndGetVersion(const std::string& method, const std::string& company, int &version);
    static void UpdateVersionCallBack(VersionCache* cache, void* param);

    static void InitVersion(const std::string& method, const std::string& company_id);
};

#endif //_VERSION_BUSINESS_H_
