CREATE TABLE IF NOT EXISTS `idb_bond`.`spread` (
  `id` VARCHAR(32) NOT NULL,
  `name` VARCHAR(45) NULL DEFAULT NULL,
  `code` VARCHAR(45) NULL DEFAULT NULL,
  `prev_bond_key_listed_market` VARCHAR(64) NULL DEFAULT NULL,
  `prev_bond_id` VARCHAR(64) NULL DEFAULT NULL,
  `post_bond_key_listed_market` VARCHAR(64) NULL DEFAULT NULL,
  `post_bond_id` VARCHAR(64) NULL DEFAULT NULL,
  `sort` INT(8) NULL DEFAULT NULL,
  `create_user` VARCHAR(32) NULL DEFAULT NULL,
  `create_time` DATETIME NULL DEFAULT NULL,
  `modify_user` VARCHAR(32) NULL DEFAULT NULL,
  `modify_time` DATETIME NULL DEFAULT NULL,
  `status` CHAR(1) NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COMMENT = '换券配置表';

CREATE TABLE IF NOT EXISTS `idb_bond`.`spread_quote` (
  `id` VARCHAR(32) NOT NULL,
  `spread_id` VARCHAR(32) NULL DEFAULT NULL,
  `company_id` VARCHAR(32) NULL DEFAULT NULL,
  `spread_code` VARCHAR(45) NULL DEFAULT NULL,
  `spread_name` VARCHAR(45) NULL DEFAULT NULL,
  `symbol` CHAR(1) NULL DEFAULT NULL,
  `bid_price` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_price` DECIMAL(12,4) NULL DEFAULT NULL,
  `bid_volume` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_volume` DECIMAL(12,4) NULL DEFAULT NULL,
  `scale` DECIMAL(12,8) NULL DEFAULT NULL COMMENT '数量比例：Vol.Bid和Vol.Ofr输入完成后，根据两个的值，计算一个比例，比如1:2这样的，在后期计算过程中会用的到',
  `inst_id` VARCHAR(32) NULL DEFAULT NULL,
  `inst_name` VARCHAR(128) NULL DEFAULT NULL,
  `agent_id` VARCHAR(32) NULL DEFAULT NULL,
  `agent_name` VARCHAR(128) NULL DEFAULT NULL,
  `broekr_id` VARCHAR(32) NULL DEFAULT NULL,
  `broker_name` VARCHAR(128) NULL DEFAULT NULL,
  `create_user` VARCHAR(32) NULL DEFAULT NULL,
  `create_time` DATETIME NULL DEFAULT NULL,
  `modify_user` VARCHAR(32) NULL DEFAULT NULL,
  `modify_time` DATETIME NULL DEFAULT NULL,
  `bid_deal_status` CHAR(1) NULL DEFAULT NULL,
  `bid_deal_time` DATETIME NULL DEFAULT NULL,
  `ofr_deal_stauts` CHAR(1) NULL DEFAULT NULL,
  `ofr_deal_time` DATETIME NULL DEFAULT NULL,
  `status` CHAR(1) NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COMMENT = '换券报价表';

CREATE TABLE IF NOT EXISTS `idb_bond`.`spread_quote_log` (
  `id` VARCHAR(32) NOT NULL,
  `method` VARCHAR(128) NULL DEFAULT NULL,
  `message_id` VARCHAR(128) NULL DEFAULT NULL,
  `login_token` VARCHAR(32) NULL DEFAULT NULL,
  `oper_user` VARCHAR(32) NULL DEFAULT NULL,
  `oper_time` DATETIME NULL DEFAULT NULL,
  `quote_id` VARCHAR(32) NULL DEFAULT NULL,
  `spread_id` VARCHAR(32) NULL DEFAULT NULL,
  `spread_code` VARCHAR(32) NULL DEFAULT NULL,
  `spread_name` VARCHAR(32) NULL DEFAULT NULL,
  `quote_side` CHAR(1) NULL DEFAULT NULL,
  `bp` DECIMAL(6,4) NULL DEFAULT NULL,
  `price` DECIMAL(12,4) NULL DEFAULT NULL,
  `bid_volume` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_volume` DECIMAL(12,4) NULL DEFAULT NULL,
  `scale` DECIMAL(12,8) NULL DEFAULT NULL COMMENT '数量比例：Vol.Bid和Vol.Ofr输入完成后，根据两个的值，计算一个比例，比如1:2这样的，在后期计算过程中会用的到',
  `inst_id` VARCHAR(32) NULL DEFAULT NULL,
  `inst_name` VARCHAR(128) NULL DEFAULT NULL,
  `agent_id` VARCHAR(32) NULL DEFAULT NULL,
  `agent_name` VARCHAR(128) NULL DEFAULT NULL,
  `broker_id` VARCHAR(32) NULL DEFAULT NULL,
  `broker_name` VARCHAR(128) NULL DEFAULT NULL,
  `create_user` VARCHAR(32) NULL DEFAULT NULL,
  `create_time` DATETIME NULL DEFAULT NULL,
  `modify_user` VARCHAR(32) NULL DEFAULT NULL,
  `modify_time` DATETIME NULL DEFAULT NULL,
  `bid_deal_status` CHAR(1) NULL DEFAULT NULL,
  `bid_deal_time` DATETIME NULL DEFAULT NULL,
  `ofr_deal_status` CHAR(1) NULL DEFAULT NULL,
  `ofr_deal_time` DATETIME NULL DEFAULT NULL,
  `status` CHAR(1) NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COMMENT = '换券报价表';

CREATE TABLE IF NOT EXISTS `idb_bond`.`spread_best_quote` (
  `id` VARCHAR(32) NOT NULL,
  `spread_id` VARCHAR(32) NULL DEFAULT NULL,
  `spread_code` VARCHAR(32) NULL DEFAULT NULL,
  `spread_name` VARCHAR(32) NULL DEFAULT NULL,
  `bid_id` VARCHAR(32) NULL DEFAULT NULL,
  `bid_price` DECIMAL(12,4) NULL DEFAULT NULL,
  `bid_volume` DECIMAL(12,4) NULL DEFAULT NULL,
  `bid_inst_id` VARCHAR(32) NULL DEFAULT NULL,
  `bid_inst_name` VARCHAR(128) NULL DEFAULT NULL,
  `bid_agent_id` VARCHAR(32) NULL DEFAULT NULL,
  `bid_agent_name` VARCHAR(128) NULL DEFAULT NULL,
  `bid_broker_id` VARCHAR(32) NULL DEFAULT NULL,
  `bid_broker_name` VARCHAR(128) NULL DEFAULT NULL,
  `ofr_id` VARCHAR(32) NULL DEFAULT NULL,
  `ofr_price` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_volume` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_inst_id` VARCHAR(32) NULL DEFAULT NULL,
  `ofr_inst_name` VARCHAR(128) NULL DEFAULT NULL,
  `ofr_agent_id` VARCHAR(32) NULL DEFAULT NULL,
  `ofr_agent_name` VARCHAR(128) NULL DEFAULT NULL,
  `ofr_broker_id` VARCHAR(32) NULL DEFAULT NULL,
  `ofr_broker_name` VARCHAR(128) NULL DEFAULT NULL,
  `create_time` DATETIME NULL DEFAULT NULL,
  `status` CHAR(1) NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `idb_bond`.`spread_route` (
  `id` VARCHAR(32) NOT NULL,
  `quote_type` INT(11) NULL DEFAULT NULL,
  `quote_id` VARCHAR(32) NULL DEFAULT NULL,
  `goods_code_l` VARCHAR(32) NULL DEFAULT NULL,
  `goods_id_l` VARCHAR(32) NULL DEFAULT NULL,
  `goods_name_l` VARCHAR(128) NULL DEFAULT NULL,
  `bid_price_l` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_price_l` DECIMAL(12,4) NULL DEFAULT NULL,
  `bid_volum_l` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_volum_l` DECIMAL(12,4) NULL DEFAULT NULL,
  `ins_id_l` VARCHAR(32) NULL DEFAULT NULL,
  `ins_name_l` VARCHAR(128) NULL DEFAULT NULL,
  `trader_id_l` VARCHAR(32) NULL DEFAULT NULL,
  `trader_name_l` VARCHAR(128) NULL DEFAULT NULL,
  `broker_id_l` VARCHAR(32) NULL DEFAULT NULL,
  `broker_name_l` VARCHAR(128) NULL DEFAULT NULL,
  `goods_code_r` VARCHAR(32) NULL DEFAULT NULL,
  `goods_id_r` VARCHAR(32) NULL DEFAULT NULL,
  `goods_name_r` VARCHAR(128) NULL DEFAULT NULL,
  `bid_price_r` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_price_r` DECIMAL(12,4) NULL DEFAULT NULL,
  `bid_volum_r` DECIMAL(12,4) NULL DEFAULT NULL,
  `ofr_volum_r` DECIMAL(12,4) NULL DEFAULT NULL,
  `ins_id_r` VARCHAR(32) NULL DEFAULT NULL,
  `ins_name_r` VARCHAR(128) NULL DEFAULT NULL,
  `trader_id_r` VARCHAR(32) NULL DEFAULT NULL,
  `trader_name_r` VARCHAR(128) NULL DEFAULT NULL,
  `broker_id_r` VARCHAR(32) NULL DEFAULT NULL,
  `broker_name_r` VARCHAR(128) NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COMMENT = 'spread套算路径表';

