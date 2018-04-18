#include "add_bond_quote.h"

#include <iostream>
#include <map>
#include "include/fid.h"
#include "include/constants.h"
#include "include/common.h"
#include "message.h"
#include "service/send_service.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "boost/lexical_cast.hpp"


namespace bcserver_service_transform {

    AddBondQuote::AddBondQuote(const sdbus::Message *msg)
    {
        sdbus::VariantMap parsedMsgMap;
        sdbus::VariantList parserResultList;
        msg->GetMap(FID_QQ2IDB_PARSEDMSG, parsedMsgMap);
        parsedMsgMap.GetList(FID_QQ2IDB_PARSER_RESULT, parserResultList);

        LOGGER_INFO("parsedMsgMap in msg: [" << parsedMsgMap.Size() << "]");
        LOGGER_INFO("parserResultList in msg: [" << parserResultList.Size() << "]");
        for(int i = 0; i < parserResultList.Size(); ++i)
        {
            sdbus::VariantMap quoteMsgMap = parserResultList.Get(i)->ToMap();
            BOND_VARIANT_MAP src;

            sdMapToBoostVarMap<std::string>(quoteMsgMap, src, FID_BOND_CODE);           // 代码	BondCode	　varchar(32)
            sdMapToBoostVarMap<std::string>(quoteMsgMap, src, FID_BOND_SHORT_NAME);     // 简称	BondName	　varchar(50)
            sdMapToBoostVarMap<int        >(quoteMsgMap, src, FID_QUOTE_SIDE);          // 方向	Direction	　int(1)	bid/ofr
            sdMapToBoostVarMap<double     >(quoteMsgMap, src, FID_PRICE);               // 价格	Price	　decimal(10,4)
            sdMapToBoostVarMap<int, std::string >(quoteMsgMap, src, FID_QUOTE_TYPE);           // 价格类型	QuoteType	　int(1)	收益率，净价，点差等等
            sdMapToBoostVarMap<int        >(quoteMsgMap, src, FID_VOLUME);              // 量	Volume	　int(8)
            sdMapToBoostVarMap<int        >(quoteMsgMap, src, FID_YIELD_TYPE);          // 行权/到期	Exercise	  int(1)	行权/到期
            sdMapToBoostVarMap<std::string>(quoteMsgMap, src, FID_DESCRIPTION);         // 报价备注	Comments	　varchar(255)	行权、到期、+0、+1等等
            sdMapToBoostVarMap<int, std::string >(quoteMsgMap, src, FID_BARGAIN_FLAG);  // 星号	Starmark	　int(1)	*和**，用于QB展示
            sdMapToBoostVarMap<bool, std::string>(quoteMsgMap, src, FID_FAN_DIAN_FLAG); // 是否返点	IsRebate	  bool
            sdMapToBoostVarMap<double     >(quoteMsgMap, src, FID_FAN_DIAN);            // 返点值	RebateNumber	　decimal(10,4)	返点的值，一般在0到0.10之间，为空则全返
            sdMapToBoostVarMap<std::string>(quoteMsgMap, src, FID_INSTITUTION_CODE);    // 报价群	GroupID	　varchar(32)
            sdMapToBoostVarMap<std::string>(quoteMsgMap, src, FID_TRADER_CODE);         // 报价人	QQID	　varchar(32)
            sdMapToBoostVarMap<int, std::string >(quoteMsgMap, src, FID_OCO_FLAG);

            // Not used
            sdMapToBoostVarMap<std::string>(quoteMsgMap, src, FID_QQ_SOURCE_MSG);

            LOGGER_INFO("FID_BOND_CODE in msg: [" << src[FID_BOND_CODE] << "]");
            LOGGER_INFO("FID_BOND_SHORT_NAME in msg: [" << src[FID_BOND_SHORT_NAME] << "]");
            LOGGER_INFO("FID_QUOTE_SIDE in msg: [" << src[FID_QUOTE_SIDE] << "]");
            LOGGER_INFO("FID_PRICE in msg: [" << src[FID_PRICE] << "]");
            LOGGER_INFO("FID_QUOTE_TYPE in msg: [" << src[FID_QUOTE_TYPE] << "]");
            LOGGER_INFO("FID_VOLUME in msg: [" << src[FID_VOLUME] << "]");
            LOGGER_INFO("FID_YIELD_TYPE in msg: [" << src[FID_YIELD_TYPE] << "]");
            LOGGER_INFO("FID_DESCRIPTION in msg: [" << src[FID_DESCRIPTION] << "]");
            LOGGER_INFO("FID_BARGAIN_FLAG in msg: [" << src[FID_BARGAIN_FLAG] << "]");
            LOGGER_INFO("FID_FAN_DIAN_FLAG in msg: [" << src[FID_FAN_DIAN_FLAG] << "]");
            LOGGER_INFO("FID_FAN_DIAN in msg: [" << src[FID_FAN_DIAN] << "]");
            LOGGER_INFO("FID_INSTITUTION_CODE in msg: [" << src[FID_INSTITUTION_CODE] << "]");
            LOGGER_INFO("FID_TRADER_CODE in msg: [" << src[FID_TRADER_CODE] << "]");
            LOGGER_INFO("FID_OCO_FLAG in msg: [" << src[FID_OCO_FLAG] << "]");

            mSrc_.push_back(src);
        }
    }

    bool AddBondQuote::sendquote(sdbus::Message &reply) 
    {
        copyUsefulDataForQuote();

        sdbus::Message request;
        sdbus::VariantList *list = request.AddList(FID_LIST);

        for(BOND_VARIANT_MAP_LIST::iterator it = mDes_.begin(); it != mDes_.end(); ++it)
        {
            BOND_VARIANT_MAP &des = *it;
            sdbus::VariantMap *map = list->AddMap();
            boostVarToSdMsg(des, *map);
        }

        sdbus::Message unzipedReply;
        request.SetType("BondQuote.Add");
        if(!SendService::singleton()->call(kServiceQueueBondQuote, request, reply))
        {
            LOGGER_ERROR("ERROR: Create quote failed!" )
            return false;
        }

        LOGGER_INFO("INFO: Create quote success" )
        return true;
    }

    void AddBondQuote::copyUsefulDataForQuote()
    {
        for(BOND_VARIANT_MAP_LIST::iterator it = mSrc_.begin(); it != mSrc_.end(); ++it)
        {
            BOND_VARIANT_MAP &src = *it;
            BOND_VARIANT_MAP des;
            // which don't have, for test
            des[FID_BOND_KEY_LISTED_MARKET] = src[FID_BOND_KEY_LISTED_MARKET];
            des[FID_INSTITUTION_ID] = src[FID_INSTITUTION_ID];
            des[FID_TRADER_ID] = src[FID_TRADER_ID];
            des[FID_BROKER_ID] = src[FID_BROKER_ID];

            des[FID_QUOTE_SIDE] = src[FID_QUOTE_SIDE];
            des[FID_PRICE] = src[FID_PRICE];
            des[FID_VOLUME] = src[FID_VOLUME];
            des[FID_DESCRIPTION] = src[FID_DESCRIPTION];
            des[FID_FAN_DIAN_FLAG] = src[FID_FAN_DIAN_FLAG];
            des[FID_FAN_DIAN] = src[FID_FAN_DIAN];
            des[FID_BARGAIN_FLAG] = src[FID_BARGAIN_FLAG];
            des[FID_OCO_FLAG] = src[FID_OCO_FLAG];
            des[FID_QUOTE_INTERNAL_FLAG] = src[FID_QUOTE_INTERNAL_FLAG];
            des[FID_QUOTE_TYPE] = src[FID_QUOTE_TYPE];
            des[FID_DELIVERY_DATE] = src[FID_DELIVERY_DATE];
            des[FID_YIELD_TYPE] = src[FID_YIELD_TYPE];

            // Extra field
            des[FID_BOND_CODE] = src[FID_BOND_CODE];
            des[FID_BOND_SHORT_NAME] = src[FID_BOND_SHORT_NAME];
            des[FID_INSTITUTION_CODE] = src[FID_INSTITUTION_CODE];
            des[FID_TRADER_CODE] = src[FID_TRADER_CODE];

            mDes_.push_back(des);
        }
    }

    void AddBondQuote::addError(QQ2IDB_ERROR_CODE err)
    {
        if(errList_.end() == std::find(errList_.begin(), errList_.end(), err))
            errList_.push_back(err);
    }

    void AddBondQuote::removeError(QQ2IDB_ERROR_CODE err)
    {
        std::list<int>::iterator ret = std::find(errList_.begin(), errList_.end(), err);
        if(errList_.end() != ret)
            *ret = err;
    }

    void AddBondQuote::processError()
    {

    }

}  // namespace bcserver_service_transform
