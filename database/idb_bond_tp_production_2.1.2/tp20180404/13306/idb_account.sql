-- MySQL dump 10.13  Distrib 5.6.22, for linux-glibc2.5 (x86_64)
--
-- Host: 192.168.162.110    Database: idb_account
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
-- Table structure for table `idb_account`
--

DROP TABLE IF EXISTS `idb_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_account` (
  `ID` varchar(32) NOT NULL,
  `COMPANY_ID` varchar(32) DEFAULT NULL COMMENT '公司id',
  `COMPANY_NAME` varchar(128) DEFAULT NULL COMMENT '公司名',
  `ACCOUNT_CODE` varchar(32) DEFAULT NULL COMMENT '员工代码',
  `USERNAME` varchar(128) DEFAULT NULL COMMENT '用户名 一般为email',
  `PASSWORD` varchar(128) DEFAULT NULL COMMENT '密码',
  `INIT_PASSWORD` varchar(128) DEFAULT NULL COMMENT '初始密码',
  `DISPLAY_NAME` varchar(32) DEFAULT NULL COMMENT '显示名（昵称）',
  `MOBILE` varchar(32) DEFAULT NULL COMMENT '手机',
  `TELEPHONE` varchar(32) DEFAULT NULL COMMENT '电话',
  `EMAIL` varchar(128) DEFAULT NULL COMMENT 'email',
  `RM` varchar(128) DEFAULT NULL COMMENT 'rm',
  `MSN` varchar(128) DEFAULT NULL COMMENT 'msn',
  `QQ` varchar(128) DEFAULT NULL COMMENT 'qq',
  `ADDRESS` varchar(256) DEFAULT NULL COMMENT '通讯地址',
  `ACCOUNT_TYPE` char(1) DEFAULT NULL COMMENT '1:Broker\n2:Customer',
  `IS_FORBIDDEN` char(1) DEFAULT NULL COMMENT '1:Normal\n2:Forbidden\n 3:Deleted',
  `CREATE_TIME` datetime DEFAULT NULL COMMENT '创建时间',
  `MODIFY_TIME` datetime DEFAULT NULL COMMENT '修改时间',
  `CREATE_BY` varchar(32) DEFAULT NULL COMMENT '创建者',
  `MODIFY_BY` varchar(32) DEFAULT NULL COMMENT '修改者',
  `IS_ACTIVATE` char(1) DEFAULT NULL COMMENT '是否激活',
  `STATUS` char(1) DEFAULT NULL COMMENT '1:Valid\r\n            2:Invalid',
  `CN_NAME` varchar(32) DEFAULT NULL COMMENT '中文名',
  `CN_FIRST_NAME` varchar(32) DEFAULT NULL COMMENT '中文姓',
  `EN_NAME` varchar(32) DEFAULT NULL COMMENT '英文名',
  `EN_FIRST_NAME` varchar(32) DEFAULT NULL COMMENT '英文姓',
  `SEX` char(1) DEFAULT NULL COMMENT '性别\n1.男\n2.女',
  `CTIY` varchar(45) DEFAULT NULL COMMENT '城市',
  `DEPT` varchar(45) DEFAULT NULL COMMENT '部门',
  `TITLE` varchar(45) DEFAULT NULL COMMENT '头衔',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='帐号表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_account_role_relation`
--

DROP TABLE IF EXISTS `idb_account_role_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_account_role_relation` (
  `ID` varchar(32) NOT NULL,
  `ROLE_ID` varchar(32) DEFAULT NULL COMMENT '角色ID',
  `ACCOUNT_ID` varchar(32) DEFAULT NULL COMMENT '帐号ID',
  `DEPARTMENT_ID` varchar(32) DEFAULT NULL COMMENT '部门ID',
  `DEPARTMENT_CODE` varchar(32) DEFAULT NULL COMMENT '部门代码',
  `CREATE_TIME` datetime DEFAULT NULL COMMENT '创建时间',
  `MODIFY_TIME` datetime DEFAULT NULL COMMENT '修改时间',
  `CREATE_BY` varchar(32) DEFAULT NULL COMMENT '创建者',
  `MODIFY_BY` varchar(32) DEFAULT NULL COMMENT '修改者',
  `STATUS` char(1) DEFAULT NULL COMMENT '1:Valid 2:Invalid',
  PRIMARY KEY (`ID`),
  KEY `index` (`DEPARTMENT_CODE`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='业务关系表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_company`
--

DROP TABLE IF EXISTS `idb_company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_company` (
  `ID` varchar(32) NOT NULL,
  `NAME` varchar(128) DEFAULT NULL COMMENT '公司名',
  `CODE` varchar(32) DEFAULT NULL COMMENT '公司代码',
  `EN_NAME` varchar(128) DEFAULT NULL COMMENT '英文名',
  `ADDRESS` varchar(256) DEFAULT NULL COMMENT '地址',
  `TELEPHOME` varchar(32) DEFAULT NULL COMMENT '电话',
  `CONTACT` varchar(32) DEFAULT NULL COMMENT '联系方式',
  `STATUS` char(1) DEFAULT NULL COMMENT '1:Valid 2:Invalid',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='公司表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_constants`
--

DROP TABLE IF EXISTS `idb_constants`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_constants` (
  `ID` varchar(32) NOT NULL,
  `CONSTANT_KEY` varchar(32) DEFAULT NULL COMMENT '常量代码',
  `CONSTANT_VALUE` varchar(32) DEFAULT NULL COMMENT '常量值',
  `DESCRIPTION` varchar(255) DEFAULT NULL COMMENT '说明',
  `EXTEND1` varchar(255) DEFAULT NULL COMMENT '扩展值1',
  `EXTEND2` varchar(255) DEFAULT NULL COMMENT '扩展值2',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='常量表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_country`
--

DROP TABLE IF EXISTS `idb_country`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_country` (
  `ID` varchar(32) NOT NULL,
  `NAME` varchar(32) DEFAULT NULL COMMENT '名称',
  `NAME_CN` varchar(32) DEFAULT NULL COMMENT '中文名',
  `DOMAIN` varchar(32) DEFAULT NULL COMMENT '识别码',
  `AREA_CODE` varchar(32) DEFAULT NULL COMMENT '区域码',
  `TIMEZONE` int(3) DEFAULT NULL COMMENT '时区',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='国家表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_department`
--

DROP TABLE IF EXISTS `idb_department`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_department` (
  `ID` varchar(32) NOT NULL,
  `DEPARTMENT_CODE` varchar(32) DEFAULT NULL COMMENT '部门代码',
  `NAME` varchar(32) DEFAULT NULL COMMENT '名称',
  `DESCRIPTION` varchar(255) DEFAULT NULL COMMENT '说明',
  `STATUS` char(1) DEFAULT NULL COMMENT '1:Valid 2:Invalid',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='业务部门表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_financial_company`
--

DROP TABLE IF EXISTS `idb_financial_company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_financial_company` (
  `ID` varchar(32) NOT NULL,
  `NAME` varchar(128) DEFAULT NULL,
  `CODE` varchar(32) DEFAULT NULL,
  `NAME_EN` varchar(128) DEFAULT NULL,
  `FULL_NAME` varchar(255) DEFAULT NULL,
  `FULL_NAME_EN` varchar(255) DEFAULT NULL,
  `BANK_TYPE` char(1) DEFAULT NULL,
  `CITY` varchar(64) DEFAULT NULL,
  `CITY_NAME` varchar(64) DEFAULT NULL,
  `PINYIN` varchar(255) DEFAULT NULL,
  `PINYIN_FULL` varchar(1000) DEFAULT NULL,
  `DESCRIPTION` varchar(255) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_login_log`
--

DROP TABLE IF EXISTS `idb_login_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_login_log` (
  `ID` varchar(32) NOT NULL,
  `LOGIN_STATUS_ID` varchar(32) DEFAULT NULL COMMENT '登陆状态ID',
  `ACCOUNT_ID` varchar(32) DEFAULT NULL COMMENT '帐号ID',
  `USERNAME` varchar(128) DEFAULT NULL COMMENT '用户名',
  `DISPLAY_NAME` varchar(32) DEFAULT NULL COMMENT '显示名',
  `CLIENT_TYPE` char(1) DEFAULT NULL COMMENT '用户类型',
  `CLIENT_VERSION` varchar(32) DEFAULT NULL,
  `MAC` varchar(128) DEFAULT NULL,
  `IP` varchar(50) DEFAULT NULL COMMENT 'IPAddress',
  `START_TIME` datetime DEFAULT NULL COMMENT '登陆时间',
  `END_TIME` datetime DEFAULT NULL COMMENT '退出时间',
  `STATUS` char(1) DEFAULT NULL COMMENT '1:Valid 2:Invalid',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='登陆日志表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_login_status`
--

DROP TABLE IF EXISTS `idb_login_status`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_login_status` (
  `ID` varchar(32) NOT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `TOKEN` varchar(32) DEFAULT NULL,
  `ACTIVATION` char(1) DEFAULT NULL,
  `CLIENT_TYPE` char(1) DEFAULT NULL,
  `IP` varchar(50) DEFAULT NULL,
  `SERVER` varchar(32) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
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

-- Dump completed on 2018-04-04 12:28:22
