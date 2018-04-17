-- MySQL dump 10.13  Distrib 5.5.49, for Linux (x86_64)
--
-- Host: localhost    Database: idb_bond
-- ------------------------------------------------------
-- Server version	5.5.49-log

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
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `login_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `password` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `user_name` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `flag` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `default_group` varchar(255) DEFAULT '1',
  `code` varchar(45) DEFAULT NULL,
  `phoneNumber` varchar(45) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  `address` varchar(45) DEFAULT NULL,
  `tel` varchar(45) DEFAULT NULL,
  `product` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account_all`
--

DROP TABLE IF EXISTS `account_all`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_all` (
  `id` varchar(32) NOT NULL,
  `login_name` varchar(45) DEFAULT NULL,
  `username` varchar(32) DEFAULT NULL,
  `password` varchar(32) DEFAULT NULL,
  `phoneNumber` varchar(32) DEFAULT NULL,
  `email` varchar(32) DEFAULT NULL,
  `address` varchar(32) DEFAULT NULL,
  `status` varchar(5) DEFAULT '1',
  `create_time` datetime DEFAULT NULL,
  `tel` varchar(32) DEFAULT NULL,
  `flag` varchar(45) DEFAULT NULL,
  `default_group` varchar(45) DEFAULT NULL,
  `code` varchar(45) DEFAULT NULL,
  `is_customer` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='客户信息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account_group`
--

DROP TABLE IF EXISTS `account_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_group` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `group_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `description` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `group_level` int(8) DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `dept_id` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_accountgroup_dept` (`dept_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account_group_console_permission`
--

DROP TABLE IF EXISTS `account_group_console_permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_group_console_permission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `console_permission_id` varchar(32) NOT NULL,
  `account_group_id` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=630 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account_group_member`
--

DROP TABLE IF EXISTS `account_group_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_group_member` (
  `account_group_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `account_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`account_group_id`,`account_id`),
  KEY `fk_member_account` (`account_id`),
  KEY `fk_member_group` (`account_group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`account_id`) REFER `artogrid/accoun';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bank`
--

DROP TABLE IF EXISTS `bank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bank` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `name` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `name_en` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `description` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `code` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `attribute` varchar(200) DEFAULT NULL,
  `attribute1` varchar(200) DEFAULT NULL,
  `attribute2` varchar(200) DEFAULT NULL,
  `attribute3` varchar(200) DEFAULT NULL,
  `attribute4` varchar(200) DEFAULT NULL,
  `attribute5` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; InnoDB free: 13312 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bank_account`
--

DROP TABLE IF EXISTS `bank_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bank_account` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `account_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `bank_agent_id` varchar(32) DEFAULT NULL,
  `sort` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_bank_acc_reference_account` (`account_id`),
  KEY `fk_bank_acc_ref_bank_agent` (`bank_agent_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`account_id`) REFER `artogrid/accoun';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bank_agent`
--

DROP TABLE IF EXISTS `bank_agent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bank_agent` (
  `id` varchar(32) NOT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `name` varchar(100) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `code` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `f_code` varchar(200) DEFAULT NULL,
  `attribute` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `attribute1` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `attribute2` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `attribute3` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `attribute4` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `attribute5` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `flag` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `customer_id` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_bank_agent_ref_bank` (`bank_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bank_relation`
--

DROP TABLE IF EXISTS `bank_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bank_relation` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `rel_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `MONTH` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_bank_rel_reference_bank` (`bank_id`),
  KEY `fk_bank_rel_reference_bank_2` (`rel_id`),
  KEY `fk_bank_rel_reference_goods` (`goods_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`bank_id`) REFER `artogrid/bank`(`id';
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
  `Redemption_No` decimal(3,0) DEFAULT NULL,
  `Redemption_Str` varchar(511) DEFAULT NULL,
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
  `No_Add_Issue` int(11) DEFAULT NULL,
  `Is_Cross_Mkt` varchar(1) DEFAULT NULL,
  `Is_Mortgage` varchar(1) DEFAULT NULL,
  `Mkt_Type` varchar(3) DEFAULT NULL,
  `Ann_Status` varchar(1) DEFAULT NULL,
  `Liquidity_Supporter` varchar(64) DEFAULT NULL,
  `Outstanding_Amount` decimal(16,4) DEFAULT NULL,
  `Is_CIB` varchar(1) DEFAULT NULL,
  `Yield_Curve_Type` varchar(32) DEFAULT NULL,
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Issuer_Rating_Current_NPY` varchar(10) DEFAULT NULL,
  `Rating_Current_NPY` varchar(10) DEFAULT NULL,
  `Ets_Str` varchar(16) DEFAULT NULL,
  `Issuer_Outlook_Current` varchar(20) DEFAULT NULL,
  `P_Issuer_Outlook_Current` varchar(20) DEFAULT NULL,
  `Short_Name_EN` varchar(64) DEFAULT NULL,
  `Full_Name_EN` varchar(256) DEFAULT NULL,
  `Registered_Amount` decimal(16,4) DEFAULT NULL,
  `Bid_Limit_Bottom` decimal(4,2) DEFAULT NULL,
  `Bid_Limit_Top` decimal(4,2) DEFAULT NULL,
  `Issue_End_Time` varchar(4) DEFAULT NULL,
  `Auction_Date_Time` varchar(8) DEFAULT NULL,
  `Ent_Cor` varchar(3) DEFAULT NULL,
  `Issue_Period` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_ID` (`Bond_ID`),
  KEY `delflag` (`delflag`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_20160412`
--

DROP TABLE IF EXISTS `bond_20160412`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_20160412` (
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
  `Underwriter_Code` varchar(100) DEFAULT NULL,
  `Yield_Curve_ID` varchar(15) DEFAULT NULL,
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
  `Redemption_No` decimal(3,0) DEFAULT NULL,
  `Redemption_Str` varchar(160) DEFAULT NULL,
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
  `No_Add_Issue` int(11) DEFAULT NULL,
  `Is_Cross_Mkt` varchar(1) DEFAULT NULL,
  `Is_Mortgage` varchar(1) DEFAULT NULL,
  `Mkt_Type` varchar(3) DEFAULT NULL,
  `Ann_Status` varchar(1) DEFAULT NULL,
  `Liquidity_Supporter` varchar(64) DEFAULT NULL,
  `Outstanding_Amount` decimal(16,4) DEFAULT NULL,
  `Is_CIB` varchar(1) DEFAULT NULL,
  `Yield_Curve_Type` varchar(32) DEFAULT NULL,
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Issuer_Rating_Current_NPY` varchar(10) DEFAULT NULL,
  `Rating_Current_NPY` varchar(10) DEFAULT NULL,
  `Ets_Str` varchar(16) DEFAULT NULL,
  `Issuer_Outlook_Current` varchar(20) DEFAULT NULL,
  `P_Issuer_Outlook_Current` varchar(20) DEFAULT NULL,
  `Short_Name_EN` varchar(64) DEFAULT NULL,
  `Full_Name_EN` varchar(256) DEFAULT NULL,
  `Registered_Amount` decimal(16,4) DEFAULT NULL,
  `Bid_Limit_Bottom` decimal(4,2) DEFAULT NULL,
  `Bid_Limit_Top` decimal(4,2) DEFAULT NULL,
  `Issue_End_Time` varchar(4) DEFAULT NULL,
  `Auction_Date_Time` varchar(8) DEFAULT NULL,
  `Ent_Cor` varchar(3) DEFAULT NULL,
  `Issue_Period` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Bond_ID` (`Bond_ID`),
  KEY `delflag` (`delflag`),
  KEY `Bond_Key` (`Bond_Key`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_bank`
--

DROP TABLE IF EXISTS `bond_bank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_bank` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `company_id` varchar(32) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `name_en` varchar(64) DEFAULT NULL,
  `description` varchar(1024) DEFAULT NULL,
  `code` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `attribute` varchar(200) DEFAULT NULL,
  `attribute1` varchar(200) DEFAULT NULL,
  `attribute2` varchar(200) DEFAULT NULL,
  `attribute3` varchar(200) DEFAULT NULL,
  `attribute4` varchar(200) DEFAULT NULL,
  `attribute5` varchar(200) DEFAULT NULL,
  `bank_type` varchar(200) DEFAULT NULL,
  `full_name` varchar(256) DEFAULT NULL,
  `city` varchar(256) DEFAULT NULL,
  `full_name_en` varchar(256) DEFAULT NULL,
  `pinyin` varchar(1024) DEFAULT NULL,
  `pinyin_full` varchar(2048) DEFAULT NULL,
  `city_name` varchar(255) DEFAULT NULL,
  `crm_id` int(20) DEFAULT NULL,
  `modify_time` datetime DEFAULT NULL,
  `dup_id` varchar(32) DEFAULT NULL,
  `qb_id` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; InnoDB free: 13312 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_bank_20170627`
--

DROP TABLE IF EXISTS `bond_bank_20170627`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_bank_20170627` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `company_id` varchar(32) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `name_en` varchar(64) DEFAULT NULL,
  `description` varchar(128) DEFAULT NULL,
  `code` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `attribute` varchar(200) DEFAULT NULL,
  `attribute1` varchar(200) DEFAULT NULL,
  `attribute2` varchar(200) DEFAULT NULL,
  `attribute3` varchar(200) DEFAULT NULL,
  `attribute4` varchar(200) DEFAULT NULL,
  `attribute5` varchar(200) DEFAULT NULL,
  `bank_type` varchar(200) DEFAULT NULL,
  `full_name` varchar(200) DEFAULT NULL,
  `city` varchar(255) DEFAULT NULL,
  `full_name_en` varchar(255) DEFAULT NULL,
  `pinyin` varchar(255) DEFAULT NULL,
  `pinyin_full` varchar(1000) DEFAULT NULL,
  `city_name` varchar(255) DEFAULT NULL,
  `crm_id` int(20) DEFAULT NULL,
  `modify_time` datetime DEFAULT NULL,
  `dup_id` varchar(32) DEFAULT NULL,
  `qb_id` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; InnoDB free: 13312 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_bank_account`
--

DROP TABLE IF EXISTS `bond_bank_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_bank_account` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `account_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `bank_agent_id` varchar(32) DEFAULT NULL,
  `sort` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_bank_acc_reference_account` (`account_id`),
  KEY `fk_bank_acc_ref_bank_agent` (`bank_agent_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`account_id`) REFER `artogrid/accoun';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_bank_agent`
--

DROP TABLE IF EXISTS `bond_bank_agent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_bank_agent` (
  `id` varchar(32) NOT NULL,
  `company_id` varchar(32) DEFAULT NULL,
  `bank_id` varchar(32) DEFAULT NULL,
  `name` varchar(100) DEFAULT NULL,
  `code` varchar(50) DEFAULT NULL,
  `f_code` varchar(200) DEFAULT NULL,
  `attribute` varchar(200) DEFAULT NULL,
  `attribute1` varchar(200) DEFAULT NULL,
  `attribute2` varchar(200) DEFAULT NULL,
  `attribute3` varchar(200) DEFAULT NULL,
  `attribute4` varchar(200) DEFAULT NULL,
  `attribute5` varchar(200) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `flag` varchar(5) DEFAULT NULL,
  `flag_vip` bit(1) DEFAULT b'0',
  `flag_bad` bit(1) DEFAULT b'0',
  `flag_bargain` varchar(1) DEFAULT NULL,
  `customer_id` varchar(32) DEFAULT NULL,
  `pinyin` varchar(255) DEFAULT NULL,
  `pinyin_full` varchar(1000) DEFAULT NULL,
  `crm_id` int(20) DEFAULT NULL,
  `crm_bank_id` int(20) DEFAULT NULL,
  `modify_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_bank_agent_ref_bank` (`bank_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_best_offer`
--

DROP TABLE IF EXISTS `bond_best_offer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_best_offer` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `company_id` varchar(32) DEFAULT NULL,
  `goods_id` varchar(32) DEFAULT NULL,
  `goods_code` varchar(255) DEFAULT NULL,
  `short_name` varchar(255) DEFAULT NULL,
  `goods_term` varchar(255) DEFAULT NULL,
  `goods_type` varchar(255) DEFAULT NULL,
  `goods_level` varchar(255) DEFAULT NULL,
  `profit_code` varchar(255) DEFAULT NULL,
  `ofr_agent_code` varchar(32) DEFAULT NULL,
  `ofr_price` decimal(10,4) DEFAULT NULL,
  `ofr_volume` varchar(255) DEFAULT NULL,
  `ofr_flag_vip` bit(1) DEFAULT b'0',
  `ofr_flag_bad` bit(1) DEFAULT b'0',
  `ofr_flag_bargain` varchar(1) DEFAULT NULL,
  `bid_agent_code` varchar(255) DEFAULT NULL,
  `bid_price` decimal(10,4) DEFAULT NULL,
  `bid_volume` varchar(255) DEFAULT NULL,
  `bid_flag_vip` bit(1) DEFAULT b'0',
  `bid_flag_bad` bit(1) DEFAULT b'0',
  `bid_flag_bargain` varchar(1) DEFAULT NULL,
  `status` varchar(5) DEFAULT '1',
  `create_time` datetime DEFAULT NULL,
  `underwriter` varchar(255) DEFAULT NULL,
  `bid_price_description` varchar(255) DEFAULT NULL,
  `ofr_price_description` varchar(255) DEFAULT NULL,
  `underwriter_type` varchar(255) DEFAULT NULL,
  `option_type` varchar(255) DEFAULT NULL,
  `bid_offer_count` int(11) DEFAULT '0',
  `ofr_offer_count` int(11) DEFAULT '0',
  `ofr_agent_id` varchar(255) DEFAULT NULL,
  `bid_agent_id` varchar(255) DEFAULT NULL,
  `rate_type` varchar(255) DEFAULT NULL,
  `bid_offer_id` varchar(45) DEFAULT NULL,
  `ofr_offer_id` varchar(45) DEFAULT NULL,
  `bid_before_price` decimal(10,4) DEFAULT NULL,
  `ofr_before_price` decimal(10,4) DEFAULT NULL,
  `internally` varchar(255) DEFAULT NULL,
  `bid_flag_relation` varchar(1) DEFAULT '0',
  `ofr_flag_relation` varchar(1) DEFAULT NULL,
  `ofr_quote_type` varchar(5) DEFAULT '0',
  `bid_quote_type` varchar(5) DEFAULT '0',
  `bid_return_point` decimal(10,4) DEFAULT NULL,
  `ofr_return_point` decimal(10,4) DEFAULT NULL,
  `bid_full_price` decimal(10,4) DEFAULT NULL,
  `ofr_full_price` decimal(10,4) DEFAULT NULL,
  `bid_yield` decimal(10,4) DEFAULT NULL,
  `ofr_yield` decimal(10,4) DEFAULT NULL,
  `bid_spread` decimal(10,4) DEFAULT NULL,
  `ofr_spread` decimal(10,4) DEFAULT NULL,
  `bid_clear_speed` varchar(10) DEFAULT NULL,
  `ofr_clear_speed` varchar(10) DEFAULT NULL,
  `bid_delivery_date` datetime DEFAULT NULL,
  `ofr_delivery_date` datetime DEFAULT NULL,
  `bid_exercise` varchar(10) DEFAULT NULL,
  `ofr_exercise` varchar(10) DEFAULT NULL,
  `bid_rebate` varchar(1) DEFAULT '0',
  `ofr_rebate` varchar(1) DEFAULT '0',
  `bid_offer_num` int(11) DEFAULT '0',
  `ofr_offer_num` int(11) DEFAULT '0',
  `ofr_net_price` decimal(10,4) DEFAULT NULL,
  `bid_net_price` decimal(10,4) DEFAULT NULL,
  `internally_bid_price` decimal(10,4) DEFAULT NULL,
  `internally_ofr_price` decimal(10,4) DEFAULT NULL,
  `internally_bid_return_point` decimal(10,4) DEFAULT NULL,
  `internally_ofr_return_point` decimal(10,4) DEFAULT NULL,
  `internally_bid_rebate` varchar(1) DEFAULT '0',
  `internally_ofr_rebate` varchar(1) DEFAULT '0',
  `internally_bid_offer_id` varchar(45) DEFAULT NULL,
  `internally_ofr_offer_id` varchar(45) DEFAULT NULL,
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  `ofr_broker_id` varchar(1024) DEFAULT NULL,
  `bid_broker_id` varchar(1024) DEFAULT NULL,
  `mkt_type` varchar(3) DEFAULT NULL,
  `deal_time` datetime DEFAULT NULL,
  `deal_price` decimal(10,4) DEFAULT NULL,
  `deal_type` varchar(255) DEFAULT NULL,
  `deal_volume` int(8) DEFAULT NULL,
  PRIMARY KEY (`Id`),
  KEY `goods_id` (`goods_id`,`goods_code`,`status`),
  KEY `bid_agent_code` (`bid_agent_code`),
  KEY `goods_code` (`goods_code`),
  KEY `goods_id_index` (`goods_id`),
  KEY `ofr_agent_code` (`ofr_agent_code`),
  KEY `internally` (`internally`),
  KEY `bond_key_listed_market` (`bond_key_listed_market`,`company_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_change_message`
--

DROP TABLE IF EXISTS `bond_change_message`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_change_message` (
  `Id` varchar(50) NOT NULL DEFAULT '',
  `rev` varchar(512) DEFAULT NULL,
  `sent` varchar(255) DEFAULT NULL,
  `dept` varchar(255) DEFAULT NULL,
  `type` varchar(255) DEFAULT NULL,
  `operate` varchar(255) DEFAULT NULL,
  `body` varchar(2000) DEFAULT NULL,
  `goodscode` varchar(255) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `goods_id` varchar(255) DEFAULT NULL,
  `sendStatus` varchar(255) DEFAULT NULL,
  `modify_time` datetime DEFAULT NULL,
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_deal`
--

DROP TABLE IF EXISTS `bond_deal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_deal` (
  `id` varchar(32) NOT NULL,
  `company_id` varchar(32) DEFAULT NULL,
  `deal_time` datetime DEFAULT NULL COMMENT '交易时间',
  `bid_bank_id` varchar(45) DEFAULT NULL COMMENT '买方机构id',
  `bid_bank_code` varchar(45) DEFAULT NULL COMMENT '买方机构代码',
  `bid_bank_city` varchar(45) DEFAULT NULL COMMENT '买方机构所在地',
  `bid_agent_id` varchar(45) DEFAULT NULL COMMENT '买方交易员id',
  `bid_agent_code` varchar(45) DEFAULT NULL COMMENT '买方交易员代码',
  `bid_broker_id` varchar(45) DEFAULT NULL COMMENT '买方经纪人id A',
  `bid_broker_name` varchar(45) DEFAULT NULL COMMENT '买方经纪人名字 A',
  `bid_brokerrage` decimal(10,4) DEFAULT NULL COMMENT '买方佣金',
  `bid_check` bit(1) DEFAULT b'0' COMMENT 'bid方是否确认',
  `ofr_bank_id` varchar(45) DEFAULT NULL COMMENT '卖方机构id',
  `ofr_bank_code` varchar(45) DEFAULT NULL COMMENT '卖方机构代码',
  `ofr_bank_city` varchar(45) DEFAULT NULL COMMENT '卖方机构所在地',
  `ofr_agent_id` varchar(45) DEFAULT NULL COMMENT '卖方交易员id',
  `ofr_agent_code` varchar(45) DEFAULT NULL COMMENT '卖方交易员代码',
  `ofr_broker_id` varchar(45) DEFAULT NULL COMMENT '卖方经纪人id A',
  `ofr_broker_name` varchar(45) DEFAULT NULL COMMENT '卖方经纪人名字 A',
  `ofr_brokerrage` decimal(10,4) DEFAULT NULL COMMENT '卖方佣金',
  `ofr_check` bit(1) DEFAULT b'0' COMMENT 'ofr方是否确认',
  `price` decimal(10,4) DEFAULT NULL COMMENT '价格',
  `volume` int(11) DEFAULT NULL COMMENT '数量 券面总额',
  `currency` varchar(45) DEFAULT NULL COMMENT '货币类型 CNY',
  `full_price` decimal(14,8) DEFAULT NULL COMMENT '全价',
  `net_price` decimal(14,8) DEFAULT NULL COMMENT '净价',
  `settlement_date` varchar(45) DEFAULT NULL COMMENT '交割日',
  `delivery_type` varchar(45) DEFAULT NULL COMMENT '结算方式 DVP(券款对付)',
  `goods_id` varchar(45) DEFAULT NULL,
  `goods_code` varchar(45) DEFAULT NULL,
  `goods_short_name` varchar(45) DEFAULT NULL,
  `maturity_date` varchar(45) DEFAULT NULL COMMENT '债券到期日',
  `deal_status` varchar(5) DEFAULT '0' COMMENT '成交单状态, 0-确认，1-待提交，2-已提交，3-已通过，5-未通过，7-已毁单，13-送审中，-1-已删除',
  `description` varchar(450) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL COMMENT '成交时间',
  `bid_bridge` varchar(5) DEFAULT '2' COMMENT 'Bid方是否为过桥机构 "2"-过桥机构， "1"-非过桥机构',
  `bid_charge` varchar(5) DEFAULT '1' COMMENT 'bid是否收费 "2"-收费，"1"-不收费',
  `ofr_bridge` varchar(5) DEFAULT '2' COMMENT 'Ofr方是否为过桥机构 "2"-过桥机构， "1"-非过桥机构',
  `ofr_charge` varchar(45) DEFAULT '1' COMMENT 'ofr是否收费 "2"-收费，"1"-不收费',
  `deal_type` varchar(255) DEFAULT NULL COMMENT '买卖方向, GVN/TKN/TRD',
  `group_flag` varchar(255) DEFAULT NULL,
  `time_style` varchar(255) DEFAULT NULL COMMENT '结算方式 T+0 T+1',
  `confirm_account` varchar(255) DEFAULT NULL,
  `confirm_date` date DEFAULT NULL,
  `bid_check_date` date DEFAULT NULL,
  `ofr_check_date` date DEFAULT NULL,
  `bid_trader_check` bit(1) DEFAULT b'1',
  `bid_trader_check_date` date DEFAULT NULL,
  `ofr_trader_check` bit(1) DEFAULT b'1',
  `ofr_trader_check_date` date DEFAULT NULL,
  `remarks` varchar(500) DEFAULT NULL COMMENT '后台信息',
  `odd_numbers` varchar(32) DEFAULT NULL COMMENT '单号',
  `exercise` varchar(5) DEFAULT NULL COMMENT '0-行权 1-到期',
  `is_print` varchar(5) DEFAULT '0' COMMENT '0-未打印 1-已打印',
  `return_point` decimal(10,4) DEFAULT NULL COMMENT '返点数值, 比如返0.12',
  `spread` decimal(10,4) DEFAULT NULL COMMENT '利差',
  `clear_speed` varchar(10) DEFAULT NULL,
  `delivery_date` datetime DEFAULT NULL,
  `rebate` varchar(1) DEFAULT '0' COMMENT '是否返点 0, 1',
  `internally` varchar(255) DEFAULT NULL COMMENT '是否内部成交, "1"-非内部成交，“2”-内部成交',
  `send_status` varchar(1) DEFAULT NULL,
  `quote_type` varchar(5) DEFAULT '3' COMMENT '报价类型 收益率/净价/全价/利差',
  `yield` decimal(14,8) DEFAULT NULL COMMENT '收益率',
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL COMMENT '键值加市场唯一标示一个债券',
  `Traded_Date` varchar(16) DEFAULT NULL,
  `sequence_number` int(11) DEFAULT NULL COMMENT '序列号',
  `total_price` varchar(32) DEFAULT NULL COMMENT '结算金额',
  `bid_remarks` varchar(500) DEFAULT NULL COMMENT 'bid备注',
  `ofr_remarks` varchar(500) DEFAULT NULL COMMENT 'ofr备注',
  `bid_brokerage_rate` decimal(10,4) DEFAULT NULL,
  `ofr_brokerage_rate` decimal(10,4) DEFAULT NULL,
  `bid_discount` decimal(10,4) DEFAULT NULL,
  `ofr_discount` decimal(10,4) DEFAULT NULL,
  `updateTime` datetime DEFAULT NULL COMMENT '更新时间',
  `urgent` varchar(10) DEFAULT NULL COMMENT '加急标志',
  `bidBankAgentName` varchar(45) DEFAULT NULL COMMENT '买方交易员名字',
  `bidBrokerIdB` varchar(45) DEFAULT NULL COMMENT '买方经纪人id B',
  `bidBrokerIdC` varchar(45) DEFAULT NULL COMMENT '买方经纪人id C',
  `bidBrokerIdD` varchar(45) DEFAULT NULL COMMENT '买方经纪人id D',
  `bidBrokerCodeA` varchar(45) DEFAULT NULL COMMENT '买方经纪人代码 A',
  `bidBrokerCodeB` varchar(45) DEFAULT NULL COMMENT '买方经纪人代码 B',
  `bidBrokerCodeC` varchar(45) DEFAULT NULL COMMENT '买方经纪人代码 C',
  `bidBrokerCodeD` varchar(45) DEFAULT NULL COMMENT '买方经纪人代码 D',
  `bidPercentA` varchar(5) DEFAULT NULL COMMENT '买方经纪人比例 A',
  `bidPercentB` varchar(5) DEFAULT NULL COMMENT '买方经纪人比例 B',
  `bidPercentC` varchar(5) DEFAULT NULL COMMENT '买方经纪人比例 C',
  `bidPercentD` varchar(5) DEFAULT NULL COMMENT '买方经纪人比例 D',
  `bidIsNoBrokerage` varchar(5) DEFAULT '1' COMMENT 'bid是否免佣 "2"-免佣，"1"-不免佣',
  `bidBrokerrageRemark` varchar(255) DEFAULT NULL COMMENT 'bid佣金备注',
  `bidTradeMode` varchar(45) DEFAULT NULL COMMENT 'bid方交易方式',
  `bidNoConfirm` varchar(5) DEFAULT '0' COMMENT 'Bid方NC "1"-NC，"0"-非NC',
  `bidNoComments` varchar(255) DEFAULT NULL COMMENT 'Bid方NC备注',
  `ofrBankAgentName` varchar(45) DEFAULT NULL COMMENT '卖方交易员名字',
  `ofrBrokerIdB` varchar(45) DEFAULT NULL COMMENT '卖方经纪人id B',
  `ofrBrokerIdC` varchar(45) DEFAULT NULL COMMENT '卖方经纪人id C',
  `ofrBrokerIdD` varchar(45) DEFAULT NULL COMMENT '卖方经纪人id D',
  `ofrBrokerCodeA` varchar(36) DEFAULT NULL COMMENT '卖方经纪人代码 A',
  `ofrBrokerCodeB` varchar(45) DEFAULT NULL COMMENT '卖方经纪人代码 B',
  `ofrBrokerCodeC` varchar(45) DEFAULT NULL COMMENT '卖方经纪人代码 C',
  `ofrBrokerCodeD` varchar(45) DEFAULT NULL COMMENT '卖方经纪人代码 D',
  `ofrPercentA` varchar(5) DEFAULT NULL COMMENT '卖方经纪人比例 A',
  `ofrPercentB` varchar(5) DEFAULT NULL COMMENT '卖方经纪人比例 B',
  `ofrPercentC` varchar(5) DEFAULT NULL COMMENT '卖方经纪人比例 C',
  `ofrPercentD` varchar(5) DEFAULT NULL COMMENT '卖方经纪人比例 D',
  `ofrIsNoBrokerage` varchar(5) DEFAULT '1' COMMENT 'ofr是否免佣 "2"-免佣，"1"-不免佣',
  `ofrBrokerrageRemark` varchar(255) DEFAULT NULL COMMENT 'ofr佣金备注',
  `ofrTradeMode` varchar(45) DEFAULT NULL COMMENT 'ofr方交易方式',
  `ofrNoConfirm` varchar(5) DEFAULT '0' COMMENT 'ofr方NC "1"-NC，"0"-非NC',
  `ofrNoComments` varchar(255) DEFAULT NULL COMMENT 'ofr方NC备注',
  `specialPartyBid` varchar(500) DEFAULT NULL COMMENT 'bid方特别细节',
  `specialPartyOfr` varchar(500) DEFAULT NULL COMMENT 'ofr方特别细节',
  `specialTerms` varchar(500) DEFAULT NULL COMMENT '其他细节',
  `feedback` varchar(500) DEFAULT NULL COMMENT '后台反馈',
  PRIMARY KEY (`id`),
  KEY `deal_status` (`deal_status`),
  KEY `bid_agent_code` (`bid_agent_code`,`ofr_agent_code`,`goods_short_name`),
  KEY `ofr_agent_code` (`ofr_agent_code`),
  KEY `goods_short_name` (`goods_short_name`),
  KEY `volume` (`volume`),
  KEY `goods_code` (`goods_code`),
  KEY `price` (`price`),
  KEY `bid_broker_name` (`bid_broker_name`),
  KEY `ofr_broker_name` (`ofr_broker_name`),
  KEY `create_time` (`create_time`),
  KEY `group_flag` (`group_flag`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_deal_copy`
--

DROP TABLE IF EXISTS `bond_deal_copy`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_deal_copy` (
  `id` varchar(32) NOT NULL,
  `deal_time` datetime DEFAULT NULL COMMENT '成交时间',
  `bid_bank_id` varchar(45) DEFAULT NULL COMMENT '买入机构',
  `bid_bank_code` varchar(45) DEFAULT NULL,
  `bid_bank_city` varchar(45) DEFAULT NULL COMMENT '买入机构所在地',
  `bid_agent_id` varchar(45) DEFAULT NULL COMMENT '买入客户',
  `bid_agent_code` varchar(45) DEFAULT NULL,
  `bid_broker_id` varchar(45) DEFAULT NULL COMMENT '买方经纪人',
  `bid_broker_name` varchar(45) DEFAULT NULL,
  `bid_brokerrage` decimal(10,4) DEFAULT NULL COMMENT '买方佣金',
  `bid_check` bit(1) DEFAULT b'0' COMMENT 'Âò·½¾­¼ÍÈËÈ·ÈÏ',
  `ofr_bank_id` varchar(45) DEFAULT NULL COMMENT '卖方机构',
  `ofr_bank_code` varchar(45) DEFAULT NULL COMMENT '卖方交易员',
  `ofr_bank_city` varchar(45) DEFAULT NULL,
  `ofr_agent_id` varchar(45) DEFAULT NULL,
  `ofr_agent_code` varchar(45) DEFAULT NULL,
  `ofr_broker_id` varchar(45) DEFAULT NULL,
  `ofr_broker_name` varchar(45) DEFAULT NULL,
  `ofr_brokerrage` decimal(10,4) DEFAULT NULL COMMENT '卖方佣金',
  `ofr_check` bit(1) DEFAULT b'0' COMMENT 'Âô·½¾­¼ÍÈËÈ·ÈÏ',
  `price` decimal(10,4) DEFAULT NULL COMMENT 'YIELD 收益率',
  `volume` int(11) DEFAULT NULL COMMENT '数量 FACE AMOUNT券面总额',
  `currency` varchar(45) DEFAULT NULL COMMENT '货币类型 CNY',
  `full_price` decimal(10,4) DEFAULT NULL COMMENT '全价',
  `net_price` decimal(10,4) DEFAULT NULL COMMENT '净价',
  `settlement_date` varchar(45) DEFAULT NULL COMMENT '交割日',
  `delivery_type` varchar(45) DEFAULT NULL COMMENT '结算方式：DVP(券款对付)',
  `goods_id` varchar(45) DEFAULT NULL,
  `goods_code` varchar(45) DEFAULT NULL,
  `goods_short_name` varchar(45) DEFAULT NULL,
  `maturity_date` varchar(45) DEFAULT NULL COMMENT '债券到期日',
  `deal_status` varchar(5) DEFAULT '0' COMMENT '订单状态: 0-初始 ， 1-确认 ，2- 完成',
  `description` varchar(450) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `bid_bridge` varchar(5) DEFAULT '2' COMMENT 'bidÊÇ·ñ¹ýÇÅ',
  `bid_charge` varchar(5) DEFAULT '1' COMMENT 'bidÊÇ·ñÊÕ·Ñ',
  `ofr_bridge` varchar(5) DEFAULT '2' COMMENT 'ofrÊÇ·ñ¹ýÇÅ',
  `ofr_charge` varchar(45) DEFAULT '1' COMMENT 'ofrÊÇ·ñÊÕ·Ñ',
  `deal_type` varchar(255) DEFAULT NULL,
  `group_flag` varchar(255) DEFAULT NULL COMMENT 'OCO×éID',
  `time_style` varchar(255) DEFAULT NULL,
  `confirm_account` varchar(255) DEFAULT NULL,
  `confirm_date` date DEFAULT NULL,
  `bid_check_date` date DEFAULT NULL,
  `ofr_check_date` date DEFAULT NULL,
  `bid_trader_check` bit(1) DEFAULT b'1' COMMENT 'bid·½½»Ò×Ô±È·ÈÏ',
  `bid_trader_check_date` date DEFAULT NULL COMMENT 'bid·½½»Ò×Ô±È·ÈÏÊ±¼ä',
  `ofr_trader_check` bit(1) DEFAULT b'1' COMMENT 'ofr·½½»Ò×Ô±È·ÈÏ',
  `ofr_trader_check_date` date DEFAULT NULL COMMENT 'ofr·½½»Ò×Ô±È·ÈÏÊ±¼ä',
  `remarks` varchar(500) DEFAULT NULL,
  `odd_numbers` varchar(32) DEFAULT NULL COMMENT '单号',
  `exercise` varchar(5) DEFAULT NULL COMMENT '0-行权 1-到期',
  `is_print` varchar(5) DEFAULT '0' COMMENT '0-未打印 1-已打印',
  `return_point` decimal(10,4) DEFAULT NULL,
  `spread` decimal(10,4) DEFAULT NULL,
  `clear_speed` varchar(10) DEFAULT NULL,
  `delivery_date` datetime DEFAULT NULL,
  `rebate` varchar(1) DEFAULT '0',
  `internally` varchar(255) DEFAULT NULL,
  `send_status` varchar(1) DEFAULT NULL,
  `quote_type` varchar(5) DEFAULT '3',
  `yield` decimal(10,4) DEFAULT NULL,
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  `Traded_Date` varchar(16) DEFAULT NULL,
  `sequence_number` int(11) DEFAULT NULL,
  `total_price` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `deal_status` (`deal_status`),
  KEY `bid_agent_code` (`bid_agent_code`,`ofr_agent_code`,`goods_short_name`),
  KEY `ofr_agent_code` (`ofr_agent_code`),
  KEY `goods_short_name` (`goods_short_name`),
  KEY `volume` (`volume`),
  KEY `goods_code` (`goods_code`),
  KEY `price` (`price`),
  KEY `bid_broker_name` (`bid_broker_name`),
  KEY `ofr_broker_name` (`ofr_broker_name`),
  KEY `create_time` (`create_time`),
  KEY `group_flag` (`group_flag`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_deal_log`
--

DROP TABLE IF EXISTS `bond_deal_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_deal_log` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `deal_id` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL COMMENT 'bond_deal_log的创建时间',
  `status` varchar(255) DEFAULT NULL COMMENT 'bond_deal_log的状态',
  `oper_user_id` varchar(255) DEFAULT NULL COMMENT '操作用户id',
  `action` varchar(255) DEFAULT NULL COMMENT '操作动作:  add/update/delete/check/confirm/deal cancle',
  `symbol` varchar(255) DEFAULT NULL COMMENT '审核方向： all/ofr/bid',
  `bid_bank_id` varchar(45) DEFAULT NULL COMMENT '买入机构',
  `remarks` varchar(500) DEFAULT NULL COMMENT '备注',
  `ofr_trader_check_date` date DEFAULT NULL COMMENT 'ofr方交易员确认时间',
  `ofr_trader_check` bit(1) DEFAULT b'1' COMMENT 'ofr方交易员确认',
  `bid_trader_check_date` date DEFAULT NULL COMMENT 'bid方交易员确认时间',
  `bid_trader_check` bit(1) DEFAULT b'1' COMMENT 'bid方交易员确认',
  `ofr_check_date` date DEFAULT NULL COMMENT 'ofr方经纪人确认时间',
  `bid_check_date` date DEFAULT NULL COMMENT 'bid方经纪人确认时间',
  `confirm_date` date DEFAULT NULL COMMENT '审核时间',
  `confirm_account` varchar(255) DEFAULT NULL COMMENT '审核人帐号ID',
  `time_style` varchar(255) DEFAULT NULL COMMENT '清算速度',
  `group_flag` varchar(255) DEFAULT NULL COMMENT 'OCO组ID',
  `deal_type` varchar(255) DEFAULT NULL COMMENT '交易方向',
  `ofr_charge` varchar(45) DEFAULT '1' COMMENT 'ofr是否收费',
  `ofr_bridge` varchar(5) DEFAULT '2' COMMENT 'ofr是否过桥',
  `bid_charge` varchar(5) DEFAULT '1' COMMENT 'bid是否收费',
  `bid_bridge` varchar(5) DEFAULT '2' COMMENT 'bid是否过桥',
  `deal_create_time` datetime DEFAULT NULL COMMENT 'deal的创建时间',
  `bond_deal_status` varchar(5) DEFAULT NULL COMMENT 'bonddeal的状态',
  `description` varchar(450) DEFAULT NULL COMMENT '说明',
  `deal_status` varchar(5) DEFAULT '0' COMMENT '订单状态: 0-初始 ， 1-确认 ，2- 完成',
  `maturity_date` varchar(45) DEFAULT NULL COMMENT '债券到期日',
  `goods_short_name` varchar(45) DEFAULT NULL COMMENT '产品简称',
  `goods_code` varchar(45) DEFAULT NULL COMMENT '产品代码',
  `goods_id` varchar(45) DEFAULT NULL COMMENT '产品ID',
  `delivery_type` varchar(45) DEFAULT NULL COMMENT '结算方式：DVP(券款对付)',
  `settlement_date` varchar(45) DEFAULT NULL COMMENT '交割日',
  `net_price` decimal(10,4) DEFAULT NULL COMMENT '净价',
  `full_price` decimal(10,4) DEFAULT NULL COMMENT '全价',
  `currency` varchar(45) DEFAULT NULL COMMENT '货币类型 CNY',
  `volume` int(11) DEFAULT NULL COMMENT '数量 FACE AMOUNT券面总额',
  `price` decimal(10,4) DEFAULT NULL COMMENT 'YIELD 收益率',
  `ofr_check` bit(1) DEFAULT b'0' COMMENT '卖方经纪人确认',
  `ofr_brokerrage` decimal(10,4) DEFAULT NULL COMMENT '卖方佣金',
  `ofr_broker_name` varchar(45) DEFAULT NULL COMMENT '卖方经纪人名称',
  `ofr_broker_id` varchar(45) DEFAULT NULL COMMENT '卖方经纪人ID',
  `ofr_agent_code` varchar(45) DEFAULT NULL COMMENT '卖方交易员代码',
  `ofr_agent_id` varchar(45) DEFAULT NULL COMMENT '卖方交易员ID',
  `ofr_bank_city` varchar(45) DEFAULT NULL COMMENT '卖方城市',
  `ofr_bank_code` varchar(45) DEFAULT NULL COMMENT '卖方交易员',
  `ofr_bank_id` varchar(45) DEFAULT NULL COMMENT '卖方机构',
  `bid_check` bit(1) DEFAULT b'0' COMMENT '买方经纪人确认',
  `bid_brokerrage` decimal(10,4) DEFAULT NULL COMMENT '买方佣金',
  `bid_broker_name` varchar(45) DEFAULT NULL COMMENT '买方经纪人名称',
  `bid_broker_id` varchar(45) DEFAULT NULL COMMENT '买方经纪人',
  `bid_agent_code` varchar(45) DEFAULT NULL COMMENT '卖方交易员代码',
  `bid_agent_id` varchar(45) DEFAULT NULL COMMENT '买入交易员ID',
  `bid_bank_city` varchar(45) DEFAULT NULL COMMENT '买入机构所在地',
  `bid_bank_code` varchar(45) DEFAULT NULL COMMENT '卖方机构代码',
  `deal_time` datetime DEFAULT NULL COMMENT '成交时间',
  `bond_key` varchar(25) DEFAULT NULL COMMENT '产品key',
  `listed_market` varchar(3) DEFAULT NULL COMMENT '市场代码',
  `bond_key_listed_market` varchar(28) DEFAULT NULL COMMENT '产品key+市场代码',
  `odd_numbers` varchar(32) DEFAULT NULL COMMENT '单号',
  `exercise` varchar(5) DEFAULT NULL COMMENT '0-行权 1-到期',
  `is_print` varchar(5) DEFAULT '0' COMMENT '0-未打印 1-已打印',
  `return_point` decimal(10,4) DEFAULT NULL COMMENT '返点值',
  `spread` decimal(10,4) DEFAULT NULL COMMENT '利差',
  `clear_speed` varchar(10) DEFAULT NULL COMMENT '清算速度',
  `delivery_date` datetime DEFAULT NULL COMMENT '交割时间',
  `rebate` varchar(1) DEFAULT '0' COMMENT '是否返点 0：不是 1：是 ',
  `internally` varchar(255) DEFAULT NULL COMMENT '是否公开报价  1:公开  2：不公开',
  `send_status` varchar(1) DEFAULT NULL COMMENT '发送状态',
  `quote_type` varchar(5) DEFAULT '3' COMMENT '报价方式  1；收益率 2：净价 3：全价  4：利差',
  `yield` decimal(10,4) DEFAULT NULL COMMENT '收益率',
  `Traded_Date` varchar(16) DEFAULT NULL COMMENT '交割日期',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_dev`
--

DROP TABLE IF EXISTS `bond_dev`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_dev` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `key` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
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
  `Bond_Type` varchar(3) DEFAULT NULL,
  `Bond_Subtype` varchar(3) DEFAULT NULL,
  `External_Type` varchar(3) DEFAULT NULL,
  `Maturity_Term_Y` decimal(7,4) DEFAULT NULL,
  `Maturity_Term` decimal(5,0) DEFAULT NULL,
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
  `First_Exchg_Bond_Key` varchar(50) DEFAULT NULL,
  `First_Exchg_No` decimal(2,0) DEFAULT NULL,
  `First_Exchg_Str` varchar(200) DEFAULT NULL,
  `Sec_Exchg_No` decimal(2,0) DEFAULT NULL,
  `Sec_Exchg_Year` varchar(200) DEFAULT NULL,
  `Assign_Trans_Key` varchar(7) DEFAULT NULL,
  `Assign_Trans_No` decimal(2,0) DEFAULT NULL,
  `Assign_Trans_Str` varchar(200) DEFAULT NULL,
  `Compensate_Rate` varchar(10) DEFAULT NULL,
  `Compensate_From` decimal(2,0) DEFAULT NULL,
  `Option_Exercise` varchar(3) DEFAULT NULL,
  `Option_Exercise_Date` decimal(8,0) DEFAULT NULL,
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
  `Fixing_MA_Days` decimal(3,0) DEFAULT NULL,
  `Fixing_Preceds` varchar(3) DEFAULT NULL,
  `Fixing_Calendar_Key` varchar(3) DEFAULT NULL,
  `Fixing_Tailing` varchar(3) DEFAULT NULL,
  `Fixing_Digit` decimal(1,0) DEFAULT NULL,
  `Reset_Effective` varchar(3) DEFAULT NULL,
  `Cap` varchar(32) DEFAULT NULL,
  `Flr` varchar(32) DEFAULT NULL,
  `Simple_Compound` varchar(3) DEFAULT NULL,
  `Issue_Reference` varchar(32) DEFAULT NULL,
  `Issue_Reference_Desc` varchar(50) DEFAULT NULL,
  `Variable_Schedule` varchar(3) DEFAULT NULL,
  `Coupon_Day_Adjust` varchar(3) DEFAULT NULL,
  `Coupon_Day_DMC` varchar(3) DEFAULT NULL,
  `Coupon_Calendar_Key` varchar(3) DEFAULT NULL,
  `Pricing_Conv` varchar(3) DEFAULT NULL,
  `Issue_Price` decimal(16,4) DEFAULT NULL,
  `Planned_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `First_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `Redemption_No` decimal(3,0) DEFAULT NULL,
  `Redemption_Str` varchar(160) DEFAULT NULL,
  `Maturity_Adjust` varchar(3) DEFAULT NULL,
  `Maturity_DMC` varchar(3) DEFAULT NULL,
  `Maturity_Calendar_Key` varchar(3) DEFAULT NULL,
  `Sceniority` varchar(3) DEFAULT NULL,
  `Rating_Institution_Code` varchar(7) DEFAULT NULL,
  `Rating_Augment` varchar(8) DEFAULT NULL,
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
  `Note` varchar(1024) DEFAULT NULL,
  `Term_structure` varchar(20) DEFAULT NULL,
  `Term_Before_Exercise` decimal(7,4) DEFAULT NULL,
  `Bond_Collateral` varchar(100) DEFAULT NULL,
  `Collateral_Value` decimal(16,4) DEFAULT NULL,
  `Collateral_Type` varchar(15) DEFAULT NULL,
  `Purpose_of_Issue` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `bondkey` (`Bond_Key`,`delflag`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_goods`
--

DROP TABLE IF EXISTS `bond_goods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_goods` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `profit_code` varchar(32) DEFAULT NULL COMMENT '收益模式代码',
  `goods_code` varchar(32) DEFAULT NULL COMMENT '债券代码',
  `goods_type` varchar(255) DEFAULT NULL COMMENT '债券类型',
  `bond_type` varchar(32) DEFAULT NULL COMMENT '债券类型',
  `goods_level` varchar(32) DEFAULT NULL COMMENT '评级',
  `goods_term` varchar(32) DEFAULT NULL COMMENT '期限',
  `profit_id` varchar(32) DEFAULT NULL COMMENT '收益模式ID',
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL COMMENT '债券名词',
  `short_name` varchar(255) DEFAULT NULL COMMENT '简称',
  `issue_price` varchar(255) DEFAULT NULL COMMENT '发行价格',
  `currency` varchar(255) DEFAULT NULL COMMENT '币种',
  `issue_dept` varchar(255) DEFAULT NULL COMMENT '发行人姓名',
  `base_interestrate` varchar(255) DEFAULT NULL COMMENT '基础利率',
  `interestrate_difference` varchar(255) DEFAULT NULL COMMENT '固定利差',
  `minimum_interestrate` varchar(255) DEFAULT NULL COMMENT '票面利率',
  `issue_time` varchar(255) DEFAULT NULL COMMENT '发行日期',
  `payinterest_time` varchar(255) DEFAULT NULL COMMENT '起息日期',
  `market_type` varchar(255) DEFAULT NULL COMMENT '市场别',
  `circulation` varchar(255) DEFAULT NULL COMMENT '发行量',
  `pay_type` varchar(255) DEFAULT NULL COMMENT '付息方式',
  `underwriter` varchar(255) DEFAULT NULL COMMENT '担保人',
  `vendors` varchar(255) DEFAULT NULL COMMENT '承销商',
  `level_dept` varchar(255) DEFAULT NULL COMMENT '评级机构',
  `rate_type` varchar(255) DEFAULT NULL COMMENT '利率方式',
  `back_type` varchar(255) DEFAULT '' COMMENT '还本方式',
  `underwriter_type` varchar(255) DEFAULT NULL COMMENT '担保方式',
  `goods_type_id` varchar(255) DEFAULT NULL,
  `payinterest_endtime` varchar(255) DEFAULT NULL COMMENT '到期日期',
  `next_pay_time` varchar(255) DEFAULT NULL COMMENT '下次付息时间',
  `next_pay_days` varchar(255) DEFAULT NULL COMMENT '下次付息(天)',
  `listing_time` varchar(255) DEFAULT NULL COMMENT '上市日期',
  `repurchase_rate` varchar(255) DEFAULT NULL COMMENT '回购比例',
  `issue_yield` varchar(255) DEFAULT NULL COMMENT '发行收益率',
  `level_forday` varchar(255) DEFAULT NULL COMMENT '发行日评级',
  `bond_level` varchar(255) DEFAULT NULL COMMENT '债券评级',
  `bond_level_dept` varchar(255) DEFAULT NULL COMMENT '债券评级机构',
  `remain_year` varchar(255) DEFAULT NULL COMMENT '剩余期限_年',
  `rate_style` varchar(255) DEFAULT NULL COMMENT '利率别',
  `option_type` varchar(255) DEFAULT NULL COMMENT '期权方式',
  `option_style` varchar(255) DEFAULT NULL COMMENT '期权性质',
  `option_nature` varchar(255) DEFAULT NULL COMMENT '期权类别',
  `option_travel` varchar(255) DEFAULT NULL COMMENT '期权行驶',
  `par_compensation` varchar(255) DEFAULT NULL COMMENT '票面补偿',
  `par_late` varchar(255) DEFAULT NULL COMMENT '后期票面',
  `terminal` varchar(255) DEFAULT NULL COMMENT '期限',
  `pinyin` varchar(128) DEFAULT NULL,
  `pinyin_full` varchar(512) DEFAULT NULL,
  `expired` varchar(255) DEFAULT '1',
  `bond_key` varchar(25) DEFAULT NULL COMMENT 'Sumscope主键',
  `is_mortgage` char(1) DEFAULT 'N' COMMENT '是否质押',
  `mortgage_rate` varchar(32) DEFAULT NULL COMMENT '质押比例',
  `enterprise_type` varchar(3) DEFAULT 'NSO' COMMENT '企业性质:国企=SOE，非国企=NSO，港澳台=HMT，外企=FFE',
  `is_floating_rate` char(1) DEFAULT 'N' COMMENT '是否浮息',
  `is_cross_market` char(1) DEFAULT 'N' COMMENT '是否跨市场',
  `market_code` varchar(32) DEFAULT NULL COMMENT '交易所代码',
  `day_count_convention` varchar(32) DEFAULT NULL COMMENT '日计公约（ACT365）',
  `interest_accrual_date` varchar(255) DEFAULT NULL COMMENT '计息日',
  `first_coupon_date` varchar(255) DEFAULT NULL COMMENT '第一次付息时间',
  `redemption_value` varchar(32) DEFAULT NULL COMMENT '赎回价值',
  `allow_ex_dividend` char(1) DEFAULT 'N' COMMENT '是否除息',
  `ex_dividend_term` varchar(255) DEFAULT NULL COMMENT '除息时间',
  `ex_dividend_units` varchar(3) DEFAULT NULL COMMENT '除息时间单位',
  `mkt_type` varchar(32) NOT NULL DEFAULT '2' COMMENT '1：一级半 2：上市产品',
  PRIMARY KEY (`id`),
  KEY `fk_goods_profit` (`profit_id`),
  KEY `goods_code` (`goods_code`,`status`,`id`),
  KEY `Copy_of_goods_code` (`goods_code`,`id`),
  KEY `goods_code_index` (`goods_code`),
  KEY `goods_level` (`goods_level`),
  KEY `goods_type` (`goods_type`),
  KEY `option_type` (`option_type`),
  KEY `underwriter_type` (`underwriter_type`),
  KEY `short_name` (`short_name`),
  KEY `remain_year` (`remain_year`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_goods_group`
--

DROP TABLE IF EXISTS `bond_goods_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_goods_group` (
  `id` varchar(32) NOT NULL,
  `company_id` varchar(32) DEFAULT NULL,
  `ua_id` varchar(32) DEFAULT NULL,
  `group_name` varchar(32) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `ua_flag` varchar(5) DEFAULT NULL,
  `params` mediumblob,
  `filter_condition` varchar(6000) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_goods_group_20160519`
--

DROP TABLE IF EXISTS `bond_goods_group_20160519`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_goods_group_20160519` (
  `id` varchar(32) NOT NULL,
  `ua_id` varchar(32) DEFAULT NULL,
  `group_name` varchar(32) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `ua_flag` varchar(5) DEFAULT NULL,
  `params` mediumblob,
  `filter_condition` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_goods_type`
--

DROP TABLE IF EXISTS `bond_goods_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_goods_type` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `name` varchar(255) DEFAULT NULL,
  `code` varchar(255) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `product` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_group_relation`
--

DROP TABLE IF EXISTS `bond_group_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_group_relation` (
  `group_id` varchar(32) DEFAULT NULL,
  `goods_id` varchar(32) DEFAULT NULL,
  `id` varchar(255) NOT NULL,
  `account_type` char(1) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `ofr_price_range_begin` decimal(10,4) DEFAULT NULL,
  `ofr_price_range_end` decimal(10,4) DEFAULT NULL,
  `bid_price_range_begin` decimal(10,4) DEFAULT NULL,
  `bid_price_range_end` decimal(10,4) DEFAULT NULL,
  `flag_allow` bit(1) NOT NULL DEFAULT b'1' COMMENT '1-allow 2-forbid',
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `group_id` (`group_id`),
  KEY `goods_id` (`goods_id`),
  KEY `relation_bond_key` (`bond_key_listed_market`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary table structure for view `bond_group_relation_view`
--

DROP TABLE IF EXISTS `bond_group_relation_view`;
/*!50001 DROP VIEW IF EXISTS `bond_group_relation_view`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `bond_group_relation_view` (
  `group_id` tinyint NOT NULL,
  `goods_id` tinyint NOT NULL,
  `id` tinyint NOT NULL,
  `account_type` tinyint NOT NULL,
  `create_time` tinyint NOT NULL,
  `ofr_price_range_begin` tinyint NOT NULL,
  `ofr_price_range_end` tinyint NOT NULL,
  `bid_price_range_begin` tinyint NOT NULL,
  `bid_price_range_end` tinyint NOT NULL,
  `flag_allow` tinyint NOT NULL,
  `ua_id` tinyint NOT NULL,
  `account_id` tinyint NOT NULL,
  `bond_key` tinyint NOT NULL,
  `listed_market` tinyint NOT NULL,
  `bond_key_listed_market` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `bond_list_info`
--

DROP TABLE IF EXISTS `bond_list_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_list_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
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
  `pinyin` varchar(512) DEFAULT NULL,
  `pinyin_full` varchar(1024) DEFAULT NULL,
  `Listed_Date` decimal(8,0) DEFAULT NULL,
  `Delisted_Date` decimal(8,0) DEFAULT NULL,
  `Mkt_Type` varchar(3) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_log`
--

DROP TABLE IF EXISTS `bond_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_log` (
  `id` varchar(32) NOT NULL,
  `server_name` varchar(32) DEFAULT NULL,
  `service_name` varchar(32) DEFAULT NULL,
  `method` varchar(32) DEFAULT NULL,
  `operator_id` varchar(32) DEFAULT NULL,
  `request_msg` text,
  `reply_msg` text,
  `receive_msg_time` datetime DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_offer`
--

DROP TABLE IF EXISTS `bond_offer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_offer` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `company_id` varchar(32) DEFAULT NULL,
  `goods_id` varchar(32) DEFAULT NULL,
  `goods_code` varchar(32) DEFAULT NULL,
  `profit_code` varchar(32) DEFAULT NULL,
  `bank_name` varchar(32) DEFAULT NULL,
  `bank_id` varchar(32) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `volume` int(8) DEFAULT NULL,
  `deal_status` varchar(5) DEFAULT NULL,
  `deal_time` datetime DEFAULT NULL,
  `operator_id` varchar(32) DEFAULT NULL,
  `review_id` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `bank_agent_id` varchar(32) DEFAULT NULL,
  `agent_code` varchar(50) DEFAULT NULL,
  `flag_vip` bit(1) DEFAULT b'0',
  `flag_bad` bit(1) DEFAULT b'0',
  `flag_bargain` varchar(1) DEFAULT NULL,
  `price_description` varchar(225) DEFAULT NULL,
  `goods_type` varchar(255) DEFAULT NULL,
  `goods_short_name` varchar(255) DEFAULT NULL,
  `goods_level` varchar(255) DEFAULT NULL,
  `goods_term` varchar(255) DEFAULT NULL,
  `goods_name` varchar(255) DEFAULT NULL,
  `option_type` varchar(255) DEFAULT NULL COMMENT '期权方式',
  `rate_type` varchar(255) DEFAULT NULL,
  `internally` varchar(255) DEFAULT NULL,
  `flag_relation` varchar(1) DEFAULT NULL,
  `trader_id` varchar(32) DEFAULT NULL,
  `quote_type` varchar(5) DEFAULT '3',
  `return_point` decimal(10,4) DEFAULT NULL,
  `net_price` decimal(10,4) DEFAULT NULL,
  `full_price` decimal(10,4) DEFAULT NULL,
  `yield` decimal(10,4) DEFAULT NULL,
  `spread` decimal(10,4) DEFAULT NULL,
  `clear_speed` varchar(10) DEFAULT NULL,
  `delivery_date` datetime DEFAULT NULL,
  `exercise` varchar(10) DEFAULT NULL,
  `rebate` varchar(1) DEFAULT '0',
  `send_status` varchar(1) DEFAULT NULL,
  `calculate_status` int(1) DEFAULT '1',
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `goods_id` (`goods_id`,`goods_code`,`symbol`,`status`),
  KEY `create_time` (`create_time`),
  KEY `deal_status` (`deal_status`),
  KEY `agent_code` (`agent_code`,`volume`,`goods_code`),
  KEY `volume` (`volume`),
  KEY `goods_code` (`goods_code`),
  KEY `symbol` (`symbol`,`price`),
  KEY `price` (`price`),
  KEY `deal_time` (`deal_time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`operator_id`) REFER `artogrid/accou';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_offer_history`
--

DROP TABLE IF EXISTS `bond_offer_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_offer_history` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `goods_id` varchar(32) DEFAULT NULL,
  `goods_code` varchar(32) DEFAULT NULL,
  `profit_code` varchar(32) DEFAULT NULL,
  `bank_name` varchar(32) DEFAULT NULL,
  `bank_id` varchar(32) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `volume` int(8) DEFAULT NULL,
  `deal_status` varchar(5) DEFAULT NULL,
  `deal_time` datetime DEFAULT NULL,
  `operator_id` varchar(32) DEFAULT NULL,
  `review_id` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `bank_agent_id` varchar(32) DEFAULT NULL,
  `agent_code` varchar(50) DEFAULT NULL,
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_offer_log`
--

DROP TABLE IF EXISTS `bond_offer_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_offer_log` (
  `id` varchar(32) NOT NULL COMMENT 'bondofferlog的ID,主键',
  `action` varchar(255) DEFAULT NULL COMMENT '操作方式: add,update,delete',
  `create_time` datetime DEFAULT NULL COMMENT 'bondofferlog的创建时间',
  `offer_id` varchar(32) DEFAULT NULL COMMENT '记录报价的ID',
  `oper_user_id` varchar(45) DEFAULT NULL COMMENT '操作用户ID',
  `goods_id` varchar(32) DEFAULT NULL COMMENT '产品ID',
  `goods_code` varchar(32) DEFAULT NULL COMMENT '产品代码',
  `profit_code` varchar(32) DEFAULT NULL COMMENT '操作方式',
  `bank_name` varchar(32) DEFAULT NULL COMMENT '报价机构名称',
  `bank_id` varchar(32) DEFAULT NULL COMMENT '报价机构ID',
  `description` varchar(255) DEFAULT NULL COMMENT '备注',
  `symbol` int(8) DEFAULT NULL COMMENT '买卖方向  1:bid -1:ofr',
  `price` decimal(10,4) DEFAULT NULL COMMENT '价格',
  `volume` int(8) DEFAULT NULL COMMENT '交易量',
  `deal_status` varchar(5) DEFAULT NULL COMMENT '报价状态  -1:待审核 0:有效 2：作废',
  `deal_time` datetime DEFAULT NULL COMMENT '成交时间',
  `operator_id` varchar(32) DEFAULT NULL COMMENT '报价经纪人ID',
  `review_id` varchar(32) DEFAULT NULL COMMENT '审核人ID',
  `offer_create_time` datetime DEFAULT NULL COMMENT '报价创建时间',
  `status` varchar(5) DEFAULT NULL COMMENT '记录状态  1：有效  2：无效',
  `bank_agent_id` varchar(32) DEFAULT NULL COMMENT '报价交易员ID',
  `agent_code` varchar(50) DEFAULT NULL COMMENT '报价交易员代码',
  `flag_vip` bit(1) DEFAULT b'0' COMMENT '是否vip  0：不是 1:是',
  `flag_bad` bit(1) DEFAULT b'0' COMMENT '是否坏名字  0：不是 1:是',
  `flag_bargain` varchar(1) DEFAULT NULL COMMENT '是否可议价  0：不可 1:可议价  2：更可议价',
  `price_description` varchar(225) DEFAULT NULL COMMENT '报价评论',
  `goods_type` varchar(255) DEFAULT NULL COMMENT '产品类型',
  `goods_short_name` varchar(255) DEFAULT NULL COMMENT '产品简称',
  `goods_level` varchar(255) DEFAULT NULL COMMENT '产品主体评级',
  `goods_term` varchar(255) DEFAULT NULL COMMENT '产品期限',
  `goods_name` varchar(255) DEFAULT NULL COMMENT '产品名称',
  `option_type` varchar(255) DEFAULT NULL COMMENT '期权方式',
  `rate_type` varchar(255) DEFAULT NULL COMMENT '利率类型',
  `internally` varchar(255) DEFAULT NULL COMMENT '是否公开报价  1:公开  2：不公开',
  `flag_relation` varchar(1) DEFAULT NULL COMMENT '是否OCO  0：不是 1:是OCO 2:打包',
  `trader_id` varchar(32) DEFAULT NULL COMMENT '交易员ID',
  `bond_offer_status` varchar(5) DEFAULT NULL COMMENT '报价状态  1：有效  2：无效',
  `quote_type` varchar(5) NOT NULL DEFAULT '0' COMMENT '报价方式  1；收益率 2：净价 3：全价  4：利差',
  `return_point` decimal(10,4) DEFAULT NULL COMMENT '返点值',
  `net_price` decimal(10,4) DEFAULT NULL COMMENT '净价',
  `full_price` decimal(10,4) DEFAULT NULL COMMENT '全价',
  `yield` decimal(10,4) DEFAULT NULL COMMENT '收益率',
  `spread` decimal(10,4) DEFAULT NULL COMMENT '利差',
  `clear_speed` varchar(10) DEFAULT NULL COMMENT '清算速度',
  `delivery_date` datetime DEFAULT NULL COMMENT '交割时间',
  `exercise` varchar(10) DEFAULT NULL COMMENT '是否行权',
  `rebate` varchar(1) DEFAULT '0' COMMENT '是否返点 0：不是 1：是 ',
  `send_status` varchar(1) DEFAULT NULL COMMENT '发送状态',
  `calculate_status` int(1) DEFAULT '1' COMMENT '是否自动计算 0：不是 1：是 ',
  `bond_key` varchar(25) DEFAULT NULL COMMENT '产品key',
  `listed_market` varchar(3) DEFAULT NULL COMMENT '市场代码',
  `bond_key_listed_market` varchar(28) DEFAULT NULL COMMENT '产品key+市场代码',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_profit_mode`
--

DROP TABLE IF EXISTS `bond_profit_mode`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_profit_mode` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `name` varchar(64) DEFAULT NULL,
  `description` varchar(128) DEFAULT NULL,
  `code` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary table structure for view `bond_screen`
--

DROP TABLE IF EXISTS `bond_screen`;
/*!50001 DROP VIEW IF EXISTS `bond_screen`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `bond_screen` (
  `goods_id` tinyint NOT NULL,
  `short_name` tinyint NOT NULL,
  `goods_code` tinyint NOT NULL,
  `goods_term` tinyint NOT NULL,
  `goods_level` tinyint NOT NULL,
  `goods_type` tinyint NOT NULL,
  `profit_code` tinyint NOT NULL,
  `ofr_agent_code` tinyint NOT NULL,
  `ofr_price` tinyint NOT NULL,
  `ofr_price_description` tinyint NOT NULL,
  `ofr_volume` tinyint NOT NULL,
  `ofr_flag_vip` tinyint NOT NULL,
  `ofr_flag_bad` tinyint NOT NULL,
  `ofr_flag_bargain` tinyint NOT NULL,
  `bid_agent_code` tinyint NOT NULL,
  `bid_price` tinyint NOT NULL,
  `bid_price_description` tinyint NOT NULL,
  `bid_volume` tinyint NOT NULL,
  `bid_flag_vip` tinyint NOT NULL,
  `bid_flag_bad` tinyint NOT NULL,
  `bid_flag_bargain` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Temporary table structure for view `bond_small_view`
--

DROP TABLE IF EXISTS `bond_small_view`;
/*!50001 DROP VIEW IF EXISTS `bond_small_view`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `bond_small_view` (
  `bond_key_listed_market` tinyint NOT NULL,
  `delflag` tinyint NOT NULL,
  `create_date` tinyint NOT NULL,
  `Bond_Key` tinyint NOT NULL,
  `Listed_Market` tinyint NOT NULL,
  `Bond_ID` tinyint NOT NULL,
  `Bond_Subtype` tinyint NOT NULL,
  `Short_Name` tinyint NOT NULL,
  `Is_Cross_Mkt` tinyint NOT NULL,
  `Is_Mortgage` tinyint NOT NULL,
  `Pin_Yin` tinyint NOT NULL,
  `Pin_Yin_Full` tinyint NOT NULL,
  `Listed_Date` tinyint NOT NULL,
  `Delisted_Date` tinyint NOT NULL,
  `Mkt_Type` tinyint NOT NULL,
  `Is_Municipal` tinyint NOT NULL,
  `Issuer_Rating_Current_NPY` tinyint NOT NULL,
  `Rating_Current_NPY` tinyint NOT NULL,
  `Issuer_Rating_Current` tinyint NOT NULL,
  `Maturity_Term` tinyint NOT NULL,
  `Warranter` tinyint NOT NULL,
  `Coupon_Type` tinyint NOT NULL,
  `Rating_Augment` tinyint NOT NULL,
  `Rating_Date` tinyint NOT NULL,
  `Option_Type` tinyint NOT NULL,
  `Coupon_Rate_Current` tinyint NOT NULL,
  `Maturity_Date` tinyint NOT NULL,
  `Issuer_Code` tinyint NOT NULL,
  `Next_Coupon_Date` tinyint NOT NULL,
  `Redemption_No` tinyint NOT NULL,
  `Rating_Current` tinyint NOT NULL,
  `FRN_Index_ID` tinyint NOT NULL,
  `Fixing_MA_Days` tinyint NOT NULL,
  `Coupon_Rate_Spread` tinyint NOT NULL,
  `Option_Date` tinyint NOT NULL,
  `Issue_Amount` tinyint NOT NULL,
  `Underwriter_Code` tinyint NOT NULL,
  `Issuer_Rating_Institution_Code` tinyint NOT NULL,
  `Term_Unit` tinyint NOT NULL,
  `Rating_Institution_Code` tinyint NOT NULL,
  `Interest_Start_Date` tinyint NOT NULL,
  `Issue_Year` tinyint NOT NULL,
  `Ent_Cor` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `bond_temporary_rate`
--

DROP TABLE IF EXISTS `bond_temporary_rate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_temporary_rate` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `bond_key` varchar(25) DEFAULT NULL,
  `coupon_rate_tmp` varchar(32) DEFAULT NULL,
  `rate_tmp` varchar(32) DEFAULT NULL,
  `rebate_tmp` varchar(20) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `modify_time` datetime DEFAULT NULL,
  `status` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_type_selective`
--

DROP TABLE IF EXISTS `bond_type_selective`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_type_selective` (
  `Id` varchar(32) NOT NULL,
  `selective_code` varchar(10) DEFAULT NULL,
  `type_id` varchar(32) DEFAULT NULL,
  `status` varchar(1) DEFAULT '1',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `brokerage`
--

DROP TABLE IF EXISTS `brokerage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `brokerage` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `deal_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `value` decimal(10,5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `broker_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_brokerag_reference_account` (`broker_id`),
  KEY `fk_brokerag_reference_bank` (`bank_id`),
  KEY `fk_brokerag_reference_deal` (`deal_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`broker_id`) REFER `artogrid/account';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `candlestick`
--

DROP TABLE IF EXISTS `candlestick`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `candlestick` (
  `id` varchar(45) NOT NULL,
  `in_price` varchar(45) DEFAULT NULL,
  `out_price` varchar(45) DEFAULT NULL,
  `max_price` varchar(45) DEFAULT NULL,
  `min_price` varchar(45) DEFAULT NULL,
  `time` varchar(45) DEFAULT NULL,
  `time_style` varchar(45) DEFAULT NULL,
  `status` varchar(45) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `count` int(11) DEFAULT NULL,
  `goods_id` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `china_bond_deviated_value`
--

DROP TABLE IF EXISTS `china_bond_deviated_value`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `china_bond_deviated_value` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `company_id` varchar(32) DEFAULT NULL,
  `bond_type_id` varchar(32) DEFAULT NULL,
  `bond_type_name` varchar(32) DEFAULT NULL,
  `deviated_value` varchar(10) DEFAULT '20' COMMENT '默认20bp',
  `status` varchar(1) DEFAULT NULL,
  `modify_time` datetime DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `china_bond_valuation`
--

DROP TABLE IF EXISTS `china_bond_valuation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `china_bond_valuation` (
  `Id` varchar(32) NOT NULL DEFAULT '' COMMENT 'ID',
  `bond_id` varchar(32) NOT NULL DEFAULT '',
  `listed_market` varchar(3) NOT NULL DEFAULT '' COMMENT 'CIB：yinhangjian\nSSE：shanghai\nSZE：shenzhen',
  `valuation_yield` varchar(128) DEFAULT NULL,
  `valuation_net_price` varchar(128) DEFAULT NULL,
  `valuation_date` varchar(10) DEFAULT NULL,
  `status` char(1) DEFAULT '0' COMMENT '0：valid\n1：invalid',
  `modify_time` datetime DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  `Val_Modified_Duration` decimal(7,4) DEFAULT NULL,
  `Val_Convexity` decimal(7,4) DEFAULT NULL,
  `Val_Basis_Point_Value` decimal(7,4) DEFAULT NULL,
  PRIMARY KEY (`Id`),
  KEY `bond_id_listed_market` (`bond_id`,`listed_market`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `client_login_token`
--

DROP TABLE IF EXISTS `client_login_token`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_login_token` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `customer_id` varchar(32) DEFAULT NULL,
  `ipaddress` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `info` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `activation` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `version` varchar(45) DEFAULT NULL,
  `deviceId` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_client_customer` (`customer_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `client_token_log`
--

DROP TABLE IF EXISTS `client_token_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_token_log` (
  `id` varchar(255) NOT NULL,
  `token` varchar(255) DEFAULT NULL,
  `customer_id` varchar(255) DEFAULT NULL,
  `ipaddress` varchar(255) DEFAULT NULL,
  `info` varchar(255) DEFAULT NULL,
  `client_type` varchar(255) DEFAULT NULL,
  `login_time` timestamp NULL DEFAULT NULL,
  `loginout_time` timestamp NULL DEFAULT NULL,
  `loginout_type` varchar(255) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `cluster_schedule_register`
--

DROP TABLE IF EXISTS `cluster_schedule_register`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cluster_schedule_register` (
  `id` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `context` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `node_name` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `start_time` datetime DEFAULT NULL,
  `end_time` datetime DEFAULT NULL,
  `executed` int(8) DEFAULT NULL,
  `key_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `console_permission`
--

DROP TABLE IF EXISTS `console_permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `console_permission` (
  `id` varchar(32) NOT NULL,
  `name` varchar(256) NOT NULL,
  `family` varchar(256) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `constants`
--

DROP TABLE IF EXISTS `constants`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `constants` (
  `id` varchar(32) NOT NULL,
  `name` varchar(50) DEFAULT NULL,
  `value` varchar(200) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `constants_key` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='static variable; InnoDB free: 18432 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crawl_record`
--

DROP TABLE IF EXISTS `crawl_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `crawl_record` (
  `Id` varchar(200) NOT NULL DEFAULT '',
  `create_time` datetime DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL,
  `type` varchar(255) DEFAULT NULL,
  `hashcode` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crawl_record_value`
--

DROP TABLE IF EXISTS `crawl_record_value`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `crawl_record_value` (
  `Id` varchar(200) NOT NULL DEFAULT '',
  `record_id` varchar(255) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `customer` (
  `id` varchar(32) NOT NULL,
  `username` varchar(32) NOT NULL,
  `password` varchar(32) NOT NULL,
  `phoneNumber` varchar(32) DEFAULT NULL,
  `email` varchar(32) DEFAULT NULL,
  `address` varchar(32) DEFAULT NULL,
  `status` varchar(5) DEFAULT '1',
  `create_time` datetime DEFAULT NULL,
  `tel` varchar(32) DEFAULT NULL,
  `login_name` varchar(45) DEFAULT NULL,
  `flag` varchar(45) DEFAULT NULL,
  `default_group` varchar(45) DEFAULT NULL,
  `code` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='客户信息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daily_bond_rate`
--

DROP TABLE IF EXISTS `daily_bond_rate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daily_bond_rate` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `goods_type_id` varchar(32) DEFAULT NULL,
  `bond_type` varchar(32) DEFAULT NULL,
  `comment` text,
  `deal_info` text,
  `input_date` varchar(11) DEFAULT NULL,
  `company_id` int(4) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` char(1) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daily_bond_rate_deal`
--

DROP TABLE IF EXISTS `daily_bond_rate_deal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daily_bond_rate_deal` (
  `id` varchar(32) NOT NULL,
  `Daily_Bond_Rate_Id` varchar(32) DEFAULT NULL,
  `bond_deal_id` varchar(32) DEFAULT NULL,
  `Termination` varchar(32) DEFAULT 'ex.320D 1.2Y',
  `Bond_id` varchar(45) DEFAULT NULL,
  `Short_Name` varchar(45) DEFAULT NULL,
  `rating` varchar(8) DEFAULT NULL,
  `Price` varchar(45) DEFAULT NULL,
  `Volume` varchar(45) DEFAULT NULL,
  `input_date` varchar(11) DEFAULT NULL,
  `company_id` int(4) DEFAULT NULL,
  `bond_type` varchar(32) DEFAULT NULL,
  `Create_Time` datetime DEFAULT NULL,
  `Status` char(1) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daily_bond_rate_item`
--

DROP TABLE IF EXISTS `daily_bond_rate_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daily_bond_rate_item` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `daily_bond_rate_id` varchar(32) DEFAULT NULL,
  `rating` varchar(8) DEFAULT NULL,
  `termination` varchar(8) DEFAULT NULL,
  `item_type` varchar(3) DEFAULT NULL COMMENT 'TRT:今天 YRT.昨天 SPD.变动  GOV.中票->国债 SGV.中票->国债利差 FIN.中票->金融债 SFN.中票->金融债利差',
  `rate` varchar(20) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` char(1) DEFAULT NULL,
  `input_date` varchar(11) DEFAULT NULL,
  `company_id` int(4) DEFAULT NULL,
  `bond_type` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `deal`
--

DROP TABLE IF EXISTS `deal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `deal` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `type_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_date` datetime DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `volumn` int(8) DEFAULT NULL,
  `sold_bank_code` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `sold_bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `sold_offer_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `sold_account_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `sold_type` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `sold_confirm` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `sold_check_time` datetime DEFAULT NULL,
  `bid_bank_code` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bid_bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bid_offer_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bid_account_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bid_type` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bid_confirm` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bid_check_time` datetime DEFAULT NULL,
  `operator_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `department`
--

DROP TABLE IF EXISTS `department`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `department` (
  `Id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `code` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `description` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `is_public` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `discount`
--

DROP TABLE IF EXISTS `discount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `discount` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `rate` decimal(10,6) DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_discount_reference_bank` (`bank_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`bank_id`) REFER `artogrid/bank`(`id';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `discount_month`
--

DROP TABLE IF EXISTS `discount_month`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `discount_month` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `start_price` decimal(10,4) DEFAULT NULL,
  `end_price` decimal(10,4) DEFAULT NULL,
  `rate` decimal(10,4) DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_discount_month_ref_bank` (`bank_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`bank_id`) REFER `artogrid/bank`(`id';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `feedback`
--

DROP TABLE IF EXISTS `feedback`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `feedback` (
  `id` varchar(50) NOT NULL,
  `account_id` varchar(45) DEFAULT NULL,
  `ip` varchar(45) DEFAULT NULL,
  `message` varchar(45) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `funtion_group`
--

DROP TABLE IF EXISTS `funtion_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `funtion_group` (
  `idb_account_group_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `idb_function_id` varchar(32) NOT NULL DEFAULT '',
  KEY `fk_funtion_id` (`idb_function_id`),
  KEY `fk_group_id` (`idb_account_group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `goods`
--

DROP TABLE IF EXISTS `goods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goods` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `type_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `big_number` int(8) DEFAULT NULL,
  `goods_code` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_type_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `is_outright` int(8) DEFAULT NULL,
  `product_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `is_show` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `small_number` int(8) DEFAULT NULL,
  `pay_type_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_type` int(8) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_goods_reference_goods_ty` (`goods_type_id`),
  KEY `fk_goods_reference_pay_type` (`pay_type_id`),
  KEY `fk_goods_reference_product` (`product_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`goods_type_id`) REFER `artogrid/goo';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `goods_group`
--

DROP TABLE IF EXISTS `goods_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goods_group` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_goods_gr_reference_goods` (`goods_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`goods_id`) REFER `artogrid/goods`(`';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `goods_resource`
--

DROP TABLE IF EXISTS `goods_resource`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goods_resource` (
  `Id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) DEFAULT NULL,
  `constants_key` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  `goods_id` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `goods_split`
--

DROP TABLE IF EXISTS `goods_split`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goods_split` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `goods_group_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `rate` int(8) DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_goods_sp_reference_goods_gr` (`goods_group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`goods_group_id`) REFER `artogrid/go';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `goods_type`
--

DROP TABLE IF EXISTS `goods_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goods_type` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `info` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `code` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `holiday`
--

DROP TABLE IF EXISTS `holiday`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `holiday` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL,
  `date` int(11) DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `type` varchar(255) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=47 DEFAULT CHARSET=utf8;
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `holiday_info_20180125`
--

DROP TABLE IF EXISTS `holiday_info_20180125`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `holiday_info_20180125` (
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `hot_key`
--

DROP TABLE IF EXISTS `hot_key`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hot_key` (
  `Id` varchar(100) NOT NULL DEFAULT 'hot_key',
  `HotKey` varchar(50) NOT NULL,
  `FunctionName` varchar(100) NOT NULL,
  `Description` varchar(255) DEFAULT '',
  `Function` varchar(100) NOT NULL,
  `Params` varchar(255) DEFAULT NULL,
  `ParamSize` int(8) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_ftp`
--

DROP TABLE IF EXISTS `idb_ftp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_ftp` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `ftpIp` varchar(50) DEFAULT NULL,
  `agencyName` varchar(50) DEFAULT NULL,
  `userName` varchar(50) DEFAULT NULL,
  `passWord` varchar(50) DEFAULT NULL,
  `startTime` varchar(50) DEFAULT NULL,
  `endTime` varchar(50) DEFAULT NULL,
  `gapTime` int(11) DEFAULT NULL,
  `remarks` varchar(255) DEFAULT NULL,
  `createTime` date DEFAULT NULL,
  `userStatus` bit(1) DEFAULT NULL,
  `department` varchar(50) DEFAULT NULL,
  `ftpPort` int(11) DEFAULT NULL,
  `txtType` bit(1) DEFAULT NULL,
  `excelType` bit(1) DEFAULT NULL,
  `wordType` bit(1) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_ftprules`
--

DROP TABLE IF EXISTS `idb_ftprules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_ftprules` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `ftpId` varchar(50) DEFAULT NULL,
  `goodsTypeId` varchar(255) DEFAULT NULL,
  `goodsName` varchar(255) DEFAULT NULL,
  `bidFloat` int(11) DEFAULT NULL,
  `ofrFloat` int(11) DEFAULT NULL,
  `remarks` varchar(255) DEFAULT NULL,
  `createTime` date DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_function`
--

DROP TABLE IF EXISTS `idb_function`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_function` (
  `Id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) DEFAULT NULL,
  `code` varchar(255) DEFAULT NULL,
  `function_group` varchar(255) DEFAULT NULL,
  `client_type` varchar(255) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `group_levels` varchar(255) DEFAULT NULL,
  `dept` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`Id`),
  KEY `fk_dept` (`dept`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_sync_time`
--

DROP TABLE IF EXISTS `idb_sync_time`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_sync_time` (
  `id` varchar(32) NOT NULL,
  `last_sync_time` datetime DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
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
  `institutioncol` varchar(45) DEFAULT NULL,
  `Pin_Yin` varchar(1024) DEFAULT NULL,
  `Pin_Yin_Full` varchar(7168) DEFAULT NULL,
  `Institution_Subtype` varchar(3) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `delflag` (`delflag`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `job_task`
--

DROP TABLE IF EXISTS `job_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `job_task` (
  `id` varchar(32) NOT NULL DEFAULT '0',
  `task_code` varchar(255) DEFAULT NULL,
  `task_type` varchar(255) DEFAULT NULL,
  `task_impl_class` varchar(255) DEFAULT NULL,
  `task_express` varchar(50) DEFAULT NULL,
  `state_date` datetime DEFAULT NULL,
  `state` varchar(2) DEFAULT NULL COMMENT 'u 使用中 o已结束e报错',
  `parms` varchar(500) DEFAULT NULL,
  `remark` varchar(2000) DEFAULT NULL,
  `create_date` datetime DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `login_log`
--

DROP TABLE IF EXISTS `login_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `login_log` (
  `Id` varchar(255) NOT NULL DEFAULT '',
  `token` varchar(255) DEFAULT NULL,
  `account_id` varchar(255) DEFAULT NULL,
  `ipaddress` varchar(255) DEFAULT NULL,
  `info` varchar(255) DEFAULT NULL,
  `client_type` varchar(255) DEFAULT NULL,
  `login_time` timestamp NULL DEFAULT NULL,
  `loginout_time` timestamp NULL DEFAULT NULL,
  `loginout_type` varchar(255) DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `online_time` bigint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `login_token`
--

DROP TABLE IF EXISTS `login_token`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `login_token` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `account_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `ipaddress` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `info` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `activation` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_login_to_reference_account` (`account_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`account_id`) REFER `artogrid/accoun';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `macro_base_index_values`
--

DROP TABLE IF EXISTS `macro_base_index_values`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `macro_base_index_values` (
  `ID` bigint(20) NOT NULL,
  `GJK_Code` varchar(64) NOT NULL,
  `Index_Value` double DEFAULT NULL,
  `Index_Date` date NOT NULL,
  `Last_Modify_Datetime` datetime DEFAULT NULL,
  `Flag` tinyint(1) DEFAULT '0',
  `cvalue` double DEFAULT NULL,
  `Index_Date_Emend` date DEFAULT NULL,
  PRIMARY KEY (`GJK_Code`,`Index_Date`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `offer`
--

DROP TABLE IF EXISTS `offer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offer` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `type_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bank_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `volume` int(8) DEFAULT NULL,
  `oco_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `deal_status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `deal_time` datetime DEFAULT NULL,
  `operator_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `review_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bank_agent_id` varchar(32) DEFAULT NULL,
  `agent_code` varchar(50) DEFAULT NULL,
  `goods_code` varchar(50) DEFAULT NULL,
  `vol_style` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_offer_reference_account` (`operator_id`),
  KEY `fk_offer_reference_bank` (`bank_id`),
  KEY `fk_offer_reference_goods` (`goods_id`),
  KEY `fk_offer_reference_offer_oc` (`oco_id`),
  KEY `fk_offer_ref_account_review` (`review_id`),
  KEY `FK_Reference_44` (`bank_agent_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`operator_id`) REFER `artogrid/accou';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `offer_best`
--

DROP TABLE IF EXISTS `offer_best`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offer_best` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `type_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_code` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `is_best` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `offer_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_offer_be_reference_offer` (`offer_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`offer_id`) REFER `artogrid/offer`(`';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `offer_best_history`
--

DROP TABLE IF EXISTS `offer_best_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offer_best_history` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `offer_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `type_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_code` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `offer_collection`
--

DROP TABLE IF EXISTS `offer_collection`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offer_collection` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `offer_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `type_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bank_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` varchar(10) DEFAULT NULL,
  `deal_status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `deal_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `bank_agent_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `agent_code` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_code` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `input_time` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `account_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `offer_history`
--

DROP TABLE IF EXISTS `offer_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offer_history` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `offer_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `goods_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `type_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bank_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `bank_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `deal_status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `deal_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `bank_agent_id` varchar(32) DEFAULT NULL,
  `agent_code` varchar(50) DEFAULT NULL,
  `goods_code` varchar(255) DEFAULT 'OUTRIGHT',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `offer_leg`
--

DROP TABLE IF EXISTS `offer_leg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offer_leg` (
  `id` varchar(32) NOT NULL,
  `type_name` varchar(50) DEFAULT NULL,
  `term_name` varchar(50) DEFAULT NULL,
  `goods_id` varchar(32) DEFAULT NULL,
  `goods_code` varchar(50) DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `offer_oco`
--

DROP TABLE IF EXISTS `offer_oco`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offer_oco` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `description` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
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
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_issuer_info`
--

DROP TABLE IF EXISTS `p_issuer_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_issuer_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Institution_Code` varchar(7) DEFAULT NULL,
  `Issuer_Name` varchar(64) DEFAULT NULL,
  `Issuer_Type` varchar(16) DEFAULT NULL,
  `Org_Type` varchar(16) DEFAULT NULL,
  `SW_Sector` varchar(32) DEFAULT NULL,
  `SW_Subsector` varchar(32) DEFAULT NULL,
  `Province` varchar(16) DEFAULT NULL,
  `City` varchar(16) DEFAULT NULL,
  `Regd_capital` decimal(15,2) DEFAULT NULL,
  `Regd_capital_Currency` varchar(8) DEFAULT NULL,
  `Stockholder_Name` varchar(127) DEFAULT NULL,
  `Stockholder_Type` varchar(16) DEFAULT NULL,
  `Stockholding_Percentage` varchar(16) DEFAULT NULL,
  `Actual_Controller_Name` varchar(128) DEFAULT NULL,
  `Actual_Controller_Type` varchar(16) DEFAULT NULL,
  `Ultimate_Actual_Controller_Name` varchar(128) DEFAULT NULL,
  `Ultimate_Actual_Controller_Type` varchar(16) DEFAULT NULL,
  `CBRC_Financing_Platform` varchar(1) DEFAULT NULL,
  `Endowment` varchar(64) DEFAULT NULL,
  `Municipal_Business` varchar(1) DEFAULT NULL,
  `Business_Scope` varchar(1024) DEFAULT NULL,
  `SW_Sector_Code` varchar(16) DEFAULT NULL,
  `SW_Subsector_Code` varchar(16) DEFAULT NULL,
  `Province_Code` varchar(16) DEFAULT NULL,
  `Orgnization_Code` varchar(16) DEFAULT NULL,
  `Regd_Num` varchar(31) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Institution_Code` (`Institution_Code`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `p_issuer_info_20160329`
--

DROP TABLE IF EXISTS `p_issuer_info_20160329`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_issuer_info_20160329` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Institution_Code` varchar(7) DEFAULT NULL,
  `Issuer_Name` varchar(64) DEFAULT NULL,
  `Issuer_Type` varchar(16) DEFAULT NULL,
  `Org_Type` varchar(16) DEFAULT NULL,
  `SW_Sector` varchar(32) DEFAULT NULL,
  `SW_Subsector` varchar(32) DEFAULT NULL,
  `Province` varchar(16) DEFAULT NULL,
  `City` varchar(16) DEFAULT NULL,
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
  `Business_Scope` varchar(1024) DEFAULT NULL,
  `SW_Sector_Code` varchar(16) DEFAULT NULL,
  `SW_Subsector_Code` varchar(16) DEFAULT NULL,
  `Province_Code` varchar(16) DEFAULT NULL,
  `Orgnization_Code` varchar(16) DEFAULT NULL,
  `Regd_Num` varchar(31) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Institution_Code` (`Institution_Code`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pay_type`
--

DROP TABLE IF EXISTS `pay_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pay_type` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `pay_type` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `description` varchar(500) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `product`
--

DROP TABLE IF EXISTS `product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `product` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `type_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `termination_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_product_reference_product_` (`type_id`),
  KEY `fk_product_reference_terminat` (`termination_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`type_id`) REFER `artogrid/product_t';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `product_type`
--

DROP TABLE IF EXISTS `product_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `product_type` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `name` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `description` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `code` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `value` decimal(10,4) DEFAULT NULL,
  `DEVIANION` decimal(10,4) DEFAULT NULL,
  `is_basis` bit(1) DEFAULT b'0' COMMENT '0-yes 1-no',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `product_type_rank`
--

DROP TABLE IF EXISTS `product_type_rank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `product_type_rank` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `product_type_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `rank` int(8) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_product__reference_product_` (`product_type_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`product_type_id`) REFER `artogrid/p';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `push_message_data`
--

DROP TABLE IF EXISTS `push_message_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `push_message_data` (
  `id` varchar(32) NOT NULL,
  `type` varchar(45) DEFAULT NULL COMMENT 'BOND IRS',
  `version` bigint(20) DEFAULT NULL,
  `length` int(11) DEFAULT NULL,
  `value` mediumblob,
  `server_info` varchar(255) DEFAULT NULL,
  `server_ipaddress` varchar(45) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='push message';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `push_message_data_qb`
--

DROP TABLE IF EXISTS `push_message_data_qb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `push_message_data_qb` (
  `id` varchar(32) NOT NULL,
  `type` varchar(45) DEFAULT NULL COMMENT 'BOND IRS',
  `version` bigint(20) DEFAULT NULL,
  `length` int(11) DEFAULT NULL,
  `value` mediumblob,
  `server_info` varchar(255) DEFAULT NULL,
  `server_ipaddress` varchar(45) DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `version` (`version`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='push message qb';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rate_info`
--

DROP TABLE IF EXISTS `rate_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rate_info` (
  `ID` varchar(32) NOT NULL,
  `delflag` char(1) DEFAULT '0',
  `create_date` datetime DEFAULT NULL,
  `modify_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `username` varchar(32) DEFAULT NULL,
  `inputer` varchar(32) DEFAULT NULL,
  `checker` varchar(32) DEFAULT NULL,
  `sts` char(1) DEFAULT '0',
  `gjk_code` varchar(64) DEFAULT NULL,
  `source_code` varchar(64) DEFAULT NULL,
  `series_name_local` varchar(128) DEFAULT NULL,
  `reported_unit_local` varchar(8) DEFAULT NULL,
  `unit_type_local` varchar(8) DEFAULT NULL,
  `series_freq_local` varchar(8) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `resource`
--

DROP TABLE IF EXISTS `resource`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `resource` (
  `Id` varchar(100) NOT NULL DEFAULT '',
  `contents` text,
  `type` varchar(255) DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `app_id` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `screen_account`
--

DROP TABLE IF EXISTS `screen_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `screen_account` (
  `id` varchar(32) NOT NULL,
  `account_id` varchar(32) DEFAULT NULL,
  `screen_id` varchar(32) DEFAULT NULL,
  `screen_seq` int(11) DEFAULT '1',
  `login_type` varchar(45) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `screen_config`
--

DROP TABLE IF EXISTS `screen_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `screen_config` (
  `id` varchar(32) NOT NULL,
  `name` varchar(45) DEFAULT NULL,
  `value` text,
  `account_id` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `screen_style`
--

DROP TABLE IF EXISTS `screen_style`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `screen_style` (
  `id` varchar(32) NOT NULL,
  `name` varchar(45) DEFAULT NULL,
  `value` text,
  `account_id` varchar(32) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `screen_style_account`
--

DROP TABLE IF EXISTS `screen_style_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `screen_style_account` (
  `id` varchar(32) NOT NULL,
  `account_id` varchar(45) DEFAULT NULL,
  `style_id` varchar(32) DEFAULT NULL,
  `screen_seq` int(11) DEFAULT NULL,
  `login_type` varchar(45) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `selective_info`
--

DROP TABLE IF EXISTS `selective_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `selective_info` (
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `server_list`
--

DROP TABLE IF EXISTS `server_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_list` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(32) DEFAULT NULL,
  `prot` varchar(32) DEFAULT NULL,
  `remark1` varchar(32) DEFAULT NULL,
  `remark2` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sys_company`
--

DROP TABLE IF EXISTS `sys_company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sys_company` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `name` varchar(128) DEFAULT NULL,
  `status` char(1) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_api_access_right`
--

DROP TABLE IF EXISTS `system_api_access_right`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_api_access_right` (
  `id` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `requestor_id` varchar(126) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `minor_version` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `response_type` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `api_group` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `api_class` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `api_entity` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `api_method` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `allow` varchar(1) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_api_config`
--

DROP TABLE IF EXISTS `system_api_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_api_config` (
  `id` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_approved_url`
--

DROP TABLE IF EXISTS `system_approved_url`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_approved_url` (
  `id` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `url` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_config`
--

DROP TABLE IF EXISTS `system_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_config` (
  `id` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `name` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `value` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `description` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_requestor_id`
--

DROP TABLE IF EXISTS `system_requestor_id`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_requestor_id` (
  `id` varchar(36) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `requestor_id` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `created_date` datetime DEFAULT NULL,
  `modified_date` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `task_log`
--

DROP TABLE IF EXISTS `task_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task_log` (
  `id` varchar(32) NOT NULL DEFAULT '0',
  `task_id` varchar(32) DEFAULT NULL,
  `state` varchar(2) DEFAULT NULL COMMENT 'o结束，r运行中，e异常结束',
  `start_date` datetime DEFAULT NULL,
  `finish_date` datetime DEFAULT NULL,
  `remarks` varchar(2000) DEFAULT NULL,
  `content` varchar(5000) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `term_date`
--

DROP TABLE IF EXISTS `term_date`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `term_date` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `termination_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `month` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `term_date` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `now_date` datetime DEFAULT NULL,
  `term_date1` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_term_dat_reference_terminat` (`termination_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB; (`termination_id`) REFER `artogrid/te';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `termination`
--

DROP TABLE IF EXISTS `termination`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `termination` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `name` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `code` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `month` decimal(22,0) DEFAULT NULL,
  `description` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='InnoDB free: 21504 kB';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `termination_rank`
--

DROP TABLE IF EXISTS `termination_rank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `termination_rank` (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `termination_id` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `rank` int(8) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `status` varchar(5) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `termination_id` (`termination_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `token_log`
--

DROP TABLE IF EXISTS `token_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `token_log` (
  `Id` varchar(255) NOT NULL DEFAULT '',
  `token` varchar(255) DEFAULT NULL,
  `account_id` varchar(255) DEFAULT NULL,
  `ipaddress` varchar(255) DEFAULT NULL,
  `info` varchar(255) DEFAULT NULL,
  `client_type` varchar(255) DEFAULT NULL,
  `login_time` timestamp NULL DEFAULT NULL,
  `loginout_time` timestamp NULL DEFAULT NULL,
  `loginout_type` varchar(255) DEFAULT NULL,
  `create_time` timestamp NULL DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Final view structure for view `bond_group_relation_view`
--

/*!50001 DROP TABLE IF EXISTS `bond_group_relation_view`*/;
/*!50001 DROP VIEW IF EXISTS `bond_group_relation_view`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`artogrid`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `bond_group_relation_view` AS select `r`.`group_id` AS `group_id`,`r`.`goods_id` AS `goods_id`,`r`.`id` AS `id`,`r`.`account_type` AS `account_type`,`r`.`create_time` AS `create_time`,`r`.`ofr_price_range_begin` AS `ofr_price_range_begin`,`r`.`ofr_price_range_end` AS `ofr_price_range_end`,`r`.`bid_price_range_begin` AS `bid_price_range_begin`,`r`.`bid_price_range_end` AS `bid_price_range_end`,`r`.`flag_allow` AS `flag_allow`,`gg`.`ua_id` AS `ua_id`,`acc`.`account_id` AS `account_id`,`r`.`bond_key` AS `bond_key`,`r`.`listed_market` AS `listed_market`,`r`.`bond_key_listed_market` AS `bond_key_listed_market` from (((`bond_group_relation` `r` left join `bond_goods_group` `gg` on((`gg`.`id` = `r`.`group_id`))) left join `bond_bank_agent` `a` on((`a`.`customer_id` = `gg`.`ua_id`))) left join `bond_bank_account` `acc` on((`acc`.`bank_agent_id` = `a`.`id`))) where ((`gg`.`status` = 1) and (`a`.`status` = 1) and (`acc`.`status` = 1)) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `bond_screen`
--

/*!50001 DROP TABLE IF EXISTS `bond_screen`*/;
/*!50001 DROP VIEW IF EXISTS `bond_screen`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=TEMPTABLE */
/*!50013 DEFINER=`artogrid`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `bond_screen` AS select `g`.`id` AS `goods_id`,`g`.`short_name` AS `short_name`,`g`.`goods_code` AS `goods_code`,`g`.`goods_term` AS `goods_term`,`g`.`goods_level` AS `goods_level`,`g`.`goods_type` AS `goods_type`,`g`.`profit_code` AS `profit_code`,(select `o`.`agent_code` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_agent_code`,(select `o`.`price` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_price`,(select `o`.`price_description` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_price_description`,(select `o`.`volume` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_volume`,(select `o`.`flag_vip` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_flag_vip`,(select `o`.`flag_bad` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_flag_bad`,(select `o`.`flag_bargain` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_flag_bargain`,(select `o`.`agent_code` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_agent_code`,(select `o`.`price` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_price`,(select `o`.`price_description` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_price_description`,(select `o`.`volume` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_volume`,(select `o`.`flag_vip` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_flag_vip`,(select `o`.`flag_bad` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_flag_bad`,(select `o`.`flag_bargain` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_flag_bargain` from `bond_goods` `g` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `bond_small_view`
--

/*!50001 DROP TABLE IF EXISTS `bond_small_view`*/;
/*!50001 DROP VIEW IF EXISTS `bond_small_view`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY INVOKER */
/*!50001 VIEW `bond_small_view` AS select concat(`l`.`Bond_Key`,`l`.`Listed_Market`) AS `bond_key_listed_market`,`l`.`delflag` AS `delflag`,`l`.`create_date` AS `create_date`,`l`.`Bond_Key` AS `Bond_Key`,`l`.`Listed_Market` AS `Listed_Market`,`l`.`Bond_ID` AS `Bond_ID`,`b`.`Bond_Subtype` AS `Bond_Subtype`,`l`.`Short_Name` AS `Short_Name`,`l`.`Is_Cross_Mkt` AS `Is_Cross_Mkt`,`l`.`Is_Mortgage` AS `Is_Mortgage`,`l`.`pinyin` AS `Pin_Yin`,`l`.`pinyin_full` AS `Pin_Yin_Full`,`l`.`Listed_Date` AS `Listed_Date`,`l`.`Delisted_Date` AS `Delisted_Date`,`l`.`Mkt_Type` AS `Mkt_Type`,`b`.`Is_Municipal` AS `Is_Municipal`,`b`.`Issuer_Rating_Current_NPY` AS `Issuer_Rating_Current_NPY`,`b`.`Rating_Current_NPY` AS `Rating_Current_NPY`,`b`.`Issuer_Rating_Current` AS `Issuer_Rating_Current`,`b`.`Maturity_Term` AS `Maturity_Term`,`b`.`Warranter` AS `Warranter`,`b`.`Coupon_Type` AS `Coupon_Type`,`b`.`Rating_Augment` AS `Rating_Augment`,`b`.`Rating_Date` AS `Rating_Date`,`b`.`Option_Type` AS `Option_Type`,`b`.`Coupon_Rate_Current` AS `Coupon_Rate_Current`,`b`.`Maturity_Date` AS `Maturity_Date`,`b`.`Issuer_Code` AS `Issuer_Code`,`b`.`Next_Coupon_Date` AS `Next_Coupon_Date`,`b`.`Redemption_No` AS `Redemption_No`,`b`.`Rating_Current` AS `Rating_Current`,`b`.`FRN_Index_ID` AS `FRN_Index_ID`,`b`.`Fixing_MA_Days` AS `Fixing_MA_Days`,`b`.`Coupon_Rate_Spread` AS `Coupon_Rate_Spread`,`b`.`Option_Date` AS `Option_Date`,`b`.`Issue_Amount` AS `Issue_Amount`,`b`.`Underwriter_Code` AS `Underwriter_Code`,`b`.`Issuer_Rating_Institution_Code` AS `Issuer_Rating_Institution_Code`,`b`.`Term_Unit` AS `Term_Unit`,`b`.`Rating_Institution_Code` AS `Rating_Institution_Code`,`b`.`Interest_Start_Date` AS `Interest_Start_Date`,`b`.`Issue_Year` AS `Issue_Year`,`b`.`Ent_Cor` AS `Ent_Cor` from (`bond_list_info` `l` left join `bond` `b` on((`l`.`Bond_Key` = `b`.`Bond_Key`))) where ((`l`.`delflag` = 0) and (`b`.`delflag` = 0)) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:23:57
