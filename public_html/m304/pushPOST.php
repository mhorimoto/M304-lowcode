<?php

//
// For Node and IP
//
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


$dbgf = @$_POST['DBGF'];
if ($dbgf=="on") {
    $s->assign("DBG",true);
} else {
    $s->assign("DBG",false);
}
?>
    
