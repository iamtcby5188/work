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

// ��ʼ�����ӳ�
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

// �������ӳ�, ����Ҫ���������ӳص�������
void ConnPool::destConnPool()
{
    boost::mutex::scoped_lock lock(mutex_);

    std::list<sql::Connection*>::iterator it = conn_list_.begin();

    for ( ; it != conn_list_.end(); ++it) {
        destConnection(*it);
    }

    conn_list_.clear();
}

// ����һ������
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

// ����һ������
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

// �����ӳ��л�ȡһ������
sql::Connection* ConnPool::getConnection(std::string schema, bool autocommit)
{
    boost::mutex::scoped_lock lock(mutex_);

    sql::Connection *conn = NULL;
    if (conn_list_.size() > 0) {    // ���ӳ������л�������  
        conn = conn_list_.front();  // �õ���һ������
        conn_list_.pop_front();     // �Ƴ���һ������ 
//        LOGGER_DEBUG("Remaining connection count of current connection pool: " << conn_list_.size())

        if (conn->isClosed()) {     // ���ӱ��ر�, ɾ�������½���һ��
            delete conn;
            conn = createConnection();
        }
    } else {
        // ���ӳز���, �����µ�����
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

// �������ݿ�����
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