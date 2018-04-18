/*
 * CacheCenter.h
 *
 *  Created on: Jun 3, 2014
 *      Author: theochen
 */

#ifndef CACHECONTROLLERDECLEAR_H_
#define CACHECONTROLLERDECLEAR_H_

#include "cache_controller.h"
#include "model/account_group_cache.h"
#include "model/account_permission_cache.h"
#include "model/bondtrader_cache.h"
#include "model/bondtypeselective_cache.h"
#include "model/chinabonddeviatedvalue_cache.h"
#include "model/constants_cache.h"
#include "model/holiday_info_cache.h"
#include "model/issuer_info_cache.h"
#include "model/issuinginstitution_cache.h"
#include "model/province_info_cache.h"
#include "model/selectiveinfo_cache.h"
#include "model/sw_sector_info_cache.h"
#include "model/system_company_cache.h"
#include "model/tradinginstitution_cache.h"
#include "model/institutionrating_cache.h"
#include "model/version_cache.h"

typedef CacheController<AccountGroupCache> AccountGroupCacheController;
typedef CacheController<AccountPermissionCache> AccountPermissionCacheController;
typedef CacheController<BondTraderCache> BondTraderCacheController;
typedef CacheController<BondTypeSelectiveCache> BondTypeSelectiveCacheController;
typedef CacheController<ChinaBondDeviatedValueCache> ChinaBondDeviatedValueCacheController;
typedef CacheController<ConstantsCache> ConstantsCacheController;
typedef CacheController<HolidayInfoCache> HolidayInfoCacheController;
typedef CacheController<IssuerInfoCache> IssuerInfoCacheController;
typedef CacheController<IssuingInstitutionCache> IssuingInstitutionCacheController;
typedef CacheController<ProvinceInfoCache> ProvinceInfoCacheController;
typedef CacheController<SelectiveInfoCache> SelectiveInfoCacheController;
typedef CacheController<SwSectorInfoCache> SwSectorInfoCacheController;
typedef CacheController<SystemCompanyCache> SystemCompanyCacheController;
typedef CacheController<TradingInstitutionCache> TradingInstitutionCacheController;
typedef CacheController<InstitutionRatingCache> InstitutionRatingCacheController;
typedef CacheController<VersionCache> VersionCacheController;

//class TradingInstitutionCacheController : public CacheController<TradingInstitutionCache>, sdbus::Callback {

#endif /* CACHECONTROLLERDECLEAR_H_ */
