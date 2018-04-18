/*
 * CacheCenter.cpp
 *
 *  Created on: Jun 3, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "cache_center.h"
#include "cache_monitor.h"
#include "../config.h"
#include "../include/logger.h"

#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>

#include "cache_controller_declear.h"
#include "controller/account_cachecontroller.h"
#include "controller/bondbestquote_cachecontroller.h"
#include "controller/bonddeal_cachecontroller.h"
#include "controller/bondgroup_cachecontroller.h"
#include "controller/bondgroupdetail_cachecontroller.h"
#include "controller/bondquote_cachecontroller.h"
#include "controller/bondquoterefer_cachecontroller.h"
#include "controller/bondsmallview_cachecontroller.h"
#include "controller/brokerbindingtrader_cachecontroller.h"
#include "controller/chinabondvaluation_cachecontroller.h"
#include "controller/logintoken_cachecontroller.h"

inline const char* stristr(const char* s, const char* p) { 
    while (*s != '\0') { 
        int i;
        for (i = 0; (s[i] & ~('a'-'A')) == (p[i] & ~('a' - 'A')) && p[i] != '\0'; ++i);
        if (p[i] == '\0') { 
            return s;
        }
        s += 1;
    }
    return NULL;
}

bool contains(dbUserFunctionArgument& arg1, dbUserFunctionArgument& arg2) { 
    assert(arg1.type == dbUserFunctionArgument::atString 
        && arg2.type == dbUserFunctionArgument::atString);

    return stristr(arg1.u.strValue, arg2.u.strValue) != NULL;
}

USER_FUNC(contains);

CacheCenter* CacheCenter::_singleton = NULL;

CacheCenter::CacheCenter() {
    // TODO Auto-generated constructor stub
}

void CacheCenter::AttachCurrentThread() {
    if (pDB) {
        if (pDB->isOpen() && !pDB->isAttached()) {
            pDB->attach();
        }
    } else if (_fstDb.isOpen())
        _fstDb.attach();
}

void CacheCenter::DetachCurrentThread() {
    if (pDB) {
        if (pDB->isOpen() && pDB->isAttached()) {
            pDB->detach();
        }
    } else if (_fstDb.isOpen() && _fstDb.isAttached())
        _fstDb.detach();
}

CacheCenter::~CacheCenter() {
    if (pDB) {
        if (pDB->isOpen()) {
            pDB->close();
        }
    } else if(_fstDb.isOpen()) {
        _fstDb.close();
        timer.close();
    }
}

bool CacheCenter::Init(std::string cache_name, int init_size /* = 256 */, int waitLockTimeoutMsec /* = 100 */, int nThreads /* = 1 */, int parallelScanThreshold /* = 1000 */) {
    dbDatabase::OpenParameters params;

    if (init_size == 0) {
        init_size = 256;
    }
    params.databaseName = cache_name.c_str();//"BCCache";
    params.accessType = dbDatabase::dbAllAccess;
    params.initSize = boost::lexical_cast<size_t>(init_size) * 1024 * 1024;
    params.waitLockTimeoutMsec = waitLockTimeoutMsec;
    params.nThreads = nThreads;
    params.parallelScanThreshold = parallelScanThreshold;

	timer.open(false);

    if (!_fstDb.open(params)) {
        return false;
    }

    return true;
}

void CacheCenter::CommitCache() {
    try {
        if (pDB) {
            if (pDB->isOpen())
                pDB->commit();
        } else if(_fstDb.isOpen()){
            _fstDb.commit();
        }
    }
    catch (dbException e) {
        LOGGER_INFO("Error in Commit - \n" << e.getMsg());
    }
}

void CacheCenter::RollbackCache() {
	try {
        if (pDB) {
            if (pDB->isOpen())
                pDB->rollback();
        } else if (_fstDb.isOpen()) {
            _fstDb.rollback();
        }
	}
	catch (dbException e) {
        LOGGER_ERROR("Error in Rollback - \n" << e.getMsg());
	}
}

static bool isInited = false;
static bool isOpened = false;
bool CacheCenter::Init() {
    LOGGER_DEBUG("data server param init ...");
    if (isInited)
        return true;
    local_host = Config::singleton()->getValue("FastDB.Host", "localhost");
    int _default_replicate_port = Config::singleton()->getIntValue("FastDB.ReplicatePort", 21010);

    std::string cache_mode = Config::singleton()->getValue("FastDB.WriteAccess", "0");
    params.accessType = dbDatabase::dbConcurrentRead;
    if (cache_mode == "1") {
        params.accessType = dbDatabase::dbAllAccess; // dbConcurrentUpdate
    }

    std::string servers = Config::singleton()->getValue("FastDB.ReplicatedServers", local_host + boost::lexical_cast<std::string>(_default_replicate_port));
    std::vector<char*> server_pvec;
    char *pStr = new char[256];
    int pos = 0;
    for (size_t i = 0; i < servers.size(); ++i) {
        if (servers[i] == ';') {
            pStr[pos] = '\0';
            pos = 0;
            server_pvec.push_back(pStr);
            pStr = new char[256];
            continue;
        }
        pStr[pos++] = servers[i];
    }
    pStr[pos] = '\0';
    server_pvec.push_back(pStr);
    params.nNodes = server_pvec.size();
    char **servers_c = new char*[params.nNodes];
    for (size_t m = 0; m != server_pvec.size(); ++m) {
        servers_c[m] = server_pvec[m];
        LOGGER_INFO("Rep server[" << m << "]: " << server_pvec[m]);
    }
    params.nodeAddresses = servers_c;

    // default: 1024*1024*1024, unit:M
    params.initSize = boost::lexical_cast<size_t>(Config::singleton()->getIntValue("FastDB.CacheSize", 1024)) * 1024 * 1024;

    // default:4*1024*1024, unit:B params.freeSpaceReuseThreshold;
    int iExtensionQuantum = Config::singleton()->getIntValue("FastDB.ExtensionQuantum", 4 * 1024 * 1024);
    params.extensionQuantum = boost::lexical_cast<size_t>(iExtensionQuantum)* 1024;

    // default 512*1024, unit: K
    params.initIndexSize = boost::lexical_cast<size_t>(Config::singleton()->getIntValue("FastDB.IndexSize", 512)) * 1024;
    params.nodeId = Config::singleton()->getIntValue("FastDB.NodeId", 0);
    params.nThreads = Config::singleton()->getIntValue("FastDB.WorkThreads", dbThread::numberOfProcessors()); // default:numberOfProcessors
    params.maxAsyncRecoveryIterations = Config::singleton()->getIntValue("FastDB.MaxAsyncRecoveryIterations", dbMaxAsyncRecoveryIterations);
    params.parallelScanThreshold = Config::singleton()->getIntValue("FastDB.ParallelScanThreshold", dbDefaultParallelScanThreshold);
    params.pollInterval = (Config::singleton()->getIntValue("FastDB.PollInterval", dbDefaultPollInterval / 1000)) * 1000; // ms
    params.recoveryConnectionAttempts = Config::singleton()->getIntValue("FastDB.RecoveryConnectionAttempts", dbRecoveryConnectionAttempts);
    params.replicationWriteTimeout = (Config::singleton()->getIntValue("FastDB.ReplicationWriteTimeout", dbReplicationWriteTimeout / 1000)) * 1000; // ms
    params.startupConnectionAttempts = Config::singleton()->getIntValue("FastDB.StartupConnectionAttempts", dbStartupConnectionAttempts);
    params.transactionCommitDelay = Config::singleton()->getIntValue("FastDB.TransactionCommitDelay", 0);
    params.waitLockTimeoutMsec = Config::singleton()->getIntValue("FastDB.WaitLockTimeoutMsec", INFINITE);
    params.waitReadyTimeout = (Config::singleton()->getIntValue("FastDB.WaitReadyTimeout", dbWaitReadyTimeout / 1000)) * 1000; // ms
    params.waitStatusTimeout = (Config::singleton()->getIntValue("FastDB.WaitStatusTimeout", dbWaitStatusTimeout / 1000)) * 1000; // ms

    cache_name = Config::singleton()->getValue("FastDB.CacheName", "DataCache");
    cache_file = cache_name + ".fdb";
    params.databaseFilePath = cache_file.c_str();
    params.databaseName = cache_name.c_str();

	_log_file = Config::singleton()->getValue("FastDB.LogFile", "FastDB.log");

    LOGGER_INFO("FastDB"
        << "CacheName[" << params.databaseName << "] "
        << "CacheFile[" << params.databaseFilePath << "] "
        << "CacheSize[" << params.initSize / 1024 / 1024 << "M(" << params.initSize << ")] "
        << "ExtensionQuantum[" << params.extensionQuantum << "B] "
        << "IndexSize[" << params.initIndexSize / 1024 << "K] "
        << "WorkThreads[" << params.nThreads << "] "
        << "WriteAccess[" << cache_mode << "] "
        << "ReplicatedServers[" << servers << "] "
        << "TotalNodes[" << params.nNodes << "] "
        << "NodeId[" << params.nodeId << "] ");
    if (params.nThreads < 0 || params.initSize < 1L || params.extensionQuantum < 1L || params.initIndexSize < 1L) {
        LOGGER_ERROR("config error, please check!!!");
        return false;
    }

    if (params.accessType == dbDatabase::dbConcurrentRead) {
        pDB = new dbDatabase(params.accessType, params.initSize, params.extensionQuantum, params.initIndexSize, params.nThreads);
    } else {
        pDB = new dbReplicatedDatabase(params.accessType, params.initSize, params.extensionQuantum, params.initIndexSize, params.nThreads);
    }

    isInited = true;
    LOGGER_DEBUG("data server param init success");
    return isInited;
}

void CacheCenter::Start() {
    boost::function0<void> f = boost::bind(&CacheCenter::openFastDB, this);
    boost::thread t(f);
}

void CacheCenter::Open() {
    Start();
    boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::seconds(10));
    dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(getFastDB());
    int i = 0;
    int iStatus = rDB && rDB->isOpen() ? rDB->getNodeStatus(rDB->getCurrentNodeId()) : -1;
    while (i < 10 && !(iStatus == 2 || iStatus == 3)) {
        boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::seconds(10));
        iStatus = rDB && rDB->isOpen() ? rDB->getNodeStatus(rDB->getCurrentNodeId()) : -1;
        i++;
    }
    CacheMonitor::getInstance()->setDataBase(getFastDB());
}

void CacheCenter::Close() {
    LOGGER_INFO("close fastdb database ...");
    CacheMonitor::getInstance()->setDataBase(NULL);
    if (pDB) {
        dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(pDB);
        if (rDB && rDB->isOpen()) {
            rDB->close();
            delete rDB;
        } else if (pDB->isOpen()){
            pDB->close();
            delete pDB;
        }
        pDB = NULL;
        isOpened = false;
        isInited = false;
    }
}

dbDatabase* CacheCenter::getFastDB() {
    if (pDB)
        return pDB;
    else
        return &_fstDb;
}

bool CacheCenter::openFastDB() {
    LOGGER_DEBUG("open fastdb ...");
    if (!isInited)
        Init();
    if (isOpened)
        return true;
    if (!pDB || pDB->isOpen())
        return false;
    if (params.accessType == dbDatabase::dbConcurrentRead) {
        if (!pDB->open(params.databaseName)) {
            LOGGER_ERROR("open fastdb failed!!!");
            return isOpened;
        }
    } else {
        dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(pDB);
        rDB->enableCascadeDelete(true);
        rDB->enableInverseReferencesUpdate(true);
        if (!rDB->open(params)) {
            LOGGER_ERROR("open fastdb failed!!!");
            return isOpened;
        }
    }
    LOGGER_INFO("database start with [dbDatabase::" << cvtType(params.accessType) << "], threads: " << params.nThreads);
    LOGGER_DEBUG("open fastdb success");
    isOpened = true;
    CacheMonitor::getInstance()->setDataBase(getFastDB());
    LOGGER_INFO(CacheMonitor::getInstance()->getFastDBInfo());
    return isOpened;
}

std::string CacheCenter::cvtType(dbDatabase::dbAccessType accessType) {
    return accessType == dbDatabase::dbAllAccess ? "dbAllAccess" :
        (accessType == dbDatabase::dbConcurrentUpdate ? "dbConcurrentUpdate" : "dbConcurrentRead");
}

std::string CacheCenter::getLogFile()
{
	return _log_file;
}

void CacheCenter::clearCache() {
    LOGGER_DEBUG("clear all tables data...");
    /*SwSectorInfoCacheController ssicc;
    ssicc.clearCache();
    IssuingInstitutionCacheController iicc;
    iicc.clearCache();
    BondTraderCacheController btcc;
    btcc.clearCache();
    BrokerBindingTraderCacheController bbtcc;
    bbtcc.clearCache();
    ChinaBondDeviatedValueCacheController cbdvcc;
    cbdvcc.clearCache();
    ProvinceInfoCacheController picc;
    picc.clearCache();
    TradingInstitutionCacheController ticc;
    ticc.clearCache();
    BondSmallViewCacheController bsvcc;
    bsvcc.clearCache();
    AccountCacheController acc;
    acc.clearCache();
    VersionCacheController vcc;
    vcc.clearCache();
    SelectiveInfoCacheController sicc;
    sicc.clearCache();
    AccountGroupCacheController agcc;
    agcc.clearCache();
    AccountPermissionCacheController apcc;
    apcc.clearCache();
    ConstantsCacheController ccc;
    ccc.clearCache();
    IssuerInfoCacheController icc;
    icc.clearCache();
    BondTypeSelectiveCacheController btscc;
    btscc.clearCache();
    HolidayInfoCacheController hicc;
    hicc.clearCache();
    SystemCompanyCacheController sccc;
    sccc.clearCache();
    InstitutionRatingCacheController ircc;
    ircc.clearCache();
    BondQuoteReferCacheController bqrcc;
    bqrcc.clearCache();
    BondQuoteCacheController bqcc;
    bqcc.clearCache();
    BondDealCacheController bdcc;
    bdcc.clearCache();
    ChinaBondValuationCacheController cbvcc;
    cbvcc.clearCache();*/
    Close();
    LOGGER_INFO("reopen fastdb database ...");
    Open();
    LOGGER_INFO("open fastdb databse done. " << CacheMonitor::getInstance()->getFastDBInfo());
}

bool CacheCenter::isInitData(int argc, char * argv[]) {
    bool ret = Config::singleton()->getBoolValue("FastDB.InitData", false);
    if (ret)
        return true; 
    if (argc > 1) {
        std::string param = argv[1];
        std::transform(param.begin(), param.end(), param.begin(), ::toupper);
        if ("1" == param || "TRUE" == param)
            return true;
    }
    SystemCompanyCacheController sccc;
    dbQuery q("");
    int i = sccc.getSizeByQueryInThreadSafty(q);
    if (i < 1) {
        return true;
    }
    return false;
}
