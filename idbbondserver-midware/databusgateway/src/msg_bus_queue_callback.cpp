#include <log.h>
#include <msgbus/util.hpp>
#include <sdbus/util.hpp>
#include "databus_gateway_helper.h"
#include "msg_bus_queue_callback.h"


msg_bus_queue_callback::msg_bus_queue_callback (const std::string& name)
{
	name_ = name;
	type_ = msgbus::DeliverDirect;
}

msg_bus_queue_callback::~msg_bus_queue_callback ()
{
    LOG4_INFO ("destruct msgbus queue callback.");
}


void msg_bus_queue_callback::messages (const msgbus::Message* msgs, int count)
{
	for (int i = 0; i < count; ++i)
	{
		LOG4_TRACE ("received a direct message from msgbus(%s):\n%s.",
				name_.c_str (),
				msgbus::util::to_string (msgs[i]).c_str ());
		send (msgs[i]);
	}
}

/*  MessageID's format:   orignal ReplyTo + _DATABUS_ADDRESS_GATEWAY_ +   original MessageID  */
void msg_bus_queue_callback::send (msgbus::Message msg)
{
	std::string address, message_id, replyto;
	helper::retrieve_original_from_message_id (address, message_id,
			replyto, msg.GetMessageID());
        msg.SetMessageID (message_id);
	msg.SetReplyTo (replyto);

        sdbus::Message sdmsg;
        helper::to_sdbus_message (sdmsg, msg);
	sdbus::Connection* pcon = helper::get_sdbus_connection (address);
	if (pcon != NULL)
	{
		if (pcon->Send (replyto, sdmsg))
		{
			LOG4_TRACE ("sent a direct message to sdbus (%s):\n%s.",
					replyto.c_str (),
					sdbus::util::to_string (sdmsg).c_str ());
		}else
		{
			LOG4_WARN ("failed to sent a direct message to sdbus (%s).",
					replyto.c_str ());
		}
	}else
	{
		LOG4_WARN ("can't find connection by %s and discard a sdbus message!",
                       address.c_str ());
	}
}


