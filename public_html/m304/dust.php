
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
