#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <string>
#include "logger.h"

namespace sql {
    class ResultSet;
}

class Monitor
{
public:
    Monitor();
    bool init(const std::string &name, const std::string &schema_name);

protected:
    virtual void load(sql::ResultSet* resSet) = 0;
    std::string fetchSQL(const std::string &name); 

};

#endif // _MONITOR_H_