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
				header("Location: ../login.php");
			}
		?>

		<div class="sidenav">
			<a href="../index.php">Home</a>
			<a href="../insert.php">Inserir</a>
			<a href="../edit.php">Editar</a>
			<a href="../view.php">Visualizar</a>
			<a href="../register.php">Registar</a>
			<a href="../login/logout.php" class="logout">Logout</a>
		</div>

		<div id="tr" class="main">
			<h1 id="title">Utilizadores</h1>
            
            <form class="back-btn" action="../view.php">
                <input class="button buttonSmall" type="submit" value="Voltar" />
            </form>

            <div class="table">
				<?php
					require("../db_class.php");

					try {
						//DB Init
						$db = new DB();
						$db->connect();

						$sql = "SELECT email FROM utilizador;";
						$result = $db->query($sql);

						echo("<table border=\"1\">\n");
						echo("<tr><td><b>Email</b></td></tr>\n");
						foreach($result as $row) {
							echo("<tr><td>");
							echo($row['email']);
							echo("</td></tr>\n");
						}
						echo("</table>\n");
						
						// Cleaning Up
						$result = null;
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
