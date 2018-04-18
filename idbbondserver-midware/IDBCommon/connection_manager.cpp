#include "connection_manager.h"
#include "sdbus/connection.h"
#include "include/constants.h"
#include "config.h"

ConnectionManager* ConnectionManager::singleton_ = NULL;

ConnectionManager* ConnectionManager::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new ConnectionManager();
    }

    return singleton_;
}

ConnectionManager::ConnectionManager()
{
    
}

sdbus::Connection* ConnectionManager::createConnection()
{
    sdbus::Connection *conn = new sdbus::Connection();

    if (conn) {
        std::string qpid_url = Config::singleton()->getValue("SDBus.Main.Url", "127.0.0.1");
		LOGGER_INFO("qpid url: " << qpid_url);
		int iHeartBeat = Config::singleton()->getIntValue("SDBus.Main.HeartBeat", 5);
		sdbus::Options options;
		if (iHeartBeat >= 0) {
			bool ret = options.SetOption("heartbeat", iHeartBeat);
			LOGGER_INFO("qpid heartbeat:" << iHeartBeat << ", result:" << (ret ? "true" : "false"));
		}		
		bool isReconnect = Config::singleton()->getBoolValue("SDBus.Main.Reconnect", false);
		if (isReconnect)
			options.SetOption("reconnect", isReconnect);
		conn->SetOptions(options);
        if (!conn->Connect(qpid_url)) {
            delete conn;
            conn = NULL;
			return conn;
        }
		sdbus::Options tOptions = conn->GetOptions();
		isReconnect = false;
		int tHeartBeat = -1;
		bool ret1 = tOptions.GetOption("reconnect", isReconnect);
		bool ret2 = tOptions.GetOption("heartbeat", tHeartBeat);
		std::ostringstream oss;
		oss << "qpid url:" << qpid_url;
		if (ret1)
			oss << ",reconnect:" << isReconnect;
		if (ret2)
			oss << ",heartbeat:" << tHeartBeat;
		LOGGER_INFO(oss.str());
    }

    return conn;
}

ConnectionManager::~ConnectionManager()
{
    std::map<std::string, sdbus::Connection*>::iterator it = conn_mp_.begin();

    for ( ; it != conn_mp_.end(); ++it) {
        sdbus::Connection *conn = (*it).second;

        if (conn) {
            conn->Close();
            delete conn;
        }
    }

    conn_mp_.clear();
}

sdbus::Connection* ConnectionManager::getConnection(const std::string& name /* = "Main"*/)
{
    if (conn_mp_.find(name) == conn_mp_.end()) {
        conn_mp_[name] = createConnection();
    }

    return conn_mp_[name];
}

void ConnectionManager::deleteConnection(const std::string& name /* = "Main"*/)
{
    if (conn_mp_.find(name) != conn_mp_.end()) {
        delete conn_mp_[name];
        conn_mp_.erase(name);
    }
}
