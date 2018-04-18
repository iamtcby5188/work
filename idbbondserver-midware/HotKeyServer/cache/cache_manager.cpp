#include "cache_manager.h"
#include "cache_def.h"
#include "cache/controller/hotkey_cachecontroller.h"

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

    HotKeyCacheController::singleton()->init();

    std::cout << "------------------ Caching Done, Go ahead :] -------------" << std::endl;

    TIME_COST_END("Caching ...")
}
