#ifndef __MSG_BUS_CALLBACK_H__
#define __MSG_BUS_CALLBACK_H__
#include <vector>
#include <map>
#include <sdbus/connection.h>
#include <msgbus/callback.h>

class msg_bus_callback : public msgbus::VectCallback
{
public:
	msg_bus_callback (const std::string&, const std::string&, msgbus::DeliverType);

	void add_dest (sdbus::Connection*, const std::string&, const std::string&);

	void add_dict (const std::string&, const std::string&);
protected:
	virtual void messages (const msgbus::Message*, int);
private:
	struct sd_destination
	{
		sdbus::Connection* con_;
		std::string  name_;
		int mode_;
	};

	void send (const msgbus::Message& msg);

	void to_sdbus_message (sdbus::Message&, const msgbus::Message&);

	void mbbody_to_sdbody(sdbus::Message::Map& sdmap,
        const msgbus::Message::Map& mbmap, int depth  = 0);

	std::string get (const std::string&);

        std::string address_;

	std::vector <sd_destination> sd_dest_list_;

	std::map <std::string, std::string> dict_;
};

#endif
