#ifndef _BROKERBINDINGTRADER_SYNC_H_
#define _BROKERBINDINGTRADER_SYNC_H_

#include "logger.h"
#include "cache/model/brokerbindingtrader_cache.h"

class BrokerBindingTraderSync
{    
public:
    BrokerBindingTraderSync();
    ~BrokerBindingTraderSync();

	void sync();
	void sync(BrokerBindingTraderCacheVecPtr& broker_bind_trader_vec, bool is_init);

};

#endif // _BROKERBINDINGTRADER_SYNC_H_