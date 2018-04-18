/*
 * hotkey_cachecontroller.h
 *
 *  Created on: August 27, 2014
 *      Author: yudan.chen
 */

#ifndef HOTKEY_CACHECONTROLLER_H_
#define HOTKEY_CACHECONTROLLER_H_

#include "cache/simple_cache_controller.h"
#include "cache/model/hotkey_cache.h"

class HotKeyCacheController : public SimpleCacheController<HotKeyCache, HotKeyCachePtr, HotKeyCacheVecPtr> {
public:
    static HotKeyCacheController* singleton();

    virtual ~HotKeyCacheController();

protected:
    HotKeyCacheController();

    virtual void loadCache(std::map<std::string, HotKeyCachePtr> &data_mp, sql::ResultSet* resSet);

    virtual int updateDiskDB(HotKeyCachePtr cache_new, HotKeyCachePtr cache_old);
    virtual int insertDiskDB(HotKeyCachePtr cache);
    virtual int deleteDiskDB(const std::vector<std::string> &id_list);

    static HotKeyCacheController* singleton_;
};

#endif /* HOTKEY_CACHECONTROLLER_H_ */
