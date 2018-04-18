#include "bondbestquotecalc_business.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/controller/bondbestquote_cachecontroller.h"
#include "cache/cache_center.h"
#include "service/service_manager.h"
#include "service/qb_sync_service.h"
#include "include/common.h"
#include "fid.h"
#include <set>

bool func_equal_bid(const BondQuoteCachePtr &lhs, const BondQuoteCachePtr &rhs)
{
    better_bid comp(true);
    return (!comp(lhs, rhs) && !comp(rhs, lhs));
}

bool func_equal_ofr(const BondQuoteCachePtr &lhs, const BondQuoteCachePtr &rhs)
{
    better_ofr comp(true);
    return (!comp(lhs, rhs) && !comp(rhs, lhs));
}

void BondBestQuoteCalcBusiness::MakeAllBestPrice()
{
    dbQuery q;
    q = "";    
    BondSmallViewCacheController bondSmallViewCC;
    BondSmallViewCacheVecPtr bondSmallViewList = bondSmallViewCC.getCacheListByQueryInThreadSafty(q);
    LOGGER_INFO("bond small view size: " << bondSmallViewList->size())
    for (unsigned int i = 0; i < bondSmallViewList->size(); ++i) {
        MakeBestPrice(bondSmallViewList->at(i)->bond_key_listed_market, false);
    }
    LOGGER_INFO("Init Bond Best Quote Over!")
}

BondBestQuoteCachePtr BondBestQuoteCalcBusiness::MakeBestPrice(const std::string& bond_key_listed_market, bool update)
{
    std::string str =  " bond_key_listed_market = '" + bond_key_listed_market + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());
    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_bid_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    best_price_ofr_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    best_price_bid_with_internal_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    best_price_ofr_with_internal_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());

    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        if((*it)->symbol == 1) {
            best_price_bid_with_internal_->push_back(*it);
            if((*it)->internally == "1")
                best_price_bid_->push_back(*it);
        } else if((*it)->symbol == -1) {
            best_price_ofr_with_internal_->push_back(*it);
            if((*it)->internally == "1")
                best_price_ofr_->push_back(*it);
        }
    }

    best_price_bid_->sort(better_bid());
    best_price_ofr_->sort(better_ofr());
    best_price_bid_with_internal_->sort(better_bid());
    best_price_ofr_with_internal_->sort(better_ofr());

	BondBestQuoteCachePtr cache = UpdateBestPrice(bond_key_listed_market, update);
	return cache;
}

std::vector<BondBestQuoteCachePtr> BondBestQuoteCalcBusiness::UpdateTime(const std::string& bond_key, const std::vector<std::string>& bond_key_listed_market_vec)
{
    std::string sql_str = " bond_key = '" + bond_key + "' " + GetCompanyConditionStringWithPreAnd() + " and bond_key_listed_market not in "; //Modify by Young 05-Apr-2016
    sql_str += "(";
    for (unsigned int i = 0; i < bond_key_listed_market_vec.size(); ++i)
    {
        if (i > 0) {
            sql_str += ", ";
        }
        sql_str += "'" + bond_key_listed_market_vec.at(i) + "'";
    }
    sql_str += ")";
    dbQuery q(sql_str.c_str());

    std::vector<BondBestQuoteCachePtr> bond_best_quote_vec;
    BondBestQuoteCacheController bond_best_quote_cc;

    bond_best_quote_cc.updateCacheByQueryInThreadSaftyWithoutDBSync(q, UpdateTimeCallBack, NULL, UpdateTimeSuccessed, UpdateTimeFailed, (void*)(&bond_best_quote_vec));
	
    return bond_best_quote_vec;
}

void BondBestQuoteCalcBusiness::UpdateTimeCallBack(BondBestQuoteCache* cache, void* param)
{
	if (cache) {
		cache->create_time_milli_sec = GetCurrentTimeMilliSec();
		cache->create_time = GetCurrentTimeString();
	}
}

void BondBestQuoteCalcBusiness::UpdateTimeSuccessed(BondBestQuoteCache* cache, void* param) 
{
	if (!cache)
		return;
    std::vector<BondBestQuoteCachePtr>* bond_best_quote_vec = (std::vector<BondBestQuoteCachePtr>*)param;

    BondBestQuoteCache* bond_best_quote = new BondBestQuoteCache();
    *bond_best_quote = *cache;

    bond_best_quote_vec->push_back(BondBestQuoteCachePtr(bond_best_quote));
    LOGGER_INFO("UpdateTime: Success for updating BondBestQuoteCache: " << cache->bond_key_listed_market)
}

void BondBestQuoteCalcBusiness::UpdateTimeFailed(BondBestQuoteCache* cache, void* param) 
{
	if (cache)
		LOGGER_ERROR("UpdateBondQuote: Failed for updating BondQuoteCache: " << cache->bond_key_listed_market)
}

void BondBestQuoteCalcBusiness::SortPriceBid(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' and symbol = 1 and internally = '1'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_bid_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_bid_->push_back(*it);
    }
    
    best_price_bid_->sort(better_bid());

    return;
}

void BondBestQuoteCalcBusiness::SortPriceOfr(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' and symbol = -1 and internally = '1'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_ofr_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_ofr_->push_back(*it);
    }

    best_price_ofr_->sort(better_ofr());

    return;
}

void BondBestQuoteCalcBusiness::SortPriceBidWithInternal(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' and symbol = 1" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_bid_with_internal_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_bid_with_internal_->push_back(*it);
    }
    
    best_price_bid_with_internal_->sort(better_bid());

    return;
}

void BondBestQuoteCalcBusiness::SortPriceOfrWithInternal(const std::string &bond_key_listed_market)
{
    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "' and symbol = -1" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());

    BondQuoteCacheController bond_quote_cc;
    BondQuoteCacheVecPtr bond_quote_cache_vec = bond_quote_cc.getCacheListByQueryInThreadSafty(q);

    best_price_ofr_with_internal_ = BondQuoteCacheListPtr(new std::list<BondQuoteCachePtr>());
    std::vector<BondQuoteCachePtr>::const_iterator it = bond_quote_cache_vec->begin();
    for( ; it != bond_quote_cache_vec->end(); ++it) {
        best_price_ofr_with_internal_->push_back(*it);
    }
    
    best_price_ofr_with_internal_->sort(better_ofr());

    return;
}

BondBestQuoteCachePtr BondBestQuoteCalcBusiness::UpdateBestPrice(const std::string& bond_key_listed_market, bool update)
{
    BondBestQuoteCachePtr bp = BondBestQuoteCachePtr(new BondBestQuoteCache());
    bp->company_id = GetCompanyId();
    bp->bond_key_listed_market = bond_key_listed_market;

    //bid
    std::vector<BondQuoteCachePtr> equal_bid;
    if (best_price_bid_->size() > 0) { 
        equal_bid.push_back(best_price_bid_->front());
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_->begin();
        for( ; it != best_price_bid_->end(); ++it){
            if (it == best_price_bid_->begin()) {
                continue;
            }
            if(func_equal_bid(best_price_bid_->front(), *it)){
                equal_bid.push_back(*it);
            }
            else{
                break;
            }
        }
    }

	std::vector<std::string> bid_descriptions, bid_volumes, ofr_descriptions, ofr_volumes;
    bp->bid_offer_num = best_price_bid_->size();
    bp->bid_offer_count = equal_bid.size();
    for(unsigned int i = 0; i < equal_bid.size(); ++i) {
        const BondQuoteCachePtr &ba = equal_bid[i];
        bp->company_id = ba->company_id;
		std::string _desc = getDesc(ba);
		if (!_desc.empty())
			bid_descriptions.push_back(_desc);
		std::string _vol = truncateDouble(ba->volume, 4);
		if (!_vol.empty())
			bid_volumes.push_back(_vol);
		else
			bid_volumes.push_back("0");
        if (0 == i) {
            bp->bid_offer_id = ba->id;
            bp->bid_net_price = ba->net_price;
            bp->bid_price = ba->price;
            bp->bid_full_price = ba->full_price;
            bp->bid_spread = ba->spread;
            bp->bid_yield = ba->yield;
 
            std::stringstream ssVolume;
            ssVolume << Format(ba->volume, 0, 4);
            bp->bid_volume = ssVolume.str();

            if (DOUBLE_NULL == ba->volume || 0 == ba->volume) {
                bp->bid_volume = "--";
            } else {
                std::stringstream ssVolume;
                ssVolume << Format(ba->volume, 0, 4);
				bp->bid_volume = ssVolume.str();
				bp->bid_volume_total += ba->volume;
            }
            bp->vol_bid_client = ba->volume;
            
            bp->bond_key = ba->bond_key;
            bp->listed_market = ba->listed_market;
            bp->goods_code = ba->goods_code;

            bp->bid_trader_id = ba->trader_id;
            bp->bid_flag_bargain = ba->flag_bargain;
            if (equal_bid.size() > 1) {
                bp->best_bid_description = bp->bid_volume;
                if(ba->description.length() > 0){
                    bp->best_bid_description += "(";
                    bp->best_bid_description += ba->description;
                    bp->best_bid_description += ")";
                }
            } else {
                bp->best_bid_description = ba->description;
            }
            bp->bid_flag_relation = ba->flag_relation;
            bp->bid_quote_type = ba->quote_type;
            bp->bid_rebate = ba->rebate;
            bp->bid_return_point = ba->return_point;
            bp->bid_clear_speed = ba->clear_speed;
            bp->bid_delivery_date = ba->delivery_date;
            bp->bid_exercise = ba->exercise;			
        }else{
            bp->bid_volume += "+";
            bp->best_bid_description += '\n';
            if (DOUBLE_NULL == ba->volume || 0 == ba->volume) {
                bp->bid_volume += "--";
                bp->best_bid_description += "--";
            } else {
                std::stringstream ssVolume;
                ssVolume << ba->volume;
                bp->bid_volume += ssVolume.str();
				bp->best_bid_description += ssVolume.str();
				bp->bid_volume_total += ba->volume;
            }
            bp->vol_bid_client += ba->volume;
            if(ba->description.length() > 0){
                bp->best_bid_description += "(";
                bp->best_bid_description += ba->description;
                bp->best_bid_description += ")";
            }
        }
    }
	if (!equal_bid.empty())
		bp->bid_description = join(bid_descriptions, "+") + "," + join(bid_volumes, "+");

    //ofr
    std::vector<BondQuoteCachePtr> equal_ofr;
    if (best_price_ofr_->size() > 0) { 
        equal_ofr.push_back(best_price_ofr_->front());
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_->begin();
        for( ; it != best_price_ofr_->end(); ++it){
            if (it == best_price_ofr_->begin()) {
                continue;
            }
            if(func_equal_ofr(best_price_ofr_->front(), *it)){
                equal_ofr.push_back(*it);
            }
            else{
                break;
            }
        }
    }
    bp->ofr_offer_num = best_price_ofr_->size();
    bp->ofr_offer_count = equal_ofr.size();
    for(unsigned int i = 0; i < equal_ofr.size(); ++i) {
		const BondQuoteCachePtr &ba = equal_ofr[i];
		bp->company_id = ba->company_id;
		std::string desc = getDesc(ba);
		if (!desc.empty())
			ofr_descriptions.push_back(desc);
		std::string _vol = truncateDouble(ba->volume, 4);
		if (!_vol.empty())
			ofr_volumes.push_back(_vol);
		else
			ofr_volumes.push_back("0");
        if (0 == i) {
            bp->ofr_offer_id = ba->id;
            bp->ofr_net_price = ba->net_price;
            bp->ofr_price = ba->price;
            bp->ofr_full_price = ba->full_price;
            bp->ofr_spread = ba->spread;
            bp->ofr_yield = ba->yield;

            if (DOUBLE_NULL == ba->volume || 0 == ba->volume) {
                bp->ofr_volume = "--";
            } else {
                std::stringstream ssVolume;
                ssVolume << Format(ba->volume, 0, 4);
				bp->ofr_volume = ssVolume.str();
				bp->ofr_volume_total += ba->volume;
            }
            bp->vol_ofr_client = ba->volume;
         
            bp->bond_key = ba->bond_key;
            bp->listed_market = ba->listed_market;
            bp->goods_code = ba->goods_code;

            bp->ofr_trader_id = ba->trader_id;
            bp->ofr_flag_bargain = ba->flag_bargain;
            if (equal_ofr.size() > 1) {
                bp->best_ofr_description = bp->ofr_volume;
                if(ba->description.length() > 0){
                    bp->best_ofr_description += "(";
                    bp->best_ofr_description += ba->description;
                    bp->best_ofr_description += ")";
                }
            } else {
                bp->best_ofr_description = ba->description;
            }
            bp->ofr_flag_relation = ba->flag_relation;
            bp->ofr_quote_type = ba->quote_type;
            bp->ofr_rebate = ba->rebate;
            bp->ofr_return_point = ba->return_point;
            bp->ofr_clear_speed = ba->clear_speed;
            bp->ofr_delivery_date = ba->delivery_date;
            bp->ofr_exercise = ba->exercise;
        }
        else{
            bp->ofr_volume += "+";
            bp->best_ofr_description += '\n';
            if (DOUBLE_NULL == ba->volume || 0 == ba->volume) {
                bp->ofr_volume += "--";
                bp->best_ofr_description += "--";
            } else {
                std::stringstream ssVolume;
                ssVolume << Format(ba->volume, 0, 4);
                bp->ofr_volume += ssVolume.str();
				bp->best_ofr_description += ssVolume.str();
				bp->ofr_volume_total += ba->volume;
            }            
            bp->vol_ofr_client += ba->volume;
            if(ba->description.length() > 0){
                bp->best_ofr_description += "(";
                bp->best_ofr_description += ba->description;
                bp->best_ofr_description += ")";
            }
        }
	}
	if (!equal_ofr.empty())
		bp->ofr_description = join(ofr_descriptions, "+") + "," + join(ofr_volumes, "+");

    //bid_with_internal
    std::vector<BondQuoteCachePtr> equal_internally_bid;
    if (best_price_bid_with_internal_->size() > 0) {
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_with_internal_->begin();
        for( ; it != best_price_bid_with_internal_->end(); ++it){
            if (it == best_price_bid_with_internal_->begin() || func_equal_bid(best_price_bid_with_internal_->front(), *it)) {
                if ((*it)->internally != "1") { // 内部报价
                    equal_internally_bid.push_back(*it);
                }
            } else{
                break;
            }
        }
    }

    bp->internally_bid_offer_count = best_price_bid_with_internal_->size();
    if (equal_internally_bid.size() > 0) {
        const BondQuoteCachePtr &ba = equal_internally_bid[0];        

        bp->company_id = ba->company_id;
        bp->internally_bid_offer_id = ba->id;
        bp->internally_bid_price = ba->price;
        bp->internally_bid_rebate = ba->rebate;
        bp->internally_bid_return_point = ba->return_point;

        bp->bond_key = ba->bond_key;
        bp->listed_market = ba->listed_market;
        bp->goods_code = ba->goods_code;
    }

    //ofr_with_internal
    std::vector<BondQuoteCachePtr> equal_internally_ofr;
    if (best_price_ofr_with_internal_->size() > 0) {
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_with_internal_->begin();
        for( ; it != best_price_ofr_with_internal_->end(); ++it){
            if (it == best_price_ofr_with_internal_->begin() || func_equal_bid(best_price_ofr_with_internal_->front(), *it)) {
                if ((*it)->internally != "1") { // 内部报价
                    equal_internally_ofr.push_back(*it);
                }
            } else{
                break;
            }
        }
    }

    bp->internally_ofr_offer_count = best_price_ofr_with_internal_->size();
    if (equal_internally_ofr.size() > 0) {
        const BondQuoteCachePtr &ba = equal_internally_ofr[0];

        bp->company_id = ba->company_id;
        bp->internally_ofr_offer_id = ba->id;
        bp->internally_ofr_price = ba->price;
        bp->internally_ofr_rebate = ba->rebate;
        bp->internally_ofr_return_point = ba->return_point;

        bp->bond_key = ba->bond_key;
        bp->listed_market = ba->listed_market;
        bp->goods_code = ba->goods_code;
    }

	bp->create_time_milli_sec = GetCurrentTimeMilliSec();
    bp->create_time = GetCurrentTimeString();
	
    // 获取所有报价的broker name
    std::set<std::string> broker_id_set;
    std::set<std::string> trader_id_set;
    std::list<BondQuoteCachePtr>::const_iterator it_bid = best_price_bid_with_internal_->begin();
    for (; it_bid != best_price_bid_with_internal_->end(); ++it_bid) {
        broker_id_set.insert((*it_bid)->operator_id);
        trader_id_set.insert((*it_bid)->bank_agent_id);
    }   
    std::list<BondQuoteCachePtr>::const_iterator it_ofr = best_price_ofr_with_internal_->begin();
    for (; it_ofr != best_price_ofr_with_internal_->end(); ++it_ofr) {
        broker_id_set.insert((*it_ofr)->operator_id);
        trader_id_set.insert((*it_ofr)->bank_agent_id);
    }
    std::string broker_id_list;
    for (std::set<std::string>::const_iterator iter = broker_id_set.begin(); iter != broker_id_set.end(); ++iter) {
        if (broker_id_list.length() > 0) {
            broker_id_list += ",";
        }
        broker_id_list += *iter;
        
    }
    bp->broker_id_list = broker_id_list;

    std::string trader_id_list;
    for (std::set<std::string>::const_iterator iter = trader_id_set.begin(); iter != trader_id_set.end(); ++iter) {
        if (trader_id_list.length() > 0) {
            trader_id_list += ",";
        }
        trader_id_list += *iter;
    }
    bp->trader_id_list = trader_id_list;

	CacheCenter::sharedInstance()->AttachCurrentThread();
    bp->loadCombinationFields();
    CacheCenter::sharedInstance()->DetachCurrentThread();

    std::string str = " bond_key_listed_market = '" + bond_key_listed_market + "'" + GetCompanyConditionStringWithPreAnd(); //Modify by Young 05-Apr-2016
    dbQuery q(str.c_str());
    BondBestQuoteCacheController bond_best_quote_cc;

    if (update) {
        // for update, delete old data first
        if (!bond_best_quote_cc.deleteCacheByQueryInThreadSaftyWithoutDBSync(q)) {
            LOGGER_ERROR("delete old bond best quote occur failure!")
        }
    }

    if (!bond_best_quote_cc.insertCacheInThreadSaftyWithoutDBSync(bp.get()))
    {
        LOGGER_ERROR("insert bond best quote occur failure!")
    }

    return bp;
}

int BondBestQuoteCalcBusiness::GetBestQuoteSortWithInternal(const std::string &bond_key_listed_market, std::vector<std::pair<BondQuoteCachePtr, BondQuoteCachePtr> > &result)
{
    SortPriceBidWithInternal(bond_key_listed_market);
    SortPriceOfrWithInternal(bond_key_listed_market);

    int bid_best_quote_count = 0;
    int ofr_best_quote_count = 0;

    if (best_price_bid_with_internal_->size() > 0) {
        bid_best_quote_count = 1;
       
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_with_internal_->begin();
        for (it++ ; it != best_price_bid_with_internal_->end(); ++it) {
            if (func_equal_bid(best_price_bid_with_internal_->front(), *it)) {
                bid_best_quote_count++;
            } else {
                break;
            }
        }
    }

    if (best_price_ofr_with_internal_->size() > 0) {
        ofr_best_quote_count = 1;
        
        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_with_internal_->begin();
        for (it++ ; it != best_price_ofr_with_internal_->end(); ++it) {
            if (func_equal_ofr(best_price_ofr_with_internal_->front(), *it)) {
                ofr_best_quote_count++;
            } else {
                break;
            }
        }
    }

    int i = 0;
    int j = 0;
    std::list<BondQuoteCachePtr>::const_iterator it_bid = best_price_bid_with_internal_->begin();
    std::list<BondQuoteCachePtr>::const_iterator it_ofr = best_price_ofr_with_internal_->begin();
    while (i < bid_best_quote_count || j < ofr_best_quote_count){
        std::pair<BondQuoteCachePtr, BondQuoteCachePtr> one_pair;
        if(i < bid_best_quote_count){
            one_pair.first = *it_bid;
            it_bid++;
            i++;
        }

        if(j < ofr_best_quote_count){
            one_pair.second = *it_ofr;
            it_ofr++;
            j++;
        }
        result.push_back(one_pair);
    }

    while (i < best_price_bid_with_internal_->size() || j < best_price_ofr_with_internal_->size()){
        std::pair<BondQuoteCachePtr, BondQuoteCachePtr> one_pair;
        if(i < best_price_bid_with_internal_->size()){
            one_pair.first = *it_bid;
            it_bid++;
            i++;
        }

        if(j < best_price_ofr_with_internal_->size()){
            one_pair.second = *it_ofr;
            it_ofr++;
            j++;
        }
        result.push_back(one_pair);
    }

    return std::max<int>(bid_best_quote_count, ofr_best_quote_count);
}


std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteBidWithInternal(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceBidWithInternal(bond_key_listed_market);

    int bid_best_quote_count = 0;

    if (best_price_bid_with_internal_->size() > 0) {
        bid_best_quote_count = 1;
        result.push_back(best_price_bid_with_internal_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_with_internal_->begin();
        for (it++ ; it != best_price_bid_with_internal_->end(); ++it) {
            if (func_equal_bid(best_price_bid_with_internal_->front(), *it)) {
                bid_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}

std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteOfrWithInternal(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceOfrWithInternal(bond_key_listed_market);

    int ofr_best_quote_count = 0;

    if (best_price_ofr_with_internal_->size() > 0) {
        ofr_best_quote_count = 1;
        result.push_back(best_price_ofr_with_internal_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_with_internal_->begin();
        for (it++ ; it != best_price_ofr_with_internal_->end(); ++it) {
            if (func_equal_ofr(best_price_ofr_with_internal_->front(), *it)) {
                ofr_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}

std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteBid(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceBid(bond_key_listed_market);

    int bid_best_quote_count = 0;

    if (best_price_bid_->size() > 0) {
        bid_best_quote_count = 1;
        result.push_back(best_price_bid_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_bid_->begin();
        for (it++ ; it != best_price_bid_->end(); ++it) {
            if (func_equal_bid(best_price_bid_->front(), *it)) {
                bid_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}

std::vector<BondQuoteCachePtr> BondBestQuoteCalcBusiness::GetBestQuoteOfr(const std::string &bond_key_listed_market)
{
    std::vector<BondQuoteCachePtr> result;

    SortPriceOfr(bond_key_listed_market);

    int ofr_best_quote_count = 0;

    if (best_price_ofr_->size() > 0) {
        ofr_best_quote_count = 1;
        result.push_back(best_price_ofr_->front());

        std::list<BondQuoteCachePtr>::const_iterator it = best_price_ofr_->begin();
        for (it++ ; it != best_price_ofr_->end(); ++it) {
            if (func_equal_ofr(best_price_ofr_->front(), *it)) {
                ofr_best_quote_count++;
                result.push_back(*it);
            } else {
                break;
            }
        }
    }

    return result;
}

std::string BondBestQuoteCalcBusiness::getDesc(const BondQuoteCachePtr& cache) {
	std::ostringstream oss;
	bool isIntention = (DOUBLE_NULL == cache->price || 0 == cache->price) && "0" == cache->rebate,
		isNeedBlank = isIntention;
	if (kBidQuote == cache->symbol) {
		oss << "bid(" << (isIntention ? "bid" : "");
	} else
		oss << "ofr(" << (isIntention ? "ofr" : "");
	if ("1" == cache->flag_bargain) {
		oss << (isNeedBlank ? " *" : "*");
		isNeedBlank = true;
	} else if ("2" == cache->flag_bargain) {
		oss << (isNeedBlank ? " **" : "**");
		isNeedBlank = true;
	}
	if (!cache->price_description.empty()) { // do not use cache->description 
		oss << (isNeedBlank ? " " : "") << cache->price_description;
		isNeedBlank = true;
	}
	oss << ")";
	return oss.str();
}
