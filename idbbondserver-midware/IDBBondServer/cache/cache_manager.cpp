#include "cache_manager.h"
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
#include "business/bondbestquotecalc_business.h"
#include "business/SystemCompanyBusiness.h"
#include "business/bondbestquote_business.h"
#include "service/service_manager.h"
#include "DCSConnectionService.h"
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include "business/SystemCompanyBusiness.h"
#include "business/VersionBusiness.h"
CacheManager* CacheManager::singleton_ = NULL;

CacheManager* CacheManager::singleton() {
	if (singleton_ == NULL) {
		singleton_ = new CacheManager();
	}

	return singleton_;
}

void CacheManager::InitCache() {
	TIME_COST_BEGIN("Caching ...");
	//initVersionTable();
	initMemCache();
	ChinaBondValuationCacheController chinaBondValuationCacheCC;
	BondTypeSelectiveCacheController bondTypeSelectiveCC;
	BondQuoteCacheController bondQuoteCC;
	BondQuoteReferCacheController bondQuoteReferCC;
	BondDealCacheController bondDealCC;
	BondBestQuoteCacheController bondbestquoteCC;

	chinaBondValuationCacheCC.cacheTable();
	chinaBondValuationCacheCC.commit();

	bondTypeSelectiveCC.cacheTable();
	bondTypeSelectiveCC.commit();

	bondQuoteCC.cacheTable();
	bondQuoteCC.commit();

	bondQuoteReferCC.cacheTable();
	bondQuoteReferCC.commit();

	bondDealCC.cacheTable();
	bondDealCC.commit();

	InitDcsDealCache();

	if (kCalcAndInsertIntoMysqlDatabase != "1" && kFirstLaunch) {
		bondbestquoteCC.cacheTable();
		bondbestquoteCC.commit();

		kFirstLaunch = false;
	} else {
		bondbestquoteCC.clearCache();
		BondQuoteCacheController::cacheInMem("id");
		SystemCompanyBusiness sc;
		SystemCompanyCacheVecPtr scv = sc.getSysCompany();
		for (std::vector<SystemCompanyCachePtr>::iterator it = scv->begin(); it != scv->end(); ++it) {
			BondBestQuoteCalcBusiness bond_best_quote_calc_business;
			bond_best_quote_calc_business.SetCompanyId((*it)->id);
			bond_best_quote_calc_business.MakeAllBestPrice();
			bondbestquoteCC.commit();

			BondBestQuoteCacheController bbqcc;
			BondBestQuoteBusiness bbqb((BondBestQuoteService *)(ServiceManager::singleton()->getService(kServiceBondBestQuote)));
			bbqb.SetCompanyId((*it)->id);
			std::string str = bbqb.GetCompanyConditionString();
			dbQuery q(str.c_str());
			BondBestQuoteCacheVecPtr bbqcv = bbqcc.getCacheListByQueryInThreadSafty(q);
			bbqb.InsertUpdateIntoDiskDB(*(bbqcv.get()), true);
		}
	}

	LoginTokenCacheController::singleton()->init();
	clearMemCache();
	LOGGER_INFO("------------------ Caching Done, Go ahead :] -------------");

	TIME_COST_END("Caching ...")
}

void CacheManager::InitCompanyCache() {
	SystemCompanyCacheController systemCompanyCacheCC;
	systemCompanyCacheCC.cacheTable();
	systemCompanyCacheCC.commit();

	initVersionTable();
}

void CacheManager::initMemCache() {
	useCacheInMem = true;
	AccountCacheController::cacheInMem("id");
	BondSmallViewCacheController::cacheInMem("bond_key_listed_market");
	BondTraderCacheController::cacheInMem("id");
	TradingInstitutionCacheController::cacheInMem("id");
	ChinaBondValuationCacheController::memCacheInit();
}

void CacheManager::clearMemCache() {
	useCacheInMem = false;
	AccountCacheController::clearMemCache();
	BondSmallViewCacheController::clearMemCache();
	BondTraderCacheController::clearMemCache();
	TradingInstitutionCacheController::clearMemCache();
	ChinaBondValuationCacheController::memCacheClear();
}

void CacheManager::first_InitCache() {
	TIME_COST_BEGIN("Caching ...");
	//initVersionTable();
	InitChinaBondValuationCache();
	InitBondTypeSelectiveCache();
	initMemCache();
	InitBondQuoteCache();

	//boost::function0<void> f1 = boost::bind(&CacheManager::InitChinaBondValuationCache, this);
	//boost::function0<void> f2 = boost::bind(&CacheManager::InitBondTypeSelectiveCache, this);
	//boost::function0<void> f3 = boost::bind(&CacheManager::InitBondQuoteCache, this);
	boost::function0<void> f4 = boost::bind(&CacheManager::InitBondQuoteReferCache, this);
	boost::function0<void> f5 = boost::bind(&CacheManager::InitBondDealCache, this);
	//boost::function0<void> f6 = boost::bind(&CacheManager::InitBondBestQuoteCache, this);
	//boost::function0<void> f7 = boost::bind(&CacheManager::InitLoginTokenCache, this);
	boost::function0<void> f8 = boost::bind(&CacheManager::InitDcsDealCache, this);
	//boost::thread thrd1(f1);
	//boost::thread thrd2(f2);
	//boost::thread thrd3(f3);
	boost::thread thrd4(f4);
	boost::thread thrd5(f5);
	boost::thread thrd8(f8);
	//boost::thread thrd6(f6);
	//boost::thread thrd7(f7);
	//thrd1.join();
	//thrd2.join();
	//thrd3.join();
	thrd4.join();
	thrd5.join();
	thrd8.join();
	//thrd6.join();
	//thrd7.join();

	if (state == 0 && success_count == 6) {
		LOGGER_INFO("------------------ Caching Success, Go ahead :] -------------");
	} else {
		LOGGER_ERROR("------------------ Caching failed, Go ahead :] -------------[" << state << "][" << success_count << "]");
	}

	//InitDcsDealCache();
	//InitBondQuoteReferCache();
	//InitBondDealCache();
	InitBondBestQuoteCache();
	InitLoginTokenCache();

	clearMemCache();
	LOGGER_INFO("------------------ Caching Done, Go ahead :] -------------");

	TIME_COST_END("Caching ...")
}

void CacheManager::InitChinaBondValuationCache() {
	ChinaBondValuationCacheController chinaBondValuationCacheCC;
	chinaBondValuationCacheCC.cacheTable();
	chinaBondValuationCacheCC.commit();
	success_count++;
}

void CacheManager::InitBondTypeSelectiveCache() {
	BondTypeSelectiveCacheController bondTypeSelectiveCC;
	bondTypeSelectiveCC.cacheTable();
	bondTypeSelectiveCC.commit();
	success_count++;
}

void CacheManager::InitBondQuoteCache() {
	BondQuoteCacheController bondQuoteCC;
	bondQuoteCC.cacheTable();
	bondQuoteCC.commit();
	success_count++;
}

void CacheManager::InitBondQuoteReferCache() {
	BondQuoteReferCacheController bondQuoteReferCC;
	bondQuoteReferCC.cacheTable();
	bondQuoteReferCC.commit();
	success_count++;
}

void CacheManager::InitBondDealCache() {
	BondDealCacheController bondDealCC;
	bondDealCC.cacheTable();
	bondDealCC.commit();
	success_count++;
}

void CacheManager::InitBondBestQuoteCache() {
	BondBestQuoteCacheController bondbestquoteCC;
	if (kCalcAndInsertIntoMysqlDatabase != "1" && kFirstLaunch) {
		bondbestquoteCC.cacheTable();
		bondbestquoteCC.commit();

		kFirstLaunch = false;
	} else {
		bondbestquoteCC.clearCache();
		BondQuoteCacheController::cacheInMem("id");
		SystemCompanyBusiness sc;
		SystemCompanyCacheVecPtr scv = sc.getSysCompany();
		BondDealCacheController::memCacheInit();
		for (std::vector<SystemCompanyCachePtr>::iterator it = scv->begin(); it != scv->end(); ++it) {
			BondBestQuoteCalcBusiness bond_best_quote_calc_business;
			bond_best_quote_calc_business.SetCompanyId((*it)->id);
			bond_best_quote_calc_business.MakeAllBestPrice();
			bondbestquoteCC.commit();

			BondBestQuoteCacheController bbqcc;
			BondBestQuoteBusiness bbqb((BondBestQuoteService *)(ServiceManager::singleton()->getService(kServiceBondBestQuote)));
			bbqb.SetCompanyId((*it)->id);
			std::string str = bbqb.GetCompanyConditionString();
			dbQuery q(str.c_str());
			BondBestQuoteCacheVecPtr bbqcv = bbqcc.getCacheListByQueryInThreadSafty(q);
			bbqb.InsertUpdateIntoDiskDB(*(bbqcv.get()), true);
		}
		BondDealCacheController::memCacheClear();
	}
	success_count++;
}

void CacheManager::InitLoginTokenCache() {
	LoginTokenCacheController::singleton()->init();
	success_count++;
}

bool CacheManager::InitDcsDealCache() {
	if (!kDcsEnable) {
		success_count++;
		return false;
	}

	DCSConnectionService::singleton()->getSwitch();

	int total_row_counts = 0;	  //总记录数，每次请求都返回总记录数
	int page_num = 0;			  //标记分页次数，设置分页偏移
	int page_size = kDcsPageSize; //每次请求DCS成交单数据的条数
	int ncounts = 0;			  //返回结果数
	bool success = true;

	DcsBondDealCacheController dcsBondDealCC;
	dcsBondDealCC.clearCache();

	LOGGER_DEBUG("----------------- CacheManager::InitDcsDealCache --------------------");
	do {
		DcsBondDealCacheVecPtr dcs_deal_vec(new std::vector<DcsBondDealCachePtr>());
		if (DCSConnectionService::singleton()->queryContractList(dcs_deal_vec, total_row_counts, page_num, page_size)) {
			ncounts += dcs_deal_vec->size();

			dcsBondDealCC.cacheTable(dcs_deal_vec);
			dcsBondDealCC.commit();

			++page_num;

			LOGGER_DEBUG("++++++ CacheManager Requested Rows: " << ncounts);
			LOGGER_DEBUG("++++++ CacheManager Total Rows: " << total_row_counts << ", Page Number: " << page_num);
		} else {
			success = false;
		}

	} while (total_row_counts != 0 && total_row_counts > ncounts && ncounts >= page_num * page_size);

	if ((total_row_counts == ncounts) && success) {
		success_count++;
		return true;
	} else {
		success_count++;
		return false;
	}
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

	messageTypeVec.push_back(mBondBestQuote_BestQuoteList);
	messageTypeVec.push_back(mBondBestQuote_CompletedQuoteList);
	messageTypeVec.push_back(mBondDeal_DealList);
	messageTypeVec.push_back(mDcsBondDeal_DealList);
	messageTypeVec.push_back(mBondQuote_QuoteList);
	messageTypeVec.push_back(mBondQuote_ReferQuoteList);
	SystemCompanyBusiness sc;
	SystemCompanyCacheVecPtr scv = sc.getSysCompany();
	for (std::vector<SystemCompanyCachePtr>::iterator it = scv->begin(); it != scv->end(); ++it) {
		for (std::vector<std::string>::iterator msgIt = messageTypeVec.begin(); msgIt != messageTypeVec.end(); ++msgIt) {
			VersionBusiness::InitVersion(*msgIt, (*it)->id);
		}
	}
}
