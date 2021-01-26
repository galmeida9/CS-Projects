SELECT tipo_anomalia, lingua, dia_da_semana, COUNT(*)
FROM f_anomalia
NATURAL JOIN d_lingua
NATURAL JOIN d_tempo
GROUP BY CUBE(tipo_anomalia, lingua, dia_da_semana)
ORDER BY tipo_anomalia, lingua, dia_da_semana