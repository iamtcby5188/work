-- Manually Generated: 2016-04-18 18:33
-- Author: JiaLu


ALTER TABLE `idb_bond`.`bond_bank` 
ADD COLUMN `company_id` VARCHAR(32) NULL DEFAULT NULL COMMENT '' AFTER `id`;

ALTER TABLE `idb_bond`.`bond_bank_agent` 
ADD COLUMN `company_id` VARCHAR(32) NULL DEFAULT NULL COMMENT '' AFTER `id`;

ALTER TABLE `idb_bond`.`bond_best_offer` 
ADD COLUMN `company_id` VARCHAR(32) NULL DEFAULT NULL COMMENT '' AFTER `id`;

ALTER TABLE `idb_bond`.`bond_deal` 
ADD COLUMN `company_id` VARCHAR(32) NULL DEFAULT NULL COMMENT '' AFTER `id`;

ALTER TABLE `idb_bond`.`bond_goods_group` 
ADD COLUMN `company_id` VARCHAR(32) NULL DEFAULT NULL COMMENT '' AFTER `id`;

ALTER TABLE `idb_bond`.`bond_offer` 
ADD COLUMN `company_id` VARCHAR(32) NULL DEFAULT NULL COMMENT '' AFTER `id`;

ALTER TABLE `idb_bond`.`china_bond_deviated_value` 
ADD COLUMN `company_id` VARCHAR(32) NULL DEFAULT NULL COMMENT '' AFTER `id`;

update `idb_bond`.`bond_bank`                 set company_id = '1';

update `idb_bond`.`bond_bank_agent`           set company_id = '1';

update `idb_bond`.`bond_best_offer`           set company_id = '1';

update `idb_bond`.`bond_deal`                 set company_id = '1';

update `idb_bond`.`bond_goods_group`          set company_id = '1';

update `idb_bond`.`bond_offer`                set company_id = '1';

update `idb_bond`.`china_bond_deviated_value` set company_id = '1';


SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `idb_bond`.`sys_company`
-- ----------------------------
DROP TABLE IF EXISTS `idb_bond`.`sys_company`;
CREATE TABLE `idb_bond`.`sys_company` (
  `Id` varchar(32) NOT NULL DEFAULT '',
  `name` varchar(128) DEFAULT NULL,
  `status` char(1) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Records of sys_company
-- ----------------------------
INSERT INTO `idb_bond`.`sys_company` VALUES ('1', '¹úÀû', '1');


insert into idb_account.job_task (id, task_code, task_type, task_impl_class, task_express, state_date, state, create_date, status) values ('20','20', '20', 'com.idb.idbquartz.quartz.jobs.SyncPInstitutionRatingTablesJob', '0 0 6 * * ?', current_time(), 'U', current_time(), '1');

insert into idb_bond.console_permission (id, name, family) values('998dad1d84e749ad904747700ab8e5b6', 'OFFER_BROKER', 'BASIC_FEATURE');

