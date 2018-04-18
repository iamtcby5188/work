#ifndef _INCREMENT_CACHE_H_
#define _INCREMENT_CACHE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif
#include "logger.h"
#include "sdbus/connection.h"
#include <list>
#include "boost/thread/mutex.hpp"

class IncrementData
{
public:
    IncrementData(int version, const sdbus::Message &msg) : version_(version), msg_(msg) {}

    int version_;
    sdbus::Message msg_;
};

class IncrementCache
{
public:
    typedef std::tr1::shared_ptr<IncrementData> IncrementDataPtr;

    IncrementCache(const std::string &model_name, unsigned int max_size);
    
    void Add(IncrementDataPtr increment_data);
    void Add(int version, const sdbus::Message &msg);
    
    bool Get(int version, sdbus::Message &msg);

private:
    std::string model_name_;
    unsigned int max_size_;
    std::list<IncrementDataPtr> increment_data_list_; 
    
    boost::mutex mutex_;

};

#endif // _INCREMENT_CACHE_H_
