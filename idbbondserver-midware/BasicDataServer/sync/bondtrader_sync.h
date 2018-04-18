#ifndef _BONDTRADER_SYNC_H_
#define _BONDTRADER_SYNC_H_

#include "logger.h"
#include "cache/model/bondtrader_cache.h"

class BondTraderSync
{    
public:
    BondTraderSync();
    ~BondTraderSync();

	void sync();
	void sync(BondTraderCacheVecPtr& bond_trader_vec, bool is_init);

};

#endif // _BONDTRADER_SYNC_H_