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
			<h1 id="title">Anomalias numa vizinhança</h1>
            
            <form class="back-btn" action="../view.php">
                <input class="button buttonSmall" type="submit" value="Voltar" />
            </form>

            <div class="table">
                <?php
                    require("../db_class.php");
                    $lat = $_REQUEST['lat'];
                    $long = $_REQUEST['long'];
                    $dx = $_REQUEST['dx'];
                    $dy = $_REQUEST['dy'];
                    $anomId = array();

                    if(!isset($lat) || $lat == '') {
                        echo("<p>ERRO: Não foi especificada uma latitude.</p>");
                        exit();
                    }

                    if(!isset($long) || $long == '') {
                        echo("<p>ERRO: Não foi especificada uma longitude.</p>");
                        exit();
                    }

                    if(!isset($dx) || $dx == '') {
                        echo("<p>ERRO: O parâmetro dx não foi especificado.</p>");
                        exit();
                    }

                    if(!isset($dy) || $dy == '') {
                        echo("<p>ERRO: O parâmetro dy não foi especificado.</p>");
                        exit();
                    }

                    try {
                        //DB Init
                        $db = new DB();
                        $db->connect();

                        $db->beginTransaction();

                        $sql = "SELECT * FROM incidencia";
                        $result = $db->query($sql);

                        foreach($result as $row) {
                            $id = $row['anomalia_id'];
                            $sql = "SELECT latitude, longitude FROM item WHERE id = :id;";
                            $params = [':id' => $id];
                            $itens = $db->query($sql, $params);

                            $item = $itens->fetch();
                            if ($item['latitude'] > $lat - $dx && $item['latitude'] < $lat + $dx &&
                                $item['longitude'] > $long - $dy && $item['longitude'] < $long + $dy) {
                                    array_push($anomId, $row['anomalia_id']);
                            }
                        }

                        if (count($anomId) > 0) {
                            echo("<table border=\"1\" cellspacing=\"5\">\n");
                            echo("<tr><td><b>ID</b></td><td><b>Tipo</b></td><td><b>Zona</b></td><td><b>Zona 2</b></td><td><b>Imagem</b></td>");
                        echo("<td><b>Língua</b></td><td><b>Língua 2</b></td><td><b>Data/Hora</b></td><td><b>Descrição</b></td></tr>\n");
                            foreach($anomId as $id) {
                                $sql = "SELECT * FROM anomalia WHERE id = :id;";
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
                        }
                        else {
                            echo("<p>Não foram encontradas Anomalias</p>");
                        }

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
