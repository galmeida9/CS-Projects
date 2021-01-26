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
			<h1 id="title">Editar correção</h1>
			<form class="back-btn" action="../../edit.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>
		</div>

		<div class="table">
			<?php
				require("../../db_class.php");
				$nro = $_REQUEST['nro'];
				$texto = $_REQUEST['texto'];

				if (!isset($texto) || $texto == '') {
					echo("<p><font color='red'>ERRO</font>: Não foi especificada um texto.</p>");
					exit();
				}

				try {
					//DB Init
					$db = new DB();
					$db->connect();
					
					$sql = "UPDATE proposta_de_correcao SET texto = :texto, data_hora = now() WHERE nro = :nro;";
					$params = [':nro' => $nro, ':texto' => $texto];
					$result = $db->query($sql, $params);

					echo("<p>Proposta de correção editada com sucesso.</p>");

					// Cleaning Up
					$result = null;
					$db->disconnect();
					unset($db);
				}
				catch (PDOException $e)
				{
					echo("<p><font color='red'>ERRO</font>: {$e->getMessage()}</p>");
				}
			?>
		</div>
	</body>
</html>