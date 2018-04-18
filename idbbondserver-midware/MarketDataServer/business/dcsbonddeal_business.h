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
#ifndef _DCSBONDDEAL_BUSINESS_H_
#define _DCSBONDDEAL_BUSINESS_H_

#include <vector>
#include <map>
#include "service/dcsbonddeal_service.h"
#include "object/bonddeal.h"
#include <time.h>
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016

class DcsBondDealCache;

class DcsBondDealBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    DcsBondDealBusiness(DcsBondDealService* service);

    DcsBondDealCacheVecPtr FetchBondDeals(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    DcsBondDealCacheVecPtr FetchBondDealsByDate(const std::string& cond, const std::string& datetime);

    DcsBondDealCacheVecPtr GetBondDealByID(const std::string& bond_deal_id);

    int GetBondDealCount(const sdbus::string& cond);
    
    bool AddBondDeal(const BondDeal& bond_deal, sdbus::string* exported_text = NULL, sdbus::string* bond_id = NULL);
    bool UpdateBondDeal(const BondDeal& bond_deal, sdbus::string* exported_text = NULL);

    bool UpdateBondDeal(const BondDealVecPtr& vec, sdbus::string* exported_text = NULL);
    bool DeleteBondDeal(const BondDealVecPtr& bond_deal_vec);

    sdbus::string CopyDeal(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDeal(sdbus::VariantList deal_ids);

private:
    static void UpdateBondDealCallBack(DcsBondDealCache* cache, void* param);
    static void UpdateBondDealSuccessed(DcsBondDealCache* cache, void* param);
    static void UpdateBondDealFailed(DcsBondDealCache* cache, void* param);

    static void DeleteBondDealCallBack(DcsBondDealCache* cache, void* param);
    static void DeleteBondDealSuccessed(DcsBondDealCache* cache, void* param);
    static void DeleteBondDealFailed(DcsBondDealCache* cache, void* param);

    sdbus::string CopyDealForTP(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForPATR(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForBGC(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForICAP(sdbus::VariantList deal_ids);

    sdbus::string CopyDetailedDealForTP(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForPATR(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForBGC(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForICAP(sdbus::VariantList deal_ids);

    void Init();

    bool AddDeal(const BondDeal& bond_deal,  sdbus::string* exported_text = NULL, sdbus::string* bond_id = NULL);
    int GetDealSequenceNumber();

    DcsBondDealService* service_;

};

#endif // _DCSBONDDEAL_BUSINESS_H_