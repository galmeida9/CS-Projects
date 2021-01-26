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
			<h1 id="title">Registar incidência</h1>
			<form class="back-btn" action="../register.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../db_class.php");
				$anomalia_id = $_REQUEST['anomalia_id'];
				$item_id = $_REQUEST['item_id'];

				if (!isset($anomalia_id) || $anomalia_id == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada uma anomalia.</p>");
					exit();
				}

				if (!isset($item_id) || $item_id == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificado um item.</p>");
					exit();
				}

				try {
					//DB Init
					$db = new DB();
					$db->connect();

					$sql = "INSERT INTO incidencia (anomalia_id, item_id, email)
						VALUES (:anomalia_id, :item_id, :email)";
					$params = [':anomalia_id' => $anomalia_id, ':item_id' => $item_id, ':email' => $email];
					$result = $db->query($sql, $params);

					echo("<p>Incidência registada com sucesso.</p>");
					
					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);
				}
				catch (PDOException $e)
				{
					echo("<p><font color='red'><font color='red'>ERRO</font></font>: Já existe uma incidência registada para esta anomalia.</p>");
				}
			?>
		</div>
	</body>
</html>