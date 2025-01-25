<?php

define("RLY_STAT_SEL",array(0=>"不変",1=>"時間接続",2=>"遮断保持",3=>"接続保持"));
define("LIFECOUNT_SEL",array(3=>"3秒",30=>"30秒",180=>"180秒"));
define("LV_SEL",array(1=>"A-1S-0",2=>"A-1S-1",3=>"A-10S-0",4=>"A-10S-1",
                      5=>"A-1M-0",6=>"A-1M-1",7=>"B-0",8=>"B-1",
                      9=>"S-1S-0",10=>"S-1M-0"));
define("UNIT_SEL",array("THERMO"=>"&#x2103;","HUMID"=>"%","ppm"=>"ppm","ms"=>"m/s"));
define("BLK_B_MAX_ROWS",30);
define("BLK_C_MAX_ROWS",10);
define("BLK_D_MAX_ROWS",10);

function fetch_tbl_index($pdo,$tbl) {
    $query = sprintf("SELECT * FROM %s",$tbl);
    $stmt = $pdo->query($query);
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    return($data);
}

function mk_dhcpflag($smt,$sel) {
    $smt->assign('flagDHCP',array(255=>'DHCP',0=>'STATIC'));
    $smt->assign('SELflagDHCP',$sel);
}

require_once("lib_tbl_custom.php");
require_once("lib_blk_a.php");
require_once("lib_blk_b.php");
require_once("lib_blk_c.php");
require_once("lib_blk_d.php");
require_once("lib_pkg_c.php");
require_once("lib_blk_ope.php");
require_once("lib_ope_c.php");
require_once("ccmtype_list.php");
require_once("conv2ihex.php");
require_once("lib_file_common.php");
require_once("lib_log.php");
?>
