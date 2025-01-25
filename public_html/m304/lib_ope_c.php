<?php
function mk_select_ope_c($pdo,$pkgid="") {
    if ($pkgid=="") return(false);
    $query = sprintf("SELECT * FROM ope_c WHERE PKG_ID=%d",$pkgid);
    try {
        $stmt = $pdo->query($query);
        $msg = "";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
        printf("<pre>%s</pre>",$msg);
        exit;
    }
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $sellist = array();
    foreach($data as $row) {
        $sellist += array($row['OPE_ID']=>$row['NAME']);
    }
    return($sellist);
}
?>
