#ifndef _BONDSMALLVIEW_SYNC_H_
#define _BONDSMALLVIEW_SYNC_H_

#include "logger.h"

class BondSmallViewSync 
{
public:
    BondSmallViewSync();
    ~BondSmallViewSync();

    void sync();
    //void syncForChinaBondValuationChanged(const std::vector<std::string> &updated_chinabondvaluation_vec);
   
};

#endif // _BONDSMALLVIEW_SYNC_H_