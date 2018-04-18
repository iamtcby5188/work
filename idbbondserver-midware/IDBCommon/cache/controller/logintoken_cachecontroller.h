/*
 * logintoken_cachecontroller.h
 *
 *  Created on: Jun 24, 2014
 *      Author: yudan.chen
 */

#ifndef LOGINTOKEN_CACHECONTROLLER_H_
#define LOGINTOKEN_CACHECONTROLLER_H_

#include "cache/simple_cache_controller.h"
#include "cache/model/logintoken_cache.h"

class LoginTokenCacheController : public SimpleCacheController<LoginTokenCache, LoginTokenCachePtr, LoginTokenCacheVecPtr> {
public:
	static LoginTokenCacheController* singleton();

    virtual ~LoginTokenCacheController();
	bool deleteCacheWhenOffLine();
protected:
	LoginTokenCacheController();
	virtual void loadCache(std::map<std::string, LoginTokenCachePtr> &data_mp, sql::ResultSet* resSet);

	virtual int updateDiskDB(LoginTokenCachePtr cache_new, LoginTokenCachePtr cache_old);
	virtual int insertDiskDB(LoginTokenCachePtr cache);
	virtual int deleteDiskDB(const std::vector<std::string> &id_list);

	static LoginTokenCacheController* singleton_;
};

#endif /* LOGINTOKEN_CACHECONTROLLER_H_ */
