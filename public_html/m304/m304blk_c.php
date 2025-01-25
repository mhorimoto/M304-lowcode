<?php
require_once('m304.inc');

try {
    $pdo = new PDO($dsn, $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

$pkg_id = @$_GET['PKG_ID'];
if ($pkg_id=='') {
    echo "<pre>You must define PKG_ID.</pre>\n";
    exit;
}

// Insert
if ((@$_POST['EXEC']=="新規追加")&&(@$_POST['TBL']=="blk_c")) {
    //echo "<pre>\n";
    //print_r($_POST);
    //echo "</pre>\n";
    $msg = modify_blk_c_with_pkgid_id($pdo,$_POST,"INSERT");
}

// Modify
if ((@$_POST['EXEC']=="変更")&&(@$_POST['TBL']=="blk_c")) {
    $msg = modify_blk_c_with_pkgid_id($pdo,$_POST);
}

    // List 表示
$dt = select_blk_c($pdo,"",$pkg_id);
if ($dt[0]['EMPTY']) {
    $smarty->assign("EMPTY",1);
    $dt[0]['PKG_ID'] = $pkg_id;
    $dt[0]['M304ID'] = $pkg_id;
    $dt[0]['ID'] = 0;
} else {
    $smarty->assign("EMPTY",0);
}

$displ = "m304_blk_c.tpl";
$smarty->assign("return_PTR",$pkg_id);
$smarty->assign("data",$dt);
$smarty->assign("data_c_maxrows",count($dt));
$smarty->assign("lifecount_sel",constant("LIFECOUNT_SEL"));
$smarty->assign("msg",$msg);
$smarty->display($displ);
exit;
?>
