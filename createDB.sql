CREATE DATABASE postgres;
CREATE TABLE questoes (
  IDquestao serial PRIMARY KEY,
  dominio varchar(30),
  tema varchar(30),
  dificuldade integer,
  texto varchar(200),
  resposta varchar(500),
);  

