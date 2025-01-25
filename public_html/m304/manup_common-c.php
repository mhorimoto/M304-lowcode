<?php

define("RLY_STAT_SEL",array(0=>"不変",1=>"時間接続",2=>"遮断保持",3=>"接続保持"));

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

function mk_pkg_c_index($pdo) {
    $d = fetch_tbl_index($pdo,"pkg_c order by PKG_ID");
    $r = array();
    foreach($d as $a) {
        $r += array($a['PKG_ID']=>$a['NAME']);
    }
    return($r);
}

function mk_tbl_custom_index($pdo) {
    $d = fetch_tbl_index($pdo,"tbl_custom order by FARM_ID");
    $r = array();
    foreach($d as $a) {
        $r += array($a['FARM_ID']=>$a['NAME']);
    }
    return($r);
}

function mk_insert_blk_a($pdo,$pst) {
    $sql = sprintf("tbl_custom WHERE FARM_ID=%d",$pst['FARM_ID']);
    $d = fetch_tbl_index($pdo,$sql);
    $maca = str_replace(":","",$pst['MACADDR']);
//              M304ID,UECSID,MACADDR,DHCPFLAG,IPADDR,NETMASK,
//              DEFGW, DNS,  VENDER_NAME,NODE_NAME,PKG_ID,COMMENT,FARM_ID
    $sql = sprintf("%d,UNHEX('%s'),UNHEX('%s'),%d,inet_aton('%s'),inet_aton('%s'),".
                   "inet_aton('%s'),inet_aton('%s'),'%s','%s',%d,'%s',%d,%d",
                   $pst['M304ID'],$pst['UECSID'],$maca,$pst['DHCPFLAG'],
                   $pst['FIXED_IPADDRESS'],$pst['FIXED_NETMASK'],$pst['FIXED_DEFGW'],
                   $pst['FIXED_DNS'],$pst['VENDER_NAME'],$pst['NODE_NAME'],
                   $pst['PKG_ID'],$pst['COMMENT'],$d[0]['COM_ID'],$pst['FARM_ID']);
    return($sql);
}

function insert_tbl_custom($pdo,$p) {
    $com_id  = $p['COM_ID'];
    $farm_id = $p['FARM_ID'];
    $name    = $p['NAME'];
    $address = $p['ADDRESS'];
    $query   = 'INSERT INTO tbl_custom(COM_ID,NAME,ADDRESS) VALUES';
    $query   = sprintf("%s (%d,'%s','%s')",$query,$com_id,$name,$address);
    try {
        $reqly = $pdo->query($query);
        $msg = "INSERT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

function modify_tbl_custom( $pdo, $p ) {
    $com_id  = $p['COM_ID'];
    $farm_id = $p['FARM_ID'];
    $name    = $p['NAME'];
    $address = $p['ADDRESS'];
    $query   = 'update tbl_custom set ';
    $query   = sprintf("%s COM_ID=%d,FARM_ID=%d,NAME='%s',ADDRESS='%s' WHERE SEQ=%d",
               $query,$com_id,$farm_id,$name,$address,$seq);
    $msg     = "MODIFY DONE ";
    try {
        $reqly = $pdo->query($query);
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

function select_tbl_custom($pdo) {
    $query = 'SELECT * FROM tbl_custom';
    $stmt = $pdo->query($query);
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    return($data);
}

function insert_blk_a($pdo,$pst) {
    $quval  = mk_insert_blk_a($pdo,$pst);
    $query  = 'INSERT INTO blk_a(M304ID,UECSID,MACADDR,DHCPFLAG,FIXED_IPADDRESS,FIXED_NETMASK,'.
              'FIXED_DEFGW,FIXED_DNS,VENDER_NAME,NODE_NAME,PKG_ID,COMMENT,COM_ID,FARM_ID) VALUES ';
    $query  = sprintf("%s (%s)",$query,$quval);
    try {
        $reqly   = $pdo->query($query);
        $msg = "INSERT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

function modify_blk_a($pdo,$p) {
    $maca = str_replace(":","",$p['MACADDR']);
    $sql = sprintf("tbl_custom WHERE FARM_ID=%d",$p['FARM_ID']);
    $d = fetch_tbl_index($pdo,$sql);
    $update = "UPDATE blk_a SET ";
    $where = sprintf("WHERE SEQ=%d",$p['SEQ']);
    $query = sprintf("%s M304ID=%d,UECSID=UNHEX('%s'),MACADDR=UNHEX('%s'),DHCPFLAG=%d,".
                     "FIXED_IPADDRESS=inet_aton('%s'),FIXED_NETMASK=inet_aton('%s'),".
                     "FIXED_DEFGW=inet_aton('%s'),FIXED_DNS=inet_aton('%s'),".
                     "VENDER_NAME='%s',NODE_NAME='%s',PKG_ID=%d,COMMENT='%s',".
                     "COM_ID=%d,FARM_ID=%d %s",
                     $update,$p['M304ID'],$p['UECSID'],$maca,$p['DHCPFLAG'],
                     $p['FIXED_IPADDRESS'],$p['FIXED_NETMASK'],$p['FIXED_DEFGW'],
                     $p['FIXED_DNS'],$p['VENDER_NAME'],$p['NODE_NAME'],
                     $p['PKG_ID'],$p['COMMENT'],$d[0]['COM_ID'],$p['FARM_ID'],$where);
    try {
        $reqly = $pdo->query($query);
        $msg = "UPDATE DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

function select_blk_a($pdo,$s) {
    $query = 'SELECT SEQ,M304ID,hex(UECSID) as UECSID,hex(MACADDR) as MACADDR,'.
           'DHCPFLAG,inet_ntoa(FIXED_IPADDRESS) as FIXED_IPADDRESS,'.
           'inet_ntoa(FIXED_NETMASK) as FIXED_NETMASK,inet_ntoa(FIXED_DEFGW) as FIXED_DEFGW,'.
           'inet_ntoa(FIXED_DNS) as FIXED_DNS,VENDER_NAME,NODE_NAME,PKG_ID,'.
           'COMMENT,COM_ID,FARM_ID FROM blk_a order by M304ID';
    $stmt = $pdo->query($query);
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $cindex = fetch_tbl_index($pdo,"tbl_custom");
    $s->assign('cindex',$cindex);
    $s->assign('dhcptbl',array(255=>'DHCP',0=>'STATIC'));
    $s->assign('pindex',mk_pkg_c_index($pdo));
    $s->assign('findex',mk_tbl_custom_index($pdo));
    return($data);
}

function insert_pkg_c($pdo,$p) {
    if ($p['REG_DATE']=="") {
        $query  = 'INSERT INTO pkg_c(PKG_ID,NAME,COMMENT,REG_DATE,AUTHOR) VALUES ';
        $query  = sprintf("%s ('%s','%s','%s','%s')",$query,$p['PKG_ID'],$p['NAME'],$p['COMMENT'],$p['AUTHOR']);
    } else {
        $query  = 'INSERT INTO pkg_c(PKG_ID,NAME,COMMENT,REG_DATE,AUTHOR) VALUES ';
        $query  = sprintf("%s ('%s','%s','%s','%s','%s')",$query,$p['PKG_ID'],$p['NAME'],
                          $p['COMMENT'],$p['REG_DATE'],$p['AUTHOR']);
    }
    try {
        $reqly   = $pdo->query($query);
        $msg = "INSERT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

function modify_pkg_c( $pdo, $p ) {
    $query   = 'update pkg_c set ';
    $query   = sprintf("%s PKG_ID=%d,NAME='%s',COMMENT='%s',REG_DATE='%s',AUTHOR='%s' WHERE SEQ=%d",
                       $query,$p['PKG_ID'],$p['NAME'],$p['COMMENT'],$p['REG_DATE'],
                       $p['AUTHOR'],$p['SEQ']);
    $msg     = "MODIFY DONE ";
    try {
        $reqly = $pdo->query($query);
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

function select_pkg_c($pdo) {
    $query = 'SELECT * FROM pkg_c order by PKG_ID';
    $stmt = $pdo->query($query);
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    return($data);
}

function insert_blk_ope($pdo,$p) {
    $ope_id = $p['OPE_ID'];
    $name   = $p['NAME'];
    $m304id = $p['M304ID'];
    $comment= $p['COMMENT'];
    
    $msg = "";
    $query = sprintf("INSERT INTO ope_c (OPE_ID,NAME,COMMENT) VALUES (%d,'%s','%s')",
                     $ope_id,$name,$p['COMMENT']);
    try {
        $reqly   = $pdo->query($query);
        $msg = "INSERT ope_c DONE ";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    $query = "INSERT INTO blk_ope (OPE_ID,M304ID,RLY,RLYSTATUS) VALUES ";
    $r = $_POST['RLY'];
    for ($n=1;$n<9;$n++) {
        $rr = $r[$n];
        $query2 = sprintf("%s (%d,%d,%d,%d)",$query,$ope_id,$m304id,$n,$rr);
        try {
            $reqly = $pdo->query($query2);
            $msg .= sprintf("INSERT blk_ope(%d) DONE ",$n);
        } catch(PDOException $e) {
            $msg .= $e->getMessage();
        }
    }
    return($msg);
}
function modify_blk_ope($pdo,$p) {
    print_r($p);
    exit;
    $ope_id = $p['OPE_ID'];
    $name   = $p['NAME'];
    
}

function select_blk_ope($pdo,$s) {
    $query = "SELECT blk_ope.*,ope_c.NAME,ope_c.COMMENT,ope_c.SEQ AS OPE_C_SEQ FROM blk_ope LEFT OUTER JOIN ope_c ON ope_c.ope_id=blk_ope.ope_id ORDER BY blk_ope.SEQ";
    $stmt = $pdo->query($query);
    $rv = $stmt->fetchAll(PDO::FETCH_ASSOC);
    //    echo "<pre>\n";
    $poid = $rv[0]['OPE_ID'];
    //    printf("OPE_ID=%d\n",$rv[0]['OPE_ID']);
    $tpa = array("NAME"=>$rv[0]['NAME']);
    $tpa+= array("OPE_C_SEQ"=>$rv[0]['OPE_C_SEQ']);
    foreach($rv as $key=>$va) {
        if ($poid!=$va['OPE_ID']) {
            $poid = $va['OPE_ID'];
            $tpa = array("NAME"=>$va['NAME']);
            $tpa+= array("OPE_C_SEQ"=>$va['OPE_C_SEQ']);
        }
        $data[$poid] = array("OPE_ID"=>$poid,"M304ID"=>$va['M304ID'],"COMMENT"=>$va['COMMENT']);
        $tpa += array("RLY".$va['RLY']=>$va['RLYSTATUS']);
        $data[$poid] = array_merge($data[$poid],$tpa);
    }
    //    echo "=======================\n";
    //    var_dump($data);
    //    echo "</pre>\n";
    //exit;
    $s->assign("rlystatus",constant("RLY_STAT_SEL"));
    return($data);
}

?>
