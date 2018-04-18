/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Deal Business
====================================================================================================================
   Date       Name                                  Description of Change
05-Jun-2014   James Xu                              Initial version
13-Jun-2014   James Xu                              Add AddBondDeal
20-Aug-2014   James Xu                              Add CopyDeal and CopyDetailedDeal
$HISTORY$
===================================================================================================================*/
#ifndef _BONDDEAL_BUSINESS_H_
#define _BONDDEAL_BUSINESS_H_

#include <vector>
#include <map>
#include "service/bonddeal_service.h"
#include "object/bonddeal.h"
#include <time.h>
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016
#include <boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;


class BondDealCache;

class BondDealBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    BondDealBusiness(BondDealService* service);

    BondDealCacheVecPtr FetchBondDeals(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    BondDealCacheVecPtr FetchBondDealsByDate(const std::string& cond, const std::string& datetime);

    BondDealCacheVecPtr GetBondDealByID(const std::string& bond_deal_id);

    int GetBondDealCount(const sdbus::string& cond);
    
	// Assemble Bond Deal Msg from Bond Deal Cache
	void AssembleBondDeal(BondDealCachePtr& bonddealcache, ptree* bond_deal_msg);

    bool AddBondDeal(const BondDeal& bond_deal, 
        sdbus::string* exported_text = NULL, 
        sdbus::string* bond_id = NULL, 
        ptree* bond_deal_msg_data = NULL, 
        bool manually_set_deal_id = false,
        bool ptree_is_not_list = false);
    bool UpdateBondDeal(const BondDeal& bond_deal, sdbus::string* exported_text = NULL,  ptree* bond_deal_msg = NULL);

    bool UpdateBondDeal(const BondDealVecPtr& vec, sdbus::string* exported_text = NULL, ptree* bond_deal_info_msg = NULL);
    bool DeleteBondDeal(const BondDealVecPtr& bond_deal_vec, ptree* delete_deal_msg_data = NULL);

    sdbus::string CopyDeal(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDeal(sdbus::VariantList deal_ids);

private:
    static void UpdateBondDealCallBack(BondDealCache* cache, void* param);
	static void UpdateBondDealSuccessed(BondDealCache* cache, void* param);
	static void UpdateBondDealFailed(BondDealCache* cache, void* param);

	static void DeleteBondDealCallBack(BondDealCache* cache, void* param);
	static void DeleteBondDealSuccessed(BondDealCache* cache, void* param);
	static void DeleteBondDealFailed(BondDealCache* cache, void* param);

    sdbus::string CopyDealForTP(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForPATR(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForBGC(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForICAP(sdbus::VariantList deal_ids);

    sdbus::string CopyDetailedDealForTP(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForPATR(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForBGC(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForICAP(sdbus::VariantList deal_ids);

    void Init();

    bool AddDeal(const BondDeal& bond_deal,  
        sdbus::string* exported_text = NULL, 
        sdbus::string* bond_id = NULL, 
        ptree* bond_deal_msg = NULL, 
        bool manually_set_deal_id = false,
        bool ptree_is_not_list = false);
	int GetDealSequenceNumber(const BondDealCachePtr& cache);

    BondDealService* service_;
	static boost::mutex add_deal_mutex_;
};

#endif // _BONDDEAL_BUSINESS_H_