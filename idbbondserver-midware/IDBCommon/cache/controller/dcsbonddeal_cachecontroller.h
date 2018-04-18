
#ifndef DCSBONDDEAL_CACHECONTROLLER_H_
#define DCSBONDDEAL_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/dcsbonddeal_cache.h"

class DcsBondDealCacheController : public CacheController<DcsBondDealCache> {
public:
	DcsBondDealCacheController();
	virtual ~DcsBondDealCacheController();

	void clearCache();
	void cacheTable(DcsBondDealCacheVecPtr& deal_vec);

private:
	void insertCache(DcsBondDealCache& cache);

};

#endif /* DCSBONDDEAL_CACHECONTROLLER_H_ */
