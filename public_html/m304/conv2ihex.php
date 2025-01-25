<?php
function conv2ihex($pdo,$prg,$pkg_id,$id="") {
    $query = sprintf("select inet_ntoa(FIXED_IPADDRESS) as IPA from blk_a where pkg_id=%d",$pkg_id);
    try {
        $reply = $pdo->query($query);
        $msg = "";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    $ipaddr = $reply->fetch(PDO::FETCH_ASSOC,1);
    switch($prg) {
        case 'BLK_B':
            $dt_blk_b = select_blk_b_with_pkgid($pdo,$pkg_id);
            foreach($dt_blk_b as $dbb) {
                $ope0 = '';
                $ope1 = '';
                $ope2 = '';
                $ope3 = '';
                $ope4 = '';
                unset($outp);
                $id = $dbb['ID'];
                $valid = ($dbb['VALID']=='1')? 'e' : 'd' ;
                $sthr = $dbb['STHR'];
                $stmn = $dbb['STMN'];
                $edhr = $dbb['EDHR'];
                $edmn = $dbb['EDMN'];
                $inmn = $dbb['INMN'];
                $dumn = $dbb['DUMN'];
                $cmpccmid0 = ($dbb['CMPCCMID0']!=255)? $dbb['CMPCCMID0']:'';
                $cmpccmid1 = ($dbb['CMPCCMID1']!=255)? $dbb['CMPCCMID1']:'';
                $cmpccmid2 = ($dbb['CMPCCMID2']!=255)? $dbb['CMPCCMID2']:'';
                $cmpccmid3 = ($dbb['CMPCCMID3']!=255)? $dbb['CMPCCMID3']:'';
                $cmpccmid4 = ($dbb['CMPCCMID4']!=255)? $dbb['CMPCCMID4']:'';
                if ($cmpccmid0!='') {
                    $ope0 = sprintf("%d %d %f",$cmpccmid0,$dbb['CMPOPE0'],$dbb['CMPVAL0']);
                    if ($cmpccmid1!='') {
                        $ope1 = sprintf("%d %d %d %f",$dbb['CMBCMP1'],$cmpccmid1,$dbb['CMPOPE1'],$dbb['CMPVAL1']);
                        if ($cmpccmid2!='') {
                            $ope2 = sprintf("%d %d %d %f",$dbb['CMBCMP2'],$cmpccmid2,$dbb['CMPOPE2'],$dbb['CMPVAL2']);
                            if ($cmpccmid3!='') {
                                $ope3 = sprintf("%d %d %d %f",$dbb['CMBCMP3'],$cmpccmid3,$dbb['CMPOPE3'],$dbb['CMPVAL3']);
                                if ($cmpccmid4!='') {
                                    $ope4 = sprintf("%d %d %d %f",$dbb['CMBCMP4'],$cmpccmid4,$dbb['CMPOPE4'],$dbb['CMPVAL4']);
                                }
                            }
                        }
                    }
                }
                $dt_blk_ope = select_blk_ope_with_ope_id($pdo,$dbb['OPE_ID'],"oneline");
                $cmnd = sprintf("/usr/local/bin/a3b.py %d %s %d %d %d %d M %d %d %s %s %s %s %s %s",
                                $id,$valid,$sthr,$stmn,$edhr,$edmn,$inmn,$dumn,$dt_blk_ope,
                                $ope0,$ope1,$ope2,$ope3,$ope4);
                exec($cmnd,$outp,$retv);
                foreach($outp as $row) {
                    $a3send[] = sprintf("a2sender http://%s/%s",$ipaddr['IPA'],$row);
                    //printf("%s\n",$a3send);
                }
            }
            break;
        case 'BLK_C':
            $dt_blk_c = select_blk_c($pdo,"",$pkg_id);
            foreach($dt_blk_c as $dbd) {
                unset($outp);
                $id       = $dbd['ID'];
                $valid    = ($dbd['VALID']=='1')? 'e' : 'd' ;
                $room     = $dbd['ROOM'];
                $region   = $dbd['REGION'];
                $order    = $dbd['ORD'];
                $lifecnt  = $dbd['LIFECOUNT'];
                $ccmtype  = ($dbd['CCMTYPE']=='')? '""' : $dbd['CCMTYPE'];
                $cmnd = sprintf("/usr/local/bin/a3c.py %d %s %d %d %d %d %s",
                    $id,$valid,$room,$region,$order,$lifecnt,$ccmtype);
                exec($cmnd,$outp,$retv);
                foreach($outp as $row) {
                    $a3send[] = sprintf("a2sender http://%s/%s",$ipaddr['IPA'],$row);
                }
            }
            break;
        case 'BLK_D':
            $dt_blk_d = select_blk_d($pdo,"",$pkg_id);
            foreach($dt_blk_d as $dbd) {
                unset($outp);
                $id       = $dbd['ID'];
                $valid    = ($dbd['VALID']=='1')? 'e' : 'd' ;
                $room     = $dbd['ROOM'];
                $region   = $dbd['REGION'];
                $order    = $dbd['ORD'];
                $priority = $dbd['PRIORITY'];
                $lv       = $dbd['LV'];
                $cast     = $dbd['CAST'];
                $unit     = ($dbd['UNIT']=='')? '""' : $dbd['UNIT']; 
                $ccmtype  = ($dbd['CCMTYPE']=='')? '""' : $dbd['CCMTYPE'];
                $name     = ($dbd['NAME']=='')? '""' : $dbd['NAME'];
                $cmnd = sprintf("/usr/local/bin/a3d.py %d %s %d %d %d %d %s %s %s %s",
                    $id,$valid,$room,$region,$order,$priority,$lv,$cast,$ccmtype,$unit);
                exec($cmnd,$outp,$retv);
                foreach($outp as $row) {
                    $a3send[] = sprintf("a2sender http://%s/%s",$ipaddr['IPA'],$row);
                    //printf("%s\n",$a3send);
                }
            }
            break;
        default:
    }
    $a3send[] = sprintf("a2sender http://%s/R7500",$ipaddr['IPA']);
    return($a3send);
    //print("</pre>\n");
}

if (@$_GET['PRG']!="") {
    require_once('m304.inc');
    //require_once('maketestdata.php');
    //require_once('lib_blk_d.php');
    
    try {
        $pdo = new PDO($dsn, $username, $password);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    } catch (PDOException $e) {
        echo 'Connection failed: ' . $e->getMessage();
    }
    $a = conv2ihex($pdo,$_GET['PRG'],$_GET['PKG_ID']);
    if (@$_GET['OD']!='F') {
        $OD='';
    } else {
        $id = $_GET['PKG_ID'];
        $query = sprintf("select hex(MACADDR) as MACADDR from blk_a where m304id=%d",$id);
        try {
            $reply = $pdo->query($query);
            $msg = "";
        } catch(PDOException $e) {
            $msg = $e->getMessage();
        }
        $today = getdate();
        $macaddr = $reply->fetch(PDO::FETCH_ASSOC);
        $filename = $ihexdir.$macaddr['MACADDR'].".sh";
        $handle = fopen($filename,"w");
        fprintf($handle,"#! /bin/sh\n");
        fprintf($handle,"# Create at %d/%d/%d %02d:%02d:%02d\n",$today['year'],$today['mon'],$today['mday'],
                $today['hours'],$today['minutes'],$today['seconds']);
        fprintf($handle,"# From: %s\n#\n",$_SERVER['REMOTE_ADDR']);
        for ($i=0;$i<count($a);$i++) {
            fwrite($handle,$a[$i]."\n");
        }
        fclose($handle);
        $OD='F';
    }
    $smarty->assign("IHEXDATA",$a);
    $smarty->display("conv2ihex.tpl");
}
?>
