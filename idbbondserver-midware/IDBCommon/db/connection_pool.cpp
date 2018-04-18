#include "connection_pool.h"
#include "include/constants.h"
#include "../config.h"

ConnPool* ConnPool::singleton_ = NULL;

ConnPool::ConnPool(std::string url, std::string username, std::string passwd, int size)
    : driver_(NULL), url_(url), username_(username), passwd_(passwd)
{
    try {
        driver_ = sql::mysql::get_mysql_driver_instance();
    } catch(sql::SQLException &e) {
        LOGGER_ERROR("failed to get mysql driver instance!");
        LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
        LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
    } catch(std::exception &e) {
        LOGGER_ERROR(e.what());
    } catch (...) {
        LOGGER_ERROR("unkonwn error!!! ");
    }

    initConnPool(size);
}

ConnPool::~ConnPool()
{
    destConnPool();
}

ConnPool* ConnPool::getInstance()
{
    if (singleton_ == NULL) {
        std::string host = Config::singleton()->getValue("Connection.Host", "tcp://192.168.1.102:3306");
        std::string user = Config::singleton()->getValue("Connection.User", "artogrid");
        std::string passwd = Config::singleton()->getValue("Connection.Password", "artogrid");
        int pool_size = atoi(Config::singleton()->getValue("Connection.PoolSize", "2").c_str());
        LOGGER_DEBUG("host: " << host << " user: " << user << " passwd: " << passwd << " poolsize: " << pool_size)
        singleton_ = new ConnPool(host, user, passwd, pool_size);
    }

    return singleton_;
}

void ConnPool::init() {
    LOGGER_DEBUG("db conn pool init " << conn_list_.size());
}

// 初始化连接池
void ConnPool::initConnPool(int size)
{
    boost::mutex::scoped_lock lock(mutex_);

    for (int i = 0; i < size; ++i) {
        sql::Connection *conn = createConnection();
        if (conn) {
            conn_list_.push_back(conn);
        }
    }
}

// 销毁连接池, 首先要先销毁连接池的中连接
void ConnPool::destConnPool()
{
    boost::mutex::scoped_lock lock(mutex_);

    std::list<sql::Connection*>::iterator it = conn_list_.begin();

    for ( ; it != conn_list_.end(); ++it) {
        destConnection(*it);
    }

    conn_list_.clear();
}

// 创建一个连接
sql::Connection* ConnPool::createConnection()
{
    sql::Connection *conn = NULL;
    try {
        sql::ConnectOptionsMap connection_properties;
        connection_properties["hostName"] = std::string(url_);
        connection_properties["userName"] = std::string(username_);
        connection_properties["password"] = std::string(passwd_);
        connection_properties["OPT_RECONNECT"] = true;

        conn = driver_->connect(connection_properties);
        //conn = driver_->connect(url_, username_, passwd_);
    } catch(sql::SQLException &e) {
        LOGGER_ERROR("failed to create sql connection!");
        LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
        LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
    } catch(std::exception &e) {
        LOGGER_ERROR(e.what());
    } catch (...) {
        LOGGER_ERROR("unkonwn error!!! ");
    }

    return conn;
}

// 销毁一个连接
void ConnPool::destConnection(sql::Connection* conn)
{
    if (conn) {
        try {
            conn->close();
        } catch(sql::SQLException &e) {
            LOGGER_ERROR("failed to close sql connection!");
            LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
            LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
        } catch(std::exception &e) {
            LOGGER_ERROR(e.what());
        } catch (...) {
            LOGGER_ERROR("unkonwn error!!! ");
        }

        delete conn;
    }
}

// 在连接池中获取一个连接
sql::Connection* ConnPool::getConnection(std::string schema, bool autocommit)
{
    boost::mutex::scoped_lock lock(mutex_);

    sql::Connection *conn = NULL;
    if (conn_list_.size() > 0) {    // 连接池容器中还有连接  
        conn = conn_list_.front();  // 得到第一个连接
        conn_list_.pop_front();     // 移除第一个连接 
//        LOGGER_DEBUG("Remaining connection count of current connection pool: " << conn_list_.size())

        if (conn->isClosed()) {     // 连接被关闭, 删除后重新建立一个
            delete conn;
            conn = createConnection();
        }
    } else {
        // 连接池不够, 创建新的连接
        conn = createConnection();
    }
    if (conn) {
        try {
            conn->setSchema(schema);
            conn->setAutoCommit(autocommit);
        } catch(sql::SQLException &e) {
            LOGGER_ERROR("failed to set sql connection!");
            LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
            LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
        } catch(std::exception &e) {
            LOGGER_ERROR(e.what());
        } catch (...) {
            LOGGER_ERROR("unkonwn error!!! ");
        }
    }

    return conn;
}

// 回收数据库连接
void ConnPool::releaseConnection(sql::Connection *conn)
{
    boost::mutex::scoped_lock lock(mutex_);

    if (conn) {
		if(!conn->getAutoCommit()){
			conn->setAutoCommit(true);
		}
        conn_list_.push_back(conn);
    }
}