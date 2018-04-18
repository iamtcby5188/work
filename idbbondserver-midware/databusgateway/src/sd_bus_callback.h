#ifndef __sd_bus_CALLBACK_H__
#define __sd_bus_CALLBACK_H__
#include <vector>
#include <map>
#include <engine/qpid_engine.h>
#include <sdbus/engine.h>
#include <msgbus/connection.h>
#include <sdbus/callback.h>

class sd_bus_callback : public sdbus::Callback
{
public:
	sd_bus_callback (const std::string&, const std::string&, int);

	void add_dest (msgbus::Connection*, const std::string&, const std::string&);

	void add_dict (const std::string&, const std::string&);
protected:
	virtual void OnMsgs (const sdbus::Message**, int);
private:
	struct msg_destination
	{
		msgbus::Connection* con_;
		std::string name_;
		int mode_;
	};

	void send (const sdbus::Message&);

	void to_msgbus_message (msgbus::Message&, const sdbus::Message&);

	void sdbody_to_mbbody (msgbus::Message::Map&, 
			const sdbus::Message::Map&, int = 0);

	std::string get (sdbus::uint32_t);

        std::string address_;

	std::vector <msg_destination> msg_dest_list_;

	std::map <sdbus::uint32_t, std::string> dict_;
	
};

#endif

