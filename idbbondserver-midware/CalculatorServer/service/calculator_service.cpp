#include "calculator_service.h"

#include <iostream>

#include "include/SSCalculator/SSBasicCalculationPage.h"
#include "include/SSCalculator/SSCalculatorIF.h"
#include "include/SSCalculator/SSCalculatorError.h"

#include "business/calculator_data_loader.h"
#include "business/calculator_business.h"
#include "include/fid.h"
#include "include/constants.h"
#include "include/common.h"
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include "sdbus/codec.h"

using ssCalculator::SSCalculatorIF;

using bcserver_business_calculator::CalculatorDataLoader;
using bcserver_business_calculator::CalculatorBusiness;

namespace bcserver_service_calculator {
    
CalculatorService::CalculatorService(sdbus::Connection* conn) : conn_(conn), is_ready_(false) {
    Init(kServiceQueueCalculator, sdbus::DeliverDirect);
    conn_->AddHandler(this);
    CalculatorBusiness::LoadCalculatorData(CalculatorDataLoader::kALLDB);
    is_ready_ = true;

	boost::function0<void> f = boost::bind(&CalculatorService::run, this);  
	boost::thread thrd(f); 
}

void CalculatorService::run()
{	
    boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(120));
	t.async_wait(boost::bind(&CalculatorService::loadCalculatorData, this, boost::asio::placeholders::error, &t));
	io.run();
}

void CalculatorService::loadCalculatorData(const boost::system::error_code& e, boost::asio::deadline_timer* t)
{
    LOGGER_INFO("Start load calculator data. ") 
    CalculatorBusiness::LoadCalculatorData(CalculatorDataLoader::kALLDB);    
    LOGGER_INFO("End load calculator data. ")

    t->expires_at(t->expires_at() + boost::posix_time::seconds(600));
	t->async_wait(boost::bind(&CalculatorService::loadCalculatorData, this, boost::asio::placeholders::error, t));
}

void CalculatorService::OnMsgs(const sdbus::Message ** msgs, int count) {
    if (!is_ready_) {
        // Service还没准备好，忽略消息
        std::cout << "Ignore " << count << " message." << std::endl;
        return;
    }
    for (int i = 0; i < count; i++) {
        const sdbus::Message * msg = *(msgs + i);
        sdbus::string method = msg->GetType();
        
        LOGGER_INFO("Receive message: " << method)

        sdbus::Message reply;
        reply.SetType(msg->GetType());
        reply.SetSubject(msg->GetSubject());
        reply.SetMessageID(msg->GetMessageID());

        if (method == "Calculator.BondClassification") {
            BondClassification(msg, reply);
        } else if (method == "Calculator.PriceYieldConversion") {
            PriceYieldConversion(msg, reply);
        } else if (method == "Calculator.DateAdvance") {
            DateAdvance(msg, reply);
        } else if (method == "Calculator.QuickCalculation") {
            QuickCalculation(msg, reply);
        }

        if (msg->GetReplyTo().length() > 0) {   
            size_t len = -1;
            VariantMapCalcSize(reply.MapValue(), len);
            LOGGER_INFO("Message Size: " << len)

            bool flag = conn_->Send(msg->GetReplyTo(), reply);
            
            LOGGER_INFO("ReplyTo: " << msg->GetReplyTo())
            if (flag) {
                LOGGER_INFO("Reply success: " << method)
            } else {
                LOGGER_ERROR("Reply failed: " << method)
            }
        }
    }
}

void CalculatorService::BondClassification(const sdbus::Message* msg, sdbus::Message& reply) {
    CalculatorBusiness business;

    sdbus::string bond_key_sdbus;
    msg->GetString(FID_BOND_KEY, bond_key_sdbus);

    sdbus::string settlement_date_sdbus;
    msg->GetString(FID_SETTLEMENT_DATE, settlement_date_sdbus);

    bool is_frn_bond = business.IsFRNBond(bond_key_sdbus.c_str());
    bool is_ytc_available = business.IsYTCAvailable(bond_key_sdbus.c_str(), settlement_date_sdbus.c_str());
    bool is_ytp_available = business.IsYTPAvailable(bond_key_sdbus.c_str(), settlement_date_sdbus.c_str());

    std::cout << "is frn bond: " << is_frn_bond << std::endl;  
    std::cout << "is ytc available: " << is_ytc_available << std::endl;
    std::cout << "is ytp available: " << is_ytp_available << std::endl;

    reply.SetDouble(FID_IS_FRN_BOND, is_frn_bond);
    reply.SetDouble(FID_IS_YTC_AVAILABLE, is_ytc_available);
    reply.SetDouble(FID_IS_YTP_AVAILABLE, is_ytp_available);
}


void CalculatorService::PriceYieldConversion(const sdbus::Message* msg, sdbus::Message& reply) {
    CalculatorBusiness business;

    sdbus::string calculate_action_type_sdbus;
    msg->GetString(FID_CALCULATE_ACTION_TYPE, calculate_action_type_sdbus);

    bool fandian_flag;
    msg->GetBool(FID_FAN_DIAN_FLAG, fandian_flag);

    sdbus::string quote_type_sdbus;
    msg->GetString(FID_QUOTE_TYPE, quote_type_sdbus);

    sdbus::string bond_key_sdbus;
    msg->GetString(FID_BOND_KEY, bond_key_sdbus);

    sdbus::string settlement_date_sdbus;
    msg->GetString(FID_SETTLEMENT_DATE, settlement_date_sdbus);

    double price;
    msg->GetDouble(FID_PRICE, price);

    sdbus::string index_rate_str;
    double index_rate;
    msg->GetString(FID_INDEX_RATE, index_rate_str);
    if (index_rate_str=="") {
        index_rate = INVALID_VALUE;
    } else {
        msg->GetDouble(FID_INDEX_RATE, index_rate);
    }

    double fandian;
    msg->GetDouble(FID_FAN_DIAN, fandian);

    SSBasicCalculationPage basicPage = business.PriceYieldConversion(
        calculate_action_type_sdbus.c_str(),
        fandian_flag,
        quote_type_sdbus.c_str(),
        bond_key_sdbus.c_str(),
        settlement_date_sdbus.c_str(),
        price,
        index_rate,
        fandian);

    std::cout << "clean price: " << basicPage.cleanPrice_ << std::endl;  
    std::cout << "dirty price: " << basicPage.dirtyPrice_ << std::endl;
    std::cout << "ytm: " << basicPage.yieldToMaturity_ << std::endl;
    std::cout << "spread: " << basicPage.spread_ << std::endl;
    std::cout << "ytc: " << basicPage.yieldToCall_ << std::endl;
    std::cout << "ytp: " << basicPage.yieldToPut_ << std::endl;

    bool is_frn_bond = business.IsFRNBond(bond_key_sdbus.c_str());
    bool is_ytc_available = business.IsYTCAvailable(bond_key_sdbus.c_str(), settlement_date_sdbus.c_str());
    bool is_ytp_available = business.IsYTPAvailable(bond_key_sdbus.c_str(), settlement_date_sdbus.c_str());

    std::cout << "is frn bond: " << is_frn_bond << std::endl;  
    std::cout << "is ytc available: " << is_ytc_available << std::endl;
    std::cout << "is ytp available: " << is_ytp_available << std::endl;

    reply.SetDouble(FID_INDEX_RATE, basicPage.indexRate_);
    reply.SetString(FID_INDEX_DATE, basicPage.indexDate_);
    reply.SetDouble(FID_CLEAN_PRICE, basicPage.cleanPrice_);
    reply.SetDouble(FID_DIRTY_PRICE, basicPage.dirtyPrice_);
    reply.SetDouble(FID_YIELD_TO_MATURITY, basicPage.yieldToMaturity_);
    reply.SetDouble(FID_SPREAD, basicPage.spread_);
    reply.SetDouble(FID_YIELD_TO_CALL, basicPage.yieldToCall_);
    reply.SetDouble(FID_YIELD_TO_PUT, basicPage.yieldToPut_);
    reply.SetInt32(FID_ACCRUED_DAYS, basicPage.accruedDays_);
    reply.SetDouble(FID_ACCRUED_AMOUNT, basicPage.accruedAmount_);
    reply.SetString(FID_MATURITY, basicPage.maturityDate_);
    reply.SetString(FID_EXECUTION_DATE, basicPage.executionDate_);
    reply.SetDouble(FID_MACAULAY_DURATION, basicPage.MacaulayDuration_);
    reply.SetDouble(FID_MODIFIED_DURATION, basicPage.modifiedDuration_);
    reply.SetDouble(FID_PVBP, basicPage.pvbp_);
    reply.SetDouble(FID_CONVEXITY, basicPage.convexity_);
    reply.SetString(FID_CALCULATOR_ERROR_MESSAGE, GBKToUTF8(basicPage.exceptions_));

    reply.SetDouble(FID_IS_FRN_BOND, is_frn_bond);
    reply.SetDouble(FID_IS_YTC_AVAILABLE, is_ytc_available);
    reply.SetDouble(FID_IS_YTP_AVAILABLE, is_ytp_available);
}

void CalculatorService::DateAdvance (const sdbus::Message* msg, sdbus::Message& reply) {
    CalculatorBusiness business;

    sdbus::string market_sdbus;
    msg->GetString(FID_CALENDAR_MARKET_KEY, market_sdbus);

    sdbus::string date_sdbus;
    msg->GetString(FID_DATE, date_sdbus);

    int period_num;
    msg->GetInt32(FID_PERIOD_NUM, period_num);

    sdbus::string period_unit_sdbus;
    msg->GetString(FID_PERIOD_UNIT, period_unit_sdbus);

    sdbus::string convention_sdbus;
    msg->GetString(FID_BUSINESS_DAY_CONVENTION, convention_sdbus);

    string return_date = business.DateAdvance(market_sdbus.c_str(), date_sdbus.c_str(), period_num, period_unit_sdbus.c_str(), convention_sdbus.c_str());
    std::cout << "adjusted date: " << return_date << std::endl;
    reply.SetString(FID_DATE, return_date);
}

void CalculatorService::QuickCalculation(const sdbus::Message* msg, sdbus::Message& reply)
{
    CalculatorBusiness business;

    // Date Advance
    sdbus::string market_sdbus;
    msg->GetString(FID_CALENDAR_MARKET_KEY, market_sdbus);

    sdbus::string date_sdbus;
    msg->GetString(FID_DATE, date_sdbus);

    int period_num;
    msg->GetInt32(FID_PERIOD_NUM, period_num);

    sdbus::string period_unit_sdbus;
    msg->GetString(FID_PERIOD_UNIT, period_unit_sdbus);

    sdbus::string convention_sdbus;
    msg->GetString(FID_BUSINESS_DAY_CONVENTION, convention_sdbus);

    std::string adjusted_date = business.DateAdvance(market_sdbus.c_str(), date_sdbus.c_str(), period_num, period_unit_sdbus.c_str(), convention_sdbus.c_str());

    // Price Yield Conversion  
    sdbus::string calculate_action_type_sdbus;
    msg->GetString(FID_CALCULATE_ACTION_TYPE, calculate_action_type_sdbus);

    bool fandian_flag;
    msg->GetBool(FID_FAN_DIAN_FLAG, fandian_flag);

    sdbus::string quote_type_sdbus;
    msg->GetString(FID_QUOTE_TYPE, quote_type_sdbus);

    sdbus::string bond_key_sdbus;
    msg->GetString(FID_BOND_KEY, bond_key_sdbus);

    double price;
    msg->GetDouble(FID_PRICE, price);

    sdbus::string index_rate_str;
    double index_rate;
    msg->GetString(FID_INDEX_RATE, index_rate_str);
    if (index_rate_str=="") {
        index_rate = INVALID_VALUE;
    } else {
        msg->GetDouble(FID_INDEX_RATE, index_rate);
    }

    double fandian;
    msg->GetDouble(FID_FAN_DIAN, fandian);

    SSBasicCalculationPage basicPage = business.PriceYieldConversion(
        calculate_action_type_sdbus.c_str(),
        fandian_flag,
        quote_type_sdbus.c_str(),
        bond_key_sdbus.c_str(),
        adjusted_date.c_str(),
        price,
        index_rate,
        fandian);

    std::cout << "clean price: " << basicPage.cleanPrice_ << std::endl;  
    std::cout << "dirty price: " << basicPage.dirtyPrice_ << std::endl;
    std::cout << "ytm: " << basicPage.yieldToMaturity_ << std::endl;
    std::cout << "spread: " << basicPage.spread_ << std::endl;
    std::cout << "ytc: " << basicPage.yieldToCall_ << std::endl;
    std::cout << "ytp: " << basicPage.yieldToPut_ << std::endl;

    bool is_frn_bond = business.IsFRNBond(bond_key_sdbus.c_str());
    bool is_ytc_available = business.IsYTCAvailable(bond_key_sdbus.c_str(), adjusted_date.c_str());
    bool is_ytp_available = business.IsYTPAvailable(bond_key_sdbus.c_str(), adjusted_date.c_str());

    std::cout << "is frn bond: " << is_frn_bond << std::endl;  
    std::cout << "is ytc available: " << is_ytc_available << std::endl;
    std::cout << "is ytp available: " << is_ytp_available << std::endl;

    reply.SetString(FID_SETTLEMENT_DATE, adjusted_date);

    reply.SetDouble(FID_INDEX_RATE, basicPage.indexRate_);
    reply.SetString(FID_INDEX_DATE, basicPage.indexDate_);
    reply.SetDouble(FID_CLEAN_PRICE, basicPage.cleanPrice_);
    reply.SetDouble(FID_DIRTY_PRICE, basicPage.dirtyPrice_);
    reply.SetDouble(FID_YIELD_TO_MATURITY, basicPage.yieldToMaturity_);
    reply.SetDouble(FID_SPREAD, basicPage.spread_);
    reply.SetDouble(FID_YIELD_TO_CALL, basicPage.yieldToCall_);
    reply.SetDouble(FID_YIELD_TO_PUT, basicPage.yieldToPut_);
    reply.SetInt32(FID_ACCRUED_DAYS, basicPage.accruedDays_);
    reply.SetDouble(FID_ACCRUED_AMOUNT, basicPage.accruedAmount_);
    reply.SetString(FID_MATURITY, basicPage.maturityDate_);
    reply.SetString(FID_EXECUTION_DATE, basicPage.executionDate_);
    reply.SetDouble(FID_MACAULAY_DURATION, basicPage.MacaulayDuration_);
    reply.SetDouble(FID_MODIFIED_DURATION, basicPage.modifiedDuration_);
    reply.SetDouble(FID_PVBP, basicPage.pvbp_);
    reply.SetDouble(FID_CONVEXITY, basicPage.convexity_);
    reply.SetString(FID_CALCULATOR_ERROR_MESSAGE, GBKToUTF8(basicPage.exceptions_));

    reply.SetDouble(FID_IS_FRN_BOND, is_frn_bond);
    reply.SetDouble(FID_IS_YTC_AVAILABLE, is_ytc_available);
    reply.SetDouble(FID_IS_YTP_AVAILABLE, is_ytp_available);
}

}  // namespace bcserver_service_calculator
