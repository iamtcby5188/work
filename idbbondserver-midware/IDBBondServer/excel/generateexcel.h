#ifndef _GENERATE_EXCEL_H_
#define _GENERATE_EXCEL_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif
#include "object/bond.h"
#include "object/quote.h"
#include "object/bonddeal.h"
#include "cache/controller/bondquote_cachecontroller.h"
#include "cache/model/bonddeal_cache.h"
#include "cache/model/dcsbonddeal_cache.h"

class GenerateExcel
{
public:
    GenerateExcel(std::string fileName);
    ~GenerateExcel();
public:
    bool SaveBondDataBase(const BondQuoteCacheVecPtr& cache_vec);
    bool SaveBondDataICAP(const BondQuoteCacheVecPtr& cache_vec);

    bool SaveBondDealDataPATR(const BondDealCacheVecPtr& proxyModel);
    bool SaveBondDealDataBGC(const BondDealCacheVecPtr& proxyModel);
    bool SaveBondDealDataICAP(const BondDealCacheVecPtr& proxyModel);
    bool SaveBondDealDataTPBCO(const BondDealCacheVecPtr& proxyModel);
    bool SaveBondDealDataTPBNC(const BondDealCacheVecPtr& proxyModel);

	bool SaveBondDealDataPATR(const DcsBondDealCacheVecPtr& proxyModel);
	bool SaveBondDealDataBGC(const DcsBondDealCacheVecPtr& proxyModel);
	bool SaveBondDealDataICAP(const DcsBondDealCacheVecPtr& proxyModel);
	bool SaveBondDealDataTPBCO(const DcsBondDealCacheVecPtr& proxyModel);
	bool SaveBondDealDataTPBNC(const DcsBondDealCacheVecPtr& proxyModel);

private:
    std::string GetDateString();
    std::string GetStringFromDouble(double data);
    std::string GetPriceWithR(double _price, int _type, sdbus::string rebate, double returnPoint);
private:
    std::string m_fileName;
};
#endif // _GENERATE_EXCEL_H_