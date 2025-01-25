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

$log = new M304log($pdo,$m304id);

$log->store(8,'save_dir','','OKOK');
$data_a = select_blk_a($pdo,"",$m304id);
if ($data_a===false) {
    echo "Not found M304ID=".$m304id."\n";
    exit;
}

$filesaveflag = false;
if (@$_POST['FILESAVE']) {
    if (@$_POST['FILENAME']) {
        if (@$_POST['COMMENT']) {
            if (@$_POST['OWNER']) {
                $filesaveflag = true;
                $fe = save_file($pdo,$m304id,$_POST);
                if ($fe) {
                    echo "OK";
                } else {
                    echo "Fail";
                }
            }
        }
    }
}

switch(@$_POST['cmd']) {
    case '削除':
        switch($_POST['DATA']) {
            case 'BLK_B':
                delete_blk_b($pdo,$_POST);
                break;
            case 'BLK_D':
                delete_blk_d($pdo,$_POST);
                break;
        }
        break;
    case '変更':
        switch($_POST['DATA']) {
            case 'BLK_B':
                modify_blk_b_with_pkgid_id($pdo,$_POST,"");
                break;
            case 'BLK_D':
                modify_blk_d_with_pkgid_id($pdo,$_POST,"");
                break;
        }
        break;
    case '新規追加':
        switch($_POST['DATA']) {
            case 'BLK_B':
                $r = print_r($_POST,1);
                $log->store(8,'blk_b','INSERT',$r);
                modify_blk_b_with_pkgid_id($pdo,$_POST,"INSERT");
                break;
            case 'BLK_D':
                modify_blk_d_with_pkgid_id($pdo,$_POST,"INSERT");
                break;
        }
        break;
    case 'CONV':
    case 'IH変換':
        switch($_POST['DATA']) {
            case 'BLK_B':
           //     conv2ihex($pdo,$_POST['DATA'],$_POST['PKG_ID']);
                exit;
        }
    default:
    //    echo "<pre>\n";
    //    print_r($_POST);
    //    echo "</pre>\n";
        break;
}

$data_custom = select_tbl_custom($pdo,$data_a['COM_ID'],$data_a['FARM_ID']);
$data_b = select_blk_b_with_pkgid($pdo,$data_a['PKG_ID']);
$data_opec = mk_select_ope_c($pdo,$data_a['PKG_ID']);
$data_blk_c = mk_select_blk_c($pdo,$data_a['PKG_ID'],"",$select_ce,$select_cj);
$data_d = select_blk_d($pdo,"",$data_a['PKG_ID']);
$data_save_dir = select_save_dir($pdo,$data_a['M304ID']);
$displ = "m304body2b.tpl";

$smarty->assign("save_dir",$data_save_dir);
$smarty->assign("rlystatus",constant("RLY_STAT_SEL"));
$smarty->assign("lifecount_sel",constant("LIFECOUNT_SEL"));
$smarty->assign("lv_sel",constant("LV_SEL"));
$smarty->assign("blk_b_maxrows",count($data_b));
$smarty->assign("data_a",$data_a);
$smarty->assign("data_b",$data_b);
//print("data_b_maxrows=".count($data_b));
$smarty->assign("data_blk_c",$data_blk_c);
$smarty->assign("select_ce",$select_ce);
$smarty->assign("select_cj",$select_cj);
$smarty->assign("select_rlyope",constant("SELECT_RLYOPE"));
//$smarty->assign("select_cmbope",constant("SELECT_CMBOPEE")); 
$smarty->assign("select_cmbope",constant("SELECT_CMBOPEJ"));
$smarty->assign("ope_c_name_list",$data_opec);
$smarty->assign("data_custom",$data_custom);
$smarty->assign("data_d_maxrows",count($data_d));
$smarty->assign("data_d",$data_d);
$smarty->display($displ);
exit;
?>
