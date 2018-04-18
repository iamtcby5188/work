/*
* account_permission_cache.h
*
*  Created on: Dec 24, 2014
*      Author: huameng
*/

#ifndef ACCOUNT_PERMISSION_CACHE_H_
#define ACCOUNT_PERMISSION_CACHE_H_

class AccountPermissionCache;
typedef std::tr1::shared_ptr<AccountPermissionCache> AccountPermissionCachePtr;
typedef std::tr1::shared_ptr<std::vector<AccountPermissionCachePtr> > AccountPermissionCacheVecPtr;

class AccountPermissionCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	AccountPermissionCache();
	virtual ~AccountPermissionCache();
	std::string getPrimaryKey();

	std::string account_id;
	std::string account_group_id;
	std::string permission_id;
	std::string permission_name;
	std::string permission_family;

	//Non-Dabase Fields

public:
	TYPE_DESCRIPTOR(
		(KEY(account_id, HASHED/* | INDEXED*/),
		FIELD(account_group_id), 
		KEY(permission_id, HASHED /*| INDEXED*/),
		FIELD(permission_name),
		FIELD(permission_family)
		)
		);
};

#endif /* ACCOUNT_GROUP_CACHE_H_ */
