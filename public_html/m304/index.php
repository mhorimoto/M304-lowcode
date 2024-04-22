<?php
$Version="0.70";

require_once('./init_data.php');
require_once('Smarty.class.php');
$AAA = $_POST;
$s = new Smarty();
$s->assign("VERSION",$Version);
init_data($_POST,$infomethod,$r,$opemethod,$txmethod);
require_once('ope_ccmtbl.php');
require_once('./pushPOST.php');

$target = "192.168.11.26";

$s->assign("INFOMETHOD",$infomethod);
$s->assign("OPEMETHOD",$opemethod);

//echo "<pre>\n";
//var_dump($opemethod);
//echo "</pre>\n";

switch ($AAA["EXECMODE"]) {
case "NodeInfo Build":
    $uecs = uecsid2hex($AAA['INFO_UECSID']);
    $maca = mac2hex($AAA['INFO_MAC']);
    $nodn = chr2hex($AAA['INFO_NODENAME'],16);
    $venn = chr2hex($AAA['INFO_VEN'],16);
    if ($AAA['INFO_DHCPF']=="on") {
        $ipc = "FFFFFFFF";
        $nmk = "FFFFFFFF";
        $gwy = "FFFFFFFF";
        $dns = "FFFFFFFF";
        $dhc = "FF";
    } else {
        $ipac = explode('.',$FIXEDIP);
        $ipc  = sprintf("%02X%02X%02X%02X",intval($ipac[0]),intval($ipac[1]),intval($ipac[2]),intval($ipac[3]));
        $nmkc = explode('.',$BITMASK); 
        $nmk  = sprintf("%02X%02X%02X%02X",intval($nmkc[0]),intval($nmkc[1]),intval($nmkc[2]),intval($nmkc[3]));
        $gtwc = explode('.',$GATEWAY); 
        $gwy  = sprintf("%02X%02X%02X%02X",intval($gtwc[0]),intval($gtwc[1]),intval($gtwc[2]),intval($gtwc[3]));
        $dnsc = explode('.',$DNS); 
        $dns  = sprintf("%02X%02X%02X%02X",intval($dnsc[0]),intval($dnsc[1]),intval($dnsc[2]),intval($dnsc[3]));
        $dhc = "00";
    }
    $ihex  = sprintf("a2sender http://%s/:10000000%s%s%sFFFFFFFF\n",$target,$uecs,$maca,$dhc);
    $ihex .= sprintf("a2sender http://%s/:10101000%s%s%s%sFF\n",$target,$ipc,$nmk,$gwy,$dns);
    $ihex .= sprintf("a2sender http://%s/:10102000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n",$target);
    $ihex .= sprintf("a2sender http://%s/:10103000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n",$target);
    $ihex .= sprintf("a2sender http://%s/:10104000%sFF\n",$target,$venn);
    $ihex .= sprintf("a2sender http://%s/:10105000%sFF\n",$target,$nodn);
    $infomethod['ihex'] = $ihex;
    // $infomethod['nodename'] = $AAA['INFO_NODENAME'];
    // $infomethod['uecsid'] = $AAA['INFO_UECSID'];
    // $infomethod['mac'] = $AAA['INFO_MAC'];
    // $infomethod['vencode'] = $AAA['INFO_VEN'];
    break;
case "RX Build":
    // a2.py id room region order priority lv cast sr ccm_type unit sthr stmn edhr edmn inmn dumn rly(8characters)
    $ihexrx = "";
    for ($k=0;$k<30;$k++) {
        if ($rx_valid[$k]=="on") {
            if ($rx_unit[$k]=="") {
                $rx_unit[$k]="none";
            }
            if ($rx_cast[$k]=="") {
                $rx_cast[$k]=0;
            }
            $c = sprintf("/home/staff/horimoto/bin/a2.py %d %s %s %s %s %s %s R %s %s %s %s %s %s %s %s %s%s%s%s%s%s%s%s",
                         $k,$rx_room[$k],$rx_region[$k],$rx_order[$k],$rx_priority[$k],$rx_lev[$k],$rx_cast[$k],
                         $rx_ccmtype[$k],$rx_unit[$k],$rx_sthr[$k],$rx_stmn[$k],$rx_edhr[$k],$rx_edmn[$k],
                         $rx_inmn[$k],$rx_dumn[$k],
                         $rx_rly1[$k],$rx_rly2[$k],$rx_rly3[$k],$rx_rly4[$k],$rx_rly5[$k],
                         $rx_rly6[$k],$rx_rly7[$k],$rx_rly8[$k]);
            unset($r);
            if (exec($c,$r)) {
                for ($i=0;$i<count($r);$i++) {
                    $ihexrx .= sprintf("a2sender http://%s/%s\n",$target,$r[$i]);
                }
                $adr = 0x1030 + (0x40*$k);
                $ihexrx .= sprintf("a2sender http://%s/:10%04X00%s%s%s%s%s%s%s%s%sFFFFFFFFFFFFFFFF\n",$target,
                                   $adr,$rx_cond0[$k],$rx_cmp1[$k],$rx_cond1[$k],$rx_cmp2[$k],$rx_cond2[$k],
                                   $rx_cmp3[$k],$rx_cond3[$k],$rx_cmp4[$k],$rx_cond4[$k]);
            }
        }
    }
    if ($ihexrx!="") {
        $s->assign("ihexrx",$ihexrx);
    }
    for($i=0;$i<30;$i++) {
        if ($rx_valid[$i]=="on") {
            $rx_checked[$i] = "checked";
        } else {
            $rx_checked[$i] = "";
        }
    }
    $s->assign("MAC",$MACA);
    $s->assign("VENCODE",$VEN);
    $s->assign("RX_CHECKED",$rx_checked);
    $s->assign("CMPFUNCSEL0",$rx_cond0);
    $s->assign("FUNCOPESEL1",$rx_cmp1);
    $s->assign("CMPFUNCSEL1",$rx_cond1);
    $s->assign("FUNCOPESEL2",$rx_cmp2);
    $s->assign("CMPFUNCSEL2",$rx_cond2);
    $s->assign("FUNCOPESEL3",$rx_cmp3);
    $s->assign("CMPFUNCSEL3",$rx_cond3);
    $s->assign("FUNCOPESEL4",$rx_cmp4);
    $s->assign("CMPFUNCSEL4",$rx_cond4);
    $s->assign("RXSTHR",$rx_sthr);
    $s->assign("RXSTMN",$rx_stmn);
    $s->assign("RXEDHR",$rx_edhr);
    $s->assign("RXEDMN",$rx_edmn);
    $s->assign("RXINMN",$rx_inmn);
    $s->assign("RXDUMN",$rx_dumn);
    $s->assign("RXINMN",$rx_inmn);
    $s->assign("RXDUMN",$rx_dumn);
    $s->assign("RXCAST",$rx_cast);
    $s->assign("RXUNIT",$rx_unit);
    $s->assign("RXLEVSEL",$rx_lev);
    $s->assign("RXROOM",$rx_room);
    $s->assign("RXREGION",$rx_region);
    $s->assign("RXORDER",$rx_order);
    $s->assign("RXPRIORITY",$rx_priority);
    $s->assign("RX_CCMT",$rx_ccmtype );
    $s->assign("RLYCODE1",$rx_rly1);
    $s->assign("RLYCODE2",$rx_rly2);
    $s->assign("RLYCODE3",$rx_rly3);
    $s->assign("RLYCODE4",$rx_rly4);
    $s->assign("RLYCODE5",$rx_rly5);
    $s->assign("RLYCODE6",$rx_rly6);
    $s->assign("RLYCODE7",$rx_rly7);
    $s->assign("RLYCODE8",$rx_rly8);
    $s->assign("cond0",$rx_cond0);
    $s->assign("cmp1",$rx_cmp1);
    $s->assign("cond1",$rx_cond1);
    $s->assign("cmp2",$rx_cmp2);
    $s->assign("cond2",$rx_cond2);
    $s->assign("cmp3",$rx_cmp3);
    $s->assign("cond3",$rx_cond3);
    $s->assign("cmp4",$rx_cmp4);
    $s->assign("cond4",$rx_cond4);
    //    echo "<pre>\n";
    //    $tx = $_POST["TX"];
    //    var_dump($tx[1]);
    //    exit;
    break;
case "TX Build":
    // a2.py id room region order priority lv cast sr ccm_type unit sthr stmn edhr edmn inmn dumn rly(8characters)
    $ihextx = "";
    $txmethod[0]['ccm']    = $AAA["TX_CCMTYPE"][0];
    $c = sprintf("/home/staff/horimoto/bin/a2.py 0 %s %s %s 29 2 0 S %s none 0 0 0 0 0 0 NNNNNNNN",
                 $AAA["TX_ROOM"][0],$AAA["TX_REGION"][0],$AAA["TX_ORDER"][0],$AAA["TX_CCMTYPE"][0]);
    $ihextx = "";
    unset($r);
    if (exec($c,$r)) {
        for ($i=0;$i<count($r);$i++) {
            $ihextx .= sprintf("a2sender http://%s/%s\n",$target,$r[$i]);
        }
    }
    for ($i=1;$i<10;$i++) {
        $txmethod[$i]['room']   = $AAA["TX_ROOM"][$i];
        $txmethod[$i]['region'] = $AAA["TX_REGION"][$i];
        $txmethod[$i]['order']  = $AAA["TX_ORDER"][$i];
        $txmethod[$i]['pri']    = $AAA["TX_PRIORITY"][$i];
        $txmethod[$i]['lv']     = $AAA["TX_LEV"][$i];
        $txmethod[$i]['ccm']    = $AAA["TX_CCMTYPE"][$i];
        $txmethod[$i]['cast']   = $AAA["TX_CAST"][$i];
        if ($AAA["TX_UNIT"][$i]=="") {
            $u = "none";
        } else {
            $u = $AAA["TX_UNIT"][$i];
        }
        $c = sprintf("/home/staff/horimoto/bin/a2.py %d %s %s %s %s %s %s S %s %s 0 0 0 0 0 0 NNNNNNNN",
                     $i,$AAA["TX_ROOM"][$i],$AAA["TX_REGION"][$i],$AAA["TX_ORDER"][$i],$AAA["TX_PRIORITY"][$i],
                     $AAA["TX_LEV"][$i],$AAA["TX_CAST"][$i],$AAA["TX_CCMTYPE"][$i],$u);
        unset($r);
        if (exec($c,$r)) {
            for ($n=0;$n<count($r);$n++) {
                $ihextx .= sprintf("a2sender http://%s/%s\n",$target,$r[$n]);
            }
        }
    }
    $s->assign("TXMETHOD",$txmethod);
    $s->assign("ihextx",$ihextx);
    break;
case "Ope Build":
    $ihexope = "";
    for ($k=0;$k<10;$k++) {
        if (@$ope_valid[$k]=="on") {
            unset($r);
            $ope_checked[$k] = "checked";
            $ope_cmpsel[$k] = $ope_cmp[$k];
            $c = sprintf("/home/staff/horimoto/bin/a1.py %s %s %s %s %s %s %s %s\n",
                         $k,$ope_room[$k],$ope_region[$k],$ope_order[$k],$ope_lifecnt[$k],
                         $ope_ccmtype[$k],$ope_cmp[$k],$ope_fval[$k]);
            if (exec($c,$r)) {
                $ihexope .= sprintf("a2sender http://%s/%s\n",$target,$r[0]);
            }
        }
    }
    $ihexope .= $c;
    $s->assign("ihexope",$ihexope);
    include("./assignPOP.php");
    //    $s->assign("VENCODE",$VEN);
    $s->assign("RLYCODE","");
    $s->assign("TXLEVSEL","");
    break;
default:
    for ($k=0;$k<30;$k++) {
        $RXCCMT[$k] = sprintf("Schedule%02d",$k);
        $init_rro[$k] = 1;
        $init_prio[$k] = 15;
        $none[$k] = "none";
        $zero[$k] = 0;
    }
    $s->assign("INFOMETHOD",$infomethod);
    $s->assign("OPEMETHOD",$opemethod);
    $s->assign("RXUNIT","");
    $s->assign("RXROOM",$init_rro);
    $s->assign("RXREGION",$init_rro);
    $s->assign("RXORDER",$init_rro);
    $s->assign("RXPRIORITY",$init_prio);
    $s->assign("RX_CCMT",$RXCCMT);
    $s->assign("RLYCODE","");
    $s->assign("CMPFUNCSEL","");
    $s->assign("FUNCOPESEL","");
    $s->assign("RXLEVSEL","");
    $s->assign("TXLEVSEL","");
    $s->assign("EXECMODE","");
    $s->assign("OPE_SUM_CHECKED","");
    $s->assign("TXMETHOD",$txmethod);
}
include("./assignPOP.php");
$s->display("index.tpl");
exit;


?>
