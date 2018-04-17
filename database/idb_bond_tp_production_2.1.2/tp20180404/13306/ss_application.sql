-- MySQL dump 10.13  Distrib 5.6.22, for linux-glibc2.5 (x86_64)
--
-- Host: 192.168.162.110    Database: ss_application
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
  `Bid_Outcome` varchar(16) DEFAULT NULL,
  `Flag` varchar(3) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  `Issue_Type` varchar(3) DEFAULT NULL,
  `Auction_Type` varchar(3) DEFAULT NULL,
  `Auction_Time_Start` int(6) DEFAULT NULL,
  `Auction_Time_End` int(6) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_dict`
--

DROP TABLE IF EXISTS `bond_dict`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_dict` (
  `Group_Name` varchar(30) NOT NULL DEFAULT '',
  `Selective_Code` varchar(31) NOT NULL,
  `Selective_Content` varchar(500) DEFAULT NULL,
  `SortCode` varchar(3) DEFAULT NULL,
  `Flag` varchar(3) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  PRIMARY KEY (`Group_Name`,`Selective_Code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_list`
--

DROP TABLE IF EXISTS `bond_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_list` (
  `Bond_Key_Listed_Market` varchar(35) NOT NULL,
  `Bond_Key` varchar(32) NOT NULL,
  `Listed_Market` varchar(3) DEFAULT NULL,
  `Bond_Subtype` varchar(3) DEFAULT NULL,
  `External_Type` varchar(3) DEFAULT NULL,
  `Bond_ID` varchar(32) DEFAULT NULL,
  `Short_Name` varchar(128) DEFAULT NULL,
  `Maturity_Date` varchar(10) DEFAULT NULL,
  `Option_Date` varchar(10) DEFAULT NULL,
  `Issuer_Rating` varchar(32) DEFAULT NULL,
  `Bond_Rating` varchar(32) DEFAULT NULL,
  `Guarantee` int(6) DEFAULT NULL,
  `Rate_Type` varchar(32) DEFAULT NULL,
  `Term_To_Maturity` varchar(32) DEFAULT NULL,
  `Listed_Date` varchar(10) DEFAULT NULL,
  `Market_Type` varchar(32) DEFAULT NULL,
  `Interest_Start_Date` varchar(32) DEFAULT NULL,
  `Is_Mortgage` char(1) DEFAULT NULL,
  `Is_Cross_Market` char(1) DEFAULT NULL,
  `Institution_Subtype` varchar(3) DEFAULT NULL,
  `Yield_Curve_Type` varchar(32) DEFAULT NULL,
  `Is_Municipal` varchar(1) DEFAULT NULL,
  `Issuer_Full_Name` varchar(80) DEFAULT NULL,
  `Bond_Full_Name` varchar(96) DEFAULT NULL,
  `Maturity_Term` varchar(9) DEFAULT NULL,
  `Planned_Issure_Amount` decimal(16,4) DEFAULT NULL,
  `Payment_Date` varchar(8) DEFAULT NULL,
  `Option_Type` varchar(3) DEFAULT NULL,
  `Issuer_Type` varchar(16) DEFAULT NULL,
  `Coupon_Rate_Current` decimal(7,4) DEFAULT NULL,
  `Auction_Date_Start` varchar(8) DEFAULT NULL,
  `Auction_Date_End` varchar(8) DEFAULT NULL,
  `Announce_Date` varchar(8) DEFAULT NULL,
  `Issue_Start_Date` varchar(8) DEFAULT NULL,
  `Issue_End_Date` varchar(8) DEFAULT NULL,
  `Registered_Amount` decimal(16,4) DEFAULT NULL,
  `Issuer_Code` varchar(7) DEFAULT NULL,
  `Issue_Period` varchar(16) DEFAULT NULL,
  `Underwriters` varchar(500) DEFAULT NULL,
  `MainUnderwriters` varchar(256) DEFAULT NULL,
  `JointUnderwriters` varchar(100) DEFAULT NULL,
  `FRN_Index_ID` varchar(16) DEFAULT NULL,
  `Fixing_MA_Days` int(4) DEFAULT NULL,
  `Coupon_Type` varchar(3) DEFAULT NULL,
  `Ent_Cor` varchar(8) DEFAULT NULL,
  `First_Issue_Amount` decimal(16,4) DEFAULT NULL,
  `Issue_Rate` varchar(16) DEFAULT NULL,
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  `Issue_Type` varchar(3) DEFAULT NULL,
  `Auction_Type` varchar(3) DEFAULT NULL,
  `Auction_Time_Start` int(6) DEFAULT NULL,
  `Auction_Time_End` int(6) DEFAULT NULL,
  PRIMARY KEY (`Bond_Key_Listed_Market`),
  KEY `Bond_Key` (`Bond_Key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `holiday`
--

DROP TABLE IF EXISTS `holiday`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `holiday` (
  `Country` varchar(3) NOT NULL,
  `Market` varchar(16) NOT NULL,
  `Holiday` int(8) NOT NULL,
  `Status` varchar(1) DEFAULT NULL,
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  PRIMARY KEY (`Country`,`Market`,`Holiday`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `institution`
--

DROP TABLE IF EXISTS `institution`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `institution` (
  `id` varchar(32) CHARACTER SET utf8 NOT NULL COMMENT '机构id',
  `full_name_cn` varchar(255) CHARACTER SET utf8 NOT NULL COMMENT '中文全称',
  `full_name_en` varchar(255) CHARACTER SET utf8 DEFAULT NULL COMMENT '英文全称',
  `short_name_cn` varchar(255) CHARACTER SET utf8 DEFAULT NULL COMMENT '中文简称',
  `short_name_en` varchar(255) CHARACTER SET utf8 DEFAULT NULL COMMENT '英文简称',
  `address` varchar(512) DEFAULT NULL,
  `hengtai_short_name_cn` varchar(255) CHARACTER SET utf8 DEFAULT NULL COMMENT '恒泰机构简称',
  `hengtai_full_name_cn` varchar(255) CHARACTER SET utf8 DEFAULT NULL COMMENT '恒泰机构全称',
  `Flag` varchar(10) CHARACTER SET utf8 DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`id`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='机构(hy_institution)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `macro_index`
--

DROP TABLE IF EXISTS `macro_index`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `macro_index` (
  `Code` varchar(64) NOT NULL,
  `Value` double DEFAULT NULL,
  `Date` int(8) NOT NULL,
  `EndDate` int(8) DEFAULT NULL,
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  PRIMARY KEY (`Code`,`Date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `qb_issuer_info`
--

DROP TABLE IF EXISTS `qb_issuer_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qb_issuer_info` (
  `Institution_Code` varchar(7) NOT NULL DEFAULT '',
  `Full_Name_C` varchar(80) DEFAULT NULL,
  `Short_Name_C` varchar(80) DEFAULT NULL,
  `Full_Name_E` varchar(256) DEFAULT NULL,
  `Short_Name_E` varchar(80) DEFAULT NULL,
  `SW_Sector_Code` varchar(16) DEFAULT NULL,
  `SW_Subsector_Code` varchar(16) DEFAULT NULL,
  `Province_Code` varchar(16) DEFAULT NULL,
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  PRIMARY KEY (`Institution_Code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `qb_province_info`
--

DROP TABLE IF EXISTS `qb_province_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qb_province_info` (
  `Area_Code` varchar(16) NOT NULL DEFAULT '',
  `Area_Name` varchar(16) DEFAULT NULL,
  `Area_English_Name` varchar(32) DEFAULT NULL,
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  PRIMARY KEY (`Area_Code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `qb_sw_sector_info`
--

DROP TABLE IF EXISTS `qb_sw_sector_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qb_sw_sector_info` (
  `SW_Internal_Code` varchar(16) NOT NULL DEFAULT '',
  `SW_Level` varchar(1) DEFAULT NULL,
  `SW_Name` varchar(16) DEFAULT NULL,
  `SW_English_Name` varchar(64) DEFAULT NULL,
  `SW_Upper_Grade_Code` varchar(16) DEFAULT NULL,
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  PRIMARY KEY (`SW_Internal_Code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `trade_account`
--

DROP TABLE IF EXISTS `trade_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trade_account` (
  `id` varchar(32) NOT NULL,
  `name_en` varchar(63) DEFAULT NULL COMMENT '显示名',
  `name_cn` varchar(63) DEFAULT NULL COMMENT '英文名',
  `tel` varchar(63) DEFAULT NULL COMMENT '座机',
  `mobile` varchar(63) DEFAULT NULL COMMENT '手机',
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  `acuid` char(128) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`id`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='系统账号';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `update_list`
--

DROP TABLE IF EXISTS `update_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `update_list` (
  `UpdateID` int(12) NOT NULL AUTO_INCREMENT COMMENT '升级序列号',
  `Category` varchar(32) NOT NULL COMMENT '分类，标记是哪一种升级包',
  `PatchName` varchar(32) NOT NULL COMMENT '升级包名称',
  `FileName` varchar(256) NOT NULL COMMENT '升级包的文件名',
  `OrigSize` int(10) NOT NULL COMMENT '压缩之前的文件大小',
  `MD5` varchar(64) NOT NULL COMMENT '升级包的MD5码',
  `UpdateTime` datetime NOT NULL COMMENT '最后更新时间',
  `Sequence` int(11) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL DEFAULT '00000000000000000000000000000000',
  PRIMARY KEY (`UpdateID`)
) ENGINE=InnoDB AUTO_INCREMENT=4256 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `update_list_mass`
--

DROP TABLE IF EXISTS `update_list_mass`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `update_list_mass` (
  `UpdateID` int(12) NOT NULL AUTO_INCREMENT COMMENT '升级序列号',
  `Category` varchar(32) NOT NULL COMMENT '分类，标记是哪一种升级包',
  `PatchName` varchar(32) NOT NULL COMMENT '升级包名称',
  `Period` int(12) NOT NULL,
  `FileName` varchar(256) NOT NULL COMMENT '升级包的文件名',
  `OrigSize` int(10) NOT NULL COMMENT '压缩之前的文件大小',
  `MD5` varchar(64) NOT NULL COMMENT '升级包的MD5码',
  `UpdateTime` datetime NOT NULL COMMENT '最后更新时间',
  `Sequence` int(11) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL DEFAULT '00000000000000000000000000000000',
  PRIMARY KEY (`UpdateID`,`Period`)
) ENGINE=InnoDB AUTO_INCREMENT=41829 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_account`
--

DROP TABLE IF EXISTS `user_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_account` (
  `id` varchar(32) NOT NULL,
  `institution_id` int(20) unsigned DEFAULT NULL COMMENT '所属机构',
  `name_cn` varchar(63) NOT NULL COMMENT '姓名',
  `mobile` varchar(30) DEFAULT NULL COMMENT '手机',
  `email` varchar(128) DEFAULT NULL COMMENT '邮箱地址',
  `tel` varchar(30) DEFAULT NULL COMMENT '固话',
  `qq` varchar(30) DEFAULT NULL COMMENT 'QQ',
  `fax` varchar(30) CHARACTER SET utf8 DEFAULT NULL COMMENT '传真',
  `address` varchar(511) DEFAULT NULL COMMENT '通讯地址',
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL COMMENT '该记录属于哪个平台对应的机构所有',
  PRIMARY KEY (`id`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='联系人';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_trade_relation`
--

DROP TABLE IF EXISTS `user_trade_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_trade_relation` (
  `id` varchar(32) NOT NULL,
  `user_id` varchar(32) NOT NULL COMMENT 'Parent',
  `trade_id` varchar(32) DEFAULT NULL COMMENT '客户经理',
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`id`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='联系人的客户经理';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_trade_relation_v2`
--

DROP TABLE IF EXISTS `user_trade_relation_v2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_trade_relation_v2` (
  `user_id` varchar(32) CHARACTER SET utf8 NOT NULL COMMENT 'Parent',
  `trade_id` varchar(64) CHARACTER SET utf8 DEFAULT NULL COMMENT '客户经理',
  `Flag` varchar(10) CHARACTER SET utf8 DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`user_id`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='联系人的客户经理';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:29:25
