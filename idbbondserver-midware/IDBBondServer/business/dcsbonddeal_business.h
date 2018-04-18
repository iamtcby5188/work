#ifndef _DCSBONDDEAL_BUSINESS_H_
#define _DCSBONDDEAL_BUSINESS_H_

#include <vector>
#include <map>
#include "service/dcsbonddeal_service.h"
#include "object/bonddeal.h"
#include <time.h>
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016
#include <boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;


class DcsBondDealCache;

class DcsBondDealBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    DcsBondDealBusiness(DcsBondDealService* service);

    DcsBondDealCacheVecPtr FetchBondDeals(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range);
    DcsBondDealCacheVecPtr FetchBondDealsByDate(const std::string& cond, const std::string& datetime);

    DcsBondDealCacheVecPtr GetBondDealByID(const std::string& bond_deal_id);

    int GetBondDealCount(const sdbus::string& cond);
    
	// Assemble Bond Deal Msg from Bond Deal Cache
	void AssembleBondDeal(DcsBondDealCachePtr& bonddealcache, ptree* bond_deal_msg);

    bool AddBondDeal(const BondDeal& bond_deal, 
        sdbus::string* exported_text = NULL, 
        sdbus::string* bond_id = NULL, 
        ptree* bond_deal_msg_data = NULL, 
        bool manually_set_deal_id = false,
        bool ptree_is_not_list = false);

    bool UpdateBondDeal(const BondDeal& bond_deal, sdbus::string* exported_text = NULL,  ptree* bond_deal_msg = NULL);
	bool UpdateBondDeal(BondDeal& bond_deal, sdbus::string method, sdbus::string* exported_text = NULL, ptree* bond_deal_msg = NULL);
	bool DcsAddBondDeal(const DcsBondDealCachePtr& cache);
	bool DcsUpdateBondDeal(const DcsBondDealCacheVecPtr& vec);
	bool DcsUpdateFailMsg(std::string dcs_id, std::string msg, std::string operate_type, std::string operator_name);
	bool SubmitDeal(const std::vector<std::string>& id_vec);
	bool RelateSubmitDeal(const std::vector<std::string>& id_vec, std::string comments);
	bool DestroyDeal(const std::string& id, const std::string& reason);
	bool SetUrgentDeal(const std::vector<std::string>& id_vec, std::string urgent_status);
	bool GetHistoryDetailList(DcsHistoryDetailVecPtr& detail_vec, const std::string &deal_id);
	bool SetQuestionMark(const BondDeal& bond_deal);

    bool UpdateBondDeal(const BondDealVecPtr& vec, sdbus::string* exported_text = NULL, ptree* bond_deal_info_msg = NULL);

    bool DeleteBondDeal(const BondDealVecPtr& bond_deal_vec);

    sdbus::string CopyDeal(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDeal(sdbus::VariantList deal_ids);

private:
    static void UpdateBondDealCallBack(DcsBondDealCache* cache, void* param);
    static void UpdateBondDealSuccessed(DcsBondDealCache* cache, void* param);
    static void UpdateBondDealFailed(DcsBondDealCache* cache, void* param);

    sdbus::string CopyDealForTP(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForPATR(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForBGC(sdbus::VariantList deal_ids);
    sdbus::string CopyDealForICAP(sdbus::VariantList deal_ids);

    sdbus::string CopyDetailedDealForTP(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForPATR(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForBGC(sdbus::VariantList deal_ids);
    sdbus::string CopyDetailedDealForICAP(sdbus::VariantList deal_ids);

    void Init();

    bool AddDeal(const BondDeal& bond_deal,  
        sdbus::string* exported_text = NULL, 
        sdbus::string* bond_id = NULL, 
        ptree* bond_deal_msg = NULL, 
        bool manually_set_deal_id = false,
        bool ptree_is_not_list = false);
	int GetDealSequenceNumber(const DcsBondDealCachePtr& cache);

	std::string GetSpecialFlag(const DcsBondDealCachePtr& cache);
	std::string GetCheckLevel(const DcsBondDealCachePtr& cache);

    DcsBondDealService* service_;
	static boost::mutex add_deal_mutex_;

};

#endif // _BONDDEAL_BUSINESS_H_