#ifndef _PRODUCTFAVOR_SERVICE_H_
#define _PRODUCTFAVOR_SERVICE_H_

#include "service/service.h"
#include "cache/controller/bondgroup_cachecontroller.h"
#include "cache/controller/bondgroupdetail_cachecontroller.h"

class ProductFavorService : public Service
{
public:
    //virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
    virtual void processReqMsg(const sdbus::Message* msg);

	static void SetBondGroupMap(const BondGroupCache *cache, sdbus::VariantMap *mp);
	static void SetBondGroupDetailMap(const BondGroupDetailCache *cache, sdbus::VariantMap *mp);
	void PublishBondGroup(const std::vector<BondGroupCachePtr> &bond_group_vec, const char *action_type);
	void PublishBondGroupDetail(const std::vector<BondGroupDetailCachePtr> &bond_group_vec, const char *action_type);
	
private:
    friend class ServiceManager;
    ProductFavorService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name);

    void GetBondGroupList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondGroupDetailList(const sdbus::Message* msg, sdbus::Message& reply);
    void GetBondByGroupId(const sdbus::Message* msg, sdbus::Message& reply); 
    void AddGroup(const sdbus::Message* msg, sdbus::Message& reply);
    void UpdateGroup(const sdbus::Message* msg, sdbus::Message& reply);
    void DeleteGroup(const sdbus::Message* msg, sdbus::Message& reply);
    void AddAttentionBonds(const sdbus::Message* msg, sdbus::Message& reply);

};

#endif // _BONDSERVICE_H_