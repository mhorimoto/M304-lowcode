<?php
require_once('m304.inc');
require_once('maketestdata.php');
$data = mksampledata();
$displ = "m304body.tpl";
$smarty->assign("rlystatus",constant("RLY_STAT_SEL"));
$smarty->assign("lifecount_sel",constant("LIFECOUNT_SEL"));
$smarty->assign("blk_b_maxrows",30);
$smarty->assign("data",$data);
$smarty->display($displ);
exit;
?>
