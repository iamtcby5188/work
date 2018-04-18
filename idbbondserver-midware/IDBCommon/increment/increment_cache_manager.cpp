#include "increment_cache_manager.h"


IncrementCacheManager* IncrementCacheManager::singleton_ = NULL;

IncrementCacheManager* IncrementCacheManager::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new IncrementCacheManager();
    }

    return singleton_;
}

IncrementCacheManager::IncrementCacheManager()
{

}

IncrementCacheManager::~IncrementCacheManager()
{
    Release();
}

void IncrementCacheManager::Release()
{    
    std::map<std::string, IncrementCache*>::iterator it = increment_cache_mp_.begin();
    for ( ; it != increment_cache_mp_.end(); ++it) {
		if ((*it).second)
			delete (*it).second;
    }
}

IncrementCache* IncrementCacheManager::GetIncrementCache(const std::string &model_name)
{
    std::map<std::string, IncrementCache*>::const_iterator it = increment_cache_mp_.find(model_name);
    if (it == increment_cache_mp_.end()) {
        IncrementCache* cache = new IncrementCache(model_name, 50);
        increment_cache_mp_[model_name] = cache;
        return cache;
    } else {
        return (*it).second;
    }
}