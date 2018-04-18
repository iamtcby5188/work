#ifndef _BONDBESTQUOTE_SYNC_H_
#define _BONDBESTQUOTE_SYNC_H_

#include "logger.h"

class BondBestQuoteSync 
{
public:
    BondBestQuoteSync();
    ~BondBestQuoteSync();

    void syncForBondChanged(const std::vector<std::string> &updated_bond_vec);
    //void syncForQuoteChanged(const std::vector<std::string> &updated_quote_vec);

};

#endif // _BONDBESTQUOTE_SYNC_H_