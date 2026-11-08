<?php
//////////////////////////////////////////////////
// m304ope
//
//  Create by mh@ys-lab.tech
//  Version: 1.01 (2026-11-07)
//
//////////////////////////////////////////////////

require_once('m304.inc');
require_once('lib_blk_ope.php');

try {
    $pdo = new PDO($dsn, $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

$pkg_id = $_GET['PKG_ID'] ?? '';
if ($pkg_id=='') {
    echo "<pre>You must define PKG_ID.</pre>\n";
    exit;
}

$post_exec = $_POST['EXEC'] ?? '';
$post_tbl  = $_POST['TBL'] ?? '';

// Insert
if ($post_exec === "追加" && $$post_tbl === "blk_ope") {
    $msg = insert_blk_ope($pdo,$_POST);
}

// Modify
if ($post_exec === "変更" && $post_tbl === "blk_ope") {
    $msg = modify_blk_ope($pdo,$_POST);
}
$msg = $msg ?? "";

// List 表示
$m304id = $pkg_id;
$rlyfilename = sprintf("rly-table/RLYs-%s.php",$m304id);
if (file_exists($rlyfilename)) {
    require_once($rlyfilename);
} else {
    for ($i=1;$i<9;$i++) {
        $RLYname[$i]="";
    }
}
$smarty->assign("RLYname",$RLYname);
$dt = select_blk_ope($pdo,"",$pkg_id);
$displ = "m304_blk_ope.tpl";
$smarty->assign("return_PTR",$pkg_id);
$smarty->assign("data",$dt);
$smarty->assign("rlystatus",constant("RLY_STAT_SEL"));
$smarty->assign("msg",$msg);
$smarty->display($displ);
exit;
