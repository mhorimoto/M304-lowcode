<?php
function select_blk_b($pdo,$s) {
    $ccmtl = mk_datalist("ccmlist");
    $s->assign("ccmlist",$ccmtl);
    $rlyope = mk_datalist("relay_ope");
    $s->assign("rlyope",$rlyope);
    $cmbope = mk_datalist("cmb_ope");
    $s->assign("cmbope",$cmbope);
    $query = 'SELECT max(PKG_ID) as next_pkg_id FROM blk_b';
    try {
        $stmt = $pdo->query($query);
        $msg = "SELECT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        printf("<pre>%s</pre>",$msg);
        exit;
    }
    $next_pkg_id = $stmt->fetch(PDO::FETCH_ASSOC);
    $s->assign("next_pkg_id",$next_pkg_id['next_pkg_id']+1);
    $query = 'SELECT * FROM blk_b ORDER BY PKG_ID,ID';
    try {
        $stmt = $pdo->query($query);
        $msg = "SELECT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        printf("<pre>%s</pre>",$msg);
        exit;
    }
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $s->assign("max_rows",constant("BLK_B_MAX_ROWS"));
    $query = sprintf("SELECT * FROM blk_c WHERE PKG_ID=%d AND VALID IS true ORDER BY ID",$next_pkg_id['next_pkg_id']);
    return($data);
}

function select_blk_b_with_pkgid($pdo,$pkg_id="") {
    if ($pkg_id=="") {
        return(false);
    }
    $query = sprintf("SELECT * FROM blk_b WHERE PKG_ID=%d ORDER BY ID",$pkg_id);
    try {
        $stmt = $pdo->query($query);
        $msg = "SELECT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        printf("<pre>%s</pre>",$msg);
        exit;
    }
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    foreach($data as &$row) {
        $ope_id = $row['OPE_ID'];
        $query = sprintf("SELECT * FROM ope_c WHERE OPE_ID=%d",$ope_id);
        try {
            $stmt = $pdo->query($query);
            $msg = "SELECT DONE";
        } catch(PDOException $e) {
            $msg = $e->getMessage();
            printf("<pre>%s</pre>",$msg);
            exit;
        }
        $ret = $stmt->fetch(PDO::FETCH_ASSOC);
        $row['OPE_NAME'] = $ret['NAME'];
        $row['OPE_COMMENT'] = $ret['COMMENT'];
    }
    return($data);
}

function modify_blk_b_with_pkgid_id($pdo,$p,$mode="") {
    $pkgid = $p['PKG_ID'];
    $m304id= $pkgid;
    $log = new M304log($pdo,$m304id);
    $log->store(8,'blk_b','modify_blk_b_with_pkgid_id',$mode);
    $vld   = isset($p['VALID']) ? 'true' : 'false';
    $fval0 = (@$p['CMPVAL0']=="") ? 'null' : $p['CMPVAL0'];
    $fval1 = (@$p['CMPVAL1']=="") ? 'null' : $p['CMPVAL1'];
    $fval2 = (@$p['CMPVAL2']=="") ? 'null' : $p['CMPVAL2'];
    $fval3 = (@$p['CMPVAL3']=="") ? 'null' : $p['CMPVAL3'];
    $fval4 = (@$p['CMPVAL4']=="") ? 'null' : $p['CMPVAL4'];
    $mnflag = 255;
    if ($mode=="") {
        $query = 'UPDATE blk_b SET PKG_ID=%d,M304ID=%d,ID=%d,VALID=%s,STHR=%d,STMN=%d,EDHR=%d,EDMN=%d,'.
            'MNFLAG=%d,INMN=%d,DUMN=%d,OPE_ID=%d,CMPCCMID0=%d,CMPOPE0=%d,CMPVAL0=%s,CMBCMP1=%d,'.
            'CMPCCMID1=%d,CMPOPE1=%d,CMPVAL1=%s,CMBCMP2=%d,CMPCCMID2=%d,CMPOPE2=%d,CMPVAL2=%s,'.
            'CMBCMP3=%d,CMPCCMID3=%d,CMPOPE3=%d,CMPVAL3=%s,CMBCMP4=%d,CMPCCMID4=%d,CMPOPE4=%d,CMPVAL4=%s '.
            'WHERE SEQ=%d';
        $query1   = sprintf($query,$pkgid,$m304id,$p['ID'],$vld,$p['STHR'],$p['STMN'],$p['EDHR'],$p['EDMN'],
            $mnflag,$p['INMN'],$p['DUMN'],$p['OPE_ID'],$p['CMPCCMID0'],$p['CMPOPE0'],$fval0,$p['CMBCMP1'],
            $p['CMPCCMID1'],$p['CMPOPE1'],$fval1,$p['CMBCMP2'],$p['CMPCCMID2'],$p['CMPOPE2'],$fval2,
            $p['CMBCMP3'],$p['CMPCCMID3'],$p['CMPOPE3'],$fval3,$p['CMBCMP4'],$p['CMPCCMID4'],$p['CMPOPE4'],$fval4,
            $p['SEQ']);
    } else {
        // For Insert
        $query = 'INSERT INTO blk_b (PKG_ID,M304ID,ID,VALID,STHR,STMN,EDHR,EDMN,MNFLAG,INMN,DUMN,OPE_ID,'.
            'CMPCCMID0,CMPOPE0,CMPVAL0,CMBCMP1,CMPCCMID1,CMPOPE1,CMPVAL1,CMBCMP2,CMPCCMID2,'.
            'CMPOPE2,CMPVAL2,CMBCMP3,CMPCCMID3,CMPOPE3,CMPVAL3,CMBCMP4,CMPCCMID4,CMPOPE4,CMPVAL4) '.
            'VALUES (%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,'.
            '%d,%d,%s,%d,%d,%d,%s,%d,%d,'.
            '%d,%s,%d,%d,%d,%s,%d,%d,%d,%s)';
        $query1   = sprintf($query,$pkgid,$m304id,$p['ID'],$vld,$p['STHR'],$p['STMN'],$p['EDHR'],$p['EDMN'],
            $mnflag,$p['INMN'],$p['DUMN'],$p['OPE_ID'],$p['CMPCCMID0'],$p['CMPOPE0'],$fval0,$p['CMBCMP1'],
            $p['CMPCCMID1'],$p['CMPOPE1'],$fval1,$p['CMBCMP2'],$p['CMPCCMID2'],$p['CMPOPE2'],$fval2,
            $p['CMBCMP3'],$p['CMPCCMID3'],$p['CMPOPE3'],$fval3,$p['CMBCMP4'],$p['CMPCCMID4'],$p['CMPOPE4'],$fval4);
    }
                            
    try {
        $reqly = $pdo->query($query1);
        $msg = "MODIFY DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        return($msg);
    }
    return($msg);
}

function modify_blk_b($pdo,$p) {
    $query = 'UPDATE blk_b SET PKG_ID=%d,ID=%d,VALID=%s,ROOM=%d,REGION=%d,ORD=%d,'.
           'PRIORITY=%d,LV=%d,CAST=%d,UNIT=%s,CCMTYPE=%s,NAME=%s WHERE PKG_ID=%d AND ID=%d';
    $pkgid = $p['PKG_ID'];
    for ($id=0; $id < 10; $id++) {
        $vld      = (@$p['VALID'][$id]==1) ? 'true' : 'false';
        $room     = ((@$p['ROOM'][$id]>0)&&(@$p['ROOM'][$id]<128)) ? $p['ROOM'][$id] : 1;
        $region   = ((@$p['REGION'][$id]>0)&&(@$p['REGION'][$id]<128)) ? $p['REGION'][$id] : 1;
        $ord      = ((@$p['ORD'][$id]>0)&&(@$p['ORD'][$id]<30000)) ? $p['ORD'][$id] : 1;
        $lv       = @$p['LV'][$id];
        switch($lv) {
        case 7:  // B-0
        case 8:  // B-1
            $priority = 30;
            break;
        default:
            $priority = ((@$p['PRIORITY'][$id]<0)) ? 0 : $p['PRIORITY'][$id];
            $priority = ((@$p['PRIORITY'][$id]<29)) ? $p['PRIORITY'][$id] : 29;
            break;
        }
        $cast     = ((@$p['CAST'][$id]<0)) ? 0 : $p['CAST'][$id];
        $unit     = (@$p['UNIT'][$id]!='') ? "'".$p['UNIT'][$id]."'" : "null";
        $ccmtype  = (@$p['CCMTYPE'][$id]!='') ? "'".$p['CCMTYPE'][$id]."'" : "null";
        $name     = (@$p['NAME'][$id]!='') ? "'".$p['NAME'][$id]."'" : "null";
        $query1   = sprintf($query,$pkgid,$id,$vld,$room,$region,$ord,$priority,$lv,$cast,
                            $unit,$ccmtype,$name,$pkgid,$id);
        try {
            //            $reqly = $pdo->query($query1);
            $msg = "MODIFY DONE";
        } catch(PDOException $e) {
            $msg = $e->getMessage();
            return($msg);
        }
    }
    return($msg);
}

function insert_blk_b($pdo,$p) {
    $query = 'INSERT INTO blk_b(PKG_ID,M304ID,ID,VALID,ROOM,REGION,ORD,PRIORITY,LV,CAST,UNIT,CCMTYPE,NAME) VALUES';
    $query1= '';
    for ($id=0; $id < 10; $id++) {
        $vld      = (@$p['VALID'][$id]==1) ? 'true' : 'false';
        $room     = ((@$p['ROOM'][$id]>0)&&(@$p['ROOM'][$id]<128)) ? $p['ROOM'][$id] : 1;
        $region   = ((@$p['REGION'][$id]>0)&&(@$p['REGION'][$id]<128)) ? $p['REGION'][$id] : 1;
        $ord      = ((@$p['ORD'][$id]>0)&&(@$p['ORD'][$id]<30000)) ? $p['ORD'][$id] : 1;
        $lv       = @$p['LV'][$id];
        //  printf("<pre>LV=%s</pre>\n",$lv);
        switch($lv) {
        case 7:  // B-0
        case 8:  // B-1
            $priority = 30;
            break;
        default:
            $priority = ((@$p['PRIORITY'][$id]<0)) ? 0 : $p['PRIORITY'][$id];
            $priority = ((@$p['PRIORITY'][$id]<29)) ? $p['PRIORITY'][$id] : 29;
            break;
        }
        $cast     = ((@$p['CAST'][$id]<0)) ? 0 : $p['CAST'][$id];
        $unit     = (@$p['UNIT'][$id]!='') ? "'".$p['UNIT'][$id]."'" : "null";
        $ccmtype  = (@$p['CCMTYPE'][$id]!='') ? "'".$p['CCMTYPE'][$id]."'" : "null";
        $name     = (@$p['NAME'][$id]!='') ? "'".$p['NAME'][$id]."'" : "null";
        $query1  .= sprintf("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s),",
                            $p['PKG_ID'],$p['PKG_ID'],$id,$vld,$room,$region,$ord,$priority,$lv,$cast,
                            $unit,$ccmtype,$name);
    }
    $query1 = rtrim($query1,",");
    $query = $query.$query1;
        printf("<pre>%s</pre>\n",$query);
    try {
        $reqly   = $pdo->query($query);
        $msg = "INSERT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        echo "<pre>$msg</pre>\n";
    }
    exit;
    return($msg);
}

function delete_blk_b($pdo,$p="") {
    if ($p=="") {
        return;
    }
    $query = sprintf("DELETE FROM blk_b WHERE SEQ=%d",$p['SEQ']);
    try {
        $reqly = $pdo->query($query);
        $msg = "DELETE DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        return($msg);
    }
    return($msg);
}
?>
    
