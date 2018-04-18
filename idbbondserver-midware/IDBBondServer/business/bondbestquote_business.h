/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Best Quote Business
====================================================================================================================
   Date       Name                                  Description of Change
20-Jun-2014   Huameng Jia                           Initial version
19-Aug-2014   James Xu                              Add CopyBestQuote and CopyCompletedQuote
19-Aug-2014   James Xu                              Add CopyInternalBestQuote
30-Sep-2014   James Xu                              Add CopyInternalBestQuoteForICAP
$HISTORY$
===================================================================================================================*/
#ifndef _BONDBESTQUOTE_BUSINESS_H_
#define _BONDBESTQUOTE_BUSINESS_H_

#include <vector>
#include <map>
#include <unordered_set>
#include "service/bondbestquote_service.h"
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016

class BondBestQuoteCache;

class BondBestQuoteBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    BondBestQuoteBusiness(BondBestQuoteService* service);

    BondBestQuoteCacheVecPtr FetchBondBestQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    BondBestQuoteCacheVecPtr FetchBondBestQuoteById(const std::string& bond_key_listed_market);

    int GetBondBestQuotesCount(const sdbus::string& cond);

    std::vector<std::string> GetBondBestQuoteIdList(const sdbus::string& cond, const sdbus::string& sortby);

    bool UpdateBestQuote(std::vector<std::string>& bond_key_listed_market_vec, bool update_across_market_bond = false);

    sdbus::string CopyBestQuote(const std::vector<std::string>& bond_key_listed_market_vec);
    sdbus::string CopyInternalBestQuote(const std::vector<std::string>& bond_key_listed_market_vec);

    sdbus::string CopyBestQuote(sdbus::VariantList quote_ids);
    sdbus::string CopyBestQuoteAll(sdbus::VariantList quote_ids_all);
    sdbus::string CopyInternalBestQuote(sdbus::VariantList quote_ids);
    
    bool InsertUpdateIntoDiskDB(const std::vector<BondBestQuoteCachePtr> &, bool referAllBondByCompany = false);

private:
    static void FetchBondBestQuoteId(BondBestQuoteCache* cache, void* param);

    sdbus::string CopyBestQuoteForTP(sdbus::VariantList quote_ids);
    sdbus::string CopyBestQuoteForICAP(sdbus::VariantList quote_ids);
    sdbus::string CopyBestQuoteForBGC(sdbus::VariantList quote_ids);
    sdbus::string CopyBestQuoteForPATR(sdbus::VariantList quote_ids);

    
    sdbus::string CopyInternalBestQuoteForTP(sdbus::VariantList quote_ids);
    sdbus::string CopyInternalBestQuoteForICAP(sdbus::VariantList quote_ids);
    sdbus::string CopyInternalBestQuoteForPATR(sdbus::VariantList quote_ids);
    sdbus::string CopyInternalBestQuoteForBGC(sdbus::VariantList quote_ids);

    void Init();

    BondBestQuoteService* service_;

};


#endif