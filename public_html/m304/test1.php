<?php
require_once('manup_common.php');
$dsn = 'mysql:host=localhost;dbname=m304;charset=utf8';
$username = 'm304';
$password = 'm304pw';

try {
    $pdo = new PDO($dsn, $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}
$data = mk_pkg_c_index($pdo);
var_dump($data);
?>
 
