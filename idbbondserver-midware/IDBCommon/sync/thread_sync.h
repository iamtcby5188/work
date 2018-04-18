#ifndef _THREAD_SYNC_H_
#define _THREAD_SYNC_H_

#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"

class thread_sync
{
public:
    static thread_sync* singleton();
    thread_sync();

    // best quote
    boost::timed_mutex best_quote_mutex_;
    boost::condition_variable_any best_quote_cond_var_;
    std::map<std::string, std::vector<std::string>> best_quote_company_id_map;

    bool isProcessingBondKeyListedMarket(std::string company_id, std::string bond_key_listed_market);
    bool isProcessingBondKeyListedMarket(std::string company_id, std::vector<std::string> bond_key_listed_market_vec);

    void thread_sync_begin_best_quote_calc_and_publish(std::string company_id, std::vector<std::string> bond_key);
    void thread_sync_ennnd_best_quote_calc_and_publish(std::string company_id, std::vector<std::string> bond_key);

private:
    static thread_sync* singleton_;

};

#endif // _THREAD_SYNC_H_