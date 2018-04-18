#include <sstream>
#include <iostream>
#include "bondbusiness.h"
#include "db/dbconnector.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "include/fid.h"
#include "include/common.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "config.h"
#include "business/httpclient.h"
#include "sync/account_sync.h"
BondBusiness::BondBusiness(BondService* service)
    : service_(service)
{
    Init();
}

void BondBusiness::Init()
{
    // 过滤条件涉及到的字段才需要设置
    SET_FID_MAP(FID_BOND_TYPE, goods_type_client)
    SET_FID_MAP(FID_LISTED_DATE, listed_date)
	SET_FID_MAP(FID_BOND_CODE, bond_id)
	SET_FID_MAP(FID_BOND_SHORT_NAME, short_name)
	SET_FID_MAP(FID_PLEDGED, pledged)
	SET_FID_MAP(FID_ACROSS_MARKET, across_market)
	SET_FID_MAP(FID_MARKET_TYPE, mkt_type)
	SET_FID_MAP(FID_BOND_CATEGORY, product_type)
	SET_FID_MAP(FID_ISSUER_RATING_FILTER, issuer_rating_filter)
	SET_FID_MAP(FID_LISTED_MARKET, listed_market)
	SET_FID_MAP(FID_TIME_TO_MATURITY_REAL, time_to_maturity_real)
	SET_FID_MAP(FID_HAS_OPTION, has_option)   //含权
	SET_FID_MAP(FID_GUARENTEED, guarenteed)   //有担保无担保
	SET_FID_MAP(FID_MUNICIPAL, municipal)     //城投非城投
	SET_FID_MAP(FID_ENTERPRISE_TYPE, institution_type)
	SET_FID_MAP(FID_RATE_INDEX, rate_index)
    SET_FID_MAP(FID_BOND_IS_EXPIRED, is_expired)
	SET_FID_MAP(FID_ISSUER_RATING_ORDER, issuer_rating_order)

	SET_FID_MAP(FID_FINANCING_PLATFORM, financing_platform)
	SET_FID_MAP(FID_COMPANY_BOND, company_bond)
	SET_FID_MAP(FID_ON_THE_RUN, on_the_run)
	SET_FID_MAP(FID_ISSUE_YEAR, issue_year)
    SET_FID_MAP(FID_SW_SECTOR_CODE, sw_subsector_code)
    SET_FID_MAP(FID_ISSUE_PROVINCE_CODE, province_code)
	SET_FID_MAP(FID_ISSUING_INSTITUTION, issue_institution_id)
    SET_FID_MAP(FID_ISSUE_INSTITUTION_RATING, institution_rating)
    SET_FID_MAP(FID_BOND_SUBTYPE, bond_subtype);
}

BondSmallViewCacheVecPtr BondBusiness::GetBondSmallViewList()
{
    dbQuery query("");
    BondSmallViewCacheController bondSmallViewCC;
    return bondSmallViewCC.getCacheListByQueryInThreadSafty(query);
}

BondSmallViewCacheVecPtr BondBusiness::GetBondInfoByCondition(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range)
{
    std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, range, "");
    dbQuery query(str.c_str());
    BondSmallViewCacheController bondSmallViewCC;
    return bondSmallViewCC.getCacheListByQueryInThreadSafty(query);
}

int BondBusiness::getBondInfoSizeByCondition(const sdbus::string& cond, const sdbus::string& sortby) {
	std::string str = GetFastdbQueryWithCompanyCondition(cond, sortby, "");
	dbQuery query(str.c_str());
	BondSmallViewCacheController bondSmallViewCC;
	return bondSmallViewCC.getSizeByQueryInThreadSafty(query);
}

BondSmallViewCacheVecPtr BondBusiness::GetBondInfoByID(const sdbus::string& bond_key_listed_market)
{
    dbQuery query;
    query = "bond_key_listed_market = ", bond_key_listed_market;

    BondSmallViewCacheController bondSmallViewCC;
    return bondSmallViewCC.getCacheListByQueryInThreadSafty(query);
}

TradingInstitutionCacheVecPtr BondBusiness::GetInstitutionInfoList()
{
    std::string str = GetCompanyConditionString(); //Add by Young 05-Apr-2016
    dbQuery query(str.c_str());

    TradingInstitutionCacheController institutionCC;
    return institutionCC.getCacheListByQueryInThreadSafty(query);
}

TradingInstitutionCacheVecPtr BondBusiness::GetInstitutionInfoByID(const std::string& institution_id)
{
    std::string str = "id = '" + institution_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    TradingInstitutionCacheController institutionCC;
    return institutionCC.getCacheListByQueryInThreadSafty(query);
}

BondTraderCacheVecPtr BondBusiness::GetTraderInfoList()
{
	std::string str = GetCompanyConditionString(); //Add by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondTraderCacheController bondTraderCC;
    return bondTraderCC.getCacheListByQueryInThreadSafty(query);
}

BondTraderCacheVecPtr BondBusiness::GetTraderInfoByID(const std::string& trader_id)
{
    std::string str = "id = '" + trader_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    BondTraderCacheController bondTraderCC;
    return bondTraderCC.getCacheListByQueryInThreadSafty(query);
}

BrokerBindingTraderCacheVecPtr BondBusiness::GetBrokerBindingTraderList()
{
	//std::string str = GetCompanyConditionString();
	std::string str = "";
    dbQuery query(str.c_str());
    BrokerBindingTraderCacheController brokerBindingTraderCC;
    return brokerBindingTraderCC.getCacheListByQueryInThreadSafty(query);
}

AccountCacheVecPtr BondBusiness::GetAccountInfoList()
{
	std::string str = GetCompanyConditionStringWithEndAnd() + " display_name != '' order by display_name"; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    AccountCacheController accountCC;
    return accountCC.getCacheListByQueryInThreadSafty(query);
}

AccountCacheVecPtr BondBusiness::GetAccountInfoByID(const std::string& account_id)
{
    std::string str = "id = '" + account_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
    AccountCacheController accountCC;
    return accountCC.getCacheListByQueryInThreadSafty(query);
}

ChinaBondDeviatedValueCacheVecPtr BondBusiness::GetBondDeviatedValueList()
{
	std::string str = GetCompanyConditionString(); //Add by Young 05-Apr-2016
    dbQuery query(str.c_str());
    ChinaBondDeviatedValueCacheController chinaBondDeviatedValueCC;
    return chinaBondDeviatedValueCC.getCacheListByQueryInThreadSafty(query);
}
/*
ChinaBondValuationCacheVecPtr BondBusiness::GetBondValuationList()
{
    dbQuery query("");
    ChinaBondValuationCacheController chinaBondValuationCC;
    return chinaBondValuationCC.getCacheListByQueryInThreadSafty(query);
}*/

void BondBusiness::GetBondIssueYearBound(int &max_issue_year, int &min_issue_year)
{
	BondSmallViewCacheController bondSmallViewCC;
	dbQuery query;
	query = "order by issue_year desc";
	
	BondSmallViewCachePtr smallviewcache = bondSmallViewCC.getCacheByQueryInThreadSafty(query);
	if (smallviewcache)
	{
		max_issue_year = smallviewcache->issue_year;
	}

	query = "order by issue_year asc";
	smallviewcache = bondSmallViewCC.getCacheByQueryInThreadSafty(query);
	if (smallviewcache)
	{
		min_issue_year = smallviewcache->issue_year;
	}
}

SwSectorInfoCacheVecPtr BondBusiness::GetSwSectorInfoList(const sdbus::string &sw_level)
{
	dbQuery query;	
	if (sw_level.empty())
	{
		query = "";
	}
	else
	{		
		query = "sw_level = ", sw_level;
	}
	
	SwSectorInfoCacheController sectorinfoCC;
	return sectorinfoCC.getCacheListByQueryInThreadSafty(query);
}

ProvinceInfoCacheVecPtr BondBusiness::GetBondIssueProvinceBound()
{
    dbQuery query;
    query = "";
    ProvinceInfoCacheController provinceInfoCC;
    return provinceInfoCC.getCacheListByQueryInThreadSafty(query);
}

IssuingInstitutionCacheVecPtr BondBusiness::GetIssuingInstitutionList()
{
    dbQuery query;
    query = "";
    IssuingInstitutionCacheController iiCC;
    return iiCC.getCacheListByQueryInThreadSafty(query);
}

int BondBusiness::UpdateAccountInfoByID(const AccountRole& account_role)
{
	//1. sync center DB
	if (!UpdateAccountInfoByIDSync(account_role))
	{
		return 1;
	}

	//2. update local DB
	std::string str = "id = '" + account_role.account_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
	AccountCacheController accountCC;
	AccountCachePtr cache = accountCC.getCacheByQueryInThreadSafty(query);
	std::vector<AccountCache *> account_vec;
	if (cache) 
	{
		SetAccountInfoCache(cache.get(), account_role);
		accountCC.updateCacheInThreadSafty(query, cache.get());
		account_vec.push_back(cache.get());
		if (account_vec.size() > 0) {
			service_->PublishAccountInfo(account_vec, "UpdateList");
		}
		return 0;
	}
	return 2;
}

void BondBusiness::SetAccountInfoCache(AccountCache* cache, const AccountRole& account_role)
{
	cache->display_name = account_role.display_name;
	cache->account_code = account_role.account_code;
	cache->telephone = account_role.telephone;
	cache->mobile = account_role.mobile;
	cache->email = account_role.email;
	cache->rm = account_role.rm;
	cache->msn = account_role.msn;
	cache->qq = account_role.qq;
	cache->address = account_role.address;
	cache->modify_time = GetCurrentTimeString();

}

int BondBusiness::UpdateAccountPasswordByID(const AccountRole& account_role)
{
	//1. sync center DB
	if (!UpdateAccountPasswordByIDSync(account_role))
	{
		return 1;
	}

	//2. update local DB
	std::vector<AccountCache *> account_vec;
	std::string str = "id = '" + account_role.account_id + "'" /*+ GetCompanyConditionStringWithPreAnd()*/; //Modify by Young 05-Apr-2016
    dbQuery query(str.c_str());
	AccountCacheController accountCC;
	AccountCachePtr cache = accountCC.getCacheByQueryInThreadSafty(query);
	if (cache) 
	{
		//check original password
		if (cache->password != account_role.password_org) {
			LOGGER_ERROR("orginal password is incorrect");
			return 2;
		}

		cache->password = account_role.password;
		cache->modify_time = GetCurrentTimeString();
		accountCC.updateCacheInThreadSafty(query, cache.get());
		
		account_vec.push_back(cache.get());
	}

	if (account_vec.size() > 0) {
		service_->PublishAccountInfo(account_vec, "UpdateList");
		return 0;
	}

	return 3;
}

bool BondBusiness::UpdateAccountInfoByIDSync(const AccountRole& account_role)
{
    bool isEnable = Config::singleton()->getBoolValue("SyncDB.Enable", true);
    if (!isEnable)
        return true;
	std::string ip = Config::singleton()->getValue("SyncDB.IP", "");
	std::string port = Config::singleton()->getValue("SyncDB.Port", "");
	std::string path = Config::singleton()->getValue("SyncDB.AccountInfoPath", "");
	if (ip=="" || port=="" || path=="" )
	{
		LOGGER_ERROR("SyncDB configuration information is incorrect for account info");
		return false;
	}

	std::string http_req_body = GenerateAccountInfoReqBody(account_role);
	HttpClient client(ip, port, path, http_req_body);
	if(0 != client.post())
	{
		return false;
	}
	LOGGER_INFO("UpdateAccountInfoByIDSync center process succeed");
	std::string response = client.getResponse();
	if(0 != ParseSyncResponse(response))
	{
		return false;
	}
	LOGGER_INFO("UpdateAccountInfoByIDSync local process succeed");
	return true;
}

bool BondBusiness::UpdateAccountPasswordByIDSync(const AccountRole& account_role)
{
    bool isEnable = Config::singleton()->getBoolValue("SyncDB.Enable", true);
    if (!isEnable)
        return true;
	std::string ip = Config::singleton()->getValue("SyncDB.IP", "");
	std::string port = Config::singleton()->getValue("SyncDB.Port", "");
	std::string path = Config::singleton()->getValue("SyncDB.PassordPath", "");
	if (ip=="" || port=="" || path=="" )
	{
		LOGGER_ERROR("SyncDB configuration information is incorrect for password");
		return false;
	}
	
	std::string http_req_body = GeneratePwdReqBody(account_role);
	HttpClient client(ip, port, path, http_req_body);
	if(0 != client.post())
	{
		return false;
	}
	LOGGER_INFO("UpdateAccountPasswordByIDSync center process succeed");

	std::string response = client.getResponse();
	if(0 != ParseSyncResponse(response))
	{
		return false;
	}
	LOGGER_INFO("UpdateAccountPasswordByIDSync local process succeed");
	return true;
}

std::string BondBusiness::GeneratePwdReqBody(const AccountRole& account_role)
{
	//boost::property_tree::ptree pt;
	//pt.put("id", account_role.account_id);  
	//pt.put("password", account_role.password_org);  
	//pt.put("newPasswd", account_role.password);
	//std::stringstream ss;  
	//write_json(ss, pt);  
	//std::string str_json = ss.str();  
	
	//Notice: there is a problem if using boost ptree when chinese charactor
	//So use the original way 
	std::string ret;
	ret = "{";
	ret += "\r\n";
	ret += "    \"id\": \"";
	ret += account_role.account_id;
	ret += "\",\r\n";
	ret += "    \"password\": \"";
	ret += account_role.password_org;
	ret += "\",\r\n";
	ret += "    \"newPasswd\": \"";
	ret += account_role.password;
	ret += "\"\r\n";
	ret += "}";
	LOGGER_DEBUG( "password http request body:" << ret);
	return ret;
}

std::string BondBusiness::GenerateAccountInfoReqBody(const AccountRole& account_role)
{
/*	boost::property_tree::ptree pt;
	pt.put("id", account_role.account_id);  
	pt.put("accountCode", account_role.account_code);  
	pt.put("displayName", account_role.display_name); 
	pt.put("mobile", account_role.mobile); 
	pt.put("telephone", account_role.telephone); 
	pt.put("email", account_role.email); 
	pt.put("rm", account_role.rm); 
	pt.put("msn", account_role.msn); 
	pt.put("qq", account_role.qq); 
	pt.put("address", account_role.address); 
	std::stringstream ss;  
	write_json(ss, pt);  
	std::string str_json = ss.str();*/  

	//Notice: there is a problem if using boost ptree when chinese charactor
	//So use the original way 
	std::string ret;
	ret = "{";
	ret += "\r\n";
	ret += "    \"id\": \"";
	ret += account_role.account_id;
	ret += "\",\r\n";
	ret += "    \"password\": \"";
	ret += account_role.password;
	ret += "\",\r\n";
	ret += "    \"accountCode\": \"";
	ret += account_role.account_code;
	ret += "\",\r\n";
	ret += "    \"displayName\": \"";
	ret += account_role.display_name;
	ret += "\",\r\n";
	ret += "    \"mobile\": \"";
	ret += account_role.mobile;
	ret += "\",\r\n";
	ret += "    \"telephone\": \"";
	ret += account_role.telephone;
	ret += "\",\r\n";
	ret += "    \"email\": \"";
	ret += account_role.email;
	ret += "\",\r\n";
	ret += "    \"rm\": \"";
	ret += account_role.rm;
	ret += "\",\r\n";
	ret += "    \"msn\": \"";
	ret += account_role.msn;
	ret += "\",\r\n";
	ret += "    \"qq\": \"";
	ret += account_role.qq;
	ret += "\",\r\n";
	ret += "    \"address\": \"";
	ret += account_role.address;
	ret += "\"\r\n";
	ret += "}";

	LOGGER_DEBUG("account basic http request body:" << ret);
	return ret;
}

int BondBusiness::ParseSyncResponse(const std::string& response)
{
	std::stringstream ss(response);
	boost::property_tree::ptree pt;
	try{    
		read_json(ss, pt);
	}
	catch(boost::property_tree::ptree_error & e) {
		LOGGER_ERROR("Exception: " << e.what() );
		return 4; 
	}
	catch (...) {
		LOGGER_ERROR("ParseSyncResponse read_json: unknown error occurred");
		return 5; 
	}

	try{
		std::string result = pt.get<std::string>("result");
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		LOGGER_DEBUG("sync result:"<<result);
		if (result != "true")
		{		
			int err_code = pt.get<int>("erroCode");
			std::string err_msg = pt.get<std::string>("errString");
			LOGGER_ERROR("sync error code:"<< err_code);
			LOGGER_ERROR("sync error message:" << err_msg);
			return 1;
		}
	}
	catch (boost::property_tree::ptree_error & e)
	{
		LOGGER_ERROR("Exception: " << e.what());
		return 2;
	}
	catch (...) {
		LOGGER_ERROR("ParseSyncResponse get: unknown error occurred");
		return 3; 
	}
	return 0;
}

bool BondBusiness::createOrGetInstitutionTraderInfo(const std::string &institutionCode, const std::string &traderCode, std::string &institutionId, std::string &traderId, bool &publishInstitution, bool &publishTrader)
{
    TradingInstitutionCacheController trading_institution_cc;
    std::string q_institution_str = "code = '" + institutionCode + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q_institution_code(q_institution_str.c_str());
    TradingInstitutionCachePtr trading_institution_cache = trading_institution_cc.getCacheByQueryInThreadSafty(q_institution_code);
    if (trading_institution_cache)
    {
        publishInstitution = false;
        institutionId = trading_institution_cache->id;
    } else // Insert to db if not exists
    {
        publishInstitution = true;

        TradingInstitutionCachePtr trading_institution_cache(new TradingInstitutionCache());
        trading_institution_cache->id = GetNewGuid();
        trading_institution_cache->company_id = GetCompanyId();
        trading_institution_cache->name = institutionCode;
        trading_institution_cache->name_en = institutionCode;
        trading_institution_cache->description = "";
        trading_institution_cache->code = institutionCode;
        trading_institution_cache->status = "1";
        trading_institution_cache->attribute = "";
        trading_institution_cache->attribute1 = "";
        trading_institution_cache->attribute2 = "";
        trading_institution_cache->attribute3 = "";
        trading_institution_cache->attribute4 = "";
        trading_institution_cache->attribute5 = "";
        trading_institution_cache->bank_type = "";
        trading_institution_cache->full_name = institutionCode;
        trading_institution_cache->city	= "";
        trading_institution_cache->full_name_en = "";
        trading_institution_cache->pinyin = "";
        trading_institution_cache->pinyin_full = "";
        trading_institution_cache->city_name = "";
        trading_institution_cache->create_time = GetCurrentTimeString();
        trading_institution_cache->modify_time = GetCurrentTimeString();			

        trading_institution_cc.insertCacheInThreadSafty(trading_institution_cache.get());
        institutionId = trading_institution_cache->id;
    }


    BondTraderCacheController trader_cc;
    std::string q_trader_str = "code = '" + traderCode + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q_trader_code(q_trader_str.c_str());
    BondTraderCachePtr trader_cache = trader_cc.getCacheByQueryInThreadSafty(q_trader_code);

    if (trader_cache) 
    {
        publishTrader = false;
        traderId = trader_cache->id;
    }
    else  // Insert to db if not exists
    {
        publishTrader = true;

        BondTraderCachePtr trader_cache(new BondTraderCache());
        trader_cache->id = GetNewGuid();
        trader_cache->company_id = GetCompanyId();
        trader_cache->bank_id = institutionId;
        trader_cache->name = traderCode;
        trader_cache->code = traderCode;
        trader_cache->f_code = traderCode;		
        trader_cache->attribute = "";
        trader_cache->attribute1 = "";
        trader_cache->attribute2 = "";
        trader_cache->attribute3 = "";
        trader_cache->attribute4 = "";
        trader_cache->attribute5 = "";
        trader_cache->create_time = GetCurrentTimeString();
        trader_cache->status = "1";
        trader_cache->flag = "";
        trader_cache->flag_vip = false;
        trader_cache->flag_bad = false;
        trader_cache->flag_bargain = "";
        trader_cache->customer_id = "";
        trader_cache->pinyin = "";
        trader_cache->pinyin_full = "";
        trader_cache->modify_time = GetCurrentTimeString();

        trader_cc.insertCacheInThreadSafty(trader_cache.get());
        traderId = trader_cache->id;
    }

    return true;
}
