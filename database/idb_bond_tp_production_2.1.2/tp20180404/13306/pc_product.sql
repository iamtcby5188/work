-- MySQL dump 10.13  Distrib 5.6.22, for linux-glibc2.5 (x86_64)
--
-- Host: 192.168.162.110    Database: pc_product
-- ------------------------------------------------------
-- Server version	5.6.22-enterprise-commercial-advanced-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `additional_issue_info`
--

DROP TABLE IF EXISTS `additional_issue_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `additional_issue_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `No_Add_Issue` int(10) DEFAULT NULL,
  `First_Issue_Key` varchar(25) DEFAULT NULL,
  `Add_Issue_Key` varchar(25) DEFAULT NULL,
  `Add_Full_Name` varchar(96) DEFAULT NULL,
  `Add_Announce_Date` decimal(8,0) DEFAULT NULL,
  `Add_Auction_Date` decimal(8,0) DEFAULT NULL,
  `Add_Issue_Start_Date` decimal(8,0) DEFAULT NULL,
  `Add_Issue_End_Date` decimal(8,0) DEFAULT NULL,
  `Add_Dist_Date_Start` decimal(8,0) DEFAULT NULL,
  `Add_Dist_Date_End` decimal(8,0) DEFAULT NULL,
  `Add_Payment_Date` decimal(8,0) DEFAULT NULL,
  `Add_Listed_Date` decimal(8,0) DEFAULT NULL,
  `Add_Issue_Commission_Rate` decimal(7,4) DEFAULT NULL,
  `Add_Issue_Price` decimal(7,4) DEFAULT NULL,
  `Add_Planned_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `Add_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `Reason` varchar(500) DEFAULT NULL,
  `Add_Issue_ID` varchar(16) DEFAULT NULL,
  `Add_Issue_Short_Name` varchar(16) DEFAULT NULL,
  `First_Issue_ID` varchar(16) DEFAULT NULL,
  `Mkt_Type` varchar(3) DEFAULT NULL,
  `Ann_Status` varchar(1) DEFAULT NULL,
  `Bid_Outcome` decimal(7,4) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond`
--

DROP TABLE IF EXISTS `bond`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Country` varchar(3) DEFAULT NULL,
  `Currency` varchar(3) DEFAULT NULL,
  `Bond_ID` varchar(32) DEFAULT NULL,
  `Short_Name` varchar(32) DEFAULT NULL,
  `ISIN` varchar(16) DEFAULT NULL,
  `Full_Name` varchar(96) DEFAULT NULL,
  `Issuer_Code` varchar(7) DEFAULT NULL,
  `Issuer_Key_LEI` varchar(15) DEFAULT NULL,
  `Underwriter_Code` varchar(256) DEFAULT NULL,
  `Yield_Curve_ID` varchar(15) DEFAULT NULL,
  `Yield_Curve_Type` varchar(32) DEFAULT NULL,
  `Bond_Type` varchar(3) DEFAULT NULL,
  `Bond_Subtype` varchar(3) DEFAULT NULL,
  `External_Type` varchar(3) DEFAULT NULL,
  `Maturity_Term_Y` decimal(7,4) DEFAULT NULL,
  `Maturity_Term` decimal(9,4) DEFAULT NULL,
  `Term_Unit` varchar(3) DEFAULT NULL,
  `Interest_Start_Date` decimal(8,0) DEFAULT NULL,
  `Maturity_Date` decimal(8,0) DEFAULT NULL,
  `First_Coupon_Date` decimal(8,0) DEFAULT NULL,
  `Next_Coupon_Date` decimal(8,0) DEFAULT NULL,
  `Announce_Date` decimal(8,0) DEFAULT NULL,
  `Issue_Start_Date` decimal(8,0) DEFAULT NULL,
  `Issue_End_Date` decimal(8,0) DEFAULT NULL,
  `WI_Start_Date` decimal(8,0) DEFAULT NULL,
  `WI_End_Date` decimal(8,0) DEFAULT NULL,
  `Dist_Date_Start` decimal(8,0) DEFAULT NULL,
  `Dist_Date_End` decimal(8,0) DEFAULT NULL,
  `Auction_Date_Start` decimal(8,0) DEFAULT NULL,
  `Auction_Date_End` decimal(8,0) DEFAULT NULL,
  `Payment_Date` decimal(8,0) DEFAULT NULL,
  `Listed_Date` decimal(8,0) DEFAULT NULL,
  `Delisted_Date` decimal(8,0) DEFAULT NULL,
  `Register` varchar(3) DEFAULT NULL,
  `Option_Type` varchar(3) DEFAULT NULL,
  `Option_Style` varchar(3) DEFAULT NULL,
  `Option_Date` decimal(8,0) DEFAULT NULL,
  `Option_Price_Ratio` varchar(10) DEFAULT NULL,
  `Option_Record_Date` varchar(32) DEFAULT NULL,
  `Call_No` decimal(2,0) DEFAULT NULL,
  `Call_Str` varchar(200) DEFAULT NULL,
  `Put_No` decimal(2,0) DEFAULT NULL,
  `Put_Str` varchar(200) DEFAULT NULL,
  `First_Exchg_Bond_Key` varchar(64) DEFAULT NULL,
  `First_Exchg_No` decimal(2,0) DEFAULT NULL,
  `First_Exchg_Str` varchar(200) DEFAULT NULL,
  `Sec_Exchg_No` decimal(2,0) DEFAULT NULL,
  `Sec_Exchg_Year` varchar(200) DEFAULT NULL,
  `Assign_Trans_Key` varchar(7) DEFAULT NULL,
  `Assign_Trans_No` decimal(2,0) DEFAULT NULL,
  `Assign_Trans_Str` varchar(200) DEFAULT NULL,
  `Compensate_Rate` varchar(30) DEFAULT NULL,
  `Compensate_From` varchar(16) DEFAULT NULL,
  `Option_Exercise` varchar(3) DEFAULT NULL,
  `Option_Exercise_Date` varchar(32) DEFAULT NULL,
  `Coupon_Type` varchar(3) DEFAULT NULL,
  `Coupon_Rate_Spread` decimal(7,4) DEFAULT NULL,
  `Coupon_Rate_Current` decimal(7,4) DEFAULT NULL,
  `Coupon_Frequency` varchar(3) DEFAULT NULL,
  `Compound_Frequency` varchar(3) DEFAULT NULL,
  `Interest_Basis` varchar(3) DEFAULT NULL,
  `Coupon_Dist` varchar(3) DEFAULT NULL,
  `FRN_Multiplier` decimal(7,4) DEFAULT NULL,
  `FRN_Index_ID` varchar(16) DEFAULT NULL,
  `First_Index_Rate` varchar(32) DEFAULT NULL,
  `Fixing_Frequency` varchar(3) DEFAULT NULL,
  `Fixing_MA_Days` decimal(4,0) DEFAULT NULL,
  `Fixing_Preceds` varchar(4) DEFAULT NULL,
  `Fixing_Calendar_Key` varchar(3) DEFAULT NULL,
  `Fixing_Tailing` varchar(3) DEFAULT NULL,
  `Fixing_Digit` decimal(1,0) DEFAULT NULL,
  `Reset_Effective` varchar(3) DEFAULT NULL,
  `Cap` varchar(32) DEFAULT NULL,
  `Flr` varchar(32) DEFAULT NULL,
  `Simple_Compound` varchar(3) DEFAULT NULL,
  `Issue_Reference` varchar(32) DEFAULT NULL,
  `Issue_Reference_Desc` varchar(64) DEFAULT NULL,
  `Variable_Schedule` varchar(3) DEFAULT NULL,
  `Coupon_Day_Adjust` varchar(3) DEFAULT NULL,
  `Coupon_Day_DMC` varchar(3) DEFAULT NULL,
  `Coupon_Calendar_Key` varchar(3) DEFAULT NULL,
  `Pricing_Conv` varchar(3) DEFAULT NULL,
  `Issue_Price` decimal(16,4) DEFAULT NULL,
  `Issue_Rate` decimal(16,4) DEFAULT NULL,
  `Planned_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `First_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `Issue_Amount` decimal(16,4) DEFAULT NULL,
  `Outstanding_Amount` decimal(16,4) DEFAULT NULL,
  `Redemption_No` decimal(3,0) DEFAULT NULL,
  `Redemption_Str` varchar(512) DEFAULT NULL,
  `Maturity_Adjust` varchar(3) DEFAULT NULL,
  `Maturity_DMC` varchar(3) DEFAULT NULL,
  `Maturity_Calendar_Key` varchar(3) DEFAULT NULL,
  `Sceniority` varchar(3) DEFAULT NULL,
  `Rating_Current` varchar(10) DEFAULT NULL,
  `Rating_Institution_Code` varchar(7) DEFAULT NULL,
  `Rating_Date` decimal(8,0) DEFAULT NULL,
  `Rating_Augment` varchar(64) DEFAULT NULL,
  `Warranter` varchar(100) DEFAULT NULL,
  `Warrant_Note` varchar(100) DEFAULT NULL,
  `Bond_Collateral` varchar(100) DEFAULT NULL,
  `Collateral_Value` decimal(16,4) DEFAULT NULL,
  `Evaluation_Date` decimal(8,0) DEFAULT NULL,
  `Collateral_Type` varchar(15) DEFAULT NULL,
  `Issuer_Rating_Current` varchar(10) DEFAULT NULL,
  `Issuer_Outlook_Current` varchar(20) DEFAULT NULL,
  `Issuer_Rating_Institution_Code` varchar(7) DEFAULT NULL,
  `Issuer_Rating_Date` decimal(8,0) DEFAULT NULL,
  `Issue_Commission_Rate` decimal(7,4) DEFAULT NULL,
  `Issue_Bonus_Rate` decimal(7,4) DEFAULT NULL,
  `Redm_Commission_Rate` decimal(7,4) DEFAULT NULL,
  `Risk_Weight` decimal(7,4) DEFAULT NULL,
  `Issue_Year` decimal(4,0) DEFAULT NULL,
  `Issue_No` decimal(3,0) DEFAULT NULL,
  `Quotation` varchar(3) DEFAULT NULL,
  `YC_DCT_Key` decimal(3,0) DEFAULT NULL,
  `YC_CPN_Key` decimal(3,0) DEFAULT NULL,
  `Asset_Status` varchar(10) DEFAULT NULL,
  `Purpose_of_Issue` varchar(1000) DEFAULT NULL,
  `First_Issue_Key` varchar(25) DEFAULT NULL,
  `Note` varchar(4096) DEFAULT NULL,
  `Term_structure` varchar(20) DEFAULT NULL,
  `Term_Before_Exercise` decimal(7,4) DEFAULT NULL,
  `P_Rating_Current` varchar(10) DEFAULT NULL,
  `P_Issuer_Rating_Current` varchar(10) DEFAULT NULL,
  `P_Issuer_Outlook_Current` varchar(20) DEFAULT NULL,
  `No_Add_Issue` int(11) DEFAULT NULL,
  `Is_Cross_Mkt` varchar(1) DEFAULT NULL,
  `Is_Mortgage` varchar(1) DEFAULT NULL,
  `Mkt_Type` varchar(3) DEFAULT NULL,
  `Ann_Status` varchar(1) DEFAULT NULL,
  `Liquidity_Supporter` varchar(64) DEFAULT NULL,
  `Is_CIB` varchar(1) DEFAULT NULL,
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Rating_Current_NPY` varchar(10) DEFAULT NULL,
  `Issuer_Rating_Current_NPY` varchar(10) DEFAULT NULL,
  `Ets_Str` varchar(16) DEFAULT NULL,
  `Short_Name_EN` varchar(64) DEFAULT NULL,
  `Full_Name_EN` varchar(256) DEFAULT NULL,
  `Registered_Amount` decimal(16,4) DEFAULT NULL,
  `Bid_Limit_Bottom` decimal(4,2) DEFAULT NULL,
  `Bid_Limit_Top` decimal(4,2) DEFAULT NULL,
  `Issue_End_Time` varchar(4) DEFAULT NULL,
  `Auction_Date_Time` varchar(8) DEFAULT NULL,
  `Ent_Cor` varchar(3) DEFAULT NULL,
  `Issue_Period` varchar(16) DEFAULT NULL,
  `holiday_reason_en` varchar(127) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_ID` (`Bond_ID`) USING BTREE,
  KEY `delflag` (`delflag`) USING BTREE,
  KEY `Bond_Key` (`Bond_Key`) USING BTREE,
  KEY `Issue_Start_Date` (`Issue_Start_Date`) USING BTREE,
  KEY `Interest_Start_Date` (`Interest_Start_Date`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_coupon_rate`
--

DROP TABLE IF EXISTS `bond_coupon_rate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_coupon_rate` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Coupon_Rate_Spread` decimal(7,4) DEFAULT NULL,
  `Index_Rate` decimal(7,4) DEFAULT NULL,
  `Coupon_Rate` decimal(7,4) DEFAULT NULL,
  `Interest_Start_Date_T` decimal(8,0) DEFAULT NULL,
  `Coupon_Date_Next` decimal(8,0) DEFAULT NULL,
  `Index_Rate_Start_Date` decimal(8,0) DEFAULT NULL,
  `Index_Rate_End_Date` decimal(8,0) DEFAULT NULL,
  `Is_Current_Rate` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_fundamental_info`
--

DROP TABLE IF EXISTS `bond_fundamental_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_fundamental_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Country` varchar(3) DEFAULT NULL,
  `Currency` varchar(3) DEFAULT NULL,
  `Bond_ID` varchar(32) DEFAULT NULL,
  `Short_Name` varchar(32) DEFAULT NULL,
  `ISIN` varchar(16) DEFAULT NULL,
  `Full_Name` varchar(96) DEFAULT NULL,
  `Issuer_Code` varchar(7) DEFAULT NULL,
  `Issuer_Key_LEI` varchar(15) DEFAULT NULL,
  `Bond_Type` varchar(3) DEFAULT NULL,
  `Bond_Subtype` varchar(3) DEFAULT NULL,
  `External_Type` varchar(3) DEFAULT NULL,
  `Maturity_Term_Y` decimal(7,4) DEFAULT NULL,
  `Maturity_Term` decimal(9,4) DEFAULT NULL,
  `Term_Unit` varchar(3) DEFAULT NULL,
  `Interest_Start_Date` decimal(8,0) DEFAULT NULL,
  `Maturity_Date` decimal(8,0) DEFAULT NULL,
  `First_Coupon_Date` decimal(8,0) DEFAULT NULL,
  `Announce_Date` decimal(8,0) DEFAULT NULL,
  `Issue_Start_Date` decimal(8,0) DEFAULT NULL,
  `Issue_End_Date` decimal(8,0) DEFAULT NULL,
  `WI_Start_Date` decimal(8,0) DEFAULT NULL,
  `WI_End_Date` decimal(8,0) DEFAULT NULL,
  `Dist_Date_Start` decimal(8,0) DEFAULT NULL,
  `Dist_Date_End` decimal(8,0) DEFAULT NULL,
  `Auction_Date_Start` decimal(8,0) DEFAULT NULL,
  `Auction_Date_End` decimal(8,0) DEFAULT NULL,
  `Payment_Date` decimal(8,0) DEFAULT NULL,
  `Listed_Date` decimal(8,0) DEFAULT NULL,
  `Delisted_Date` decimal(8,0) DEFAULT NULL,
  `Register` varchar(3) DEFAULT NULL,
  `Option_Type` varchar(3) DEFAULT NULL,
  `Option_Style` varchar(3) DEFAULT NULL,
  `Call_No` decimal(2,0) DEFAULT NULL,
  `Call_Str` varchar(200) DEFAULT NULL,
  `Put_No` decimal(2,0) DEFAULT NULL,
  `Put_Str` varchar(200) DEFAULT NULL,
  `First_Exchg_Bond_Key` varchar(64) DEFAULT NULL,
  `First_Exchg_No` decimal(2,0) DEFAULT NULL,
  `First_Exchg_Str` varchar(200) DEFAULT NULL,
  `Sec_Exchg_No` decimal(2,0) DEFAULT NULL,
  `Sec_Exchg_Year` varchar(200) DEFAULT NULL,
  `Assign_Trans_Key` varchar(7) DEFAULT NULL,
  `Assign_Trans_No` decimal(2,0) DEFAULT NULL,
  `Assign_Trans_Str` varchar(200) DEFAULT NULL,
  `Compensate_Rate` varchar(30) DEFAULT NULL,
  `Compensate_From` varchar(16) DEFAULT NULL,
  `Option_Exercise` varchar(3) DEFAULT NULL,
  `Option_Exercise_Date` varchar(32) DEFAULT NULL,
  `Coupon_Type` varchar(3) DEFAULT NULL,
  `Coupon_Rate_Spread` decimal(7,4) DEFAULT NULL,
  `Coupon_Frequency` varchar(3) DEFAULT NULL,
  `Compound_Frequency` varchar(3) DEFAULT NULL,
  `Interest_Basis` varchar(3) DEFAULT NULL,
  `Coupon_Dist` varchar(3) DEFAULT NULL,
  `FRN_Multiplier` decimal(7,4) DEFAULT NULL,
  `FRN_Index_ID` varchar(16) DEFAULT NULL,
  `First_Index_Rate` varchar(32) DEFAULT NULL,
  `Fixing_Frequency` varchar(3) DEFAULT NULL,
  `Fixing_MA_Days` decimal(4,0) DEFAULT NULL,
  `Fixing_Preceds` varchar(4) DEFAULT NULL,
  `Fixing_Calendar_Key` varchar(3) DEFAULT NULL,
  `Fixing_Tailing` varchar(3) DEFAULT NULL,
  `Fixing_Digit` decimal(1,0) DEFAULT NULL,
  `Reset_Effective` varchar(3) DEFAULT NULL,
  `Cap` varchar(32) DEFAULT NULL,
  `Flr` varchar(32) DEFAULT NULL,
  `Simple_Compound` varchar(3) DEFAULT NULL,
  `Issue_Reference` varchar(32) DEFAULT NULL,
  `Issue_Reference_Desc` varchar(64) DEFAULT NULL,
  `Variable_Schedule` varchar(3) DEFAULT NULL,
  `Coupon_Day_Adjust` varchar(3) DEFAULT NULL,
  `Coupon_Day_DMC` varchar(3) DEFAULT NULL,
  `Coupon_Calendar_Key` varchar(3) DEFAULT NULL,
  `Pricing_Conv` varchar(3) DEFAULT NULL,
  `Issue_Price` decimal(16,4) DEFAULT NULL,
  `Planned_Issue_Amount` decimal(16,6) DEFAULT NULL,
  `First_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `Redemption_No` decimal(3,0) DEFAULT NULL,
  `Redemption_Str` varchar(1600) DEFAULT NULL,
  `Maturity_Adjust` varchar(3) DEFAULT NULL,
  `Maturity_DMC` varchar(3) DEFAULT NULL,
  `Maturity_Calendar_Key` varchar(3) DEFAULT NULL,
  `Sceniority` varchar(3) DEFAULT NULL,
  `Rating_Institution_Code` varchar(7) DEFAULT NULL,
  `Rating_Augment` varchar(64) DEFAULT NULL,
  `Warranter` varchar(100) DEFAULT NULL,
  `Warrant_Note` varchar(100) DEFAULT NULL,
  `Evaluation_Date` decimal(8,0) DEFAULT NULL,
  `Issue_Commission_Rate` decimal(7,4) DEFAULT NULL,
  `Issue_Bonus_Rate` decimal(7,4) DEFAULT NULL,
  `Redm_Commission_Rate` decimal(7,4) DEFAULT NULL,
  `Risk_Weight` decimal(7,4) DEFAULT NULL,
  `Issue_Year` decimal(4,0) DEFAULT NULL,
  `Issue_No` decimal(3,0) DEFAULT NULL,
  `Quotation` varchar(3) DEFAULT NULL,
  `YC_DCT_Key` decimal(3,0) DEFAULT NULL,
  `YC_CPN_Key` decimal(3,0) DEFAULT NULL,
  `Asset_Status` varchar(10) DEFAULT NULL,
  `First_Issue_Key` varchar(25) DEFAULT NULL,
  `Note` varchar(4096) DEFAULT NULL,
  `Term_structure` varchar(20) DEFAULT NULL,
  `Term_Before_Exercise` decimal(7,4) DEFAULT NULL,
  `Bond_Collateral` varchar(100) DEFAULT NULL,
  `Collateral_Value` decimal(16,4) DEFAULT NULL,
  `Collateral_Type` varchar(15) DEFAULT NULL,
  `Purpose_of_Issue` varchar(1000) DEFAULT NULL,
  `Mkt_Type` varchar(3) DEFAULT NULL,
  `Ann_Status` varchar(1) DEFAULT NULL,
  `Liquidity_Supporter` varchar(64) DEFAULT NULL,
  `Ets_Str` varchar(16) DEFAULT NULL,
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Short_Name_EN` varchar(64) DEFAULT NULL,
  `Full_Name_EN` varchar(256) DEFAULT NULL,
  `Registered_Amount` decimal(16,4) DEFAULT NULL,
  `Bid_Limit_Bottom` decimal(4,2) DEFAULT NULL,
  `Bid_Limit_Top` decimal(4,2) DEFAULT NULL,
  `Payment_Time` varchar(8) DEFAULT NULL,
  `Auction_Date_Time` varchar(8) DEFAULT NULL,
  `Ent_Cor` varchar(3) DEFAULT NULL,
  `Issue_Period` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `bondkey` (`Bond_Key`,`delflag`) USING BTREE,
  UNIQUE KEY `E_IDX_1xr8x6qvgf05p` (`delflag`,`Bond_Key`) USING BTREE,
  UNIQUE KEY `E_IDX_44tgihq1sta2t` (`delflag`,`Full_Name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_tender`
--

DROP TABLE IF EXISTS `bond_tender`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_tender` (
  `id` varchar(32) NOT NULL,
  `Bond_Key` varchar(25) NOT NULL,
  `bond_id` varchar(32) NOT NULL,
  `upper_limit` varchar(16) NOT NULL,
  `lower_limit` varchar(16) NOT NULL,
  `step` varchar(16) NOT NULL,
  `intervened` int(1) NOT NULL DEFAULT '0' COMMENT '是否经过人工修改。 (预留)',
  `modified_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='保存匹配成功的标书信息(上下限及步长)， 如果人工修改， intervened 字段置1。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_tender_detail`
--

DROP TABLE IF EXISTS `bond_tender_detail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_tender_detail` (
  `id` varchar(32) NOT NULL,
  `Bond_Key` varchar(25) NOT NULL,
  `bond_id` varchar(32) NOT NULL,
  `price` varchar(16) NOT NULL,
  `rate` varchar(16) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='保存匹配成功后计算出的标书详情(价格与参考收益率)。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cdc_bond_valuation`
--

DROP TABLE IF EXISTS `cdc_bond_valuation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cdc_bond_valuation` (
  `ID` varchar(32) NOT NULL,
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Short_Name` varchar(32) DEFAULT NULL,
  `Bond_ID` varchar(32) DEFAULT NULL,
  `Valuate_Date` decimal(8,0) DEFAULT NULL,
  `Listed_Market` varchar(3) DEFAULT NULL,
  `Remaining_Year` decimal(7,4) DEFAULT NULL,
  `Val_Intraday_Dirty_Price` decimal(7,4) DEFAULT NULL,
  `Val_Intraday_Accrued_Interest` decimal(7,4) DEFAULT NULL,
  `Val_Clean_Price` decimal(7,4) DEFAULT NULL,
  `Val_Yield` decimal(7,4) DEFAULT NULL,
  `Val_Modified_Duration` decimal(7,4) DEFAULT NULL,
  `Val_Convexity` decimal(7,4) DEFAULT NULL,
  `Val_Basis_Point_Value` decimal(7,4) DEFAULT NULL,
  `Val_Spread_Duration` decimal(7,4) DEFAULT NULL,
  `Val_Spread_Convexity` decimal(7,4) DEFAULT NULL,
  `Market_Dirty_Price` decimal(7,4) DEFAULT NULL,
  `Market_Clean_Price` decimal(7,4) DEFAULT NULL,
  `Market_Yield` decimal(7,4) DEFAULT NULL,
  `Market_Modified_Duration` decimal(7,4) DEFAULT NULL,
  `Market_Convexity` decimal(7,4) DEFAULT NULL,
  `Market_Basis_Point_Value` decimal(7,4) DEFAULT NULL,
  `Market_Spread_Duration` decimal(7,4) DEFAULT NULL,
  `Market_Spread_Convexity` decimal(7,4) DEFAULT NULL,
  `Credibility` varchar(16) DEFAULT NULL,
  `Val_Rate_Duration` decimal(7,4) DEFAULT NULL,
  `Val_Rate_Convexity` decimal(7,4) DEFAULT NULL,
  `Market_Rate_Duration` decimal(7,4) DEFAULT NULL,
  `Market_Rate_Convexity` decimal(7,4) DEFAULT NULL,
  `Val_Closed_Dirty_Price` decimal(7,4) DEFAULT NULL,
  `Val_Closed_Accrued_Interest` decimal(7,4) DEFAULT NULL,
  `Remaining_Par_Value` decimal(7,4) DEFAULT NULL,
  `Val_Spread` decimal(7,4) DEFAULT NULL,
  `Yield_Curve_ID` varchar(128) DEFAULT NULL,
  `Market_Spread` decimal(7,4) DEFAULT NULL,
  `Absolute_Liquidity_Coefficient` decimal(7,4) DEFAULT NULL,
  `Position_Percentage` decimal(7,4) DEFAULT NULL,
  `Relative_Liquidity_Coefficient` decimal(7,4) DEFAULT NULL,
  `Relative_Liquidity_Value` decimal(7,4) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `IndexBondKey` (`Bond_Key`) USING BTREE,
  KEY `IndexValuateDate` (`Valuate_Date`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cdc_curve_dict`
--

DROP TABLE IF EXISTS `cdc_curve_dict`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cdc_curve_dict` (
  `id` int(11) NOT NULL,
  `Code` varchar(32) NOT NULL,
  `Source` varchar(10) NOT NULL,
  `CurveName` varchar(64) NOT NULL,
  `CurvePeriodType` varchar(32) NOT NULL,
  `Market` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cdc_yield_curve`
--

DROP TABLE IF EXISTS `cdc_yield_curve`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cdc_yield_curve` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `CurveType` varchar(32) NOT NULL COMMENT '曲线类型',
  `Period` varchar(8) NOT NULL COMMENT '期限',
  `Price` decimal(10,5) DEFAULT NULL COMMENT '价格',
  `Yield` decimal(10,5) NOT NULL COMMENT '收益率',
  `IndexDesc` varchar(64) DEFAULT NULL COMMENT '指标简介',
  `Date` date NOT NULL COMMENT '日期',
  `Src` varchar(12) DEFAULT 'CDC' COMMENT '来源',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `holiday_info`
--

DROP TABLE IF EXISTS `holiday_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `holiday_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `country` varchar(3) DEFAULT NULL,
  `market_type` varchar(16) DEFAULT NULL,
  `holiday_date` date DEFAULT NULL,
  `holiday_reason` varchar(32) DEFAULT NULL,
  `holiday_reason_en` varchar(127) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `institution`
--

DROP TABLE IF EXISTS `institution`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `institution` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Institution_Code` varchar(7) DEFAULT NULL,
  `Institution_Type` varchar(3) DEFAULT NULL,
  `Institution_Rating` varchar(10) DEFAULT NULL,
  `Establishment_Date` decimal(8,0) DEFAULT NULL,
  `Change_Date` decimal(8,0) DEFAULT NULL,
  `Full_Name_C` varchar(80) DEFAULT NULL,
  `Short_Name_C` varchar(80) DEFAULT NULL,
  `Full_Name_E` varchar(256) DEFAULT NULL,
  `Short_Name_E` varchar(80) DEFAULT NULL,
  `Country` varchar(8) DEFAULT NULL,
  `Province` varchar(16) DEFAULT NULL,
  `City` varchar(16) DEFAULT NULL,
  `Largest_Shareholder` varchar(50) DEFAULT NULL,
  `Legal_Representative` varchar(50) DEFAULT NULL,
  `Registered_Capital` varchar(100) DEFAULT NULL,
  `Registered_Address` varchar(100) DEFAULT NULL,
  `Business_Scope` varchar(1024) DEFAULT NULL,
  `Product` varchar(500) DEFAULT NULL,
  `Rating_Date` decimal(8,0) DEFAULT NULL,
  `Rating` varchar(10) DEFAULT NULL,
  `Rating_Institution_Code` varchar(7) DEFAULT NULL,
  `Former_Name` varchar(256) DEFAULT NULL,
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Issuer_Type` varchar(16) DEFAULT NULL,
  `Org_Type` varchar(16) DEFAULT NULL,
  `SW_Sector` varchar(32) DEFAULT NULL,
  `SW_Subsector` varchar(32) DEFAULT NULL,
  `Regd_capital` decimal(15,2) DEFAULT NULL,
  `Regd_capital_Currency` varchar(8) DEFAULT NULL,
  `Stockholder_Name` varchar(64) DEFAULT NULL,
  `Stockholder_Type` varchar(16) DEFAULT NULL,
  `Stockholding_Percentage` varchar(16) DEFAULT NULL,
  `Actual_Controller_Name` varchar(128) DEFAULT NULL,
  `Actual_Controller_Type` varchar(16) DEFAULT NULL,
  `Ultimate_Actual_Controller_Name` varchar(128) DEFAULT NULL,
  `Ultimate_Actual_Controller_Type` varchar(16) DEFAULT NULL,
  `CBRC_Financing_Platform` varchar(1) DEFAULT NULL,
  `Endowment` varchar(64) DEFAULT NULL,
  `Municipal_Business` varchar(1) DEFAULT NULL,
  `Institution_Subtype` varchar(3) DEFAULT NULL,
  `Pin_Yin` varchar(1024) DEFAULT NULL,
  `Pin_Yin_Full` varchar(7168) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `delflag` (`delflag`),
  KEY `Institution_Code` (`Institution_Code`),
  KEY `Municipal_Business` (`Municipal_Business`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `issuer_info`
--

DROP TABLE IF EXISTS `issuer_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `issuer_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `is_municipal` varchar(1) DEFAULT NULL,
  `institution_code` varchar(7) DEFAULT NULL,
  `issuer_name` varchar(64) DEFAULT NULL,
  `issuer_type` varchar(16) DEFAULT NULL,
  `org_type` varchar(16) DEFAULT NULL,
  `sw_sector` varchar(32) DEFAULT NULL,
  `sw_subsector` varchar(32) DEFAULT NULL,
  `province` varchar(16) DEFAULT NULL,
  `city` varchar(16) DEFAULT NULL,
  `regd_capital` decimal(15,2) DEFAULT NULL,
  `regd_capital_currency` varchar(8) DEFAULT NULL,
  `stockholder_name` varchar(127) DEFAULT NULL,
  `stockholder_type` varchar(16) DEFAULT NULL,
  `stockholding_percentage` varchar(16) DEFAULT NULL,
  `actual_controller_name` varchar(128) DEFAULT NULL,
  `actual_controller_type` varchar(16) DEFAULT NULL,
  `ultimate_actual_controller_name` varchar(128) DEFAULT NULL,
  `ultimate_actual_controller_type` varchar(16) DEFAULT NULL,
  `cbrc_financing_platform` varchar(1) DEFAULT NULL,
  `endowment` varchar(64) DEFAULT NULL,
  `municipal_business` varchar(1) DEFAULT NULL,
  `business_scope` varchar(1024) DEFAULT NULL,
  `sw_sector_code` varchar(16) DEFAULT NULL,
  `sw_subsector_code` varchar(16) DEFAULT NULL,
  `province_code` varchar(16) DEFAULT NULL,
  `Orgnization_Code` varchar(31) DEFAULT NULL,
  `Regd_Num` varchar(31) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Institution_Code` (`institution_code`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_auction_info`
--

DROP TABLE IF EXISTS `p_auction_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_auction_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `No_Auction` int(10) DEFAULT NULL,
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Auction_Date` decimal(8,0) DEFAULT NULL,
  `Issue_Type` varchar(3) DEFAULT NULL,
  `Auction_Type` varchar(3) DEFAULT NULL,
  `Auction_Cap_Unit` varchar(8) DEFAULT NULL,
  `Auction_Flr` varchar(32) DEFAULT NULL,
  `Auction_Flr_Unit` varchar(8) DEFAULT NULL,
  `Step` decimal(7,4) DEFAULT NULL,
  `Step_Unit` varchar(8) DEFAULT NULL,
  `Basic_Bidding_Unit` decimal(16,4) DEFAULT NULL,
  `Min_Bid` decimal(16,4) DEFAULT NULL,
  `Max_Bid` decimal(16,4) DEFAULT NULL,
  `Payment` decimal(16,4) DEFAULT NULL,
  `First_Issue` decimal(16,4) DEFAULT NULL,
  `Add_Issue` decimal(16,4) DEFAULT NULL,
  `Basic_Underwritering` decimal(16,4) DEFAULT NULL,
  `Planed_Auction_Amount` decimal(16,4) DEFAULT NULL,
  `Actual_Auction_Amount` decimal(16,4) DEFAULT NULL,
  `Num_Auction_Participant` decimal(8,0) DEFAULT NULL,
  `Num_Biding_Company` decimal(8,0) DEFAULT NULL,
  `Num_Bidding` decimal(8,0) DEFAULT NULL,
  `Valid_Bidding` decimal(8,0) DEFAULT NULL,
  `Invalid_Bidding` decimal(8,0) DEFAULT NULL,
  `Bid_Amount` decimal(16,4) DEFAULT NULL,
  `H_Bidding` decimal(7,4) DEFAULT NULL,
  `H_Bidding_Unit` varchar(8) DEFAULT NULL,
  `L_Bidding` decimal(7,4) DEFAULT NULL,
  `L_Bidding_Unit` varchar(8) DEFAULT NULL,
  `Award_Amount` decimal(16,4) DEFAULT NULL,
  `Num_Bidding_Winners` decimal(8,0) DEFAULT NULL,
  `Award_Bidding` decimal(8,0) DEFAULT NULL,
  `Dealers_Award_Amount` decimal(16,4) DEFAULT NULL,
  `H_Winning_Bidding` decimal(7,4) DEFAULT NULL,
  `H_Winning_Bidding_Unit` varchar(8) DEFAULT NULL,
  `L_Winning_Bidding` decimal(7,4) DEFAULT NULL,
  `L_Winning_Bidding_Unit` varchar(8) DEFAULT NULL,
  `Marginal_Bidding_Amount` decimal(16,4) DEFAULT NULL,
  `Marginal_Winning_Bidding` decimal(16,4) DEFAULT NULL,
  `Subscription_Ratio` decimal(7,4) DEFAULT NULL,
  `Auction_Cap` varchar(32) DEFAULT NULL,
  `Auction_Time_Start` decimal(6,0) DEFAULT NULL,
  `Auction_Time_End` decimal(6,0) DEFAULT NULL,
  `WA_Tender_Rate` decimal(7,4) DEFAULT NULL,
  `MG_Tender_Rate` decimal(7,4) DEFAULT NULL,
  `Marginal_Ratio` decimal(7,4) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_bond_list_info`
--

DROP TABLE IF EXISTS `p_bond_list_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_bond_list_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT NULL,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Listed_Market` varchar(3) DEFAULT NULL,
  `Bond_ID` varchar(32) DEFAULT NULL,
  `Short_Name` varchar(32) DEFAULT NULL,
  `Is_Cross_Mkt` varchar(1) DEFAULT NULL,
  `Is_Mortgage` varchar(1) DEFAULT NULL,
  `Listed_Date` decimal(8,0) DEFAULT NULL,
  `Delisted_Date` decimal(8,0) DEFAULT NULL,
  `Mkt_Type` varchar(3) DEFAULT NULL,
  `PINYIN` varchar(1024) DEFAULT NULL,
  `PINYIN_FULL` varchar(1024) DEFAULT NULL,
  `Short_Name_EN` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE,
  KEY `Listed_Market` (`Listed_Market`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_bond_rating`
--

DROP TABLE IF EXISTS `p_bond_rating`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_bond_rating` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` datetime DEFAULT NULL,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Rating_Institution_Code` varchar(7) DEFAULT NULL,
  `Rating_Date` decimal(8,0) DEFAULT NULL,
  `Bond_Rating` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_conversion_factor`
--

DROP TABLE IF EXISTS `p_conversion_factor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_conversion_factor` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `listed_mkt_type` varchar(3) DEFAULT NULL,
  `bond_id` varchar(16) DEFAULT NULL,
  `short_name` varchar(16) DEFAULT NULL,
  `conversion_rate` decimal(7,4) DEFAULT NULL,
  `conversion_start_date` decimal(8,0) DEFAULT NULL,
  `conversion_end_date` decimal(8,0) DEFAULT NULL,
  `publish_date` decimal(8,0) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `bond_id` (`bond_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_dict_holiday`
--

DROP TABLE IF EXISTS `p_dict_holiday`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_dict_holiday` (
  `Country` varchar(3) NOT NULL,
  `Market` varchar(16) NOT NULL,
  `Holiday` int(8) NOT NULL,
  `Status` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`Country`,`Market`,`Holiday`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_dict_macro_index`
--

DROP TABLE IF EXISTS `p_dict_macro_index`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_dict_macro_index` (
  `Code` varchar(64) NOT NULL,
  `Value` double DEFAULT NULL,
  `Date` int(8) NOT NULL,
  `EndDate` int(8) DEFAULT NULL,
  PRIMARY KEY (`Code`,`Date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_dict_qb_bond_list`
--

DROP TABLE IF EXISTS `p_dict_qb_bond_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_dict_qb_bond_list` (
  `Bond_Key` varchar(32) NOT NULL,
  `Guarantee` int(6) DEFAULT NULL,
  `Rate_Type` varchar(32) DEFAULT NULL,
  `Option_Type` varchar(32) DEFAULT '',
  `Is_Underwriter` varchar(1) DEFAULT NULL,
  `Underwriters` varchar(500) DEFAULT NULL,
  `MainUnderwriters` varchar(256) DEFAULT NULL,
  `JointUnderwriters` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`Bond_Key`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_institution_rating`
--

DROP TABLE IF EXISTS `p_institution_rating`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_institution_rating` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Institution_Code` varchar(7) DEFAULT NULL,
  `Institution_Type` varchar(3) DEFAULT NULL,
  `Rating_Institution_Code` varchar(7) DEFAULT NULL,
  `Rating_Date` decimal(8,0) DEFAULT NULL,
  `Rate` varchar(10) DEFAULT NULL,
  `Outlook` varchar(3) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Institution_Code` (`Institution_Code`) USING BTREE,
  KEY `Rating_Date` (`Rating_Date`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_province_info`
--

DROP TABLE IF EXISTS `p_province_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_province_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Area_Code` varchar(16) DEFAULT NULL,
  `Area_English_Name` varchar(32) DEFAULT NULL,
  `Area_Name` varchar(16) DEFAULT NULL,
  `Area_Level` varchar(16) DEFAULT NULL,
  `Area_Upper_Grade_Code` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_selective_info`
--

DROP TABLE IF EXISTS `p_selective_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_selective_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Selective_Code` varchar(31) DEFAULT NULL,
  `Selective_Content` varchar(500) DEFAULT NULL,
  `Sortcode` varchar(3) DEFAULT NULL,
  `Group_Name` varchar(30) DEFAULT NULL,
  `Selective_Content_EN` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_sw_sector_info`
--

DROP TABLE IF EXISTS `p_sw_sector_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_sw_sector_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `SW_External_Code` varchar(16) DEFAULT NULL,
  `SW_Internal_Code` varchar(16) DEFAULT NULL,
  `SW_Level` varchar(1) DEFAULT NULL,
  `SW_Name` varchar(16) DEFAULT NULL,
  `SW_English_Name` varchar(64) DEFAULT NULL,
  `SW_Upper_Grade_Code` varchar(16) DEFAULT NULL,
  `SW_Status` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_underwriter_info`
--

DROP TABLE IF EXISTS `p_underwriter_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_underwriter_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Bond_Key` varchar(25) DEFAULT NULL,
  `Underwriter_Code` varchar(7) DEFAULT NULL,
  `Underwriter_Type` varchar(3) DEFAULT NULL,
  `Book_Runner` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tender_upload`
--

DROP TABLE IF EXISTS `tender_upload`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tender_upload` (
  `id` varchar(32) NOT NULL,
  `Bond_Key` varchar(25) NOT NULL,
  `bond_id` varchar(32) NOT NULL,
  `user_id` varchar(32) NOT NULL,
  `user_name` varchar(255) DEFAULT NULL,
  `display_name` varchar(255) DEFAULT NULL,
  `company_id` varchar(32) DEFAULT NULL,
  `company_name` varchar(255) DEFAULT NULL,
  `upper_limit` varchar(16) NOT NULL COMMENT '上限',
  `lower_limit` varchar(16) NOT NULL COMMENT '下限',
  `step` varchar(16) NOT NULL COMMENT '步长',
  `is_match` int(1) NOT NULL DEFAULT '0' COMMENT '是否为匹配结果 (可用于查找感谢名单)',
  `match_num` int(1) NOT NULL DEFAULT '0' COMMENT '当前匹配数量。 （页面显示进度）',
  `modified_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='保存用户上传标书信息(上下限及步长)。 可根据 Bond_Key 与 is_match 字段查询匹配成功的感谢名单。 ';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:30:00
