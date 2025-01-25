<?php
function mk_pkg_c_index($pdo) {
    $d = fetch_tbl_index($pdo,"pkg_c order by PKG_ID");
    $r = array();
    foreach($d as $a) {
        $r += array($a['PKG_ID']=>$a['NAME']);
    }
    return($r);
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

?>
