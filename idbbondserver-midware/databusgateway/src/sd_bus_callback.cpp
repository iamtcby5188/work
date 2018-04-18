#include <cstdlib>
#include <vector>
#include <log.h>
#include <msgbus/util.hpp>
#include <sdbus/util.hpp>
#include "msg_bus_queue_callback.h"
#include "databus_gateway_helper.h"
#include "sd_bus_callback.h"

sd_bus_callback::sd_bus_callback (const std::string& address, const std::string& name, int type)
: address_ (address)
{
	Init (name, type);
}


void sd_bus_callback::add_dest (msgbus::Connection* pcon, 
		const std::string& name, const std::string& mode)
{
	msg_destination dest;
	dest.con_ = pcon;
	dest.name_ = name;
	dest.mode_ = std::atoi (mode.c_str ());

	msg_dest_list_.push_back (dest);
}



void sd_bus_callback::OnMsgs (const sdbus::Message **msgs, int count)
{
	for (int i = 0; i < count; ++i)
	{
		/* handle each message */
		LOG4_TRACE ("received a message from sdbus(%s : %d):\n%s", 
				Name ().c_str (), Type (),
				sdbus::util::to_string (*msgs[i]).c_str ());
		send (*msgs[i]);
	}
}

void sd_bus_callback::send (const sdbus::Message& sdmsg)
{
	msgbus::Message mbmsg;
	helper::to_msgbus_message (mbmsg, sdmsg);
	for (int i = 0; i < msg_dest_list_.size (); ++i)
	{
		std::string name = msg_dest_list_[i].name_;
		if (name.empty ())
			name = mbmsg.GetReplyTo ();
		if (name.empty ())
		{
			LOG4_WARN ("exchange name is empty and discard the message:\n%s.",
					msgbus::util::to_string (mbmsg).c_str ());
		}else
		{
                    msgbus::DeliverType type = (msgbus::DeliverType)msg_dest_list_[i].mode_;
                    if (sdbus::DeliverDirect == Type ())
                    {
                        type = msgbus::DeliverDirect;
                        mbmsg.SetMessageID (helper::create_message_id_from_original (
                                 mbmsg.GetReplyTo (), address_, mbmsg.GetMessageID()));                        
                        mbmsg.SetReplyTo (DATABUS_GATEWAY_QUEUE_FOR_MSGBUS_RESPONSE); 
                    }
                            
   		    if (msg_dest_list_[i].con_->send (name,
				mbmsg, type))
		         LOG4_TRACE ("sent a message to msgbus(%s : %d):\n%s.", 
					name.c_str (),
					type,
					msgbus::util::to_string (mbmsg).c_str ());
		    else
			    LOG4_WARN ("failed to send a message to msgbus(%s : %d).",
					name.c_str (),
					type);
                   
                    
		}
	}
}


