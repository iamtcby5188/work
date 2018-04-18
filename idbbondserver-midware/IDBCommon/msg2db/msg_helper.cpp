#include "msg_helper.h"
#include "sdbus/message.h"
#include "sdbus/variant.h"
#include <string>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

void MsgHelper::MessageToString(const sdbus::Message & msg, std::string & ret)
{
    
	HeaderToString(msg, ret);
	try
	{
		MapToString(msg.MapValue(), 0, ret);

		if (ret[ret.size() - 1] != '\n') {
			ret.push_back('\n');
		} else {
			while (ret[ret.size() - 2] == '\n') {
				ret.resize(ret.size() - 1);
			}
		}
	}
	catch (std::exception const&  ex)
	{
		LOGGER_ERROR("Error when parsing message: "<<ex.what() << " Pos:"__FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
	}
}

std::string MsgHelper::Blank(int level)
{
    return std::string(level * 2, '-');
}

std::string MsgHelper::TypeToString(sdbus::int16_t type)
{
    if (type == sdbus::Variant::VAR_BOOL) {
        return "B  ";
    } else if (type == sdbus::Variant::VAR_UINT8) {
        return "U8 ";
    } else if (type == sdbus::Variant::VAR_UINT16) {
        return "U16";
    } else if (type == sdbus::Variant::VAR_UINT32) {
        return "U32";
    } else if (type == sdbus::Variant::VAR_UINT64) {
        return "U64";
    } else if (type == sdbus::Variant::VAR_INT8) {
        return "I8 ";
    } else if (type == sdbus::Variant::VAR_INT16) {
        return "I16";
    } else if (type == sdbus::Variant::VAR_INT32) {
        return "I32";
    } else if (type == sdbus::Variant::VAR_INT64) {
        return "I64";
    } else if (type == sdbus::Variant::VAR_FLOAT) {
        return "FLO";
    } else if (type == sdbus::Variant::VAR_DOUBLE) {
        return "DOU";
    } else if (type == sdbus::Variant::VAR_STRING) {
        return "STR";
    } else if (type == sdbus::Variant::VAR_MAP) {
        return "MAP";
    } else if (type == sdbus::Variant::VAR_LIST) {
        return "LIST";
    } else {
        throw std::runtime_error("TypeToString error");
    }
}

void MsgHelper::HeaderToString(const sdbus::Message & msg, std::string & ret)
{
    ret += "[Type](" + msg.GetType() + ")\n";
    ret += "[Subject](" + msg.GetSubject() + ")\n";
    ret += "[MessageID](" + msg.GetMessageID() + ")\n";
    ret += "[ReplyTo](" + msg.GetReplyTo() + ")\n";
}

void MsgHelper::MapToString(const sdbus::Variant::Map & map, int level, std::string & ret)
{
	int size = map.GetFieldCount();
	for (int i=0; i<size; ++i) {
		sdbus::Variant var;
		const sdbus::Variant *p_var = &var;
		if (!map.GetFieldValue(i,p_var))
		{
			continue;
		}
		sdbus::uint32_t name = p_var->Tag;
		char str_name[10];
		sprintf(str_name,"%d",name);
		std::string type = TypeToString(p_var->Type);
		ret += Blank(level) + "<" + type + ">[" + str_name + "]";

		if (type == "MAP") {
			ret += "\n";
			MapToString(*(p_var), level + 1, ret);
		} else if (type == "LIST") {
			ret += "\n";
			ListToString(*(p_var), level + 1, ret);
		} else {
			AtomToString(*(p_var), ret);
			ret += "\n";
		}
	}
}

void MsgHelper::ListToString(const sdbus::Variant::List & list, int level, std::string & ret)
{
	for (size_t i = 0; i != (int)list.Size(); ++i) {
		char str_name[10];
		const sdbus::Variant *p_var = list.Get(i);
		sdbus::uint32_t name = p_var->Tag;
		std::string type = TypeToString(p_var->Type);
		if (type == "MAP")
		{
			sprintf(str_name,"%d",i);
			ret += Blank(level) + "<" + type + ">[" + str_name + "]";
		}
		else
		{
			sprintf(str_name,"%d",name);
			ret += Blank(level) + "<" + type + ">[" + str_name + "]";
		}
		
		if (type == "MAP") {
			ret += "\n";
			MapToString(*(list[i]), level + 1, ret);
		} else if (type == "LIST") {
			ret += "\n";
			ListToString(*(list[i]), level + 1, ret);
		} else {
			AtomToString(*(list[i]), ret);
			ret += "\n";
		}
	}
}

void MsgHelper::AtomToString(const sdbus::Variant & atom, std::string & ret)
{
    sdbus::int16_t type = atom.Type;

    if (type == sdbus::Variant::VAR_BOOL) {
        ret += "(" + std::string(bool(atom) ? "true": "false") + ")";
    } else if (type == sdbus::Variant::VAR_UINT8) {
        ret += "(" + boost::lexical_cast<std::string>(int(sdbus::uint8_t(atom))) + ")";
    } else if (type == sdbus::Variant::VAR_UINT16) {
        ret += "(" + boost::lexical_cast<std::string>(sdbus::uint16_t(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_UINT32) {
        ret += "(" + boost::lexical_cast<std::string>(sdbus::uint32_t(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_UINT64) {
        ret += "(" + boost::lexical_cast<std::string>(sdbus::uint64_t(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_INT8) {
        ret += "(" + boost::lexical_cast<std::string>(int(sdbus::int8_t(atom))) + ")";
    } else if (type == sdbus::Variant::VAR_INT16) {
        ret += "(" + boost::lexical_cast<std::string>(sdbus::int16_t(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_INT32) {
        ret += "(" + boost::lexical_cast<std::string>(sdbus::int32_t(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_INT64) {
        ret += "(" + boost::lexical_cast<std::string>(sdbus::int64_t(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_FLOAT) {
        ret += "(" + boost::lexical_cast<std::string>(float(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_DOUBLE) {
        ret += "(" + boost::lexical_cast<std::string>(double(atom)) + ")";
    } else if (type == sdbus::Variant::VAR_STRING) {
        std::string str;
        if (!atom.GetString(str)) {
            throw std::runtime_error("AtomToString error");
        }
        ret += "(" + str + ")";
    } else {
        throw std::runtime_error("AtomToString error");
    }
}
