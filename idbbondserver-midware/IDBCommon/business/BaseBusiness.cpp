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
#include "BaseBusiness.h"
#include <iostream>
#include "logger.h"
#include "common.h"
#include "fid.h"

BaseBusiness::BaseBusiness(void)
	: mCompanyId("")
{
}


BaseBusiness::~BaseBusiness(void)
{
}

std::string BaseBusiness::GetCompanyId()
{
	return mCompanyId;
}

std::string BaseBusiness::GetCompanyConditionString()
{
    std::string s;
    if(!GetCompanyId().empty()) {
        s = " company_id = '" + GetCompanyId() + "'";
    }

    return s;
}

std::string BaseBusiness::GetCompanyConditionStringWithPreAnd()
{
    std::string s;
    if(!GetCompanyId().empty()) {
        s = " and " + GetCompanyConditionString();
    }

    return s;
}

std::string BaseBusiness::GetCompanyConditionStringWithEndAnd()
{
    std::string s;
    if(!GetCompanyId().empty()) {
        s = GetCompanyConditionString() + " and ";
    }
    return s;
}

void BaseBusiness::SetCompanyId(std::string company_id)
{
	mCompanyId = company_id;
    SET_FID_MAP(FID_ACCOUNT_COMPANY_ID, company_id)
}

std::string BaseBusiness::GetFastdbQueryWithCompanyCondition(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range, 
    const std::string& sortby_default)
{
    std::ostringstream oss;
    oss << cond;
    if(!GetCompanyId().empty()) {
        if(!cond.empty())
            oss << " and ";

        oss << FID_ACCOUNT_COMPANY_ID << " = " << GetCompanyId();
    }

    return ParseParam(oss.str(), sortby, range, fid_mp_, sortby_default);
}

std::string BaseBusiness::GetAccountId()
{
	return mAccountId;
}

void BaseBusiness::SetAccountId(std::string account_id)
{
	mAccountId = account_id;
}

void BaseBusiness::setRequestMsg(const sdbus::Message* request) {
    request_ = request;
}

const sdbus::Message* BaseBusiness::getRequestMsg() const{
    return request_;
}

