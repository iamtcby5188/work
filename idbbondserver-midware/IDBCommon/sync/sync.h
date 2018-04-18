#ifndef _SYNC_H_
#define _SYNC_H_

#include <string>
#include <vector>
#include <log4cxx/logger.h>
#include "cache_def.h"

template<class C, class M, class T, class T_Ptr, class T_Vec_Ptr>
void db_sync(C& controller, M& monitor, const std::string& key, T_Vec_Ptr& vec,
    std::vector<T*>& update_vec, std::vector<T*>& add_vec, std::vector<T*>& delete_vec)
{
    std::map<std::string, T_Ptr>& monitor_mp = monitor.refMap();

    typename std::map<std::string, T_Ptr>::const_iterator monitor_iter = monitor_mp.begin();
    for (; monitor_iter != monitor_mp.end(); ++monitor_iter) {
        T_Ptr monitor_cache = (*monitor_iter).second;
        T* cache_new = monitor_cache.get();

        if (cache_new == NULL) {
            continue;
        }

        std::string str = key + " = '" + cache_new->getPrimaryKey() + "'";
        dbQuery q(str.c_str());
        T_Ptr cache = controller.getCacheByQueryInThreadSafty(q);

        if (cache) {
            // Update
            if (*cache_new != *cache) {
                controller.updateCacheInThreadSaftyWithoutDBSync(q, cache_new);
                update_vec.push_back(cache_new);
            }            
        } else {
            // Add
            controller.insertCacheInThreadSaftyWithoutDBSync(cache_new);
            add_vec.push_back(cache_new);
        }
    }

    typename std::vector<T_Ptr>::const_iterator iter = vec->begin();
    for (; iter != vec->end(); ++iter) {
        T_Ptr cache = *iter;

        T_Ptr monitor_cache = monitor.get(cache->getPrimaryKey());

        if (monitor_cache == NULL) {
            // Delete
            std::string str = key + " = '" + cache->getPrimaryKey() + "'"; 
            dbQuery q(str.c_str());
            controller.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
            delete_vec.push_back(cache.get());
        } 
    }
}

template<class C, class M, class T, class T_Ptr, class T_Vec_Ptr>
void valuation_sync(C& controller, M& monitor, const std::string& key, T_Vec_Ptr& vec,
	std::vector<T*>& update_vec, std::vector<T*>& add_vec, std::vector<T*>& delete_vec)
{
	std::map<std::string, T_Ptr>& monitor_mp = monitor.refMap();

	typename std::map<std::string, T_Ptr>::const_iterator monitor_iter = monitor_mp.begin();
	for (; monitor_iter != monitor_mp.end(); ++monitor_iter) {
		T_Ptr monitor_cache = (*monitor_iter).second;
		T* cache_new = monitor_cache.get();

		if (cache_new == NULL) {
			continue;
		}

		std::string str = key + " = '" + cache_new->getPrimaryKey() + "'";
		dbQuery q(str.c_str());
		T_Ptr cache = controller.getCacheByQueryInThreadSafty(q);

		if (cache) {
			// Update
			if (*cache_new != *cache) {
				update_vec.push_back(cache_new);
			}
		}
		else {
			// Add
			add_vec.push_back(cache_new);
		}
	}

	typename std::vector<T_Ptr>::const_iterator iter = vec->begin();
	for (; iter != vec->end(); ++iter) {
		T_Ptr cache = *iter;

		T_Ptr monitor_cache = monitor.get(cache->getPrimaryKey());

		if (monitor_cache == NULL) {
			// Delete
			delete_vec.push_back(cache.get());
		}
	}
}

template<class C, class M, class T, class T_Ptr, class T_Vec_Ptr>
void crm_sync(C& controller, M& monitor, const std::string& key, T_Vec_Ptr& vec,
	std::vector<T*>& update_vec, std::vector<T*>& add_vec, std::vector<T*>& delete_vec)
{
	std::map<std::string, T_Ptr>& monitor_mp = monitor.refMap();

	typename std::map<std::string, T_Ptr>::const_iterator monitor_iter = monitor_mp.begin();
    for (; monitor_iter != monitor_mp.end(); ++monitor_iter) {
		T_Ptr monitor_cache = (*monitor_iter).second;
		T* cache_new = monitor_cache.get();

		if (cache_new == NULL) {
			continue;
		}

		std::string str = key + " = '" + cache_new->getPrimaryKey() + "'";
		dbQuery q(str.c_str());
		T_Ptr cache = controller.getCacheByQueryInThreadSafty(q);

		if (cache) {
			// Update
			if (*cache_new != *cache) {
				controller.updateCacheInThreadSafty(q, cache_new);
				update_vec.push_back(cache_new);
			}
		}
		else {
			// Add
			controller.insertCacheInThreadSafty(cache_new);
			add_vec.push_back(cache_new);
		}
	}

	typename std::vector<T_Ptr>::const_iterator iter = vec->begin();
    for (; iter != vec->end(); ++iter) {
		T_Ptr cache = *iter;

		T_Ptr monitor_cache = monitor.get(cache->getPrimaryKey());

		if (monitor_cache == NULL) {
			// Delete
			std::string str = key + " = '" + cache->getPrimaryKey() + "'";
			dbQuery q(str.c_str());
			controller.deleteCacheByQueryInThreadSafty(q);
			delete_vec.push_back(cache.get());
		}
	}
}

template<class C, class M, class T, class T_Ptr, class T_Vec_Ptr>
void dcs_sync(C& controller, M& monitor, const std::string& key, T_Vec_Ptr& vec,
	std::vector<T_Ptr>& update_vec, std::vector<T_Ptr>& add_vec, std::vector<T_Ptr>& delete_vec)
{
	std::map<std::string, T_Ptr>& monitor_mp = monitor.refMap();

	typename std::map<std::string, T_Ptr>::const_iterator monitor_iter = monitor_mp.begin();
    for (; monitor_iter != monitor_mp.end(); ++monitor_iter) {
		T_Ptr cache_new = (*monitor_iter).second;

		if (!cache_new) {
			continue;
		}

		std::string str = key + " = '" + cache_new->getPrimaryKey() + "'";
		dbQuery q(str.c_str());
		T_Ptr cache = controller.getCacheByQueryInThreadSafty(q);

		if (cache) {
			// Update
			if (*cache_new != *cache) {
				controller.updateCacheInThreadSaftyWithoutDBSync(q, cache_new.get());
				update_vec.push_back(cache_new);
			}
		}
		else {
			// Add
			controller.insertCacheInThreadSaftyWithoutDBSync(cache_new.get());
			add_vec.push_back(cache_new);
		}
	}

	typename std::vector<T_Ptr>::const_iterator iter = vec->begin();
    for (; iter != vec->end(); ++iter) {
		T_Ptr cache = *iter;

		T_Ptr monitor_cache = monitor.get(cache->getPrimaryKey());

		if (!monitor_cache) {
			// Delete
			std::string str = key + " = '" + cache->getPrimaryKey() + "'";
			dbQuery q(str.c_str());
			controller.deleteCacheByQueryInThreadSaftyWithoutDBSync(q);
			delete_vec.push_back(cache);
		}
	}
}

#endif // _SYNC_H_