/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    BaseBusiness �ṩ��ȡ��ǰ�û���CompanyId�ӿ�
====================================================================================================================
   Date       Name                                  Description of Change
18-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _SYSTEM_COMPANY_BUSINESS_H_
#define _SYSTEM_COMPANY_BUSINESS_H_

#include <string>
#include "cache/model/system_company_cache.h"

class SystemCompanyBusiness
{
public:
	SystemCompanyBusiness(void);
	virtual ~SystemCompanyBusiness(void);

    SystemCompanyCacheVecPtr getSysCompany();


private:
	std::string mCompanyId;
};

#endif
