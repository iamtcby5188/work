# 本sql中idb_bond.bond_goods_type,idb_bond.bond_type_selective中应已经包含ABS或NCD相关的记录
# select * from idb_bond.bond_goods_type where status='1' and product in ('ABS','NCD');
# select a.* from idb_bond.bond_type_selective a left join (select * from idb_bond_goods_type where status='1' and product in ('ABS','NCD')) b on a.type_id=b.id where a.status='1';

# 信用台(BCO)包含ABS
update idb_bond.bond_goods_type set product='BCO' where id='402880f034219aed013421c2f7c40970';

# ABS独立
update idb_bond.bond_goods_type set product='ABS' where id='402880f034219aed013421c2f7c40970';


# 信用台(BCO)包含NCD
update idb_bond.bond_goods_type set status='2' where product='NCD';
delete from idb_bond.bond_goods_type where id='402880f034219aed013421c2f7c40979';
insert into idb_bond.bond_goods_type (id, name, code, status, create_time, description, product) values('402880f034219aed013421c2f7c40979', 'NCD', 'NCD', '1', current_timestamp(), '同业存单', 'BCO');
update idb_bond.bond_type_selective set type_id='402880f034219aed013421c2f7c40979' where selective_code in ('SPD','MCD','SHD','CCD','FRD','RRD','RTD','OTD');

# NCD独立
delete from idb_bond.bond_goods_type where id='402880f034219aed013421c2f7c40979';
update idb_bond.bond_goods_type set status='1' where product='NCD';
delete from idb_bond.bond_type_selective where selective_code in ('SPD','MCD','SHD','CCD','FRD','RRD','RTD','OTD');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('42', 'SPD', '402880f034219aed013421c2f7c40971', '1');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('43', 'MCD', '402880f034219aed013421c2f7c40972', '1');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('44', 'SHD', '402880f034219aed013421c2f7c40973', '1');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('45', 'CCD', '402880f034219aed013421c2f7c40974', '1');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('46', 'FRD', '402880f034219aed013421c2f7c40975', '1');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('47', 'RRD', '402880f034219aed013421c2f7c40976', '1');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('48', 'RTD', '402880f034219aed013421c2f7c40977', '1');
insert into idb_bond.bond_type_selective (id, selective_code, type_id, status) values('49', 'OTD', '402880f034219aed013421c2f7c40978', '1');
