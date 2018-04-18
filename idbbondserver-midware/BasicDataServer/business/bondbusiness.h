#ifndef _BONDBUSINESS_H_
#define _BONDBUSINESS_H_

#include <vector>
#include "service/bondservice.h"
#include "object/bond.h"
#include "logger.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "object/accountrole.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016

class BondBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    BondBusiness(BondService* service);

    BondSmallViewCacheVecPtr GetBondSmallViewList();
    BondSmallViewCacheVecPtr GetBondInfoByCondition(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
	int getBondInfoSizeByCondition(const sdbus::string& cond, const sdbus::string& sortby);
    BondSmallViewCacheVecPtr GetBondInfoByID(const sdbus::string& bond_key_listed_market);

    TradingInstitutionCacheVecPtr GetInstitutionInfoList();
    TradingInstitutionCacheVecPtr GetInstitutionInfoByID(const std::string& institution_id);

    BondTraderCacheVecPtr GetTraderInfoList();
    BondTraderCacheVecPtr GetTraderInfoByID(const std::string& trader_id);

    AccountCacheVecPtr GetAccountInfoList();
    AccountCacheVecPtr GetAccountInfoByID(const std::string& account_id);
	int UpdateAccountInfoByID(const AccountRole& account_role);
	int UpdateAccountPasswordByID(const AccountRole& account_role);

    BrokerBindingTraderCacheVecPtr GetBrokerBindingTraderList();
    ChinaBondDeviatedValueCacheVecPtr GetBondDeviatedValueList();
    //ChinaBondValuationCacheVecPtr GetBondValuationList();

    void GetBondIssueYearBound(int &max_issue_year, int &min_issue_year);
    ProvinceInfoCacheVecPtr GetBondIssueProvinceBound();
	SwSectorInfoCacheVecPtr GetSwSectorInfoList(const sdbus::string &sw_level);
    IssuingInstitutionCacheVecPtr GetIssuingInstitutionList();

    bool createOrGetInstitutionTraderInfo(const std::string &institutionName, const std::string &traderName, std::string &institutionId, std::string &traderId, bool &publishInstitution, bool &publishTrader);

private:
	void SetAccountInfoCache(AccountCache* cache, const AccountRole& account_role);
	bool UpdateAccountInfoByIDSync(const AccountRole& account_role);
	bool UpdateAccountPasswordByIDSync(const AccountRole& account_role);

private:
    void Init();
	int ParseSyncResponse(const std::string& response);
	std::string GeneratePwdReqBody(const AccountRole& account_role);
	std::string GenerateAccountInfoReqBody(const AccountRole& account_role);


    BondService* service_;

};

#endif // _BONDBUSINESS_H_