#include <iostream>
#include "cache_manager.h"
#include "cache_def.h"

#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondquoterefer_cachecontroller.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "cache/controller/dcsbonddeal_cachecontroller.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "cache/controller/logintoken_cachecontroller.h"
#include "cache/cache_controller_declear.h"

CacheManager* CacheManager::singleton_ = NULL;

CacheManager* CacheManager::singleton() {
    if (singleton_ == NULL) {
        singleton_ = new CacheManager();
    }

    return singleton_;
}

void CacheManager::InitCache()
{
    TIME_COST_BEGIN("Caching ...")

//    BondQuoteCacheController bondQuoteCC;
//    bondQuoteCC.GetQuoteRemote();
//    bondQuoteCC.cacheTable();

 //   BondQuoteReferCacheController bondQuoteReferCC;
 //   bondQuoteReferCC.cacheTable();
	//bondQuoteReferCC.commit();

 //   BondDealCacheController bondDealCC;
 //   bondDealCC.cacheTable();
 //   bondDealCC.commit();

 //   BondBestQuoteCacheController bondbestquoteCC;
 //   bondbestquoteCC.clearCache();
 //   bondbestquoteCC.commit();

    SystemCompanyCacheController systemCompanyCacheCC;
    systemCompanyCacheCC.cacheTable();
    systemCompanyCacheCC.commit();

    ChinaBondValuationCacheController chinaBondValuationCC;
	chinaBondValuationCC.cacheTable();
	chinaBondValuationCC.commit();

    LOGGER_INFO("------------------ Caching Done, Go ahead :] -------------");

    TIME_COST_END("Caching ...")
}

void CacheManager::InitCompanyCache()
{
	SystemCompanyCacheController systemCompanyCacheCC;
	systemCompanyCacheCC.cacheTable();
	systemCompanyCacheCC.commit();
}

void CacheManager::clearAllCache()
{
	BondDealCacheController bond_deal_cc;
	bond_deal_cc.clearCache();
	DcsBondDealCacheController dcs_bond_deal_cc;
	dcs_bond_deal_cc.clearCache();
	BondQuoteReferCacheController bond_quote_refer_cc;
	bond_quote_refer_cc.clearCache();
	BondQuoteCacheController bond_quote_cc;
	bond_quote_cc.clearCache();
	BondBestQuoteCacheController bond_best_quote_cc;
	bond_best_quote_cc.clearCache();
	BondSmallViewCacheController bond_small_view_cc;
	bond_small_view_cc.clearCache();
	InstitutionRatingCacheController inst_ratting_cc;
	inst_ratting_cc.clearCache();
	IssuerInfoCacheController issuer_info_cc;
	issuer_info_cc.clearCache();
	IssuingInstitutionCacheController issuing_inst_cc;
	issuing_inst_cc.clearCache();
	BondTraderCacheController bond_trader_cc;
	bond_trader_cc.clearCache();
	BondTypeSelectiveCacheController bond_type_selective_cc;
	bond_type_selective_cc.clearCache();
	BrokerBindingTraderCacheController broker_binding_trader_cc;
	broker_binding_trader_cc.clearCache();
	ChinaBondDeviatedValueCacheController china_bond_deviated_cc;
	china_bond_deviated_cc.clearCache();
	ChinaBondValuationCacheController china_bond_valuation_cc;
	china_bond_valuation_cc.clearCache();
	ConstantsCacheController constancs_cc;
	constancs_cc.clearCache();
	HolidayInfoCacheController holiday_info_cc;
	holiday_info_cc.clearCache();
	ProvinceInfoCacheController province_cc;
	province_cc.clearCache();
	SelectiveInfoCacheController selective_info_cc;
	selective_info_cc.clearCache();
	SwSectorInfoCacheController swsector_info_cc;
	swsector_info_cc.clearCache();
	SystemCompanyCacheController system_company_cc;
	system_company_cc.clearCache();
	TradingInstitutionCacheController trading_inst_cc;
	trading_inst_cc.clearCache();
	AccountGroupCacheController account_group_cc;
	account_group_cc.clearCache();
	AccountPermissionCacheController account_permission_cc;
	account_permission_cc.clearCache();
	AccountCacheController account_cc;
	account_cc.clearCache();
}
