/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Define some constants
====================================================================================================================
   Date       Name                                  Description of Change
27-Aug-2014   James Xu                              Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <string>
#include "sdbus/string.h" //Add by Young 05-Apr-2016 
#include "sdbus/message.h"

#define DECIMAL_NULL -999
#define DOUBLE_NULL -999
#define FLOATING_PRECISION 1.0e-7

// service name
static const std::string kServiceLogin = "Login";
static const std::string kServiceHeartBeat = "HeartBeat";
static const std::string kServiceBondInfo = "Bond";
static const std::string kServiceBondQuote = "BondQuote";
static const std::string kServiceBondDeal = "BondDeal";
static const std::string kServiceDcsBondDeal = "DcsBondDeal";
static const std::string kServiceBondBestQuote = "BondBestQuote";
static const std::string kServiceBondCompletedQuote = "BondCompletedQuote";
static const std::string kServiceManagement = "Management";
static const std::string kServiceHotKey = "HotKey";
static const std::string kServiceProductFavor = "ProductFavor";
static const std::string kServiceSync = "Sync";
static const std::string kServiceCalculator = "Calculator";
static const std::string kServiceQBSync = "QBSync";
static const std::string kServiceUserInfo = "UserInfo";

// qpid service queue
static const std::string kServiceQueueLogin = "CQ.SDBUS.Bond.Service.Login";
static const std::string kServiceQueueHeartBeat = "CQ.SDBUS.Bond.Service.HeartBeat";
static const std::string kServiceQueueBondInfo = "CQ.SDBUS.Bond.Service.BondInfo";
static const std::string kServiceQueueBondQuote = "CQ.SDBUS.Bond.Service.BondQuote";
static const std::string kServiceQueueBondDeal = "CQ.SDBUS.Bond.Service.BondDeal";
static const std::string kServiceQueueDcsBondDeal = "CQ.SDBUS.Bond.Service.DcsBondDeal";
static const std::string kServiceQueueBondBestQuote = "CQ.SDBUS.Bond.Service.BondBestQuote";
static const std::string kServiceQueueBondCompletedQuote = "CQ.SDBUS.Bond.Service.BondCompletedQuote";
static const std::string kServiceQueueManagement = "CQ.SDBUS.Bond.Service.Management";
static const std::string kServiceQueueHotKey = "CQ.SDBUS.Bond.Service.HotKey";
static const std::string kServiceQueueProductFavor = "CQ.SDBUS.Bond.Service.ProductFavor";
static const std::string kServiceQueueSync = "CQ.SDBUS.Bond.Service.Sync";
static const std::string kServiceQueueTPRFA = "CQ.SDBUS.Bond.Service.TPRFA";
static const std::string kServiceQueueMessageResend = "CQ.SDBUS.Bond.Service.MessageResend";
static const std::string kServiceQueueMessageResendRecieve = "CQ.SDBUS.Bond.Service.MessageResendRecieve";
static const std::string kServiceQueueCalculator = "CQ.SDBUS.Bond.Service.Calculator";
static const std::string kServiceQueueUserInfo = "CQ.SDBUS.Bond.Service.UserInfo";
static const std::string kServiceQueuePinyin = "CQ.SDBUS.Bond.Service.Pinyin";
static const std::string kServiceQueueQQ2IDB = "CQ.SDBUS.Bond.Service.QQ2IDBS";

// qpid topic
static const std::string kTopicLogin = "CQ.SDBUS.Bond.Topic.Login";
static const std::string kTopicHeartBeat = "CQ.SDBUS.Bond.Topic.HeartBeat";
static const std::string kTopicBondInfo = "CQ.SDBUS.Bond.Topic.BondInfo";
static const std::string kTopicBondDetail = "CQ.SDBUS.Bond.Topic.BondDetail";
static const std::string kTopicBondQuote = "CQ.SDBUS.Bond.Topic.BondQuote";
static const std::string kTopicBondDeal = "CQ.SDBUS.Bond.Topic.BondDeal";
static const std::string kTopicDcsBondDeal = "CQ.SDBUS.Bond.Topic.DcsBondDeal";
static const std::string kTopicBondBestQuote = "CQ.SDBUS.Bond.Topic.BondBestQuote";
static const std::string kTopicBondCompletedQuote = "CQ.SDBUS.Bond.Topic.BondCompletedQuote";
static const std::string kTopicManagement = "CQ.SDBUS.Bond.Topic.Management";
static const std::string kTopicHotKey = "CQ.SDBUS.Bond.Topic.HotKey";
static const std::string kTopicProductFavor = "CQ.SDBUS.Bond.Topic.ProductFavor";
static const std::string kTopicSync = "CQ.SDBUS.Bond.Topic.Sync";

// qpid send service ret queue
static const std::string kServiceRetQueueMain = "CQ.SDBUS.Bond.RetQueue.Main";
static const std::string kServiceRetQueueProductFavor = "CQ.SDBUS.Bond.RetQueue.ProductFavor";
static const std::string kServiceRetQueueHotKey = "CQ.SDBUS.Bond.RetQueue.HotKey";
static const std::string kServiceRetQueueBasicData = "CQ.SDBUS.Bond.RetQueue.BasicData";

// qpid for QB sync
extern std::string kQBSyncTopic;
extern std::string kQBSyncServiceQueue;

// best quote make and insert into to mysql database
extern std::string kCalcAndInsertIntoMysqlDatabase;
extern bool kFirstLaunch;

// use cache in mem
extern bool useCacheInMem;

// database pool config
static const int kDBCoonPoolSize = 5;

// 成交单确认状态
const std::string kDealStatusNoChecked = "0";
const std::string kDealStatusOneChecked = "1";
const std::string kDealStatusBothChecked = "2";
const std::string kDealStatusCompleted = "3";
const std::string kDealStatusRefered = "4";
const std::string kDealStatusCanceled = "5";

// DCS成交单状态
const std::string kDcsDealtoBeConfirm                  = "0"; // 待确认
const std::string kDcsDealtoBeSubmit                   = "1"; // 待提交
const std::string kDcsDealSubmitted                    = "2"; // 已提交
const std::string kDcsDealPassed                       = "3"; // 已通过
const std::string kDcsDealNoPass                       = "5"; // 未通过
const std::string kDcsDealDestroyed                    = "7"; // 已毁单
const std::string kDcsDealIntheReview                  = "13"; // 送审中（高级经理签字状态）
const std::string kDcsDealDeleted                      = "-1"; // 已删除

const std::string kDcsDealtoBeConfirmString            = "待确认";
const std::string kDcsDealtoBeSubmitString             = "待提交";
const std::string kDcsDealSubmittedString              = "已提交";
const std::string kDcsDealPassedString                 = "已通过";
const std::string kDcsDealNoPassString                 = "未通过";
const std::string kDcsDealDestroyedString              = "已毁单";
const std::string kDcsDealIntheReviewString            = "送审中";
const std::string kDcsDealDeletedString                = "已删除";
const std::string kDcsDealInHandString                 = "处理中";

//DCS开关
extern bool kDcsEnable;

//DCS收单状态
extern bool kDcsSwitchStatus;

//每次请求DCS成交单数据的条数
extern int kDcsPageSize;

const std::string kCompanyTP = "1";
const std::string kCompanyICAP = "2";
const std::string kCompanyBGC = "3";
const std::string kCompanyPATR = "4";

const std::string kOperatorAdd = "Add";
const std::string kOperatorUpdate = "Update";
const std::string kOperatorDelete = "Delete";

const int kNormalQuote = 1;   //正常导出
const int kInternalQuote = 2; //暗盘导出
const std::string kCompanyTPName = "TPSC";
const std::string kCompanyICAPName = "上海国际货币";

extern std::string GetCurrentCompanyId(const sdbus::Message* msg);
extern std::string GetCurrentCompanyId(std::string account_id);
extern std::string GetCurrentCompanyName(std::string company_id);
extern std::string GetCurrentCompanyIdByAccountName(std::string account_name);
extern std::string GetCurrentAccountName(std::string account_id);

// Bid/Ofr
const int kBidQuote = 1;
const int kOfrQuote = -1;

// 内部报价
const std::string kQuoteInternal = "2"; // 内部报价
const std::string kQuoteNotInternal = "1"; // 非内部报价

// 内部成交
const std::string kDealInternal = "2"; // 内部成交
const std::string kDealNotInternal = "1"; // 非内部成交

// 上市日期类型 <与客户端值同步>
const std::string kMaturityDateNormal = "Normal";
const std::string kMaturityDateHoliday = "Holiday";
const std::string kMaturityDateSaturday = "Saturday";
const std::string kMaturityDateSunday = "Sunday";

// 条数限制为500
static const int kAmountLimit = 1500;

extern int kServiceSyncLimit;

// All method types
const std::string mBond_AccountInfo = "Bond.AccountInfo";
const std::string mBond_BondDeviatedValue = "Bond.BondDeviatedValue";
const std::string mBond_BrokerBindingTrader = "Bond.BrokerBindingTrader";
const std::string mBond_GetBondInfoByCondition = "Bond.GetBondInfoByCondition";
const std::string mBond_InstitutionInfo = "Bond.InstitutionInfo";
const std::string mBond_TraderInfo = "Bond.TraderInfo";
const std::string mBondBestQuote_BestQuoteList = "BondBestQuote.BestQuoteList";
const std::string mBondBestQuote_CompletedQuoteList = "BondBestQuote.CompletedQuoteList";
const std::string mBondDeal_DealList = "BondDeal.DealList";
const std::string mDcsBondDeal_DealList = "DcsBondDeal.DealList";
const std::string mBondDetail_BondSmallViewDetail = "BondDetail.BondSmallViewDetail";
const std::string mBondInfo_BondSmallView = "BondInfo.BondSmallView";
const std::string mBondQuote_QuoteList = "BondQuote.QuoteList";
const std::string mBondQuote_ReferQuoteList = "BondQuote.ReferQuoteList";
const std::string mBond_CacheReload = "Bond.CacheReload";
#endif

