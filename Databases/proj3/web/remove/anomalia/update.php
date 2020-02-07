<html>
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<link rel="stylesheet" href="../../style.css" />
	</head>
	<body>
		<?php
			session_start();
			if (!isset($_SESSION['email'])) {
				header("Location: ../../login/login.php");
			}
		?>

		<div>
			<h1 id="title">Remover anomalia</h1>
			<form class="back-btn" action="../../edit.php">
			    <input class="button buttonSmall" type="submit" value="Sair" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../../db_class.php");
				$id = $_REQUEST['id'];

				try {
					//DB Init
					$db = new DB();
					$db->connect();

					$sql = "DELETE FROM anomalia WHERE id=:id";
					$params = [':id' => $id];
					$result = $db->query($sql, $params);

					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);

					echo("<p>Anomalia removida com sucesso.</p>");
				}
				catch (PDOException $e)
				{
					echo("<p><font color='red'>ERRO</font>: {$e->getMessage()}</p>");
				}
			?>
		</div>
	</body>
</html>
