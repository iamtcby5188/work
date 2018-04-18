#include "productfavor_service.h"
#include "business/productfavor_business.h"
#include "include/fid.h"
#include "include/common.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"
#include "msg2db/msg_log_service.h"

ProductFavorService::ProductFavorService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{
}

void ProductFavorService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "ProductFavor.BondGroup") {
        if (subject == "model") 
            GetBondGroupList(msg, reply);
    }else if (method == "ProductFavor.BondGroupDetail"){
        if (subject == "model")
            GetBondGroupDetailList(msg, reply);
    }else if (method == "ProductFavor.BondByGroupId"){
        GetBondByGroupId(msg, reply);
    }else if (method == "ProductFavor.AddGroup"){
        AddGroup(msg, reply);
    }else if (method == "ProductFavor.UpdateGroup"){
        UpdateGroup(msg, reply);
    }else if (method == "ProductFavor.DeleteGroup"){
        DeleteGroup(msg, reply);
    }else if (method == "ProductFavor.AddAttentionBonds"){
        AddAttentionBonds(msg, reply);
    }
    
    if (msg->GetReplyTo().length() > 0) {   
        size_t len = -1;
        VariantMapCalcSize(reply.MapValue(), len);
        LOGGER_INFO("Message Size: " << len)

        bool flag = conn_->Send(msg->GetReplyTo(), reply);
        if (flag) {
            LOGGER_INFO("Reply success: " << msg->GetType())
        } else {
            LOGGER_ERROR("Reply failed: " << msg->GetType())
        }
    }
	MsgLogService::singleton()->FilterAndSaveMsg(*msg,reply);
}

void ProductFavorService::GetBondGroupList(const sdbus::Message* msg, sdbus::Message& reply)
{
    ProductFavorBusiness business(this);

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);


    BondGroupCacheVecPtr bondgroups = business.GetBondGroupList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondGroupCachePtr>::const_iterator it = bondgroups->begin();
    for ( ; it != bondgroups->end(); ++it) {
        BondGroupCache* cache = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        mp->SetString(FID_GROUP_ID, cache->id);
        mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
        mp->SetString(FID_USER_ACCOUNT_ID, cache->ua_id);
        mp->SetString(FID_GROUP_NAME, cache->group_name);
		mp->SetString(FID_GROUP_FILTER, cache->filter_condition);
    }
}

void ProductFavorService::GetBondGroupDetailList(const sdbus::Message* msg, sdbus::Message& reply)
{
	ProductFavorBusiness business(this);

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    BondGroupDetailCacheVecPtr bondgroups = business.GetBondGroupDetailList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondGroupDetailCachePtr>::const_iterator it = bondgroups->begin();
    for ( ; it != bondgroups->end(); ++it) {
        BondGroupDetailCache* cache = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        mp->SetString(FID_GROUP_ID, cache->group_id);
        mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    }
}

void ProductFavorService::GetBondByGroupId(const sdbus::Message* msg, sdbus::Message& reply)
{
    std::string groupid;

    msg->GetString(FID_GROUP_ID, groupid);
    BondKeyListedMarketVecPtr bondkeylistedmarkets = BondGroupDetailCacheController::singleton()->getBondByGroupId(groupid);

    LOGGER_INFO("BondByGroupId count: " << bondkeylistedmarkets->size())

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<std::string>::const_iterator it = bondkeylistedmarkets->begin();
    for ( ; it != bondkeylistedmarkets->end(); ++it) {

        sdbus::VariantMap *mp = list->AddMap();

        mp->SetString(FID_BOND_KEY_LISTED_MARKET, it->c_str());
    }
}

void ProductFavorService::AddGroup(const sdbus::Message* msg, sdbus::Message& reply)
{
    ProductFavorBusiness business(this);

    BondGroupCachePtr cache(new BondGroupCache());

    msg->GetString(FID_USER_ACCOUNT_ID,cache->ua_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID,cache->company_id);
    msg->GetString(FID_GROUP_NAME, cache->group_name);
	msg->GetString(FID_GROUP_FILTER, cache->filter_condition);

    cache->id = GetNewGuid();
	cache->status = "1";
	cache->ua_flag = "2";
	cache->create_time = GetCurrentTimeString();
    if(business.AddGroup(cache))
        reply.SetString(FID_GROUP_ID, cache->id);

}

void ProductFavorService::UpdateGroup(const sdbus::Message* msg, sdbus::Message& reply)
{
    ProductFavorBusiness business(this);

    BondGroupCachePtr cache(new BondGroupCache());
    msg->GetString(FID_GROUP_ID,cache->id);
    msg->GetString(FID_ACCOUNT_COMPANY_ID,cache->company_id);
    msg->GetString(FID_USER_ACCOUNT_ID,cache->ua_id);
    msg->GetString(FID_GROUP_NAME, cache->group_name);
	msg->GetString(FID_GROUP_FILTER, cache->filter_condition);

	cache->status = "1";
	cache->ua_flag = "2";
	cache->create_time = GetCurrentTimeString();
    business.UpdateGroup(cache);
}

void ProductFavorService::DeleteGroup(const sdbus::Message* msg, sdbus::Message& reply)
{
    ProductFavorBusiness business(this);

	std::string id;
	msg->GetString(FID_GROUP_ID, id);
    //BondGroupCachePtr cache(new BondGroupCache());
	//BondGroupCache cache(*BondGroupCacheController::singleton()->get(id.c_str()).get());

    BondGroupCachePtr cacheSrc = BondGroupCacheController::singleton()->get(id);
    if(!cacheSrc.get())
        return;
    BondGroupCachePtr cache(new BondGroupCache(*cacheSrc));

	//msg->GetString(FID_GROUP_ID,cache->id);
	//msg->GetString(FID_USER_ACCOUNT_ID,cache->ua_id);
	//msg->GetString(FID_GROUP_NAME, cache->group_name);
	//msg->GetString(FID_GROUP_FILTER, cache->filter_condition);
	cache->status = "2";
	//cache->ua_flag = "2";
	cache->create_time = GetCurrentTimeString();

	//BondGroupCachePtr cp(cache);
	//cp->id = cache.id;
	//cp->ua_id = cache.ua_id;
	//cp->group_name = cache.group_name;
	//cp->status = cache.status;
	//cp->create_time = cache.create_time;
	//cp->ua_flag = cache.ua_flag;
	//cp->filter_condition = cache.filter_condition;

	business.DeleteGroup(cache);

}

void ProductFavorService::AddAttentionBonds(const sdbus::Message* msg, sdbus::Message& reply)
{
    ProductFavorBusiness business(this);

    BondGroupCachePtr cache(new BondGroupCache());
    sdbus::Variant::List bondkeylistedmarkets;

    msg->GetString(FID_GROUP_ID,cache->id);
    msg->GetString(FID_ACCOUNT_COMPANY_ID,cache->company_id);
	msg->GetString(FID_GROUP_NAME, cache->group_name);
	msg->GetString(FID_USER_ACCOUNT_ID, cache->ua_id);
	msg->GetString(FID_GROUP_FILTER, cache->filter_condition);
    msg->GetList(FID_LIST,bondkeylistedmarkets);

    business.AddAttentionBonds(cache,bondkeylistedmarkets);
}

void ProductFavorService::SetBondGroupMap(const BondGroupCache *cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_GROUP_ID, cache->id);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
	mp->SetString(FID_USER_ACCOUNT_ID, cache->ua_id);
	mp->SetString(FID_GROUP_NAME, cache->group_name);
	mp->SetString(FID_GROUP_FILTER, cache->filter_condition);
}

void ProductFavorService::SetBondGroupDetailMap(const BondGroupDetailCache *cache, sdbus::VariantMap *mp)
{
	mp->SetString(FID_GROUP_ID, cache->id);
	mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
}

void ProductFavorService::PublishBondGroup(const std::vector<BondGroupCachePtr> &bond_group_vec, const char *action_type)
{
	sdbus::Message msg;
	sdbus::VariantList *list = msg.AddList(FID_LIST);    

	for (size_t index = 0; index != bond_group_vec.size(); ++index){
		sdbus::VariantMap *mp = list->AddMap();
		SetBondGroupMap(bond_group_vec.at(index).get(), mp);
	}    

	msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(bond_group_vec.size() > 0)
        companyId = bond_group_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    msg.SetType("ProductFavor.BondGroup");

    if(!companyId.empty())
        companyId = "." + companyId;
	bool flag = conn_->Publish(topic_name_ + companyId, msg);

	if (flag) {
		LOGGER_INFO("Publish Succeed BondGroup Vec")
	} else {
		LOGGER_ERROR("Publish Failed BondGroup Vec")
	}
}

void ProductFavorService::PublishBondGroupDetail(const std::vector<BondGroupDetailCachePtr> &bond_group_detail_vec, const char *action_type)
{
	sdbus::Message msg;
	sdbus::VariantList *list = msg.AddList(FID_LIST);    

	for (size_t index = 0; index != bond_group_detail_vec.size(); ++index){
		sdbus::VariantMap *mp = list->AddMap();
		SetBondGroupDetailMap(bond_group_detail_vec.at(index).get(), mp);
	}    

	msg.SetString(FID_ACTION_TYPE, action_type);

	msg.SetType("ProductFavor.BondGroupDetail");
	bool flag = conn_->Publish(topic_name_, msg);

	if (flag) {
		LOGGER_INFO("Publish Succeed BondGroupDetail Vec")
	} else {
		LOGGER_ERROR("Publish Failed BondGroupDetail Vec")
	}
}