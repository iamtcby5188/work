#include "oper_log.h"
#include "include/fid.h"
#include "include/common.h"
#include "include/constants.h"
#include "cache/controller/chinabondvaluation_cachecontroller.h"
#include "cache/controller/account_cachecontroller.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

// fid与表中字段的映射
static std::map<int, std::string> fid_map = {
	{ FID_OPER_TIME, "oper_time" },
	{ FID_OPER_TYPE, "method" },
	{ FID_ID, "offer_id" },
	{ FID_DEAL_ID, "deal_id" },
	{ FID_BOND_KEY_LISTED_MARKET, "bond_key_listed_market" }
};

void OperLog::GetQuoteOperLog(const sdbus::Message* msg, sdbus::Message& reply)
{
	GetOperLog("bond_offer_log", msg, reply, boost::bind(&OperLog::QuoteRes2vm, this, _1, _2));
}

void OperLog::GetDealOperLog(const sdbus::Message* msg, sdbus::Message& reply)
{
	GetOperLog("bond_deal_log", msg, reply, boost::bind(&OperLog::DealRes2vm, this, _1, _2));
}

void OperLog::GetOperLog(const std::string &tableName, const sdbus::Message* msg, sdbus::Message &reply, const RowOperFunc &rowOper)
{
	std::string cond, sortby, range;
	msg->GetString(FID_CONDITIONS, cond);
	msg->GetString(FID_SORT_BY, sortby);
	msg->GetString(FID_RANGE, range);

	cond = ParseCondition(cond, fid_map);
	sortby = ParseSortBy(sortby, fid_map, "desc");
	range = ParseRange(range);

	std::ostringstream countSql, sql;
	countSql << "SELECT COUNT(*) FROM " << tableName << " WHERE " << cond;
	sql << "SELECT * FROM " << tableName << " WHERE " << cond << " " << sortby << " " << range;
	LOGGER_INFO("GetOperLog, sql:" << sql.str());

	int rowsCount = getRowsCount(countSql.str().c_str());
	if (rowsCount == 0) {
		return;
	}

	sql::ResultSet *res = DBConnector::getInstance()->executeQuery(sql.str().c_str(), SCHEMA_IDB_BOND);
	if (res) {

		reply.SetInt32(FID_ROW_COUNT, rowsCount);
		std::map<std::string, std::string> accountName = getAccountTable();
		sdbus::VariantList *list = reply.AddList(FID_LIST);
		
		while (res->next()) {
			sdbus::VariantMap *vm = list->AddMap();
			// 初始化债券信息
			std::string bondKeyListedMarked = res->getString("bond_key_listed_market");
			BondSmallViewCachePtr curSmallViewPtr = getBondSmallView(bondKeyListedMarked);
			if (curSmallViewPtr) {
				BondSmallViewChache2Map(curSmallViewPtr.get(), vm);
			}
			// 使用日志表中的新数值覆盖
			// 公共属性
			VariantMapPtr curCDCVariantPtr = getCDCValuations(bondKeyListedMarked);
			if (curCDCVariantPtr) {
				double tmpDouble;
				if (curCDCVariantPtr->GetDouble(FID_CDC_VALUATIONS_YIELD, tmpDouble)) {
					vm->SetDouble(FID_CDC_VALUATIONS_YIELD, tmpDouble); // 估价收益率
				}
				if (curCDCVariantPtr->GetDouble(FID_CDC_VALUATIONS_CLEAN_PRICE, tmpDouble)) {
					vm->SetDouble(FID_CDC_VALUATIONS_CLEAN_PRICE, tmpDouble); // 估价净价
				}
			}
			vm->SetString(FID_BOND_KEY_LISTED_MARKET, bondKeyListedMarked);
			vm->SetString(FID_OPER_TIME, res->getString("oper_time"));  // 操作时间
			vm->SetString(FID_OPER_TYPE, res->getString("method")); // 操作类型
			vm->SetString(FID_OPER_USERNAME, accountName[res->getString("oper_id")]);
			rowOper(res, vm);
		}
		delete res;
	}
}

void OperLog::BondSmallViewChache2Map(const BondSmallViewCache *cache, sdbus::VariantMap *mp)
{
	if (!cache || !mp) {
		return;
	}

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
	mp->SetString(FID_ISSUE_INSTITUTION_RATING, cache->institution_rating);
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
	mp->SetString(FID_MATURITY_DATE_TYPE, cache->maturity_date_type);
	mp->SetInt32(FID_MATURITY_HOLIDAYS, cache->maturity_holidays);
	// ------------------------- Combined Fields (above) ----------------------------
	mp->SetString(FID_TIME_TO_MATURITY, cache->time_to_maturity);
	mp->SetDouble(FID_TIME_TO_MATURITY_REAL, cache->time_to_maturity_real);
	//cache->ttm = GetTimeToMaturity(cache->maturity_date, cache->interest_start_date);
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
	mp->SetString(FID_ISSUE_PROVINCE_CODE, cache->province_code);
	mp->SetString(FID_FINANCING_PLATFORM, cache->financing_platform);
	mp->SetString(FID_ISSUING_INSTITUTION, cache->issue_institution_id);
	mp->SetString(FID_COMPLETED_BOND_TYPE, cache->completed_bond_type);
	mp->SetInt32(FID_COMPLETED_QUOTE_SORT, cache->bond_type_sort_client);
	mp->SetString(FID_ACROSS_MARKET_BOND_CODES, cache->across_market_bond_ids);
	mp->SetString(FID_COMPANY_BOND, cache->ent_cor);
	mp->SetString(FID_OUTLOOK, cache->outlook); //Add by Young 20-Apr-2016
}

BondSmallViewCachePtr OperLog::getBondSmallView(const std::string &listedMarket)
{
	std::unordered_map<std::string, BondSmallViewCachePtr>::iterator iter = smallViewCacheMap_.find(listedMarket);
	if (iter != smallViewCacheMap_.end()) {
		return iter->second;
	}

	std::string str = "bond_key_listed_market = '" + listedMarket + "'";
	dbQuery query(str.c_str());
	BondSmallViewCacheController bondSmallViewCC;
	BondSmallViewCacheVecPtr smallViewPtr = bondSmallViewCC.getCacheListByQueryInThreadSafty(query);
	if (smallViewPtr->size() > 0) {
		BondSmallViewCachePtr result = smallViewPtr->at(0);
		smallViewCacheMap_[listedMarket] = result;
		return result;
	}
	return BondSmallViewCachePtr();
}

VariantMapPtr OperLog::getCDCValuations(const std::string &listMarked)
{
	std::unordered_map<std::string, VariantMapPtr>::iterator iter = cdcValuationsMap_.find(listMarked);
	if (iter != cdcValuationsMap_.end()) {
		return iter->second;
	}

	VariantMapPtr result(new sdbus::VariantMap());
	ChinaBondValuationCacheController chinabondvaluation_cc;
	dbQuery q;
	q = "bond_key_listed_market = ", listMarked;
	ChinaBondValuationCacheVecPtr chinabondvaluations = chinabondvaluation_cc.getCacheListByQueryInThreadSafty(q);
	std::vector<ChinaBondValuationCachePtr>::const_iterator it = chinabondvaluations->begin();
	for (; it != chinabondvaluations->end(); ++it) {
		ChinaBondValuationCache* chinabondvaluation = it->get();
		std::string valuation_yield = chinabondvaluation->valuation_yield;
		std::string::size_type index = valuation_yield.find("|");
		if (index != std::string::npos) {
			std::string tmp = valuation_yield.substr(0, index);
			valuation_yield = tmp;
		}

		result->SetString(FID_CDC_VALUATIONS_YIELD, valuation_yield);
		result->SetString(FID_CDC_VALUATIONS_CLEAN_PRICE, chinabondvaluation->valuation_net_price);
		return result;
	}

	return VariantMapPtr();
}

std::string OperLog::getBrokerName(const std::string &operatorId)
{
	std::unordered_map<std::string, std::string>::iterator iter = brokerNameMap_.find(operatorId);
	if (iter != brokerNameMap_.end()) {
		return iter->second;
	}

	std::string str = "id = '" + operatorId + "'";
	dbQuery query(str.c_str());
	AccountCacheController accountCC;
	AccountCachePtr cache = accountCC.getCacheByQueryInThreadSafty(query);
	if (cache) {
		brokerNameMap_[operatorId] = cache->display_name;
		return cache->display_name;
	}
	return "";
}


int OperLog::getRowsCount(const char *countSql) 
{
	int result = 0;
	sql::ResultSet *res = DBConnector::getInstance()->executeQuery(countSql, SCHEMA_IDB_BOND);
	if (res) {
		if (res->next()) {
			result = res->getInt(1);
		}
		delete res;
	}
	return result;
};

std::map<std::string, std::string> OperLog::getAccountTable() 
{
	const std::string sql = "SELECT id, username FROM idb_account";
	sql::ResultSet *res = DBConnector::getInstance()->executeQuery(sql, SCHEMA_IDB_ACCOUNT);
	std::map<std::string, std::string> result;
	if (res) {
		while (res->next()) {
			result[res->getString("id")] = res->getString("username");
		}
		delete res;
	}
	return result;
};

void OperLog::QuoteRes2vm(sql::ResultSet *res, sdbus::VariantMap *vm)
{
	vm->SetString(FID_BOND_CODE, res->getString("goods_code")); // 代码
	vm->SetString(FID_BOND_SHORT_NAME, res->getString("goods_short_name")); // 简称

	// 计算价格字段和返点字段
	double price = res->getDouble("price");
	std::string rebate = res->getString("rebate");
	double returnPoint = res->getDouble("return_point");
	int symbol = res->getInt("symbol");
	std::string price_string, fan_dian_string;
	GetPriceStrings(price, rebate, returnPoint, symbol, 0, price_string, fan_dian_string);

	if (symbol == kBidQuote) {
		vm->SetDouble(FID_BID_PRICE, price);
		vm->SetDouble(FID_BID_FAN_DIAN, returnPoint);
		vm->SetString(FID_BID_FAN_DIAN_FLAG, rebate);
		vm->SetString(FID_BID_INTERNAL_FLAG, res->getString("internally"));
		vm->SetString(FID_BID_DESCRIPTION, res->getString("price_description"));
		vm->SetString(FID_BID_BARGAIN_FLAG, res->getString("flag_bargain"));
		vm->SetString(FID_BID_OCO_FLAG, res->getString("flag_relation"));
		// 价格和返点字符串
		vm->SetString(FID_BID_PRICE_STRING, price_string);
		vm->SetString(FID_BID_FAN_DIAN_STRING, fan_dian_string);
	} else {
		vm->SetDouble(FID_OFR_PRICE, price);
		vm->SetDouble(FID_OFR_FAN_DIAN, returnPoint);
		vm->SetString(FID_OFR_FAN_DIAN_FLAG, rebate);
		vm->SetString(FID_OFR_INTERNAL_FLAG, res->getString("internally"));
		vm->SetString(FID_OFR_DESCRIPTION, res->getString("price_description"));
		vm->SetString(FID_OFR_BARGAIN_FLAG, res->getString("flag_bargain"));
		vm->SetString(FID_OFR_OCO_FLAG, res->getString("flag_relation"));
		// 价格和返点字符串
		vm->SetString(FID_OFR_PRICE_STRING, price_string);
		vm->SetString(FID_OFR_FAN_DIAN_STRING, fan_dian_string);
	}

	vm->SetDouble(FID_VOLUME, res->getDouble("volume")); // Volume
	vm->SetString(FID_BROKER_NAME, getBrokerName(res->getString("operator_id"))); // Broker
	vm->SetString(FID_ISSUER_RATING, res->getString("goods_level"));
	vm->SetString(FID_CREATE_TIME, res->getString("create_time"));
}

void OperLog::DealRes2vm(sql::ResultSet *res, sdbus::VariantMap *vm)
{
	vm->SetString(FID_BOND_CODE, res->getString("goods_code")); // 代码
	vm->SetString(FID_BOND_SHORT_NAME, res->getString("goods_short_name")); // 简称
	vm->SetDouble(FID_PRICE, res->getDouble("price")); // Px
	vm->SetDouble(FID_VOLUME, res->getDouble("volume")); // Vol
	vm->SetString(FID_BID_BROKER_NAME_A, res->getString("bid_broker_name")); // Broker(B);
	vm->SetString(FID_OFR_BROKER_NAME_A, res->getString("ofr_broker_name")); // Broker(O);
	vm->SetString(FID_DEAL_TIME, res->getString("deal_time").substr(0, 10)); // 交易日
	vm->SetString(FID_SETTLEMENT_DATE, res->getString("settlement_date")); // 交割日
	vm->SetString(FID_CREATE_TIME, res->getString("create_time")); // 成交日
	vm->SetDouble(FID_CLEAN_PRICE, res->getDouble("net_price")); // 估价净价
	vm->SetDouble(FID_YIELD, res->getDouble("yield")); // 估价收益率(%)

	std::string bidTraderName = res->getString("bid_bank_code");
	std::string bidBankAgentName = res->getString("bidBankAgentName");
	bidTraderName += bidBankAgentName.empty() ? "" : "(" + bidBankAgentName + ")";
	std::string ofrTraderName = res->getString("ofr_bank_code");
	std::string ofrBankAgentName = res->getString("ofrBankAgentName");
	ofrTraderName += ofrTraderName.empty() ? "" : "(" + ofrBankAgentName + ")";
	vm->SetString(FID_BID_INSTITUTION_TRADER_NAMES, bidTraderName); // CP.Bid
	vm->SetString(FID_OFR_INSTITUTION_TRADER_NAMES, ofrTraderName); // CP.Ofr
}