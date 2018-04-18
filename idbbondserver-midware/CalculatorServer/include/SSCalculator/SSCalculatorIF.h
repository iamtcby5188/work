#ifndef SS_CALCULATOR_IF_H
#define SS_CALCULATOR_IF_H

#include <vector>
#include "SSSingleRawBondFundamentalInfo.h"
#include "SSSingleInterestRate.h"
#include "SSSingleCalendar.h"
#include "SSBasicCalculationPage.h"
#include "SSHoldingPeriodInterest.h"
#include "SSHoldingPeriodResult.h"
#include "SSSingleCashflow.h"

using std::vector;

namespace ssCalculator {

    class SSCalculator;

    class SSCalculatorIF{
    public:
        static SSCalculatorIF *instance();
        static void destory();

        // set data
        void setBondFundamentalInfo(const vector<SSSingleRawBondFundamentalInfo>& all_info);
        void addBondFundamentalInfo(const vector<SSSingleRawBondFundamentalInfo>& all_info);
        void setInterestRate(const vector<SSSingleInterestRate>& all_rates);
        void addInterestRate(const vector<SSSingleInterestRate>& all_rates);
        void setCalendar(const vector<SSSingleCalendar>& all_calendars);

        // date manipulation
        string dateAdvance(const string& market, const string& date_str, int period_num, const string& period_unit, const string& convention);

        // simple calculation
        double accruedAmount (const string& bond_Key, const string& settlementDate) const;

        double dirtyPriceToCleanPrice (const string& bond_Key, const string& settlementDate, double dirtyPrice) const;
        double ytmToCleanPrice (const string& bond_Key, const string& settlementDate, double ytm) const;
        double marketSpreadToCleanPrice (const string& bond_Key, const string& settlementDate, double marketSpread) const;
        double ytcToCleanPrice (const string& bond_Key, const string& settlementDate, double ytc) const;
        double ytpToCleanPrice (const string& bond_Key, const string& settlementDate, double ytp) const;

        double cleanPriceToDirtyPrice (const string& bond_Key, const string& settlementDate, double cleanPrice) const;
        double cleanPriceToYTM (const string& bond_Key, const string& settlementDate, double cleanPrice) const;
        double cleanPriceToMarketSpread (const string& bond_Key, const string& settlementDate, double cleanPrice) const;
        double cleanPriceToYTC (const string& bond_Key, const string& settlementDate, double cleanPrice) const;
        double cleanPriceToYTP (const string& bond_Key, const string& settlementDate, double cleanPrice) const;

        double notional(const string& bond_Key, const string& date) const;

        // cashflows
        vector<SSSingleCashflow> cashflowsTable(const string& bond_Key, const string& settlementDate) const;

        // basic calculation page
        bool isFRNBond(const string& bond_Key) const;
        bool isOptionEmbedded(const string& bond_Key, const string& settlementDate) const;
        bool isYTCAvailable(const string& bond_Key, const string& settlementDate) const;
        bool isYTPAvailable(const string& bond_Key, const string& settlementDate) const;
        bool isListed(const string& bond_Key, const string& today) const;
        double prevailingRate (const string& bond_Key, const string& settlementDate) const;

        SSBasicCalculationPage basicPageInitialize(const string& bond_Key, const string& settlementDate) const;
        SSBasicCalculationPage setCouponRateSpread (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate) const;
        SSBasicCalculationPage setIndexRate (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate) const;
        SSBasicCalculationPage clickPrevailingRate (const string& bond_Key, const string& settlementDate, double couponRateSpread) const;

        SSBasicCalculationPage cleanPriceToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double cleanPrice) const;
        SSBasicCalculationPage dirtyPriceToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double dirtyPrice) const;
        SSBasicCalculationPage ytmToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double ytm) const;
        SSBasicCalculationPage marketSpreadToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double marketSpread) const;
        SSBasicCalculationPage ytcToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double ytc) const;
        SSBasicCalculationPage ytpToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread,double indexRate, double ytp) const;

        // rebate mode
        SSBasicCalculationPage rebateToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double quotedYTM, double rebate) const;
        SSBasicCalculationPage rebateCleanPriceToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double quotedCleanPrice, double rebate) const;
        SSBasicCalculationPage rebateDirtyPriceToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double quotedDirtyPrice, double rebate) const;
        SSBasicCalculationPage rebateMarketSpreadToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double quotedMarketSpread, double rebate) const;
        SSBasicCalculationPage rebateYTCToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double quotedYTC, double rebate) const;
        SSBasicCalculationPage rebateYTPToAll (const string& bond_Key, const string& settlementDate, double couponRateSpread, double indexRate, double quotedYTP, double rebate) const;

        // holding period yield page
        // trade condition --> basic calculation page

        // holding condition
        vector<SSHoldingPeriodInterest> holdingPeriodInterests (
            const string& startDate,
            const string& endDate,
            const string& indexID,
            double spread,
            int fixingPeriodNumber,
            const string& fixingPeriodUnit,
            const string& simpleCompound,
            const string& interestBasis,
            int preceeds,
            double notional) const;
        double holdingCost(const vector<SSHoldingPeriodInterest>&) const;
        // result
        SSHoldingPeriodResult holdingPeriodResult (
            const string& bond_Key,
            const string& startDate,
            const string& endDate,
            const string& today,
            double notional, 
            double startCleanPrice, 
            double startAccruedInterest, 
            double startFrontDeskCommission,
            double startBackStageCommission,
            double endCleanPrice,
            double endAccruedInterest,
            double endFrontDeskCommission,
            double endBackStageCommission,
            double interestTax,
            double businessTax,
            double holdingCost,
            bool buyThenSell,
            int payHoldingCost) const;

        // Implied Repo Rate
        double impliedRepoRate(const string& bond_Key, const string& tradeDate, const string& settlementDate, double futurePrice, double convertFactor, double dirtyPrice) const;
        double futurePriceByDPAndIRR(const string& bond_Key, const string& tradeDate, const string& settlementDate, double convertFactor, double dirtyPrice, double irr) const;
        double dirtyPriceByFPAndIRR(const string& bond_Key, const string& tradeDate, const string& settlementDate, double convertFactor, double futurePrice, double irr) const;

        // bond valuation
        SSBasicCalculationPage qbBondValuation(const string& bond_key, const string& settlementDate, double ytm, const map<string, double>& qbYieldCurve, double upwardParallelShift = 0);

        // exceptions
        string exceptions() const;

    private:
        SSCalculatorIF();
        virtual ~SSCalculatorIF();
        static SSCalculatorIF *instance_;
        SSCalculator* impl_;
    };

}
#endif
