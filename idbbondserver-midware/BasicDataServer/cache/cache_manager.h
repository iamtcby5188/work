#ifndef _CACHE_MANAGER_H_
#define _CACHE_MANAGER_H_

class CacheManager
{
public:
    static CacheManager* singleton();

	void InitCache(bool is_init = false);

    void InitSimpleCache();

private:
    CacheManager() {}

    static CacheManager* singleton_;
    void initVersionTable();
	void memCacheClear();
};

#endif // _CACHE_MANAGER_H_