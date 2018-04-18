#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_

#include "logger.h"
#include <map>

namespace sdbus {
    class Connection;
}

class ConnectionManager
{
public:
    static ConnectionManager* singleton();

    sdbus::Connection* getConnection(const std::string& name = "Main");

    void deleteConnection(const std::string& name = "Main");

    ~ConnectionManager();

private:
    ConnectionManager();

    sdbus::Connection* createConnection();

    std::map<std::string, sdbus::Connection*> conn_mp_;

    static ConnectionManager *singleton_;

};

#endif // _CONNECTIONMANAGER_H_