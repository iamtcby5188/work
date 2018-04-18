#include "account_monitor.h"
#include "cache/cache_center.h"

AccountMonitor::AccountMonitor()
    : Monitor()
{

}

AccountCachePtr AccountMonitor::get(const std::string &id) const
{
    std::map<std::string, AccountCachePtr>::const_iterator it = account_mp_.find(id);

    if (it != account_mp_.end()) {
        return (*it).second;
    }
    return AccountCachePtr();
}

int AccountMonitor::size() const
{
    return account_mp_.size();
}

std::map<std::string, AccountCachePtr>& AccountMonitor::refMap()
{
    return account_mp_;
}

void AccountMonitor::load(sql::ResultSet* resSet)
{
    AccountCachePtr cache(new AccountCache());

    std::string id = resSet->getString("id");
    if (id.length() == 0) {
        return;
    }

    cache->id = resSet->getString("id");
    cache->company_id = resSet->getString("company_id");
    cache->company_name = resSet->getString("company_name");
    cache->account_code = resSet->getString("account_code");
    cache->username = resSet->getString("username");
    cache->password = resSet->getString("password");
    cache->display_name = resSet->getString("display_name");
    cache->mobile = resSet->getString("mobile");
    cache->telephone = resSet->getString("telephone");
    cache->email = resSet->getString("email");
    cache->rm = resSet->getString("rm");
    cache->msn = resSet->getString("msn");
    cache->qq = resSet->getString("qq");
    cache->address = resSet->getString("address");
    cache->account_type = resSet->getString("account_type");
    cache->is_forbidden = resSet->getString("is_forbidden");
    cache->create_time = resSet->getString("create_time");
    cache->modify_time = resSet->getString("modify_time");
    cache->create_by = resSet->getString("create_by");
    cache->modify_by = resSet->getString("modify_by");
    cache->status = resSet->getString("status");
    cache->role_id = resSet->getString("role_id");

    CacheCenter::sharedInstance()->AttachCurrentThread();
    cache->loadCombinationFields();
    CacheCenter::sharedInstance()->DetachCurrentThread();

    account_mp_[id] = cache;
}