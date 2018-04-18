#ifndef _CHINABONDVALUATION_SYNC_H_
#define _CHINABONDVALUATION_SYNC_H_

#include "logger.h"
#include "cache/model/bondquote_cache.h"
#include "cache/model/bondquoterefer_cache.h"
#include "cache/model/bondbestquote_cache.h"
#include "cache/model/bonddeal_cache.h"
#include "cache/model/chinabondvaluation_cache.h"
#include <unordered_map>
class ChinaBondValuationSync
{    
public:
    ChinaBondValuationSync();
    ~ChinaBondValuationSync();

    void sync();

private:
	static void updateBondCache(const std::unordered_map<std::string, ChinaBondValuationCache*> &cache,const int& updateType);

	static void UpdateBondQuoteCallBack(BondQuoteCache* cache, void* param);
	static void UpdateBondQuoteReferCallBack(BondQuoteReferCache* cache, void* param);
	static void UpdateBondBestQuoteCallBack(BondBestQuoteCache* cache, void* param);
	static void UpdateBondDealCallBack(BondDealCache* cache, void* param);

	static void ClearBondQuoteCallBack(BondQuoteCache* cache, void* param);
	static void ClearBondQuoteReferCallBack(BondQuoteReferCache* cache, void* param);
	static void ClearBondBestQuoteCallBack(BondBestQuoteCache* cache, void* param);
	static void ClearBondDealCallBack(BondDealCache* cache, void* param);

};

#endif // _CHINABONDVALUATION_SYNC_H_