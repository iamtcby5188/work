#include <cstdlib>
#include <log.h>
#include "databus_gateway_helper.h"

const std::string helper::_DATABUS_ = "_DATABUS_";
const std::string helper::_GATEWAY_ = "_GATEWAY_";

std::map <std::string, std::string>
helper::msgbus_dict_;

std::map <sdbus::uint32_t, std::string>
helper::sdbus_dict_;

std::map <std::string, msgbus::Connection*>
helper::msgbus_connection_dict_;

std::map <std::string, sdbus::Connection*>
helper::sdbus_connection_dict_;

/*  message id format:  replyto + _DATABUS_ + address + _GATEWAY_ + message_id   */

void helper::retrieve_original_from_message_id (std::string& address, 
		std::string& original_message_id,
		std::string& original_replyto, const std::string& message_id)
{
	std::string::size_type databus_pos, gateway_pos;
	databus_pos = message_id.find (_DATABUS_);
	if (databus_pos != std::string::npos)
	{
		original_replyto = message_id.substr (0, databus_pos);
		if (databus_pos + _DATABUS_.size () < message_id.size())
		{
			databus_pos += _DATABUS_.size ();
                        gateway_pos = message_id.find (_GATEWAY_);
                        if ((gateway_pos != std::string::npos)
			   && (gateway_pos > databus_pos))
			{
			    address = message_id.substr (databus_pos, gateway_pos - databus_pos);
				if (gateway_pos + _GATEWAY_.size () < message_id.size ())
				{
					gateway_pos += _GATEWAY_.size ();
					original_message_id = message_id.substr (gateway_pos);
				}
			}
		}
	}else
	{
	}
}

std::string helper::create_message_id_from_original (const std::string& replyto,
		const std::string& address, const std::string& message_id)
{
	return (replyto + _DATABUS_ + address + _GATEWAY_ + message_id);
}


void helper::to_sdbus_message (sdbus::Message& sdmsg, const msgbus::Message& mbmsg)
{
    sdmsg.SetType (mbmsg.GetType ());
    sdmsg.SetSubject(mbmsg.GetSubject());
    sdmsg.SetMessageID(mbmsg.GetMessageID());
    sdmsg.SetReplyTo(mbmsg.GetReplyTo());
    mbbody_to_sdbody(sdmsg.MapValue(), mbmsg.MapValue());
}

void helper::mbbody_to_sdbody(sdbus::Message::Map& sdmap,
		                    const msgbus::Message::Map& mbmap, int depth /* = 0 */)
{
	std::string key;
    int type = -1;
    for (int i = 0; i < mbmap.fieldCount(); ++i)
    {
        if (!mbmap.getFieldInfo(i, key, type))
        {
            LOG4_WARN ("getFieldInfo() failed by index %d.", i);
            continue;
        }
        std::string stag = get(key);
        if (stag.empty())
        {
            LOG4_DEBUG ("tag is not found by key: %s.", key.c_str ());
            continue;
        }

	sdbus::uint32_t tag = std::atoi(stag.c_str());
        switch(type)
        {
        case msgbus::Message::FIELD_BOOL:
        {
            bool ret = false;
            mbmap.getBool(key, ret);
            sdmap.SetBool(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_UINT8:
        {
            msgbus::uint8_t ret = 0;
            mbmap.getUint8(key, ret);
            sdmap.SetUInt8(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_UINT16:
        {
            msgbus::uint16_t ret = 0;
            mbmap.getUint16(key, ret);
            sdmap.SetUInt16(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_UINT32:
        {
            msgbus::uint32_t ret = 0;
            mbmap.getUint32(key, ret);
            sdmap.SetUInt32(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_UINT64:
        {
            msgbus::uint64_t ret = 0;
            mbmap.getUint64(key, ret);
            sdmap.SetUInt64(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_INT8:
        {
            msgbus::int8_t ret = 0;
            mbmap.getInt8(key, ret);
            sdmap.SetInt8(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_INT16:
        {
            msgbus::int16_t ret = 0;
            mbmap.getInt16(key, ret);
            sdmap.SetInt16(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_INT32:
        {
            msgbus::int32_t ret = 0;
            mbmap.getInt32(key, ret);
            sdmap.SetInt32(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_INT64:
        {
            msgbus::int32_t ret = 0;
            mbmap.getInt32(key, ret);
            sdmap.SetInt32(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_FLOAT:
        {
            float ret = 0;
            mbmap.getFloat(key, ret);
            sdmap.SetFloat(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_DOUBLE:
        {
            double ret = 0;
            mbmap.getDouble(key, ret);
            sdmap.SetDouble(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_STRING:
        {
            std::string ret;
            mbmap.getString(key, ret);
            sdmap.SetString(tag, ret);
        }
        break;
        case msgbus::Message::FIELD_RAW:
        {
            msgbus::Message::Raw ret;
            mbmap.GetRaw (key.c_str (), ret);
            sdmap.SetRaw (tag, (const char*)ret.Data, ret.Length);
        }
        break;
        case msgbus::Message::FIELD_MESSAGE:
        {
            msgbus::Message::Map ret;
            mbmap.getMap(key, ret);
            sdbus::Message::Map sdm;
            mbbody_to_sdbody(sdm, ret, depth + 1);
            sdmap.SetMap(tag, sdm);
        }
        break;
        case msgbus::Message::FIELD_LIST:
        {
            msgbus::Message::List ret;
            mbmap.getList(key, ret);
            sdbus::Message::List sdl;
            sdbus::Message::Map sdm;
            for (msgbus::Message::List::const_iterator cit =
                    ret.begin(); cit != ret.end(); ++cit)
            {
                sdm.Clear();
                mbbody_to_sdbody(sdm, *cit, depth + 1);
                sdl.PushBack(sdm);
            }
            if (!sdmap.SetList(tag, sdl))
                LOG4_WARN ("failed to set list by tag: %u.", tag);
        }
        break;
        default:
        break;
        }
    }
}

void helper::to_msgbus_message (msgbus::Message& mbmsg,
                const sdbus::Message& sdmsg)
{
        mbmsg.SetType ((std::string)sdmsg.GetType ());
        mbmsg.SetSubject ((std::string)sdmsg.GetSubject());
        mbmsg.SetMessageID ((std::string)sdmsg.GetMessageID ());
        mbmsg.SetReplyTo ((std::string)sdmsg.GetReplyTo ());
        sdbody_to_mbbody (mbmsg.MapValue (), sdmsg.MapValue ());
}

void helper::sdbody_to_mbbody (msgbus::Message::Map& mbmap,
                const sdbus::Message::Map& sdmap, int depth /* = 0 */)
{
    std::string key;
        int type = -1;
        sdbus::uint32_t tag = 0;
        const sdbus::Variant* v = NULL;
        for (unsigned int i = 0;i < sdmap.Size (); ++i)
        {
        sdmap.GetFieldInfo (i, tag, type);
                key = get (tag);
                if (key.empty ())
                {
                        LOG4_DEBUG ("key is not found by tag: %u.", tag);
                }else
                {
                    sdmap.GetFieldValue (i, v);
                    if (v != NULL)
                    {
                            switch (v->Type)
                            {
                            case sdbus::Variant::VAR_BOOL:
                                        mbmap.SetBool (key.c_str (), *v);
                                    break;
                                case sdbus::Variant::VAR_UINT8:
                                        mbmap.SetUInt8 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_UINT16:
                                        mbmap.SetUInt16 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_UINT32:
                                        mbmap.SetUInt32 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_UINT64:
                                        mbmap.SetUInt64 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_INT8:
                                        mbmap.SetInt8 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_INT16:
                                        mbmap.SetInt16 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_INT32:
                                        mbmap.SetInt32 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_INT64:
                                        mbmap.SetInt64 (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_FLOAT:
                                        mbmap.SetFloat (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_DOUBLE:
                                        mbmap.SetDouble (key.c_str (), *v);
                                        break;
                                case sdbus::Variant::VAR_STRING:
                                        {
                                                std::string ret = (std::string)(sdbus::string)*v;
                                            mbmap.SetString (key.c_str (), ret);
                                        }
                                        break;
                                case sdbus::Variant::VAR_RAW:
                                        {
                                                const void * ret = NULL;
                                                std::size_t size = 0;
                                                v->RefRaw (ret, size);
                                            mbmap.SetRaw (key.c_str (), (const char *)ret, size);
                                        }
                                        break;
                                case sdbus::Variant::VAR_MAP:
                                        {
                                                const sdbus::VariantMap& ret = *v;
                                                msgbus::Message::Map inner_map;
                                                sdbody_to_mbbody (inner_map, ret, depth + 1);
                                                mbmap.SetMap (key.c_str (), inner_map);
                                        }
                                        break;
                                case sdbus::Variant::VAR_LIST:
                                        {
                                                const sdbus::VariantList& ret = *v;
                                                msgbus::Message::List inner_list;
                                                msgbus::Message::Map inner_map;
                                                for (std::size_t i = 0; i < ret.Size (); ++i)
                                                {
                                                        inner_map.clear ();
                                                        sdbody_to_mbbody (inner_map, *ret[i], depth + 1);
                                                        inner_list.push_back (inner_map);
                                                }
                                                mbmap.SetList (key.c_str (), inner_list);
                                        }
                                        break;
                            default:
                                        LOG4_WARN ("unknown type: %u.", v->Type);
                                    break;
                            }
                    }
                }
        }
}


void helper::add_msgbus_dict (const std::string& key, const std::string& tag)
{
    msgbus_dict_.insert (std::make_pair (key, tag));
}

void helper::add_sdbus_dict (const std::string& tag, const std::string& key)
{
    sdbus_dict_.insert (std::make_pair (std::atoi(tag.c_str ()), key));
}


std::string helper::get (const std::string& key)
{
    std::map <std::string, std::string>::iterator it 
        = msgbus_dict_.find (key);
    if (it != msgbus_dict_.end ())
        return it->second;
    else
        return "";
}

std::string helper::get (sdbus::uint32_t tag)
{
        std::map <sdbus::uint32_t, std::string>::iterator it =
                sdbus_dict_.find (tag);
        if (it != sdbus_dict_.end ())
                return it->second;
        else
                return "";
}


msgbus::Connection*
helper::add_msgbus_connection (const std::string& address)
{
    std::map <std::string, msgbus::Connection*>::iterator msg_con_it
                            = msgbus_connection_dict_.find (address);
    if (msg_con_it == msgbus_connection_dict_.end ())
    {
            msgbus::Connection* pconnection = new msgbus::Connection ();
            if (pconnection->connect (address))
            {
            LOG4_INFO ("connect to msgbus(%s) successfully.", address.c_str ());
            }else
            {
                LOG4_INFO ("failed to connect to msgbus(%s).", address.c_str ());
            }

                msg_con_it = msgbus_connection_dict_.insert (
                                std::make_pair (address, pconnection)).first;
    }

    return msg_con_it->second;
}


sdbus::Connection*
helper::add_sdbus_connection (const std::string& address)
{
    std::map <std::string, sdbus::Connection*>::iterator sd_con_it
                        = sdbus_connection_dict_.find (address);
        if (sd_con_it == sdbus_connection_dict_.end ())
        {
            sdbus::Connection* pconnection = new sdbus::Connection ();
            if (pconnection->Connect (address))
            {
            LOG4_INFO ("connect to sdbus(%s) successfully.", address.c_str ());
            }else
                {
                        LOG4_INFO ("failed to connect to sdbus(%s).", address.c_str ());
            }

            sd_con_it = sdbus_connection_dict_.insert (
                                std::make_pair (address, pconnection)).first;
        }

        return sd_con_it->second;
}

msgbus::Connection*
helper::get_msgbus_connection (const std::string& address)
{
        std::map <std::string, msgbus::Connection*>::iterator it =
                msgbus_connection_dict_.find (address);
        if (it != msgbus_connection_dict_.end ())
                return it->second;
        else
                return NULL;
}

sdbus::Connection*
helper::get_sdbus_connection (const std::string& address)
{
        std::map <std::string, sdbus::Connection*>::iterator it =
                sdbus_connection_dict_.find (address);
        if (it != sdbus_connection_dict_.end ())
                return it->second;
        else
                return NULL;
}



