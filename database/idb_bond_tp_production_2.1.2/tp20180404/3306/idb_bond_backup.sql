-- MySQL dump 10.13  Distrib 5.5.49, for Linux (x86_64)
--
-- Host: localhost    Database: idb_bond_backup
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
  `company_id` varchar(32) DEFAULT '1',
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
-- Table structure for table `bond_offer_log`
--

DROP TABLE IF EXISTS `bond_offer_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_offer_log` (
  `id` varchar(32) NOT NULL,
  `action` varchar(255) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL COMMENT 'bondofferlog的创建时间',
  `offer_id` varchar(32) DEFAULT NULL,
  `oper_user_id` varchar(45) DEFAULT NULL,
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
  `offer_create_time` datetime DEFAULT NULL,
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
  `bond_offer_status` varchar(5) DEFAULT NULL,
  `quote_type` varchar(5) NOT NULL DEFAULT '0',
  `return_point` decimal(10,4) DEFAULT NULL,
  `net_price` decimal(10,4) DEFAULT NULL,
  `full_price` decimal(10,4) DEFAULT NULL,
  `yield` decimal(10,4) DEFAULT NULL,
  `spread` decimal(10,4) DEFAULT NULL,
  `clear_speed` varchar(10) DEFAULT NULL,
  `delivery_date` datetime DEFAULT NULL,
  `exercise` varchar(10) DEFAULT NULL,
  `rebate` varchar(1) DEFAULT NULL,
  `send_status` varchar(1) DEFAULT NULL,
  `calculate_status` int(1) DEFAULT NULL,
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `bond_key_listed_market` varchar(28) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test3`
--

DROP TABLE IF EXISTS `test3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test3` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `goods_code` varchar(255) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `volume` int(11) DEFAULT NULL,
  `symbol` int(11) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `testoffer`
--

DROP TABLE IF EXISTS `testoffer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `testoffer` (
  `id` varchar(32) NOT NULL DEFAULT '',
  `goods_code` varchar(32) DEFAULT NULL,
  `price` decimal(10,4) DEFAULT NULL,
  `volume` int(8) DEFAULT NULL,
  `symbol` int(8) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:24:21
