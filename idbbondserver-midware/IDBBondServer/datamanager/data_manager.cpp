#include "data_manager.h"
#include "bond_data_manager.h"
#include "institution_data_manager.h"
#include "trader_data_manager.h"
#include "account_data_manager.h"
#include "deviated_value_data_manager.h"
#include "broker_binding_data_manager.h"
#include "cache/model/system_company_cache.h"
#include "include/common.h"
#include "business/SystemCompanyBusiness.h"

DataManager* DataManager::singleton_ = NULL;

DataManager::DataManager()
{
    bond_data_manager_ = NULL;
    institution_data_manager_ = NULL;
    trader_data_manager_ = NULL;
    account_data_manager_ = NULL;
    deviated_value_data_manager_ = NULL;
    broker_binding_data_manager_ = NULL;
    conn_ = NULL;
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
	if (ccv && ccv->size() < 1) {
		LOGGER_ERROR("no valid company!!!");
		return false;
	}

	std::vector<SystemCompanyCachePtr>::const_iterator it = ccv->begin();
	for (; it != ccv->end(); ++it)
	{
		if (institutionDataManagerPtrMap.find((*it)->id) == institutionDataManagerPtrMap.end())
		{
			LOGGER_INFO("Not find copmany:[" + (*it)->id + "]");
			institution_data_manager_ = new InstitutionDataManager(conn, (*it)->id);
			if (institution_data_manager_ && institution_data_manager_->init()) {
				institutionDataManagerPtrMap[(*it)->id] = institution_data_manager_;
			}
			else
			{
				LOGGER_ERROR("init institution failed!");
				return false;
			}

			trader_data_manager_ = new TraderDataManager(conn, (*it)->id);
			if (trader_data_manager_ && trader_data_manager_->init()) {
				traderDataManagerPtrMap[(*it)->id] = trader_data_manager_;
			}
			else
			{
				LOGGER_ERROR("init trader failed!");
				return false;
			}

			account_data_manager_ = new AccountDataManager(conn, (*it)->id);
			if (account_data_manager_ && account_data_manager_->init()) {
				accountDataManagerPtrMap[(*it)->id] = account_data_manager_;
			}
			else
			{
				LOGGER_ERROR("init account failed!");
				return false;
			}

			deviated_value_data_manager_ = new DeviatedValueDataManager(conn, (*it)->id);
			if (deviated_value_data_manager_ && deviated_value_data_manager_->init()) {
				deviatedValueDataManagerPtrMap[(*it)->id] = deviated_value_data_manager_;
			}
			else
			{
				LOGGER_ERROR("init deviated value failed!");
				return false;
			}

			broker_binding_data_manager_ = new BrokerBindingDataManager(conn, (*it)->id);
			if (broker_binding_data_manager_ && broker_binding_data_manager_->init()) {
				brokerBindingDataManagerPtrMap[(*it)->id] = broker_binding_data_manager_;
			}
			else
			{
				LOGGER_ERROR("init broker binding failed!");
				return false;
			}
		}
	}

	bond_data_manager_ = new BondDataManager(conn);
	if ((NULL == bond_data_manager_) || !bond_data_manager_->init()) {
		LOGGER_ERROR("init bond failed!");
		return false;
	}

	return true;
}


bool DataManager::reloadData()
{
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
		institution_data_manager_ = institutionDataManagerPtrMap[(*it)->id];
		if (institution_data_manager_) {
			if (!institution_data_manager_->FetchInstitutionRemote()) {
				LOGGER_ERROR("reload institution data failed!");
			}
		}

		trader_data_manager_ = traderDataManagerPtrMap[(*it)->id];
		if (trader_data_manager_) {
			if (!trader_data_manager_->FetchTraderRemote()) {
				LOGGER_ERROR("reload trader data failed!");
			}
		}

		account_data_manager_ = accountDataManagerPtrMap[(*it)->id];
		if (account_data_manager_) {
			if (!account_data_manager_->FetchAccountRemote()) {
				LOGGER_ERROR("reload account data failed!");
			}
		}

		deviated_value_data_manager_ = deviatedValueDataManagerPtrMap[(*it)->id];
		if (deviated_value_data_manager_) {
			if (!deviated_value_data_manager_->FetchDeviatedValueRemote()) {
				LOGGER_ERROR("reload deviated value data failed!");
			}
		}

		broker_binding_data_manager_ = brokerBindingDataManagerPtrMap[(*it)->id];
		if (broker_binding_data_manager_) {
			if (!broker_binding_data_manager_->FetchBrokerBindingRemote()) {
				LOGGER_ERROR("reload broker binding data failed!");
			}
		}
	}

	if (bond_data_manager_) {
		if (!bond_data_manager_->FetchBondRemote()) {
			LOGGER_ERROR("reload bond data failed!");
		}
	}

    return true;
}
