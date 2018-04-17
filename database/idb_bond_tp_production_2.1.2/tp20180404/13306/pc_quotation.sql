-- MySQL dump 10.13  Distrib 5.6.22, for linux-glibc2.5 (x86_64)
--
-- Host: 192.168.162.110    Database: pc_quotation
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
-- Table structure for table `BOND_COLLECT_AVERAGE`
--

DROP TABLE IF EXISTS `BOND_COLLECT_AVERAGE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `BOND_COLLECT_AVERAGE` (
  `ID` varchar(32) NOT NULL,
  `BOND_KEY` varchar(25) NOT NULL,
  `LISTED_MARKET` varchar(3) NOT NULL,
  `UNDERWRITER_ID` varchar(32) NOT NULL,
  `COUNTER_PARTY_INSTI` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTI_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手机构名称',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易名称',
  `WGHT_AVG_VOL` varchar(32) DEFAULT NULL COMMENT '全场加权中标面值',
  `WGHT_AVG_PRICE` varchar(20) DEFAULT NULL COMMENT '全场加权平均中标价格',
  `WGHT_AVG_COUPON` varchar(20) DEFAULT NULL COMMENT '全场加权平均中标收益率',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '本方交易员',
  `DELIVERY_TYPE` int(11) DEFAULT NULL,
  `RETURN_TYPE` int(11) DEFAULT NULL,
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_DF_PROPRIETARY`
--

DROP TABLE IF EXISTS `TB_DF_PROPRIETARY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_DF_PROPRIETARY` (
  `INSTITUTION_ID` varchar(20) NOT NULL COMMENT '机构ID',
  `INSTITUTION_NAME` varchar(128) NOT NULL COMMENT '机构名称',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  PRIMARY KEY (`INSTITUTION_ID`,`UNDERWRITER_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_EXPORT_STATUS`
--

DROP TABLE IF EXISTS `TB_EXPORT_STATUS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_EXPORT_STATUS` (
  `EXPORT_ID` varchar(32) NOT NULL COMMENT ' ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(32) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `STATUS` int(11) NOT NULL DEFAULT '0' COMMENT '0-未导出  1-已导出',
  `OPERATOR_ID` varchar(32) NOT NULL COMMENT '操作员ID',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '更新时间',
  PRIMARY KEY (`EXPORT_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_LISTED_TRADE`
--

DROP TABLE IF EXISTS `TB_LISTED_TRADE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_LISTED_TRADE` (
  `LISTED_ID` varchar(32) NOT NULL COMMENT '上市交易信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(32) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `NET_PRICE` varchar(32) DEFAULT NULL COMMENT '交易净价',
  `BID_VALUE` varchar(32) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `TRADE_TO_SETTLEMENT` varchar(100) DEFAULT NULL COMMENT '清算速度，T+0, T+1等',
  `TRADE_DATE` decimal(20,0) DEFAULT NULL COMMENT '交易日',
  `SETTLEMENT_DATE` decimal(20,0) DEFAULT NULL COMMENT '结算日',
  `COMMISSION` varchar(32) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `CAPITAL_COST_TYPE` varchar(32) DEFAULT NULL COMMENT '资金成本类型，有以下几种形式：R001, R007，票面(FaceValue)，或直接输入数值。其他形式均为非法。',
  `CAPITAL_COST_VALUE` varchar(32) DEFAULT NULL COMMENT '资金成本值',
  `BID_RATE` varchar(32) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(32) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名，可能为空',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`LISTED_ID`),
  KEY `INDEX_TRADE_ID` (`TRADE_ID`),
  KEY `INDEX_BK_LM_UND` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`),
  KEY `INDEX_BK_LM` (`BOND_KEY`,`LISTED_MARKET`),
  KEY `INDEX_BOND_KEY` (`BOND_KEY`),
  KEY `INDEX_UNDERWRITER_ID` (`UNDERWRITER_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_LISTED_TRADE_HISTORY`
--

DROP TABLE IF EXISTS `TB_LISTED_TRADE_HISTORY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_LISTED_TRADE_HISTORY` (
  `HISTORY_ID` varchar(32) NOT NULL COMMENT '历史记录ID',
  `LISTED_ID` varchar(32) NOT NULL COMMENT '上市交易信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(32) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `NET_PRICE` varchar(32) DEFAULT NULL COMMENT '交易净价',
  `BID_VALUE` varchar(32) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `TRADE_TO_SETTLEMENT` varchar(100) DEFAULT NULL COMMENT '清算速度，T+0, T+1等',
  `TRADE_DATE` decimal(20,0) DEFAULT NULL COMMENT '交易日',
  `SETTLEMENT_DATE` decimal(20,0) DEFAULT NULL COMMENT '结算日',
  `COMMISSION` varchar(32) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `CAPITAL_COST_TYPE` varchar(32) DEFAULT NULL COMMENT '资金成本类型，有以下几种形式：R001, R007，票面(FaceValue)，或直接输入数值。其他形式均为非法。',
  `CAPITAL_COST_VALUE` varchar(32) DEFAULT NULL COMMENT '资金成本值',
  `BID_RATE` varchar(32) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(32) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT ' 交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名，可能为空',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向'' AFTER `OPERATOR_ID',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`HISTORY_ID`),
  KEY `INDEX_TRADE_ID` (`TRADE_ID`),
  KEY `INDEX_BK_LM_UND` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`),
  KEY `INDEX_BK_LM` (`BOND_KEY`,`LISTED_MARKET`),
  KEY `INDEX_BOND_KEY` (`BOND_KEY`),
  KEY `INDEX_UNDERWRITER_ID` (`UNDERWRITER_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PAYMENT`
--

DROP TABLE IF EXISTS `TB_PAYMENT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PAYMENT` (
  `PAYMENT_ID` varchar(32) NOT NULL COMMENT '缴款分销信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `PAYMENT_PRICE` varchar(32) DEFAULT NULL COMMENT '缴款分销价格',
  `BID_VALUE` varchar(32) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `COMMISSION` varchar(32) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `BID_RATE` varchar(32) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(32) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`PAYMENT_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PAYMENT_HISTORY`
--

DROP TABLE IF EXISTS `TB_PAYMENT_HISTORY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PAYMENT_HISTORY` (
  `HISTORY_ID` varchar(32) NOT NULL COMMENT '主键',
  `PAYMENT_ID` varchar(32) NOT NULL COMMENT '缴款分销信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `PAYMENT_PRICE` varchar(20) DEFAULT NULL COMMENT '缴款分销价格',
  `BID_VALUE` varchar(20) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `COMMISSION` varchar(20) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `BID_RATE` varchar(20) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(20) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`HISTORY_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PROPORTION_STATISTICS`
--

DROP TABLE IF EXISTS `TB_PROPORTION_STATISTICS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PROPORTION_STATISTICS` (
  `PSID` varchar(32) NOT NULL COMMENT 'id',
  `UNDERWRITERID` varchar(32) NOT NULL COMMENT '机构id',
  `OPERATORID` varchar(32) NOT NULL COMMENT '操作员',
  `BEGINDATE` decimal(8,0) DEFAULT NULL COMMENT '起始时间',
  `ENDDATE` decimal(8,0) DEFAULT NULL COMMENT '结束时间',
  `ISSUEAMOUNTALL` varchar(20) DEFAULT NULL COMMENT '总发行量',
  `BIDVOLALL` varchar(20) DEFAULT '' COMMENT '总投标量',
  `OUTCOMEVOLALL` varchar(20) DEFAULT NULL COMMENT '总中标量',
  `OV_IA_PROPORTION` varchar(9) DEFAULT NULL COMMENT '中标量市场占比',
  `OV_BV_PROPORTION` varchar(9) DEFAULT NULL COMMENT '中标量/总投标量',
  `BV_IA_PROPORTION` varchar(9) DEFAULT NULL COMMENT '总投标量/总发行量',
  `UPDATETIME` datetime DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  `BDEL` tinyint(1) DEFAULT NULL COMMENT '是否删除',
  PRIMARY KEY (`PSID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PROPORTION_STATISTICS_HISTORY`
--

DROP TABLE IF EXISTS `TB_PROPORTION_STATISTICS_HISTORY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PROPORTION_STATISTICS_HISTORY` (
  `HISID` varchar(32) NOT NULL COMMENT 'id',
  `PSID` varchar(32) NOT NULL COMMENT 'id',
  `UNDERWRITERID` varchar(32) NOT NULL COMMENT '机构id',
  `OPERATORID` varchar(32) NOT NULL COMMENT '操作员',
  `BEGINDATE` decimal(8,0) DEFAULT NULL COMMENT '起始时间',
  `ENDDATE` decimal(8,0) DEFAULT NULL COMMENT '结束时间',
  `ISSUEAMOUNTALL` varchar(20) DEFAULT NULL COMMENT '总发行量',
  `BIDVOLALL` varchar(20) DEFAULT '' COMMENT '总投标量',
  `OUTCOMEVOLALL` varchar(20) DEFAULT NULL COMMENT '总中标量',
  `OV_IA_PROPORTION` varchar(9) DEFAULT NULL COMMENT '中标量市场占比',
  `OV_BV_PROPORTION` varchar(9) DEFAULT NULL COMMENT '中标量/总投标量',
  `BV_IA_PROPORTION` varchar(9) DEFAULT NULL COMMENT '总投标量/总发行量',
  `UPDATETIME` datetime DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  `BDEL` tinyint(1) DEFAULT NULL COMMENT '是否删除',
  PRIMARY KEY (`HISID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PROXY_LISTED_TRADE`
--

DROP TABLE IF EXISTS `TB_PROXY_LISTED_TRADE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PROXY_LISTED_TRADE` (
  `PROXY_LISTED_ID` varchar(32) NOT NULL COMMENT ' 代缴款上市交易信息ID',
  `PROXY_PAYMENT_ID` varchar(32) NOT NULL COMMENT '代缴款缴款分销信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(32) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `NET_PRICE` varchar(32) DEFAULT NULL COMMENT '交易净价',
  `BID_VALUE` varchar(32) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `TRADE_TO_SETTLEMENT` varchar(100) DEFAULT NULL COMMENT '清算速度，T+0, T+1等',
  `TRADE_DATE` decimal(20,0) DEFAULT NULL COMMENT '交易日',
  `SETTLEMENT_DATE` decimal(20,0) DEFAULT NULL COMMENT '结算日',
  `COMMISSION` varchar(32) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `CAPITAL_COST_TYPE` varchar(32) DEFAULT NULL COMMENT '资金成本类型，有以下几种形式：R001, R007，票面(FaceValue)，或直接输入数值。其他形式均为非法。',
  `CAPITAL_COST_VALUE` varchar(32) DEFAULT NULL COMMENT '资金成本值',
  `BID_RATE` varchar(32) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(32) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名，可能为空',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`PROXY_LISTED_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PROXY_LISTED_TRADE_HISTORY`
--

DROP TABLE IF EXISTS `TB_PROXY_LISTED_TRADE_HISTORY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PROXY_LISTED_TRADE_HISTORY` (
  `HISTORY_ID` varchar(32) NOT NULL COMMENT '历史记录ID',
  `PROXY_LISTED_ID` varchar(32) NOT NULL COMMENT ' 代缴款上市交易信息ID',
  `PROXY_PAYMENT_ID` varchar(32) NOT NULL COMMENT '代缴款缴款分销信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(32) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `NET_PRICE` varchar(32) DEFAULT NULL COMMENT '交易净价',
  `BID_VALUE` varchar(32) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `TRADE_TO_SETTLEMENT` varchar(100) DEFAULT NULL COMMENT '清算速度，T+0, T+1等',
  `TRADE_DATE` decimal(20,0) DEFAULT NULL COMMENT '交易日',
  `SETTLEMENT_DATE` decimal(20,0) DEFAULT NULL COMMENT '结算日',
  `COMMISSION` varchar(32) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `CAPITAL_COST_TYPE` varchar(32) DEFAULT NULL COMMENT '资金成本类型，有以下几种形式：R001, R007，票面(FaceValue)，或直接输入数值。其他形式均为非法。',
  `CAPITAL_COST_VALUE` varchar(32) DEFAULT NULL COMMENT '资金成本值',
  `BID_RATE` varchar(32) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(32) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名，可能为空',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`HISTORY_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PROXY_PAYMENT`
--

DROP TABLE IF EXISTS `TB_PROXY_PAYMENT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PROXY_PAYMENT` (
  `PROXY_PAYMENT_ID` varchar(32) NOT NULL COMMENT '代缴款缴款分销信息ID',
  `PROXY_LISTED_ID` varchar(32) NOT NULL COMMENT ' 代缴款上市交易信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `PAYMENT_PRICE` varchar(32) DEFAULT NULL COMMENT '缴款分销价格',
  `BID_VALUE` varchar(32) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `COMMISSION` varchar(32) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `BID_RATE` varchar(32) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(32) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`PROXY_PAYMENT_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PROXY_PAYMENT_HISTORY`
--

DROP TABLE IF EXISTS `TB_PROXY_PAYMENT_HISTORY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PROXY_PAYMENT_HISTORY` (
  `HISTORY_ID` varchar(32) NOT NULL COMMENT '主键',
  `PROXY_PAYMENT_ID` varchar(32) NOT NULL COMMENT '代缴款缴款分销信息ID',
  `PROXY_LISTED_ID` varchar(32) NOT NULL COMMENT ' 代缴款上市交易信息ID',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易明细ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员名',
  `PAYMENT_PRICE` varchar(20) DEFAULT NULL COMMENT '缴款分销价格',
  `BID_VALUE` varchar(20) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `COMMISSION` varchar(20) DEFAULT NULL COMMENT '交易手续费',
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式，0无效，1无返，2单返，3折返。默认为0',
  `BID_RATE` varchar(20) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(20) DEFAULT NULL COMMENT '中标价格',
  `TRADE_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易机构ID',
  `TRADE_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易机构名',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '我方交易员ID',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `BUY_SELL_DIRECTION` int(4) DEFAULT NULL COMMENT '具体的卖出/买入方向  上市交易：0：上市卖出（客户） 1：上市卖出（自营） 2：上市代投买入  3：上市代缴买入  缴款分销：0：分销卖出（客户） 1：分销卖出（代缴款） 2：分销代投买入 3：缴款金额',
  `REMARKS` varchar(30) DEFAULT NULL COMMENT '备注',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`HISTORY_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_TRADE`
--

DROP TABLE IF EXISTS `TB_TRADE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_TRADE` (
  `TRADE_COMMISSION_ID` varchar(32) NOT NULL DEFAULT '',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COMMISSION` varchar(20) DEFAULT NULL COMMENT '交易明细手续费',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`TRADE_COMMISSION_ID`),
  KEY `INDEX_TB_TRADE` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`),
  KEY `INDEX_TB_TRADE_HISTORY` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`),
  KEY `INDEX_TB_TRADE_DETAIL` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`),
  KEY `INDEX_TB_TRADE_DETAIL_HISTORY` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`),
  KEY `INDEX_TB_PAYMENT` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`),
  KEY `INDEX_TB_PAYMENT_HISTORY` (`BOND_KEY`,`LISTED_MARKET`,`UNDERWRITER_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_TRADE_DETAIL`
--

DROP TABLE IF EXISTS `TB_TRADE_DETAIL`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_TRADE_DETAIL` (
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易信息ID',
  `COLLECT_ID` varchar(128) DEFAULT NULL COMMENT '分配结果汇总ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员',
  `BID_RATE` varchar(20) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(20) DEFAULT NULL COMMENT '中标价格',
  `BID_VALUE` varchar(20) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `DELIVERY_TYPE` int(11) DEFAULT NULL COMMENT '交割方式',
  `CAPITAL_COST_TYPE` varchar(32) DEFAULT NULL COMMENT '资金成本类型',
  `CAPITAL_COST_VALUE` varchar(32) DEFAULT NULL,
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式',
  `COMMISSION` varchar(20) DEFAULT NULL COMMENT '交易明细手续费',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '本方交易员',
  `OPERATOR_ID` varchar(32) NOT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`TRADE_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_TRADE_DETAIL_HISTORY`
--

DROP TABLE IF EXISTS `TB_TRADE_DETAIL_HISTORY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_TRADE_DETAIL_HISTORY` (
  `HISTORY_ID` varchar(32) NOT NULL COMMENT '主键',
  `TRADE_ID` varchar(32) NOT NULL COMMENT '交易信息ID',
  `COLLECT_ID` varchar(128) DEFAULT NULL COMMENT '分配结果汇总ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COUNTER_PARTY_INSTITUTION` varchar(32) DEFAULT NULL COMMENT '交易对手机构ID',
  `COUNTER_PARTY_INSTITUTION_NAME` varchar(128) DEFAULT NULL COMMENT '交易对手机构名',
  `COUNTER_PARTY_TRADER` varchar(32) DEFAULT NULL COMMENT '交易对手交易员ID',
  `COUNTER_PARTY_TRADER_NAME` varchar(32) DEFAULT NULL COMMENT '交易对手交易员',
  `BID_RATE` varchar(20) DEFAULT NULL COMMENT '中标利率',
  `BID_PRICE` varchar(20) DEFAULT NULL COMMENT '中标价格',
  `BID_VALUE` varchar(20) DEFAULT NULL COMMENT '中标面值，存储单位为万元',
  `DELIVERY_TYPE` int(11) DEFAULT NULL COMMENT '交割方式',
  `CAPITAL_COST_TYPE` varchar(32) DEFAULT NULL COMMENT '资金成本类型',
  `CAPITAL_COST_VALUE` varchar(32) DEFAULT NULL,
  `RETURN_TYPE` int(11) DEFAULT NULL COMMENT '返费方式',
  `COMMISSION` varchar(20) DEFAULT NULL COMMENT '交易明细手续费',
  `TRADER_ID` varchar(32) DEFAULT NULL COMMENT '本方交易员',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`HISTORY_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_TRADE_HISTORY`
--

DROP TABLE IF EXISTS `TB_TRADE_HISTORY`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_TRADE_HISTORY` (
  `HISTORY_ID` varchar(32) NOT NULL COMMENT '主键',
  `TRADE_COMMISSION_ID` varchar(32) DEFAULT NULL COMMENT '交易信息ID',
  `BOND_KEY` varchar(32) NOT NULL COMMENT '债券编号',
  `LISTED_MARKET` varchar(3) NOT NULL COMMENT '发行市场',
  `UNDERWRITER_ID` varchar(32) NOT NULL COMMENT '承销机构ID',
  `COMMISSION` varchar(20) DEFAULT NULL COMMENT '交易明细手续费',
  `OPERATOR_ID` varchar(32) DEFAULT NULL COMMENT '操作员ID',
  `UPDATE_TIME` datetime DEFAULT NULL COMMENT '操作时间',
  PRIMARY KEY (`HISTORY_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_bid_info`
--

DROP TABLE IF EXISTS `bond_bid_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_bid_info` (
  `ID` varchar(32) NOT NULL,
  `DelFlag` tinyint(4) DEFAULT NULL COMMENT '0-有效',
  `UnderwriterID` varchar(32) NOT NULL,
  `BondKey` varchar(25) DEFAULT NULL,
  `ListMarket` varchar(3) DEFAULT NULL,
  `BidPrice` decimal(20,4) DEFAULT NULL COMMENT '中标价格',
  `BidMultiple` decimal(20,4) DEFAULT NULL COMMENT '边际倍数',
  `Unit` tinyint(4) DEFAULT NULL,
  `TenderingType` tinyint(4) DEFAULT NULL COMMENT '招标方式      --0：荷兰式，：美国式，：混合式',
  `WeightedAveragePrice` decimal(20,4) DEFAULT NULL COMMENT '全场加权平均中标价格',
  `BidQuantity` decimal(20,4) DEFAULT NULL COMMENT '无主承销商时的中标量',
  `DistributionPrice` decimal(20,4) DEFAULT NULL COMMENT '无主承销商时的分销价格',
  `FeeRate` decimal(20,4) DEFAULT NULL COMMENT '无主承销商时的手续费率',
  `BidQuantitySum` decimal(20,4) DEFAULT NULL,
  `DeliveryType` tinyint(4) DEFAULT NULL,
  `OTime` timestamp NULL DEFAULT NULL,
  `BidRate` decimal(20,4) DEFAULT NULL COMMENT '中标收益率',
  `WeightedAverageCoupon` decimal(20,4) DEFAULT NULL COMMENT '全场加权平均中标收益率',
  `MarketMutiple` decimal(20,4) DEFAULT NULL COMMENT '全场倍数（客户填写，不参与计算）',
  `BondCoupon` decimal(20,4) DEFAULT NULL COMMENT '票面利率（客户填写，不参与计算）',
  PRIMARY KEY (`ID`),
  KEY `BkLm` (`DelFlag`,`BondKey`,`ListMarket`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='中标信息表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_bid_quantity`
--

DROP TABLE IF EXISTS `bond_bid_quantity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_bid_quantity` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `DelFlag` tinyint(4) DEFAULT '0' COMMENT '0有效',
  `UnderwriterID` varchar(32) NOT NULL,
  `DidInfoID` varchar(32) DEFAULT '0' COMMENT 'hy_bond_bid_info ID',
  `UnderwriterCode` varchar(8) DEFAULT '0' COMMENT '承/分销商ID',
  `UnderwriterType` varchar(3) DEFAULT '0' COMMENT '承/分销商类型 主MUN/分UNG',
  `BidQuantity` decimal(20,4) DEFAULT '0.0000' COMMENT '中标量',
  `DistributionPrice` decimal(20,4) NOT NULL DEFAULT '0.0000' COMMENT '分销价格',
  `FeeRate` decimal(20,4) NOT NULL DEFAULT '0.0000' COMMENT '手续费率',
  PRIMARY KEY (`ID`,`UnderwriterID`),
  KEY `DII` (`DelFlag`,`DidInfoID`)
) ENGINE=InnoDB AUTO_INCREMENT=72938 DEFAULT CHARSET=utf8 COMMENT='承/分销商与对应的中标量';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_main_underwriter_notice`
--

DROP TABLE IF EXISTS `bond_main_underwriter_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_main_underwriter_notice` (
  `NoticeID` varchar(32) NOT NULL,
  `BondKey` varchar(25) DEFAULT NULL,
  `ListedMarket` varchar(3) DEFAULT NULL,
  `DeleteFlag` tinyint(4) DEFAULT NULL COMMENT '0:有效 1:无效',
  `MWNotice` varchar(1024) DEFAULT NULL COMMENT '公告内容',
  `OperatorID` varchar(32) DEFAULT NULL COMMENT 'PC操作员用户ID',
  `UnderwriterID` varchar(32) NOT NULL,
  `UpdateTime` datetime DEFAULT NULL COMMENT '修改时间',
  PRIMARY KEY (`NoticeID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_notice`
--

DROP TABLE IF EXISTS `bond_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_notice` (
  `ID` varchar(32) NOT NULL,
  `DelFlag` tinyint(4) DEFAULT NULL COMMENT '0:有效 1:无效',
  `UnderwriterID` varchar(32) NOT NULL,
  `Title` varchar(160) DEFAULT NULL,
  `Notice` varchar(1024) DEFAULT NULL COMMENT '公告内容',
  `BCUserID` varchar(32) DEFAULT NULL COMMENT '用户ID',
  `Time` datetime DEFAULT NULL COMMENT '修改时间',
  `type` int(11) DEFAULT '0',
  `url` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='公告管理';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_quot`
--

DROP TABLE IF EXISTS `bond_quot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_quot` (
  `id` varchar(32) NOT NULL COMMENT 'BondKey + ListedMarket',
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `push_status` tinyint(1) DEFAULT NULL COMMENT '0:未推送，1:已推送',
  `once_pushed` tinyint(1) DEFAULT NULL,
  `quot_status` tinyint(1) DEFAULT NULL COMMENT '状态（是否投标等）0:未投标 1:已投标 2:已分配 3:撤销分配（与1平行）4:撤销投标（1:已投标，在承分销页面显示为“等待分配”）',
  `delay_publication` tinyint(1) DEFAULT NULL COMMENT '是否推迟发行 0:不推迟 1:已推迟',
  `have_bid` tinyint(1) DEFAULT NULL COMMENT '是否有报价，包括未确认、已确认和已撤销 0:没有 1:有',
  `predict_yield_s` varchar(16) DEFAULT NULL COMMENT '预测价格区间(起始)',
  `predict_yield_e` varchar(16) DEFAULT NULL COMMENT '预测价格区间(结束)',
  `predict_issue_date` date DEFAULT NULL COMMENT '预测发行日期',
  `bid_amount_limit_unit` tinyint(1) DEFAULT NULL COMMENT '投标总量上限单位		--	0:百分比(%),1:面值(元)'';',
  `bid_amount_limit` varchar(10) DEFAULT NULL COMMENT '投标总量上限''',
  `bid_yield_s` varchar(16) DEFAULT NULL COMMENT '投标区间%(起始)',
  `bid_yield_e` varchar(16) DEFAULT NULL COMMENT '投标区间%(结束)',
  `suggest_lower_limit` varchar(16) DEFAULT NULL,
  `suggest_upper_limit` varchar(16) DEFAULT NULL,
  `auction_end_time` datetime DEFAULT NULL COMMENT '截标时间',
  `confirmed_bid` varchar(4) DEFAULT NULL,
  `description` varchar(1000) DEFAULT NULL COMMENT '产品描述',
  `bid_price` decimal(20,4) DEFAULT NULL,
  `update_time` datetime DEFAULT NULL COMMENT '最后更新时间',
  `operator_id` varchar(8) DEFAULT NULL,
  `underwrite_name` varchar(128) DEFAULT NULL,
  `modify_flag` smallint(2) unsigned NOT NULL DEFAULT '0',
  `unit` smallint(2) DEFAULT NULL COMMENT '单位 -- 0:收益率(%),1:全价(元)',
  `unit_confirmed` smallint(2) DEFAULT NULL,
  `quotation_info_edited` smallint(2) DEFAULT NULL,
  `auction_limit_unit` smallint(2) DEFAULT NULL COMMENT '单一标位上限单位	-- 0:收益率(%),1:全价(元)',
  `auction_limit` varchar(16) DEFAULT NULL COMMENT '投标总量上限',
  `recommended` smallint(2) DEFAULT NULL COMMENT '0：取消推荐，1：推荐',
  `approval_msg` varchar(60) DEFAULT NULL,
  `send_to_hengtai` smallint(2) DEFAULT NULL COMMENT '宏源专用，是否推送至衡泰，0：未推送/已撤回，1：已推送',
  `credit_balance_controlled` varchar(1) DEFAULT NULL COMMENT '是否需要进行授信余额控制。0：不需要；1：需要。默认为0。',
  `bond_score` varchar(20) DEFAULT NULL COMMENT '债券评分（平安版本需求）',
  `supervisor` varchar(32) DEFAULT NULL COMMENT '负责人，目前暂定使用ID，由当前交易员中选择',
  `UnderwriterID` varchar(32) NOT NULL,
  `spread` varchar(8) DEFAULT NULL COMMENT '客户编辑的基准利率值',
  `MargGuid` varchar(160) DEFAULT NULL COMMENT '边际指导内容',
  `edit_price` decimal(20,4) DEFAULT NULL,
  `distribute_price` decimal(20,4) DEFAULT NULL,
  `PushTime` datetime DEFAULT NULL COMMENT '最后推送的时间',
  `return_rate` varchar(12) DEFAULT NULL COMMENT '手续费',
  `return_rate_unit` int(4) DEFAULT NULL COMMENT '手续费单位，0：利率；1：价格',
  PRIMARY KEY (`id`,`UnderwriterID`),
  KEY `BONDKEY_UNDERWRITERID_INDEX` (`bond_key`,`UnderwriterID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_quot_history`
--

DROP TABLE IF EXISTS `bond_quot_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_quot_history` (
  `id` varchar(32) NOT NULL,
  `bond_key` varchar(25) DEFAULT NULL,
  `listed_market` varchar(3) DEFAULT NULL,
  `push_status` tinyint(1) DEFAULT NULL COMMENT '0:未推送，1:已推送',
  `once_pushed` tinyint(1) DEFAULT NULL,
  `ori_quot_status` tinyint(1) DEFAULT NULL COMMENT '前一次quot_status',
  `quot_status` tinyint(1) DEFAULT NULL COMMENT '状态（是否投标等）0:未投标 1:已投标 2:已分配 3:撤销分配（与1平行）',
  `delay_publication` tinyint(1) DEFAULT NULL COMMENT '是否推迟发行 0:不推迟 1:已推迟',
  `have_bid` tinyint(1) DEFAULT NULL,
  `predict_yield_s` varchar(16) DEFAULT NULL COMMENT '预测价格区间(起始)',
  `predict_yield_e` varchar(16) DEFAULT NULL COMMENT '预测价格区间(结束)',
  `predict_issue_date` date DEFAULT NULL COMMENT '预测发行日期',
  `bid_amount_limit_unit` tinyint(1) DEFAULT NULL COMMENT '投标总量上限单位--0:百分比(%),1:面值(元)',
  `bid_amount_limit` varchar(10) DEFAULT NULL COMMENT '投标总量上限',
  `bid_yield_s` varchar(16) DEFAULT NULL COMMENT '投标区间%(起始)',
  `bid_yield_e` varchar(16) DEFAULT NULL COMMENT '投标区间%(结束)',
  `suggest_lower_limit` varchar(16) DEFAULT NULL,
  `suggest_upper_limit` varchar(16) DEFAULT NULL,
  `auction_end_time` datetime DEFAULT NULL COMMENT '截标时间',
  `confirmed_bid` varchar(4) DEFAULT NULL,
  `bid_price` decimal(20,4) DEFAULT NULL,
  `unit` varchar(1) DEFAULT NULL COMMENT '单位 -- 0:收益率(%),1:全价(元)',
  `auction_limit_unit` varchar(1) DEFAULT NULL COMMENT '单一标位上限单位	-- 0:收益率(%),1:全价(元)',
  `auction_limit` varchar(16) DEFAULT NULL,
  `description` varchar(1000) DEFAULT NULL COMMENT '产品描述',
  `update_time` datetime DEFAULT NULL COMMENT '最后更新时间',
  `operator_id` varchar(8) DEFAULT NULL,
  `underwrite_name` varchar(128) DEFAULT NULL,
  `modify_flag` smallint(2) unsigned NOT NULL DEFAULT '0',
  `unit_confirmed` smallint(2) DEFAULT NULL,
  `quotation_info_edited` smallint(2) DEFAULT NULL,
  `recommended` smallint(2) DEFAULT NULL COMMENT '0：取消推荐，1：推荐',
  `approval_msg` varchar(60) DEFAULT NULL,
  `send_to_hengtai` smallint(2) DEFAULT NULL COMMENT '宏源专用，是否推送至衡泰，0：未推送/已撤回，1：已推送',
  `credit_balance_controlled` varchar(1) DEFAULT NULL COMMENT '是否需要进行授信余额控制。0：不需要；1：需要。默认为0。',
  `bond_score` varchar(20) DEFAULT NULL COMMENT '债券评分（平安版本需求）',
  `supervisor` varchar(32) DEFAULT NULL COMMENT '负责人，目前暂定使用ID，由当前交易员中选择',
  `UnderwriterID` varchar(32) NOT NULL,
  `spread` varchar(8) DEFAULT NULL,
  `MargGuid` varchar(50) DEFAULT NULL COMMENT '边际指导内容',
  `edit_price` decimal(20,4) DEFAULT NULL,
  `distribute_price` decimal(20,4) DEFAULT NULL,
  `PushTime` datetime DEFAULT NULL COMMENT '最后推送的时间',
  `return_rate` varchar(12) DEFAULT NULL COMMENT '手续费',
  `return_rate_unit` int(4) DEFAULT NULL COMMENT '手续费单位，0：利率；1：价格',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_quote_announced`
--

DROP TABLE IF EXISTS `bond_quote_announced`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_quote_announced` (
  `ID` varchar(32) NOT NULL,
  `DelFlag` varchar(25) DEFAULT NULL COMMENT '0-有效',
  `UnderwriterID` varchar(32) NOT NULL,
  `BondKey` varchar(25) DEFAULT NULL,
  `ListMarket` varchar(3) DEFAULT NULL,
  `Vol` varchar(12) DEFAULT NULL,
  `Unit` tinyint(4) DEFAULT NULL,
  `BidCoupon` varchar(12) DEFAULT NULL COMMENT '投标票面',
  `ExType` tinyint(4) DEFAULT NULL COMMENT '补充信息，0:票面;1:综收',
  `Coupon` varchar(12) DEFAULT NULL COMMENT '票面数值',
  `ReturnType` tinyint(4) DEFAULT NULL COMMENT '选择票面后，选择是否单返。若选择票面则此项无意义。0:无返;1:单返',
  `ReturnRate` varchar(12) DEFAULT NULL COMMENT '单返数值',
  `ComprehensiveIncome` varchar(12) DEFAULT NULL COMMENT '综收数值',
  `UpperLimit` varchar(12) DEFAULT NULL COMMENT '上限，仅在选择综收时有效',
  `Operation` tinyint(4) DEFAULT NULL COMMENT '报价标志，0--未确认，1--已确认, 2-撤销',
  `BCCompanyID` varchar(32) DEFAULT NULL COMMENT '机构ID',
  `BCUserID` varchar(32) DEFAULT NULL COMMENT '联系人ID',
  `BCTraderID` varchar(32) DEFAULT NULL COMMENT '交易员ID',
  `QTime` timestamp NULL DEFAULT NULL COMMENT '报价时间',
  `OutcomeFaceValue` decimal(20,4) DEFAULT NULL COMMENT '中标票面',
  `OutcomeNetPrice` decimal(20,4) DEFAULT NULL COMMENT '中标净价',
  `OutcomeQuantity` decimal(20,4) DEFAULT NULL COMMENT '中标面值',
  `SingleReturnType` tinyint(4) DEFAULT NULL COMMENT '单返方式，0为净价，1为其它',
  `Commission` decimal(20,4) DEFAULT NULL COMMENT '手续费',
  `OTime` timestamp NULL DEFAULT NULL COMMENT '中标时间',
  `Source` tinyint(4) DEFAULT NULL COMMENT '当前记录来源，0为宏源录入，1为QB录入',
  `QBUserID` varchar(32) DEFAULT NULL COMMENT 'QB用户ID',
  `QBInstitutionID` varchar(32) DEFAULT NULL COMMENT 'QB机构ID',
  `QBUserName` varchar(64) DEFAULT NULL COMMENT 'QB用户名',
  `QBInstitutionName` varchar(64) DEFAULT NULL COMMENT 'QB机构名',
  `Remarks` varchar(64) DEFAULT NULL COMMENT '备注',
  `ExternalInstitution` varchar(100) DEFAULT NULL,
  `ExternalInfo` varchar(300) DEFAULT NULL COMMENT '从外部接收的交易中存储的信息（宏源端在此字段中存储外部交易ID）',
  `DeliveryType` tinyint(4) DEFAULT NULL COMMENT '交割方式。0:上市，1:上市代持，2:分销-单返，3:分销-折返。默认为0。',
  `PayInstitutionID` varchar(32) DEFAULT NULL COMMENT '缴款机构ID',
  `PIUserID` varchar(32) DEFAULT NULL COMMENT '缴款机构联系人ID',
  `PayPrice` varchar(20) DEFAULT NULL COMMENT '缴款价格',
  `CouponInstitutionID` varchar(32) DEFAULT NULL COMMENT '过券机构',
  `CIUserID` varchar(32) DEFAULT NULL COMMENT '过券机构联系人ID',
  `CouponPrice` varchar(20) DEFAULT NULL COMMENT '过券价格',
  `PayInstitution` varchar(100) DEFAULT NULL COMMENT '缴款机构名称',
  `ModifiedTag` int(11) DEFAULT NULL,
  `comment` varchar(512) DEFAULT NULL,
  `SuExType` int(11) DEFAULT NULL,
  `SuDeliveryType` int(11) DEFAULT NULL,
  `SuReturnType` int(11) DEFAULT NULL,
  `unit_v2` int(11) DEFAULT NULL,
  `bidcoupon_v2_y` varchar(128) DEFAULT NULL,
  `bidcoupon_v2_p` varchar(128) DEFAULT NULL,
  `BidDocPrice` varchar(12) DEFAULT NULL COMMENT '投标标位，在有标书时根据BidCoupon查表得出；没有标书时客户端发布为空 ',
  `ReturnRateUnit` int(4) DEFAULT NULL COMMENT '手续费单位，0：利率；1：价格',
  `BUY_SELL` int(4) DEFAULT NULL COMMENT ' 0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， 1表示银行间，2表示上交所，3表示深交所',
  `DELIVERY_PRICE` varchar(20) DEFAULT NULL COMMENT '交割价格',
  `MAIN_VENDORS_ID` varchar(32) DEFAULT NULL COMMENT '主承销商ID',
  `MAIN_VENDORS_NAME` varchar(32) DEFAULT NULL COMMENT '主承销商名称',
  `UNDERWRITER_CODE` varchar(128) DEFAULT NULL COMMENT '分销团',
  `SUPERVISOR` varchar(32) DEFAULT NULL COMMENT '负责人',
  `MARKET_MAKER` varchar(32) DEFAULT NULL COMMENT '做市商 ',
  `NET_PRICE` varchar(32) DEFAULT NULL COMMENT '交易净价 ',
  PRIMARY KEY (`ID`),
  KEY `BkLmCID` (`DelFlag`,`BondKey`,`ListMarket`,`BCCompanyID`) USING BTREE,
  KEY `QTime` (`QTime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='已公告报价-分配';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_quote_announced_history`
--

DROP TABLE IF EXISTS `bond_quote_announced_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_quote_announced_history` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `QuoteID` varchar(32) DEFAULT '0' COMMENT 'hy_bond_quote_qnnounced.ID报价id',
  `DelFlag` varchar(25) DEFAULT NULL COMMENT '0-有效',
  `Vol` varchar(12) DEFAULT NULL,
  `Unit` tinyint(4) DEFAULT NULL,
  `BidCoupon` varchar(12) DEFAULT NULL COMMENT '投标票面',
  `ExType` tinyint(4) DEFAULT NULL COMMENT '补充信息，0:票面;1:综收',
  `Coupon` varchar(12) DEFAULT NULL COMMENT '票面数值',
  `ReturnType` tinyint(4) DEFAULT NULL COMMENT '选择票面后，选择是否单返。若选择票面则此项无意义。0:无返;1:单返',
  `ReturnRate` varchar(12) DEFAULT NULL COMMENT '单返数值',
  `ComprehensiveIncome` varchar(12) DEFAULT NULL COMMENT '综收数值',
  `UpperLimit` varchar(12) DEFAULT NULL COMMENT '上限，仅在选择综收时有效',
  `Operation` tinyint(4) DEFAULT NULL COMMENT '报价标志，0--未确认，1--已确认, 2-撤销',
  `BCCompanyID` varchar(32) DEFAULT NULL COMMENT '机构ID',
  `BCUserID` varchar(32) DEFAULT NULL COMMENT '联系人ID',
  `BCTraderID` varchar(32) DEFAULT NULL COMMENT '交易员ID',
  `QTime` timestamp NULL DEFAULT NULL COMMENT '报价时间',
  `OutcomeFaceValue` decimal(20,4) DEFAULT NULL COMMENT '中标票面',
  `OutcomeNetPrice` decimal(20,4) DEFAULT NULL COMMENT '中标净价',
  `OutcomeQuantity` decimal(20,4) DEFAULT NULL COMMENT '中标面值',
  `SingleReturnType` tinyint(4) DEFAULT NULL COMMENT '单返方式，0为净价，1为其它',
  `Commission` decimal(20,4) DEFAULT NULL COMMENT '手续费',
  `OTime` timestamp NULL DEFAULT NULL COMMENT '中标时间',
  `Source` tinyint(4) DEFAULT NULL COMMENT '当前记录来源，0为宏源录入，1为QB录入',
  `QBUserID` varchar(32) DEFAULT NULL COMMENT 'QB用户ID',
  `QBInstitutionID` varchar(32) DEFAULT NULL COMMENT 'QB机构ID',
  `QBUserName` varchar(64) DEFAULT NULL COMMENT 'QB用户名',
  `QBInstitutionName` varchar(64) DEFAULT NULL COMMENT 'QB机构名',
  `DTime` timestamp NULL DEFAULT NULL COMMENT '报价删除时间',
  `OperatorID` varchar(32) DEFAULT NULL COMMENT '操作发行人',
  `ExternalInstitution` varchar(100) DEFAULT NULL COMMENT '从外部接收的交易中存储的机构标识/名称',
  `ExternalInfo` varchar(300) DEFAULT NULL COMMENT '从外部接收的交易中存储的信息（宏源端在此字段中存储外部交易ID）',
  `DeliveryType` tinyint(4) DEFAULT NULL COMMENT '交割方式。0:上市，1:上市代持，2:分销-单返，3:分销-折返。默认为0。',
  `PayInstitutionID` varchar(32) DEFAULT NULL COMMENT '缴款机构ID',
  `PIUserID` varchar(32) DEFAULT NULL COMMENT '缴款机构联系人ID',
  `PayPrice` varchar(20) DEFAULT NULL COMMENT '缴款价格',
  `CouponInstitutionID` varchar(32) DEFAULT NULL COMMENT '过券机构',
  `CIUserID` varchar(32) DEFAULT NULL COMMENT '过券机构联系人ID',
  `CouponPrice` varchar(20) DEFAULT NULL COMMENT '过券价格',
  `PayInstitution` varchar(100) DEFAULT NULL COMMENT '缴款机构名称',
  `UnderwriterID` varchar(32) NOT NULL,
  `ModifiedTag` int(11) DEFAULT '0',
  `comment` varchar(512) DEFAULT NULL,
  `SuExType` int(11) DEFAULT NULL,
  `SuDeliveryType` int(11) DEFAULT NULL,
  `SuReturnType` int(11) DEFAULT NULL,
  `unit_v2` int(11) DEFAULT NULL,
  `bidcoupon_v2_y` varchar(128) DEFAULT NULL,
  `bidcoupon_v2_p` varchar(128) DEFAULT NULL,
  `CreateTime` timestamp NULL DEFAULT NULL COMMENT '添加这条记录的时间',
  `BidDocPrice` varchar(12) DEFAULT NULL COMMENT '投标标位， 在有标书时根据BidCoupon查表得出；没有标书时客户端发布为空',
  `ReturnRateUnit` int(4) DEFAULT NULL,
  `BondKey` varchar(25) DEFAULT NULL,
  `ListMarket` varchar(3) DEFAULT NULL,
  `BUY_SELL` int(4) DEFAULT NULL COMMENT '0和空表示卖出方向，1表示买入方向',
  `MANAGED_MARKET` int(4) DEFAULT NULL COMMENT '托管市场， -1表示银行间/交易所，0表示银行间，1表示上交所， 2表示深交所',
  `DELIVERY_PRICE` varchar(20) DEFAULT NULL COMMENT ' 交割价格',
  `MAIN_VENDORS_ID` varchar(32) DEFAULT NULL COMMENT '主承销商ID',
  `MAIN_VENDORS_NAME` varchar(32) DEFAULT NULL COMMENT '主承销商名称',
  `UNDERWRITER_CODE` varchar(32) DEFAULT NULL COMMENT '分销团',
  `SUPERVISOR` varchar(32) DEFAULT NULL COMMENT '负责人',
  `MARKET_MAKER` varchar(32) DEFAULT NULL COMMENT '做市商 ',
  `NET_PRICE` varchar(32) DEFAULT NULL COMMENT '交易净价 ',
  PRIMARY KEY (`ID`,`UnderwriterID`),
  KEY `QuoteID` (`QuoteID`),
  KEY `Index_CreateTime` (`CreateTime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=68039 DEFAULT CHARSET=utf8 COMMENT='已公告报价操作历史';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_quote_unannounced`
--

DROP TABLE IF EXISTS `bond_quote_unannounced`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_quote_unannounced` (
  `ID` varchar(32) NOT NULL,
  `DelFlag` tinyint(4) DEFAULT NULL COMMENT '0-有效',
  `UnderwriterID` varchar(32) NOT NULL,
  `BondKey` varchar(25) DEFAULT NULL,
  `ListMarket` varchar(3) DEFAULT NULL,
  `Yield` varchar(8) DEFAULT NULL COMMENT '价格收益率',
  `Vol` varchar(12) DEFAULT NULL COMMENT '面值',
  `Operation` tinyint(4) DEFAULT NULL COMMENT '报价标志，0--未确认，1--已确认',
  `BCCompanyID` varchar(32) DEFAULT NULL COMMENT '机构ID',
  `BCUserID` varchar(32) DEFAULT NULL COMMENT '联系人ID',
  `BCTraderID` varchar(32) DEFAULT NULL COMMENT '交易员ID',
  `QTime` timestamp NULL DEFAULT NULL COMMENT '报价时间',
  `Source` tinyint(4) DEFAULT NULL COMMENT '当前记录来源，0为宏源录入，1为QB录入',
  `QBUserID` varchar(32) DEFAULT NULL COMMENT 'QB用户ID',
  `QBInstitutionID` varchar(32) DEFAULT NULL COMMENT 'QB机构ID',
  `QBUserName` varchar(64) DEFAULT NULL COMMENT 'QB用户名',
  `QBInstitutionName` varchar(64) DEFAULT NULL COMMENT 'QB机构名',
  PRIMARY KEY (`ID`),
  KEY `BkLmCID` (`DelFlag`,`BondKey`,`ListMarket`,`BCCompanyID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='未公告报价';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_quote_unannounced_history`
--

DROP TABLE IF EXISTS `bond_quote_unannounced_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_quote_unannounced_history` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `QuoteID` varchar(32) DEFAULT '0' COMMENT 'hy_bond_quote_unqnnounced.ID报价id',
  `DelFlag` tinyint(4) DEFAULT NULL COMMENT '0-有效',
  `Yield` varchar(8) DEFAULT NULL COMMENT '价格收益率',
  `Vol` varchar(12) DEFAULT NULL COMMENT '面值',
  `Operation` tinyint(4) DEFAULT NULL COMMENT '报价标志，0--未确认，1--已确认',
  `BCCompanyID` varchar(32) DEFAULT NULL COMMENT '机构ID',
  `BCUserID` varchar(32) DEFAULT NULL COMMENT '联系人ID',
  `BCTraderID` varchar(32) DEFAULT NULL COMMENT '交易员ID',
  `QTime` timestamp NULL DEFAULT NULL COMMENT '报价时间',
  `Source` tinyint(4) DEFAULT NULL COMMENT '当前记录来源，0为宏源录入，1为QB录入',
  `QBUserID` varchar(32) DEFAULT NULL COMMENT 'QB用户ID',
  `QBInstitutionID` varchar(32) DEFAULT NULL COMMENT 'QB机构ID',
  `QBUserName` varchar(64) DEFAULT NULL COMMENT 'QB用户名',
  `QBInstitutionName` varchar(64) DEFAULT NULL COMMENT 'QB机构名',
  `DTime` timestamp NULL DEFAULT NULL COMMENT '报价删除时间',
  `OperatorID` varchar(32) DEFAULT NULL COMMENT '操作发行人',
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `QuoteID` (`QuoteID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='未公告报价操作历史';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_recommended_info`
--

DROP TABLE IF EXISTS `bond_recommended_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_recommended_info` (
  `ID` varchar(32) NOT NULL,
  `OperatorID` varchar(32) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  `BondKey` varchar(25) NOT NULL,
  `ListMarket` varchar(3) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_tend_flag_list`
--

DROP TABLE IF EXISTS `bond_tend_flag_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_tend_flag_list` (
  `BondKey` varchar(25) NOT NULL,
  `ListedMarket` varchar(25) NOT NULL,
  `Flag` varchar(1) NOT NULL,
  `ModifiedTime` datetime NOT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`BondKey`,`UnderwriterID`,`ListedMarket`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='债券标书使用标志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_tender_custom`
--

DROP TABLE IF EXISTS `bond_tender_custom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_tender_custom` (
  `UnderwriterID` varchar(32) NOT NULL,
  `BondKey` varchar(25) NOT NULL,
  `ListedMarket` varchar(6) NOT NULL,
  `UpperLimit` varchar(16) NOT NULL,
  `LowerLimit` varchar(16) NOT NULL,
  `Step` varchar(16) NOT NULL,
  `DelFlag` char(3) DEFAULT NULL,
  `OperatorID` varchar(32) NOT NULL,
  `ModifiedTime` datetime DEFAULT NULL,
  PRIMARY KEY (`BondKey`,`ListedMarket`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='自定义标书设置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bond_tender_detail_custom`
--

DROP TABLE IF EXISTS `bond_tender_detail_custom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bond_tender_detail_custom` (
  `Id` varchar(64) NOT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  `BondKey` varchar(25) NOT NULL,
  `ListedMarket` varchar(3) NOT NULL,
  `Price` varchar(16) NOT NULL,
  `Rate` varchar(16) NOT NULL,
  `DelFlag` char(1) DEFAULT NULL,
  `OperatorID` varchar(32) DEFAULT NULL,
  `ModifiedTime` datetime DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='保存计算出的标书详情(价格与参考收益率)。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `client_account`
--

DROP TABLE IF EXISTS `client_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_account` (
  `full_name` varchar(200) NOT NULL COMMENT '客户名称',
  `short_name` varchar(200) DEFAULT NULL COMMENT '客户简称',
  `institution` varchar(200) DEFAULT NULL COMMENT '所属机构',
  `cdc_account_name` varchar(200) DEFAULT NULL COMMENT '中债登账户名称',
  `cdc_account` varchar(20) DEFAULT NULL COMMENT '中债登账号',
  `sch_account_name` varchar(200) DEFAULT NULL COMMENT '清算所账户名称',
  `sch_account` varchar(20) DEFAULT NULL COMMENT '清算所账号',
  `hengtai_full_name` varchar(200) DEFAULT NULL COMMENT '对应机构',
  `hengtai_short_name` varchar(200) DEFAULT NULL COMMENT '对应机构简称',
  `trade_name` varchar(20) DEFAULT NULL COMMENT '交易员',
  `crm_full_name` varchar(200) DEFAULT NULL COMMENT 'CRM对应机构',
  `crm_short_name` varchar(20) DEFAULT NULL COMMENT 'CRM对应简称',
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`full_name`,`UnderwriterID`),
  KEY `crm_full_name` (`crm_full_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crm_hengtai_relation`
--

DROP TABLE IF EXISTS `crm_hengtai_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `crm_hengtai_relation` (
  `crm_short_name_cn` varchar(255) NOT NULL COMMENT 'crm的机构简称',
  `crm_full_name_cn` varchar(255) DEFAULT NULL COMMENT 'crm的机构全称',
  `hengtai_short_name_cn` varchar(255) DEFAULT NULL COMMENT '对应恒泰机构简称',
  `hengtai_full_name_cn` varchar(255) DEFAULT NULL COMMENT '对应恒泰机构全称',
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`crm_short_name_cn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_contact_info`
--

DROP TABLE IF EXISTS `idb_contact_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_contact_info` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `UnderwriterID` varchar(32) NOT NULL,
  `ContactMsg` varchar(256) DEFAULT NULL COMMENT '联系信息',
  `QRCodeMsg` varchar(256) DEFAULT NULL COMMENT '生成二维码的信息',
  `UserIDList` varchar(1024) DEFAULT NULL COMMENT '宏源方被选中的联系人ID列表',
  `TraderID` varchar(32) DEFAULT NULL COMMENT '当前操作员ID',
  `OTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '最后修改时间',
  PRIMARY KEY (`ID`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='前台联系方式';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `market_stream`
--

DROP TABLE IF EXISTS `market_stream`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `market_stream` (
  `Sequence` int(11) unsigned NOT NULL COMMENT 'Market Stream序号',
  `Type` tinyint(1) DEFAULT NULL COMMENT '记录类型，0:无效；1:债券信息变更；2:报价信息变更',
  `BondKey` varchar(64) NOT NULL COMMENT '债券key',
  `ListedMarket` varchar(8) NOT NULL COMMENT '发行市场',
  `QuoteUUID` varchar(32) DEFAULT NULL COMMENT '报价UUID，如果不是报价相关记录则此字段为空',
  `Content` varchar(4000) DEFAULT NULL COMMENT 'Market Stream JSON内容',
  `TriggerMsgType` varchar(32) DEFAULT NULL COMMENT '触发该条日志的消息号',
  `CreateTime` datetime DEFAULT NULL COMMENT '生成记录的时间',
  `ConfirmFlag` tinyint(1) DEFAULT NULL COMMENT '是否被确认，0:未确认；1:已确认（是否有人读过）',
  `ConfirmID` varchar(32) DEFAULT NULL COMMENT '确认人ID',
  `ConfirmTime` datetime DEFAULT NULL COMMENT '确认时间',
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`Sequence`,`UnderwriterID`),
  KEY `Index_BondKey_ListedMarket` (`BondKey`,`ListedMarket`) USING BTREE,
  KEY `Index_CreateTime` (`CreateTime`) USING BTREE,
  KEY `Index_ConfirmID` (`ConfirmID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `push_info`
--

DROP TABLE IF EXISTS `push_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `push_info` (
  `BondKeyListedMarket` varchar(32) NOT NULL COMMENT 'BondKey + ListedMarket + "-" + UnderwriterID',
  `BondKey` varchar(25) DEFAULT NULL,
  `ListedMarket` varchar(3) DEFAULT NULL,
  `PushMode` varchar(1) DEFAULT NULL COMMENT '推送方式，0：全量；1：白名单；2：黑名单（排除模式）',
  `PushList` varchar(1350) DEFAULT NULL COMMENT '推送机构列表，以“|”分隔。具体内容是黑名单还是白名单要看m_PushMode的设定',
  `OperatorID` varchar(25) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`BondKeyListedMarket`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `push_info_history`
--

DROP TABLE IF EXISTS `push_info_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `push_info_history` (
  `ID` varchar(32) NOT NULL,
  `BondKeyListedMarket` varchar(30) DEFAULT NULL,
  `BondKey` varchar(25) DEFAULT NULL,
  `ListedMarket` varchar(3) DEFAULT NULL,
  `PushMode` varchar(1) DEFAULT NULL COMMENT '推送方式，0：全量；1：白名单；2：黑名单（排除模式）',
  `PushList` varchar(1350) DEFAULT NULL COMMENT '推送机构列表，以“|”分隔。具体内容是黑名单还是白名单要看m_PushMode的设定',
  `OperatorID` varchar(25) DEFAULT NULL,
  `UnderwriterID` varchar(32) DEFAULT NULL,
  `TimeStamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_account`
--

DROP TABLE IF EXISTS `user_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_account` (
  `id` varchar(32) NOT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  `institution_id` int(20) unsigned DEFAULT NULL COMMENT '所属机构',
  `name_cn` varchar(63) NOT NULL COMMENT '姓名',
  `mobile` varchar(30) DEFAULT NULL COMMENT '手机',
  `email` varchar(128) DEFAULT NULL COMMENT '邮箱地址',
  `tel` varchar(30) DEFAULT NULL COMMENT '固话',
  `qq` varchar(30) DEFAULT NULL COMMENT 'QQ',
  `Flag` varchar(10) DEFAULT NULL,
  `LastModifyTime` datetime DEFAULT NULL,
  `Sequence` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='联系人';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_behavior_fee_rate`
--

DROP TABLE IF EXISTS `user_behavior_fee_rate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_behavior_fee_rate` (
  `BondKey` varchar(25) NOT NULL,
  `ListMarket` varchar(3) NOT NULL,
  `BidQuantity` decimal(20,4) DEFAULT NULL COMMENT '无主承销商的中标量',
  `FeeRate` decimal(20,4) DEFAULT NULL COMMENT '无主承销商的手续费率',
  `SumFee` decimal(20,4) DEFAULT NULL COMMENT '有主承销商的手续费和',
  `SumBidQuantity` decimal(20,4) DEFAULT NULL COMMENT '有主承销商的中标总量(承销团总中标量)',
  `AvgFeeRate` decimal(20,4) DEFAULT NULL COMMENT 'SumFee/SumBidQuantity',
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`BondKey`,`ListMarket`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_behavior_statistics`
--

DROP TABLE IF EXISTS `user_behavior_statistics`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_behavior_statistics` (
  `ID` varchar(32) NOT NULL,
  `BondKey` varchar(25) NOT NULL,
  `ShortName` varchar(128) DEFAULT NULL,
  `MarketType` varchar(1) DEFAULT NULL,
  `BidCoupon` varchar(12) DEFAULT NULL,
  `Vol` int(11) DEFAULT NULL COMMENT '申购量(万)',
  `QTime` datetime DEFAULT NULL COMMENT '申购时间',
  `OutcomeNetPrice` decimal(20,4) DEFAULT NULL COMMENT '中标净价',
  `OutcomeQuantity` int(11) DEFAULT NULL COMMENT '中标量',
  `Commission` decimal(20,4) DEFAULT NULL COMMENT '手续费',
  `Fee` decimal(20,4) DEFAULT NULL,
  `QuotStatus` varchar(2) DEFAULT NULL COMMENT '//--状态说明：(宁波银行的需求中：信用债没有“已投标”状态，信用债和利率债均无“已分配”状态，以“等待分配审批”代替。分配流程所有债券均需要审批。)\r\n	//--0:未投标\r\n	//--1:等待分配（申购审批通过也变为此状态）\r\n	//--2:已分配\r\n	//--3:撤销分配\r\n	//--4:撤销投标\r\n	//--5:等待投标审批\r\n	//--6:投标审批中\r\n	//--7:投标审批未通过\r\n	//--8:等待分配审批\r\n	//--9:分配审批中\r\n	//--10:分配审批未通过\r\n	//--11:分配审批通过\r\n	//--实际的状态变更流程（排序时的顺序）如下：\r\n	//--0:未投标 < 4:撤销投标 < 5:等待投标审批 < 6:投标审批中 < 7:投标审批未通过 < 1:等待分配 < 8:等待分配审批 < 9:分配审批中< 3:撤销分配 < 10:分配审批未通过 < 11:分配审批通过 < 2:已分配',
  `InterestStartDate` varchar(8) DEFAULT NULL COMMENT '起息日',
  `BondSubtype` varchar(3) DEFAULT NULL COMMENT '债券类型',
  `IssuerRating` varchar(5) DEFAULT NULL COMMENT '主体评级',
  `MaturityTerm` varchar(6) DEFAULT NULL COMMENT '债券期限',
  `CouponRateCurrent` decimal(8,2) DEFAULT NULL COMMENT '	债券收益',
  `InstitutionSubtype` varchar(3) DEFAULT NULL COMMENT '企业性质',
  `BCCompanyID` varchar(32) DEFAULT NULL COMMENT '客户ID',
  `CustomerFullName` varchar(255) DEFAULT NULL COMMENT '客户全称',
  `WebBondType` varchar(10) DEFAULT NULL COMMENT '网页上用的债券种类',
  `WebTerm` varchar(10) DEFAULT NULL COMMENT '网页上用的债券期限',
  `WebTerm1` varchar(10) DEFAULT NULL COMMENT '另一种网页上用的债券期限',
  `WebCouponRateCurrent` varchar(10) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`ID`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_bond_favorite`
--

DROP TABLE IF EXISTS `user_bond_favorite`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_bond_favorite` (
  `UserID` varchar(32) NOT NULL COMMENT '用户ID',
  `FavoriteBonds` varchar(16384) DEFAULT NULL,
  `UnderwriterID` varchar(32) NOT NULL,
  PRIMARY KEY (`UserID`,`UnderwriterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='我的关注债券列表';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:29:44
