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
			<h1 id="title">Inserir item</h1>
			<form class="back-btn" action="../insert.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../db_class.php");

				$descricao = $_REQUEST['descricao'];
				$localizacao = $_REQUEST['localizacao'];
				$local = $_REQUEST['local'];

				if (!isset($descricao) || $descricao == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada uma descrição.</p>");
					exit();
				}

				if (!isset($localizacao) || $localizacao == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada uma localização.</p>");
					exit();
				}

				if (!isset($local) || $local == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificado um local.</p>");
					exit();
				}
				
				try {
					//DB Init
            		$db = new DB();
            		$db->connect();
					
					$db->beginTransaction();

					$sql = "SELECT latitude, longitude FROM local_publico WHERE nome=:local;";
					$params = [':local' => $local];
					$result = $db->query($sql, $params);

					$row = $result->fetch();
					$latitude = $row['latitude'];
					$longitude = $row['longitude'];
					
					$sql = "INSERT INTO item (descricao, localizacao, latitude, longitude)
						VALUES (:descricao, :localizacao, :latitude, :longitude)";
					$params = [':descricao' => $descricao, ':localizacao' => $localizacao,
						':latitude' => $latitude, ':longitude' => $longitude];
					$result = $db->query($sql, $params);

					$db->commit();
					echo("<p>Item inserido com sucesso.</p>");

					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);
				}
				catch (PDOException $e)
				{
					$db->rollBack();
					echo("<p><font color='red'>ERRO</font>: {$e->getMessage()}</p>");
				}
			?>
		</div>
	</body>
</html>