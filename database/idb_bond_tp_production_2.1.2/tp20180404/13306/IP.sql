-- MySQL dump 10.13  Distrib 5.6.22, for linux-glibc2.5 (x86_64)
--
-- Host: 192.168.162.110    Database: IP
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
-- Table structure for table `BondQuote`
--

DROP TABLE IF EXISTS `BondQuote`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `BondQuote` (
  `bondQuoteUuid` bigint(20) NOT NULL AUTO_INCREMENT,
  `bondCode` varchar(255) DEFAULT NULL,
  `shortName` varchar(255) DEFAULT NULL,
  `fromName` varchar(255) DEFAULT NULL,
  `fromQQNumber` varchar(255) DEFAULT NULL,
  `toName` varchar(255) DEFAULT NULL,
  `toQQNumber` varchar(255) DEFAULT NULL,
  `bondQuote` blob,
  `msgText` varchar(255) DEFAULT NULL,
  `state` tinyint(4) DEFAULT NULL,
  `msgType` int(11) DEFAULT NULL,
  `lastOperationTime` varchar(255) DEFAULT NULL,
  `originalProbobility` int(11) DEFAULT NULL,
  `nowProbobility` int(11) DEFAULT NULL,
  `time` bigint(20) DEFAULT NULL,
  `accountId` varchar(255) DEFAULT NULL,
  `accountCompanyId` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`bondQuoteUuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `BondQuoteMessage`
--

DROP TABLE IF EXISTS `BondQuoteMessage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `BondQuoteMessage` (
  `msgId` bigint(20) NOT NULL AUTO_INCREMENT,
  `fromName` varchar(255) DEFAULT NULL,
  `fromQQNumber` varchar(255) DEFAULT NULL,
  `toName` varchar(255) DEFAULT NULL,
  `toQQNumber` varchar(255) DEFAULT NULL,
  `quotes` blob,
  `msgText` text,
  `msgType` int(11) DEFAULT NULL,
  `lastOperationTime` varchar(255) DEFAULT NULL,
  `time` bigint(20) DEFAULT NULL,
  `loginQQ` varchar(255) DEFAULT NULL,
  `groupName` varchar(255) DEFAULT NULL,
  `groupCode` varchar(255) DEFAULT NULL,
  `distinctMsgId` bigint(20) DEFAULT NULL,
  `isDeal` tinyint(4) DEFAULT NULL,
  `trader` varchar(255) DEFAULT NULL,
  `company` varchar(255) DEFAULT NULL,
  `broker` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`msgId`)
) ENGINE=InnoDB AUTO_INCREMENT=142217 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bind_qq_company`
--

DROP TABLE IF EXISTS `bind_qq_company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bind_qq_company` (
  `loginQQ` varchar(255) NOT NULL DEFAULT '',
  `companyId` varchar(255) DEFAULT NULL,
  `companyName` varchar(255) DEFAULT NULL,
  `lastOperationTime` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`loginQQ`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `condition_bond_quote_item`
--

DROP TABLE IF EXISTS `condition_bond_quote_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `condition_bond_quote_item` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `direction` varchar(255) DEFAULT NULL,
  `termStart` varchar(255) DEFAULT NULL,
  `termEnd` varchar(255) DEFAULT NULL,
  `ratingStart` varchar(255) DEFAULT NULL,
  `ratingEnd` varchar(255) DEFAULT NULL,
  `volumeStart` varchar(255) DEFAULT NULL,
  `volumeEnd` varchar(255) DEFAULT NULL,
  `bondType` varchar(255) DEFAULT NULL,
  `text` varchar(255) DEFAULT NULL,
  `lineNo` int(11) DEFAULT NULL,
  `probobility` int(11) DEFAULT NULL,
  `parent_id` bigint(20) DEFAULT NULL,
  `createTime` bigint(20) DEFAULT NULL,
  `msgType` int(11) DEFAULT NULL,
  `loginQQ` varchar(255) DEFAULT NULL,
  `groupName` varchar(255) DEFAULT NULL,
  `groupCode` varchar(255) DEFAULT NULL,
  `distinctMsgId` bigint(20) DEFAULT NULL,
  `fromQQNumber` varchar(255) DEFAULT NULL,
  `distinctChildMsgId` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `disable_user_group`
--

DROP TABLE IF EXISTS `disable_user_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `disable_user_group` (
  `qq` varchar(255) NOT NULL DEFAULT '',
  `role` int(11) DEFAULT NULL,
  `createTime` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`qq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `friend_list_info`
--

DROP TABLE IF EXISTS `friend_list_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friend_list_info` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `loginQQ` varchar(255) DEFAULT NULL,
  `userQQ` varchar(255) DEFAULT NULL,
  `showName` varchar(255) DEFAULT NULL,
  `lastOperationTime` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_login_and_user_qq` (`loginQQ`,`userQQ`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `friend_list_version`
--

DROP TABLE IF EXISTS `friend_list_version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friend_list_version` (
  `uin` varchar(255) NOT NULL DEFAULT '',
  `friendList` blob,
  `time` bigint(20) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`uin`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `friendlistversion`
--

DROP TABLE IF EXISTS `friendlistversion`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friendlistversion` (
  `uin` varchar(255) NOT NULL DEFAULT '',
  `friendList` blob,
  `time` bigint(20) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`uin`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `hybroker`
--

DROP TABLE IF EXISTS `hybroker`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hybroker` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `brokerId` varchar(255) DEFAULT NULL,
  `brokerName` varchar(255) DEFAULT NULL,
  `borkerQQ` varchar(255) DEFAULT NULL,
  `account` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `hybroker_account_idx` (`account`)
) ENGINE=InnoDB AUTO_INCREMENT=1515 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `localmessageinfo`
--

DROP TABLE IF EXISTS `localmessageinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `localmessageinfo` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `fromUin` varchar(255) DEFAULT NULL,
  `fromName` varchar(255) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `time` bigint(20) DEFAULT NULL,
  `msg` blob,
  `seq` bigint(20) DEFAULT NULL,
  `loginQQ` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=142259 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `push_settings`
--

DROP TABLE IF EXISTS `push_settings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `push_settings` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `value` int(11) DEFAULT NULL,
  `accountId` varchar(255) DEFAULT NULL,
  `accountCompanyId` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `standard_bond_quote_item`
--

DROP TABLE IF EXISTS `standard_bond_quote_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `standard_bond_quote_item` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `direction` varchar(255) DEFAULT NULL,
  `bondKey` varchar(255) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `volumn` varchar(255) DEFAULT NULL,
  `price` varchar(255) DEFAULT NULL,
  `priceTag` varchar(255) DEFAULT NULL,
  `text` varchar(255) DEFAULT NULL,
  `lineNo` int(11) DEFAULT NULL,
  `probobility` int(11) DEFAULT NULL,
  `parent_id` bigint(20) DEFAULT NULL,
  `createTime` bigint(20) DEFAULT NULL,
  `msgType` int(11) DEFAULT NULL,
  `loginQQ` varchar(255) DEFAULT NULL,
  `groupName` varchar(255) DEFAULT NULL,
  `groupCode` varchar(255) DEFAULT NULL,
  `distinctMsgId` bigint(20) DEFAULT NULL,
  `fromQQNumber` varchar(255) DEFAULT NULL,
  `distinctChildMsgId` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `standard_bond_quote_item_bondKey_idx` (`bondKey`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:27:36
