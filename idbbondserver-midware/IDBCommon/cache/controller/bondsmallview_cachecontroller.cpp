/*
 * BondSmallViewCacheController.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "cache/cache_center.h"
#include "bondsmallview_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include <ctime>
#include <unordered_map>

BondSmallViewCacheController::BondSmallViewCacheController() {
    // TODO Auto-generated constructor stub
}

BondSmallViewCacheController::~BondSmallViewCacheController() {
    // TODO Auto-generated destructor stub
}

void BondSmallViewCacheController::loadAcrossMarketBondIds()
{
    /*dbQuery query("");
    BondSmallViewCacheVecPtr bond_small_view_cache_vec = getCacheListByQueryInThreadSafty(query);

    std::vector<BondSmallViewCachePtr>::iterator it = bond_small_view_cache_vec->begin();
    for( ; it != bond_small_view_cache_vec->end(); ++it) {
        BondSmallViewCachePtr cache = *it;
        if (cache) {
            loadAcrossMarketBondIds(cache.get());
        }
    }*/
	LOGGER_INFO("====smallview loadAcrossMarketBondIds query ...");
	dbQuery query("");
	BondSmallViewCacheVecPtr bond_small_view_cache_vec = getCacheListByQueryInThreadSafty(query);
	if(bond_small_view_cache_vec->empty()){
		return;
	}
	LOGGER_INFO("====smallview loadAcrossMarketBondIds query size: " << bond_small_view_cache_vec->size());
	std::unordered_map<std::string, std::string> map;
	std::vector<BondSmallViewCachePtr>::iterator it = bond_small_view_cache_vec->begin();
	for( ; it != bond_small_view_cache_vec->end(); ++it) {
		BondSmallViewCachePtr cache = *it;
		if (cache && !cache->bond_id.empty()) {
			std::string key = cache->bond_key;
			if(map.find(key) == map.end()){
				map[key] = cache->bond_id;
			} else if (map[key] == cache->bond_id) {
				continue;
			} else {
				map[key] = map[key] + "," + cache->bond_id;
			}
		}
	}
	if(map.empty()){
		return;
	}
	LOGGER_INFO("====smallview loadAcrossMarketBondIds underedmap size: " << map.size());
	it = bond_small_view_cache_vec->begin();
	dbQuery q;
	int count = 0;
	for( ; it != bond_small_view_cache_vec->end(); ++it) {
		BondSmallViewCachePtr cache = *it;
		if (cache && !cache->bond_id.empty()) {
			const std::string t_bond_ids = map[cache->bond_key];
			if (t_bond_ids.empty() || cache->across_market_bond_ids == t_bond_ids)
				continue;
			cache->across_market_bond_ids += "," + t_bond_ids;
			
			q = "bond_key_listed_market = ", cache->bond_key_listed_market;
			updateCacheInThreadSaftyWithoutDBSync(q, cache.get());
			count++;
		}
	}
	LOGGER_INFO("====smallview loadAcrossMarketBondIds update size: " << count);
}

void BondSmallViewCacheController::loadAcrossMarketBondIds(BondSmallViewCache *cache)
{
    std::string across_market_bond_ids;
    dbQuery q;
    q = "bond_key = ", cache->bond_key;
    BondSmallViewCacheVecPtr bond_small_view_cache_vec = getCacheListByQueryInThreadSafty(q);
    std::vector<BondSmallViewCachePtr>::const_iterator it = bond_small_view_cache_vec->begin();
    for( ; it != bond_small_view_cache_vec->end(); ++it) {
        if (*it) {
            if (across_market_bond_ids.length() > 0) {
                across_market_bond_ids += ",";
            }
            across_market_bond_ids += (*it)->bond_id;
        }
    }
    cache->across_market_bond_ids = across_market_bond_ids;

    q = "bond_key_listed_market = ", cache->bond_key_listed_market;
    updateCacheInThreadSaftyWithoutDBSync(q, cache);
}

/* Add by Young 20-Apr-2016 
 * 更新债券发行人评级信息
*/
void BondSmallViewCacheController::loadIssuerRating()
{
	LOGGER_INFO("====smallview loadIssuerRating query ...");
	dbQuery q("");
	BondSmallViewCacheVecPtr bond_small_view_vec = getCacheListByQueryInThreadSafty(q);
	if(bond_small_view_vec->empty()) return;
	LOGGER_INFO("====smallview loadIssuerRating bond size: " << bond_small_view_vec->size());

	InstitutionRatingCacheController institution_rating_cc;
    InstitutionRatingCacheVecPtr institution_rating_vec = institution_rating_cc.getCacheListByQueryInThreadSafty(q);
	if(institution_rating_vec->empty()) return;

	LOGGER_INFO("====smallview loadIssuerRating institution rating size: " << institution_rating_vec->size());

    std::unordered_map<std::string, InstitutionRatingCachePtr> irrCache;
    for (size_t i = 0; i < institution_rating_vec->size(); ++i) {
        InstitutionRatingCachePtr irrc = institution_rating_vec->at(i);
        if (irrc) {
            irrCache[irrc->institution_code] = irrc;
        }
    }
	LOGGER_INFO("====smallview loadIssuerRating institution rating underedmap size: " << irrCache.size());

	int count = 0;
	std::vector<BondSmallViewCachePtr>::iterator bond_small_view_it = bond_small_view_vec->begin();
	for( ; bond_small_view_it != bond_small_view_vec->end(); ++bond_small_view_it) 
	{
		BondSmallViewCachePtr bond_small_view_cache = *bond_small_view_it;
		if(bond_small_view_cache)
		{
			/*bool isMatched = false; //用于判断BondSmallViewCache中issuer_code字段和InstitutionRatingCache中institution_code是否匹配

			std::vector<InstitutionRatingCachePtr>::iterator institution_rating_it = institution_rating_vec->begin();
			for( ; institution_rating_it != institution_rating_vec->end(); ++institution_rating_it)
			{
				InstitutionRatingCachePtr institution_rating_cache = *institution_rating_it;
				if(institution_rating_cache)
				{
					if(bond_small_view_cache->issuer_code == institution_rating_cache->institution_code)
					{
						isMatched = true;
						bond_small_view_cache->issuer_rating_current = institution_rating_cache->rate;
						bond_small_view_cache->outlook = institution_rating_cache->outlook;

						break;
					}
				}
			}

			//若匹配不成功，则将issuer_rating_current和outlook字段置空
			if(!isMatched)
			{
				bond_small_view_cache->issuer_rating_current = "";
				bond_small_view_cache->outlook = "";
			}*/
            if (bond_small_view_cache->issuer_code.empty() || irrCache.find(bond_small_view_cache->issuer_code) == irrCache.end()) {
                bond_small_view_cache->issuer_rating_current = "";
                bond_small_view_cache->outlook = "";
				continue;
            } else {
                InstitutionRatingCachePtr irrc = irrCache.at(bond_small_view_cache->issuer_code);
                if (irrc) {
                    bond_small_view_cache->issuer_rating_current = irrc->rate;
                    bond_small_view_cache->outlook = irrc->outlook;
                } else {
                    bond_small_view_cache->issuer_rating_current = "";
                    bond_small_view_cache->outlook = "";
					continue;
                }
            }

			std::string str = "bond_key_listed_market = '" + bond_small_view_cache->bond_key_listed_market + "'";
			dbQuery q(str.c_str());
			updateCacheInThreadSaftyWithoutDBSync(q, bond_small_view_cache.get());
			count++;
		}
	}
	LOGGER_INFO("====smallview loadIssuerRating update size: " << count);
}

/* Add by Young 20-Apr-2016 
 * 更新债券发行人评级信息
*/
void BondSmallViewCacheController::loadIssuerRating(BondSmallViewCache *cache)
{
	if(NULL == cache) return;

	InstitutionRatingCacheController institution_rating_cc;
    std::string str = "institution_code = '" + cache->issuer_code + "'";
    dbQuery q(str.c_str());
	//根据institution_code查询到的发行人评级信息只有一条
    InstitutionRatingCachePtr institution_rating_cache = institution_rating_cc.getCacheByQueryInThreadSafty(q);
	
	if(institution_rating_cache)
	{
		cache->issuer_rating_current = institution_rating_cache->rate;
		cache->outlook = institution_rating_cache->outlook;
	}
	else //若发行人评级信息不存在，则将BondSmallViewCache中issuer_rating_current和outlook字段置空
	{
		cache->issuer_rating_current = "";
		cache->outlook = "";
	}

	str = "bond_key_listed_market = '" + cache->bond_key_listed_market + "'";
	q = str.c_str();
	updateCacheInThreadSaftyWithoutDBSync(q, cache);
}

void BondSmallViewCacheController::insertCache(sql::ResultSet* resSet)
{
    SET_PROPERTIES_BEGIN(BondSmallViewCache);

    //TIME_COST_BEGIN("insertBondSmallView");

    // Set customerized properties
    //CacheCenter::sharedInstance()->AttachCurrentThread();
    cache.loadCombinationFields();
    //CacheCenter::sharedInstance()->DetachCurrentThread();

    SET_PROPERTIES_END;
    
    //TIME_COST_END("insertBondSmallView");
}

