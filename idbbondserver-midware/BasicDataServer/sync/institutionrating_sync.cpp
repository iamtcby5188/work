/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    InstitutionRatingSync 用于同步发行人评级表数据
====================================================================================================================
   Date       Name                                  Description of Change
20-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#include "institutionrating_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/cache_controller_declear.h"
#include "monitor/institutionrating_monitor.h"
#include "service/service_manager.h"
#include "service/bondservice.h"
#include "bondsmallview_sync.h"

InstitutionRatingSync::InstitutionRatingSync()
{

}

InstitutionRatingSync::~InstitutionRatingSync()
{

}

void InstitutionRatingSync::sync()
{
    InstitutionRatingCacheController institution_rating_cc;
    InstitutionRatingMonitor monitor;

    monitor.init("FetchInstitutionRatingSQL", InstitutionRatingCache::SCHEMA_NAME);
    
	std::vector<InstitutionRatingCache*> update_vec;
    std::vector<InstitutionRatingCache*> add_vec;
    std::vector<InstitutionRatingCache*> delete_vec;

    dbQuery q("");
    InstitutionRatingCacheVecPtr vec = institution_rating_cc.getCacheListByQueryInThreadSafty(q);

    db_sync<InstitutionRatingCacheController, InstitutionRatingMonitor, InstitutionRatingCache, InstitutionRatingCachePtr, InstitutionRatingCacheVecPtr>(
        institution_rating_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);
        
    LOGGER_INFO("update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size())
    
    for(int i = 0; i < update_vec.size(); ++i)
	{
		updateBondSmallViewCache(update_vec[i]);
	}

	for(int i = 0; i < add_vec.size(); ++i)
	{
		updateBondSmallViewCache(add_vec[i]);
	}
	
	for(int i = 0; i < delete_vec.size(); ++i)
	{
		std::string str = "institution_code = '" + delete_vec[i]->institution_code + "'";
		dbQuery q(str.c_str());
		//根据institution_code查询到的发行人评级信息只有一条
		InstitutionRatingCachePtr institution_rating_cache = institution_rating_cc.getCacheByQueryInThreadSafty(q);

		//若发行人评级信息不存在，则将BondSmallViewCache中issuer_rating_current和outlook字段置空
		if(!institution_rating_cache)
		{
			InstitutionRatingCachePtr cache(new InstitutionRatingCache());
			cache->rate = "";
			cache->outlook = "";
			institution_rating_cache = cache;
		}

		updateBondSmallViewCache(institution_rating_cache.get());
	}

}

void InstitutionRatingSync::updateBondSmallViewCache(InstitutionRatingCache* cache)
{
	if(NULL == cache) return;

	BondSmallViewCacheController bond_small_view_cc;
	std::string str = "issuer_code = '" + cache->institution_code + "'";
	dbQuery q(str.c_str());
	BondSmallViewCacheVecPtr bond_small_view_vec = bond_small_view_cc.getCacheListByQueryInThreadSafty(q);
	if(bond_small_view_vec->size() < 1) return;

	std::vector<BondSmallViewCachePtr>::iterator it = bond_small_view_vec->begin();
	for( ; it != bond_small_view_vec->end(); ++it) 
	{
		BondSmallViewCachePtr bond_small_view_cache = *it;
		if(bond_small_view_cache)
		{
			bond_small_view_cache->issuer_rating_current = cache->rate;
			bond_small_view_cache->outlook = cache->outlook;

			std::string str = "bond_key_listed_market = '" + bond_small_view_cache->bond_key_listed_market + "'";
			dbQuery q(str.c_str());
			bond_small_view_cc.updateCacheInThreadSaftyWithoutDBSync(q, bond_small_view_cache.get());
		}
	}
}

