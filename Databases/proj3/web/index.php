<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<link rel="stylesheet" href="style.css" />
	</head>
	<body>
		<?php
			session_start();
            if (!isset($_SESSION['email'])) {
				header("Location: login/login.php");
			}
		?>

		<div class="sidenav">
			<a class="active" href="">Home</a>
			<a href="insert.php">Inserir</a>
			<a href="edit.php">Editar</a>
			<a href="view.php">Visualizar</a>
			<a href="register.php">Registar</a>
			<a href="login/logout.php" class="logout">Logout</a>
		</div>

		<div id="tr" class="main">
			<h1 id="title">Translate Right</h1>
		</div>
		
		<div id="repor">
			<h3 id="indexTitle">Repor Base de Dados</h3>
			<center>
				<form method="post">
    				<input class="button" type="submit" name="repor" value="Repor dados" />
				</form>
			</center>
		</div>

		<?php
			if (isset($_POST['repor'])) {
				require("db_class.php");
				$db = new DB();
				$db->connect();
				$db->executeFileQuery("schema.sql");
				$db->executeFileQuery("populate.sql");
				
				// Cleaning Up
				$db->disconnect();
				unset($db);
				header("Location: index.php");
			}
		?>
	</body>
</html>