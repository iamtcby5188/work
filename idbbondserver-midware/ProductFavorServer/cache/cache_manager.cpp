#include "cache_manager.h"
#include "cache_def.h"
#include "cache/controller/bondgroup_cachecontroller.h"
#include "cache/controller/bondgroupdetail_cachecontroller.h"

CacheManager* CacheManager::singleton_ = NULL;

CacheManager* CacheManager::singleton() {
    if (singleton_ == NULL) {
        singleton_ = new CacheManager();
    }

    return singleton_;
}

void CacheManager::InitCache()
{
    TIME_COST_BEGIN("Caching ...")

    BondGroupCacheController::singleton()->init();

    BondGroupDetailCacheController::singleton()->init();

    LOGGER_ERROR("------------------ Caching Done, Go ahead :] -------------");

    TIME_COST_END("Caching ...")
}
