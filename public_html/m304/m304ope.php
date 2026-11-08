<?php
require_once('m304.inc');
//require_once('maketestdata.php');
require_once('lib_blk_ope.php');

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
if ((@$_POST['EXEC']=="追加")&&(@$_POST['TBL']=="blk_ope")) {
    //echo "<pre>\n";
    //print_r($_POST);
    //echo "</pre>\n";
    $msg = insert_blk_ope($pdo,$_POST);
}

// Modify
if ((@$_POST['EXEC']=="変更")&&(@$_POST['TBL']=="blk_ope")) {
    $msg = modify_blk_ope($pdo,$_POST);
}

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
echo "<pre>\n";
print_r($dt);
?>
