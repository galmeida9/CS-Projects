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
			<a href="index.php">Home</a>
			<a href="insert.php">Inserir</a>
			<a href="edit.php">Editar</a>
			<a href="view.php">Visualizar</a>
			<a class="active" href="">Registar</a>
			<a href="login/logout.php" class="logout">Logout</a>
		</div>

		<div id="tr" class="main">
			<h1 id="title">Registar</h1>

			<div class="row">
				<div class="column">
					<b>Incidência</b>
					<form action="register/incidencia.php" method="post">
						<p>
							<label for="b">Anomalia:</label>
							<select name="anomalia_id">
							<?php
								require_once("db_class.php");

								try {
									//DB Init
									$db = new DB();
									$db->connect();
									
									$sql = "SELECT id, descricao FROM anomalia";
									$result = $db->query($sql);

									foreach($result as $row) {
										$id = $row['id'];
										$descricao = $row['descricao'];
										printf('<option value="%1$s">%1$s - %2$s</option>', $id, $descricao);
									}
									
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
							</select>
						</p>
						<p>
							<label for="b">Item:</label>
							<select name="item_id">
							<?php
								require_once("db_class.php");

								try {									
									//DB Init
									$db = new DB();
									$db->connect();
									
									$sql = "SELECT id, descricao FROM item";
									$result = $db->query($sql);

									foreach($result as $row) {
										$id = $row['id'];
										$descricao = $row['descricao'];
										printf('<option value="%1$s">%1$s - %2$s</option>', $id, $descricao);
									}
									
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
							</select>
						</p>
						<p> <input class="button buttonSmall" id="submit-btn" type="submit" value="Submeter"/> </p>
					</form>
				</div>
			</div>

			<div class="row">
				<div class="column">
					<b>Duplicado</b>
					<form action="register/duplicado.php" method="post">
						<p>
							<label for="b">Item 1:</label>
							<select name="item1">
							<?php
								require_once("db_class.php");

								try {
									//DB Init
									$db = new DB();
									$db->connect();
									
									$sql = "SELECT id, descricao FROM item";
									$result = $db->query($sql);

									foreach($result as $row) {
										$id = $row['id'];
										$descricao = $row['descricao'];
										printf('<option value="%1$s">%1$s - %2$s</option>', $id, $descricao);
									}
									
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
							</select>
						</p>
						<p>
							<label for="b">Item 2:</label>
							<select name="item2">
							<?php
								require_once("db_class.php");
								
								try {
									//DB Init
									$db = new DB();
									$db->connect();
									
									$sql = "SELECT id, descricao FROM item";
									$result = $db->query($sql);

									foreach($result as $row) {
										$id = $row['id'];
										$descricao = $row['descricao'];
										printf('<option value="%1$s">%1$s - %2$s</option>', $id, $descricao);
									}
									
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
							</select>
						</p>
						<p> <input class="button buttonSmall" id="submit-btn" type="submit" value="Submeter"/> </p>
					</form>
				</div>
			</div>
		</div>
	</body>
</html>
