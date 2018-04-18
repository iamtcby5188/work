#ifndef _BONDBESTQUOTECALC_BUSINESS_H_
#define _BONDBESTQUOTECALC_BUSINESS_H_

#include <include/constants.h>
#include <map>
#include <vector>
#include <list>
#include <math.h>
#include <cache/model/bondquote_cache.h>
#include <cache/model/bondbestquote_cache.h>
#include "logger.h"
#include "business/BaseBusiness.h" //Add by Young 05-Apr-2016

class better_bid
{
public:
    better_bid(bool ignore_flag = false) 
        : m_ignore_flag(ignore_flag) {};
    bool operator()(const BondQuoteCachePtr &lhs, const BondQuoteCachePtr &rhs)
    {
        double lnet_price = lhs->net_price;
        double rnet_price = rhs->net_price;
        if (lnet_price != DOUBLE_NULL || rnet_price != DOUBLE_NULL) {
            if (fabs(lnet_price - rnet_price) > FLOATING_PRECISION) {
                return lnet_price > rnet_price;  // �� > С > ��   97.52 > 95.43 > DOUBLE_NULL
            }
        }

        //price
        double lprice = lhs->price;
        double rprice = rhs->price;
        if (lprice != DOUBLE_NULL || rprice != DOUBLE_NULL) {
            if (fabs(lprice - rprice) > FLOATING_PRECISION) { 
                if (lprice == DOUBLE_NULL) {
                    return false;
                } else if (rprice == DOUBLE_NULL) {
                    return true;
                } else if (lprice < 30.0 && rprice < 30.0) { // price < 30  ������
                    return lprice < rprice;     // С > �� > ��+���� > ��    3.50 ���� 3.53
                } else if (lprice >= 30.0 && rprice >= 30.0) { // price >30  ����
                    return lprice > rprice;     // �� > С > ��+���� > ��    99.7 ���� 99.1
                } else {
                    return lprice > rprice;     // ���� > ������     99.1 ���� 3.5
                }
            }
        }

        //rebate ����
        std::string lrebate = lhs->rebate;
        std::string rrebate = rhs->rebate;
        double lreturn_point = lhs->return_point;
        double rreturn_point = rhs->return_point;
        lreturn_point = lreturn_point == DOUBLE_NULL ? 99999 : lreturn_point;
        rreturn_point = rreturn_point == DOUBLE_NULL ? 99999 : rreturn_point;

        //�ж������
        bool lbid = false;
        bool rbid = false;
        if(lprice == DOUBLE_NULL && "0" == lrebate) {
            lbid = true;
        }
        if(rprice == DOUBLE_NULL && "0" == rrebate) {
            rbid = true;
        }
        if(rbid != lbid) {
            return lbid < rbid;     // �������С
        }

        //�жϷ���
        //rebate(1) > not rebate(0), not better
        // ������ > ����      �� 3.50  ���� 3.50F0.10
        if (lrebate > rrebate) {
            return false;           
        }
        //not rebate(0) < rebate(1), better
        else if(lrebate < rrebate) {
            return true;
        }
        // both not rebate, compare other parameters
        else if("0" == lrebate && "0" == rrebate) {
            //do nothing;
        }
        else if(fabs(lreturn_point - rreturn_point) > FLOATING_PRECISION) {
            return lreturn_point < rreturn_point;   // С >  �� > ��    ����F0.10 ���� F0.12 ���� F��
        }

        if(m_ignore_flag) {
            return false;
        }

        //flag_bargain �����      0-��    1- *   2-**
        std::string lbargain = lhs->flag_bargain;   
        std::string rbargain = rhs->flag_bargain;
        if(lbargain != rbargain) {
            return lbargain < rbargain;             // 0 > 1 > 2
        }

        //flag_relation  OCO/���     0-��  1- OCO 2-���   
        std::string lrelation = lhs->flag_relation;
        std::string rrelation = rhs->flag_relation;
        if(lrelation != rrelation) {
            return lrelation < rrelation;           // 0 > 1 > 2 
        }

        return lhs->create_time < rhs->create_time;
    }

private:
    bool m_ignore_flag;
};

class better_ofr
{
public:
    better_ofr(bool ignore_flag = false) 
        : m_ignore_flag(ignore_flag) {};
    bool operator()(const BondQuoteCachePtr &lhs, const BondQuoteCachePtr &rhs)
    {
        //rebate
        std::string lrebate = lhs->rebate;
        std::string rrebate = rhs->rebate;
        //net price
        double lnet_price = lhs->net_price;
        double rnet_price = rhs->net_price;
        //price
        double lprice = lhs->price;
        double rprice = rhs->price;
        //return point
        double lreturn_point = lhs->return_point;
        double rreturn_point = rhs->return_point;
        lreturn_point = lreturn_point == DOUBLE_NULL ? 99999 : lreturn_point;
        rreturn_point = rreturn_point == DOUBLE_NULL ? 99999 : rreturn_point;

        //ƽ�۷����
        bool blpar = false;
        bool brpar = false;
        if("1" == lrebate && lreturn_point == DOUBLE_NULL && lprice == DOUBLE_NULL) blpar = true;
        if("1" == rrebate && rreturn_point == DOUBLE_NULL && rprice == DOUBLE_NULL) brpar = true;
        if(blpar != brpar) return blpar > brpar;

        //net price
        if (lnet_price != DOUBLE_NULL || rnet_price != DOUBLE_NULL) {
            if (fabs(lnet_price - rnet_price) > FLOATING_PRECISION) {      // С > �� > ��  95.11>98.23
                if (lnet_price == DOUBLE_NULL) {
                    return false;
                } else if (rnet_price == DOUBLE_NULL) {
                    return true;
                } else {
                    return lnet_price < rnet_price;
                }
            }
        }

        //price
        if (lprice != DOUBLE_NULL || rprice != DOUBLE_NULL) {
            if (fabs(lprice - rprice) > FLOATING_PRECISION) {
                if (lprice == DOUBLE_NULL) {
                    return lrebate == "1";      // ��+���� > ��ֵ > ��
                } else if (rprice == DOUBLE_NULL) {
                    return rrebate == "0";      // ��+���� > ��ֵ > ��            
                } else if (lprice < 30.0 && rprice < 30.0) { // price < 30  ������
                    return lprice > rprice;     //  ��+���� > �� > С > ��    ���� 3.53 ���� 3.50
                } else if (lprice >= 30.0 && rprice >= 30.0) { // price >30  ����
                    return lprice < rprice;     // ��+���� > С > �� > ��     ���� 99.1 ���� 99.7
                } else {
                    return lprice > rprice;     // ���� > ������     99.1 ���� 3.5
                }
            }
        }

        //rebate(1) > not rebate(0), better
        if (lrebate > rrebate) {
            return true;
        }
        //not rebate(0) < rebate(1), better
        else if (lrebate < rrebate) {
            return false;
        }
        // both not rebate, compare other parameters
        else if ("0" == lrebate && "0" == rrebate) {
            //do nothing;
        }
        else if (fabs(lreturn_point - rreturn_point) > FLOATING_PRECISION) {
            return lreturn_point > rreturn_point;   // �� > �� >  С      ����F�� ���� F0.12 ���� F0.10
        }

        if(m_ignore_flag) {
            return false;
        }

        //flag_bargain �����      0-��    1- *   2-**
        std::string lbargain = lhs->flag_bargain;
        std::string rbargain = rhs->flag_bargain;
        if(lbargain != rbargain) {
            return lbargain < rbargain;         // 0 > 1 > 2 
        }

        //flag_relation  OCO/���     0-��  1- OCO 2-���
        std::string lrelation = lhs->flag_relation;
        std::string rrelation = rhs->flag_relation;
        if(lrelation != rrelation) {
            return lrelation < rrelation;       // 0 > 1 > 2 
        }

        return lhs->create_time < rhs->create_time;
    }

private:
    bool m_ignore_flag;
};

typedef std::tr1::shared_ptr<std::list<BondQuoteCachePtr> > BondQuoteCacheListPtr;
class BondBestQuoteCalcBusiness
	: public BaseBusiness //Add by Young 05-Apr-2016
{
public:
    int GetBestQuoteSortWithInternal(const std::string &bond_key_listed_market, std::vector<std::pair<BondQuoteCachePtr, BondQuoteCachePtr> > &result);
    std::vector<BondQuoteCachePtr> GetBestQuoteBidWithInternal(const std::string &bond_key_listed_market);
    std::vector<BondQuoteCachePtr> GetBestQuoteOfrWithInternal(const std::string &bond_key_listed_market);
    std::vector<BondQuoteCachePtr> GetBestQuoteBid(const std::string &bond_key_listed_market);
    std::vector<BondQuoteCachePtr> GetBestQuoteOfr(const std::string &bond_key_listed_market);

protected:
    void SortPriceBid(const std::string& bond_key_listed_market);
    void SortPriceOfr(const std::string& bond_key_listed_market);
    void SortPriceBidWithInternal(const std::string& bond_key_listed_market);
    void SortPriceOfrWithInternal(const std::string& bond_key_listed_market);

private:
    // ʹ��Vector sortʱ����iterator range����, �ʸ���List
    // huameng.jia 2014.11.14
    BondQuoteCacheListPtr best_price_bid_;
    BondQuoteCacheListPtr best_price_ofr_;
    BondQuoteCacheListPtr best_price_bid_with_internal_;
    BondQuoteCacheListPtr best_price_ofr_with_internal_;

};

#endif
