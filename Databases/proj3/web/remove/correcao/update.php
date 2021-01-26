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

			$email = $_SESSION['email'];
		?>

		<div>
			<h1 id="title">Remover correção</h1>
			<form class="back-btn" action="../../edit.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../../db_class.php");
				$nro = $_REQUEST['nro'];

				try{
					//DB Init
					$db = new DB();
					$db->connect();

					$db->beginTransaction();

					$sql = "SELECT * FROM utilizador_qualificado WHERE email=:email";
					$params = [':email' => $email];
					$result = $db->query($sql, $params);
					$user = $result->fetch();

					if (!$user) {
						echo("<p><font color='red'>ERRO</font>: Ação reservada a utilizadores qualificados.</p>");
						$db->rollBack();
						exit();
					}

					$sql = "DELETE FROM correcao WHERE nro=:nro";
					$params = [':nro' => $nro];
					$result = $db->query($sql, $params);

					$db->commit();
					echo("<p>Correção removida com sucesso.</p>");

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