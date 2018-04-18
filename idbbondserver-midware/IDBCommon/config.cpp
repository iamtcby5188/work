#include "config.h"

#include "util/pugixml/pugixml.hpp"
#include "common.h"

Config* Config::singleton_ = NULL;

Config::Config()
{
    config_file_ = "config.xml";
    config_file_common_ = "";
}

Config* Config::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new Config();
    }

    return singleton_;
}

void Config::config(const std::string &config_file)
{
    config_file_ = config_file;
}

bool Config::load()
{
    loadConfigFilePath();
    if(config_file_common_ != "")
        loadConfigFile(config_file_common_);

    loadConfigFile(config_file_);

    if(config_mp_.find("Connection.Host") == config_mp_.end()) {
        LOGGER_INFO("Connection.Host not found");
        return false;
    }
    if(config_mp_.find("Connection.User") == config_mp_.end()) {
        LOGGER_INFO("Connection.User not found");
        return false;
    }
    if(config_mp_.find("Connection.Password") == config_mp_.end()) {
        LOGGER_INFO("Connection.Password not found");
        return false;
    }
    if(config_mp_.find("SDBus.Main.Url") == config_mp_.end()) {
        LOGGER_INFO("SDBus.Main.Url not found");
        return false;
    }

    return true;
}

void Config::loadConfigFilePath()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_file_.c_str());

    if (result.status != pugi::status_ok) {
        return;
    }

    pugi::xml_node root = doc.child("Config");
    if (!root.child("CommonServerCfgFilePath").empty()) {
        config_file_common_ = root.child("CommonServerCfgFilePath").child_value();
    }
}

bool Config::loadConfigFile(std::string configFileName)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(configFileName.c_str());

    if (result.status != pugi::status_ok) {
        LOGGER_INFO(std::string("Load config file: ") + configFileName + " failed!")
        return false;
    }

    pugi::xml_node root = doc.child("Config");
    
    //if (root.child("Connection").empty() || root.child("SDBus").empty()) {
    //    return false;
    //}

    if (!root.child("Connection").child("Host").empty()) {
        config_mp_["Connection.Host"] = root.child("Connection").child("Host").child_value();
    }
    if (!root.child("Connection").child("User").empty()) {
        config_mp_["Connection.User"] = root.child("Connection").child("User").child_value();
    }
    if (!root.child("Connection").child("Password").empty()) {
        config_mp_["Connection.Password"] = root.child("Connection").child("Password").child_value();
    }
    if (!root.child("Connection").child("DataBase").empty()) {
        config_mp_["Connection.DataBase"] = root.child("Connection").child("DataBase").child_value();
    }
    if (!root.child("Connection").child("PoolSize").empty()) {
        config_mp_["Connection.PoolSize"] = root.child("Connection").child("PoolSize").child_value();
    }

    if (!root.child("SDBus").child("Main").empty()) {
        config_mp_["SDBus.Main.Url"] = root.child("SDBus").child("Main").child("Url").child_value();
	}
	if (!root.child("SDBus").child("Main").child("HeartBeat").empty()) {
		config_mp_["SDBus.Main.HeartBeat"] = root.child("SDBus").child("Main").child("HeartBeat").child_value();
	}
    if (!root.child("SDBus").child("QB").empty()) {
		config_mp_["SDBus.QB.Url"] = root.child("SDBus").child("QB").child("Url").child_value(); 
    }
    if (!root.child("SyncDB").child("Enable").empty()) {
        config_mp_["SyncDB.Enable"] = root.child("SyncDB").child("Enable").child_value();
    }
	if (!root.child("SyncDB").child("IP").empty()) {
		config_mp_["SyncDB.IP"] = root.child("SyncDB").child("IP").child_value();
	}
	if (!root.child("SyncDB").child("Port").empty()) {
		config_mp_["SyncDB.Port"] = root.child("SyncDB").child("Port").child_value();
	}
	if (!root.child("SyncDB").child("PassordPath").empty()) {
		config_mp_["SyncDB.PassordPath"] = root.child("SyncDB").child("PassordPath").child_value();
	}
	if (!root.child("SyncDB").child("AccountInfoPath").empty()) {
		config_mp_["SyncDB.AccountInfoPath"] = root.child("SyncDB").child("AccountInfoPath").child_value();
	}

    if (!root.child("QBSync").child("Topic").empty()) {
        config_mp_["QBSync.Topic"] = root.child("QBSync").child("Topic").child_value();
    }
    if (!root.child("QBSync").child("ServiceQueue").empty()) {
        config_mp_["QBSync.ServiceQueue"] = root.child("QBSync").child("ServiceQueue").child_value();
    }

    //if (!root.child("CurrentCompany").child("ID").empty()) {
    //    config_mp_["CurrentCompany.ID"] = root.child("CurrentCompany").child("ID").child_value();
    //}
    //if (!root.child("CurrentCompany").child("Name").empty()) {
    //    config_mp_["CurrentCompany.Name"] = root.child("CurrentCompany").child("Name").child_value();
    //}


    if (!root.child("LogMsg").child("Enable").empty()) {
		config_mp_["LogMsg.Enable"] = root.child("LogMsg").child("Enable").child_value();
	}
	if (!root.child("LogMsg").child("ThreadCount").empty()) {
		config_mp_["LogMsg.ThreadCount"] = root.child("LogMsg").child("ThreadCount").child_value();
	}
	if (!root.child("LogMsg").child("Destination").empty()) {
		config_mp_["LogMsg.Destination"] = root.child("LogMsg").child("Destination").child_value();
	}

    if (!root.child("CalcAndInsertIntoMysqlDatabase").empty()) {
        config_mp_["CalcAndInsertIntoMysqlDatabase"] = root.child("CalcAndInsertIntoMysqlDatabase").child_value();
    }

	if (!root.child("ClientVersion").empty()) {
		config_mp_["ClientVersion"] = root.child("ClientVersion").child_value();
	}

    LOGGER_INFO(std::string("Load config file: ") + configFileName + " succeed!")
    return true;
}

bool Config::load(const std::string &topic)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_file_.c_str());

    if (result.status != pugi::status_ok) {
        return false;
    }

    pugi::xml_node root = doc.child("Config");
    
    // Ö§³ÖÇ¶Ì× huameng.jia 2015.05.18
    std::vector<std::string> subs = split(topic, '.');
    pugi::xml_node node = root;
    for (unsigned int i = 0; i < subs.size(); ++i) {
        if (!node.empty()) {
            node = node.child(subs[i].c_str());
        }
    }
    //pugi::xml_node node = root.child(topic.c_str());
    if (node.empty()) {
        return false;
    }

    for (pugi::xml_node sub_node = node.first_child(); sub_node; sub_node = sub_node.next_sibling()) {
        config_mp_[topic + "." + sub_node.name()] = sub_node.child_value();
    }

    return true;
}

std::string Config::getValue(const std::string &name)
{
    if (config_mp_.find(name) == config_mp_.end()) {
        return "";
    }

    return config_mp_[name];
}

std::string Config::getValue(const std::string &name, const std::string &default_value)
{
    if (config_mp_.find(name) == config_mp_.end()) {
        return default_value;
    }

    return config_mp_[name];
}

int Config::getIntValue(const std::string& name, const int& default_value) {
    if (config_mp_.find(name) == config_mp_.end()) {
        return default_value;
    }
    return atoi(config_mp_[name].c_str());
}

bool Config::getBoolValue(const std::string& name, const bool& default_value) {
    if (config_mp_.find(name) == config_mp_.end()) {
        return default_value;
    }
    return config_mp_[name] == "true" || config_mp_[name] == "TRUE" || config_mp_[name] == "1";
}