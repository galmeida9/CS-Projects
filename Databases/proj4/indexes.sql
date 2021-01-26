--1.1)
-- Não fazer index

--1.2)
drop index proposta_de_correcao_data_hora_idx;
create index proposta_de_correcao_data_hora_idx 
on proposta_de_correcao using btree (data_hora);

--2)
drop index incidencia_anomalia_id_idx;
create index incidencia_anomalia_id_idx 
on incidencia using hash (anomalia_id);

--3.1) CHECK
-- Não fazer index

--3.2) CHECK
drop index correcao_anomalia_id_idx;
create index correcao_anomalia_id_idx 
on correcao using btree (anomalia_id);

--4)
drop index anomalia_ts_tem_anomalia_traducao_idx;
create index anomalia_ts_tem_anomalia_traducao_idx 
on anomalia using btree (ts, tem_anomalia_traducao);