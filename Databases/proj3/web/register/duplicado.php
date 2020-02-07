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
			<h1 id="title">Registar duplicado</h1>
			<form class="back-btn" action="../register.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../db_class.php");
				$item1 = $_REQUEST['item1'];
				$item2 = $_REQUEST['item2'];

				if (!isset($item1) || $item1 == '') {
					echo("<p><font color='red'><font color='red'>ERRO</font></font>: O primeiro item não foi especificado.</p>");
					exit();
				}

				if (!isset($item2) || $item2 == '') {
					echo("<p><font color='red'><font color='red'>ERRO</font></font>: O segundo item não foi especificado.</p>");
					exit();
				}

				if ($item1 == $item2) {
					echo("<p><font color='red'><font color='red'>ERRO</font></font>: Selecione itens distintos.</p>");
					exit();
				}

				try {
					//DB Init
					$db = new DB();
					$db->connect();

					$sql = "INSERT INTO duplicado (item1, item2)
						VALUES (:item1, :item2)";
					$params = [':item1' => $item1, ':item2' => $item2];
					$result = $db->query($sql, $params);

					echo("<p>Duplicado registado com sucesso.</p>");
					
					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);
				}
				catch (PDOException $e)
				{
					echo("<p><font color='red'><font color='red'>ERRO</font></font>: {$e->getMessage()}</p>");
				}
			?>
		</div>
	</body>
</html>