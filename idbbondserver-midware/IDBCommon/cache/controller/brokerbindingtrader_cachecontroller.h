/*
 * brokerbindingtrader_cachecontroller.h
 *
 *  Created on: September 22, 2014
 *      Author: yudan.chen
 */

#ifndef BROKER_BINDING_TRADER_CACHECONTROLLER_H_
#define BROKER_BINDING_TRADER_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/brokerbindingtrader_cache.h"

class BrokerBindingTraderCacheController : public CacheController<BrokerBindingTraderCache> {
public:
    BrokerBindingTraderCacheController();
    virtual ~BrokerBindingTraderCacheController();

private:
    void insertCache(sql::ResultSet* resSet);

};

#endif /* BROKER_BINDING_TRADER_CACHECONTROLLER_H_ */
