#ifndef _INCREMENT_CACHE_MANAGER_H_
#define _INCREMENT_CACHE_MANAGER_H_

#include "increment_cache.h"
#include "boost/thread/mutex.hpp"

class IncrementCacheManager
{
public:
    ~IncrementCacheManager();

    static IncrementCacheManager* singleton();

    void Release();

    IncrementCache* GetIncrementCache(const std::string &model_name);

private:
    IncrementCacheManager();
  
    std::map<std::string, IncrementCache*> increment_cache_mp_;

    static IncrementCacheManager *singleton_;
};


#endif // _INCREMENT_CACHE_MANAGER_H_