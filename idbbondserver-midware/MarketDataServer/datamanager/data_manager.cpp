#include "data_manager.h"
#include "quote_data_manager.h"
#include "deal_data_manager.h"
#include "dcs_deal_data_manager.h"
#include "bestquote_data_manager.h"
#include "referquote_data_manager.h"
#include "valuation_data_manager.h"
#include "cache/model/system_company_cache.h"
#include "include/common.h"
#include "sdbus/callback.h"
#include "business/SystemCompanyBusiness.h"
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>

DataManager* DataManager::singleton_ = NULL;

DataManager::DataManager()
{
    quote_data_manager_ = NULL;
    deal_data_manager_ = NULL;
	dcs_deal_data_manager_ = NULL;
    referquote_data_manager_ = NULL;
    bestquote_data_manager_ = NULL;
    conn_ = NULL;

    success_count = 0;
    state = 0;
}

DataManager::~DataManager()
{

}

DataManager* DataManager::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new DataManager();
    }

    return singleton_;
}

bool DataManager::init(sdbus::Connection *conn)
{
    conn_ = conn;
    SystemCompanyBusiness sb;
    SystemCompanyCacheVecPtr ccv = sb.getSysCompany();
	if(ccv && ccv->size() < 1){
		LOGGER_ERROR("no valid company!!!");
		return false;
	}

    std::vector<SystemCompanyCachePtr>::const_iterator it = ccv->begin();
    for(;it != ccv->end(); ++it)
    {
        if(quoteDataManagerPtrMap.find((*it)->id) == quoteDataManagerPtrMap.end())
        {
            LOGGER_INFO("Not find copmany!");
            quote_data_manager_ = new QuoteDataManager(conn, (*it)->id);
            if (quote_data_manager_ && quote_data_manager_->init()) {
                quoteDataManagerPtrMap[(*it)->id] = quote_data_manager_;
            }
			else
			{
				LOGGER_ERROR("init quote failed!");
				return false;
			}

            deal_data_manager_ = new DealDataManager(conn, (*it)->id);
            if (deal_data_manager_ && deal_data_manager_->init()) {
                dealDataManagerPtrMap[(*it)->id] = deal_data_manager_;
            }
			else
			{
				LOGGER_ERROR("init deal failed!");
				return false;
			}

			dcs_deal_data_manager_ = new DcsDealDataManager(conn, (*it)->id);
			if (dcs_deal_data_manager_ && dcs_deal_data_manager_->init()) {
				dcsDealDataManagerPtrMap[(*it)->id] = dcs_deal_data_manager_;
			}
			else
			{
				LOGGER_ERROR("init dcs deal failed!");
				return false;
			}

            referquote_data_manager_ = new ReferQuoteDataManager(conn, (*it)->id);
            if (referquote_data_manager_ && referquote_data_manager_->init()) {
                referQuoteDataManagerPtrMap[(*it)->id] = referquote_data_manager_;
            }
			else
			{
				LOGGER_ERROR("init refer quote failed!");
				return false;
			}

            bestquote_data_manager_ = new BestQuoteDataManager(conn, (*it)->id);
            if (bestquote_data_manager_ && bestquote_data_manager_->init()) {
                bestQuoteDataManagerPtrMap[(*it)->id] = bestquote_data_manager_;
            }
			else
			{
				LOGGER_ERROR("init best quote failed!");
				return false;
			}
        } 
    }

    return true;
}

bool DataManager::reloadData()
{
	TIME_COST_BEGIN("reload data ...");

    SystemCompanyBusiness sb;
    SystemCompanyCacheVecPtr ccv = sb.getSysCompany();
    if (ccv && ccv->size() < 1){
        LOGGER_ERROR("no valid company!!!");
        return false;
    }

    std::vector<SystemCompanyCachePtr>::const_iterator it = ccv->begin();
	for (; it != ccv->end(); ++it)
	{
		LOGGER_INFO("Find copmany:[" + (*it)->id + "]");

		boost::function0<void> f1 = boost::bind(&DataManager::loadQuoteData, this, it);
		boost::thread thrd1(f1);

		boost::function0<void> f2 = boost::bind(&DataManager::loadDealData, this, it);
		boost::thread thrd2(f2);

		boost::function0<void> f3 = boost::bind(&DataManager::loadDcsDealData, this, it);
		boost::thread thrd3(f3);

		boost::function0<void> f4 = boost::bind(&DataManager::loadReferData, this, it);
		boost::thread thrd4(f4);

		boost::function0<void> f5 = boost::bind(&DataManager::loadBestData, this, it);
		boost::thread thrd5(f5);

		thrd1.join();
		thrd2.join();
		thrd3.join();
		thrd4.join();
		thrd5.join();
	}

	TIME_COST_END("reload data ...");

    return true;
}

void DataManager::loadQuoteData(const std::vector<SystemCompanyCachePtr>::const_iterator &it)
{
	quote_data_manager_ = quoteDataManagerPtrMap[(*it)->id];
	if (quote_data_manager_) {
		if (!quote_data_manager_->FetchQuoteRemote()) {
			LOGGER_ERROR("load quote data failed");
		}
	}
}

void DataManager::loadDealData(const std::vector<SystemCompanyCachePtr>::const_iterator &it)
{
	deal_data_manager_ = dealDataManagerPtrMap[(*it)->id];
	if (deal_data_manager_) {
		if (!deal_data_manager_->FetchDealRemote()) {
			LOGGER_ERROR("load deal data failed");
		}
	}
}

void DataManager::loadDcsDealData(const std::vector<SystemCompanyCachePtr>::const_iterator &it)
{
	dcs_deal_data_manager_ = dcsDealDataManagerPtrMap[(*it)->id];
	if (dcs_deal_data_manager_) {
		if (!dcs_deal_data_manager_->FetchDcsDealRemote()) {
			LOGGER_ERROR("load dcs deal data failed");
		}
	}
}

void DataManager::loadReferData(const std::vector<SystemCompanyCachePtr>::const_iterator &it)
{
	referquote_data_manager_ = referQuoteDataManagerPtrMap[(*it)->id];
	if (referquote_data_manager_) {
		if (!referquote_data_manager_->FetchReferQuoteRemote()) {
			LOGGER_ERROR("load refer quote data failed");
		}
	}
}

void DataManager::loadBestData(const std::vector<SystemCompanyCachePtr>::const_iterator &it)
{
	bestquote_data_manager_ = bestQuoteDataManagerPtrMap[(*it)->id];
	if (bestquote_data_manager_) {
		if (!bestquote_data_manager_->FetchBestQuoteRemote()) {
			LOGGER_ERROR("load best quote data failed");
		}
	}
}