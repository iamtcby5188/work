#include "bondcompletedquote_service.h"
#include "business/bondcompletedquote_business.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "sdbus/codec.h"
#include "msg2db/msg_log_service.h"

BondCompletedQuoteService::BondCompletedQuoteService(sdbus::Connection *conn, const std::string& queue_name, const std::string& topic_name)
    : Service(conn, queue_name, topic_name)
{

}

void BondCompletedQuoteService::processReqMsg(const sdbus::Message* msg)
{
    sdbus::string method  = msg->GetType();   
    sdbus::string subject = msg->GetSubject();

    sdbus::Message reply, zip_reply;
    reply.SetType(msg->GetType());
    reply.SetSubject(msg->GetSubject());
    reply.SetMessageID(msg->GetMessageID());

    if (method == "BondCompletedQuote.CompletedQuoteDetailList") {
        GetCompletedQuoteDetailList(msg, reply);
    }else if (method == "BondCompletedQuote.CopyCompletedQuote") {
        CopyCompletedQuote(msg, reply);
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

void BondCompletedQuoteService::GetCompletedQuoteDetailList(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::string cond, sortby, range;
    msg->GetString(FID_CONDITIONS, cond);

    BondCompletedQuoteBusiness business;
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    CompletedQuoteDetailVecPtr detail_vec = business.GetCompletedQuoteDetailList(cond.c_str());

    sdbus::VariantList *replylist = reply.AddList(FID_LIST);

    std::vector<CompletedQuoteDetailPtr>::const_iterator it = detail_vec->begin();
    for ( ; it != detail_vec->end(); ++it){
        CompletedQuoteDetail *bonddetail = (*it).get();

        sdbus::VariantMap *mp = replylist->AddMap();

        mp->SetString(FID_BID_QUOTE_ID, bonddetail->bid_quote_id);
        mp->SetString(FID_BOND_KEY_LISTED_MARKET, bonddetail->bond_key_listed_market);
        mp->SetString(FID_BID_INSTITUTION_ID, bonddetail->bid_issue_institution_id);
        mp->SetString(FID_BID_TRADER_ID, bonddetail->bid_trader_id);
        mp->SetString(FID_BID_BARGAIN_FLAG, bonddetail->bid_bargain_flag_);
        mp->SetString(FID_BID_BROKER_NAME, bonddetail->bid_broker_name_);
        mp->SetString(FID_BID_DESCRIPTION, bonddetail->bid_description_);
        mp->SetDouble(FID_BID_FAN_DIAN, bonddetail->bid_fan_dian_);
        mp->SetString(FID_BID_FAN_DIAN_FLAG, bonddetail->bid_fan_dian_flag_);
        mp->SetString(FID_BID_INSTITUTION_TRADER_NAMES, bonddetail->bid_institution_trader_name_);
        mp->SetString(FID_BID_INTERNAL_FLAG, bonddetail->bid_internal_flag_);
        mp->SetString(FID_BID_OCO_FLAG, bonddetail->bid_oco_flag_);
        mp->SetDouble(FID_BID_PRICE, bonddetail->bid_price_);
        mp->SetDouble(FID_BID_VOLUME, bonddetail->bid_volume_); 
        mp->SetDouble(FID_BEST_BID_VOLUME, bonddetail->bid_volume_);
        mp->SetString(FID_BID_PRICE_STRING, bonddetail->bid_price_string_);
        mp->SetString(FID_BID_FAN_DIAN_STRING, bonddetail->bid_fan_dian_string_);

        mp->SetString(FID_OFR_QUOTE_ID, bonddetail->ofr_quote_id);
		mp->SetString(FID_OFR_INSTITUTION_ID, bonddetail->ofr_issue_institution_id);
        mp->SetString(FID_OFR_TRADER_ID, bonddetail->ofr_trader_id);
        mp->SetString(FID_OFR_BARGAIN_FLAG, bonddetail->ofr_bargain_flag_);
        mp->SetString(FID_OFR_BROKER_NAME, bonddetail->ofr_broker_name_);
        mp->SetString(FID_OFR_DESCRIPTION, bonddetail->ofr_description_);
        mp->SetDouble(FID_OFR_FAN_DIAN, bonddetail->ofr_fan_dian_);
        mp->SetString(FID_OFR_FAN_DIAN_FLAG, bonddetail->ofr_fan_dian_flag_);
        mp->SetString(FID_OFR_INSTITUTION_TRADER_NAMES, bonddetail->ofr_institution_trader_name_);
        mp->SetString(FID_OFR_INTERNAL_FLAG, bonddetail->ofr_internal_flag_);
        mp->SetString(FID_OFR_OCO_FLAG, bonddetail->ofr_oco_flag_);
        mp->SetDouble(FID_OFR_PRICE, bonddetail->ofr_price_);
        mp->SetDouble(FID_OFR_VOLUME, bonddetail->ofr_volume_);
        mp->SetDouble(FID_BEST_OFR_VOLUME, bonddetail->ofr_volume_);
        mp->SetString(FID_OFR_PRICE_STRING, bonddetail->ofr_price_string_);
        mp->SetString(FID_OFR_FAN_DIAN_STRING, bonddetail->ofr_fan_dian_string_);

        mp->SetBool(FID_IS_BEST_QUOTE, bonddetail->is_best_quote_);
    }
}

void BondCompletedQuoteService::CopyCompletedQuote(const sdbus::Message* msg, sdbus::Message& reply)
{
    sdbus::VariantList list;
    sdbus::string title;
    int internalflag;
    msg->GetList(FID_LIST, list);
    msg->GetString(FID_BOND_TYPE, title);
    msg->GetInt32(FID_QUOTE_INTERNAL_FLAG, internalflag);

    BondCompletedQuoteBusiness business(this);
	/* START Add by Young 05-Apr-2016 */
	std::string account_id, company_id;
	msg->GetString(FID_ACCOUNT_ID, account_id);
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if(company_id.empty()){
		company_id = GetCurrentCompanyId(account_id);
	}
	business.SetCompanyId(company_id);
	/* END Add by Young 05-Apr-2016 */

    bool isInternalQuote = internalflag == kInternalQuote ? true : false;
    //sdbus::string exportedString = business.CopyCompletedQuote(list, title, kCurrentCompanyId, isInternalQuote);
    sdbus::string exportedString = business.CopyCompletedQuote(list);
    reply.SetString(FID_EXPORTED_TEXT, exportedString);
}
