/*==================================================================================================================
                                    Copyright (C) 2016 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    InstitutionRatingSync 用于同步发行人评级表数据
====================================================================================================================
   Date       Name                                  Description of Change
20-Apr-2016   Young                                 Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _INSTITUTIONRATING_SYNC_H_
#define _INSTITUTIONRATING_SYNC_H_

#include "logger.h"
#include "cache/model/institutionrating_cache.h"

class InstitutionRatingSync
{    
public:
    InstitutionRatingSync();
    ~InstitutionRatingSync();

    void sync();

private:
	void updateBondSmallViewCache(InstitutionRatingCache* cache);
};

#endif