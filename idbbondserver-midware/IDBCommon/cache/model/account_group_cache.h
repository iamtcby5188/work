/*
* account_group_cache.h
*
*  Created on: July 08, 2014
*      Author: theochen
*/

#ifndef ACCOUNT_GROUP_CACHE_H_
#define ACCOUNT_GROUP_CACHE_H_

class AccountGroupCache;
typedef std::tr1::shared_ptr<AccountGroupCache> AccountGroupCachePtr;
typedef std::tr1::shared_ptr<std::vector<AccountGroupCachePtr> > AccountGroupCacheVecPtr;

class AccountGroupCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
	AccountGroupCache();
	virtual ~AccountGroupCache();
	std::string getPrimaryKey();

	std::string account_id;
	std::string account_group_id;

	// Non table-related fields
	std::string group_name;
	std::string account_group_description;
	std::string account_group_level;
	std::string department_id;
	std::string department_name;
	std::string department_code;
	std::string department_description;
	std::string department_is_public;

	//Non-Dabase Fields

public:
	TYPE_DESCRIPTOR(
		(KEY(account_id, HASHED/* | INDEXED*/),
		KEY(account_group_id, HASHED/* | INDEXED*/), 
		FIELD(group_name),
		FIELD(account_group_description),
		FIELD(account_group_level),
		KEY(department_id, HASHED/* | INDEXED*/),
		FIELD(department_name),
		FIELD(department_code),
		FIELD(department_description),
		FIELD(department_is_public)
		)
		);
};

#endif /* ACCOUNT_GROUP_CACHE_H_ */
