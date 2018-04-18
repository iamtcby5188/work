#ifndef __MSG_BUS_QUEUE_CALLBACK_H__
#define __MSG_BUS_QUEUE_CALLBACK_H__
#include <sdbus/connection.h>
#include <msgbus/callback.h>

#define DATABUS_GATEWAY_QUEUE_FOR_MSGBUS_RESPONSE  "DATABUS_GATEWAY_QUEUE_FOR_MSGBUS_RESPONSE"

class msg_bus_queue_callback : public msgbus::VectCallback
{
public:
	msg_bus_queue_callback (const std::string&);
        virtual ~msg_bus_queue_callback ();

protected:
	virtual void messages (const msgbus::Message*, int);

	void send (msgbus::Message msg);

private:
};


#endif

