<?php

function uecsid2hex($it) {
    if (strlen($it)!=12) {
        return("000000000000");
    } else {
        return(strtoupper($it));
    }
}

function mac2hex($it) {
    $maca = str_replace(":","",$it);
    return(strtoupper($maca));
}

function chr2hex($it,$cnt) {
    $array_it = str_split($it);
    $hchr = "";
    foreach($array_it as $ai) {
        $hchr .= sprintf("%02X",ord($ai));
    }
    $hchr .= str_repeat("00",$cnt);
    $hchr = substr($hchr,0,$cnt*2);
    return($hchr);
}

?>
