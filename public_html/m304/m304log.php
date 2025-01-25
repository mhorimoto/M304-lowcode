<?php
require_once('m304.inc');

// Log events notified via the http interface.

$m304id = @$_GET['ID'];
if ($m304id=='') {
    echo "<pre>You must define ID.</pre>\n";
    exit;
}

try {
    $pdo = new PDO($dsn, $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

$log = new M304log($pdo,$m304id);

$facility = 7;
$tbl = null;
$msg0 = null;
$msg1 = null;
$msg2 = null;


if (isset($_POST['FACILITY'])) {
    if (is_numeric($_POST['FACILITY'])) {
        $facility = $_POST['FACILITY'];
    }
}
$tbl  = (isset($_POST['TBL']))  ? $_POST['TBL'] : null;
$msg0 = (isset($_POST['MSG0'])) ? $_POST['MSG0'] : null;
$msg1 = (isset($_POST['MSG1'])) ? $_POST['MSG1'] : null;
$msg2 = (isset($_POST['MSG2'])) ? $_POST['MSG2'] : null;
$log->store($facility,$tbl,$msg0,$msg1,$msg2);
exit;
?>
