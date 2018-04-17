ALTER TABLE `idb_bond_backup`.`bond_deal` MODIFY COLUMN `confirm_account`  varchar(255) NULL DEFAULT NULL COMMENT '确认者账号' AFTER `time_style`;


ALTER TABLE `idb_bond_backup`.`bond_offer` ADD COLUMN `location`  varchar(45) NULL DEFAULT NULL AFTER `bond_key_listed_market`;


ALTER TABLE `idb_bond_backup`.`bond_offer_log` ADD COLUMN `location`  varchar(45) NULL DEFAULT NULL AFTER `bond_key_listed_market`;