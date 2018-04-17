ALTER TABLE `idb_account`.`idb_account` MODIFY COLUMN `ACCOUNT_CODE`  varchar(64) NULL DEFAULT NULL AFTER `COMPANY_NAME`;
ALTER TABLE `idb_account`.`idb_account` MODIFY COLUMN `DISPLAY_NAME`  varchar(64) NULL DEFAULT NULL COMMENT '显示名（昵称）' AFTER `INIT_PASSWORD`;
ALTER TABLE `idb_account`.`idb_account` MODIFY COLUMN `MOBILE`  varchar(64) NULL DEFAULT NULL COMMENT '手机' AFTER `DISPLAY_NAME`;
ALTER TABLE `idb_account`.`idb_account` MODIFY COLUMN `TELEPHONE`  varchar(64) NULL DEFAULT NULL COMMENT '电话' AFTER `MOBILE`;


ALTER TABLE `idb_account`.`idb_account_role_relation` MODIFY COLUMN `ACCOUNT_ID`  varchar(32) NULL DEFAULT NULL COMMENT '账号ID' AFTER `ROLE_ID`;


ALTER TABLE `idb_account`.`idb_financial_company` MODIFY COLUMN `BANK_TYPE`  char(1) NULL DEFAULT NULL COMMENT '银行类型 1.银行 2.保险公司 3.证券公司 4.其他' AFTER `FULL_NAME_EN`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `MODIFY_TIME`  datetime NULL DEFAULT NULL AFTER `STATUS`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `fullname_cn_front`  varchar(128) NULL DEFAULT NULL COMMENT '中文全称前部分' AFTER `MODIFY_TIME`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `fullname_cn_later`  varchar(128) NULL DEFAULT NULL COMMENT '中文全称前后部分' AFTER `fullname_cn_front`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `fullname_en_front`  varchar(128) NULL DEFAULT NULL COMMENT '英文全称前部分' AFTER `fullname_cn_later`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `fullname_en_later`  varchar(128) NULL DEFAULT NULL COMMENT '英文全称后部分' AFTER `fullname_en_front`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `shortname_cn`  varchar(128) NULL DEFAULT NULL COMMENT '中文简称' AFTER `fullname_en_later`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `shortname_en`  varchar(128) NULL DEFAULT NULL COMMENT '英文简称' AFTER `shortname_cn`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `bank_nature`  char(1) NULL DEFAULT NULL COMMENT '银行性质 1.国有大行 2.政策性 3.股分制 4.城商行 5.其他' AFTER `shortname_en`;
ALTER TABLE `idb_account`.`idb_financial_company` ADD COLUMN `bank_level`  char(1) NULL DEFAULT NULL COMMENT '行级别：1.总行2.一级分行3.二级分行4.支行' AFTER `bank_nature`;


ALTER TABLE `idb_account`.`idb_login_log` MODIFY COLUMN `ACCOUNT_ID`  varchar(32) NULL DEFAULT NULL COMMENT '账号ID' AFTER `LOGIN_STATUS_ID`;
ALTER TABLE `idb_account`.`idb_login_log` MODIFY COLUMN `CLIENT_TYPE`  char(1) NULL DEFAULT '' COMMENT '用户类型' AFTER `DISPLAY_NAME`;
ALTER TABLE `idb_account`.`idb_login_log` MODIFY COLUMN `IP`  varchar(50) NULL DEFAULT '' COMMENT 'IPAdress' AFTER `MAC`;


ALTER TABLE `idb_account`.`idb_login_status` ADD COLUMN `CLIENT_LOCATION`  varchar(45) NULL DEFAULT NULL AFTER `STATUS`;