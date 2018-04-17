ALTER TABLE `idb_bond`.`bond_bank` MODIFY COLUMN `attribute`  varchar(256) NULL DEFAULT NULL AFTER `status`;
ALTER TABLE `idb_bond`.`bond_bank` MODIFY COLUMN `attribute1`  varchar(256) NULL DEFAULT NULL AFTER `attribute`;
ALTER TABLE `idb_bond`.`bond_bank` MODIFY COLUMN `attribute2`  varchar(256) NULL DEFAULT NULL AFTER `attribute1`;
ALTER TABLE `idb_bond`.`bond_bank` MODIFY COLUMN `attribute3`  varchar(256) NULL DEFAULT NULL AFTER `attribute2`;
ALTER TABLE `idb_bond`.`bond_bank` MODIFY COLUMN `attribute4`  varchar(256) NULL DEFAULT NULL AFTER `attribute3`;
ALTER TABLE `idb_bond`.`bond_bank` MODIFY COLUMN `attribute5`  varchar(256) NULL DEFAULT NULL AFTER `attribute4`;


ALTER TABLE `idb_bond`.`bond_bank_account` ADD COLUMN `account_department`  varchar(32) NULL DEFAULT NULL AFTER `sort`;


ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `code`  varchar(64) NULL DEFAULT NULL AFTER `name`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `attribute`  varchar(256) NULL DEFAULT NULL AFTER `f_code`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `attribute1`  varchar(256) NULL DEFAULT NULL AFTER `attribute`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `attribute2`  varchar(256) NULL DEFAULT NULL AFTER `attribute1`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `attribute3`  varchar(256) NULL DEFAULT NULL AFTER `attribute2`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `attribute4`  varchar(256) NULL DEFAULT NULL AFTER `attribute3`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `attribute5`  varchar(256) NULL DEFAULT NULL AFTER `attribute4`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `pinyin`  varchar(1024) NULL DEFAULT NULL AFTER `customer_id`;
ALTER TABLE `idb_bond`.`bond_bank_agent` MODIFY COLUMN `pinyin_full`  varchar(2048) NULL DEFAULT NULL AFTER `pinyin`;


ALTER TABLE `idb_bond`.`bond_deal` MODIFY COLUMN `volume`  decimal(18,6) NULL DEFAULT NULL COMMENT '数量 券面总额' AFTER `price`;


ALTER TABLE `idb_bond`.`bond_offer` MODIFY COLUMN `volume`  decimal(18,6) NULL DEFAULT NULL AFTER `price`;
ALTER TABLE `idb_bond`.`bond_offer` ADD COLUMN `location`  varchar(42) NULL DEFAULT NULL AFTER `bond_key_listed_market`;


ALTER TABLE `idb_bond`.`bond_offer_log` ADD COLUMN `location`  varchar(45) NULL DEFAULT NULL AFTER `bond_key_listed_market`;


ALTER TABLE `idb_bond`.`push_message_data` MODIFY COLUMN `value`  longblob NULL AFTER `length`;


ALTER TABLE `idb_bond`.`push_message_data_qb` MODIFY COLUMN `value`  longblob NULL AFTER `length`;

DROP TABLE IF EXISTS `idb_bond`.`user_settings`;
CREATE TABLE if not exists `idb_bond`.`user_settings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `company_id` int(11) DEFAULT NULL,
  `account_id` varchar(32) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `modify_time` datetime DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `setting_value` varchar(255) DEFAULT NULL,
  `status` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;


ALTER VIEW `idb_bond`.`bond_small_view` AS 
select concat(`l`.`Bond_Key`,`l`.`Listed_Market`) AS `bond_key_listed_market`,`l`.`delflag` AS `delflag`,`l`.`create_date` AS `create_date`,`l`.`Bond_Key` AS `Bond_Key`,`l`.`Listed_Market` AS `Listed_Market`,`l`.`Bond_ID` AS `Bond_ID`,`b`.`Bond_Subtype` AS `Bond_Subtype`,`l`.`Short_Name` AS `Short_Name`,`l`.`Is_Cross_Mkt` AS `Is_Cross_Mkt`,`l`.`Is_Mortgage` AS `Is_Mortgage`,`l`.`pinyin` AS `Pin_Yin`,`l`.`pinyin_full` AS `Pin_Yin_Full`,`l`.`Listed_Date` AS `Listed_Date`,`l`.`Delisted_Date` AS `Delisted_Date`,`l`.`Mkt_Type` AS `Mkt_Type`,`b`.`Is_Municipal` AS `Is_Municipal`,`b`.`Issuer_Rating_Current_NPY` AS `Issuer_Rating_Current_NPY`,`b`.`Rating_Current_NPY` AS `Rating_Current_NPY`,`b`.`Issuer_Rating_Current` AS `Issuer_Rating_Current`,`b`.`Maturity_Term` AS `Maturity_Term`,`b`.`Warranter` AS `Warranter`,`b`.`Coupon_Type` AS `Coupon_Type`,`b`.`Rating_Augment` AS `Rating_Augment`,`b`.`Rating_Date` AS `Rating_Date`,`b`.`Option_Type` AS `Option_Type`,`b`.`Coupon_Rate_Current` AS `Coupon_Rate_Current`,`b`.`Maturity_Date` AS `Maturity_Date`,`b`.`Issuer_Code` AS `Issuer_Code`,`b`.`Next_Coupon_Date` AS `Next_Coupon_Date`,`b`.`Redemption_No` AS `Redemption_No`,`b`.`Rating_Current` AS `Rating_Current`,`b`.`FRN_Index_ID` AS `FRN_Index_ID`,`b`.`Fixing_MA_Days` AS `Fixing_MA_Days`,`b`.`Coupon_Rate_Spread` AS `Coupon_Rate_Spread`,`b`.`Option_Date` AS `Option_Date`,`b`.`Issue_Amount` AS `Issue_Amount`,`b`.`Underwriter_Code` AS `Underwriter_Code`,`b`.`Issuer_Rating_Institution_Code` AS `Issuer_Rating_Institution_Code`,`b`.`Term_Unit` AS `Term_Unit`,`b`.`Rating_Institution_Code` AS `Rating_Institution_Code`,`b`.`Interest_Start_Date` AS `Interest_Start_Date`,`b`.`Issue_Year` AS `Issue_Year`,`b`.`Ent_Cor` AS `Ent_Cor`,`l`.`modify_date` AS `bondlistinfo_modify_date`,`b`.`modify_date` AS `bond_modify_date` from (`idb_bond`.`bond_list_info` `l` left join `idb_bond`.`bond` `b` on((`l`.`Bond_Key` = `b`.`Bond_Key`))) where ((`l`.`delflag` = 0) and (`b`.`delflag` = 0)) ;


