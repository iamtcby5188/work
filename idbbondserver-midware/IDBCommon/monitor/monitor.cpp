#include "monitor.h"
#include "db/dbconnector.h"
#include "util/pugixml/pugixml.hpp"
#include "constants.h"
#include "include/common.h"
Monitor::Monitor()
{

}

bool Monitor::init(const std::string &name, const std::string &schema_name)
{
    if (name.length() == 0 || schema_name.length() == 0) {
        return false;
    }

    std::string sql_str = fetchSQL(name);

    if (sql_str.length() == 0) {
        return false;
    }

    sql::ResultSet* resSet = DBConnector::getInstance()->executeQuery(sql_str, schema_name);
    if (resSet == NULL) {
        return false;
    }

    LOGGER_INFO("Load " << name << " - Result Set: " << resSet->rowsCount())      
    while (resSet->next()) {
        load(resSet);
    }
    delete resSet;
    LOGGER_INFO("Successfully load data... " << name)  
	return true;
}

std::string Monitor::fetchSQL(const std::string &name) 
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sqlquery.xml");

    if (result.status != pugi::status_ok) {
        return "";
    }
    std::string sql_str = doc.child("SQL").child(name.c_str()).child_value();
	trimSql(sql_str);
    //if(name == "FetchAccountSQL")
    //    sql_str += " AND idb_account.company_id = '" + kCurrentCompanyId + "';"; //Del by Young 05-Apr-2016

    return sql_str;
}