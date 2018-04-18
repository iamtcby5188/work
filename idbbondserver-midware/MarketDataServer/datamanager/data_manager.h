#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "logger.h"
#if !defined(_MSC_VER)
#include <tr1/memory>
#endif

#include "cache/model/system_company_cache.h"

class Service;
namespace sdbus {
    class Connection;
    class Callback;
}    

class QuoteDataManager;
class DealDataManager;
class DcsDealDataManager;
class ReferQuoteDataManager;
class BestQuoteDataManager;
class ValuationDataManager;

class DataManager
{
public:
    static DataManager* singleton();

    bool init(sdbus::Connection *conn);
    bool reloadData();

    ~DataManager();
private:
    void loadQuoteData(const std::vector<SystemCompanyCachePtr>::const_iterator &it);
    void loadDealData(const std::vector<SystemCompanyCachePtr>::const_iterator &it);
	void loadDcsDealData(const std::vector<SystemCompanyCachePtr>::const_iterator &it);
    void loadReferData(const std::vector<SystemCompanyCachePtr>::const_iterator &it);
    void loadBestData(const std::vector<SystemCompanyCachePtr>::const_iterator &it);

private:
    DataManager();
    sdbus::Connection *getConnection();

    QuoteDataManager* quote_data_manager_;
    DealDataManager* deal_data_manager_;
	DcsDealDataManager* dcs_deal_data_manager_;
    ReferQuoteDataManager* referquote_data_manager_;
    BestQuoteDataManager* bestquote_data_manager_;
    ValuationDataManager* valuation_data_manager_;

    std::map<std::string, QuoteDataManager*> quoteDataManagerPtrMap;
    std::map<std::string, DealDataManager*> dealDataManagerPtrMap;
	std::map<std::string, DcsDealDataManager*> dcsDealDataManagerPtrMap;
    std::map<std::string, ReferQuoteDataManager*> referQuoteDataManagerPtrMap;
    std::map<std::string, BestQuoteDataManager*> bestQuoteDataManagerPtrMap;

    static DataManager *singleton_;

    sdbus::Connection *conn_;

    volatile int success_count;
    volatile int state;
};

#endif // _DATA_MANAGER_H_