#include <log.h>
#include <msgbus/util.hpp>
#include <sdbus/util.hpp>
#include "databus_gateway_helper.h"
#include "sd_bus_queue_callback.h"


sd_bus_queue_callback::sd_bus_queue_callback (const std::string& name)
{
    Init (name, sdbus::DeliverDirect);
}

sd_bus_queue_callback::~sd_bus_queue_callback ()
{
    LOG4_INFO ("destruct sdbus queue callback.");
}


void sd_bus_queue_callback::OnMsgs (const sdbus::Message** sds, int count)
{
	for (int i = 0; i < count; ++i)
	{
		LOG4_TRACE ("received a direct message from sdbus(%s):\n%s.",
				Name().c_str (),
				sdbus::util::to_string (*sds[i]).c_str ());
		send (*sds[i]);
	}
}

/*  MessageID's format:   orignal ReplyTo + _DATABUS_ADDRESS_GATEWAY_ +   original MessageID  */
void sd_bus_queue_callback::send (sdbus::Message sd)
{
	std::string address, message_id, replyto;
	helper::retrieve_original_from_message_id (address, message_id,
			replyto, sd.GetMessageID().c_str ());
        sd.SetMessageID (message_id);
	sd.SetReplyTo (replyto);

        msgbus::Message msg;
        helper::to_msgbus_message (msg, sd);
	msgbus::Connection* pcon = helper::get_msgbus_connection (address);
	if (pcon != NULL)
	{
		if (pcon->send (replyto, msg))
		{
			LOG4_TRACE ("sent a direct message to sdbus (%s):\n%s.",
					replyto.c_str (),
					msgbus::util::to_string (msg).c_str ());
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


