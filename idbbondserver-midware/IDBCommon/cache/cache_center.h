/*
 * CacheCenter.h
 *
 *  Created on: Jun 3, 2014
 *      Author: theochen
 */

#ifndef CACHECENTER_H_
#define CACHECENTER_H_

#include "cache_def.h"
#include "db/dbconnector.h"
#include "fastdb/inc/fastdb.h"

class CacheCenter {

private:
    CacheCenter();
    CacheCenter(CacheCenter const& cacheCenter); // never implement
    virtual ~CacheCenter();

    void operator=(CacheCenter const& cacheCenter); // never implement

public:
    static CacheCenter* sharedInstance()
    {
        if (_singleton == NULL)
        {
            _singleton = new CacheCenter();

        }
        return _singleton;
    }

    bool Init(std::string cache_name, int init_size = 256, int waitLockTimeoutMsec = 100, int nThreads = 1, int parallelScanThreshold = 1000);

    bool Init();
    void Start();
    void Open();
    void Close();
    dbDatabase* getFastDB();

    void CommitCache();
	void RollbackCache();
    void AttachCurrentThread();
    void DetachCurrentThread();

	std::string getLogFile();

    void clearCache();

    bool isInitData(int argc, char * argv[]);

private:
    static CacheCenter* _singleton;
    bool openFastDB();
    std::string cvtType(dbDatabase::dbAccessType accessType);

    dbDatabase _fstDb;
	dbLocalEvent timer;
    dbDatabase *pDB;
    dbDatabase::OpenParameters params;
    std::string local_host, cache_name, cache_file;
	std::string _log_file;

};

#endif /* CACHECENTER_H_ */
