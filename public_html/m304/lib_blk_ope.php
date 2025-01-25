<?php
function insert_blk_ope($pdo,$p) {
    $ope_id = $p['OPE_ID'];
    $pkg_id = $p['PKG_ID'];
    $name   = $p['NAME'];
    $m304id = $p['M304ID'];
    $comment= $p['COMMENT'];
    
    $msg = "";
    $query = sprintf("INSERT INTO ope_c (PKG_ID,OPE_ID,M304ID,NAME,COMMENT) VALUES (%d,%d,%d,'%s','%s')",
                    $pkg_id,$ope_id,$m304id,$name,$p['COMMENT']);
    try {
        $reqly   = $pdo->query($query);
        $msg = "INSERT ope_c DONE ";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    $query = "INSERT INTO blk_ope (OPE_ID,PKG_ID,M304ID,RLY,RLYSTATUS) VALUES ";
    $r = $_POST['RLY'];
    for ($n=1;$n<9;$n++) {
        $rr = $r[$n];
        $query2 = sprintf("%s (%d,%d,%d,%d,%d)",$query,$ope_id,$pkg_id,$m304id,$n,$rr);
        try {
            $reqly = $pdo->query($query2);
            $msg .= sprintf("%d:%s / ",$n,$query2);
        } catch(PDOException $e) {
            $msg .= $e->getMessage();
        }
    }
    return($msg);
}
function modify_blk_ope($pdo,$p) {
    $pkg_id = $p['PKG_ID'];
    $ope_id = $p['OPE_ID'];
    $name   = $p['NAME'];
    $m304id = $p['M304ID'];
    $comment= $p['COMMENT'];
    $rly    = $p['RLY'];
    $id     = $p['ID'];
    $query  = "UPDATE ope_c SET ";
    $query1 = sprintf("%s OPE_ID=%d,NAME='%s',COMMENT='%s' WHERE OPE_ID=%d;",
                      $query,$ope_id,$name,$comment,$id);
    $query  = "UPDATE blk_ope SET ";
    foreach($rly as $rlyk=>$rlyv) {
        $query1 .= sprintf("%s OPE_ID=%d,M304ID=%d,RLYSTATUS=%d WHERE OPE_ID=%d AND RLY=%d;",
                          $query,$ope_id,$m304id,$rlyv,$id,$rlyk);
    }
    try {
        $reqly = $pdo->query($query1);
        $msg .= sprintf("MODIFY blk_ope(%d) and ope_c DONE ",$id);
    } catch(PDOException $e) {
        $msg .= $e->getMessage();
    }
    return($msg);
}

function select_blk_ope($pdo,$s="",$pid="") {
    $query1 = "SELECT SEQ as OPEC_SEQ,PKG_ID as OPEC_PKG_ID,OPE_ID,NAME,COMMENT from ope_c";
    if ($pid!="") {
        $query1 = sprintf("%s WHERE PKG_ID=%d ORDER BY OPE_ID",$query1,$pid);
    }
    $stmt1 = $pdo->query($query1);
    while($rv1 = $stmt1->fetch(PDO::FETCH_ASSOC)) {
        $data[$rv1['OPE_ID']] = $rv1;
        $query2 = "SELECT SEQ,OPE_ID,PKG_ID,M304ID,RLY,RLYSTATUS FROM blk_ope WHERE OPE_ID=".$rv1['OPE_ID'];
        $stmt2 = $pdo->query($query2);
        while($rv2=$stmt2->fetch(PDO::FETCH_ASSOC)) {
            $data[$rv1['OPE_ID']]['RLYSTATUS'][$rv2['RLY']] = $rv2['RLYSTATUS'];
            $data[$rv1['OPE_ID']]['M304ID'] = $rv2['M304ID'];
            $data[$rv1['OPE_ID']]['BLKOPE_SEQ'][$rv2['RLY']] = $rv2['SEQ'];
        }
    }
    if ($s!="") {
        $s->assign("rlystatus",constant("RLY_STAT_SEL"));
    }
    return($data);
}

function select_blk_ope_with_ope_id($pdo,$opeid="",$mode="") {
    if ($opeid=="") return(null);
    $rc = array(1=>0,0,0,0,0,0,0,0);
    $query = sprintf("SELECT RLY,RLYSTATUS FROM blk_ope WHERE OPE_ID=%d ORDER BY RLY",$opeid);
    try {
        $reply = $pdo->query($query);
    } catch(PDOException $e) {
        $msg .= $e->getMessage();
    }
    $data = $reply->fetchAll(PDO::FETCH_ASSOC);
    if ($mode="oneline") {
        foreach($data as $row) {
            $rc[$row['RLY']] = $row['RLYSTATUS'];
        }
        $rcst = join($rc);
    } else {
        $rcst = $rc;
    }
    return($rcst);
}

function select_ope_c_with_ope_id($pdo,$id='') {
    if ($id=='') return(false);
    $query = sprintf("SELECT * FROM ope_c WHERE OPE_ID=%d",$id);
    try {
        $reqly = $pdo->query($query);
    } catch(PDOException $e) {
        $msg .= $e->getMessage();
    }
    $data = $reply->fetch(PDO::FETCH_ASSOC);
    return($data);
}
?>
