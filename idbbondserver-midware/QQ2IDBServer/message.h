#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "sdbus/connection.h"
#include <map>
#include <list>
#include "boost/variant.hpp"
#include "boost/lexical_cast.hpp"

namespace bcserver_service_transform {
    typedef boost::variant<std::string, int, double, bool> BOND_VARIANT;
    typedef std::map<int, BOND_VARIANT> BOND_VARIANT_MAP;
    typedef std::list<BOND_VARIANT_MAP> BOND_VARIANT_MAP_LIST;

    const int FID_QQ_SOURCE_MSG = 100002;

    void boostVarToSdMsg(const BOND_VARIANT &bondBoostVar, int fid, sdbus::VariantMap &sdMap);
    void boostVarToSdMsg(const BOND_VARIANT_MAP &bondBoostVarMap, sdbus::VariantMap &sdMap);
    void sdMsgToBoostVar(const sdbus::VariantMap &sdMap, int fid, BOND_VARIANT_MAP &bondBoostVarMap);

    // F:type from, T:type to
    template<class F, class T> void sdMapToBoostVarMap(const sdbus::VariantMap &sdMap, BOND_VARIANT_MAP &bondBoostVarMap, int fid)
    {
        std::string a;
        int b;
        double c;
        bool d;
        BOND_VARIANT bv;
        if(boost::is_same<F, std::string>::value) {
            sdMap.GetString(fid, a);
            bv = a;
        } else if(boost::is_same<F, int>::value) {
            sdMap.GetInt32(fid, b);
            bv = b;
        } else if(boost::is_same<F, double>::value) {
            sdMap.GetDouble(fid, c);
            bv = c;
        } else if(boost::is_same<F, bool>::value) {
            sdMap.GetBool(fid, d);
            bv = d;
        } else
            return;

        if(boost::is_same<F, T>::value)
            bondBoostVarMap[fid] = bv;
        else
            bondBoostVarMap[fid] = boost::lexical_cast<T>(bv);
    }

    template<class FT> void sdMapToBoostVarMap(const sdbus::VariantMap &sdMap, BOND_VARIANT_MAP &bondBoostVarMap, int fid)
    {
        sdMapToBoostVarMap<FT, FT>(sdMap, bondBoostVarMap, fid);
    }

}  // namespace bcserver_service_transform

#endif