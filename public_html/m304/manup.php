<?php
require_once('m304.inc');

try {
    $pdo = new PDO($dsn, $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}

// データベース接続とSmartyの設定コードを含めます。

$tag = @$_GET['T'];
$msg = "";

switch( @$_POST['EXEC'] ) {
case '追加':
    $tag = $_POST['TBL'];
    switch($tag) {
    case 'custom':
        $msg = insert_tbl_custom($pdo,$_POST);
        break;
    case 'blk_a':
        $msg = insert_blk_a($pdo,$_POST);
        break;
    case 'blk_b':
        printf("<pre>%s</pre>",print_r($_POST,1));
        exit;
        $msg = insert_blk_b($pdo,$_POST);
        break;
    case 'blk_c':
        $msg = insert_blk_c($pdo,$_POST);
        break;
    case 'blk_d':
        $msg = insert_blk_d($pdo,$_POST);
        break;
    case 'blk_ope':
        $msg = insert_blk_ope($pdo,$_POST);
        break;
    case 'pkg_c':
        $msg = insert_pkg_c($pdo,$_POST);
        break;
    }
    break;

case '変更':
    $tag = $_POST['TBL'];
    $seq = $_POST['SEQ'];
    switch($tag) {
    case 'custom':
        $msg = modify_tbl_custom($pdo,$_POST);
        break;
    case 'blk_a':
        $msg = modify_blk_a($pdo,$_POST);
        break;
    case 'blk_c':
        $msg = modify_blk_c($pdo,$_POST);
        break;
    case 'blk_d':
        $msg = modify_blk_d($pdo,$_POST);
        break;
    case 'pkg_c':
        $msg = modify_pkg_c($pdo,$_POST);
        break;
    case 'blk_ope':
        $msg = modify_blk_ope($pdo,$_POST);
        break;
    }
    break;

case '削除':
    $tag = $_POST['TBL'];
    $seq = $_POST['SEQ'];
    switch($tag) {
    case 'custom':
        $query   = sprintf("delete from tbl_custom WHERE SEQ=%d",$seq);
        break;
    case 'blk_a':
        $query   = sprintf("delete from blk_a WHERE SEQ=%d",$seq);
        break;
    case 'blk_c':
        $query  = sprintf("DELETE FROM blk_c WHERE PKG_ID=%d",$_POST['PKGID']);
        break;
    case 'blk_d':
        $query  = sprintf("DELETE FROM blk_d WHERE PKG_ID=%d",$_POST['PKG_ID']);
        break;
    case 'blk_ope':
        $query   = sprintf("delete from blk_ope where OPE_ID=%d;delete from ope_c where OPE_ID=%d",
                           $_POST['ID'],$_POST['ID']);
        break;
    case 'pkg_c':
        $query   = sprintf("delete from pkg_c WHERE SEQ=%d",$seq);
        break;
    }
    try {
        $reqly = $pdo->query($query);
        $msg = "DELETE DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    
    break;
}

//////////////////////////////////////////////////
// 一覧表示用の既存データの採取
//////////////////////////////////////////////////
switch($tag) {
case 'custom':
    $data  = select_tbl_custom($pdo);
    $displ = 'custom_list.tpl';
    break;
case 'blk_a':
    $data = select_blk_a($pdo,$smarty);
    $displ = 'blk_a_list.tpl';
    break;
case 'blk_b':
    $data = select_blk_b($pdo,$smarty);
    $displ = 'blk_b_list.tpl';
    break;
case 'blk_c':
    $data = select_blk_c($pdo,$smarty);
    $msg  = $data['msg'];
    $displ = 'blk_c_list.tpl';
    break;
case 'blk_d':
    $data = select_blk_d($pdo,$smarty);
    //    $msg  = $data['msg'];
    $displ = 'blk_d_list.tpl';
    break;
case 'blk_ope':
    $data = select_blk_ope($pdo,$smarty);
    $displ = 'blk_ope_list.tpl';
    break;
case 'pkg_c':
    $data = select_pkg_c($pdo);
    $displ = 'pkg_c_list.tpl';
    break;
default:
    exit;
}

$smarty->assign('msg',$msg);
$smarty->assign('data', $data);
$smarty->display($displ);
exit;
?>
