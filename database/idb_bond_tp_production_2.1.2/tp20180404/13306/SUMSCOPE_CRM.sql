-- MySQL dump 10.13  Distrib 5.6.22, for linux-glibc2.5 (x86_64)
--
-- Host: 192.168.162.110    Database: SUMSCOPE_CRM
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
-- Table structure for table `TB_DEFAULT_CONFIG`
--

DROP TABLE IF EXISTS `TB_DEFAULT_CONFIG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_DEFAULT_CONFIG` (
  `ID` int(11) NOT NULL,
  `CRM_LOGIN_ADDR` varchar(255) DEFAULT NULL,
  `TEMP_INST_ADDR` varchar(255) DEFAULT NULL,
  `CLIENT_ANALYSIS_ADDR` varchar(255) DEFAULT NULL,
  `TEMP_INST_PAGE_WIDTH` double DEFAULT NULL,
  `TEMP_INST_PAGE_HEIGHT` double DEFAULT NULL,
  `DEFAULT_LOGO_URL_PORT` int(6) DEFAULT NULL,
  `EDIT_BOND_TYPE` varchar(1) DEFAULT NULL COMMENT '1 允许编辑债券类别 0 不允许',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_FUNCTION_LIST`
--

DROP TABLE IF EXISTS `TB_FUNCTION_LIST`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_FUNCTION_LIST` (
  `FUNC_ID` int(10) NOT NULL AUTO_INCREMENT COMMENT '功能编号',
  `FUNC_DEF` varchar(50) NOT NULL COMMENT '功能定义',
  `FUNC_DES` varchar(200) NOT NULL COMMENT '功能描述',
  `FUNC_PARENT_ID` int(10) DEFAULT NULL COMMENT '父节点id。对应本表的FUNC_ID。',
  `FUNC_TYPE` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '功能类型。0：实际功能；1：功能分组',
  `FUNC_MUTEX` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '下属节点是否互斥。0：不互斥；1：互斥',
  `FUNC_REQUIRED` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '下属节点是否必选。0：无限制；1：至少选一个；2：必须全选',
  `FUNC_IS_VALID` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '功能是否生效',
  `FUNC_SORT` int(10) DEFAULT NULL,
  PRIMARY KEY (`FUNC_ID`),
  UNIQUE KEY `UN_FUNC_DEF` (`FUNC_DEF`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=280 DEFAULT CHARSET=utf8 COMMENT='一级分销客户端功能列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_HT_ORG`
--

DROP TABLE IF EXISTS `TB_HT_ORG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_HT_ORG` (
  `NAME` varchar(60) NOT NULL,
  `SHORT_NAME` varchar(60) DEFAULT NULL,
  `CREATE_TIME` datetime DEFAULT NULL,
  `ZZD_ACCOUNT_NAME` varchar(100) DEFAULT NULL,
  `ZZD_ACCOUNT` varchar(30) DEFAULT NULL,
  `QSS_ACCOUNT_NAME` varchar(100) DEFAULT NULL,
  `QSS_ACCOUNT` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`NAME`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_MENU`
--

DROP TABLE IF EXISTS `TB_MENU`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_MENU` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `MENU_NAME` varchar(12) DEFAULT NULL,
  `SORT` int(11) unsigned zerofill DEFAULT NULL,
  `P_ID` int(11) DEFAULT NULL COMMENT '父节点',
  `URL` varchar(50) DEFAULT NULL,
  `ICON` varchar(50) DEFAULT NULL,
  `AUTHORITY` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=99 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_MODEL`
--

DROP TABLE IF EXISTS `TB_MODEL`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_MODEL` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `NAME` varchar(100) CHARACTER SET latin1 DEFAULT NULL,
  `URL` varchar(50) CHARACTER SET latin1 DEFAULT NULL,
  `CRMID` int(11) DEFAULT NULL,
  `CREATE_TIME` date DEFAULT NULL,
  `DESCRIPTION` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `UNIN_NAME` (`NAME`,`CRMID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=229 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_PROJECT`
--

DROP TABLE IF EXISTS `TB_PROJECT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_PROJECT` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `NAME` varchar(50) DEFAULT NULL,
  `FILE_PATH` varchar(100) DEFAULT NULL,
  `UPLOAD_TYPE` varchar(2) DEFAULT NULL,
  `STATUS` varchar(1) DEFAULT NULL,
  `CREATE_TIME` date DEFAULT NULL,
  `FTP_URL` varchar(100) DEFAULT NULL,
  `FTP_PORT` int(4) DEFAULT NULL,
  `FTP_USER` varchar(20) DEFAULT NULL,
  `FTP_PASSWORD` varchar(10) DEFAULT NULL,
  `FTP_ROOT` varchar(100) DEFAULT NULL,
  `FILESERVER_IP` varchar(100) DEFAULT NULL,
  `FILESERVER_PORT` int(4) DEFAULT NULL,
  `FILESERVER_ROOT` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_ROLE`
--

DROP TABLE IF EXISTS `TB_ROLE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_ROLE` (
  `ID` int(3) NOT NULL AUTO_INCREMENT,
  `ROLE_NAME` varchar(12) DEFAULT NULL,
  `STATUS` int(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_ROLE_MENU`
--

DROP TABLE IF EXISTS `TB_ROLE_MENU`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_ROLE_MENU` (
  `ROLE_ID` int(3) NOT NULL,
  `MENU_ID` int(3) NOT NULL,
  PRIMARY KEY (`ROLE_ID`,`MENU_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_SYS_USER`
--

DROP TABLE IF EXISTS `TB_SYS_USER`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_SYS_USER` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `UID` varchar(20) NOT NULL,
  `NAME` varchar(20) DEFAULT NULL,
  `EMAIL` varchar(50) DEFAULT NULL,
  `PASSWORD` varchar(50) NOT NULL,
  `LAST_LOGIN_TIME` varchar(20) DEFAULT NULL,
  `STATUS` varchar(1) DEFAULT NULL,
  `IP` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `SYS_USER_UN` (`UID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_UPGRADE_INFO`
--

DROP TABLE IF EXISTS `TB_UPGRADE_INFO`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_UPGRADE_INFO` (
  `pkg_id` int(10) NOT NULL AUTO_INCREMENT,
  `pkg_title` varchar(64) DEFAULT NULL,
  `pkg_link` varchar(512) DEFAULT NULL,
  `pkg_dw_way` varchar(8) DEFAULT NULL,
  `pkg_memo` varchar(512) DEFAULT NULL,
  `pkg_ext` varchar(32) DEFAULT NULL,
  `dw_isforce` char(1) DEFAULT NULL,
  `pkg_size` int(11) DEFAULT NULL,
  `dw_model_code` varchar(32) DEFAULT NULL,
  `pkg_local_folder` varchar(256) DEFAULT NULL,
  `pkg_kill_processes` varchar(256) DEFAULT NULL,
  `pkg_generate` varchar(256) DEFAULT NULL,
  `delflag` char(1) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `project_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`pkg_id`)
) ENGINE=InnoDB AUTO_INCREMENT=895 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_USER_ROLE`
--

DROP TABLE IF EXISTS `TB_USER_ROLE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_USER_ROLE` (
  `USER_ID` int(11) NOT NULL,
  `ROLE_ID` int(3) NOT NULL,
  PRIMARY KEY (`USER_ID`,`ROLE_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TB_VERSION`
--

DROP TABLE IF EXISTS `TB_VERSION`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TB_VERSION` (
  `memo` varchar(255) DEFAULT NULL,
  `version_alias` varchar(255) DEFAULT NULL,
  `update_version` varchar(255) DEFAULT NULL,
  `version` varchar(255) NOT NULL DEFAULT '',
  `project_id` int(11) NOT NULL DEFAULT '0',
  `upload_user` varchar(10) DEFAULT NULL,
  `upload_time` date DEFAULT NULL,
  `file_size` int(11) DEFAULT NULL,
  `file_count` int(11) DEFAULT NULL,
  `status` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`version`,`project_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `USER_FUNCTION_CONFIG`
--

DROP TABLE IF EXISTS `USER_FUNCTION_CONFIG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `USER_FUNCTION_CONFIG` (
  `csId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CONFIG_ID` varchar(32) NOT NULL,
  `CONFIG_NAME` varchar(15) NOT NULL COMMENT '配置列表名称',
  `CONFIG_SET` varchar(5000) DEFAULT NULL COMMENT '功能配置列表',
  PRIMARY KEY (`csId`),
  KEY `suId` (`csId`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 COMMENT='功能配置列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `acId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  `acUid` varchar(64) NOT NULL COMMENT '登录邮箱',
  `acPasswordMd5` varchar(32) NOT NULL COMMENT '登录密码',
  `acNameCn` varchar(64) NOT NULL COMMENT '中文名',
  `acNameEn` varchar(64) NOT NULL DEFAULT '' COMMENT '英文名',
  `acRole` smallint(6) unsigned NOT NULL COMMENT '系统账户角色值',
  `acRoleName` varchar(64) NOT NULL DEFAULT '' COMMENT '账户角色名称',
  `acTeamIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '所属团队ID列表',
  `acManageDepIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '所属部门或管辖部门ID列表',
  `acDistributionSystemAuthIdList` varchar(128) NOT NULL DEFAULT '' COMMENT '分销系统操作权限ID列表',
  `acContactDeleteAuthAcTeamIdList` varchar(128) NOT NULL DEFAULT '' COMMENT '账户对团队下联系人的删除权限',
  `acContactModifyAuthAcTeamIdList` varchar(128) NOT NULL DEFAULT '' COMMENT '账户对团队下联系人的修改权限',
  `acContactSearchAuthAcTeamIdList` varchar(128) NOT NULL DEFAULT '' COMMENT '账户对团队下联系人的查看权限',
  `acNoteSearchAuthAcTeamIdList` varchar(128) NOT NULL DEFAULT '' COMMENT '账户对团队下笔记的查看权限',
  `acPosition` varchar(64) NOT NULL DEFAULT '' COMMENT '职务',
  `acMobilePhone` varchar(64) NOT NULL DEFAULT '' COMMENT '手机',
  `acSerialNumber` varchar(64) NOT NULL DEFAULT '' COMMENT '工号',
  `acFixedPhone` varchar(64) NOT NULL DEFAULT '' COMMENT '座机',
  `acContactCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '管理的联系人计数',
  `acAssignedNoteCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '指派给的笔记计数',
  `acSharedNoteCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '分享给的笔记计数',
  `acIPEnable` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'IP绑定是否启用',
  `acMacEnable` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'mac绑定是否启用',
  `acCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `acModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `acIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `updateflag` int(11) DEFAULT '0',
  PRIMARY KEY (`acId`,`CRMID`),
  KEY `acIsValid` (`acIsValid`) USING BTREE,
  KEY `acUid` (`acUid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=75 DEFAULT CHARSET=utf8 COMMENT='系统账户';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseAccountDepartment`
--

DROP TABLE IF EXISTS `baseAccountDepartment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseAccountDepartment` (
  `basacdepId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  `basacdepCode` varchar(64) NOT NULL COMMENT '部门代码',
  `basacdepName` varchar(64) NOT NULL COMMENT '部门名称',
  `basacdepNameSort` varchar(64) NOT NULL DEFAULT '' COMMENT '部门名称-排序',
  `basacdepTeamIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '包含团队ID列表',
  `basacdepCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '所属或管辖该部门计数',
  `basacdepCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `basacdepModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `basacdepIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  PRIMARY KEY (`basacdepId`,`CRMID`),
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE,
  KEY `basacdepIsValid` (`basacdepIsValid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='系统用户部门';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseAccountTeam`
--

DROP TABLE IF EXISTS `baseAccountTeam`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseAccountTeam` (
  `basactemId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `basactemCode` varchar(64) NOT NULL COMMENT '团队代码',
  `basactemName` varchar(64) NOT NULL COMMENT '团队名称',
  `basactemNameSort` varchar(64) NOT NULL DEFAULT '' COMMENT '团队名称-排序',
  `basactemCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '所属或管辖该团队计数',
  `basactemCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `basactemModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `basactemIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`basactemId`,`CRMID`),
  KEY `basactemIsValid` (`basactemIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='系统用户团队';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseCity`
--

DROP TABLE IF EXISTS `baseCity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseCity` (
  `basctyId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `basctyName` varchar(64) CHARACTER SET utf8 NOT NULL COMMENT '城市名称',
  `basctyNameSort` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '城市名称-排序',
  `basctyCityType` bigint(20) unsigned NOT NULL COMMENT '城市类型',
  `basctyCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该城市计数',
  `basctyCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `basctyModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `basctyIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`basctyId`,`CRMID`)
) ENGINE=InnoDB AUTO_INCREMENT=362 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseCityType`
--

DROP TABLE IF EXISTS `baseCityType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseCityType` (
  `bascityTId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `bascityTName` varchar(64) NOT NULL COMMENT '城市类型名称',
  `bascityTNameSort` varchar(64) NOT NULL DEFAULT '' COMMENT '城市类型名称-排序',
  `bascityTCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该城市类型计数',
  `bascityTCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `bascityTModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `bascityTIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`bascityTId`,`CRMID`),
  KEY `bascityTIsValid` (`bascityTIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8 COMMENT='城市类型';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseContactTag`
--

DROP TABLE IF EXISTS `baseContactTag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseContactTag` (
  `bascttId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `bascttName` varchar(64) NOT NULL COMMENT '联系人标签名称',
  `bascttNameSort` varchar(64) NOT NULL DEFAULT '' COMMENT '联系人标签名称-排序',
  `bascttCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该联系人标签计数',
  `bascttCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `bascttModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `bascttIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`bascttId`,`CRMID`),
  KEY `bascttIsValid` (`bascttIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='联系人标签';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseCustomerType`
--

DROP TABLE IF EXISTS `baseCustomerType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseCustomerType` (
  `bascstTId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `bascstTName` varchar(64) CHARACTER SET utf8 NOT NULL COMMENT '机构类型名称',
  `bascstTNameSort` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '机构类型-排序',
  `bascstTCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该机构类型计数',
  `bascstTCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `bascstTModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `bascstTIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`bascstTId`,`CRMID`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseDepartment`
--

DROP TABLE IF EXISTS `baseDepartment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseDepartment` (
  `basdepId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `basdepName` varchar(64) CHARACTER SET utf8 NOT NULL COMMENT '部门名称',
  `basdepNameSort` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '部门名称-排序',
  `basdepCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该部门计数',
  `basdepCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `basdepModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `basdepIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`basdepId`,`CRMID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseIndustry`
--

DROP TABLE IF EXISTS `baseIndustry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseIndustry` (
  `basindId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `basindName` varchar(64) CHARACTER SET utf8 NOT NULL COMMENT '行业名称',
  `basindNameSort` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '行业名称-排序',
  `basindCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该行业计数',
  `basindCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `basindModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `basindIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`basindId`,`CRMID`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `baseNoteTag`
--

DROP TABLE IF EXISTS `baseNoteTag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `baseNoteTag` (
  `basnttId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `basnttName` varchar(64) NOT NULL COMMENT '笔记标签名称',
  `basnttNameSort` varchar(64) NOT NULL DEFAULT '' COMMENT '笔记标签名称-排序',
  `basnttCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该笔记标签计数',
  `basnttCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `basnttModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `basnttIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`basnttId`,`CRMID`),
  KEY `basnttIsValid` (`basnttIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='笔记标签';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `basePosition`
--

DROP TABLE IF EXISTS `basePosition`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `basePosition` (
  `basposId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `basposName` varchar(64) CHARACTER SET utf8 NOT NULL COMMENT '职位名称',
  `basposNameSort` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '职位名称-排序',
  `basposCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该职位计数',
  `basposCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `basposModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `basposIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`basposId`,`CRMID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `careerPath`
--

DROP TABLE IF EXISTS `careerPath`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `careerPath` (
  `ctacpId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ctacpContact` bigint(20) unsigned NOT NULL COMMENT '联系人姓名',
  `ctacpType` bigint(20) unsigned NOT NULL COMMENT '变更类型',
  `ctacpField` bigint(20) unsigned NOT NULL COMMENT '变更项',
  `ctacpOldValue` text COMMENT '原值',
  `ctacpNewValue` text COMMENT '新值',
  `ctacpCreatedBy` bigint(20) unsigned NOT NULL COMMENT '创建人',
  `ctacpCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `ctacpIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`ctacpId`,`CRMID`),
  KEY `ctacpContact` (`ctacpContact`) USING BTREE,
  KEY `ctacpType` (`ctacpType`) USING BTREE,
  KEY `ctacpField` (`ctacpField`) USING BTREE,
  KEY `ctacpCreatedBy` (`ctacpCreatedBy`) USING BTREE,
  KEY `ctacpIsValid` (`ctacpIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Career Path';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `contact`
--

DROP TABLE IF EXISTS `contact`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `contact` (
  `ctaId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  `ctaCustomer` bigint(20) unsigned NOT NULL COMMENT '所属机构',
  `ctaPreCustomer` bigint(20) unsigned DEFAULT NULL COMMENT '前所属机构',
  `ctaName` varchar(64) CHARACTER SET utf8 NOT NULL COMMENT '姓名',
  `ctaNameSort` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '姓名-排序',
  `ctaNameEn` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '英文名',
  `ctaGender` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '性别,1/男,0/女',
  `ctaPosition` bigint(20) unsigned DEFAULT NULL COMMENT '职位',
  `ctaDepartment` bigint(20) unsigned DEFAULT NULL COMMENT '部门',
  `ctaBirthday` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '生日',
  `ctaQBId` varchar(32) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人在QB系统的ID',
  `ctaManagerIdList` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人客户经理ID列表',
  `ctaTagIdList` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人标签ID列表',
  `ctaIsResigned` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否离职,0/在职,1/离职',
  `ctaUniversity` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人毕业院校',
  `ctaCommunicationAddress` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人通讯地址',
  `ctaHomeAddress` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人家庭地址',
  `ctaHobby` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人兴趣爱好',
  `ctaEmailList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人邮箱列表',
  `ctaMobilePhoneList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人手机列表',
  `ctaFixedPhoneList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人固定电话列表',
  `ctaFaxList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人传真列表',
  `ctaQQList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人QQ列表',
  `ctaMSNList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人MSN列表',
  `ctaRMList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人RM列表',
  `ctaWMList` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '联系人WM列表',
  `ctaCreatedBy` bigint(20) unsigned NOT NULL COMMENT '创建人',
  `ctaCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `ctaModifiedBy` bigint(20) unsigned NOT NULL COMMENT '修改人',
  `ctaModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `ctaIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  PRIMARY KEY (`ctaId`,`CRMID`)
) ENGINE=InnoDB AUTO_INCREMENT=9431 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `contactChangeLog`
--

DROP TABLE IF EXISTS `contactChangeLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `contactChangeLog` (
  `ctaclId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ctaclContact` bigint(20) unsigned NOT NULL COMMENT '联系人',
  `ctaclType` bigint(20) unsigned NOT NULL COMMENT '变更类型',
  `ctaclField` bigint(20) unsigned NOT NULL COMMENT '变更项',
  `ctaclOldValue` text COMMENT '原值',
  `ctaclNewValue` text COMMENT '新值',
  `ctaclCreatedBy` bigint(20) unsigned NOT NULL COMMENT '创建人',
  `ctaclCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `ctaclIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`ctaclId`,`CRMID`),
  KEY `ctaclContact` (`ctaclContact`) USING BTREE,
  KEY `ctaclType` (`ctaclType`) USING BTREE,
  KEY `ctaclField` (`ctaclField`) USING BTREE,
  KEY `ctaclCreatedBy` (`ctaclCreatedBy`) USING BTREE,
  KEY `ctaclIsValid` (`ctaclIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=9329 DEFAULT CHARSET=utf8 COMMENT='联系人信息变更日志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `customer` (
  `cstId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  `cstNameCn` varchar(128) CHARACTER SET utf8 NOT NULL COMMENT '中文全称',
  `cstShortNameCn` varchar(128) CHARACTER SET utf8 NOT NULL COMMENT '中文简称',
  `cstNameEn` varchar(128) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '英文全称',
  `cstShortNameEn` varchar(128) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '英文简称',
  `cstCustomerType` bigint(20) unsigned NOT NULL COMMENT '机构类型',
  `cstCity` bigint(20) unsigned NOT NULL COMMENT '所在城市',
  `cstIndustry` bigint(20) unsigned NOT NULL COMMENT '行业类型',
  `cstAddress` varchar(512) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '通信地址',
  `cstComment` text CHARACTER SET utf8 COMMENT '备注',
  `cstNameCnSort` varchar(128) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '中文全称-排序',
  `cstCount` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '使用该机构计数',
  `cstQBId` varchar(32) CHARACTER SET utf8 NOT NULL DEFAULT '' COMMENT '机构在QB系统的ID',
  `cstCreatedBy` bigint(20) unsigned NOT NULL COMMENT '创建人',
  `cstCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `cstModifiedBy` bigint(20) unsigned NOT NULL COMMENT '修改人',
  `cstModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `cstIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  PRIMARY KEY (`cstId`,`CRMID`)
) ENGINE=InnoDB AUTO_INCREMENT=4033 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `customerChangeLog`
--

DROP TABLE IF EXISTS `customerChangeLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `customerChangeLog` (
  `cstclId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `cstclCustomer` bigint(20) unsigned NOT NULL COMMENT '机构',
  `cstclType` bigint(20) unsigned NOT NULL COMMENT '变更类型',
  `cstclField` bigint(20) unsigned NOT NULL COMMENT '变更项',
  `cstclOldValue` text COMMENT '原值',
  `cstclNewValue` text COMMENT '新值',
  `cstclCreatedBy` bigint(20) unsigned NOT NULL COMMENT '创建人',
  `cstclCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `cstclIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`cstclId`,`CRMID`),
  KEY `cstclCustomer` (`cstclCustomer`) USING BTREE,
  KEY `cstclType` (`cstclType`) USING BTREE,
  KEY `cstclField` (`cstclField`) USING BTREE,
  KEY `cstclCreatedBy` (`cstclCreatedBy`) USING BTREE,
  KEY `cstclIsValid` (`cstclIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=3963 DEFAULT CHARSET=utf8 COMMENT='机构信息变更日志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `customerDepositAccount`
--

DROP TABLE IF EXISTS `customerDepositAccount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `customerDepositAccount` (
  `cstdaId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `cstdaCustomerId` bigint(20) unsigned NOT NULL COMMENT '机构ID',
  `cstdaShortName` varchar(64) NOT NULL DEFAULT '' COMMENT '简称',
  `cstdaAccountName` varchar(128) NOT NULL DEFAULT '' COMMENT '户名',
  `cstdaAccount` varchar(128) NOT NULL DEFAULT '' COMMENT '账号',
  `cstdaOpenBank` varchar(128) NOT NULL DEFAULT '' COMMENT '开户行',
  `cstdaCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `cstdaModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `cstdaIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`cstdaId`,`CRMID`),
  KEY `cstdaCustomerId` (`cstdaCustomerId`) USING BTREE,
  KEY `cstdaIsValid` (`cstdaIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='机构托管账户';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `customerFundAccount`
--

DROP TABLE IF EXISTS `customerFundAccount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `customerFundAccount` (
  `cstfaId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `cstfaCustomerId` bigint(20) unsigned NOT NULL COMMENT '机构ID',
  `cstfaShortName` varchar(64) NOT NULL DEFAULT '' COMMENT '简称',
  `cstfaAccountName` varchar(128) NOT NULL DEFAULT '' COMMENT '户名',
  `cstfaAccount` varchar(128) NOT NULL DEFAULT '' COMMENT '账号',
  `cstfaOpenBank` varchar(128) NOT NULL DEFAULT '' COMMENT '开户行',
  `cstfaPayNumber` varchar(128) NOT NULL DEFAULT '' COMMENT '支付系统行号',
  `cstfaCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `cstfaModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `cstfaIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`cstfaId`,`CRMID`),
  KEY `cstfaCustomerId` (`cstfaCustomerId`) USING BTREE,
  KEY `cstfaIsValid` (`cstfaIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='机构资金账户';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `filter`
--

DROP TABLE IF EXISTS `filter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `filter` (
  `fltId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `fltName` varchar(128) NOT NULL DEFAULT '' COMMENT '名称',
  `fltTarget` bigint(20) unsigned NOT NULL COMMENT '类型',
  `fltCondition` text NOT NULL COMMENT '筛选条件',
  `fltCreatedBy` bigint(20) unsigned NOT NULL COMMENT '创建人',
  `fltCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `fltModifiedBy` bigint(20) unsigned NOT NULL COMMENT '修改人',
  `fltModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `fltIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`fltId`,`CRMID`),
  KEY `fltTarget` (`fltTarget`) USING BTREE,
  KEY `fltCreatedBy` (`fltCreatedBy`) USING BTREE,
  KEY `fltModifiedBy` (`fltModifiedBy`) USING BTREE,
  KEY `fltIsValid` (`fltIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='筛选器';
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
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='前台联系方式';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metaCareerPathField`
--

DROP TABLE IF EXISTS `metaCareerPathField`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metaCareerPathField` (
  `mtCpfId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `mtCpfSemantics` varchar(64) NOT NULL COMMENT '语义信息',
  `mtCpfTextCN` varchar(64) NOT NULL COMMENT '文本(中文)',
  `mtCpfIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  PRIMARY KEY (`mtCpfId`),
  UNIQUE KEY `mtCpfSemantics` (`mtCpfSemantics`) USING BTREE,
  KEY `mtCpfIsValid` (`mtCpfIsValid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='元数据表—Career Path变更项';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metaChangeLogField`
--

DROP TABLE IF EXISTS `metaChangeLogField`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metaChangeLogField` (
  `mtClfId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `mtClfSemantics` varchar(64) NOT NULL COMMENT '语义信息',
  `mtClfTextCN` varchar(64) NOT NULL COMMENT '文本(中文)',
  `mtClfIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  PRIMARY KEY (`mtClfId`),
  UNIQUE KEY `mtClfSemantics` (`mtClfSemantics`) USING BTREE,
  KEY `mtClfIsValid` (`mtClfIsValid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=45 DEFAULT CHARSET=utf8 COMMENT='元数据表—日志变更项';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metaChangeLogType`
--

DROP TABLE IF EXISTS `metaChangeLogType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metaChangeLogType` (
  `mtCltId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `mtCltSemantics` varchar(64) NOT NULL COMMENT '语义信息',
  `mtCltTextCN` varchar(64) NOT NULL COMMENT '文本(中文)',
  `mtCltIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  PRIMARY KEY (`mtCltId`),
  UNIQUE KEY `mtCltSemantics` (`mtCltSemantics`) USING BTREE,
  KEY `mtCltIsValid` (`mtCltIsValid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='元数据表—日志变更类型';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `note`
--

DROP TABLE IF EXISTS `note`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `note` (
  `nteId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `nteContact` bigint(20) unsigned NOT NULL COMMENT '联系人',
  `nteType` bigint(20) unsigned NOT NULL COMMENT '笔记类型',
  `nteEventDate` datetime NOT NULL COMMENT '事情发生日期',
  `nteIsFollowUp` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否要跟进,0/不跟进,1/跟进',
  `nteNextFollowUp` datetime NOT NULL DEFAULT '1970-01-01 00:00:01' COMMENT '下次跟进日期',
  `nteContent` text COMMENT '内容(纯文本)',
  `nteAttachmentUrlList` varchar(640) NOT NULL DEFAULT '' COMMENT '附件url列表',
  `nteAssignedToIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '指派给客户经理ID列表',
  `nteSharedToIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '分享给客户经理ID列表',
  `nteSharedToTeamIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '分享给团队ID列表',
  `nteSharedToDepIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '分享给部门ID列表',
  `nteTagIdList` varchar(64) NOT NULL DEFAULT '' COMMENT '笔记标签ID列表',
  `nteCreatedBy` bigint(20) unsigned NOT NULL COMMENT '创建人',
  `nteCreatedAt` datetime NOT NULL COMMENT '创建时间',
  `nteModifiedBy` bigint(20) unsigned NOT NULL COMMENT '修改人',
  `nteModifiedAt` datetime NOT NULL COMMENT '修改时间',
  `nteIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `CRMID` bigint(20) unsigned NOT NULL DEFAULT '1' COMMENT 'UnderwriterCRMID',
  PRIMARY KEY (`nteId`,`CRMID`),
  KEY `nteContact` (`nteContact`) USING BTREE,
  KEY `nteType` (`nteType`) USING BTREE,
  KEY `nteCreatedBy` (`nteCreatedBy`) USING BTREE,
  KEY `nteModifiedBy` (`nteModifiedBy`) USING BTREE,
  KEY `nteIsValid` (`nteIsValid`) USING BTREE,
  KEY `UnderwriterCRMID` (`CRMID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='笔记';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `systemUser`
--

DROP TABLE IF EXISTS `systemUser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `systemUser` (
  `suId` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `suNameCN` varchar(64) NOT NULL COMMENT '中文全称',
  `suShortNameCN` varchar(64) NOT NULL DEFAULT '' COMMENT '中文简称',
  `suNameEN` varchar(64) NOT NULL DEFAULT '' COMMENT '英文全称',
  `suShortNameEN` varchar(64) NOT NULL DEFAULT '' COMMENT '英文简称',
  `suAddress` varchar(64) DEFAULT '' COMMENT '地址',
  `suIsValid` tinyint(1) unsigned NOT NULL COMMENT '有效性',
  `suCompanyId` varchar(32) DEFAULT NULL,
  `RECOMMEND_LIMIT` int(10) unsigned zerofill DEFAULT NULL,
  `INSTITUTION_CODE` varchar(32) DEFAULT NULL COMMENT '机构代码',
  `CRM_LOGIN_LOGO` varchar(100) DEFAULT NULL COMMENT 'CRM login logo下载地址',
  `CRM_INDEX_LOGO` varchar(100) DEFAULT NULL COMMENT 'CRM Index Logo 下载地址',
  `LOGO_URL` varchar(100) DEFAULT '' COMMENT 'logo图片下载地址',
  `QR_LOGO_URL` varchar(100) DEFAULT NULL,
  `CRM_LOGIN_ADDR` varchar(100) DEFAULT NULL COMMENT 'CRM login IP/域名',
  `TEMP_INST_ADDR` varchar(100) DEFAULT NULL COMMENT '临时机构IP/域名',
  `TEMP_INST_PAGE_WIDTH` int(10) DEFAULT '410' COMMENT '临时机构页面宽度',
  `TEMP_INST_PAGE_HEIGHT` int(10) DEFAULT '226' COMMENT '临时机构页面高度',
  `CLIENT_ANALYSIS_ADDR` varchar(100) DEFAULT NULL COMMENT '客户行为分析IP/域名',
  `VERSION_SHOWN` varchar(100) DEFAULT NULL COMMENT '版本名称（用于客户端显示等）',
  `VERSION_SHOWN_CODE` varchar(15) DEFAULT NULL,
  `FUNCTION_CONFIG_ID` bigint(20) unsigned DEFAULT NULL COMMENT '功能配置选择',
  `updateip` varchar(255) DEFAULT '',
  `updateport` int(11) DEFAULT '0',
  `updateroot` varchar(255) DEFAULT '',
  `updateparam` varchar(255) DEFAULT '',
  `ProtocolHttpFolder` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`suId`),
  UNIQUE KEY `UN_suCompanyId` (`suCompanyId`),
  KEY `suIsValid` (`suIsValid`) USING BTREE,
  KEY `suId` (`suId`) USING BTREE,
  KEY `systemUser_ConfigSet` (`FUNCTION_CONFIG_ID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8 COMMENT='系统使用者信息';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-04 12:28:05
