#include <cstdlib>
#include <boutil/process.h>
#include <boutil/ini.h>
#include <log.h>
#include "databus_gateway_helper.h"
#include "msg_bus_queue_callback.h"
#include "sd_bus_queue_callback.h"
#include "data_bus_gateway.h"

#define MSGBUS       "msgbus"
#define SDBUS        "sdbus"
#define SERVICE      "service"
#define SRC          "src"
#define DEST         "dest"
#define KEY2FID      "key2fid"
#define MULTIKEY     "multikey"
#define FID2KEY      "fid2key"
#define MULTIFID     "multifid"


data_bus_gateway::~data_bus_gateway ()
{
	std::map <std::string, msgbus::Callback*>::iterator msg_bus_callback_it;
	for (msg_bus_callback_it = msg_bus_callback_dict_.begin (); 
	    msg_bus_callback_it != msg_bus_callback_dict_.end (); 
		++msg_bus_callback_it)
		delete msg_bus_callback_it->second;
	std::map <std::string, sdbus::Callback*>::iterator sd_bus_callback_it;
	for (sd_bus_callback_it = sd_bus_callback_dict_.begin(); 
		sd_bus_callback_it != sd_bus_callback_dict_.end ();
		++sd_bus_callback_it)
		delete sd_bus_callback_it->second;
}

void data_bus_gateway::init ()
{
	boutil::ini ini (boutil::process::run_path () 
			+ SLASH_CHAR + ".."
                        + SLASH_CHAR + "cfg"
                        + SLASH_CHAR + "cfg.ini");
	std::string info;
	ini.init (info);
        LOG4_INFO ("load configuration info:\n%s.", info.c_str ());

	for (bool exist = ini.begin (SERVICE); 
		exist; exist = ini.next ())
	{
        init (ini.get (SRC), ini.get (DEST));
	}
	

	if (ini.begin (KEY2FID))
	{		
		for (bool exist = ini.field_begin (); exist; exist = ini.field_next ())
                {
		    helper::add_msgbus_dict (ini.get_field().first, ini.get_field().second);    		    
		    helper::add_sdbus_dict (ini.get_field().second, ini.get_field().first);    		    
                }
	}

        if (ini.begin (MULTIKEY))
        {
             for (bool exist = ini.field_begin (); exist; exist = ini.field_next ())
                  helper::add_msgbus_dict (ini.get_field().first, ini.get_field().second);
        }

        
        if (ini.begin (MULTIFID))
        {
             for (bool exist = ini.field_begin (); exist; exist = ini.field_next ())
                 helper::add_sdbus_dict (ini.get_field().first, ini.get_field ().second);
        }

}

void data_bus_gateway::init (const std::string& src, const std::string& dest)
{
	std::string protocol,  address, name, mode;
	get_param (protocol, address, name, mode, src);
    
	if (protocol == MSGBUS)
	{
		msgbus::Connection* pmsgcon
		    = helper::add_msgbus_connection (address);
		std::map <std::string, msgbus::Callback*>::iterator msg_it 
			= init_msgbus_callback_dict (src, address, name, mode);
                msgbus::DeliverType src_mode = (msgbus::DeliverType)std::atoi(mode.c_str ());
		get_param (protocol, address, name, mode, dest);
		if (protocol == SDBUS)
		{
			sdbus::Connection* psdcon
				= helper::add_sdbus_connection (address);
			((msg_bus_callback*)msg_it->second)->add_dest (psdcon, name, mode);
                       
                        if (msgbus::DeliverDirect == src_mode)
                            create_sdbus_queue_callback (address);
		}
	}else if (protocol == SDBUS)
	{
		sdbus::Connection* psdcon
			= helper::add_sdbus_connection (address);
		std::map <std::string, sdbus::Callback*>::iterator sd_it
			= init_sdbus_callback_dict (src, address, name, mode);
                int src_mode = std::atoi (mode.c_str ());
               	get_param (protocol, address, name, mode, dest);
		if (protocol == MSGBUS)
		{
                     
                    	msgbus::Connection*  pmsgcon
				= helper::add_msgbus_connection (address);
			((sd_bus_callback*)sd_it->second)->add_dest (pmsgcon, name, mode);
                     
                    if (sdbus::DeliverDirect == src_mode)
                            create_msgbus_queue_callback (address);
  
                        
		}else
                    LOG4_INFO ("dest protocol: %s.", protocol.c_str ());
	}else
	{
	}
}
std::map <std::string, msgbus::Callback*>::iterator 
data_bus_gateway::init_msgbus_callback_dict (const std::string& url, const std::string& address,
		const std::string& name, const std::string& mode)
{
    std::map <std::string, msgbus::Callback*>::iterator msg_it;
    msg_it = msg_bus_callback_dict_.find (url);
    if (msg_it == msg_bus_callback_dict_.end ())
    {
	     msg_bus_callback * callback = new msg_bus_callback (address, name,
	           (msgbus::DeliverType)std::atoi (mode.c_str ()));
             msgbus::Connection * pmsgcon = helper::get_msgbus_connection (address);
             if (pmsgcon != NULL)
             {
     	         if (pmsgcon->setHandler (callback))
	         {
                     msg_it = msg_bus_callback_dict_.insert (
				     std::make_pair (url, callback)).first;			
				 LOG4_INFO ("add a msgbus callback: %s.", url.c_str ());
		 }
             }
    }
    return msg_it;
}

std::map <std::string, sdbus::Callback*>::iterator
data_bus_gateway::init_sdbus_callback_dict (const std::string& url, const std::string& address,
		const std::string& name, const std::string& mode)
{
   std::map <std::string, sdbus::Callback*>::iterator sd_it;
   sdbus::Connection* psdcon = helper::get_sdbus_connection (address);
   if (psdcon != NULL)
   {

   int type = std::atoi (mode.c_str ());
	sd_it  = sd_bus_callback_dict_.find (url);
 	if (sd_it == sd_bus_callback_dict_.end ())
  	{
	     sd_bus_callback * callback = new sd_bus_callback (address, name,
	           type); 
             if (psdcon->AddHandler (callback))
	     {
                 sd_it = sd_bus_callback_dict_.insert (
				     std::make_pair (url, callback)).first;			
		 LOG4_INFO ("add a sdbus callback: %s.", url.c_str ());
	      }
	}
   }

   return sd_it;
}

void data_bus_gateway::create_msgbus_queue_callback (const std::string& address)
{
    msgbus::Connection* pmsgcon = helper::get_msgbus_connection (address);
    if (pmsgcon != NULL)
    {
       std::string mburl = address + "_" + DATABUS_GATEWAY_QUEUE_FOR_MSGBUS_RESPONSE 
                          + "_DELIVER_DIRECT";
       if (msg_bus_callback_dict_.find (mburl) 
             == msg_bus_callback_dict_.end ())
       {
           msg_bus_queue_callback * pcallback = new msg_bus_queue_callback (
              DATABUS_GATEWAY_QUEUE_FOR_MSGBUS_RESPONSE);
           if (pcallback != NULL)
           {
               if (pmsgcon->setHandler (pcallback))
               {
                   msg_bus_callback_dict_.insert (std::make_pair (
                      mburl, pcallback));
                   LOG4_INFO ("add a msgbus callback: %s.", mburl.c_str ());
               }else
                   LOG4_WARN ("failed to add a msgbus callback: %s.", mburl.c_str ());
           } 
       }
    }

}



void data_bus_gateway::create_sdbus_queue_callback (const std::string& address)
{
    sdbus::Connection* psdcon = helper::get_sdbus_connection (address);
    if (psdcon != NULL)
    {
       std::string sdurl = address + "_" + DATABUS_GATEWAY_QUEUE_FOR_SDBUS_RESPONSE 
                          + "_DELIVER_DIRECT";
       if (sd_bus_callback_dict_.find (sdurl) 
             == sd_bus_callback_dict_.end ())
       {
           sd_bus_queue_callback * pcallback = new sd_bus_queue_callback (
              DATABUS_GATEWAY_QUEUE_FOR_SDBUS_RESPONSE);
           if (pcallback != NULL)
           {
               if (psdcon->AddHandler (pcallback))
               {
                   sd_bus_callback_dict_.insert (std::make_pair (
                      sdurl, pcallback));
                   LOG4_INFO ("add a sdbus callback: %s.", sdurl.c_str ());
               }else
                   LOG4_WARN ("failed to add a sdbus callback: %s.", sdurl.c_str ());
           } 
       }
    }

}



void data_bus_gateway::get_param (std::string& protocol, std::string& address, 
		std::string& name, std::string& mode, const std::string& src)
{
    std::string::size_type pos = src.find (':');
	if (pos != std::string::npos)
	{
		protocol = src.substr (0, pos);
		pos += 3;
		if (pos < src.size ())
		{
		    std::string::size_type slash_pos = src.find ('/', pos);
		    if (slash_pos != std::string::npos)
		    {
			    address = src.substr (pos, slash_pos - pos);
			    pos = ++slash_pos;
				if (pos < src.size ())
				{
			        slash_pos = src.find ('?', pos);
			        if (slash_pos != std::string::npos)
			        {
			            name = src.substr (pos, slash_pos - pos);
			            pos = ++slash_pos;
						if (pos < src.size ())
						{
							mode = src.substr (pos);
						}
                    }
				}
		    }
		}
	}
}

void data_bus_gateway::start ()
{
	init ();
	while (true)
	{
		sleep (30000);
	}
}

void data_bus_gateway::stop ()
{
}

