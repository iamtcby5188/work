ALTER TABLE `idb_bond`.`bond_offer` 
ADD COLUMN `LOCATION` VARCHAR(45) NULL DEFAULT NULL AFTER `BOND_KEY_LISTED_MARKET`, 
COMMENT = '报价表' ;

ALTER TABLE `idb_bond`.`bond_offer_log` 
ADD COLUMN `LOCATION` VARCHAR(45) NULL DEFAULT NULL AFTER `BOND_KEY_LISTED_MARKET`, 
COMMENT = '报价日志表' ;

drop table if exists `idb_bond`.`p_issuer_info`
CREATE TABLE `idb_bond`.`p_issuer_info` (
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
  `Stockholder_Name` varchar(127) DEFAULT NULL,
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
  `Orgnization_Code` varchar(16) DEFAULT NULL,
  `Regd_Num` varchar(31) DEFAULT NULL,
  `Listed_Type` varchar(15) DEFAULT NULL,
  `Uni_Code` varchar(18) DEFAULT NULL,
  `Municipal_code` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `E_IDX_4mk1u17osruhd` (`delflag`,`institution_code`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

