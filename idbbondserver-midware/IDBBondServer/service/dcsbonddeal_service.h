#ifndef _DCSBONDDEAL_SERVICE_H_
#define _DCSBONDDEAL_SERVICE_H_

#include "service/service.h"
#include "object/bonddeal.h"
#include "object/DcsHistoryDetail.h"
#include "cache/controller/dcsbonddeal_cachecontroller.h"

class DcsBondDealService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);
   
    static void SetBondDealMap(const DcsBondDealCache *cache, sdbus::VariantMap *mp);
    
    void PublishBondDeal(const std::vector<DcsBondDealCachePtr> &bond_deal_vec, const char* action_type, bool without_data = true); // change default to true

	void PublishDcsFailMsg(sdbus::Message& message);

private:
    friend class ServiceManager;
	DcsBondDealService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

	void SyncBondDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void AddBondDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateBondDeal(const sdbus::Message* msg, sdbus::Message& reply);
	void UpdateBondDealStatus(const sdbus::Message* msg, sdbus::Message& reply);
	void SubmitDeal(const sdbus::Message* msg, sdbus::Message& reply);
	void RelateSubmitDeal(const sdbus::Message* msg, sdbus::Message& reply);
	void DestroyDeal(const sdbus::Message* msg, sdbus::Message& reply);
	void SetUrgentDeal(const sdbus::Message* msg, sdbus::Message& reply);
	void GetHistoryDetailList(const sdbus::Message* msg, sdbus::Message& reply);
	void SetQuestionMark(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateDealInfo(const sdbus::Message* msg, sdbus::Message& reply);
	void UpdateDealInfoUndo(const sdbus::Message* msg, sdbus::Message& reply);

    void DeleteBondDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondDealByID(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondDealList(const sdbus::Message* msg, sdbus::Message& reply);

    void GetBondDealListInc(const sdbus::Message* msg, sdbus::Message& reply);

    void CopyDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void CopyDetailedDeal(const sdbus::Message* msg, sdbus::Message& reply);
    void ExportDealData(const sdbus::Message* msg, sdbus::Message& reply);

	void SetHistoryDetailMap(const DcsHistoryDetail *detail, sdbus::VariantMap *mp);

};

#endif // _DCSBONDDEAL_SERVICE_H_