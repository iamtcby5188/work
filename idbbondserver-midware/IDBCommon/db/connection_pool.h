#ifndef _CONNECTION_POOL_H_
#define _CONNECTION_POOL_H_

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <boost/thread/mutex.hpp>
#include <list>
#include <string>
#include "logger.h"

class ConnPool
{
public:
    ~ConnPool();

    static ConnPool* getInstance();

    void init();

    void initConnPool(int init_size);
    void destConnPool();

    sql::Connection* createConnection();
    void destConnection(sql::Connection* conn);

    sql::Connection* getConnection(std::string schema, bool autocommit = true);
    void releaseConnection(sql::Connection *conn);

private:
    ConnPool(std::string url, std::string username, std::string passwd, int size);

    sql::mysql::MySQL_Driver* driver_;

    std::list<sql::Connection*> conn_list_;

    std::string url_;
    std::string username_;
    std::string passwd_;

    boost::mutex mutex_;

    static ConnPool* singleton_;

};


#endif // _CONNECTION_POOL_H_