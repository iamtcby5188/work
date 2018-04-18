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
#include "cache/model/dcsbonddeal_cache.h"
#include "object/quote.h"
#include "object/yijibanbond.h"
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016
#include <boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;

class BondQuoteCache;
class BondQuoteReferCache;

class BondQuoteBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    BondQuoteBusiness(BondQuoteService* service);
    typedef std::tr1::shared_ptr<std::vector<BondQuotePtr> > BondQuoteVecPtr;
    typedef std::tr1::shared_ptr<std::vector<YiJiBanBondPtr> > YiJiBanBondVecPtr;

    BondQuoteCacheVecPtr FetchBondQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    BondQuoteReferCacheVecPtr FetchBondReferQuotes(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    BondQuoteCacheVecPtr FetchBondQuotesByDate(const std::string& cond, const std::string& bondtype, const std::string& datetime);

    BondQuoteCacheVecPtr GetBondQuoteByID(const std::string& bond_quote_id);
    BondQuoteReferCacheVecPtr GetBondReferByID(const std::string& bond_quote_id);
    BondQuoteCacheVecPtr GetBondQuoteByConditions(const int& side, const std::string& internal_flag, const std::string& bond_key_listed_market, const std::string& institution_id, const std::string& trader_id);

    std::vector<std::string> GetBondQuoteIdList(const sdbus::string& cond, const sdbus::string& sortby);

    int GetBondQuotesCount(const sdbus::string& cond);
    int GetBondReferQuotesCount(const sdbus::string& cond);

	void AssembleBondQuote(BondQuoteCachePtr& bondquotecache, ptree* bond_quote_msg);
	void AssembleReferQuote(BondQuoteReferCachePtr& bondquotecache, ptree* bond_quote_msg);
    bool AddQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL, bool manually_set_deal_id = false);
    bool UpdateQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL);
	bool UpdateReferQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL);
    bool UpdateQuoteInfo(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL);
    bool DeleteReferredQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, ptree* refer_quote_msg = NULL);
    bool DeleteQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL);
    bool EditReferredQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL);
    bool ReferBondQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL);
    bool UnreferBondQuote(const BondQuoteVecPtr& vec, sdbus::string* exported_text = NULL, bool export_is_internal = false, ptree* bond_quote_msg = NULL);

	bool ReferBondQuoteAll(std::string bond_category = "all");

    bool GvnTknQuote(sdbus::VariantList quote_ids, sdbus::string* exported_text = NULL, sdbus::VariantList *deal_id_list = NULL, ptree* gvntkn_msg = NULL);

    sdbus::string CopyQuote(sdbus::VariantList quote_ids);
    sdbus::string CopyReferredQuote(sdbus::VariantList quote_ids);

    bool UpdateYiJiBanRate(const YiJiBanBondPtr &yijiban_bond_ptr); //add by lsq to update 意向债
    YiJiBanBondPtr FetchYiJiBanBond(const sdbus::string &bond_key); //query 意向债信息

    static void SetBondQuoteFromCache(const BondQuoteCache* cache, BondQuote* bondquote);
    static void SetBondReferQuoteFromCache(const BondQuoteReferCache* cache, BondQuote* bondquote);
private:
    static void FetchBondQuoteId(BondQuoteCache* cache, void* param);

    static void DeleteReferredQuoteCallBack(BondQuoteReferCache* cache, void* param);
    static void DeleteReferredQuoteSuccessed(BondQuoteReferCache* cache, void* param);
    static void DeleteReferredQuoteFailed(BondQuoteReferCache* cache, void* param);

    static void DeleteQuoteCallBack(BondQuoteCache* cache, void* param);
    static void DeleteQuoteSuccessed(BondQuoteCache* cache, void* param);
    static void DeleteQuoteFailed(BondQuoteCache* cache, void* param);

    static void UpdateBondQuoteCallBack(BondQuoteCache* cache, void* param);
    static void UpdateBondQuoteSuccessed(BondQuoteCache* cache, void* param);
    static void UpdateBondQuoteFailed(BondQuoteCache* cache, void* param);
	static void UpdateBondReferQuoteCallBack(BondQuoteReferCache* cache, void* param);
	static void UpdateBondReferQuoteSuccessed(BondQuoteReferCache* cache, void* param);
	static void UpdateBondReferQuoteFailed(BondQuoteReferCache* cache, void* param);

    static void ReferBondQuoteCallBack(BondQuoteCache* cache, void* param);
    static void ReferBondQuoteSuccessed(BondQuoteCache* cache, void* param);
    static void ReferBondQuoteFailed(BondQuoteCache* cache, void* param);

    static void UnReferBondQuoteCallBack(BondQuoteReferCache* cache, void* param);
    static void UnReferBondQuoteCallBackBatch(std::vector<BondQuoteReferCachePtr> &cache_vec, void* param);
    static void UnReferBondQuoteSuccessed(BondQuoteReferCache* cache, void* param);
    static void UnReferBondQuoteFailed(BondQuoteReferCache* cache, void* param);
    

    static void EditReferredQuoteCallBack(BondQuoteReferCache* cache, void* param);
    static void EditReferredQuoteSuccessed(BondQuoteReferCache* cache, void* param);
    static void EditReferredQuoteFailed(BondQuoteReferCache* cache, void* param);

    sdbus::string CopyQuoteForTP(sdbus::VariantList quote_ids);
    sdbus::string CopyQuoteForICAP(sdbus::VariantList quote_ids);
    sdbus::string CopyQuoteForBGC(sdbus::VariantList quote_ids);
    sdbus::string CopyQuoteForPATR(sdbus::VariantList quote_ids);

    sdbus::string CopyReferredQuoteForTP(sdbus::VariantList quote_ids);
    sdbus::string CopyReferredQuoteForPATR(sdbus::VariantList quote_ids);
    
    void Init();
	void SyncBondQuoteToQB(std::vector<BondQuoteCachePtr>& bond_quote_vec, const std::string& kServiceQBSync, const int method);
	void SyncBondBestQuoteToQB(const std::vector<std::string>& bond_key_listed_market_vec);

    BondQuoteService* service_;

};

#endif // _BONDQUOTE_BUSINESS_H_