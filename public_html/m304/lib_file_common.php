<?php
function save_file($pdo,$id="",$post) {
    if ($id=="") {
        echo "ID not found";
        return;
    }
    delete_save_file($pdo,$id,$post['FILENAME']);
    copy_save_file($pdo,$id,$post);
    return(isExist_save_file($pdo,$id,$post['FILENAME']));
}

function load_file($pdo,$id="",$fln) {
    if ($id=="") {
        echo "ID not found";
        return;
    }
    $rv = isExist_save_file($pdo,$id,$fln);
    if ($rv===false) return;
    // blk_a 一旦消す
    $sql = "DELETE FROM blk_a WHERE M304ID=:m304id";
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $rv = $sth->execute();
    // From save_blk_a
    $blk_a = "SEQ,M304ID,UECSID,MACADDR,DHCPFLAG,FIXED_IPADDRESS,FIXED_NETMASK,FIXED_DEFGW,FIXED_DNS,VENDER_NAME,NODE_NAME,PKG_ID,COMMENT,COM_ID,FARM_ID";
    $sql = sprintf("INSERT INTO blk_a (%s) SELECT %s FROM save_blk_a WHERE M304ID=:m304id AND FILENAME=:filename",$blk_a,$blk_a);
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $sth->bindParam('filename',$fln,PDO::PARAM_STR);
    $rv = $sth->execute();
    // blk_b 一旦消す
    $sql = "DELETE FROM blk_b WHERE M304ID=:m304id";
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $rv = $sth->execute();
    // From save_blk_b
    $blk_b = "SEQ,M304ID,PKG_ID,ID,VALID,STHR,STMN,EDHR,EDMN,MNFLAG,INMN,DUMN,OPE_ID,CMPCCMID0,CMPOPE0,CMPVAL0,CMBCMP1,CMPCCMID1,CMPOPE1,CMPVAL1,CMBCMP2,CMPCCMID2,CMPOPE2,CMPVAL2,CMBCMP3,CMPCCMID3,CMPOPE3,CMPVAL3,CMBCMP4,CMPCCMID4,CMPOPE4,CMPVAL4";
    $sql = sprintf("INSERT INTO blk_b (%s) SELECT %s FROM save_blk_b WHERE M304ID=:m304id AND FILENAME=:filename",$blk_b,$blk_b);
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $sth->bindParam('filename',$fln,PDO::PARAM_STR);
    $rv = $sth->execute();
    // blk_c 一旦消す
    $sql = "DELETE FROM blk_c WHERE M304ID=:m304id";
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $rv = $sth->execute();
    // From save_blk_c
    $blk_c = "SEQ,M304ID,PKG_ID,ID,VALID,ROOM,REGION,ORD,LIFECOUNT,CCMTYPE,NAME";
    $sql = sprintf("INSERT INTO blk_c (%s) SELECT %s FROM save_blk_c WHERE M304ID=:m304id AND FILENAME=:filename",$blk_c,$blk_c);
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $sth->bindParam('filename',$fln,PDO::PARAM_STR);
    $rv = $sth->execute();
    // blk_d 一旦消す
    $sql = "DELETE FROM blk_d WHERE M304ID=:m304id";
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $rv = $sth->execute();
    // From save_blk_d
    $blk_d = "SEQ,M304ID,PKG_ID,ID,VALID,ROOM,REGION,ORD,PRIORITY,LV,CAST,UNIT,CCMTYPE,NAME";
    $sql = sprintf("INSERT INTO blk_d (%s) SELECT %s FROM save_blk_d WHERE M304ID=:m304id AND FILENAME=:filename",$blk_d,$blk_d);
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $sth->bindParam('filename',$fln,PDO::PARAM_STR);
    $rv = $sth->execute();
    // blk_ope 一旦消す
    $sql = "DELETE FROM blk_ope WHERE M304ID=:m304id";
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $rv = $sth->execute();
    // From save_blk_ope
    $blk_ope = "SEQ,OPE_ID,PKG_ID,M304ID,RLY,RLYSTATUS";
    $sql = sprintf("INSERT INTO blk_ope (%s) SELECT %s FROM save_blk_ope WHERE M304ID=:m304id AND FILENAME=:filename",$blk_ope,$blk_ope);
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $sth->bindParam('filename',$fln,PDO::PARAM_STR);
    $rv = $sth->execute();
    // ope_c 一旦消す
    $sql = "DELETE FROM ope_c WHERE M304ID=:m304id";
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $rv = $sth->execute();
    // From save_ope_c
    $ope_c = "SEQ,M304ID,PKG_ID,OPE_ID,NAME,COMMENT";
    $sql = sprintf("INSERT INTO ope_c (%s) SELECT %s FROM save_ope_c WHERE M304ID=:m304id AND FILENAME=:filename",$ope_c,$ope_c);
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $sth->bindParam('filename',$fln,PDO::PARAM_STR);
    $rv = $sth->execute();
    // pkg_c 一旦消す
    $sql = "DELETE FROM pkg_c WHERE M304ID=:m304id";
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $rv = $sth->execute();
    // From save_pkg_c
    $pkg_c = "SEQ,M304ID,PKG_ID,NAME,COMMENT,REG_DATE,AUTHOR,base,free";
    $sql = sprintf("INSERT INTO pkg_c (%s) SELECT %s FROM save_pkg_c WHERE M304ID=:m304id AND FILENAME=:filename",$pkg_c,$pkg_c);
    $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
    $sth->bindParam('m304id',$id,PDO::PARAM_INT);
    $sth->bindParam('filename',$fln,PDO::PARAM_STR);
    $rv = $sth->execute();
}

function copy_save_file($pdo,$id,$post) {
    $tbl = array( "blk_a", "blk_b", "blk_c", "blk_d", "blk_ope", "ope_c", "pkg_c");
    // save_dir
    $sql = sprintf("INSERT INTO save_dir (M304ID,FILENAME,OWNER,COMMENT) VALUES (%d,'%s','%s','%s')",
                   $id,$post['FILENAME'],$post['OWNER'],$post['COMMENT']);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
    // save_blk_a
    $from_blk_a = sprintf("SEQ,M304ID,'%s',UECSID,MACADDR,DHCPFLAG,FIXED_IPADDRESS,FIXED_NETMASK,FIXED_DEFGW,FIXED_DNS,VENDER_NAME,NODE_NAME,PKG_ID,COMMENT,COM_ID,FARM_ID",$post['FILENAME']);
    $to_blk_a = "SEQ,M304ID,FILENAME,UECSID,MACADDR,DHCPFLAG,FIXED_IPADDRESS,FIXED_NETMASK,FIXED_DEFGW,FIXED_DNS,VENDER_NAME,NODE_NAME,PKG_ID,COMMENT,COM_ID,FARM_ID";
    $sql = sprintf("INSERT INTO save_blk_a (%s) SELECT %s FROM blk_a WHERE M304ID=%d",$to_blk_a,$from_blk_a,$id);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
    // save_blk_b
    $from_blk_b = sprintf("SEQ,M304ID,'%s',PKG_ID,ID,VALID,STHR,STMN,EDHR,EDMN,MNFLAG,INMN,DUMN,OPE_ID,CMPCCMID0,CMPOPE0,CMPVAL0,CMBCMP1,CMPCCMID1,CMPOPE1,CMPVAL1,CMBCMP2,CMPCCMID2,CMPOPE2,CMPVAL2,CMBCMP3,CMPCCMID3,CMPOPE3,CMPVAL3,CMBCMP4,CMPCCMID4,CMPOPE4,CMPVAL4",$post['FILENAME']);
    $to_blk_b = "SEQ,M304ID,FILENAME,PKG_ID,ID,VALID,STHR,STMN,EDHR,EDMN,MNFLAG,INMN,DUMN,OPE_ID,CMPCCMID0,CMPOPE0,CMPVAL0,CMBCMP1,CMPCCMID1,CMPOPE1,CMPVAL1,CMBCMP2,CMPCCMID2,CMPOPE2,CMPVAL2,CMBCMP3,CMPCCMID3,CMPOPE3,CMPVAL3,CMBCMP4,CMPCCMID4,CMPOPE4,CMPVAL4";
    $sql = sprintf("INSERT INTO save_blk_b (%s) SELECT %s FROM blk_b WHERE M304ID=%d",$to_blk_b,$from_blk_b,$id);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
    // save_blk_c
    $from_blk_c = sprintf("SEQ,M304ID,'%s',PKG_ID,ID,VALID,ROOM,REGION,ORD,LIFECOUNT,CCMTYPE,NAME",$post['FILENAME']);
    $to_blk_c = "SEQ,M304ID,FILENAME,PKG_ID,ID,VALID,ROOM,REGION,ORD,LIFECOUNT,CCMTYPE,NAME";
    $sql = sprintf("INSERT INTO save_blk_c (%s) SELECT %s FROM blk_c WHERE M304ID=%d",$to_blk_c,$from_blk_c,$id);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
    // save_blk_d
    $from_blk_d = sprintf("SEQ,M304ID,'%s',PKG_ID,ID,VALID,ROOM,REGION,ORD,PRIORITY,LV,CAST,UNIT,CCMTYPE,NAME",$post['FILENAME']);
    $to_blk_d = "SEQ,M304ID,FILENAME,PKG_ID,ID,VALID,ROOM,REGION,ORD,PRIORITY,LV,CAST,UNIT,CCMTYPE,NAME";
    $sql = sprintf("INSERT INTO save_blk_d (%s) SELECT %s FROM blk_d WHERE M304ID=%d",$to_blk_d,$from_blk_d,$id);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
    // save_blk_ope
    $from_blk_ope = sprintf("SEQ,'%s',OPE_ID,PKG_ID,M304ID,RLY,RLYSTATUS",$post['FILENAME']);
    $to_blk_ope = "SEQ,FILENAME,OPE_ID,PKG_ID,M304ID,RLY,RLYSTATUS";
    $sql = sprintf("INSERT INTO save_blk_ope (%s) SELECT %s FROM blk_ope WHERE M304ID=%d",$to_blk_ope,$from_blk_ope,$id);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
    // save_ope_c
    $from_ope_c = sprintf("SEQ,M304ID,'%s',PKG_ID,OPE_ID,NAME,COMMENT",$post['FILENAME']);
    $to_ope_c = "SEQ,M304ID,FILENAME,PKG_ID,OPE_ID,NAME,COMMENT";
    $sql = sprintf("INSERT INTO save_ope_c (%s) SELECT %s FROM ope_c WHERE M304ID=%d",$to_ope_c,$from_ope_c,$id);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
    // save_pkg_c
    $from_pkg_c = sprintf("SEQ,M304ID,'%s',PKG_ID,NAME,COMMENT,REG_DATE,AUTHOR,base,free",$post['FILENAME']);
    $to_pkg_c = "SEQ,M304ID,FILENAME,PKG_ID,NAME,COMMENT,REG_DATE,AUTHOR,base,free";
    $sql = sprintf("INSERT INTO save_pkg_c (%s) SELECT %s FROM pkg_c WHERE M304ID=%d",$to_pkg_c,$from_pkg_c,$id);
    $sth = $pdo->prepare($sql);
    $rv = $sth->execute();
}

function delete_save_file($pdo,$id,$fln) {
    $tbl = array( "save_blk_a", "save_blk_b", "save_blk_c", "save_blk_d",
        "save_blk_ope", "save_dir", "save_ope_c", "save_pkg_c");
    foreach($tbl as $t) {
        $sql = sprintf("DELETE FROM %s WHERE M304ID=:m304id AND FILENAME=:filename",$t);
        $sth = $pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
        $sth->bindParam('m304id',$id,PDO::PARAM_INT);
        $sth->bindParam('filename',$fln,PDO::PARAM_STR);
        $rv = $sth->execute();
    }
}

function isExist_save_file($pdo,$id,$fln) {
    $query = sprintf("SELECT count(*) FROM save_dir WHERE M304ID=%d AND FILENAME='%s'",
                     $id,$fln);
    $stmt = $pdo->query($query);
    $r = $stmt->fetch();
    if ($r[0]==0) {
        return(false);
    }
    return(true);
}

function select_save_dir($pdo,$id="") {
    if ($id=="") {
        $query = 'SELECT * FROM save_dir order by M304ID,TOD';
        $stmt = $pdo->query($query);
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    } else {
        $query = sprintf('SELECT * FROM save_dir where M304ID=%d',$id);
        $stmt = $pdo->query($query);
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    }
    return($data);
}

// Make sample data
/*
insert into save_blk_a(seq,m304id,filename) values (4,1,'test002');
insert into save_blk_b(seq,m304id,filename) values (4,1,'test002');
insert into save_blk_c(seq,m304id,filename) values (4,1,'test002');
insert into save_blk_d(seq,m304id,filename) values (4,1,'test002');
insert into save_blk_ope(seq,m304id,filename) values (4,1,'test002');
insert into save_dir(m304id,filename) values (1,'test002');
insert into save_ope_c(seq,m304id,filename) values (4,1,'test002');
insert into save_pkg_c(seq,m304id,filename) values (4,1,'test002');
*/

?>
