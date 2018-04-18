#ifndef _DBCONNECTOR_H_
#define _DBCONNECTOR_H_

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>

class ConnPool;

typedef struct {
	std::vector<std::string> sql_vec;
    std::string sql_str;
    std::string schema;
} UpdateMsg;

class DBConnector
{
public:
    static DBConnector* getInstance();

    sql::ResultSet* executeQuery(std::string sql_str, std::string schema);

    int executeUpdate(std::string sql_str, std::string schema);
    void executeUpdate(const UpdateMsg& update_msg);
	void executeUpdate(const std::vector<std::string>& sql_vec, const std::string& schma);

    // 事务处理
    sql::Connection* beginTransaction(std::string schema);
    int executeUpdate(sql::Connection* conn, std::string sql_str);
    void commit(sql::Connection* conn);
    void rollback(sql::Connection* conn);
    
    // 更新线程
    void threadExeUpdate();
private:
    DBConnector();
    ~DBConnector();

	bool executeBatchUpdate(sql::Connection* conn, const std::vector<std::string>& sql_vec, const std::string& schema);

    std::queue<UpdateMsg> msg_queue_;
    boost::mutex queue_mutex_;
    boost::condition_variable_any queue_cond_var_;

    ConnPool *conn_pool_;

    static DBConnector* singleton_;

};

#endif