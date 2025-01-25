<?php
function select_blk_d($pdo,$s="",$id="") {
    if ($id=="") {
        $query = 'SELECT max(PKG_ID) as next_pkg_id FROM blk_d';
        try {
            $stmt = $pdo->query($query);
            $msg = "SELECT DONE";
        } catch(PDOException $e) {
            $msg = $e->getMessage();
            printf("<pre>%s</pre>",$msg);
            exit;
        }
        $next_pkg_id = $stmt->fetch(PDO::FETCH_ASSOC);
        if ($s!="") {
            $s->assign("next_pkg_id",$next_pkg_id['next_pkg_id']+1);
        }
        $query = 'SELECT * FROM blk_d ORDER BY PKG_ID,ID';
        $stmt = $pdo->query($query);
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
        if ($s!="") {
            $s->assign("lv_sel",constant("LV_SEL"));
            $s->assign("unit_sel",constant("UNIT_SEL"));
            $s->assign("max_rows",constant("BLK_C_MAX_ROWS"));
        }
    } else {
        $query = sprintf("SELECT * FROM blk_d WHERE PKG_ID=%d ORDER BY ID",$id);
        try {
            $stmt = $pdo->query($query);
            $msg = "SELECT DONE";
        } catch(PDOException $e) {
            $msg = $e->getMessage();
            printf("<pre>%s</pre>",$msg);
            exit;
        }
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
        if ($data==NULL) {
            print("BLK_D is NULL");
        }
    }
    return($data);
}

function delete_blk_d($pdo,$p) {
    $query = sprintf("DELETE FROM blk_d WHERE PKG_ID=%d AND ID=%d",$p['PKGID'],$p['ID']);
    try {
        $stmt = $pdo->query($query);
        $msg = "DELETE DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        printf("<pre>%s</pre>",$msg);
        exit;
    }
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    return($data);
}


function modify_blk_d($pdo,$p) {
    $query = 'UPDATE blk_d SET PKG_ID=%d,ID=%d,VALID=%s,ROOM=%d,REGION=%d,ORD=%d,'.
           'PRIORITY=%d,LV=%d,CAST=%d,UNIT=%s,CCMTYPE=%s,NAME=%s WHERE PKG_ID=%d AND ID=%d';
    $new_pkgid = $p['PKGID'];
    $pkgid = $p['PKG_ID'];
    for ($id=0; $id < 10; $id++) {
        $vld      = (isset($p['VALID'][$id])) ? "true" : "false";
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
        $query1   = sprintf($query,$new_pkgid,$id,$vld,$room,$region,$ord,$priority,$lv,$cast,
                            $unit,$ccmtype,$name,$pkgid,$id);
        try {
            //            $reqly = $pdo->query($query1);
            //            $msg .= "MODIFY DONE ".$query1."<br>";
            $msg = "MODIFY DONE ";
        } catch(PDOException $e) {
            $msg .= $e->getMessage()."<br>";
            return($msg);
        }
    }
    return($msg);
}

function modify_blk_d_with_pkgid_id($pdo,$p,$mode="") {
    if ($mode=="") {
        $query = 'UPDATE blk_d SET VALID=%s,ROOM=%d,REGION=%d,ORD=%d,'.
            'PRIORITY=%d,LV=%d,CAST=%d,UNIT=%s,CCMTYPE=%s,NAME=%s WHERE PKG_ID=%d AND ID=%d';
    } else {
        $query = 'INSERT INTO blk_d(PKG_ID,ID,VALID,ROOM,REGION,ORD,PRIORITY,LV,CAST,UNIT,CCMTYPE,NAME) VALUES';
        $query1= '';
    }
    $vld      = (isset($p['VALID'])) ? "true" : "false";
    $room     = ((@$p['ROOM']>0)&&(@$p['ROOM']<128)) ? $p['ROOM'] : 1;
    $region   = ((@$p['REGION']>0)&&(@$p['REGION']<128)) ? $p['REGION'] : 1;
    $ord      = ((@$p['ORD']>0)&&(@$p['ORD']<30000)) ? $p['ORD'] : 1;
    $lv       = @$p['LV'];
    switch($lv) {
        case 7:  // B-0
        case 8:  // B-1
            $priority = 30;
            break;
        default:
            $priority = ((@$p['PRIORITY']<0)) ? 0 : $p['PRIORITY'];
            $priority = ((@$p['PRIORITY']<29)) ? $p['PRIORITY'] : 29;
            break;
    }
    $cast     = ((@$p['CAST']<0)) ? 0 : $p['CAST'];
    $unit     = (@$p['UNIT']!='') ? "'".$p['UNIT']."'" : "null";
    $ccmtype  = (@$p['CCMTYPE']!='') ? "'".$p['CCMTYPE']."'" : "null";
    $name     = (@$p['NAME']!='') ? "'".$p['NAME']."'" : "null";
    if ($mode=="") {
        $query1 = sprintf($query,$vld,$room,$region,$ord,$priority,$lv,$cast,
                            $unit,$ccmtype,$name,$p['PKGID'],$p['ID']);
    } else {
        $query1 = sprintf("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",
                            $p['PKGID'],$p['ID'],$vld,$room,$region,$ord,$priority,$lv,$cast,
                            $unit,$ccmtype,$name);
        $query1 = $query.$query1;
    }
    try {
        $reqly = $pdo->query($query1);
        $msg = "DONE ".$query1."<br>";
    } catch(PDOException $e) {
        $msg .= $e->getMessage()."<br>";
    }
    return($msg);
}

function insert_blk_d($pdo,$p) {
    $query = 'INSERT INTO blk_d(PKG_ID,ID,VALID,ROOM,REGION,ORD,PRIORITY,LV,CAST,UNIT,CCMTYPE,NAME) VALUES';
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
        $query1  .= sprintf("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s),",
                            $p['PKG_ID'],$id,$vld,$room,$region,$ord,$priority,$lv,$cast,
                            $unit,$ccmtype,$name);
    }
    $query1 = rtrim($query1,",");
    $query = $query.$query1;
    //    printf("<pre>%s</pre>\n",$query);
    try {
        $reqly   = $pdo->query($query);
        $msg = "INSERT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

?>
    
