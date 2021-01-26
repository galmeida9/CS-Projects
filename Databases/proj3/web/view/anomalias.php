<html>
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="stylesheet" href="../style.css" />
        <link rel="stylesheet" href="../modal.css" />
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
			<h1 id="title">Anomalias entre dois locais</h1>
            
            <form class="back-btn" action="../view.php">
                <input class="button buttonSmall" type="submit" value="Voltar" />
            </form>

            <div class="table">
                <?php
                    require("../db_class.php");
                    $result = array();

                    $local1 = $_REQUEST['local1'];
                    $local2 = $_REQUEST['local2'];

                    if ($local1 == $local2) {
                        echo("<p><font color='red'>ERRO</font>: Selecione locais distintos.</p>");
                        exit();
                    }

                    try {
                        //DB Init
                        $db = new DB();
                        $db->connect();

                        $db->beginTransaction();

                        $sql = "SELECT latitude, longitude FROM local_publico WHERE nome=:local1;";
                        $params = [':local1' => $local1];
                        $local_table = $db->query($sql, $params);
                        $local = $local_table->fetch();
                        $lat1 = $local['latitude'];
                        $lon1 = $local['longitude'];

                        $sql = "SELECT latitude, longitude FROM local_publico WHERE nome=:local2;";
                        $params = [':local2' => $local2];
                        $local_table = $db->query($sql, $params);
                        $local = $local_table->fetch();
                        $lat2 = $local['latitude'];
                        $lon2 = $local['longitude'];

                        $coord = [min($lat1, $lat2), max($lat1, $lat2), min($lon1, $lon2), max($lon1, $lon2)];

                        $sql = "SELECT anomalia_id, item_id FROM incidencia;";
                        $incidencias = $db->query($sql);

                        foreach($incidencias as $row) {
                            $id = $row['item_id'];
                            $sql = "SELECT latitude, longitude FROM item WHERE id=:id;";
                            $params = [':id' => $id];
                            $itens = $db->query($sql, $params);

                            $item = $itens->fetch();
                            $latitude = $item['latitude'];
                            $longitude = $item['longitude'];
                            if ($coord[0] <= $latitude && $latitude <= $coord[1]
                            && $coord[2] <= $longitude && $longitude <= $coord[3]) {
                                array_push($result, $row['anomalia_id']);
                            }
                        }

                        echo("<table border=\"1\" cellspacing=\"5\">\n");
                        echo("<tr><td><b>ID</b></td><td><b>Tipo</b></td><td><b>Zona</b></td><td><b>Zona 2</b></td><td><b>Imagem</b></td>");
                        echo("<td><b>Língua</b></td><td><b>Língua 2</b></td><td><b>Data/Hora</b></td><td><b>Descrição</b></td></tr>\n");
                        foreach($result as $id) {
                            $sql = "SELECT * FROM anomalia WHERE id=:id;";
                            $params = [':id' => $id];
                            $anomalias = $db->query($sql, $params);
                            $anomalia = $anomalias->fetch();

                            echo("<tr>\n");
                            echo("<td>{$anomalia['id']}</td>\n");

                            if ($anomalia['tem_anomalia_redacao'] == 1) {
                                echo("<td>Redação</td>\n");
                            } else {
                                echo("<td><b>Tradução</b></td>\n");
                            }

                            echo("<td>{$anomalia['zona']}</td>\n");

                            $sql = "SELECT zona2, lingua2 FROM anomalia_traducao WHERE id=:id;";
                            $params = [':id' => $anomalia['id']];
                            $result2 = $db->query($sql, $params);

                            $row2 = $result2->fetch();
                            if ($row2['zona2'] == null) {
                                echo("<td>-</td>\n");
                            } else {
                                echo("<td>{$row2['zona2']}</td>\n");
                            }

                            echo("<td><a onclick='showImg(\"{$anomalia['imagem']}\")'>Ver</a></td>\n");
                            echo("<td>{$anomalia['lingua']}</td>\n");

                            if ($row2['lingua2'] == null) {
                                echo("<td>-</td>\n");
                            } else {
                                echo("<td>{$row2['lingua2']}</td>\n");
                            }

                            echo("<td>{$anomalia['ts']}</td>\n");
                            echo("<td>{$anomalia['descricao']}</td>\n");                     
                            echo("</tr>\n");
                        }
                        echo("</table>\n");

                        $db->commit();
                        
                        // Cleaning Up
                        $result = null;
                        unset($db);
                    }
                    catch (PDOException $e)
                    {
                        $db->rollBack();
                        echo("<p><font color='red'>ERRO</font>: {$e->getMessage()}</p>");
                    }
                ?>
            </div>
            <div id="myModal" class="modal">
                <span class="close">&times;</span>
                <img class="modal-content" id="img01">
            </div>
		</div>
    </body>
    
    <script src="../modal.js"></script>
</html>
