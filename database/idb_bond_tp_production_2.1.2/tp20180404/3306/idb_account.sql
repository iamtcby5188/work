-- MySQL dump 10.13  Distrib 5.5.49, for Linux (x86_64)
--
-- Host: localhost    Database: idb_account
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `auth_group`
--

DROP TABLE IF EXISTS `auth_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(80) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `auth_group_permissions`
--

DROP TABLE IF EXISTS `auth_group_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_group_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `group_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `group_id` (`group_id`,`permission_id`),
  KEY `auth_group_permissions_0e939a4f` (`group_id`),
  KEY `auth_group_permissions_8373b171` (`permission_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `auth_permission`
--

DROP TABLE IF EXISTS `auth_permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_permission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `content_type_id` int(11) NOT NULL,
  `codename` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `content_type_id` (`content_type_id`,`codename`),
  KEY `auth_permission_417f1b1c` (`content_type_id`)
) ENGINE=MyISAM AUTO_INCREMENT=382 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `auth_user`
--

DROP TABLE IF EXISTS `auth_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `password` varchar(128) NOT NULL,
  `last_login` datetime NOT NULL,
  `is_superuser` tinyint(1) NOT NULL,
  `username` varchar(30) NOT NULL,
  `first_name` varchar(30) NOT NULL,
  `last_name` varchar(30) NOT NULL,
  `email` varchar(75) NOT NULL,
  `is_staff` tinyint(1) NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `date_joined` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `auth_user_groups`
--

DROP TABLE IF EXISTS `auth_user_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user_groups` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id` (`user_id`,`group_id`),
  KEY `auth_user_groups_e8701ad4` (`user_id`),
  KEY `auth_user_groups_0e939a4f` (`group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `auth_user_user_permissions`
--

DROP TABLE IF EXISTS `auth_user_user_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user_user_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id` (`user_id`,`permission_id`),
  KEY `auth_user_user_permissions_e8701ad4` (`user_id`),
  KEY `auth_user_user_permissions_8373b171` (`permission_id`)
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
  `node_name` varchar(512) DEFAULT NULL,
  `start_time` datetime DEFAULT NULL,
  `end_time` datetime DEFAULT NULL,
  `executed` int(8) DEFAULT NULL,
  `key_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
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
-- Table structure for table `django_admin_log`
--

DROP TABLE IF EXISTS `django_admin_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_admin_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `action_time` datetime NOT NULL,
  `object_id` longtext,
  `object_repr` varchar(200) NOT NULL,
  `action_flag` smallint(5) unsigned NOT NULL,
  `change_message` longtext NOT NULL,
  `content_type_id` int(11) DEFAULT NULL,
  `user_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `django_admin_log_417f1b1c` (`content_type_id`),
  KEY `django_admin_log_e8701ad4` (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `django_content_type`
--

DROP TABLE IF EXISTS `django_content_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_content_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `app_label` varchar(100) NOT NULL,
  `model` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `django_content_type_app_label_45f3b1d93ec8c61c_uniq` (`app_label`,`model`)
) ENGINE=MyISAM AUTO_INCREMENT=128 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `django_migrations`
--

DROP TABLE IF EXISTS `django_migrations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_migrations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `app` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `applied` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `django_session`
--

DROP TABLE IF EXISTS `django_session`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_session` (
  `session_key` varchar(40) NOT NULL,
  `session_data` longtext NOT NULL,
  `expire_date` datetime NOT NULL,
  PRIMARY KEY (`session_key`),
  KEY `django_session_de54fa62` (`expire_date`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_account`
--

DROP TABLE IF EXISTS `idb_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_account` (
  `ID` varchar(32) NOT NULL,
  `COMPANY_ID` varchar(32) DEFAULT NULL COMMENT '公司id',
  `ACCOUNT_CODE` varchar(32) DEFAULT NULL,
  `USERNAME` varchar(128) DEFAULT NULL COMMENT '用户名 一般为email',
  `PASSWORD` varchar(128) DEFAULT NULL COMMENT '密码',
  `DISPLAY_NAME` varchar(32) DEFAULT NULL COMMENT '显示名（昵称）',
  `MOBILE` varchar(32) DEFAULT NULL COMMENT '手机',
  `TELEPHONE` varchar(32) DEFAULT NULL COMMENT '电话',
  `EMAIL` varchar(128) DEFAULT NULL COMMENT 'email',
  `RM` varchar(128) DEFAULT NULL COMMENT 'rm',
  `MSN` varchar(128) DEFAULT NULL COMMENT 'msn',
  `QQ` varchar(128) DEFAULT NULL COMMENT 'qq',
  `ADDRESS` varchar(256) DEFAULT NULL COMMENT '通讯地址',
  `ACCOUNT_TYPE` char(1) DEFAULT NULL COMMENT '1:Broker\r\n2:Customer',
  `IS_FORBIDDEN` char(1) DEFAULT NULL COMMENT '1:Normal\r\n2:Forbidden\r\n 3:Deleted',
  `CREATE_TIME` datetime DEFAULT NULL,
  `MODIFY_TIME` datetime DEFAULT NULL,
  `CREATE_BY` varchar(32) DEFAULT NULL,
  `MODIFY_BY` varchar(32) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL COMMENT '1:Valid\r\n            2:Invalid',
  `COMPANY_NAME` varchar(128) DEFAULT NULL COMMENT '公司名',
  `INIT_PASSWORD` varchar(128) DEFAULT NULL COMMENT '初始密码',
  `IS_ACTIVATE` char(1) DEFAULT NULL COMMENT '是否激活',
  `CN_NAME` varchar(32) DEFAULT NULL COMMENT '中文名',
  `CN_FIRST_NAME` varchar(32) DEFAULT NULL COMMENT '中文姓',
  `EN_NAME` varchar(32) DEFAULT NULL COMMENT '英文名',
  `EN_FIRST_NAME` varchar(32) DEFAULT NULL COMMENT '英文姓',
  `SEX` char(1) DEFAULT NULL COMMENT '性别\r\n1.男\r\n2.女',
  `CTIY` varchar(45) DEFAULT NULL COMMENT '城市',
  `DEPT` varchar(45) DEFAULT NULL COMMENT '部门',
  `TITLE` varchar(45) DEFAULT NULL COMMENT '头衔',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_account_head_image`
--

DROP TABLE IF EXISTS `idb_account_head_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_account_head_image` (
  `ID` varchar(32) NOT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `CRC` varchar(128) DEFAULT NULL,
  `IMAGE_DATA` blob,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_account_limit`
--

DROP TABLE IF EXISTS `idb_account_limit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_account_limit` (
  `ID` varchar(32) NOT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `DEPARTMENT_CODE` varchar(32) DEFAULT NULL,
  `MAX_ONLINE_SIZE` int(4) DEFAULT NULL,
  `MAX_MACHINE_SIZE` int(4) DEFAULT NULL,
  `START_TIME` datetime DEFAULT NULL,
  `END_TIME` datetime DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_account_machine_reg`
--

DROP TABLE IF EXISTS `idb_account_machine_reg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_account_machine_reg` (
  `ID` varchar(32) NOT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `USERNAME` varchar(128) DEFAULT NULL,
  `MACHINE_ID` varchar(128) DEFAULT NULL,
  `MACHINE_INDEX` int(4) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `DELETE_TIME` datetime DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_account_role_relation`
--

DROP TABLE IF EXISTS `idb_account_role_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_account_role_relation` (
  `ID` varchar(32) NOT NULL,
  `ROLE_ID` varchar(32) DEFAULT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `DEPARTMENT_ID` varchar(32) DEFAULT NULL,
  `DEPARTMENT_CODE` varchar(32) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `MODIFY_TIME` datetime DEFAULT NULL,
  `CREATE_BY` varchar(32) DEFAULT NULL,
  `MODIFY_BY` varchar(32) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_calendar_event`
--

DROP TABLE IF EXISTS `idb_calendar_event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_calendar_event` (
  `ID` varchar(32) NOT NULL,
  `TYPE` char(1) DEFAULT NULL,
  `YEAR` int(5) DEFAULT NULL,
  `MONTH` int(2) DEFAULT NULL,
  `DATE_IN_MONTH` int(2) DEFAULT NULL,
  `HOURS` int(2) DEFAULT NULL,
  `MINUTES` int(2) DEFAULT NULL,
  `DAY` int(1) DEFAULT NULL,
  `WEEK_SIZE` int(2) DEFAULT NULL,
  `NAME` varchar(32) DEFAULT NULL,
  `INFO` varchar(255) DEFAULT NULL,
  `LEVEL` char(1) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_company`
--

DROP TABLE IF EXISTS `idb_company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_company` (
  `ID` varchar(32) NOT NULL,
  `NAME` varchar(128) DEFAULT NULL,
  `CODE` varchar(32) DEFAULT NULL,
  `EN_NAME` varchar(128) DEFAULT NULL,
  `ADDRESS` varchar(256) DEFAULT NULL,
  `TELEPHOME` varchar(32) DEFAULT NULL,
  `CONTACT` varchar(32) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_constants`
--

DROP TABLE IF EXISTS `idb_constants`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_constants` (
  `ID` varchar(32) NOT NULL,
  `CONSTANT_KEY` varchar(32) DEFAULT NULL,
  `CONSTANT_VALUE` varchar(32) DEFAULT NULL,
  `DESCRIPTION` varchar(255) DEFAULT NULL,
  `EXTEND1` varchar(255) DEFAULT NULL,
  `EXTEND2` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_country`
--

DROP TABLE IF EXISTS `idb_country`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_country` (
  `ID` varchar(32) NOT NULL,
  `NAME` varchar(32) DEFAULT NULL,
  `NAME_CN` varchar(32) DEFAULT NULL,
  `DOMAIN` varchar(32) DEFAULT NULL,
  `AREA_CODE` varchar(32) DEFAULT NULL,
  `TIMEZONE` int(3) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_country_calendar_event`
--

DROP TABLE IF EXISTS `idb_country_calendar_event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_country_calendar_event` (
  `ID` varchar(32) NOT NULL,
  `CALENDAR_EVENT_ID` varchar(32) DEFAULT NULL,
  `COUNTRY_ID` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_department`
--

DROP TABLE IF EXISTS `idb_department`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_department` (
  `ID` varchar(32) NOT NULL,
  `DEPARTMENT_CODE` varchar(32) DEFAULT NULL,
  `NAME` varchar(32) DEFAULT NULL,
  `DESCRIPTION` varchar(255) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_error_log_record`
--

DROP TABLE IF EXISTS `idb_error_log_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_error_log_record` (
  `ID` varchar(32) NOT NULL,
  `ERROR_LOG_ID` varchar(32) DEFAULT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `USERNAME` varchar(32) DEFAULT NULL,
  `CODE` varchar(32) DEFAULT NULL,
  `CLIENT_IP` varchar(32) DEFAULT NULL,
  `CLIENT_OS` varchar(32) DEFAULT NULL,
  `CLIENT_TYPE` varchar(32) DEFAULT NULL,
  `DEPARTMENT_CODE` varchar(32) DEFAULT NULL,
  `TOKEN` varchar(32) DEFAULT NULL,
  `CUSTOM_INFO` varchar(1000) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_feedback`
--

DROP TABLE IF EXISTS `idb_feedback`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_feedback` (
  `ID` varchar(32) NOT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `USERNAME` varchar(32) DEFAULT NULL,
  `CLIENT_IP` varchar(32) DEFAULT NULL,
  `CLIENT_OS` varchar(32) DEFAULT NULL,
  `CLIENT_TYPE` varchar(32) DEFAULT NULL,
  `DEPARTMENT_CODE` varchar(32) DEFAULT NULL,
  `FEEDBACK_CONTENT` varchar(1000) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
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
-- Table structure for table `idb_financial_company_relation`
--

DROP TABLE IF EXISTS `idb_financial_company_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_financial_company_relation` (
  `ID` varchar(32) NOT NULL,
  `DEPARTMENT_ID` varchar(32) DEFAULT NULL,
  `FINANCIAL_COMPANY_ID` varchar(32) DEFAULT NULL,
  `MAPPING_ID` varchar(32) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `MODIFY_TIME` datetime DEFAULT NULL,
  `CREATE_BY` varchar(32) DEFAULT NULL,
  `MODIFY_BY` varchar(32) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
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
-- Table structure for table `idb_ftp_20160519`
--

DROP TABLE IF EXISTS `idb_ftp_20160519`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_ftp_20160519` (
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_login_log`
--

DROP TABLE IF EXISTS `idb_login_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_login_log` (
  `ID` varchar(32) NOT NULL,
  `LOGIN_STATUS_ID` varchar(32) DEFAULT NULL,
  `ACCOUNT_ID` varchar(32) DEFAULT NULL,
  `USERNAME` varchar(128) DEFAULT NULL,
  `DISPLAY_NAME` varchar(32) DEFAULT NULL,
  `CLIENT_TYPE` char(1) DEFAULT NULL,
  `IP` varchar(50) DEFAULT NULL,
  `START_TIME` datetime DEFAULT NULL,
  `END_TIME` datetime DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_role`
--

DROP TABLE IF EXISTS `idb_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_role` (
  `ID` varchar(32) NOT NULL,
  `NAME` varchar(32) DEFAULT NULL,
  `DESCRIPTION` varchar(255) DEFAULT NULL,
  `LEVEL` int(8) DEFAULT NULL,
  `ACCOUNT_TYPE` char(1) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
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
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `idb_user`
--

DROP TABLE IF EXISTS `idb_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idb_user` (
  `ID` varchar(32) NOT NULL,
  `USERNAME` varchar(32) DEFAULT NULL,
  `PASSWORD` varchar(32) DEFAULT NULL,
  `CODE` varchar(32) DEFAULT NULL,
  `TYPE` varchar(32) DEFAULT NULL,
  `EMAIL` varchar(64) DEFAULT NULL,
  `EMAIL_BAT` varchar(64) DEFAULT NULL,
  `PHONE` varchar(32) DEFAULT NULL,
  `PHONE_BAT` varchar(32) DEFAULT NULL,
  `STATUS` char(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
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
-- Table structure for table `job_task_20160519`
--

DROP TABLE IF EXISTS `job_task_20160519`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `job_task_20160519` (
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='InnoDB free: 21504 kB';
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='InnoDB free: 21504 kB';
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='InnoDB free: 21504 kB';
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='InnoDB free: 21504 kB';
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='InnoDB free: 21504 kB';
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:22:58
