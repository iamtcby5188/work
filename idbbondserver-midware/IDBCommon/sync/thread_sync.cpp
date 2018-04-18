#include "thread_sync.h"
#include "logger.h"

thread_sync* thread_sync::singleton_ = NULL;
thread_sync* thread_sync::singleton()
{
    if (singleton_ == NULL) {
        singleton_ = new thread_sync();
    }

    return singleton_;
}

thread_sync::thread_sync()
{

}

bool thread_sync::isProcessingBondKeyListedMarket(std::string company_id, std::string bond_key)
{
    if(best_quote_company_id_map.find(company_id) == best_quote_company_id_map.end())
        return false;

    return std::find(best_quote_company_id_map[company_id].begin(), best_quote_company_id_map[company_id].end(), bond_key) != best_quote_company_id_map[company_id].end();
}

bool thread_sync::isProcessingBondKeyListedMarket(std::string company_id, std::vector<std::string> bond_key_map)
{
    int count = 0;
    if(best_quote_company_id_map.find(company_id) != best_quote_company_id_map.end())
        count = best_quote_company_id_map[company_id].size();

    LOGGER_INFO("<<<<<--Size of bond_key in processing: [" << count << "]");
    if(count == 0)
        return false;

    for(std::vector<std::string>::iterator it = bond_key_map.begin(); it != bond_key_map.end(); ++it)
    {
        if(std::find(best_quote_company_id_map[company_id].begin(), best_quote_company_id_map[company_id].end(), *it) != best_quote_company_id_map[company_id].end()) {
            LOGGER_INFO("<<<<<--There is thread processing bond_key: [" << *it << "]");
            return true;
        }
    }
    return false;
}

void thread_sync::thread_sync_begin_best_quote_calc_and_publish(std::string company_id, std::vector<std::string> bond_key)
{
    LOGGER_INFO("<<<<<--thread sync begin...");
    boost::timed_mutex::scoped_timed_lock lock(best_quote_mutex_);

    while(isProcessingBondKeyListedMarket(company_id, bond_key)) {        
        LOGGER_INFO("<<<<<--begin wait other thread...");
        best_quote_cond_var_.wait(lock);
        LOGGER_INFO("<<<<<--ennnd wait other thread...");
    }

    if(best_quote_company_id_map.find(company_id) == best_quote_company_id_map.end()) {
        std::vector<std::string> vec;
        best_quote_company_id_map[company_id] = vec;
    }
    
    std::vector<std::string> &v = best_quote_company_id_map[company_id];
    for(std::vector<std::string>::iterator it = bond_key.begin(); it != bond_key.end(); ++it) {
        LOGGER_INFO("<<<<<--Processing bond_key begin: [" << *it << "]");
        if (std::find(v.begin(), v.end(), *it) == v.end())
            v.push_back(*it);
    }

    LOGGER_INFO("<<<<<--thread sync ennnd...");
    LOGGER_INFO("<<<<<--begin work...");
}

void thread_sync::thread_sync_ennnd_best_quote_calc_and_publish(std::string company_id, std::vector<std::string> bond_key)
{
    LOGGER_INFO("<<<<<--ennnd work...");
    LOGGER_INFO("<<<<<--thread sync begin...");
    boost::timed_mutex::scoped_timed_lock lock(best_quote_mutex_);

    if(best_quote_company_id_map.find(company_id) != best_quote_company_id_map.end()) {
        for(std::vector<std::string>::iterator it = bond_key.begin(); it != bond_key.end(); ++it)
        {
            std::vector<std::string>::iterator eraser_it = std::find(best_quote_company_id_map[company_id].begin(), 
                best_quote_company_id_map[company_id].end(), *it);
            if(eraser_it != best_quote_company_id_map[company_id].end()) {
                LOGGER_INFO("<<<<<--Process bond_key finished: [" << *it << "]");
                best_quote_company_id_map[company_id].erase(eraser_it);
            }
        }
    }

    best_quote_cond_var_.notify_all();
    LOGGER_INFO("<<<<<--thread sync ennnd...");
}


