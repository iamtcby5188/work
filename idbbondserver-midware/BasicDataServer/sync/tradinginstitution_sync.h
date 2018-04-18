#ifndef _TRADINGINSTITUTION_SYNC_H_
#define _TRADINGINSTITUTION_SYNC_H_

#include "logger.h"
#include "cache/model/tradinginstitution_cache.h"

class TradingInstitutionSync
{    
public:
    TradingInstitutionSync();
    ~TradingInstitutionSync();

	void sync();
	void sync(TradingInstitutionCacheVecPtr& inst_vec, bool is_init);

};

#endif // _TRADINGINSTITUTION_SYNC_H_