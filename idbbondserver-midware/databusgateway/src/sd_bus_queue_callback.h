#ifndef __SD_BUS_QUEUE_CALLBACK_H__
#define __SD_BUS_QUEUE_CALLBACK_H__
#include <sdbus/connection.h>
#include <sdbus/callback.h>

#define DATABUS_GATEWAY_QUEUE_FOR_SDBUS_RESPONSE  "DATABUS_GATEWAY_QUEUE_FOR_SDBUS_RESPONSE"

class sd_bus_queue_callback : public sdbus::Callback
{
public:
	sd_bus_queue_callback (const std::string&);
        virtual ~sd_bus_queue_callback ();

protected:
	virtual void OnMsgs (const sdbus::Message**, int);

	void send (sdbus::Message sd);

private:
};


#endif

