#ifndef _OPER_LOG_H_
#define _OPER_LOG_H_
 
#include <string>
#include <unordered_map>
#include <boost/function.hpp>
#include <memory>
#include "cache/controller/bondsmallview_cachecontroller.h"

namespace sdbus {
	class Message;
	class VariantMap;
}
namespace sql {
	class ResultSet;
}

typedef boost::function<void(sql::ResultSet *from, sdbus::VariantMap *to)> RowOperFunc;
typedef  std::tr1::shared_ptr<sdbus::VariantMap> VariantMapPtr;

class OperLog {
public:
	void GetQuoteOperLog(const sdbus::Message* msg, sdbus::Message& reply);
	void GetDealOperLog(const sdbus::Message* msg, sdbus::Message& reply);
	void GetOperLog(const std::string &tableName, const sdbus::Message* msg, sdbus::Message &reply, const RowOperFunc &rowOper);

	static void BondSmallViewChache2Map(const BondSmallViewCache *cache, sdbus::VariantMap *mp);

	BondSmallViewCachePtr getBondSmallView(const std::string &listedMarket);
	VariantMapPtr getCDCValuations(const std::string &listMarked);
	std::string getBrokerName(const std::string &operatorId);

private:
	int getRowsCount(const char *countSql);
	std::map<std::string, std::string> getAccountTable();

	void QuoteRes2vm(sql::ResultSet *res, sdbus::VariantMap *vm);
	void DealRes2vm(sql::ResultSet *res, sdbus::VariantMap *vm);

private:
	std::unordered_map<std::string, BondSmallViewCachePtr> smallViewCacheMap_;
	std::unordered_map<std::string, VariantMapPtr> cdcValuationsMap_;
	std::unordered_map<std::string, std::string> brokerNameMap_;
};

#endif // _OPER_LOG_H_
