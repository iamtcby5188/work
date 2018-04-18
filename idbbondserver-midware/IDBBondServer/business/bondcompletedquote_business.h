#ifndef _BONDCOMPLETEDQUOTE_BUSINESS_H_
#define _BONDCOMPLETEDQUOTE_BUSINESS_H_

#include <vector>
#include <sstream>
#include "service/bondcompletedquote_service.h"
#include "object/quote.h"
#include "object/completedquote.h"
#include "db/dbconnector.h"
#include "include/common.h"
#include "include/fid.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/model/bondbestquote_cache.h"
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016

typedef std::tr1::shared_ptr<std::vector<int> > ExpTypeVecPtr;

class BondCompletedQuoteBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    BondCompletedQuoteBusiness();
    BondCompletedQuoteBusiness(BondCompletedQuoteService* service);

    CompletedQuoteDetailVecPtr GetCompletedQuoteDetailList(const std::string& cond);
    sdbus::string CopyCompletedQuote(sdbus::VariantList quote_ids, const sdbus::string& title, const std::string& companyId, bool isInternalQuote);
    sdbus::string CopyCompletedQuote(sdbus::VariantList quote_ids);
private:
    void FetchBidQuote(BondQuoteCache* cache, CompletedQuoteDetail* detail);
    void FetchOfrQuote(BondQuoteCache* cache, CompletedQuoteDetail* detail);
    ExpTypeVecPtr GetCompanyConfig(const std::string& companyId);
    sdbus::string GetFieldContent(BondBestQuoteCachePtr best_quote_cache, int fieldType, bool isInternalQuote);
    sdbus::string GetPriceAndNotes(BondBestQuoteCachePtr best_quote_cache, bool isInternalQuote);  

    sdbus::string GetTitle(const sdbus::string& bond_type);
    
    void Init();

	BondCompletedQuoteService* service_; //Add by Young 05-Apr-2016

};

#endif // _BONDCOMPLETEDQUOTE_BUSINESS_H_