#include <cstdlib>
#include <log.h>
#include <msgbus/util.hpp>
#include <sdbus/util.hpp>
#include "databus_gateway_helper.h"
#include "sd_bus_queue_callback.h"
#include "msg_bus_callback.h"

msg_bus_callback::msg_bus_callback (const std::string& address, 
                                const std::string& name, msgbus::DeliverType type)
: address_ (address)
{
	name_ = name;
	type_ = type;
}

void msg_bus_callback::add_dest (sdbus::Connection* pcon, 
		const std::string& name, const std::string& mode)
{
	sd_destination dest;
	dest.con_ = pcon;
	dest.name_ = name;
	dest.mode_ = std::atoi (mode.c_str ());

	sd_dest_list_.push_back (dest);
}

void msg_bus_callback::messages (const msgbus::Message* msgs, int count)
{
	for (int i = 0; i < count; ++i)
	{
		/* handle each message. */
		LOG4_TRACE ("received a message form msgbus(%s : %d):\n%s", 
				name_.c_str (), type_, msgbus::util::to_string (msgs[i]).c_str ());
		send (msgs[i]);
	}
}
void msg_bus_callback::send (const msgbus::Message& mbmsg)
{
    bool ret = false;
    sdbus::Message sdmsg;
    helper::to_sdbus_message (sdmsg, mbmsg);
    for (int i = 0; i < sd_dest_list_.size (); ++i)
    {
//		sdmsg.SetType (sd_dest_list_[i].name_);
        std::string name = sd_dest_list_[i].name_;
	if (name.empty ())
		name = sdmsg.GetReplyTo ();
	if (name.empty ())
	{
		LOG4_WARN ("exchange name is empty and discard the message:\n%s.",
				sdbus::util::to_string (sdmsg).c_str ());
	}else
	{
            int type = sd_dest_list_[i].mode_;
            if (msgbus::DeliverDirect == type_)
            {
                type = sdbus::DeliverDirect;
                sdmsg.SetMessageID (helper::create_message_id_from_original(
                              mbmsg.GetReplyTo (), address_, mbmsg.GetMessageID()));
                sdmsg.SetReplyTo (DATABUS_GATEWAY_QUEUE_FOR_SDBUS_RESPONSE);
            }

	    if (sd_dest_list_[i].mode_)
	        ret = sd_dest_list_[i].con_->Publish (name, sdmsg);
	    else
	        ret = sd_dest_list_[i].con_->Send (name, sdmsg);

	    if (ret)
	        LOG4_TRACE ("sent a message to sdbus(%s : %d):\n%s.", 
			name.c_str (),
		        sd_dest_list_[i].mode_ , 
			sdbus::util::to_string (sdmsg).c_str ());
	    else
	        LOG4_WARN ("failed to send a message to sdbus(%s : %d).",
			name.c_str (), 
			sd_dest_list_[i].mode_);
	}
    }
}


