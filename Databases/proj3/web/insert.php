<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<link rel="stylesheet" href="style.css" />
	</head>
	<script type="text/javascript">
		function toggleForm() {  
			if(document.getElementById("tem_anomalia_traducao").checked == true) {
				document.getElementById("zona2").disabled = false;
				document.getElementById("lingua2").disabled = false;
			} else {
				document.getElementById("zona2").disabled = true;
				document.getElementById("lingua2").disabled = true; 
			}
		}  
	</script> 
	<body>
		<?php
			session_start();
            if (!isset($_SESSION['email'])) {
				header("Location: login/login.php");
			}
		?>
		
		<div class="sidenav">
			<a href="index.php">Home</a>
			<a class="active" href="">Inserir</a>
			<a href="edit.php">Editar</a>
			<a href="view.php">Visualizar</a>
			<a href="register.php">Registar</a>
			<a href="login/logout.php" class="logout">Logout</a>
		</div>

		<div class="main">
			<h1 id="title">Inserir</h1>

			<div class="row">
				<div class="column">
					<b>Local</b>
					<form action="insert/place.php" method="post">
						<p>
							<label>Latitude:</label>
							<input type="text" name="latitude">
						</p>
						<p>
							<label>Longitude:</label>
							<input type="text" name="longitude">
						</p>
						<p>
							<label>Nome:</label>
							<input type="text" name="nome">
						</p>
						<p> <input class="button buttonSmall" id="submit-btn" type="submit" value="Submeter"/> </p>
					</form>
				</div>
				
				<div class="column">
					<b>Item</b>
					<form action="insert/item.php" method="post">
						<p>
							<label>Descrição:</label>
							<input type="text" name="descricao">
						</p>
						<p>
							<label>Localização:</label>
							<input type="text" name="localizacao">
						</p>
						<p>
							<label>Local:</label>
							<select name="local">
							<?php
								require_once("db_class.php");

								try {
									//DB Init
									$db = new DB();
									$db->connect();
									
									$sql = "SELECT nome FROM local_publico";
									$result = $db->query($sql);

									foreach($result as $row) {
										$local = $row['nome'];
										printf('<option value="%1$s">%1$s</option>', $local);
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

				<div class="column">
					<b>Anomalia</b>
					<form action="insert/anomalia.php" method="post">
						<p>
							<label>Zona:</label>
							<input type="text" name="zona">
						</p>
						<p>
							<label>Imagem:</label>
							<input type="text" name="imagem">
						</p>
						<p>
							<label>Língua:</label>
							<input type="text" name="lingua">
						</p>
						<p>
							<label>Descrição:</label>
							<input type="text" name="descricao">
						</p>
						<p>
							<label>Anomalia Tradução?:</label>
							<input id="tem_anomalia_traducao" type="checkbox" onclick="toggleForm()" name="tem_anomalia_traducao" value="Yes">
						</p>
						<div id="anomalia_traducao">
							<p>
								<label>Zona 2:</label>
								<input id="zona2" type="text" name="zona2" disabled>
							</p>
							<p>
								<label>Língua 2:</label>
								<input id="lingua2" type="text" name="lingua2" disabled>
							</p>
						</div>
						<p> <input class="button buttonSmall" id="submit-btn" type="submit" value="Submeter"/> </p>
					</form>
				</div>
			</div>

			<div class="row">
				<div class="column">
					<b>Proposta de correção</b>
					<form action="insert/pcorrecao.php" method="post">
						<p>
							<label>Texto:</label>
							<input id="long-input" type="text" name="texto">
						</p>
						<p> <input class="button buttonSmall" id="submit-btn" type="submit" value="Submeter"/> </p>
					</form>
				</div>

				<div class="column">
					<b>Correção</b>
					<form action="insert/correcao.php" method="post">
						<p>
							<label>Anomalia:</label>
							<select name="anomalia_id">
							<?php
								try{
									require_once("db_class.php");
									
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
							<label>Proposta de correção:</label>
							<select name="nro">
							<?php
								try{
									require_once("db_class.php");
									
									//DB Init
									$db = new DB();
									$db->connect();
									
									$sql = "SELECT nro, texto FROM proposta_de_correcao";
									$result = $db->query($sql);

									foreach($result as $row) {
										$nro = $row['nro'];
										$texto = $row['texto'];
										printf('<option value="%1$s">%1$s - %2$s</option>', $nro, $texto);
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
