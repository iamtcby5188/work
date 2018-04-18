#ifndef MSG_HELPER_H_
#define MSG_HELPER_H_

#include <string>
#include "sdbus/message.h"
#include "sdbus/variant.h"
#include "logger.h"

class MsgHelper {
public:
    ////////////////////////////////////////////////////////////////////////////
    static void MessageToString(const sdbus::Message & msg, std::string & ret);

private:
    ////////////////////////////////////////////////////////////////////////////
    static std::string Blank(int level);
    static std::string TypeToString(sdbus::int16_t type);

    ////////////////////////////////////////////////////////////////////////////
    static void HeaderToString(const sdbus::Message & msg, std::string & ret);
    static void MapToString(const sdbus::Variant::Map & map, int level, std::string & ret);
    static void ListToString(const sdbus::Variant::List & list, int level, std::string & ret);
    static void AtomToString(const sdbus::Variant & atom, std::string & ret);
};

#endif //MSG_HELPER_H_
