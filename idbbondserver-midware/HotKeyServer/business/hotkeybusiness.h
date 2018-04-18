#ifndef _HOTKEYBUSINESS_H_
#define _HOTKEYBUSINESS_H_

#include <vector>
#include "service/hotkeyservice.h"
#include "cache/model/hotkey_cache.h"
#include "logger.h"

class HotKeyBusiness
{
public:
    HotKeyBusiness(HotKeyService* service);
    HotKeyCacheVecPtr GetHotKeyList();

private:
    static void FetchHotKey(HotKeyCache* cache, void* param);
    HotKeyService* service_;

};

#endif // _HOTKEYBUSINESS_H_