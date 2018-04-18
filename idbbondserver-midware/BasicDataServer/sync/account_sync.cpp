#include "account_sync.h"
#include "sync/sync.h"
#include "constants.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "monitor/account_monitor.h"
#include "service/service_manager.h"
#include "service/bondservice.h"

AccountSync::AccountSync()
{

}

AccountSync::~AccountSync()
{

}

void AccountSync::sync(bool publish /* true */)
{
    AccountCacheController account_cc;
    AccountMonitor monitor;

    monitor.init("FetchAccountSQL", AccountCache::SCHEMA_NAME);

    BondService *bond_service = (BondService*)ServiceManager::singleton()->getService(kServiceBondInfo);
    
    dbQuery q;
    q = "";
    AccountCacheVecPtr vec = account_cc.getCacheListByQueryInThreadSafty(q);

    std::vector<AccountCache*> update_vec;
    std::vector<AccountCache*> add_vec;
    std::vector<AccountCache*> delete_vec;

    db_sync<AccountCacheController, AccountMonitor, AccountCache, AccountCachePtr, AccountCacheVecPtr>(
        account_cc, monitor, "id", vec, update_vec, add_vec, delete_vec);
    
    LOGGER_INFO("update: " << update_vec.size() << " add: " << add_vec.size() << " delete: " << delete_vec.size())

    if (publish) {
        if (update_vec.size() > 0) {
            bond_service->PublishAccountInfo(update_vec, "UpdateList");        
        }

        if (add_vec.size() > 0) {
            bond_service->PublishAccountInfo(add_vec, "AddList");              
        }

        if (delete_vec.size() > 0) {
            bond_service->PublishAccountInfo(delete_vec, "DeleteList");         
        }
    
        /*std::vector<std::string> updated_account_vec;
        if (update_vec.size() > 0) {
            std::vector<AccountCache*>::const_iterator it = update_vec.begin();
            for ( ; it != update_vec.end(); ++it) {
                updated_account_vec.push_back((*it)->id);
            }
        }

        if (updated_account_vec.size() > 0) {
            BondQuoteSync bondquote_sync;
            bondquote_sync.syncForAccountChanged(updated_account_vec);

            BondReferQuoteSync bondreferquote_sync;
            bondreferquote_sync.syncForAccountChanged(updated_account_vec);

            BondDealSync bonddeal_sync;
            bonddeal_sync.syncForAccountChanged(updated_account_vec);
        }*/
    }
}

void AccountSync::syncDepartment()
{
    // 重新加载用户部门缓存
    AccountGroupCacheController accountGroupCC;
    accountGroupCC.cacheTable();
    accountGroupCC.commit();

    // 重新加载用户权限缓存
    AccountPermissionCacheController accountPermissionCC;
    accountPermissionCC.cacheTable();
    accountPermissionCC.commit();

    sync();
}

void AccountSync::syncPermission()
{
    // 重新加载用户权限缓存
    AccountPermissionCacheController accountPermissionCC;
    accountPermissionCC.cacheTable();
    accountPermissionCC.commit();

    sync();
}