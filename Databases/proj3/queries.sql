-- 1)

SELECT L.nome
FROM incidencia
JOIN item ON incidencia.item_id = item.id
NATURAL JOIN local_publico AS L
GROUP BY L.latitude, L.longitude, L.nome
HAVING COUNT(anomalia_id) >= ALL (
	SELECT COUNT(anomalia_id)
	FROM incidencia
	JOIN item ON incidencia.item_id = item.id
	NATURAL JOIN local_publico AS L
	GROUP BY L.latitude, L.longitude, L.nome
)

-- 2)

SELECT incidencia.email
FROM anomalia 
JOIN incidencia ON anomalia.id = incidencia.anomalia_id
NATURAL JOIN utilizador_regular
WHERE anomalia.ts BETWEEN '2019-01-01 00:00:00' AND '2019-06-30 23:59:59'
GROUP BY incidencia.email
HAVING COUNT(*) >= ALL(
	SELECT COUNT(*)
	FROM anomalia JOIN incidencia ON anomalia.id = incidencia.anomalia_id
	WHERE anomalia.ts BETWEEN '2019-01-01 00:00:00' AND '2019-06-30 23:59:59'
	GROUP BY incidencia.email
)

-- 3)

SELECT email
FROM incidencia 
JOIN item ON incidencia.item_id = item.id 
JOIN anomalia ON incidencia.anomalia_id = anomalia.id
WHERE item.latitude > 39.336775 AND anomalia.ts BETWEEN '2019-01-01 00:00:00' AND '2019-12-31 23:59:59'
GROUP BY email
HAVING COUNT( DISTINCT item.latitude, item.longitude) = (
	SELECT COUNT(*)
	FROM local_publico
	WHERE latitude > 39.336775
)

-- 4)

SELECT X.email
FROM correcao AS C 
CROSS JOIN (
	incidencia AS I 
	JOIN anomalia AS A ON A.id = I.anomalia_id
	JOIN item ON item.id = I.item_id
) AS X
WHERE X.latitude < 39.336775 AND X.ts BETWEEN '2019-01-01 00:00:00' AND '2019-12-31 23:59:59'

EXCEPT 

SELECT X.email
FROM correcao AS C 
CROSS JOIN (
	incidencia AS I 
	JOIN anomalia AS A ON A.id = I.anomalia_id
	JOIN item ON item.id = I.item_id
) AS X
WHERE C.anomalia_id = X.anomalia_id AND C.email = X.email
AND X.latitude < 39.336775 AND X.ts BETWEEN '2019-01-01 00:00:00' AND '2019-12-31 23:59:59'
