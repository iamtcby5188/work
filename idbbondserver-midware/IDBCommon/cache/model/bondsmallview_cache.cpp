/*
 * BondSmallViewCache.cpp
 *
 *  Created on: May 22, 2014
 *      Author: theochen
 */

#include "cache_def.h"
#include "include/common.h"
#include "bondsmallview_cache.h"
#include "cache/cache_controller_declear.h"

std::string BondSmallViewCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
std::string BondSmallViewCache::TABLE_NAME = "bond_small_view";
std::string BondSmallViewCache::PKEY_NAME = "bond_key_listed_market";
std::string BondSmallViewCache::SQL_TAG = "FetchBondSmallViewSQL";

BondSmallViewCache::BondSmallViewCache() {
	// TODO Auto-generated constructor stub
	// real8
	coupon_rate_current = DOUBLE_NULL;
	coupon_rate_spread = DOUBLE_NULL;
	issue_amount = DOUBLE_NULL;

	// double
	ttm = DOUBLE_NULL;
	time_to_maturity_real = DOUBLE_NULL;

	// bool
	is_expired = false;

	// int8
	version = -1;

	// int4
	rating_date = -1;
	delflag = -1;
	maturity_date = -1;
	listed_date = -1;
	delisted_date = -1;
	next_coupon_date = -1;
	redemption_no = -1;
	interest_start_date = -1;
	fixing_ma_days = -1;
	option_date = -1;
	issue_year = -1;
	sort_remain = -1;

	// int
	maturity_holidays = -1; // 到期日的节假日天数
	issuer_rating_order = -1;
	bond_type_sort_client = -1;

	// time_t

	// string
}

BondSmallViewCache::~BondSmallViewCache() {
	// TODO Auto-generated destructor stub
}

std::string BondSmallViewCache::getPrimaryKey() {
	return bond_key_listed_market;
}

bool BondSmallViewCache::operator==(const BondSmallViewCache &rhs) {
	return isCacheEqual(*this, rhs);
}

bool BondSmallViewCache::operator!=(const BondSmallViewCache &rhs) {
	return !operator==(rhs);
}

static std::unordered_map<std::string, SelectiveInfoCacheVecPtr> selectiveInfoCacheMap; // key: selective_code, value: SelectiveInfoCacheListPtr
static SelectiveInfoCacheVecPtr tmp_selective_info_cache_vec;
void BondSmallViewCache::memCacheInit() {
	LOGGER_DEBUG("selectiveInfoCacheMap init mem cache ...");
	memCacheClear();
	dbQuery q;
	q = "order by selective_code, group_name";
	SelectiveInfoCacheController selective_info_cc;
	tmp_selective_info_cache_vec = selective_info_cc.getCacheListByQueryInThreadSafty(q);
	if (!tmp_selective_info_cache_vec->empty()) {
		for (std::vector<SelectiveInfoCachePtr>::iterator it = tmp_selective_info_cache_vec->begin(); it != tmp_selective_info_cache_vec->end(); ++it) {
			std::unordered_map<std::string, SelectiveInfoCacheVecPtr>::iterator iter = selectiveInfoCacheMap.find((*it)->selective_code);
			if (iter == selectiveInfoCacheMap.end()) {
				SelectiveInfoCacheVecPtr vec(new std::vector<SelectiveInfoCachePtr>());
				vec->push_back(*it);
				selectiveInfoCacheMap[(*it)->selective_code] = vec;
			} else {
				SelectiveInfoCacheVecPtr vec = selectiveInfoCacheMap[(*it)->selective_code];
				vec->push_back(*it);
			}
		}
	}
	LOGGER_DEBUG("selectiveInfoCacheMap init mem cache success. size:" << selectiveInfoCacheMap.size());
}

void BondSmallViewCache::memCacheClear() {
	//tmp_selective_info_cache_list->clear();
	selectiveInfoCacheMap.clear();
}

void BondSmallViewCache::loadCombinationFields() {
	loadIssuingInstitution();
	loadWarranterInstitution();
	loadUnderwriterInstitution();
	loadIusserRatingInstitution();
	loadRatingInstitution();
	loadRatingAugmentClient();
	loadCouponTypeClient();
	//loadChinaBondValuationFields();
	loadHolidayInfoFields();
	loadTimeToMaturityFields();
	loadHasOptionFields();
	loadIssuerRatingFilter();
	loadGuarenteed();
	loadOtherFields();
	loadIssuerInfoFields();
	across_market_bond_ids = bond_id;
}

void BondSmallViewCache::loadIssuingInstitution() {
	//IssuingInstitutionCacheController issuingInstitutionCC;				 
	//dbQuery q;
	//q = "institution_code = ", issuer_code;
	//IssuingInstitutionCachePtr issuingInstitution = issuingInstitutionCC.getCacheByQueryInThreadSafty(q);
	IssuingInstitutionCachePtr issuingInstitution = IssuingInstitutionCacheController::getCacheInMem("institution_code", issuer_code);
	if (issuingInstitution == NULL) {
		institution_type = GBKToUTF8("其他");
		issuer_client = "";
		issue_institution_pinyin_ = "";
		issue_institution_pinyinfull_ = "";
		issue_institution_id = "";
		institution_rating = "";
	} else {
		if (issuingInstitution->institution_subtype.length() == 0) {
			institution_type = GBKToUTF8("其他");
		} else {
			std::string _inst_type;
			if (useCacheInMem && !selectiveInfoCacheMap.empty()) {
				std::unordered_map<std::string, SelectiveInfoCacheVecPtr>::iterator it = selectiveInfoCacheMap.find(issuingInstitution->institution_subtype);
				if (it != selectiveInfoCacheMap.end()) {
					SelectiveInfoCacheVecPtr selective_info_cache_vec = selectiveInfoCacheMap[issuingInstitution->institution_subtype];
					for (std::vector<SelectiveInfoCachePtr>::const_iterator iter = selective_info_cache_vec->begin(); iter != selective_info_cache_vec->end(); ++iter) {
						if ("institution_subtype" == (*iter)->group_name) {
							_inst_type = (*iter)->selective_content;
							break;
						}
					}
				}
			} else {
				SelectiveInfoCacheController selectiveInfoCC;
				dbQuery q;
				q = "group_name = ", "institution_subtype", " and selective_code = ", issuingInstitution->institution_subtype;
				SelectiveInfoCachePtr cache = selectiveInfoCC.getCacheByQueryInThreadSafty(q);
				if (cache)
					_inst_type = cache->selective_content;
			}
			if (!_inst_type.empty()) {
				institution_type = _inst_type;
			} else {
				institution_type = GBKToUTF8("其他");
			}
		}

		//institution_type = issuingInstitution->institution_subtype;
		issuer_client = issuingInstitution->full_name_c;
		issue_institution_pinyin_ = issuingInstitution->Pin_Yin;
		issue_institution_pinyinfull_ = issuingInstitution->Pin_Yin_Full;
		institution_rating = issuingInstitution->institution_rating;
		issue_institution_id = issuingInstitution->id;
	}
}

void BondSmallViewCache::loadWarranterInstitution() {
	//IssuingInstitutionCacheController issuingInstitutionCC;
	//dbQuery q;
	//q = "institution_code = ", warranter;
	//IssuingInstitutionCachePtr warranterInstitution = issuingInstitutionCC.getCacheByQueryInThreadSafty(q);
	IssuingInstitutionCachePtr warranterInstitution = IssuingInstitutionCacheController::getCacheInMem("institution_code", warranter);
	if (warranterInstitution == NULL) {
		warranter_client = "";
	} else {
		warranter_client = warranterInstitution->full_name_c;
	}
}

void BondSmallViewCache::loadUnderwriterInstitution() {
	//IssuingInstitutionCacheController issuingInstitutionCC;
	//dbQuery q;
	//q = "institution_code = ", this->underwriter_code;
	//IssuingInstitutionCachePtr underwriterInstitution = issuingInstitutionCC.getCacheByQueryInThreadSafty(q);
	IssuingInstitutionCachePtr underwriterInstitution = IssuingInstitutionCacheController::getCacheInMem("institution_code", underwriter_code);
	if (underwriterInstitution == NULL) {
		underwriter_client = "";
	} else {
		underwriter_client = underwriterInstitution->full_name_c;
	}

}

void BondSmallViewCache::loadIusserRatingInstitution() {
	//IssuingInstitutionCacheController issuingInstitutionCC;
	//dbQuery q;
	//q = "institution_code = ", issuer_rating_institution_code;
	//IssuingInstitutionCachePtr issuerRatingInstitution = issuingInstitutionCC.getCacheByQueryInThreadSafty(q);
	IssuingInstitutionCachePtr issuerRatingInstitution = IssuingInstitutionCacheController::getCacheInMem("institution_code", issuer_rating_institution_code);
	if (issuerRatingInstitution == NULL) {
		issuer_rating_institution_client = "";
	} else {
		issuer_rating_institution_client = issuerRatingInstitution->full_name_c;
	}
}

void BondSmallViewCache::loadRatingInstitution() {
	//IssuingInstitutionCacheController issuingInstitutionCC;
	//dbQuery q;
	//q = "institution_code = ", rating_institution_code;
	//IssuingInstitutionCachePtr ratingInstitution = issuingInstitutionCC.getCacheByQueryInThreadSafty(q);
	IssuingInstitutionCachePtr ratingInstitution = IssuingInstitutionCacheController::getCacheInMem("institution_code", rating_institution_code);
	if (ratingInstitution == NULL) {
		rating_institution_client = "";
	} else {
		rating_institution_client = ratingInstitution->full_name_c;
	}
}

void BondSmallViewCache::loadCouponTypeClient() {
	coupon_type_client = "";
	if (coupon_type == "FRN") {
		if (coupon_rate_spread != 0) {
			char buff[64] = { 0 };
			sprintf(buff, "%s %dD", frn_index_id.c_str(), fixing_ma_days);
			coupon_type_client = buff;
			if (coupon_rate_spread > 0) {
				coupon_type_client += "+";
			}
			std::stringstream ssSpread;
			ssSpread << coupon_rate_spread << "%";
			coupon_type_client += ssSpread.str();
		}
	} else {
		if (coupon_type.length() > 0) {
			if (useCacheInMem && !selectiveInfoCacheMap.empty()) {
				std::unordered_map<std::string, SelectiveInfoCacheVecPtr>::iterator it = selectiveInfoCacheMap.find(coupon_type);
				if (it != selectiveInfoCacheMap.end()) {
					SelectiveInfoCacheVecPtr selective_info_cache_vec = selectiveInfoCacheMap[coupon_type];
					for (std::vector<SelectiveInfoCachePtr>::const_iterator iter = selective_info_cache_vec->begin(); iter != selective_info_cache_vec->end(); ++iter) {
						if ("Coupon_Type" == (*iter)->group_name) {
							coupon_type_client = (*iter)->selective_content;
							break;
						}
					}
				}
			} else {
				SelectiveInfoCacheController selectiveInfoCC;
				dbQuery q;
				q = "group_name = ", "Coupon_Type", " and selective_code = ", coupon_type;
				SelectiveInfoCachePtr couponTypeInfo = selectiveInfoCC.getCacheByQueryInThreadSafty(q);
				if (couponTypeInfo) {
					coupon_type_client = couponTypeInfo->selective_content;
				}
			}
		}
	}
}

void BondSmallViewCache::loadRatingAugmentClient() {
	if (rating_augment.length() > 0) {
		std::vector<std::string> arr = split(rating_augment, '|');
		for (size_t i = 0; i < arr.size(); ++i) {
			std::string str;
			if (useCacheInMem && !selectiveInfoCacheMap.empty()) {
				std::unordered_map<std::string, SelectiveInfoCacheVecPtr>::iterator it = selectiveInfoCacheMap.find(arr[i]);
				if (it != selectiveInfoCacheMap.end()) {
					SelectiveInfoCacheVecPtr selective_info_cache_vec = selectiveInfoCacheMap[arr[i]];
					for (std::vector<SelectiveInfoCachePtr>::const_iterator iter = selective_info_cache_vec->begin(); iter != selective_info_cache_vec->end(); ++iter) {
						if ("Rating_Augment" == (*iter)->group_name) {
							str = (*iter)->selective_content;
							break;
						}
					}
				}
			} else {
				SelectiveInfoCacheController selectiveInfoCC;
				dbQuery q;
				q = "group_name = ", "Rating_Augment", " and selective_code = ", arr[i];
				SelectiveInfoCachePtr selectiveInfoCache = selectiveInfoCC.getCacheByQueryInThreadSafty(q);
				if (selectiveInfoCache)
					str = selectiveInfoCache->selective_content;
			}
			arr[i] = str;
		}

		rating_augment_client = join(arr, "|");
		//cout << rating_augment_client << endl;

	} else {
		rating_augment_client = "";
	}
}
/*
void BondSmallViewCache::loadChinaBondValuationFields()
{
if (bond_id.length() == 0) {
return;
}

ChinaBondValuationCacheController china_bond_valuation_cc;
dbQuery q;
std::string datetime = GetTDateString("%Y%m%d");
q = "bond_id = ", bond_id, " and valuation_date = ", datetime, " order by valuation_date desc limit 1";
ChinaBondValuationCachePtr china_bond_valuation_cache = china_bond_valuation_cc.getCacheByQueryInThreadSafty(q);

if (china_bond_valuation_cache) {
cdc_valuation_yield = china_bond_valuation_cache->valuation_yield;
cdc_valuation_net_price = china_bond_valuation_cache->valuation_net_price;
} else {
cdc_valuation_yield = "";
cdc_valuation_net_price = "";
}
}*/

void BondSmallViewCache::loadHolidayInfoFields() {
	std::string date = IntToString(maturity_date);
	if (date.length() != 8) {
		maturity_date_type = kMaturityDateNormal;
		maturity_holidays = -1;
		return;
	}

	// check and set maturity_date holidays
	maturity_holidays = -1;
	std::string t_date = dateShortToLong(date);
	if (lessThan1Year(t_date)) {
		maturity_holidays = 0;
		const std::vector<std::string>& vecs = getHolidayCache();
		bool contains = (std::find(vecs.begin(), vecs.end(), t_date) != vecs.end());
		if (contains) {
			boost::gregorian::date start = strToDate(dateShortToLong(date));
			boost::gregorian::date end = getNextWorkDate(vecs, strToDate(dateShortToLong(date)));
			maturity_holidays = dateDuration(start, end).days();
		}
	}

	std::string year = date.substr(0, 4);
	std::string month = date.substr(4, 2);
	std::string day = date.substr(6, 2);

	size_t iyear = atoi(year.c_str());
	size_t imonth = atoi(month.c_str());
	size_t iday = atoi(day.c_str());

	size_t weekday = getWeekDay(iyear, imonth, iday);
	switch (weekday) {
	case 0:
		maturity_date_type = kMaturityDateSunday;
		return;
	case 6:
		maturity_date_type = kMaturityDateSaturday;
		return;
	default:
	{
		std::string str_maturity_date = year + "-" + month + "-" + day;
		//HolidayInfoCacheController holiday_info_cc;
		//std::string q = GetFastdbQuery(std::string("holiday_date"), str_maturity_date);
		//HolidayInfoCachePtr holiday_info_cache = holiday_info_cc.getCacheByQuery(q);
		HolidayInfoCachePtr holiday_info_cache = HolidayInfoCacheController::getCacheInMem("holiday_date", str_maturity_date);
		if (holiday_info_cache != NULL) {
			maturity_date_type = kMaturityDateHoliday;
		} else {
			maturity_date_type = kMaturityDateNormal;
		}
		return;
	}
	}
}

void BondSmallViewCache::loadTimeToMaturityFields() {
	time_to_maturity = GetTimeToMaturity(maturity_date, interest_start_date, option_type_client);
	if (option_type == "ETS") {
		time_to_maturity += "+NY";
	}
	ttm = GetTimeToMaturity(maturity_date, interest_start_date);
	time_to_maturity_real = GetTimeToMaturityReal(maturity_date, interest_start_date, option_type_client);
}

void BondSmallViewCache::loadHasOptionFields() {
	if (option_type == "ETS" && isBCOCategoryType(product_type)) {
		has_option = "ETS";
	} else if ((option_type == "PUT" || option_type == "CAL" || option_type == "CNP" || option_type == "ETS") && option_type_client.length() > 0) {
		has_option = "Y";
	} else {
		has_option = "N";
	}
	if (useCacheInMem && !selectiveInfoCacheMap.empty()) {
		std::unordered_map<std::string, SelectiveInfoCacheVecPtr>::iterator it = selectiveInfoCacheMap.find(option_type);
		if (it != selectiveInfoCacheMap.end()) {
			SelectiveInfoCacheVecPtr selective_info_cache_vec = selectiveInfoCacheMap[option_type];
			if (!selective_info_cache_vec->empty()) {
				option_client = (*selective_info_cache_vec->begin())->group_name;
			}
		}
	} else {
		SelectiveInfoCacheController selective_info_cc;
		dbQuery q;
		q = "selective_code = ", option_type, "limit 1";
		SelectiveInfoCachePtr selective_info_cache = selective_info_cc.getCacheByQueryInThreadSafty(q);
		if (selective_info_cache)
			option_client = selective_info_cache->selective_content;
	}
	//has_option = option_type_client.length() > 0? "Y": "N";
}

void BondSmallViewCache::loadIssuerRatingFilter() {
	if (issuer_rating_current != "AAA" &&
		issuer_rating_current != "AA+" &&
		issuer_rating_current != "AAA-" &&
		issuer_rating_current != "AA" &&
		issuer_rating_current != "AA-" &&
		issuer_rating_current != "A+") {
		issuer_rating_filter = "Other";
	} else {
		issuer_rating_filter = issuer_rating_current;
	}
}

void BondSmallViewCache::loadGuarenteed() {
	if (rating_augment.length() > 0 && rating_augment != "NON" && rating_augment != "LIQ") {
		guarenteed = "Y";
	} else {
		guarenteed = "N";
	}
}

void BondSmallViewCache::loadOtherFields() {
	municipal = is_municipal == "Y" ? "Y" : "N";
	rate_index = ParseRateType(frn_index_id, next_coupon_date, maturity_date);
	pledged = is_mortgage == "Y" ? "Y" : "N";
	across_market = is_cross_mkt == "Y" ? "Y" : "N";
	issuer_rating_order = ParseRatingStr(issuer_rating_current);
	completed_bond_type = GetCompletedBondType(goods_type_client, frn_index_id, option_type);
	bond_type_sort_client = GetSortIndexByBondType(completed_bond_type);

	company_bond = ent_cor;
	on_the_run = CheckOnTheRun(listed_date);
}

void BondSmallViewCache::loadIssuerInfoFields() {
	//IssuerInfoCacheController issuer_info_cc;
	//std::string q = GetFastdbQuery(std::string("institution_code"), issuer_code);
	//IssuerInfoCachePtr issuer_info_cache = issuer_info_cc.getCacheByQuery(q);
	IssuerInfoCachePtr issuer_info_cache = IssuerInfoCacheController::getCacheInMem("institution_code", issuer_code);
	if (issuer_info_cache != NULL) {
		sw_sector = issuer_info_cache->sw_sector;
		sw_subsector = issuer_info_cache->sw_subsector;
		sw_sector_code = issuer_info_cache->sw_sector_code;
		sw_subsector_code = issuer_info_cache->sw_subsector_code;
		province = issuer_info_cache->province;
		province_code = issuer_info_cache->province_code;
		financing_platform = issuer_info_cache->CBRC_Financing_Platform.empty() ? "N" : "Y";
	} else {
		sw_sector = "";
		sw_subsector = "";
		sw_sector_code = "";
		sw_subsector_code = "";
		province = "";
		province_code = "";
		financing_platform = "N";
	}
}

void BondSmallViewCache::loadIssuerRatingFields() {
	if (!issuer_code.empty()) {
		InstitutionRatingCachePtr cache = InstitutionRatingCacheController::getCacheInMem("institution_code", issuer_code);
		if (cache) {
			issuer_rating_current = cache->rate;
			outlook = cache->outlook;
			return;
		}
	}
	issuer_rating_current = "";
	outlook = "";
}

REGISTER(BondSmallViewCache);

