#ifndef _CACHE_MANAGER_H_
#define _CACHE_MANAGER_H_

class CacheManager
{
public:
    static CacheManager* singleton();

    void InitCache();
    void first_InitCache();
    void InitCompanyCache();
	bool InitDcsDealCache();
	
private:
    void InitChinaBondValuationCache();
    void InitBondTypeSelectiveCache();
    void InitBondQuoteCache();
    void InitBondQuoteReferCache();
    void InitBondDealCache();
    void InitBondBestQuoteCache();
    void InitLoginTokenCache();

private:
    CacheManager() { success_count = state = 0; }

    void initMemCache();
    void clearMemCache();

    static CacheManager* singleton_;
    void initVersionTable();
    volatile int success_count;
    volatile int state;
};

#endif // _CACHE_MANAGER_H_