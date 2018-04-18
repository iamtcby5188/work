#include "hotkeybusiness.h"
#include "cache/controller/hotkey_cachecontroller.h"

HotKeyBusiness::HotKeyBusiness(HotKeyService* service)
    : service_(service)
{
}

HotKeyCacheVecPtr HotKeyBusiness::GetHotKeyList()
{
    return HotKeyCacheController::singleton()->getVec();
}
