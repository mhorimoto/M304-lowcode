<?php
//
//  INFO
//
$s->assign("ihex",$infomethod['ihex']);
//
//  RX
//
//
//  OPE
//
for ($i=0;$i<10;$i++) {
    $ope_cmpsel[$i] = $ope_cmp[$i];
    if ($ope_valid[$i]=="on") {
        $checked[$i] = "checked";
    } else {
        $checked[$i] = "";
    }
    $opemethod[$i] = array(
        "checked"=>$ope_checked[$i],"valid"=>$ope_valid[$i],"ccmtype"=>$ope_ccmtype[$i],
        "cmpsel"=>$ope_cmpsel[$i],
        "cmp"=>$ope_cmp[$i],"fval"=>$ope_fval[$i],"sum"=>$ope_sum[$i],"room"=>$ope_room[$i],
        "region"=>$ope_region[$i],"order"=>$ope_order[$i],"lifecnt"=>$ope_lifecnt[$i]
    );
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
$s->assign("OPEMETHOD",$opemethod);
//
//  TX
//
?>
