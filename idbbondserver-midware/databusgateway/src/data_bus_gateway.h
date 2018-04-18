#ifndef __DATA_BUS_GATEWAY_H__
#define __DATA_BUS_GATEWAY_H__
#include <string>
#include <map>
#include <msgbus/connection.h>
#include <sdbus/connection.h>
#include <engine/qpid_engine.h>
#include "msg_bus_callback.h"
#include "sd_bus_callback.h"

class data_bus_gateway
{
public:
	~data_bus_gateway ();

	void start ();
	void stop ();

protected:
private:
    void init ();

	void init (const std::string&, const std::string&);

	void get_param (std::string&, std::string&, std::string&, std::string&, const std::string&);

	std::map <std::string, msgbus::Callback*>::iterator 
		init_msgbus_callback_dict (const std::string&,
			const std::string&, const std::string&, const std::string&);

	std::map <std::string, sdbus::Callback*>::iterator 
		init_sdbus_callback_dict (const std::string&,
			const std::string&, const std::string&, const std::string&);

        void create_msgbus_queue_callback (const std::string&);
        void create_sdbus_queue_callback (const std::string&);

	std::map <std::string, msgbus::Callback*> msg_bus_callback_dict_;

	std::map <std::string, sdbus::Callback*>  sd_bus_callback_dict_;

    engine::qpid::EngineInitializer engine_;
};

#endif

