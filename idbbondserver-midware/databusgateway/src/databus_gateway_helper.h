#ifndef __DATABUS_GATEWAY_HELPER_H__
#define __DATABUS_GATEWAY_HELPER_H__
#include <string>
#include <map>
#include <msgbus/message.h>
#include <sdbus/message.h>
#include <msgbus/connection.h>
#include <sdbus/connection.h>

class helper
{
public:
    static const std::string _DATABUS_;
    static const std::string _GATEWAY_;

    static void retrieve_original_from_message_id (std::string&, 
			std::string&, std::string&, const std::string&); 

    static std::string create_message_id_from_original (const std::string&,
			const std::string&, const std::string&);

    static void to_sdbus_message (sdbus::Message&,
			const msgbus::Message&);

    static void to_msgbus_message (msgbus::Message&,
			const sdbus::Message&);

    static void add_msgbus_dict (const std::string&, const std::string&);
    static void add_sdbus_dict (const std::string&, const std::string&);


    static msgbus::Connection* add_msgbus_connection (const std::string&);

    static sdbus::Connection* add_sdbus_connection (const std::string&);

    static msgbus::Connection* get_msgbus_connection (const std::string&);

    static sdbus::Connection* get_sdbus_connection (const std::string&);


private:

	static void mbbody_to_sdbody (sdbus::Message::Map& sdmap,
			const msgbus::Message::Map& mbmap, int depth  = 0 );

	static void sdbody_to_mbbody (msgbus::Message::Map&,
			const sdbus::Message::Map&, int =  0);

        static std::string get (const std::string&);
        static std::string get (sdbus::uint32_t);

        static std::map <std::string, std::string> msgbus_dict_;

        static std::map <sdbus::uint32_t, std::string>  sdbus_dict_;

        static std::map <std::string, msgbus::Connection*> msgbus_connection_dict_;

        static std::map <std::string, sdbus::Connection*> sdbus_connection_dict_;

};

#endif

