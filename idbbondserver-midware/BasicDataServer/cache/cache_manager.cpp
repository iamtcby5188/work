#include "cache_manager.h"
#include "cache_def.h"
#include "cache/cache_center.h"
#include "cache/cache_monitor.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "cache/controller/bondgroup_cachecontroller.h"
#include "cache/controller/bondgroupdetail_cachecontroller.h"
#include "CRMConnectionService.h"
#include "business/SystemCompanyBusiness.h"
#include "business/VersionBusiness.h"
CacheManager* CacheManager::singleton_ = NULL;

CacheManager* CacheManager::singleton() {
	if (singleton_ == NULL) {
		singleton_ = new CacheManager();
	}

	return singleton_;
}

void CacheManager::InitCache(bool is_init) {
	TIME_COST_BEGIN("Caching ...");
	useCacheInMem = true;
	SystemCompanyCacheController systemCompanyCacheCC;
	systemCompanyCacheCC.cacheTable();
	systemCompanyCacheCC.commit();

	initVersionTable();

	AccountGroupCacheController accountGroupCC;
	accountGroupCC.cacheTable();
	accountGroupCC.commit();

	AccountPermissionCacheController accountPermissionCC;
	accountPermissionCC.cacheTable();
	accountPermissionCC.commit();

	AccountCacheController accountCC;
	accountCC.cacheTable();
	accountCC.commit();

	BondTypeSelectiveCacheController bondTypeSelectiveCC;
	bondTypeSelectiveCC.cacheTable();
	bondTypeSelectiveCC.commit();

	SelectiveInfoCacheController selectiveCC;
	selectiveCC.cacheTable();
	selectiveCC.commit();

	IssuingInstitutionCacheController issuingInstitutionCacheCC;
	issuingInstitutionCacheCC.cacheTable();
	issuingInstitutionCacheCC.commit();

	AccountCacheController::cacheInMem("id");
	BrokerBindingTraderCacheController brokerbindingtraderCC;
	brokerbindingtraderCC.cacheTable();
	brokerbindingtraderCC.commit();

	BondTraderCacheController BondTraderCC;
	BondTraderCC.cacheTable();
	BondTraderCC.commit();

	TradingInstitutionCacheController tradingInstitutionCacheCC;
	tradingInstitutionCacheCC.cacheTable();
	tradingInstitutionCacheCC.commit();

	//向CRM发送异步请求，获取机构和交易员信息
	CRMConnectionService::singleton()->queryInstitutionList(is_init);

	//ChinaBondValuationCacheController chinaBondValuationCC;
	//chinaBondValuationCC.cacheTable();
	//chinaBondValuationCC.commit();

	ChinaBondDeviatedValueCacheController chinabonddeviatedvalueCC;
	chinabonddeviatedvalueCC.cacheTable();
	chinabonddeviatedvalueCC.commit();

	HolidayInfoCacheController holidayinfoCC;
	holidayinfoCC.cacheTable();
	holidayinfoCC.commit();

	IssuerInfoCacheController issuerinfoCC;
	issuerinfoCC.cacheTable();
	issuerinfoCC.commit();

	SwSectorInfoCacheController sectorinfoCC;
	sectorinfoCC.cacheTable();
	sectorinfoCC.commit();

	ProvinceInfoCacheController provinceinfoCC;
	provinceinfoCC.cacheTable();
	provinceinfoCC.commit();

	/* Add by Young 20-Apr-2016 */
	InstitutionRatingCacheController institutionRatingCC;
	institutionRatingCC.cacheTable();
	institutionRatingCC.commit();

	IssuingInstitutionCacheController::cacheInMem("institution_code");
	IssuerInfoCacheController::cacheInMem("institution_code");
	InstitutionRatingCacheController::cacheInMem("institution_code");
	HolidayInfoCacheController::cacheInMem("holiday_date");
	BondSmallViewCache::memCacheInit();
	initHolidayCache();
	BondSmallViewCacheController bondSmallViewCC;
	bondSmallViewCC.cacheTable();
	bondSmallViewCC.commit();
	bondSmallViewCC.loadAcrossMarketBondIds();
	//bondSmallViewCC.loadIssuerRating(); //Add by Young 20-Apr-2016
	BondSmallViewCache::memCacheClear();

	/*
	BondQuoteCacheController bondQuoteCC;
	bondQuoteCC.cacheTable();
	bondQuoteCC.commit();

	BondQuoteReferCacheController bondQuoteReferCC;
	bondQuoteReferCC.cacheTable();
	bondQuoteReferCC.commit();

	BondDealCacheController bondDealCC;
	bondDealCC.cacheTable();
	bondDealCC.commit();
	*/

	ConstantsCacheController constantsCC;
	constantsCC.cacheTable();
	constantsCC.commit();

	// We should commit to make the insertion works
	//CommitCache();

	/*
	BondBestQuoteCacheController bondbestquoteCC;
	bondbestquoteCC.clearCache();
	BondBestQuoteCalcBusiness bond_best_quote_calc_business;
	bond_best_quote_calc_business.MakeAllBestPrice();
	bondbestquoteCC.commit();
	*/

	//	LoginTokenCacheController::singleton()->init();
	//dbThread* thread = new dbThread[THREAD_COUNT];
	//for (int i=0; i<THREAD_COUNT; ++i)
	//{		 
	//    //thread[i].create(testMultiThread, NULL);   
	//    thread[i].create(testMultiThreadWithoutDBSync, NULL);
	//}
	//for (int i=0; i<THREAD_COUNT; ++i)
	//{		 
	//    thread[i].join();
	//}
	useCacheInMem = false;
	memCacheClear();
	LOGGER_INFO("------------------ Caching Done, Go ahead :] -------------");

	TIME_COST_END("Caching ...")
}

void CacheManager::InitSimpleCache() {
	BondGroupCacheController::singleton()->init();
	BondGroupDetailCacheController::singleton()->init();
}

void CacheManager::initVersionTable() {
	VersionCacheController vcc;
	vcc.clearCache();
	std::vector<std::string> messageTypeVec;
	messageTypeVec.push_back(mBond_AccountInfo);
	messageTypeVec.push_back(mBond_BondDeviatedValue);
	messageTypeVec.push_back(mBond_BrokerBindingTrader);
	messageTypeVec.push_back(mBond_GetBondInfoByCondition);
	messageTypeVec.push_back(mBond_InstitutionInfo);
	messageTypeVec.push_back(mBond_TraderInfo);
	messageTypeVec.push_back(mBondDetail_BondSmallViewDetail);
	messageTypeVec.push_back(mBondInfo_BondSmallView);
	SystemCompanyBusiness sc;
	SystemCompanyCacheVecPtr scv = sc.getSysCompany();
	for (std::vector<SystemCompanyCachePtr>::iterator it = scv->begin(); it != scv->end(); ++it) {
		for (std::vector<std::string>::iterator msgIt = messageTypeVec.begin(); msgIt != messageTypeVec.end(); ++msgIt) {
			VersionBusiness::InitVersion(*msgIt, (*it)->id);
		}
	}
}

void CacheManager::memCacheClear() {
	IssuingInstitutionCacheController::clearMemCache();
	IssuerInfoCacheController::clearMemCache();
	HolidayInfoCacheController::clearMemCache();
	InstitutionRatingCacheController::cacheInMem("institution_code");
}

/*
int THREAD_COUNT = 20;

void thread_proc testMultiThreadWithoutDBSync(void *param)
{
for (int i=0; i<100; ++i) {
BondBestQuoteCacheController bondBestQuoteCC;
bondBestQuoteCC.testUpdateWithoutDBSync();
}
}

void thread_proc testMultiThread(void *param)
{
dbQuery q;
q = "";

AccountCacheController accountCC;
//accountCC.testCaseShowAll();

AccountGroupCacheController accountGroupCC;
//accountGroupCC.testCaseShowAll();


BondTypeSelectiveCacheController bondTypeSelectiveCC;
bondTypeSelectiveCC.testInsert();
bondTypeSelectiveCC.testUpdate();
//bondTypeSelectiveCC.testDelete();
//cout << bondTypeSelectiveCC.getSizeByQueryInThreadSafty(q) << endl;
//bondTypeSelectiveCC.testCaseShowAll();


SelectiveInfoCacheController selectiveCC;
selectiveCC.testInsert();
selectiveCC.testUpdate();
//selectiveCC.testDelete();
//cout << selectiveCC.getSizeByQueryInThreadSafty(q) << endl;
//selectiveCC.testCaseShowAll();


IssuingInstitutionCacheController issuingInstitutionCacheCC;
issuingInstitutionCacheCC.testInsert();
issuingInstitutionCacheCC.testUpdate();
//issuingInstitutionCacheCC.testDelete();

//issuingInstitutionCacheCC.testCaseShowAll();

TradingInstitutionCacheController tradingInstitutionCacheCC;
tradingInstitutionCacheCC.testInsert();
tradingInstitutionCacheCC.testUpdate();
//tradingInstitutionCacheCC.testDelete();

//tradingInstitutionCacheCC.testCaseShowAll();

BondSmallViewCacheController bondSmallViewCC;
//cout << bondSmallViewCC.getSizeByQueryInThreadSafty(q) << endl;
//bondSmallViewCC.testCaseShowAll();

BondQuoteCacheController bondQuoteCC;
bondQuoteCC.testInsert();
bondQuoteCC.testUpdate();
//bondQuoteCC.testDelete();
//cout << bondQuoteCC.getSizeByQueryInThreadSafty(q) << endl;
//bondQuoteCC.testCaseShowAll();

BondQuoteReferCacheController bondQuoteReferCC;
bondQuoteReferCC.testInsert();
bondQuoteReferCC.testUpdate();
//bondQuoteReferCC.testDelete();

//bondQuoteReferCC.testCaseShowAll();

BondBestQuoteCacheController bondBestQuoteCC;
bondBestQuoteCC.testInsert(true);
//q = "id = ", "theochen";
//bondBestQuoteCC.testCommit();
bondBestQuoteCC.testUpdate();
//bondBestQuoteCC.testDelete();
//cout << bondBestQuoteCC.getSizeByQueryInThreadSafty(q) << endl;
//bondBestQuoteCC.testCaseShowAll();

BondDealCacheController bondDealCC;
bondDealCC.testInsert();
bondDealCC.testUpdate();
//bondDealCC.testDelete();
//cout << bondDealCC.getSizeByQueryInThreadSafty(q) << endl;
//bondDealCC.testCaseShowAll();

BondTraderCacheController BondTraderCC;
BondTraderCC.testInsert();
BondTraderCC.testUpdate();
//BondTraderCC.testDelete();
//cout << BondTraderCC.getSizeByQueryInThreadSafty(q) << endl;
//BondTraderCC.testCaseShowAll();

ChinaBondValuationCacheController chinaBondValuationCC;
chinaBondValuationCC.testInsert();
chinaBondValuationCC.testUpdate();
//chinaBondValuationCC.testDelete();
//cout << chinaBondValuationCC.getSizeByQueryInThreadSafty(q) << endl;
//chinaBondValuationCC.testCaseShowAll();

BondGroupCacheController bondGroupCC;
bondGroupCC.testInsert();
bondGroupCC.testUpdate();
//bondGroupCC.testDelete();
//cout << bondGroupCC.getSizeByQueryInThreadSafty(q) << endl;
//bondGroupCC.testCaseShowAll();

BondGroupDetailCacheController bondGroupDetailCC;
bondGroupDetailCC.testInsert();
bondGroupDetailCC.testUpdate();
//bondGroupDetailCC.testDelete();
//cout << bondGroupDetailCC.getSizeByQueryInThreadSafty(q) << endl;
//bondGroupDetailCC.testCaseShowAll();

ConstantsCacheController constantsCC;
//constantsCC.testInsert();
//constantsCC.testUpdate();
//   constantsCC.testCaseShowAll();
_endthread();

}
*/

