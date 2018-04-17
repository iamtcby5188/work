-- MySQL Workbench Synchronization
-- Generated: 2016-02-23 16:33
-- Model: IDBBond
-- Version: 1.0
-- Project: IDBBond
-- Author: Xing

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

ALTER TABLE `idb_bond`.`account_group_console_permission` 
ENGINE = MyISAM ;

ALTER TABLE `idb_bond`.`bond` 
ADD COLUMN `Registered_Amount` DECIMAL(16,4) NULL DEFAULT NULL COMMENT '' AFTER `Full_Name_EN`,
ADD COLUMN `Bid_Limit_Bottom` DECIMAL(4,2) NULL DEFAULT NULL COMMENT '' AFTER `Registered_Amount`,
ADD COLUMN `Bid_Limit_Top` DECIMAL(4,2) NULL DEFAULT NULL COMMENT '' AFTER `Bid_Limit_Bottom`,
ADD COLUMN `Issue_End_Time` VARCHAR(4) NULL DEFAULT NULL COMMENT '' AFTER `Bid_Limit_Top`,
ADD COLUMN `Auction_Date_Time` VARCHAR(8) NULL DEFAULT NULL COMMENT '' AFTER `Issue_End_Time`,
ADD COLUMN `Ent_Cor` VARCHAR(3) NULL DEFAULT NULL COMMENT '' AFTER `Auction_Date_Time`,
ADD COLUMN `Issue_Period` VARCHAR(16) NULL DEFAULT NULL COMMENT '' AFTER `Ent_Cor`;

ALTER TABLE `idb_bond`.`bond_dev` 
ENGINE = MyISAM ;

ALTER TABLE `idb_bond`.`bond_list_info` 
DROP INDEX `E_IDX_3t2zu2n5gquez` ;

ALTER TABLE `idb_bond`.`china_bond_valuation` 
ENGINE = InnoDB ,
ADD COLUMN `Val_Modified_Duration` DECIMAL(7,4) NULL DEFAULT NULL COMMENT '' AFTER `bond_key_listed_market`,
ADD COLUMN `Val_Convexity` DECIMAL(7,4) NULL DEFAULT NULL COMMENT '' AFTER `Val_Modified_Duration`,
ADD COLUMN `Val_Basis_Point_Value` DECIMAL(7,4) NULL DEFAULT NULL COMMENT '' AFTER `Val_Convexity`;

ALTER TABLE `idb_bond`.`console_permission` 
ENGINE = MyISAM ;

ALTER TABLE `idb_bond`.`hot_key` 
ENGINE = MyISAM ;

DROP TABLE IF EXISTS `idb_bond`.`selective_info_20150831` ;

DROP TABLE IF EXISTS `idb_bond`.`contract_log` ;

DROP TABLE IF EXISTS `idb_bond`.`contract_detail` ;

DROP TABLE IF EXISTS `idb_bond`.`bond_small_view_inc` ;


-- -----------------------------------------------------
-- Placeholder table for view `idb_bond`.`bond_screen`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `idb_bond`.`bond_screen` (`goods_id` INT, `short_name` INT, `goods_code` INT, `goods_term` INT, `goods_level` INT, `goods_type` INT, `profit_code` INT, `ofr_agent_code` INT, `ofr_price` INT, `ofr_price_description` INT, `ofr_volume` INT, `ofr_flag_vip` INT, `ofr_flag_bad` INT, `ofr_flag_bargain` INT, `bid_agent_code` INT, `bid_price` INT, `bid_price_description` INT, `bid_volume` INT, `bid_flag_vip` INT, `bid_flag_bad` INT, `bid_flag_bargain` INT);

-- -----------------------------------------------------
-- Placeholder table for view `idb_bond`.`bond_group_relation_view`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `idb_bond`.`bond_group_relation_view` (`group_id` INT, `goods_id` INT, `id` INT, `account_type` INT, `create_time` INT, `ofr_price_range_begin` INT, `ofr_price_range_end` INT, `bid_price_range_begin` INT, `bid_price_range_end` INT, `flag_allow` INT, `ua_id` INT, `account_id` INT, `bond_key` INT, `listed_market` INT, `bond_key_listed_market` INT);

-- -----------------------------------------------------
-- Placeholder table for view `idb_bond`.`bond_small_view`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `idb_bond`.`bond_small_view` (`bond_key_listed_market` INT, `delflag` INT, `create_date` INT, `Bond_Key` INT, `Listed_Market` INT, `Bond_ID` INT, `Bond_Subtype` INT, `Short_Name` INT, `Is_Cross_Mkt` INT, `Is_Mortgage` INT, `Pin_Yin` INT, `Pin_Yin_Full` INT, `Listed_Date` INT, `Delisted_Date` INT, `Mkt_Type` INT, `Is_Municipal` INT, `Issuer_Rating_Current_NPY` INT, `Rating_Current_NPY` INT, `Issuer_Rating_Current` INT, `Maturity_Term` INT, `Warranter` INT, `Coupon_Type` INT, `Rating_Augment` INT, `Rating_Date` INT, `Option_Type` INT, `Coupon_Rate_Current` INT, `Maturity_Date` INT, `Issuer_Code` INT, `Next_Coupon_Date` INT, `Redemption_No` INT, `Rating_Current` INT, `FRN_Index_ID` INT, `Fixing_MA_Days` INT, `Coupon_Rate_Spread` INT, `Option_Date` INT, `Issue_Amount` INT, `Underwriter_Code` INT, `Issuer_Rating_Institution_Code` INT, `Term_Unit` INT, `Rating_Institution_Code` INT, `Interest_Start_Date` INT, `Issue_Year` INT, `Ent_Cor` INT);


USE `idb_bond`;

-- -----------------------------------------------------
-- View `idb_bond`.`bond_screen`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `idb_bond`.`bond_screen`;
USE `idb_bond`;
CREATE  OR REPLACE ALGORITHM=TEMPTABLE DEFINER=`artogrid`@`localhost` SQL SECURITY DEFINER VIEW `bond_screen` AS select `g`.`id` AS `goods_id`,`g`.`short_name` AS `short_name`,`g`.`goods_code` AS `goods_code`,`g`.`goods_term` AS `goods_term`,`g`.`goods_level` AS `goods_level`,`g`.`goods_type` AS `goods_type`,`g`.`profit_code` AS `profit_code`,(select `o`.`agent_code` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_agent_code`,(select `o`.`price` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_price`,(select `o`.`price_description` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_price_description`,(select `o`.`volume` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_volume`,(select `o`.`flag_vip` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_flag_vip`,(select `o`.`flag_bad` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_flag_bad`,(select `o`.`flag_bargain` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = -(1)) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` limit 1) AS `ofr_flag_bargain`,(select `o`.`agent_code` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_agent_code`,(select `o`.`price` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_price`,(select `o`.`price_description` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_price_description`,(select `o`.`volume` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_volume`,(select `o`.`flag_vip` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_flag_vip`,(select `o`.`flag_bad` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_flag_bad`,(select `o`.`flag_bargain` from `bond_offer` `o` where ((`o`.`status` = 1) and (`o`.`deal_status` = 0) and (`o`.`symbol` = 1) and (`o`.`goods_id` = `g`.`id`)) order by `o`.`price` desc limit 1) AS `bid_flag_bargain` from `bond_goods` `g`;


USE `idb_bond`;

-- -----------------------------------------------------
-- View `idb_bond`.`bond_group_relation_view`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `idb_bond`.`bond_group_relation_view`;
USE `idb_bond`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`artogrid`@`localhost` SQL SECURITY DEFINER VIEW `bond_group_relation_view` AS select `r`.`group_id` AS `group_id`,`r`.`goods_id` AS `goods_id`,`r`.`id` AS `id`,`r`.`account_type` AS `account_type`,`r`.`create_time` AS `create_time`,`r`.`ofr_price_range_begin` AS `ofr_price_range_begin`,`r`.`ofr_price_range_end` AS `ofr_price_range_end`,`r`.`bid_price_range_begin` AS `bid_price_range_begin`,`r`.`bid_price_range_end` AS `bid_price_range_end`,`r`.`flag_allow` AS `flag_allow`,`gg`.`ua_id` AS `ua_id`,`acc`.`account_id` AS `account_id`,`r`.`bond_key` AS `bond_key`,`r`.`listed_market` AS `listed_market`,`r`.`bond_key_listed_market` AS `bond_key_listed_market` from (((`bond_group_relation` `r` left join `bond_goods_group` `gg` on((`gg`.`id` = `r`.`group_id`))) left join `bond_bank_agent` `a` on((`a`.`customer_id` = `gg`.`ua_id`))) left join `bond_bank_account` `acc` on((`acc`.`bank_agent_id` = `a`.`id`))) where ((`gg`.`status` = 1) and (`a`.`status` = 1) and (`acc`.`status` = 1));


USE `idb_bond`;

-- -----------------------------------------------------
-- View `idb_bond`.`bond_small_view`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `idb_bond`.`bond_small_view`;
USE `idb_bond`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY INVOKER VIEW `bond_small_view` AS select concat(`l`.`Bond_Key`,`l`.`Listed_Market`) AS `bond_key_listed_market`,`l`.`delflag` AS `delflag`,`l`.`create_date` AS `create_date`,`l`.`Bond_Key` AS `Bond_Key`,`l`.`Listed_Market` AS `Listed_Market`,`l`.`Bond_ID` AS `Bond_ID`,`b`.`Bond_Subtype` AS `Bond_Subtype`,`l`.`Short_Name` AS `Short_Name`,`l`.`Is_Cross_Mkt` AS `Is_Cross_Mkt`,`l`.`Is_Mortgage` AS `Is_Mortgage`,`l`.`pinyin` AS `Pin_Yin`,`l`.`pinyin_full` AS `Pin_Yin_Full`,`l`.`Listed_Date` AS `Listed_Date`,`l`.`Delisted_Date` AS `Delisted_Date`,`l`.`Mkt_Type` AS `Mkt_Type`,`b`.`Is_Municipal` AS `Is_Municipal`,`b`.`Issuer_Rating_Current_NPY` AS `Issuer_Rating_Current_NPY`,`b`.`Rating_Current_NPY` AS `Rating_Current_NPY`,`b`.`Issuer_Rating_Current` AS `Issuer_Rating_Current`,`b`.`Maturity_Term` AS `Maturity_Term`,`b`.`Warranter` AS `Warranter`,`b`.`Coupon_Type` AS `Coupon_Type`,`b`.`Rating_Augment` AS `Rating_Augment`,`b`.`Rating_Date` AS `Rating_Date`,`b`.`Option_Type` AS `Option_Type`,`b`.`Coupon_Rate_Current` AS `Coupon_Rate_Current`,`b`.`Maturity_Date` AS `Maturity_Date`,`b`.`Issuer_Code` AS `Issuer_Code`,`b`.`Next_Coupon_Date` AS `Next_Coupon_Date`,`b`.`Redemption_No` AS `Redemption_No`,`b`.`Rating_Current` AS `Rating_Current`,`b`.`FRN_Index_ID` AS `FRN_Index_ID`,`b`.`Fixing_MA_Days` AS `Fixing_MA_Days`,`b`.`Coupon_Rate_Spread` AS `Coupon_Rate_Spread`,`b`.`Option_Date` AS `Option_Date`,`b`.`Issue_Amount` AS `Issue_Amount`,`b`.`Underwriter_Code` AS `Underwriter_Code`,`b`.`Issuer_Rating_Institution_Code` AS `Issuer_Rating_Institution_Code`,`b`.`Term_Unit` AS `Term_Unit`,`b`.`Rating_Institution_Code` AS `Rating_Institution_Code`,`b`.`Interest_Start_Date` AS `Interest_Start_Date`,`b`.`Issue_Year` AS `Issue_Year`,`b`.`Ent_Cor` AS `Ent_Cor` from (`bond_list_info` `l` left join `bond` `b` on((`l`.`Bond_Key` = `b`.`Bond_Key`))) where ((`l`.`delflag` = 0) and (`b`.`delflag` = 0));

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
