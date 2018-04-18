#ifndef _CACHE_MANAGER_H_
#define _CACHE_MANAGER_H_

class CacheManager
{
public:
    static CacheManager* singleton();

    void InitCache();

private:
    CacheManager() {}

    static CacheManager* singleton_;

};

#endif // _CACHE_MANAGER_H_