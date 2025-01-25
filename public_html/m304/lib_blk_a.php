<?php
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

function select_blk_a($pdo,$s="",$id="") {
    if ($id=="") {
        $query = 'SELECT SEQ,M304ID,hex(UECSID) as UECSID,hex(MACADDR) as MACADDR,'.
               'DHCPFLAG,inet_ntoa(FIXED_IPADDRESS) as FIXED_IPADDRESS,'.
               'inet_ntoa(FIXED_NETMASK) as FIXED_NETMASK,inet_ntoa(FIXED_DEFGW) as FIXED_DEFGW,'.
               'inet_ntoa(FIXED_DNS) as FIXED_DNS,VENDER_NAME,NODE_NAME,PKG_ID,'.
               'COMMENT,COM_ID,FARM_ID FROM blk_a order by M304ID';
        $stmt = $pdo->query($query);
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
        $cindex = fetch_tbl_index($pdo,"tbl_custom");
        if ($s!="") {
            $s->assign('cindex',$cindex);
            $s->assign('dhcptbl',array(255=>'DHCP',0=>'STATIC'));
            $s->assign('pindex',mk_pkg_c_index($pdo));
            $s->assign('findex',mk_tbl_custom_index($pdo));
        }
    } else {
        $query = sprintf('SELECT SEQ,M304ID,hex(UECSID) as UECSID,hex(MACADDR) as MACADDR,'.
                         'DHCPFLAG,inet_ntoa(FIXED_IPADDRESS) as FIXED_IPADDRESS,'.
                         'inet_ntoa(FIXED_NETMASK) as FIXED_NETMASK,inet_ntoa(FIXED_DEFGW) as FIXED_DEFGW,'.
                         'inet_ntoa(FIXED_DNS) as FIXED_DNS,VENDER_NAME,NODE_NAME,PKG_ID,'.
                         'COMMENT,COM_ID,FARM_ID FROM blk_a where M304ID=%d',$id);
        $stmt = $pdo->query($query);
        $data = $stmt->fetch(PDO::FETCH_ASSOC);
    }
    return($data);
}
?>
