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

			$email = $_SESSION['email'];
		?>

		<div>
			<h1 id="title">Inserir correção</h1>
			<form class="back-btn" action="../insert.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../db_class.php");
				$anomalia_id = $_REQUEST['anomalia_id'];
				$nro = $_REQUEST['nro'];

				if(!isset($anomalia_id) || $anomalia_id == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificado um ID de anomalia.</p>");
					exit();
				}

				try{
					//DB Init
					$db = new DB();
					$db->connect();

					$sql = "INSERT INTO correcao (email, nro, anomalia_id)
						VALUES (:email, :nro, :anomalia_id)";
					$params = [':email' => $email, ':nro' => $nro, ':anomalia_id' => $anomalia_id];
					$result = $db->query($sql, $params);

					echo("<p>Correção inserida com sucesso.</p>");

					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);
				}
				catch (PDOException $e)
				{
					echo("<p><font color='red'>ERRO</font>: A proposta de correção selecionada não foi feita por este utilizador.</p>");
				}
			?>
		</div>
	</body>
</html>