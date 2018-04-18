#ifndef _MESSAGERESENDSERVICE_H_
#define _MESSAGERESENDSERVICE_H_

#if !defined(_MSC_VER)
#include <tr1/memory>
#endif
#include "logger.h"
#include "qpid/messaging/Connection.h"
#include "qpid/messaging/Session.h"
#include "qpid/messaging/Receiver.h"
#include "qpid/messaging/Sender.h"
#include "qpid/messaging/Address.h"
#include "qpid/messaging/Message.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include "object/bonddeal.h"
#include "object/quote.h"
#include "cache/controller/bonddeal_cachecontroller.h"
#include "business/bondquote_business.h"
#include <boost/property_tree/ptree.hpp>
#include <queue>

using namespace boost::property_tree;

class MessageResendService
{
public:
    static MessageResendService* singleton();

	~MessageResendService();

	bool Open(const std::string& url) ;
	void Close() ;

	inline const std::string& ServiceName() const {return _service_name;}
	inline void setServiceName(const std::string& name) { _service_name = name;}
	inline const std::string& ServiceNameForReciever() const {return _service_name_for_recieve;}
	inline void setServiceNameForReciever(const std::string& name) { _service_name_for_recieve = name;}

    void PushMsg(const std::string& text);

    void threadSendMsg();
	void threadDoRecieveMsgs();

	bool RecieveRequest();

private:
    MessageResendService();
	void SendRequest(const std::string& text, const std::string subject);

	void AssembleBondDeal(BondDealPtr bond_deal, ptree* each_bond_deal_data);
	void AssembleBondQuote(BondQuotePtr bond_quote, ptree* each_bond_quote_data);

	void AUpdateBondDeal(const ptree* pt);
	void DeleteBondDeal(const ptree* pt);
	void AUpdateBondQuote(const ptree* pt);
	void UpdateBondQuote(const ptree* pt);
	void ReferQuote(const ptree* pt);
	void DeleteReferredQuote(const ptree* pt);
	void UnReferQuote(const ptree* pt);
	void DeleteQuote(const ptree* pt);
	void UpdateReferredQuote(const ptree* pt);

	void AddUpdateSingleQuote(BondQuotePtr bond_quote, BondQuoteBusiness::BondQuoteVecPtr &add_vec, BondQuoteBusiness::BondQuoteVecPtr &update_vec);
	void EditReferredQuoteSingle(BondQuotePtr bond_quote);

	double getDouble(std::string key, ptree* pt, double default_value);
	bool getBoolean(std::string key, ptree* pt, bool default_value);

	std::string _service_name;
	std::string _service_name_for_recieve;
	std::string _type;
    
    std::queue<std::string> msg_queue_;
    boost::mutex queue_mutex_;
    boost::condition_variable_any queue_cond_var_;

	::qpid::messaging::Connection* _connection;
	bool _finaled;

    static MessageResendService* singleton_;

};

#endif
