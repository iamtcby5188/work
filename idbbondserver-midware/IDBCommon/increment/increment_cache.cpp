#include "increment_cache.h"

IncrementCache::IncrementCache(const std::string &model_name, unsigned int max_size)
    : model_name_(model_name), max_size_(max_size)
{

}

void IncrementCache::Add(IncrementDataPtr increment_data)
{
	if (!increment_data.get()) {
		LOGGER_WARN("data error, no cache!!!");
		return;
	}
    LOGGER_INFO("Increment Cache Info begin")
    boost::mutex::scoped_lock lock(mutex_);

    if (increment_data_list_.size() >= max_size_) {
        increment_data_list_.pop_back();
    }
    increment_data_list_.push_front(increment_data);

    LOGGER_INFO("Increment Cache Info") 
    LOGGER_INFO("[model_name]: " << model_name_ << ", [size]: " << increment_data_list_.size())
}
   
void IncrementCache::Add(int version, const sdbus::Message &msg)
{
    Add(IncrementDataPtr(new IncrementData(version, msg)));
}

bool IncrementCache::Get(int version, sdbus::Message &msg) 
{
    boost::mutex::scoped_lock lock(mutex_);
	if (increment_data_list_.empty())
		return false;
    std::list<IncrementDataPtr>::const_iterator it = increment_data_list_.begin();
    for ( ; it != increment_data_list_.end(); ++it) {
        if ((*it).get() && (*it)->version_ == version) {
            msg = (*it)->msg_;
            return true;
        }
    }

    return false;
}