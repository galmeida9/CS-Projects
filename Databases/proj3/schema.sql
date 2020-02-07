-- Drop all tables
drop table if exists local_publico cascade;
drop table if exists item cascade;
drop table if exists anomalia cascade;
drop table if exists anomalia_traducao cascade;
drop table if exists duplicado cascade;
drop table if exists utilizador cascade;
drop table if exists utilizador_qualificado cascade;
drop table if exists utilizador_regular cascade;
drop table if exists incidencia cascade;
drop table if exists proposta_de_correcao cascade;
drop table if exists correcao cascade;

-- Create all tables
create table local_publico (
    latitude float not null,
    longitude float not null,
    nome varchar(50) not null,
    constraint pk_latitude_longitude primary key(latitude, longitude)
);

create table item (
    id serial,
    descricao varchar(200) not null,
    localizacao varchar(50) not null,
    latitude float not null,
    longitude float not null,
    constraint pk_item_id primary key(id),
    constraint fk_latitude foreign key(latitude, longitude) references local_publico(latitude, longitude) on delete cascade
);

create table anomalia (
    id serial,
    zona box not null,
    imagem varchar(512) not null,
    lingua varchar(20) not null,
    ts timestamp(0) not null,
    descricao varchar(200) not null,
    tem_anomalia_redacao boolean,
    constraint pk_anomalia_id primary key(id)
);

create table anomalia_traducao (
    id serial,
    zona2 box not null,
    lingua2 varchar(20) not null,
    constraint pk_anomalia_traducao_id primary key(id),
    constraint fk_anomalia_traducao_id foreign key(id) references anomalia(id) on delete cascade
);

create table duplicado (
    item1 serial,
    item2 serial check(item1 < item2),
    constraint pk_item_ids primary key(item1, item2),
    constraint fk_item1 foreign key(item1) references item(id) on delete cascade,
    constraint fk_item2 foreign key(item2) references item(id) on delete cascade
);

create table utilizador (
    email varchar(40) not null,
    psw varchar(15) not null,
    constraint pk_utilizador_email primary key(email)
);

create table utilizador_qualificado (
    email varchar(40) not null,
    constraint pk_utilizador_qualificado_email primary key(email),
    constraint fk_utilizador_qualificado_email foreign key(email) references utilizador(email) on delete cascade
);

create table utilizador_regular (
    email varchar(40) not null,
    constraint pk_utilizador_regular_email primary key(email),
    constraint fk_utilizador_regular_email foreign key(email) references utilizador(email) on delete cascade
);

create table incidencia (
    anomalia_id serial,
    item_id serial,
    email varchar(40) not null,
    constraint pk_incidencia primary key(anomalia_id),
    constraint fk_anomalia_id foreign key(anomalia_id) references anomalia(id) on delete cascade,
    constraint fk_item_id foreign key(item_id) references item(id) on delete cascade,
    constraint fk_incidencia_email foreign key(email) references utilizador(email) on delete cascade
);

create table proposta_de_correcao (
    email varchar(40) not null,
    nro serial,
    data_hora timestamp not null,
    texto varchar(200) not null,
    constraint pk_email_nro primary key(email, nro),
    constraint fk_proposta_de_correcao_email foreign key(email) references utilizador_qualificado(email) on delete cascade
);

create table correcao (
    email varchar(40) not null,
    nro integer not null,
    anomalia_id integer not null,
    constraint pk_email_nro_anomalia_id primary key(email, nro, anomalia_id),
    constraint pk_correcao_email foreign key(email, nro) references proposta_de_correcao(email, nro) on delete cascade,
    constraint pk_correcao_anomalia_id foreign key(anomalia_id) references incidencia(anomalia_id) on delete cascade
);

