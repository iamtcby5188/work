#ifndef _BONDQUOTE_SYNC_H_
#define _BONDQUOTE_SYNC_H_

#include "logger.h"

class BondQuoteSync 
{
public:
    BondQuoteSync();
    ~BondQuoteSync();

    void syncForBondChanged(const std::vector<std::string> &updated_bond_vec);
    //void syncForAccountChanged(const std::vector<std::string> &updated_account_vec);
    //void syncForBondTraderChanged(const std::vector<std::string> &updated_bondtrader_vec);
    //void syncForTradingInstitutionChanged(const std::vector<std::string> &updated_tradinginstitution_vec);

};

#endif // _BONDQUOTE_SYNC_H_