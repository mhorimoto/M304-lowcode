<?php
function select_blk_c($pdo,$s="",$pkg_id) {
    if ($pkg_id=="") {
        $query = 'SELECT max(PKG_ID) as next_pkg_id FROM blk_c';
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
        $query = 'SELECT * FROM blk_c ORDER BY PKG_ID,ID';
        $stmt = $pdo->query($query);
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
        if ($s!="") {
            $s->assign("lifecount_sel",constant("LIFECOUNT_SEL"));
            $s->assign("max_rows",constant("BLK_C_MAX_ROWS"));
        }
    } else {
        $query = sprintf("SELECT * FROM blk_c WHERE PKG_ID=%d ORDER BY ID",$pkg_id);
        try {
            $stmt = $pdo->query($query);
            $msg = "SELECT DONE";
        } catch(PDOException $e) {
            $msg = $e->getMessage();
            printf("<pre>%s</pre>",$msg);
            exit;
        }
        $data['EMPTY']=0;
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
        if ($data==NULL) {
            print("BLK_C is NULL");
            $data[0]['EMPTY']=1;
            $data[0]['PKG_ID']=$pkg_id;
        }
    }
    return($data);
}

function modify_blk_c($pdo,$p) {
    //        printf("<pre>%s</pre>\n",print_r($p,1));
    //        exit;
    $query = "UPDATE blk_c SET PKG_ID=%d,ID=%d,VALID=%s,ROOM=%d,REGION=%d,ORD=%d,".
           "LIFECOUNT=%d,CCMTYPE=%s,NAME=%s WHERE PKG_ID=%d AND ID=%d";
    $pkgid = $p['PKGID'];
    for ($id=0;$id<10;$id++) {
        $vid    = (@$p['VALID'][$id]==1) ? 'true' : 'false';
        $room   = ((@$p['ROOM'][$id]>0)&&(@$p['ROOM'][$id]<128)) ? $p['ROOM'][$id] : 1;
        $region = ((@$p['REGION'][$id]>0)&&(@$p['REGION'][$id]<128)) ? $p['REGION'][$id] : 1;
        $ord    = ((@$p['ORD'][$id]>0)&&(@$p['ORD'][$id]<30000)) ? $p['ORD'][$id] : 1;
        $lifecnt= @$p['LIFECOUNT'][$id];
        $ccmtype= (@$p['CCMTYPE'][$id]!='') ? "'".$p['CCMTYPE'][$id]."'" : "null";
        $name   = (@$p['NAME'][$id]!='') ? "'".$p['NAME'][$id]."'" : "null";
        $query1 = sprintf($query,$pkgid,$id,$vid,$room,$region,$ord,$lifecnt,$ccmtype,$name,$pkgid,$id);
        try {
            $reqly = $pdo->query($query1);
            $msg = "MODIFY DONE";
        } catch(PDOException $e) {
            $msg = $e->getMessage();
            return($msg);
        }
    }
    return($msg);
}

function insert_blk_c($pdo,$p) {
    //   print_r($p);
    $query = 'INSERT INTO blk_c(PKG_ID,M304ID,ID,VALID,ROOM,REGION,ORD,LIFECOUNT,CCMTYPE,NAME) VALUES';
    $query1= '';
    for ($id=0; $id < 10; $id++) {
        $vld    = (@$p['VALID'][$id]==1) ? 'true' : 'false';
        $room   = ((@$p['ROOM'][$id]>0)&&(@$p['ROOM'][$id]<128)) ? $p['ROOM'][$id] : 1;
        $region = ((@$p['REGION'][$id]>0)&&(@$p['REGION'][$id]<128)) ? $p['REGION'][$id] : 1;
        $ord    = ((@$p['ORD'][$id]>0)&&(@$p['ORD'][$id]<30000)) ? $p['ORD'][$id] : 1;
        $ccmtype= (@$p['CCMTYPE'][$id]!='') ? "'".$p['CCMTYPE'][$id]."'" : "null";
        $name   = (@$p['NAME'][$id]!='') ? "'".$p['NAME'][$id]."'" : "null";
        $query1.= sprintf("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s),",
                          $p['PKG_ID'],$p['PKG_ID'],$id,$vld,$room,$region,$ord,$p['LIFECOUNT'][$id],
                          $ccmtype,$name);
    }
    $query1 = rtrim($query1,",");
    $query = $query.$query1;
    try {
        $reqly   = $pdo->query($query);
        $msg = "INSERT DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}

function mk_select_blk_c($pdo,$pkg_id="",$id="",&$sele,&$selj) {
    if (($pkg_id=="")&&($id=="")) {
        $query = "SELECT * FROM blk_c ORDER BY PKG_ID,ID";
    } else if ($id=="") {
        $query = sprintf("SELECT * FROM blk_c WHERE PKG_ID=%d ORDER BY ID",$pkg_id);
    } else if ($pkg_id=="") {
        $query = sprintf("SELECT * FROM blk_c WHERE ID=%d ORDER BY PKG_ID",$id);
    } else {
        $query = sprintf("SELECT * FROM blk_c WHERE PKG_ID=%d AND ID=%d",$pkg_id,$id);
    }
    try {
        $reply = $pdo->query($query);
        $msg = "NORMAL";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    $data = $reply->fetchAll(PDO::FETCH_ASSOC);
    foreach($data as &$row) {
        $row['JTEXT']  = sprintf("%d/%d/%d:%s",$row['ROOM'],$row['REGION'],$row['ORD'],$row['NAME']);
        $row['ETEXT']  = sprintf("%d/%d/%d:%s",$row['ROOM'],$row['REGION'],$row['ORD'],$row['CCMTYPE']);
    }
    $cnt = count($data);
    $sele = array(strval(255)=>"");
    $selj = array(strval(255)=>"");
    for ($i=0;$i<$cnt;$i++) {
        $sid  = strval($data[$i]['ID']);
        $selj += array("$sid"=>$data[$i]['JTEXT']);
        $sele += array("$sid"=>$data[$i]['ETEXT']);
    }
    return($data);
}

function modify_blk_c_with_pkgid_id($pdo,$p,$mode="") {
    $id     = @$p['ID'];
    $vld    = (@$p['VALID']==1) ? 'true' : 'false';
    $room   = ((@$p['ROOM']>0)&&(@$p['ROOM']<128)) ? $p['ROOM'] : 1;
    $region = ((@$p['REGION']>0)&&(@$p['REGION']<128)) ? $p['REGION'] : 1;
    $ord    = ((@$p['ORD']>0)&&(@$p['ORD']<30000)) ? $p['ORD'] : 1;
    $ccmtype= (@$p['CCMTYPE']!='') ? "'".$p['CCMTYPE']."'" : "null";
    $name   = (@$p['NAME']!='') ? "'".$p['NAME']."'" : "null";
    $query1 = '';
    $pkgid  = $p['PKG_ID'];
    if ($mode=="INSERT") {
        $query = 'INSERT INTO blk_c(PKG_ID,M304ID,ID,VALID,ROOM,REGION,ORD,LIFECOUNT,CCMTYPE,NAME) VALUES';
        $query1.= sprintf("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s),",
                        $pkgid,$pkgid,$id,$vld,$room,$region,$ord,$p['LIFECOUNT'],$ccmtype,$name);
        $query1 = rtrim($query1,",");
        $query = $query.$query1;
    } else {
        $m304id = $p['M304ID'];
        $query = sprintf("UPDATE blk_c SET PKG_ID=%d,M304ID=%d,ID=%d,VALID=%s,ROOM=%d,REGION=%d,ORD=%d,".
                "LIFECOUNT=%d,CCMTYPE=%s,NAME=%s WHERE PKG_ID=%d AND ID=%d",
                $pkgid,$m304id,$id,$vld,$room,$region,$ord,$p['LIFECOUNT'],$ccmtype,$name,$pkgid,$id);
    }
    echo "<pre>\n".$query."</pre>\n";
    try {
        $reqly = $pdo->query($query);
        $msg   = $mode." DONE";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    return($msg);
}
?>
