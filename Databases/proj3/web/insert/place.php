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
			<h1 id="title">Inserir local</h1>
			<form class="back-btn" action="../insert.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../db_class.php");
				$latitude = $_REQUEST['latitude'];
				$longitude = $_REQUEST['longitude'];
				$nome = $_REQUEST['nome'];

				if(!isset($latitude) || $latitude == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada uma latitude.</p>");
					exit();
				}

				if(!isset($longitude) || $longitude == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada uma longitude.</p>");
					exit();
				}

				if(!isset($nome) || $nome == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificado um nome.</p>");
					exit();
				}

				try{
					//DB Init
					$db = new DB();
					$db->connect();

					$sql = "INSERT INTO local_publico (latitude, longitude, nome)
						VALUES (:latitude, :longitude, :nome)";
					$params = [':latitude' => $latitude, ':longitude' => $longitude, ':nome' => $nome];
					$result = $db->query($sql, $params);

					echo("<p>Local inserido com sucesso.</p>");
					
					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);
				}
				catch (PDOException $e)
				{
					echo("<p><font color='red'>ERRO</font>: latitude e longitude têm de ser números.</p>");
				}
			?>
		</div>
	</body>
</html>