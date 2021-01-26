DROP TABLE IF EXISTS d_utilizador CASCADE;
DROP TABLE IF EXISTS d_tempo CASCADE;
DROP TABLE IF EXISTS d_local CASCADE;
DROP TABLE IF EXISTS d_lingua CASCADE;
DROP TABLE IF EXISTS f_anomalia CASCADE;

-- Dimensões

create table d_utilizador(
    id_utilizador serial not null,
    email varchar(40) not null,
    tipo varchar(40) not null, --TODO: Check
    constraint pk_utilizador primary key(id_utilizador)
);

create table d_tempo(
    id_tempo serial not null,
    dia integer not null,
    dia_da_semana varchar(40) not null,
    semana integer not null,
    mes integer not null,
    trimestre integer not null,
    ano integer not null,
    UNIQUE (dia, mes, ano),
    constraint pk_tempo primary key(id_tempo)
);

create table d_local(
    id_local serial not null,
    latitude float not null,
    longitude float not null,
    nome varchar(40) not null,
    constraint pk_local primary key(id_local)
);

create table d_lingua(
    id_lingua serial not null,
    lingua varchar(40) not null,
    constraint pk_lingua primary key(id_lingua)
);

--Factos

create table f_anomalia(
    id_utilizador serial,
    id_tempo serial,
    id_local serial,
    id_lingua serial,
    tipo_anomalia varchar(40),
    com_proposta boolean,
    primary key(id_utilizador, id_tempo, id_local, id_lingua), --TODO: Verificar se é chave primária ou não
    constraint fk_f_utilizador foreign key(id_utilizador) references d_utilizador(id_utilizador) on delete cascade on update cascade,
    constraint fk_f_tempo foreign key(id_tempo) references d_tempo(id_tempo) on delete cascade on update cascade,
    constraint fk_f_local foreign key(id_local) references d_local(id_local) on delete cascade on update cascade,
    constraint fk_f_lingua foreign key(id_lingua) references d_lingua(id_lingua) on delete cascade on update cascade
);

--Carregar dados nas tabelas de dimensões

INSERT INTO d_utilizador(email, tipo)
    SELECT email, 
        CASE WHEN EXISTS ( SELECT q.email FROM utilizador_qualificado AS q WHERE q.email = u.email )
        THEN 'Utilizador Qualificado'
        ELSE 'Utilizador Regular'
        END
    FROM utilizador AS u;

INSERT INTO d_tempo(dia, dia_da_semana, semana, mes, trimestre, ano)
    SELECT DISTINCT EXTRACT(day FROM ts), EXTRACT(isodow FROM ts), EXTRACT(week FROM ts), EXTRACT(month FROM ts), EXTRACT(QUARTER FROM ts), EXTRACT(year FROM ts)
    FROM anomalia;

INSERT INTO d_local(latitude, longitude, nome)
    SELECT DISTINCT latitude, longitude, nome
    FROM local_publico;

INSERT INTO d_lingua(lingua)
    SELECT DISTINCT lingua
    FROM anomalia;

--Carregar dados na tabela de factos

INSERT INTO f_anomalia
    SELECT id_utilizador, id_tempo, id_local, id_lingua, 
    (CASE WHEN An.tem_anomalia_redacao --TODO: Verificar se pode ser assim
	THEN 'Redacao'
	ELSE 'Traducao'
	END),
	(CASE WHEN Cor.nro IS NOT NULL 
	THEN true
	ELSE false
	END) AS com_proposta
    -- Get data
    FROM incidencia AS Inc
    JOIN anomalia AS An ON An.id = Inc.anomalia_id
    JOIN item AS It ON It.id = Inc.item_id
    LEFT JOIN correcao AS Cor ON Cor.anomalia_id = Inc.anomalia_id
    -- Join with dimensions
    JOIN d_lingua AS dLi ON An.lingua = dLi.lingua
    JOIN d_local AS dLo ON (dLo.latitude = It.latitude and dLo.longitude = It.longitude)
    JOIN d_tempo AS dT ON (EXTRACT(day FROM An.ts), EXTRACT(month FROM An.ts), EXTRACT(year FROM An.ts)) = (dT.dia, dT.mes, dT.ano)
    JOIN d_utilizador AS dU ON dU.email = Inc.email;
