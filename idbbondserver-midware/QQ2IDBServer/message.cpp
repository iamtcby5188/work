#include <iostream>
#include <map>
#include "include/fid.h"
#include "include/constants.h"
#include "include/common.h"
#include <message.h>
#include "boost/type_traits/is_same.hpp"
#include "boost/lexical_cast.hpp"

namespace bcserver_service_transform {

    void boostVarToSdMsg(const BOND_VARIANT &bondBoostVar, int fid, sdbus::VariantMap &sdMap)
    {
        if(boost::get<std::string>(&bondBoostVar) != NULL)
            sdMap.SetString(fid, boost::get<std::string>(bondBoostVar));
        else if(boost::get<int>(&bondBoostVar) != NULL)
            sdMap.SetInt32(fid, boost::get<int>(bondBoostVar));
        else if(boost::get<double>(&bondBoostVar) != NULL)
            sdMap.SetDouble(fid, boost::get<double>(bondBoostVar));
        else if(boost::get<bool>(&bondBoostVar) != NULL)
            sdMap.SetBool(fid, boost::get<bool>(bondBoostVar));
    }

    void boostVarToSdMsg(const BOND_VARIANT_MAP &bondBoostVarMap, sdbus::VariantMap &sdMap)
    {
        for(BOND_VARIANT_MAP::const_iterator it = bondBoostVarMap.begin(); it != bondBoostVarMap.end(); ++it)
            boostVarToSdMsg(it->second, it->first, sdMap);
    }

}  // namespace bcserver_service_transform
