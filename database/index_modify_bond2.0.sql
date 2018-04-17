DROP INDEX goods_id ON bond_best_offer;
DROP INDEX bid_agent_code ON bond_best_offer;
DROP INDEX goods_code ON bond_best_offer;
DROP INDEX goods_id_index ON bond_best_offer;
DROP INDEX ofr_agent_code ON bond_best_offer;
DROP INDEX internally ON bond_best_offer;
DROP INDEX bond_key_listed_market ON bond_best_offer;

CREATE INDEX status_time ON bond_best_offer (status ,create_time);
CREATE INDEX status ON bond_best_offer (status );
CREATE INDEX company_bondlist ON bond_best_offer (company_id, bond_key_listed_market);


DROP INDEX goods_id ON bond_offer;
DROP INDEX agent_code ON bond_offer;
DROP INDEX volume ON bond_offer;
DROP INDEX goods_code ON bond_offer;
DROP INDEX symbol ON bond_offer;
DROP INDEX price ON bond_offer;
DROP INDEX deal_time ON bond_offer;
DROP INDEX create_time ON bond_offer;
DROP INDEX deal_status ON bond_offer;

CREATE INDEX status_deal ON bond_offer (status ,deal_status);
CREATE INDEX dealstats_createtime ON bond_offer (deal_status,create_time);