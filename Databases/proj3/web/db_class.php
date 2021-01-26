<?php

require_once("db_credentials.php");

class DB {

    var $HOST = HOST;
    var $DATABASE = DATABASE;
    var $USER = USER;
    var $PORT = PORT;
    var $PASSWORD = PASSWORD;
    var $dbObj;
    var $debug;

    public function __construct($debug = True) {
        $this->debug = $debug;
    }

    public function connect() {
        $this->dbObj = new PDO("pgsql:host=$this->HOST;dbname=$this->DATABASE", $this->USER, $this->PASSWORD);
        $this->dbObj->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }

    public function disconnect() {
        $this->dbObj = null;
    }

    public function query($sql, $params = []) {
        $result = $this->dbObj->prepare($sql);
        $result->execute($params);
        return $result;
    }

    public function beginTransaction() {
        $this->dbObj->beginTransaction();
    }

    public function commit() {
        $this->dbObj->commit();
    }

    public function rollBack() {
        $this->dbObj->rollBack();
    }

    public function queryTransaction($sql) {
        $result = $this->dbObj->prepare($sql);
        $result->execute();
        return $result;
    }

    public function debug_to_console($data) {
        if (!$this->debug) return;
        $output = $data;
        if (is_array($output))
            $output = implode(',', $output);
    
        echo("<script>console.log('DB Debug: " . $output . "' );</script>");
    }

    public function executeFileQuery($filename) {
        $file = fopen("../$filename", "r");
        $query = null;
        if ($file) {
            while (($line = fgets($file)) !== false) {
                if (strpos($line, '--') !== false) {
                    // Do nothing
                }
                else if ('' === trim($line)) {
                    if (!is_null($query)) {
                        $this->query($query);
                        $query = null;
                    }
                }
                else if (strpos($line, 'drop') !== false) {
                    $this->query($line);
                }
                else if (strpos($line, 'insert') !== false) {
                    $this->query($line);
                }
                else {
                    if (is_null($query)) {
                        $query = $line;
                    }
                    else {
                        $query = $query . $line;
                    }
                }
            }
            fclose($file);
        }
        else {
            echo("<p>error opening file.</p>");
        }
    }
}

?>