<?php
require_once('Smarty.class.php');
require_once('ope_ccmtbl.php');

// Version: 0.60
    
$s = new Smarty();
$AAA    = $_POST;
$EM     = @$_POST["EXECMODE"];

//
// For Node and IP
//
$AFN        = @$_POST["afn"];
$VEN        = @$_POST["ven"];
$UECSID     = @$_POST["uecsid"];
$MACA       = @$_POST["mac"];
$DHCPF      = @$_POST["dhcpf"];
$FIXEDIP    = @$_POST["FIXIP"];
$BITMASK    = @$_POST["BITMASK"];
$GATEWAY    = @$_POST["GATEWAY"];
$DNS        = @$_POST["DNS"];
//
// For RX
//
$rx_valid   = @$_POST["RX_VALID"];
$rx_sthr    = @$_POST["RX_STHR"];
$rx_stmn    = @$_POST["RX_STMN"];
$rx_edhr    = @$_POST["RX_EDHR"];
$rx_edmn    = @$_POST["RX_EDMN"];
$rx_inmn    = @$_POST["RX_INMN"];
$rx_dumn    = @$_POST["RX_DUMN"];
$rx_inmn    = @$_POST["RX_INMN"];
$rx_dumn    = @$_POST["RX_DUMN"];
$rx_cast    = @$_POST["RX_CAST"];
$rx_unit    = @$_POST["RX_UNIT"];
$rx_lev     = @$_POST["RX_Lev"];
$rx_room    = @$_POST["RX_ROOM"];
$rx_region  = @$_POST["RX_REGION"];
$rx_order   = @$_POST["RX_ORD"];
$rx_priority= @$_POST["RX_PRIORITY"];
$rx_ccmtype = @$_POST["RX_CCMTYPE"];
$rx_rly1    = @$_POST["RX_RLY1"];
$rx_rly2    = @$_POST["RX_RLY2"];
$rx_rly3    = @$_POST["RX_RLY3"];
$rx_rly4    = @$_POST["RX_RLY4"];
$rx_rly5    = @$_POST["RX_RLY5"];
$rx_rly6    = @$_POST["RX_RLY6"];
$rx_rly7    = @$_POST["RX_RLY7"];
$rx_rly8    = @$_POST["RX_RLY8"];
$rx_cond0   = @$_POST["cond0"];
$rx_cmp1    = @$_POST["cmp1"];
$rx_cond1   = @$_POST["cond1"];
$rx_cmp2    = @$_POST["cmp2"];
$rx_cond2   = @$_POST["cond2"];
$rx_cmp3    = @$_POST["cmp3"];
$rx_cond3   = @$_POST["cond3"];
$rx_cmp4    = @$_POST["cmp4"];
$rx_cond4   = @$_POST["cond4"];
//
// For OPE
//
$ope_valid   = @$_POST["OPE_VALID"];
$ope_ccmtype = @$_POST["OPE_CCMTYPE"];
$ope_cmp     = @$_POST["OPE_CMP"];
$ope_fval    = @$_POST["OPE_FVAL"];
$ope_room    = @$_POST["OPE_ROOM"];
$ope_region  = @$_POST["OPE_REGION"];
$ope_order   = @$_POST["OPE_ORD"];
$ope_lifecnt = @$_POST["OPE_LIFECNT"];
//echo "<pre>\n";
//echo $AAA["EXECMODE"]."\n";
//echo $EM."\n";
//var_dump($AAA);
//exit;
$target = "192.168.11.26";
$vender = array(
    array("code"=>"AMPSD", "name"=>"AMPSD"),
    array("code"=>"HOLLY", "name"=>"HOLLY&amp;Co.,Ltd."),
    array("code"=>"YSL",   "name"=>"LLC YS Lab")
);

$rlyopt = array(
    array("v"=>"N", "name"=>"-"),
    array("v"=>"B", "name"=>"B"),
    array("v"=>"M", "name"=>"M"),
    array("v"=>"T", "name"=>"T")
);

$cmpfunc = array(
    array("f"=>"FF", "name"=>"-"),
    array("f"=>"00", "name"=>"C00"),
    array("f"=>"01", "name"=>"C01"),
    array("f"=>"02", "name"=>"C02"),
    array("f"=>"03", "name"=>"C03"),
    array("f"=>"04", "name"=>"C04"),
    array("f"=>"05", "name"=>"C05"),
    array("f"=>"06", "name"=>"C06"),
    array("f"=>"07", "name"=>"C07"),
    array("f"=>"08", "name"=>"C08"),
    array("f"=>"09", "name"=>"C09")
);

$funcope = array(
    array("f"=>"FF", "name"=>"-"),
    array("f"=>"06", "name"=>"&amp;&amp;"),
    array("f"=>"07", "name"=>"||")
);

$cmpope = array(
    array("ope"=>"FF", "name"=>"-"),
    array("ope"=>"01", "name"=>"=="),
    array("ope"=>"02", "name"=>"&gt;"),
    array("ope"=>"03", "name"=>"&lt;"),
    array("ope"=>"04", "name"=>"&ge;"),
    array("ope"=>"05", "name"=>"&le;"),
    array("ope"=>"08", "name"=>"!="),
);

$lvarr = array(
    array("v"=>"01", "name"=>"A-1S-0"),
    array("v"=>"02", "name"=>"A-1S-1"),
    array("v"=>"03", "name"=>"A-10S-0"),
    array("v"=>"04", "name"=>"A-10S-1"),
    array("v"=>"05", "name"=>"A-1M-0"),
    array("v"=>"06", "name"=>"A-1M-1"),
    array("v"=>"07", "name"=>"B-0"),
    array("v"=>"08", "name"=>"B-1"),
    array("v"=>"09", "name"=>"S-1S-0"),
    array("v"=>"0A", "name"=>"S-1M-0")
);

$txmethod = array(
    array("ccm"=>"cnd",  "desc"=>"機器状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"29","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr1", "desc"=>"リレー1の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr2", "desc"=>"リレー2の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr3", "desc"=>"リレー3の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr4", "desc"=>"リレー4の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr5", "desc"=>"リレー5の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr6", "desc"=>"リレー6の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr7", "desc"=>"リレー7の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"RelayOpr8", "desc"=>"リレー8の状態", "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
    array("ccm"=>"aux", "desc"=>"予備", "room"=>"1","region"=>"1","order"=>"1","pri"=>"29","cast"=>"0","unit"=>"none","lv"=>"02")
);
    
//$s->assign("DHCPCHECK","");
//$s->assign("ihex","NON");
//$s->assign("ihexope","NON");

$s->assign("CMPFUNC",$cmpfunc);
$s->assign("FUNCOPE",$funcope);
$s->assign("CMPOPE",$cmpope);
$s->assign("RLY",$rlyopt);
$s->assign("VEN",$vender);
$s->assign("LV",$lvarr);
$s->assign("TXMETHOD",$txmethod);
//$s->assign("LVL",$lvarr);

if (!@$AAA["EXECMODE"]) {
    $AAA["EXECMODE"] = null;
}

switch ($AAA["EXECMODE"]) {
case "NodeInfo Build":
    $uecs = uecsid2hex($UECSID);
    $maca = mac2hex($MACA);
    $nodn = chr2hex($AFN,16);
    $venn = chr2hex($VEN,16);
    if ($DHCPF=="on") {
        $s->assign("DHCPCHECK","checked");
        $ipc = "FFFFFFFF";
        $nmk = "FFFFFFFF";
        $gwy = "FFFFFFFF";
        $dns = "FFFFFFFF";
        $dhc = "FF";
    } else {
        $s->assign("DHCPCHECK","");
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
    $s->assign("ihex",$ihex);
    $s->assign("AFN",$AFN);
    $s->assign("MAC",$MACA);
    $s->assign("VENCODE",$VEN);
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
    $s->assign("ihex",$ihex);
    $s->assign("AFN",$AFN);
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
    if ($DHCPF=="on") {
        $s->assign("DHCPCHECK","checked");
    } else {
        $s->assign("DHCPCHECK","");
    }
    $s->assign("OPE_VALID",$ope_valid);
    $s->assign("OPE_CHECKED",$ope_checked);
    $s->assign("OPE_CCMTYPE",$ope_ccmtype);
    $s->assign("OPE_CMPSEL",$ope_cmpsel);
    $s->assign("OPE_FVAL",$ope_fval);
    $s->assign("OPE_ROOM",$ope_room);
    $s->assign("OPE_REGION",$ope_region);
    $s->assign("OPE_ORD",$ope_order);
    $s->assign("OPE_LIFECNT",$ope_lifecnt);
    $s->assign("ihexope",$ihexope);
    $s->assign("AFN",$AFN);
    $s->assign("MAC",$MACA);
    $s->assign("VENCODE",$VEN);
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

    $s->assign("RXUNIT","");
    $s->assign("RXROOM",$init_rro);
    $s->assign("RXREGION",$init_rro);
    $s->assign("RXORDER",$init_rro);
    $s->assign("RXPRIORITY",$init_prio);
    $s->assign("RX_CCMT",$RXCCMT);
    $s->assign("DHCPCHECK","checked");
    $s->assign("MAC","02:a2:73:00:00:00");
    $s->assign("ihex","NON");
    $s->assign("ihexrx","NON");
    $s->assign("ihextx","NON");
    $s->assign("ihexope","NON");
    $s->assign("VENCODE","");
    $s->assign("RLYCODE","");
    $s->assign("CMPFUNCSEL","");
    $s->assign("FUNCOPESEL","");
    $s->assign("RXLEVSEL","");
    $s->assign("TXLEVSEL","");
    $s->assign("EXECMODE","");
    $s->assign("OPE_SUM_CHECKED","");
    $s->assign("TXMETHOD",$txmethod);
}


$s->display("index.tpl");
exit;


?>
