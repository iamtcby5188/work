/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Bond Quote Business
====================================================================================================================
   Date       Name                                  Description of Change
20-Jun-2014   Huameng Jia                           Initial version
19-Aug-2014   James Xu                              Add CopyQuote
21-Aug-2014   James Xu                              Add CopyReferredQuote
$HISTORY$
===================================================================================================================*/
#ifndef _BONDQUOTE_BUSINESS_H_
#define _BONDQUOTE_BUSINESS_H_

#include <vector>
#include <map>
#include "service/bondquote_service.h"
#include "object/quote.h"
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016

class BondQuoteCache;
class BondQuoteReferCache;

class BondQuoteBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    BondQuoteBusiness(BondQuoteService* service);
    typedef std::tr1::shared_ptr<std::vector<BondQuotePtr> > BondQuoteVecPtr;

    BondQuoteCacheVecPtr FetchBondQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    BondQuoteReferCacheVecPtr FetchBondReferQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    BondQuoteCacheVecPtr FetchBondQuotesByDate(const std::string& cond, const std::string& bondtype, const std::string& datetime);

    BondQuoteCacheVecPtr GetBondQuoteByID(const std::string& bond_quote_id);
    BondQuoteReferCacheVecPtr GetBondReferByID(const std::string& bond_quote_id);
    BondQuoteCacheVecPtr GetBondQuoteByConditions(const int& side, const std::string& internal_flag, const std::string& bond_key_listed_market, const std::string& institution_id, const std::string& trader_id);

    std::vector<std::string> GetBondQuoteIdList(const sdbus::string& cond, const sdbus::string& sortby);

    int GetBondQuotesCount(const sdbus::string& cond);
    int GetBondReferQuotesCount(const sdbus::string& cond);

    sdbus::string CopyQuote(sdbus::VariantList quote_ids);
    sdbus::string CopyReferredQuote(sdbus::VariantList quote_ids);

    static void SetBondQuoteFromCache(const BondQuoteCache* cache, BondQuote* bondquote);
    static void SetBondReferQuoteFromCache(const BondQuoteReferCache* cache, BondQuote* bondquote);

private:
    static void FetchBondQuoteId(BondQuoteCache* cache, void* param);

    sdbus::string CopyQuoteForTP(sdbus::VariantList quote_ids);

    sdbus::string CopyReferredQuoteForTP(sdbus::VariantList quote_ids);
    
    void Init();

    BondQuoteService* service_;

};

#endif // _BONDQUOTE_BUSINESS_H_