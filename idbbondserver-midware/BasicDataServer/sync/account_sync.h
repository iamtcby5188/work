#ifndef _ACCOUNT_SYNC_H_
#define _ACCOUNT_SYNC_H_

#include "logger.h"

class AccountSync
{    
public:
    AccountSync();
    ~AccountSync();

    void sync(bool publish = true);
    void syncDepartment();
    void syncPermission();

};

#endif // _ACCOUNT_SYNC_H_