#include "dbconnector.h"
#include "connection_pool.h"
#include <boost/locale/encoding.hpp>	
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>

DBConnector* DBConnector::singleton_ = 0;

DBConnector* DBConnector::getInstance()
{
    if (singleton_ == NULL) {
        singleton_ = new DBConnector();
    }

    return singleton_;
}

DBConnector::DBConnector()
{
    conn_pool_ = ConnPool::getInstance();

    boost::function0<void> f = boost::bind(&DBConnector::threadExeUpdate, this);  
    boost::thread thrd(f); 
}

DBConnector::~DBConnector()
{

}

sql::ResultSet* DBConnector::executeQuery(std::string sql_str, std::string schema)
{
    sql::Connection *conn = conn_pool_->getConnection(schema);
	sql::Statement *stmt = NULL;
    sql::ResultSet *resSet = NULL;

    if (conn == NULL) {
        return NULL;
    }

	try {
		stmt = conn->createStatement();
		resSet = stmt->executeQuery(sql_str);
    } catch(sql::SQLException &e) {
        LOGGER_ERROR("failed to execute query! schema:" << schema << ", sql:" << sql_str);
        LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
        LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
    } catch(std::exception &e) {
        LOGGER_ERROR(e.what());
    } catch (...) {
        LOGGER_ERROR("unkonwn error!!! ");
    }

	if (stmt != NULL)
	{
		delete stmt;
    }

    conn_pool_->releaseConnection(conn);
	return resSet;
}

int DBConnector::executeUpdate(std::string sql_str, std::string schema)
{		
    UpdateMsg update_msg;
    update_msg.sql_str = sql_str;
    update_msg.schema = schema;

    boost::mutex::scoped_lock lock(queue_mutex_);
    msg_queue_.push(update_msg);
    queue_cond_var_.notify_one();

    return 1;
}

void DBConnector::executeUpdate(const std::vector<std::string>& sql_vec, const std::string& schma){
	if(schma.empty() || sql_vec.empty()){
		return;
	}
	UpdateMsg update_msg;
	update_msg.schema = schma;
	if(sql_vec.size() == 1){
		update_msg.sql_str = sql_vec.at(0);
	}else{
		update_msg.sql_vec = sql_vec;
	}	

	boost::mutex::scoped_lock lock(queue_mutex_);
	msg_queue_.push(update_msg);
	queue_cond_var_.notify_one();
}

void DBConnector::executeUpdate(const UpdateMsg& update_msg)
{		
    std::string sql_str = update_msg.sql_str;
    std::string schema = update_msg.schema;

    sql::Connection *conn = conn_pool_->getConnection(schema);	  
    if (conn == NULL) {
        return;
    }

	if(!sql_str.empty()){
		sql::Statement *stmt = NULL;
		sql::ResultSet *resSet = NULL;
		int effectedLines = -1;

		if(sql_str.size() > 5000)
			LOGGER_INFO("Too Long sql string, ignore for LOGGER_INFO");

		//if(sql_str.size() <= 5000)
		//	LOGGER_INFO(sql_str);

		try {
			stmt = conn->createStatement(); 
			effectedLines = stmt->executeUpdate(sql_str);
		} catch(sql::SQLException &e) {
			LOGGER_ERROR("failed to execute update! schema:" << schema << ", sql:" << sql_str);
			LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
			LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
		} catch(std::exception &e) {
            LOGGER_ERROR(e.what());
        } catch (...) {
            LOGGER_ERROR("unkonwn error!!! ");
        }

		if (stmt != NULL){
			delete stmt;
		}
	}
    
	if(!update_msg.sql_vec.empty()){
		conn->commit();
		executeBatchUpdate(conn, update_msg.sql_vec, update_msg.schema);
	}
    conn_pool_->releaseConnection(conn);
    return;
}

bool DBConnector::executeBatchUpdate(sql::Connection* conn, const std::vector<std::string>& sql_vec, const std::string& schema){
	if(!conn || !conn->isValid() || sql_vec.empty()){
		return true;
	}
	sql::Statement *stmt = NULL;
	bool result = false;
	try{
		LOGGER_DEBUG("execute sql, schema[" << schema << "], sql count[" << sql_vec.size() << "] start ...");
		conn->setAutoCommit(false);		
		stmt = conn->createStatement();
		int ret = -1;
		//int iCnt = 0, _DB_COMMIT_COUNT = 1000;;
		for(size_t i = 0; i < sql_vec.size(); ++i){
			const std::string& sql = sql_vec.at(i);
			ret = stmt->executeUpdate(sql);
			//iCnt ++;
			//if(iCnt >= _DB_COMMIT_COUNT){
			//	conn->commit();
			//	iCnt = 0;
			//}
			LOGGER_TRACE("execute sql[" << sql << "] success, schema[" << schema << "] effective lines[" << ret << "]");
		}
		conn->commit();
		stmt->close();
		result = true;
		LOGGER_DEBUG("execute sql, schema[" << schema << "], sql count[" << sql_vec.size() << "] success");
	}catch(sql::SQLException &e){
		if(conn != NULL){
			conn->rollback();
		}
		LOGGER_ERROR("failed to execute update! schema:" << schema << ", sql_vec count:" << sql_vec.size());
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
		result = false;
	} catch(std::exception &e) {
		if(conn != NULL){
			conn->rollback();
		}
		LOGGER_ERROR(e.what());
		result = false;
    } catch (...) {
        LOGGER_ERROR("unkonwn error!!! ");
    }
	if(stmt != NULL){
		delete stmt;
	}
	conn->setAutoCommit(true);
	return result;
}

sql::Connection* DBConnector::beginTransaction(std::string schema)
{
    sql::Connection *conn = conn_pool_->getConnection(schema, false);	  

    return conn;
}

int DBConnector::executeUpdate(sql::Connection* conn, std::string sql_str)
{
    sql::Statement *stmt = NULL;
    sql::ResultSet *resSet = NULL;
    int effectedLines = -1;
 
    if (conn == NULL) {
        return -1;
    }

    //LOGGER_INFO(sql_str);
    try {
        stmt = conn->createStatement(); 
        effectedLines = stmt->executeUpdate(sql_str);
    } catch(sql::SQLException &e) {
        LOGGER_ERROR("failed to execute update! " << sql_str);
        LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
        LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
    } catch(std::exception &e) {
        LOGGER_ERROR(e.what());
    } catch (...) {
        LOGGER_ERROR("unkonwn error!!! ");
    }

    if (stmt != NULL)
    {
        delete stmt;
    }

    return effectedLines;
}

void DBConnector::commit(sql::Connection* conn)
{
    if (conn == NULL) {
        return;
    }

    conn->commit();
    conn_pool_->releaseConnection(conn);
}

void DBConnector::rollback(sql::Connection* conn)
{
    if (conn == NULL) {
        return;
    }

    conn->rollback();
    conn_pool_->releaseConnection(conn);
}

void DBConnector::threadExeUpdate()
{    
    LOGGER_DEBUG("start thread to update db")

    while(true) {
        UpdateMsg update_msg;

        {
            boost::mutex::scoped_lock lock(queue_mutex_);

            while(msg_queue_.empty()) {        
                queue_cond_var_.wait(lock);
            }

            update_msg = msg_queue_.front();
            msg_queue_.pop();
        }
        
        executeUpdate(update_msg);

    }
}