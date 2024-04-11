<?php
require_once('Smarty.class.php');
require_once('ope_ccmtbl.php');

$s = new Smarty();
$EM=$_POST["EXECMODE"];
$AFN=$_POST["afn"];
$VEN=$_POST["ven"];
$UECSID=$_POST["uecsid"];
$MACA=$_POST["mac"];
$DHCPF=$_POST["dhcpf"];
$FIXEDIP=@$_POST["FIXIP"];
$BITMASK=@$_POST["BITMASK"];
$GATEWAY=@$_POST["GATEWAY"];
$DNS=@$_POST["DNS"];
$AAA=$_POST;
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

if ($EM=="NodeInfo Build") {
    $uecs  = uecsid2hex($UECSID);
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
} else {
    $s->assign("DHCPCHECK","checked");
    $s->assign("MAC","02:a2:73:");
    $s->assign("ihex","NON");
    $s->assign("VENCODE","");
}
$s->assign("RLY",$rlyopt);
$s->assign("VEN",$vender);
if ($EM=="Build") {
    for ($i=0;$i<8;$i++) {
        $ccmtbl=mk4ccmtbl($AAA,$i);
        $hv = mk4hexdump($ccmtbl,16);
        $tv = hex2chr($hv,16);
        if (substr($ccmtbl,0,2)=="01") {
            $ep .= mk4eepromimage($hv,16,0x80+(0x10*$i*2));
        }
        $s->assign("hCCML$i",$hv);
        $s->assign("tCCML$i",$tv);
        $hv = mk4hexdump(substr($ccmtbl,32,32),16);
        $tv = hex2chr($hv,16);
        if (substr($ccmtbl,0,2)=="01") {
            $ep .= mk4eepromimage($hv,16,0x90+(0x10*$i*2));
        }
        $s->assign("hCCMH$i",$hv);
        $s->assign("tCCMH$i",$tv);
    }
    $s->assign("eROM",$ep);
    $s->display("hexdump.tpl");
    exit;
} else if ($EM=="arraydump") {
    echo "<pre>\n";
    print_r($AAA);
    echo "</pre>\n";
    exit;
}
$s->display("index.tpl");
exit;


?>
