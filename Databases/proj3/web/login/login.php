<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="stylesheet" href="login.css" />
    </head>

    <body>
        <?php
            session_start();
        ?>

        <form action="" method="post" id="login">
            <div class="imgcontainer">
                <img src="../resources/logo.png" alt="logo" class="logo">
            </div>
          
            <div class="container">
                <label for="uname"><b>Email</b></label>
                <input type="text" placeholder="Enter email" name="uname" required>
            
                <label for="psw"><b>Password</b></label>
                <input type="password" placeholder="Enter password" name="psw" required>
            
                <button type="submit">Login</button>
            </div>
        </form>

        <?php
            require("../db_class.php");
            $username = $_REQUEST['uname'];
            $psw = $_REQUEST['psw'];

            try {
                //DB Init
                $db = new DB();
                $db->connect();

                $sql = "SELECT psw FROM utilizador WHERE email = :username;";
                $params = [':username' => $username];
                $result = $db->query($sql, $params);

                $isEmpty = 1;
                foreach($result as $row) {
                    $realPsw = $row['psw'];
                    if ($realPsw == $psw) {
                        $_SESSION['email'] = $username;
                        header("Location: ../index.php");
                    }
                    else {
                        echo("<center class='error'>Wrong password.</center>");
                    }
                    $isEmpty = 0;
                    break;
                }
                if ($isEmpty && $username != "") {
                    echo("<center class='error'>Wrong email.</center>");
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
    </body>
</html>