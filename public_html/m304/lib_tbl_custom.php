<?php
function mk_tbl_custom_index($pdo) {
    $d = fetch_tbl_index($pdo,"tbl_custom order by FARM_ID");
    $r = array();
    foreach($d as $a) {
        $r += array($a['FARM_ID']=>$a['NAME']);
    }
    return($r);
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

function select_tbl_custom($pdo,$com_id="",$farm_id="") {
    if (($com_id!="") && ($farm_id!="")) {
        $query = sprintf("SELECT * FROM tbl_custom WHERE COM_ID=%d AND FARM_ID=%d",$com_id,$farm_id);
        $stmt = $pdo->query($query);
        $data = $stmt->fetch(PDO::FETCH_ASSOC);
    } else {
        $query = "SELECT * FROM tbl_custom";
        $stmt = $pdo->query($query);
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    }
    return($data);
}
?>
