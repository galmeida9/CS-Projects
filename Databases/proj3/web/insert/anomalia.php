<html>
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<link rel="stylesheet" href="../style.css" />
	</head>
	<body>
		<?php
			session_start();
			if (!isset($_SESSION['email'])) {
				header("Location: ../login/login.php");
			}
		?>

		<div>
			<h1 id="title">Inserir anomalia</h1>
			<form class="back-btn" action="../insert.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php

				function overlap($zona, $zona2) {
					list($z1x1, $z1y1, $z1x2, $z1y2) = sscanf($zona, "(%d,%d,%d,%d)");
					list($z2x1, $z2y1, $z2x2, $z2y2) = sscanf($zona2, "(%d,%d,%d,%d)");
				    
				    // If one zone is on left side of other
				    if ($z1x1 >= $z2x2 || $z2x1 >= $z1x2) {
				        return false;
				    }
				    
				    // If one zone is above other
				    if ($z1y2 <= $z2y1 || $z2y1 >= $z1y2) {
				        return false;
				    }
				    return true;
				}
				
				require("../db_class.php");
				$zona = $_REQUEST['zona'];
				$imagem = $_REQUEST['imagem'];
				$lingua = $_REQUEST['lingua'];
				$descricao = $_REQUEST['descricao'];
				$tem_anomalia_redacao = ($_POST['tem_anomalia_traducao'] == 'Yes') ? 'false' : 'true';
				$zona2 = $_REQUEST['zona2'];
				$lingua2 = $_REQUEST['lingua2'];

				if (!isset($zona) || $zona == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada uma zona.</p>");
					exit();
				}

				if (!isset($imagem) || $imagem == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada uma imagem.</p>");
					exit();
				}

				if (!filter_var($imagem, FILTER_VALIDATE_URL)) { 
					echo("<p><font color='red'>ERRO</font>: Não foi especificada um url válido de uma imagem.</p>");
					exit();
				}

				if (!isset($lingua) || $lingua == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificado uma língua.</p>");
					exit();
				}

				if ($tem_anomalia_redacao == 'false') {

					// RI-1
					if (overlap($zona, $zona2)) {
						echo("<p><font color='red'>ERRO</font>: A segunda zona não se pode sobrepor à primeira.</p>");
						exit();
					} 
				        
					// RI-2
					if ($lingua == $lingua2) {
						echo("<p><font color='red'>ERRO</font>: A segunda língua não pode ser igual à primeira.</p>");
						exit();
					}					
				}

				try {
					//DB Init
					$db = new DB();
					$db->connect();

					if ($tem_anomalia_redacao == 'false') {
						$db->beginTransaction();
					}
					
					$sql = "INSERT INTO anomalia (zona, imagem, lingua, ts, descricao, tem_anomalia_redacao)
						VALUES (:zona, :imagem, :lingua, now(), :descricao, :tem_anomalia_redacao)";
					$params = [':zona' => $zona, ':imagem' => $imagem, ':lingua' => $lingua, 
						':descricao' => $descricao, ':tem_anomalia_redacao' => $tem_anomalia_redacao];
					$result = $db->query($sql, $params);

					if ($tem_anomalia_redacao == 'false') {
						$sql = "SELECT MAX(id) FROM anomalia;";
						$result = $db->query($sql);
						$row = $result->fetch();

						$id = $row['max'];
						$sql = "INSERT INTO anomalia_traducao (id, zona2, lingua2)
							VALUES (:id, :zona2, :lingua2)";
						$params = [':id' => $id, ':zona2' => $zona2, ':lingua2' => $lingua2];
						$result = $db->query($sql, $params);
					}
					
					if ($tem_anomalia_redacao == 'false') {
						$db->commit();
					}
					echo("<p>Anomalia inserida com sucesso.</p>");

					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);
				}
				catch (PDOException $e)
				{
					if ($tem_anomalia_redacao == 'false') {
						$db->rollBack();
					}
					echo("<p><font color='red'>ERRO</font> :Zona deve ser da forma (x1,y1,x2,y2) e caso existam duas zonas, elas não se podem intersetar.</p>");
				}
			?>
		</div>
	</body>
</html>