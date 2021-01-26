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

		<div class="sidenav">
			<a href="../../index.php">Home</a>
			<a href="../../insert.php">Inserir</a>
			<a href="../../edit.php">Editar</a>
			<a href="../../view.php">Visualizar</a>
			<a href="../../register.php">Registar</a>
			<a href="../../login/logout.php" class="logout">Logout</a>
		</div>

		<div id="tr" class="main">
			<h1 id="title">Remover proposta de correção</h1>
            
            <form class="back-btn" action="../../edit.php">
			    <input class="button buttonSmall" type="submit" value="Voltar" />
			</form>

            <div class="table">
				<?php
					require("../../db_class.php");
					
					try {
						//DB Init
						$db = new DB();
						$db->connect();

						$sql = "SELECT * FROM proposta_de_correcao;";
						$result = $db->query($sql);

						echo("<table border=\"1\" cellspacing=\"5\">\n");
						echo("<tr><td><b>Número</b></td><td><b>Email</b></td>");
						echo("<td><b>Data/Hora</b></td><td><b>Texto</b></td></tr>\n");
						foreach($result as $row) {
							echo("<tr>\n");
							echo("<td>{$row['nro']}</td>\n");
							echo("<td>{$row['email']}</td>\n");
							echo("<td>{$row['data_hora']}</td>\n");
							echo("<td>{$row['texto']}</td>\n");
							echo("<td><a href=\"update.php?nro={$row['nro']}\">Remover</a></td>\n");
							echo("</tr>\n");
						}
						echo("</table>\n");

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
		</div>

	</body>
</html>