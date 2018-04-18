#ifndef BCSERVER_SERVICE_CALCULATOR_SERVICE_H_
#define BCSERVER_SERVICE_CALCULATOR_SERVICE_H_

#include "sdbus/connection.h"
#include "logger.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

namespace bcserver_service_calculator {

class CalculatorService : public sdbus::Callback {
public:
    CalculatorService(sdbus::Connection* conn);
    virtual void OnMsgs(const sdbus::Message ** msgs, int count);
   
	void run();
	void loadCalculatorData(const boost::system::error_code& e, boost::asio::deadline_timer* t);

private:
    void BondClassification (const sdbus::Message* msg, sdbus::Message& reply);
    void PriceYieldConversion (const sdbus::Message* msg, sdbus::Message& reply);
    void DateAdvance (const sdbus::Message* msg, sdbus::Message& reply);

    void QuickCalculation(const sdbus::Message* msg, sdbus::Message& reply);

    sdbus::Connection* conn_;
    bool is_ready_;
    
};

}  // namespace bcserver_service_calculator

#endif