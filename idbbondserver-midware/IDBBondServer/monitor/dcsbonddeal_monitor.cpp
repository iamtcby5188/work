#include "dcsbonddeal_monitor.h"
#include "cache/cache_center.h"
#include "DCSConnectionService.h"

DCSBondDealMonitor::DCSBondDealMonitor()
    : Monitor()
{

}

bool DCSBondDealMonitor::init()
{
	if (!kDcsEnable) return false;

	DCSConnectionService::singleton()->getSwitch();

	int total_row_counts = 0;	  //总记录数，每次请求都返回总记录数
	int page_num = 0;			  //标记分页次数，设置分页偏移
	int page_size = kDcsPageSize; //每次请求DCS成交单数据的条数
	int ncounts = 0;			  //返回结果数
	bool success = true;

	do
	{
		DcsBondDealCacheVecPtr deal_vec(new std::vector<DcsBondDealCachePtr>());
		if (DCSConnectionService::singleton()->queryContractList(deal_vec, total_row_counts, page_num, page_size))
		{
			ncounts += deal_vec->size();

			std::vector<DcsBondDealCachePtr>::const_iterator it = deal_vec->begin();
			for (; it != deal_vec->end(); ++it)
			{
				load(*it);
			}

			++page_num;

			LOGGER_DEBUG("++++++ DCSBondDealMonitor Requested Rows: " << ncounts);
			LOGGER_DEBUG("++++++ DCSBondDealMonitor Total Rows: " << total_row_counts << ", Page Number: " << page_num);
		}
		else
		{
			success = false;
		}

	} while (total_row_counts != 0 && total_row_counts > ncounts && ncounts >= page_num * page_size);
	
	if ((total_row_counts == ncounts) && success)
	{
		return true;
	}
	else
	{
		return false;
	}
}

DcsBondDealCachePtr DCSBondDealMonitor::get(const std::string &bond_key_listed_market) const
{
    std::map<std::string, DcsBondDealCachePtr>::const_iterator it = bond_deal_mp_.find(bond_key_listed_market);

	if (it != bond_deal_mp_.end()) {
        return (*it).second;
    }
	return DcsBondDealCachePtr();
}

int DCSBondDealMonitor::size() const
{
	return bond_deal_mp_.size();
}

std::map<std::string, DcsBondDealCachePtr>& DCSBondDealMonitor::refMap()
{
	return bond_deal_mp_;
}

void DCSBondDealMonitor::load(DcsBondDealCachePtr cache)
{
	if (NULL == cache) return;

	std::string id = cache->id;
	if (id.length() == 0) return;

	CacheCenter::sharedInstance()->AttachCurrentThread();
	cache->loadCombinationFields();
	CacheCenter::sharedInstance()->DetachCurrentThread();

	bond_deal_mp_[id] = cache;
}
