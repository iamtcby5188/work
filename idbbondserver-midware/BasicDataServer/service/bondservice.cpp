#include "bondservice.h"
#include "business/bondbusiness.h"
#include "include/fid.h"
#include "include/common.h"
#include "sdbus/connection.h"
#include "sdbus/codec.h"
#include "cache/controller/bondsmallview_cachecontroller.h"
#include "cache/cache_controller_declear.h"
#include "cache/controller/account_cachecontroller.h"
#include "cache/controller/brokerbindingtrader_cachecontroller.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "service/publish_manager.h"
#include "increment/increment_cache_manager.h"
#include "sync/bondsmallview_sync.h"
#include "msg2db/msg_log_service.h"
#include "boost/regex.hpp"
#include <boost/locale/encoding.hpp>
#include "CRMConnectionService.h"
#include "sync/tradinginstitution_sync.h"
#include "sync/bondtrader_sync.h"
#include "sync/brokerbindingtrader_sync.h"

BondService::BondService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{

}

void BondService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply, zip_reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "BondInfo.BondSmallView") {    //全量的供搜索用
        if (subject == "model") {
			GetBondSmallViewList(msg, reply);
			reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
		} else if (subject == "increment") {
			GetBondSmallViewInc(msg, reply);
		}
	} else if (method == "BondDetail.BondSmallViewDetail") {    //全量的供搜索用
        if (subject == "model") {
			GetBondSmallViewDetailList(msg, reply);
			reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
		} else if (subject == "increment") {
			GetBondSmallViewDetailInc(msg, reply);
		}
    } else if (method == "Bond.GetBondInfoByCondition" || method == "Bond.GetBondForGroupSearch") {      
		if (subject == "model")	{
			GetBondInfoByCondition(msg, reply);
			reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
		} else if (subject == "increment") {
			GetBondInfoByConditionInc(msg, reply);
		}        
    } else if (method == "Bond.GetBondInfoByID") {
        GetBondInfoByID(msg, reply);
        reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
    } else if (method == "Bond.InstitutionInfo") {		//全量的供搜索用
		if (subject == "model")	{
			GetInstitutionInfoList(msg, reply);
			reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
		} else if (subject == "increment") {
			GetInstitutionInfoListInc(msg, reply);
		}
    } else if (method == "Bond.GetInstitutionInfoByID") {
        GetInstitutionInfoByID(msg, reply);
    } else if (method == "Bond.TraderInfo") {  //全量的供搜索用
		if (subject == "model")	{			
			GetTraderInfoList(msg, reply);
			reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
		} else if (subject == "increment") {
			GetTraderInfoListInc(msg, reply);
		}		
    } else if (method == "Bond.GetTraderInfoByID") {
        GetTraderInfoByID(msg, reply);
    } else if (method == "Bond.AccountInfo") {  //全量的供搜索用
        if (subject == "model")	{			
			GetAccountInfoList(msg, reply);
			reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
		} else if (subject == "increment") {
			GetAccountInfoListInc(msg, reply);
		}	
	} else if (method == "Bond.GetAccountInfoById") {
        GetAccountInfoByID(msg, reply);
	} else if (method == "Bond.UpdateAccountInfoById") { 
		UpdateAccountInfoByID(msg, reply);
	} else if (method == "Bond.UpdateAccountPasswordById") { 
		UpdateAccountPasswordByID(msg, reply);
	} else if (method == "Bond.BrokerBindingTrader") {
        if (subject == "model")	{			
			GetBrokerBindingTraderList(msg, reply);
			reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
		} else if (subject == "increment") {
			GetBrokerBindingTraderListInc(msg, reply);
		}	
    } else if (method == "Bond.BondDeviatedValue") {
        if (subject == "model")	{			
            GetBondDeviatedValueList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
        } else if (subject == "increment") {
            GetBondDeviatedValueListInc(msg, reply);
        }		
    /*}  else if (method == "Bond.BondValuation") {
        if (subject == "model")	{			
            GetBondValuationList(msg, reply);
            reply.SetInt32(FID_PUSH_MESSAGE_VERSION, PublishManager::singleton()->getVersion(method.c_str(), msg));
        }	*/
    } else if (method == "Bond.GetBondIssueYearBound") {
		GetBondIssueYearBound(msg, reply);
    } else if (method == "Bond.GetSwSectorInfoList") {
		GetSwSectorInfoList(msg, reply);
    } else if (method == "Bond.GetProvinceInfoList") {
        GetProvinceInfoList(msg, reply);
    } else if (method == "Bond.IssueInstitution") {
        if (subject == "model")	{
            GetIssuingInstitutionList(msg, reply);
        }
    } else if (method == "Bond.GetBondValuation") {
        GetBondValuation(msg, reply);
    } else if (method == "Bond.createOrGetAndPublishInstitutionTraderInfo") {
        createOrGetAndPublishInstitutionTraderInfo(msg, reply);
    } else if (method == "Bond.ManualSyncCRM") { //手动重新请求同步CRM机构、联系人、联系人账户关联数据
		//向CRM发送异步请求，获取机构和交易员信息
		CRMConnectionService::singleton()->queryInstitutionList();

		LOGGER_DEBUG("------ Bond.ManualSyncCRM");
	} else if (method == "Bond.ManualReloadCacheCRM") { //手动重新加载本地机构、联系人、联系人账户关联数据
		LOGGER_DEBUG("------ Bond.ManualReloadCacheCRM START");

		TradingInstitutionSync inst_sync;
		inst_sync.sync();

		BondTraderSync bond_trader_sync;
		bond_trader_sync.sync();

		BrokerBindingTraderSync broker_bind_trader_sync;
		broker_bind_trader_sync.sync();

		LOGGER_DEBUG("------ Bond.ManualReloadCacheCRM END");
	} else if (method == "Bond.GetTreasuryFuture") {
		GetTreasuryFuture(msg, reply);
	}
    
    if (msg->GetReplyTo().length() > 0) {
        size_t len = -1;
        VariantMapCalcSize(reply.MapValue(), len);
        LOGGER_INFO("Message Size: " << len)

        bool flag;
        if (zipMessage(reply, zip_reply)) {
            zip_reply.SetType(msg->GetType());
            zip_reply.SetSubject(msg->GetSubject());
            zip_reply.SetMessageID(msg->GetMessageID());

            VariantMapCalcSize(zip_reply.MapValue(), len);
            LOGGER_INFO("Zip Message Size: " << len)
            
            flag = conn_->Send(msg->GetReplyTo(), zip_reply);
        } else {           
            flag = conn_->Send(msg->GetReplyTo(), reply);
        }

        if (flag) {
            LOGGER_INFO("Reply success: " << msg->GetType())
        } else {
            LOGGER_ERROR("Reply failed: " << msg->GetType())
        }
    }
	MsgLogService::singleton()->FilterAndSaveMsg(*msg,reply);
}

void BondService::SetBondSmallViewMap(const BondSmallViewCache* cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_BOND_CODE, cache->bond_id);
    mp->SetString(FID_BOND_SHORT_NAME, cache->short_name);
    mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->SetString(FID_ISSUER_RATING, cache->issuer_rating_current);
    mp->SetString(FID_PIN_YIN, cache->pin_yin);
    mp->SetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->SetString(FID_ISSUE_INSTITUTION, cache->issuer_client);
    mp->SetString(FID_ISSUE_INSTITUTION_PINYIN, cache->issue_institution_pinyin_);
    mp->SetString(FID_ISSUE_INSTITUTION_PINYINFULL, cache->issue_institution_pinyinfull_);
    mp->SetString(FID_BOND_CATEGORY, cache->product_type);
    mp->SetBool(FID_BOND_IS_EXPIRED, cache->is_expired);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
}

void BondService::SetBondInfoMap(const BondSmallViewCache* cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_BOND_CODE, cache->bond_id);
    mp->SetString(FID_BOND_KEY, cache->bond_key);
    mp->SetString(FID_BOND_SHORT_NAME, cache->short_name);
    mp->SetString(FID_LISTED_MARKET, cache->listed_market);
    mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
    mp->SetString(FID_ISSUER_RATING, cache->issuer_rating_current);
	mp->SetInt32(FID_ISSUER_RATING_ORDER, cache->issuer_rating_order);
    mp->SetString(FID_PIN_YIN, cache->pin_yin);
    mp->SetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
    mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
    mp->SetString(FID_UNDER_WRITER_NAME, cache->underwriter_client);
    mp->SetDouble(FID_ISSUE_AMOUNT, cache->issue_amount);
    mp->SetDouble(FID_COUPON_RATE, cache->coupon_rate_current);
    mp->SetInt32(FID_MATURITY, cache->maturity_date);
    mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
    mp->SetDouble(FID_TIME_TO_MATURITY_REAL,cache->time_to_maturity_real);
    mp->SetString(FID_RATE_TYPE, cache->coupon_type_client);
    mp->SetString(FID_BOND_TYPE, cache->goods_type_client);
    mp->SetString(FID_OPTION_TYPE, cache->option_client);
    mp->SetString(FID_ISSUE_INSTITUTION, cache->issuer_client);
    mp->SetString(FID_ISSUE_INSTITUTION_PINYIN, cache->issue_institution_pinyin_);
    mp->SetString(FID_ISSUE_INSTITUTION_PINYINFULL, cache->issue_institution_pinyinfull_);
    mp->SetString(FID_BOND_CATEGORY, cache->product_type);
    mp->SetString(FID_HAS_OPTION, cache->has_option);
    mp->SetString(FID_MARKET_TYPE, cache->mkt_type);
    mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
    mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
}

//IDBBondServer从basicdataserver取值setmap
void BondService::SetBondSmallViewDetailMap(const BondSmallViewCache* cache, sdbus::VariantMap *mp)
{
	mp->SetString(FID_LISTED_MARKET, cache->listed_market);
	mp->SetString(FID_BOND_KEY_LISTED_MARKET, cache->bond_key_listed_market);
	mp->SetString(FID_BOND_CODE, cache->bond_id);
	mp->SetString(FID_BOND_SUBTYPE, cache->bond_subtype);
	mp->SetString(FID_ISSUER_RATING, cache->issuer_rating_current);
	mp->SetString(FID_BOND_SHORT_NAME, cache->short_name);
	mp->SetString(FID_COUPON_TYPE, cache->coupon_type);
	mp->SetString(FID_OPTION_TYPE, cache->option_client);
	mp->SetString(FID_PIN_YIN, cache->pin_yin);
	mp->SetString(FID_PIN_YIN_FULL, cache->pin_yin_full);
	mp->SetDouble(FID_COUPON_RATE, cache->coupon_rate_current);
	mp->SetInt32(FID_MATURITY, cache->maturity_date);
    mp->SetInt32(FID_INTEREST_START_DATE, cache->interest_start_date);
	mp->SetInt32(FID_LISTED_DATE, cache->listed_date);
	mp->SetInt32(FID_REDEMPTION_NO, cache->redemption_no);
	mp->SetString(FID_ENTERPRISE_TYPE, cache->institution_type);
	mp->SetString(FID_BOND_RATING, cache->rating_current);
	mp->SetString(FID_BOND_KEY, cache->bond_key);
	mp->SetString(FID_MARKET_TYPE, cache->mkt_type);
	mp->SetInt32(FID_OPTION_DATE, cache->option_date);
	mp->SetDouble(FID_ISSUE_AMOUNT, cache->issue_amount);
	mp->SetString(FID_ISSUER_RATING_NPY, cache->issuer_rating_current_npy);
	mp->SetString(FID_BOND_RATING_NPY, cache->rating_current_npy);

    mp->SetString(FID_REDEMPTION_NO_DESCRIPTION, cache->redemption_no_client);
    mp->SetString(FID_BOND_TYPE, cache->goods_type_client);
    mp->SetString(FID_BOND_CATEGORY, cache->product_type);
    mp->SetBool(FID_BOND_IS_EXPIRED, cache->is_expired);

	// ----------------- Combination Fields ----------------------
	//
    mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->cdc_valuation_yield);
    mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->cdc_valuation_net_price);

    mp->SetString(FID_GUARANTEE_TYPE, cache->rating_augment_client);
    mp->SetString(FID_RATE_TYPE, cache->coupon_type_client);
    mp->SetString(FID_RATING_INSTITUTION, cache->rating_institution_client);
    mp->SetString(FID_ISSUE_INSTITUTION, cache->issuer_client);
    mp->SetString(FID_ISSUE_INSTITUTION_PINYIN, cache->issue_institution_pinyin_);
    mp->SetString(FID_ISSUE_INSTITUTION_PINYINFULL, cache->issue_institution_pinyinfull_);
    mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
    mp->SetString(FID_ID, cache->issue_institution_id);
    mp->SetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);
    mp->SetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
	// ------------------------- Combined Fields (above) ----------------------------
	mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
	mp->SetDouble(FID_TIME_TO_MATURITY_REAL, cache->time_to_maturity_real);
	mp->SetString(FID_HAS_OPTION, cache->has_option);
	mp->SetString(FID_ISSUER_RATING_FILTER, cache->issuer_rating_filter);
	mp->SetString(FID_GUARENTEED, cache->guarenteed);
	mp->SetString(FID_MUNICIPAL, cache->municipal); 
	mp->SetString(FID_RATE_INDEX, cache->rate_index);
	mp->SetString(FID_PLEDGED, cache->pledged);
	mp->SetString(FID_ACROSS_MARKET, cache->across_market);
	mp->SetInt32(FID_ISSUE_YEAR, cache->issue_year);
	mp->SetString(FID_SW_SECTOR_UPPER_CODE, cache->sw_sector_code);
    mp->SetString(FID_SW_SECTOR_CODE, cache->sw_subsector_code);
    mp->SetString(FID_ISSUE_PROVINCE, cache->province);
    mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);
    mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id); // issuing institution
    mp->SetString(FID_COMPLETED_BOND_TYPE, cache->completed_bond_type);
    mp->SetInt32(FID_COMPLETED_QUOTE_SORT, cache->bond_type_sort_client);
    mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
	mp->SetString(FID_COMPANY_BOND, cache->ent_cor);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
}

void BondService::SetInstitutionInfoMap(const TradingInstitutionCache* cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->SetString(FID_INSTITUTION_CODE, cache->code);
    mp->SetString(FID_INSTITUTION_NAME, cache->name);
    mp->SetString(FID_PIN_YIN, cache->pinyin);
    mp->SetString(FID_PIN_YIN_FULL, cache->pinyin_full);
	mp->SetString(FID_INSTITUTION_FULL_NAME, cache->full_name);
	mp->SetString(FID_INTERNAL_INSTITUTION_TYPE, cache->attribute2);
	mp->SetString(FID_MODIFY_TIME, cache->modify_time=="" ? cache->create_time : cache->modify_time);
	mp->SetString(FID_CITY_NAME, cache->city_name);
}

void BondService::SetTraderInfoMap(const BondTraderCache* cache, sdbus::VariantMap *mp)
{   
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
	mp->SetString(FID_INSTITUTION_CODE, cache->code);
    mp->SetString(FID_TRADER_CODE, cache->f_code);
    mp->SetString(FID_TRADER_NAME, cache->name);
    mp->SetString(FID_PIN_YIN, cache->pinyin);
    mp->SetString(FID_PIN_YIN_FULL, cache->pinyin_full);
    mp->SetBool(FID_VIP_FLAG, cache->flag_vip);
    mp->SetBool(FID_BAD_FLAG, cache->flag_bad);
    mp->SetString(FID_TRADABLE_BOND_CATEGORY, cache->attribute4);
    mp->SetString(FID_INSTITUTION_ID, cache->bank_id);
	mp->SetString(FID_MODIFY_TIME, cache->modify_time=="" ? cache->create_time : cache->modify_time);
    mp->SetString(FID_TRADER_BARGAIN, cache->attribute3);
}

void BondService::SetAccountInfoMap(const AccountCache* cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_LOGIN_NAME, cache->username);
    mp->SetString(FID_USER_NAME, cache->display_name);
    mp->SetString(FID_ACCOUNT_PASSWORD, cache->password);
	mp->SetString(FID_ACCOUNT_CODE, cache->account_code);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
	mp->SetString(FID_ACCOUNT_COMPANY_NAME, cache->company_name);
	mp->SetString(FID_ACCOUNT_DEPARTMENT, cache->department_codes);
    mp->SetString(FID_ACCOUNT_ROLE, cache->role_codes);
    mp->SetString(FID_ACCOUNT_PERMISSION, cache->permission_codes);
    mp->SetString(FID_PIN_YIN, cache->pinyin);
	mp->SetString(FID_MODIFY_TIME, cache->modify_time=="" ? cache->create_time : cache->modify_time);
	
	mp->SetString(FID_ACCOUNT_TELEPHONE, cache->telephone);
	mp->SetString(FID_ACCOUNT_PHONE, cache->mobile);
	mp->SetString(FID_ACCOUNT_EMAIL, cache->email);
	mp->SetString(FID_ACCOUNT_RM, cache->rm);
	mp->SetString(FID_ACCOUNT_MSN, cache->msn);
	mp->SetString(FID_ACCOUNT_QQ, cache->qq);
	mp->SetString(FID_ACCOUNT_ADDRESS, cache->address);
	mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
	mp->SetString(FID_ACCOUNT_COMPANY_NAME, cache->company_name);
}

void BondService::SetBrokerBindingTraderMap(const BrokerBindingTraderCache* cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_BROKER_ID, cache->account_id);
    mp->SetString(FID_TRADER_ID, cache->bank_agent_id);
	mp->SetString(FID_ACCOUNT_DEPARTMENT, cache->account_department);
	mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
}

void BondService::SetBondDeviatedValueMap(const ChinaBondDeviatedValueCache* cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->SetString(FID_BOND_TYPE_ID, cache->bond_type_id);
    mp->SetString(FID_BOND_TYPE, cache->bond_type_name);
    mp->SetDouble(FID_DIFF_WITH_VALUATION, cache->deviated_value);
    mp->SetString(FID_BOND_TYPE_STATUS, cache->status);
    mp->SetString(FID_BOND_TYPE_MODIFY_TIME, cache->modify_time);
}
/*
void BondService::SetBondValuationMap(const ChinaBondValuationCache* cache, sdbus::VariantMap *mp)
{
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_BOND_CODE, cache->bond_id);
    mp->SetString(FID_CDC_VALUATIONS_DATE, cache->valuation_date);
    mp->SetString(FID_CDC_VALUATIONS_YIELD, cache->valuation_yield);
    mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, cache->valuation_net_price);
    mp->SetString(FID_DEL_FLAG, cache->status);
}*/

void BondService::PublishBondSmallView(const std::vector<BondSmallViewCache*> &bond_vec, const char* action_type)
{
    sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index < bond_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBondSmallViewMap(bond_vec.at(index), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    PublishManager::singleton()->publish(topic_name_, "BondInfo.BondSmallView", msg);
}

void BondService::PublishBondSmallViewDetail(const std::vector<BondSmallViewCache*> &bond_vec, const char* action_type)
{
    sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index < bond_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBondSmallViewDetailMap(bond_vec.at(index), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    PublishManager::singleton()->publish(kTopicBondDetail, "BondDetail.BondSmallViewDetail", msg);
}

void BondService::PublishBondInfo(const std::vector<BondSmallViewCache*> &bond_vec, const char* action_type)
{
    sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index != bond_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBondInfoMap(bond_vec.at(index), mp);
    }

    msg.SetString(FID_ACTION_TYPE, action_type);

    PublishManager::singleton()->publish(topic_name_, "Bond.GetBondInfoByCondition", msg);
}

void BondService::PublishInstitutionInfo(const std::vector<TradingInstitutionCache*> &institution_vec, const char* action_type)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index != institution_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetInstitutionInfoMap(institution_vec.at(index), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(institution_vec.size() > 0)
        companyId = institution_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    PublishManager::singleton()->publish(topic_name_, "Bond.InstitutionInfo", msg);*/
	std::map<std::string, sdbus::VariantList> tMap;
	for(size_t index = 0; index < institution_vec.size(); ++index){
		TradingInstitutionCache* cache = institution_vec.at(index);
		if(cache->company_id.length() < 1){
			continue;
		}
		if(tMap.count(cache->company_id) < 1){
			sdbus::VariantList list;
			tMap[cache->company_id] = list;
		}
		sdbus::VariantMap *mp = tMap[cache->company_id].AddMap();
		SetInstitutionInfoMap(cache, mp);
	}
	std::map<std::string, sdbus::VariantList>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message msg;
		msg.SetString(FID_ACTION_TYPE, action_type);
		msg.SetString(FID_ACCOUNT_COMPANY_ID, iter->first);
		msg.SetList(FID_LIST, iter->second);
		PublishManager::singleton()->publish(topic_name_, "Bond.InstitutionInfo", msg);
	}
}

void BondService::PublishTraderInfo(const std::vector<BondTraderCache*> &trader_vec, const char* action_type)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index != trader_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetTraderInfoMap(trader_vec.at(index), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(trader_vec.size() > 0)
        companyId = trader_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    PublishManager::singleton()->publish(topic_name_, "Bond.TraderInfo", msg);*/
	std::map<std::string, sdbus::VariantList> tMap;
	for(size_t index = 0; index < trader_vec.size(); ++index){
		BondTraderCache* cache = trader_vec.at(index);
		if(cache->company_id.length() < 1){
			continue;
		}
		if(tMap.count(cache->company_id) < 1){
			sdbus::VariantList list;
			tMap[cache->company_id] = list;
		}
		sdbus::VariantMap *mp = tMap[cache->company_id].AddMap();
		SetTraderInfoMap(cache, mp);
	}
	std::map<std::string, sdbus::VariantList>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message msg;
		msg.SetString(FID_ACTION_TYPE, action_type);
		msg.SetString(FID_ACCOUNT_COMPANY_ID, iter->first);
		msg.SetList(FID_LIST, iter->second);
		PublishManager::singleton()->publish(topic_name_, "Bond.TraderInfo", msg);
	}
}

void BondService::PublishAccountInfo(const std::vector<AccountCache*> &account_vec, const char* action_type)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index != account_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetAccountInfoMap(account_vec.at(index), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(account_vec.size() > 0)
        companyId = account_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    PublishManager::singleton()->publish(topic_name_, "Bond.AccountInfo", msg);*/
	std::map<std::string, sdbus::VariantList> tMap;
	for(size_t index = 0; index < account_vec.size(); ++index){
		AccountCache* cache = account_vec.at(index);
		if(cache->company_id.length() < 1){
			continue;
		}
		if(tMap.count(cache->company_id) < 1){
			sdbus::VariantList list;
			tMap[cache->company_id] = list;
		}
		sdbus::VariantMap *mp = tMap[cache->company_id].AddMap();
		SetAccountInfoMap(cache, mp);
	}
	std::map<std::string, sdbus::VariantList>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message msg;
		msg.SetString(FID_ACTION_TYPE, action_type);
		msg.SetString(FID_ACCOUNT_COMPANY_ID, iter->first);
		msg.SetList(FID_LIST, iter->second);
		PublishManager::singleton()->publish(topic_name_, "Bond.AccountInfo", msg);
	}
}

void BondService::PublishBrokerBindingTrader(const std::vector<BrokerBindingTraderCache*> &binding_vec, const char* action_type)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index != binding_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBrokerBindingTraderMap(binding_vec.at(index), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    PublishManager::singleton()->publish(topic_name_, "Bond.BrokerBindingTrader", msg);*/
	std::map<std::string, sdbus::VariantList> tMap;
	for(size_t index = 0; index < binding_vec.size(); ++index){
		BrokerBindingTraderCache* cache = binding_vec.at(index);
		if(cache->company_id.length() < 1){
			continue;
		}
		if(tMap.count(cache->company_id) < 1){
			sdbus::VariantList list;
			tMap[cache->company_id] = list;
		}

		sdbus::VariantMap *mp = tMap[cache->company_id].AddMap();
		SetBrokerBindingTraderMap(cache, mp);
	}
	std::map<std::string, sdbus::VariantList>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message msg;
		msg.SetString(FID_ACTION_TYPE, action_type);
		msg.SetString(FID_ACCOUNT_COMPANY_ID, iter->first);
		msg.SetList(FID_LIST, iter->second);
		PublishManager::singleton()->publish(topic_name_, "Bond.BrokerBindingTrader", msg);
	}
}

void BondService::PublishBondDeviatedValue(const std::vector<ChinaBondDeviatedValueCache*> &deviatedvalue_vec, const char* action_type)
{
    /*sdbus::Message msg;
    sdbus::VariantList *list = msg.AddList(FID_LIST);    

    for (size_t index = 0; index != deviatedvalue_vec.size(); ++index)
    {
        sdbus::VariantMap *mp = list->AddMap();
        SetBondDeviatedValueMap(deviatedvalue_vec.at(index), mp);
    }    

    msg.SetString(FID_ACTION_TYPE, action_type);

    std::string companyId;
    if(deviatedvalue_vec.size() > 0)
        companyId = deviatedvalue_vec.at(0)->company_id;
    msg.SetString(FID_ACCOUNT_COMPANY_ID, companyId);

    PublishManager::singleton()->publish(topic_name_, "Bond.BondDeviatedValue", msg);*/
	std::map<std::string, sdbus::VariantList> tMap;
	for(size_t index = 0; index < deviatedvalue_vec.size(); ++index){
		ChinaBondDeviatedValueCache* cache = deviatedvalue_vec.at(index);
		if(cache->company_id.length() < 1){
			continue;
		}
		if(tMap.count(cache->company_id) < 1){
			sdbus::VariantList list;
			tMap[cache->company_id] = list;
		}
		sdbus::VariantMap *mp = tMap[cache->company_id].AddMap();
		SetBondDeviatedValueMap(cache, mp);
	}
	std::map<std::string, sdbus::VariantList>::iterator iter;
	for(iter = tMap.begin(); iter != tMap.end(); ++iter){
		sdbus::Message msg;
		msg.SetString(FID_ACTION_TYPE, action_type);
		msg.SetString(FID_ACCOUNT_COMPANY_ID, iter->first);
		msg.SetList(FID_LIST, iter->second);
		PublishManager::singleton()->publish(topic_name_, "Bond.BondDeviatedValue", msg);
	}
}
/*
void BondService::PublishBondValuation()
{
    sdbus::Message msg;
    msg.SetString(FID_ACTION_TYPE, "Sync");

    PublishManager::singleton()->publish(topic_name_, "Bond.BondValuation", msg);
}*/

void BondService::GetBondSmallViewList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    BondSmallViewCacheVecPtr bonds = business.GetBondSmallViewList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondSmallViewCachePtr>::const_iterator it = bonds->begin();
    for ( ; it != bonds->end(); ++it) {
        BondSmallViewCache* bond = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondSmallViewMap(bond, mp);
    }
}

void BondService::GetBondSmallViewDetailList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    //BondSmallViewCacheVecPtr bonds = business.GetBondSmallViewList();

	int start = 0, cnt = 0;
	ParseRange(range, start, cnt);
	int row_cnt = business.getBondInfoSizeByCondition(cond, sortby);
	BondSmallViewCacheVecPtr bonds;
	if (start < row_cnt) {
		// Get bond quotes based on CONDITION, SORTBY & RANGE
		bonds = business.GetBondInfoByCondition(cond, sortby, range);
	} else {
		bonds = BondSmallViewCacheVecPtr(new std::vector<BondSmallViewCachePtr>());
	}
	reply.SetInt32(FID_ROW_COUNT, row_cnt);

	sdbus::VariantList *list = reply.AddList(FID_LIST);
    std::vector<BondSmallViewCachePtr>::const_iterator it = bonds->begin();
    for ( ; it != bonds->end(); ++it) {
        BondSmallViewCache* bond = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondSmallViewDetailMap(bond, mp);
    }
}

void BondService::GetBondInfoByCondition(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    BondSmallViewCacheVecPtr bonds = business.GetBondInfoByCondition(cond,sortby,range);
	reply.SetInt32(FID_ROW_COUNT, business.getBondInfoSizeByCondition(cond, sortby));

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondSmallViewCachePtr>::const_iterator it = bonds->begin();
    for ( ; it != bonds->end(); ++it) {
        BondSmallViewCache* bond = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondInfoMap(bond, mp);
    }
}

void BondService::GetBondInfoByID(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

    sdbus::string bond_key_listed_market;

    msg->GetString(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market);

    BondSmallViewCacheVecPtr bonds = business.GetBondInfoByID(bond_key_listed_market);

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondSmallViewCachePtr>::const_iterator it = bonds->begin();
    for ( ; it != bonds->end(); ++it)
    {
        BondSmallViewCache* bond = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondInfoMap(bond, mp);
    }
}

bool BondService::GetBondInfoByCode(const sdbus::Message* msg, sdbus::Message& reply)
{
    //BondBusiness business(this);
    std::string bond_code, bond_short_name;
    std::string queryId, queryShortName, queryListedMarket;

    msg->GetString(FID_BOND_CODE, bond_code);
    msg->GetString(FID_BOND_SHORT_NAME, bond_short_name);
    LOGGER_INFO("bond_code:[" << bond_code << "]")
    LOGGER_INFO("bond_short_name:[" << bond_short_name << "]")

    if(bond_code.empty())
    {
        std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_BOND_CODE_IS_EMPTY) + getIntToStringWithBracket(QQ2IDB_ERROR_BOND_CODE_IS_EMPTY);
        reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
        reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_BOND_CODE_IS_EMPTY);
        reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
        LOGGER_ERROR(err)
        return false;
    }

    boost::regex pattern("(\\d+).(SH|SZ|IB)", boost::regex::icase);
    boost::smatch result;
    if(bond_code.find('.') != std::string::npos)
    {
        if(boost::regex_match(bond_code, result, pattern))
        {
            queryId = result[1];
            queryListedMarket = result[2];
            LOGGER_INFO("queryId[" << queryId << "]," << "queryListedMarket[" << queryListedMarket << "]");

            std::transform(queryListedMarket.begin(), queryListedMarket.end(), queryListedMarket.begin(), static_cast<int(*)(int)>(std::toupper));
            if(queryListedMarket == "SH")
                queryListedMarket = "SSE";
            else if(queryListedMarket == "SZ")
                queryListedMarket = "SZE";
            else if(queryListedMarket == "IB")
                queryListedMarket = "CIB";
        } else
        {
            std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_BOND_CODE_WITH_INVALID_FORMAT) + getIntToStringWithBracket(QQ2IDB_ERROR_BOND_CODE_WITH_INVALID_FORMAT);
            reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
            reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_BOND_CODE_WITH_INVALID_FORMAT);
            reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
            LOGGER_ERROR(err)
            return false;
        }
    } else
    {
        queryId = bond_code;
    }
    queryShortName = bond_short_name;

    std::string bond_query_sql = "bond_id = '" + queryId + "'";
    std::string bond_query_sql_only_code = bond_query_sql;
    if(!queryShortName.empty()) {
        bond_query_sql += " and short_name = '" + queryShortName + "'";
    }
    if(!queryListedMarket.empty()) {
        bond_query_sql += " and listed_market = '" + queryListedMarket + "'";
    }
    LOGGER_INFO("Bond query only code:[" << bond_query_sql_only_code << "]")
    LOGGER_INFO("Bond query full:[" << bond_query_sql << "]")

    dbQuery q_bond_code(bond_query_sql.c_str());
    dbQuery q_bond_code_simple(bond_query_sql_only_code.c_str());
    BondSmallViewCacheController bond_smallview_cc;
    BondSmallViewCacheVecPtr bond_smallview_cache = bond_smallview_cc.getCacheListByQueryInThreadSafty(q_bond_code_simple);
    LOGGER_INFO("Simple Query result size:[" << bond_smallview_cache->size() << "]")

    if(bond_smallview_cache->size() == 0)
    {
        std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_BOND_CODE_NOT_FOUND_IN_IDB) + getIntToStringWithBracket(QQ2IDB_ERROR_BOND_CODE_NOT_FOUND_IN_IDB);
        reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
        reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_BOND_CODE_NOT_FOUND_IN_IDB);
        reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
        LOGGER_ERROR(err)
        return false;
    }

    if(bond_smallview_cache->size() == 1)
    {
        BondSmallViewCachePtr ref = bond_smallview_cache->front();
        reply.SetString(FID_BOND_KEY_LISTED_MARKET, ref->bond_key_listed_market);
        LOGGER_INFO("bond_key_listed_market[" << ref->bond_key_listed_market << "]")
        return true;
    }

    bond_smallview_cache = bond_smallview_cc.getCacheListByQueryInThreadSafty(q_bond_code);
    LOGGER_INFO("Full Query result size:[" << bond_smallview_cache->size() << "]")
    if(bond_smallview_cache->size() == 1)
    {
        BondSmallViewCachePtr ref = bond_smallview_cache->front();
        reply.SetString(FID_BOND_KEY_LISTED_MARKET, ref->bond_key_listed_market);
        if(ref->bond_key_listed_market.empty())
        {
            std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_FOUND_EMPTY_BOND_CODE_WITH_MARKET_AND_SHORTNAME_IN_IDB) + getIntToStringWithBracket(QQ2IDB_ERROR_FOUND_EMPTY_BOND_CODE_WITH_MARKET_AND_SHORTNAME_IN_IDB);
            reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
            reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_FOUND_EMPTY_BOND_CODE_WITH_MARKET_AND_SHORTNAME_IN_IDB);
            reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
            LOGGER_ERROR(err)
            return false;
        }

        LOGGER_INFO("bond_key_listed_market[" << ref->bond_key_listed_market << "]")
        LOGGER_INFO("listed_market[" << ref->listed_market << "]")
        LOGGER_INFO("short_name[" << ref->short_name << "]")
        LOGGER_INFO("short_name from utf to gbk[" << boost::locale::conv::between(ref->short_name, "GBK", "UTF-8") << "]")
        LOGGER_INFO("short_name from gbk to utf[" << boost::locale::conv::between(ref->short_name, "UTF-8", "GBK") << "]")
    } else if(bond_smallview_cache->size() == 0)
    {
        std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_BOND_NOT_FOUND_WITH_MARKET_AND_SHORTNAME_IN_IDB) + getIntToStringWithBracket(QQ2IDB_ERROR_BOND_NOT_FOUND_WITH_MARKET_AND_SHORTNAME_IN_IDB);
        reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
        reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_BOND_NOT_FOUND_WITH_MARKET_AND_SHORTNAME_IN_IDB);
        reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
        LOGGER_ERROR(err)
        return false;
    } else
    {
        std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_FOUND_MULTI_BOND_IN_IDB) + getIntToStringWithBracket(QQ2IDB_ERROR_FOUND_MULTI_BOND_IN_IDB);
        reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
        reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_FOUND_MULTI_BOND_IN_IDB);
        reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
        LOGGER_ERROR(err)
        return false;
    }

    return true;
}

void BondService::GetInstitutionInfoList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    TradingInstitutionCacheVecPtr institutions = business.GetInstitutionInfoList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<TradingInstitutionCachePtr>::const_iterator it = institutions->begin();
    for ( ; it != institutions->end(); ++it) {
        TradingInstitutionCache* institution = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetInstitutionInfoMap(institution, mp);
    }
}

void BondService::GetInstitutionInfoByID(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    std::string institution_id;

    msg->GetString(FID_INSTITUTION_ID, institution_id);

    TradingInstitutionCacheVecPtr institutions = business.GetInstitutionInfoByID(institution_id);

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<TradingInstitutionCachePtr>::const_iterator it = institutions->begin();
    for ( ; it != institutions->end(); ++it) {
        TradingInstitutionCache* institution = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetInstitutionInfoMap(institution, mp);
    }
}

void BondService::GetTraderInfoList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    BondTraderCacheVecPtr traders = business.GetTraderInfoList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondTraderCachePtr>::const_iterator it = traders->begin();
    for ( ; it != traders->end(); ++it) {
        BondTraderCache* trader = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetTraderInfoMap(trader, mp);
    }
}

void BondService::GetTraderInfoByID(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    std::string trader_id;

    msg->GetString(FID_TRADER_ID, trader_id);

    BondTraderCacheVecPtr traders = business.GetTraderInfoByID(trader_id);

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BondTraderCachePtr>::const_iterator it = traders->begin();
    for ( ; it != traders->end(); ++it) {
        BondTraderCache* trader = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetTraderInfoMap(trader, mp);
    }
}

void BondService::GetAccountInfoList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    AccountCacheVecPtr accounts = business.GetAccountInfoList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<AccountCachePtr>::const_iterator it = accounts->begin();
    for ( ; it != accounts->end(); ++it) {
        AccountCache* account = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetAccountInfoMap(account, mp);
    }
}

void BondService::UpdateAccountInfoByID(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

	AccountRole account;
	msg->GetString(FID_ACCOUNT_ID, account.account_id);
	msg->GetString(FID_USER_NAME, account.display_name);
	msg->GetString(FID_ACCOUNT_CODE, account.account_code);
	msg->GetString(FID_ACCOUNT_TELEPHONE, account.telephone);
	msg->GetString(FID_ACCOUNT_PHONE, account.mobile);
	msg->GetString(FID_ACCOUNT_EMAIL, account.email);
	msg->GetString(FID_ACCOUNT_RM, account.rm);
	msg->GetString(FID_ACCOUNT_MSN, account.msn);
	msg->GetString(FID_ACCOUNT_QQ, account.qq);
	msg->GetString(FID_ACCOUNT_ADDRESS, account.address);
	msg->GetString(FID_ACCOUNT_PASSWORD, account.password);
	
	int ret = business.UpdateAccountInfoByID(account);
	reply.SetInt32(FID_RETURN_CODE, ret);
}

void BondService::UpdateAccountPasswordByID(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

	AccountRole account;
	msg->GetString(FID_ACCOUNT_ID, account.account_id);
	msg->GetString(FID_ACCOUNT_PASSWORD, account.password);
	msg->GetString(FID_ACCOUNT_PASSWORD_ORG, account.password_org);

	int ret = business.UpdateAccountPasswordByID(account);
	reply.SetInt32(FID_RETURN_CODE, ret);
	reply.SetString(FID_ACCOUNT_PASSWORD, account.password);
}

void BondService::GetAccountInfoByID(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);
	
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    AccountCacheVecPtr accounts = business.GetAccountInfoByID(account_id);

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<AccountCachePtr>::const_iterator it = accounts->begin();
    for ( ; it != accounts->end(); ++it) {
        AccountCache* account = it->get();

        sdbus::VariantMap *mp = list->AddMap();
        
        SetAccountInfoMap(account, mp);
    }
}

void BondService::GetBrokerBindingTraderList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    BrokerBindingTraderCacheVecPtr bindings = business.GetBrokerBindingTraderList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<BrokerBindingTraderCachePtr>::const_iterator it = bindings->begin();
    for ( ; it != bindings->end(); ++it) {
        BrokerBindingTraderCache* binding = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBrokerBindingTraderMap(binding, mp);
    }
}

void BondService::GetBondDeviatedValueList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    ChinaBondDeviatedValueCacheVecPtr values = business.GetBondDeviatedValueList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<ChinaBondDeviatedValueCachePtr>::const_iterator it = values->begin();
    for ( ; it != values->end(); ++it) {
        ChinaBondDeviatedValueCache* deviated_value = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondDeviatedValueMap(deviated_value, mp);
    }
}
/*
void BondService::GetBondValuationList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

    sdbus::string cond, sortby, range;

    msg->GetString(FID_CONDITIONS, cond);
    msg->GetString(FID_SORT_BY, sortby);
    msg->GetString(FID_RANGE, range);

    ChinaBondValuationCacheVecPtr values = business.GetBondValuationList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    std::vector<ChinaBondValuationCachePtr>::const_iterator it = values->begin();
    for ( ; it != values->end(); ++it) {
        ChinaBondValuationCache* valuation = it->get();

        sdbus::VariantMap *mp = list->AddMap();

        SetBondValuationMap(valuation, mp);
    }
}*/

void BondService::GetBondIssueYearBound(const sdbus::Message* msg, sdbus::Message& reply)
{
    //business上获得年的最大值和最小值
    int max_issue_year = 2015, min_issue_year = 2000;
    BondBusiness business(this);	

    business.GetBondIssueYearBound(max_issue_year, min_issue_year);

    reply.SetInt32(FID_MAX_ISSUE_YEAR, max_issue_year);
    reply.SetInt32(FID_MIN_ISSUE_YEAR, min_issue_year);
}

void BondService::GetProvinceInfoList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

    ProvinceInfoCacheVecPtr provinceinfolist = business.GetBondIssueProvinceBound();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    auto it = provinceinfolist->begin();
    for ( ; it != provinceinfolist->end(); ++it)
    {
        ProvinceInfoCache* provinceinfo = it->get();
		if(provinceinfo->area_level =="PRN")
		{
			sdbus::VariantMap *mp = list->AddMap();
			mp->SetString(FID_ISSUE_PROVINCE, provinceinfo->area_name);
			mp->SetString(FID_ISSUE_PROVINCE_CODE, provinceinfo->area_code);
		}
    }
}

void BondService::GetSwSectorInfoList(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondBusiness business(this);

    sdbus::string sw_level;
	msg->GetString(FID_SW_SECTOR_LEVEL, sw_level);

	SwSectorInfoCacheVecPtr sectorinfolist = business.GetSwSectorInfoList(sw_level);

	sdbus::VariantList *list = reply.AddList(FID_LIST);

	std::vector<SwSectorInfoCachePtr>::const_iterator it = sectorinfolist->begin();
	for ( ; it != sectorinfolist->end(); ++it) {
		SwSectorInfoCache* sectorinfo = it->get();

		sdbus::VariantMap *mp = list->AddMap();
		mp->SetString(FID_SW_SECTOR_NAME, sectorinfo->sw_name);
		mp->SetString(FID_SW_SECTOR_CODE, sectorinfo->sw_internal_code);
		mp->SetString(FID_SW_SECTOR_UPPER_CODE, sectorinfo->sw_upper_grade_code);
		mp->SetString(FID_SW_SECTOR_LEVEL, sectorinfo->sw_level);
	}
}

void BondService::GetIssuingInstitutionList(const sdbus::Message* msg, sdbus::Message& reply)
{
    BondBusiness business(this);

    IssuingInstitutionCacheVecPtr iiList = business.GetIssuingInstitutionList();

    sdbus::VariantList *list = reply.AddList(FID_LIST);

    auto it = iiList->begin();
    for ( ; it != iiList->end(); ++it)
    {
        IssuingInstitutionCache* iiInfo = it->get();

        sdbus::VariantMap *mp = list->AddMap();
        mp->SetString(FID_ID, iiInfo->id);
        mp->SetString(FID_INSTITUTION_FULL_NAME, iiInfo->full_name_c);
        mp->SetString(FID_PIN_YIN, iiInfo->Pin_Yin);
        mp->SetString(FID_PIN_YIN_FULL, iiInfo->Pin_Yin_Full);
    }
}

void BondService::GetBondValuation(const sdbus::Message* msg, sdbus::Message& reply)
{
	BondBusiness business(this);

	sdbus::VariantList bond_key_listed_market_list;
	msg->GetList(FID_LIST, bond_key_listed_market_list);

	sdbus::VariantList *list = reply.AddList(FID_LIST);

	for (int i = 0; i != bond_key_listed_market_list.Size(); ++i)
	{
		const sdbus::VariantMap& mp = bond_key_listed_market_list.Get(i)->ToMap();
        sdbus::string bond_key_listed_market;
		mp.GetString(FID_BOND_KEY_LISTED_MARKET, bond_key_listed_market);

		BondSmallViewCacheVecPtr bonds = business.GetBondInfoByID(bond_key_listed_market);

		std::vector<BondSmallViewCachePtr>::const_iterator it = bonds->begin();
		for ( ; it != bonds->end(); ++it)
		{
			BondSmallViewCache* bond = it->get();

			sdbus::VariantMap *mp = list->AddMap();
			
			mp->SetString(FID_BOND_KEY_LISTED_MARKET, bond->bond_key_listed_market);
			mp->SetString(FID_CDC_VALUATIONS_YIELD, bond->cdc_valuation_yield);
			mp->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, bond->cdc_valuation_net_price);
		}
	}
}

void BondService::GetBondSmallViewInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);

	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("BondInfo.BondSmallView");
	if (cache) {
		cache->Get(version, reply);
	}
}

void BondService::GetBondSmallViewDetailInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);

	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("BondDetail.BondSmallViewDetail");
	if (cache) {
		cache->Get(version, reply);
	}
}

void BondService::GetBondInfoByConditionInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);

	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("Bond.GetBondInfoByCondition");
	if (cache) {
		cache->Get(version, reply);
	}
}

void BondService::GetInstitutionInfoListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("Bond.InstitutionInfo." + company_id);
	if (cache) {
		cache->Get(version, reply);
	}
}

void BondService::GetTraderInfoListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("Bond.TraderInfo." + company_id);
	if (cache) {
		cache->Get(version, reply);
	}
}

void BondService::GetAccountInfoListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("Bond.AccountInfo." + company_id);
	if (cache) {
		cache->Get(version, reply);
	}
}

void BondService::GetBrokerBindingTraderListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("Bond.BrokerBindingTrader." + company_id);
	if (cache) {
		cache->Get(version, reply);
	}
}

void BondService::GetBondDeviatedValueListInc(const sdbus::Message* msg, sdbus::Message& reply)
{
	int version; 
	msg->GetInt32(FID_PUSH_MESSAGE_VERSION, version);
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	IncrementCache *cache = IncrementCacheManager::singleton()->GetIncrementCache("Bond.BondDeviatedValue." + company_id);
	if (cache) {
		cache->Get(version, reply);
	}
}

bool BondService::createOrGetAndPublishInstitutionTraderInfo(const sdbus::Message* msg, sdbus::Message& reply)
{
    if(!GetBondInfoByCode(msg, reply))
        return false;

    BondBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id.c_str());
	/* END Add by Young 05-Apr-2016 */

    std::string institutionCode, traderCode, institutionId, traderId;
    bool publishInstitution = false, publishTrader = false;
    msg->GetString(FID_INSTITUTION_CODE, institutionCode);
    msg->GetString(FID_TRADER_CODE, traderCode);
    LOGGER_INFO("institutionCode in createOrGetAndPublishInstitutionTraderInfo in basic:[" << institutionCode << "]")
    LOGGER_INFO("traderCode in createOrGetAndPublishInstitutionTraderInfo in basic:[" << traderCode << "]")

    business.createOrGetInstitutionTraderInfo(institutionCode, traderCode, institutionId, traderId, publishInstitution, publishTrader);
    if(institutionId.empty())
    {
        std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_BOND_INSTITUTION_NOT_FOUND_IN_IDB) + getIntToStringWithBracket(QQ2IDB_ERROR_BOND_INSTITUTION_NOT_FOUND_IN_IDB);
        reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
        reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_BOND_INSTITUTION_NOT_FOUND_IN_IDB);
        reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
        LOGGER_ERROR(err)
        return false;
    }
    if(traderId.empty())
    {
        std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_BOND_TRADER_NOT_FOUND_IN_IDB) + getIntToStringWithBracket(QQ2IDB_ERROR_BOND_TRADER_NOT_FOUND_IN_IDB);
        reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
        reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_BOND_TRADER_NOT_FOUND_IN_IDB);
        reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
        LOGGER_ERROR(err)
        return false;
    }

    reply.SetString(FID_INSTITUTION_ID, institutionId);
    reply.SetString(FID_TRADER_ID, traderId);
    LOGGER_INFO("institutionId in createOrGetAndPublishInstitutionTraderInfo in basic:[" << institutionId << "]")
    LOGGER_INFO("traderId in createOrGetAndPublishInstitutionTraderInfo in basic:[" << traderId << "]")

    if(publishInstitution) {
        LOGGER_INFO("PublishInstitution")
        TradingInstitutionCacheController trading_institution_cc;
        dbQuery q_institution_id;
        q_institution_id = "id = ", institutionId;
        TradingInstitutionCachePtr trading_institution_cache = trading_institution_cc.getCacheByQueryInThreadSafty(q_institution_id);
        if(!trading_institution_cache) {
            std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_INSTITUTION) + getIntToStringWithBracket(QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_INSTITUTION);
            reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
            reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_INSTITUTION);
            reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
            LOGGER_ERROR(err)
            return false;
        }

        std::vector<TradingInstitutionCache*> insV;
        insV.push_back(trading_institution_cache.get());
        PublishInstitutionInfo(insV, "AddList");
    }

    if(publishTrader) {
        LOGGER_INFO("PublishTrader")
        BondTraderCacheController trader_cc;
        dbQuery q_trader_id;
        q_trader_id = "id = ", traderId;
        BondTraderCachePtr trader_cache = trader_cc.getCacheByQueryInThreadSafty(q_trader_id);
        if(!trader_cache) {
            std::string err = QQ2IDB_ERROR_NAME(QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_TRADER) + getIntToStringWithBracket(QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_TRADER);
            reply.SetBool(FID_QQ2IDB_IS_SUCCESS, false);
            reply.SetInt32(FID_QQ2IDB_ERROR_CODE, QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_TRADER);
            reply.SetString(FID_QQ2IDB_ERROR_MSG, err);
            LOGGER_ERROR(err)
            return false;
        }
        std::vector<BondTraderCache*> traV;
        traV.push_back(trader_cache.get());
        PublishTraderInfo(traV, "AddList");
    }

    reply.SetBool(FID_QQ2IDB_IS_SUCCESS, true);
    return true;
}

void BondService::GetTreasuryFuture(const sdbus::Message* msg, sdbus::Message& reply) {
	ConstantsCacheController ccc;
	dbQuery q("constants_key='TREASURY_FUTURE'");
	ConstantsCacheVecPtr vecs = ccc.getCacheListByQueryInThreadSafty(q);
	sdbus::VariantList* list = reply.AddList(FID_LIST);
	for (size_t i = 0; i < vecs->size(); i++) {
		ConstantsCachePtr cc = vecs->at(i);
		sdbus::VariantMap* map = list->AddMap();
		map->SetString(FID_BOND_SHORT_NAME, cc->name);
		map->SetString(FID_BOND_CODE, cc->value);
	}
}