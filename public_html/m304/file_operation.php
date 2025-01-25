<?php
require_once('m304.inc');

try {
    $pdo = new PDO($dsn, $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

$m304id = @$_GET['ID'];
if ($m304id=='') {
    echo "<pre>You must define ID.</pre>\n";
    exit;
}

if (@$_POST['OPEB']=="L") {
    load_file($pdo,$m304id,$_POST['FILENAME']);
}

if (@$_POST['OPEB']=="D") {
    delete_save_file($pdo,$m304id,$_POST['FILENAME']);
}

$data_a = select_blk_a($pdo,"",$m304id);
if ($data_a===false) {
    echo "Not found M304ID=".$m304id."\n";
    exit;
}
$save_dir = select_save_dir($pdo,$m304id);
if ($save_dir===false) {
    $msg = "保存されたファイルはありません。";
} else {
    $msg = sprintf("以下の %d 個見つかりました。",count($save_dir));
}

$data_custom = select_tbl_custom($pdo,$data_a['COM_ID'],$data_a['FARM_ID']);
$displ = "file_operation.tpl";
$smarty->assign("data_a",$data_a);
$smarty->assign("save_dir",$save_dir);
$smarty->assign("msg",$msg);

$smarty->display($displ);
exit;

?>
