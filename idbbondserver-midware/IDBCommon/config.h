#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>
#include "logger.h"

class Config
{
public:
    static Config* singleton();

    void config(const std::string &config_file);

    // default load
    bool load();

    // special load
    bool load(const std::string &topic);

    std::string getValue(const std::string &name);    
    std::string getValue(const std::string &name, const std::string &default_value);
    int getIntValue(const std::string& name, const int& default_value);
    bool getBoolValue(const std::string& name, const bool& default_value);

protected:

    bool loadConfigFile(std::string configFileName);
    void loadConfigFilePath();

private:
    Config();

    std::string config_file_;
    std::string config_file_common_;

    std::map<std::string, std::string> config_mp_;

    static Config* singleton_;

};

#endif // _BONDCONFIG_H_