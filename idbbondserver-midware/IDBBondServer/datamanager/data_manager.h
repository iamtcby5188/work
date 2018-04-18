#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "logger.h"
#if !defined(_MSC_VER)
#include <tr1/memory>
#endif

class Service;
namespace sdbus {
    class Connection;
    class Callback;
}    

class BondDataManager;
class InstitutionDataManager;
class TraderDataManager;
class AccountDataManager;
class DeviatedValueDataManager;
class BrokerBindingDataManager;

class DataManager
{
public:
    static DataManager* singleton();

    bool init(sdbus::Connection *conn);
    bool reloadData();

    ~DataManager();
private:
    DataManager();

    BondDataManager* bond_data_manager_;
    InstitutionDataManager* institution_data_manager_;
    TraderDataManager* trader_data_manager_;
    AccountDataManager* account_data_manager_;
    DeviatedValueDataManager* deviated_value_data_manager_;
    BrokerBindingDataManager* broker_binding_data_manager_;

    std::map<std::string, InstitutionDataManager*> institutionDataManagerPtrMap;
    std::map<std::string, TraderDataManager*> traderDataManagerPtrMap;
    std::map<std::string, AccountDataManager*> accountDataManagerPtrMap;
    std::map<std::string, DeviatedValueDataManager*> deviatedValueDataManagerPtrMap;
    std::map<std::string, BrokerBindingDataManager*> brokerBindingDataManagerPtrMap;

    static DataManager *singleton_;

    sdbus::Connection *conn_;

};

#endif // _DATA_MANAGER_H_