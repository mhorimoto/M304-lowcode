<?php
function init_data($AAA,&$infomethod,&$rxmethod,&$opemethod,&$txmethod) {
    global $s;
    if (count($AAA)==0) {
        $dhcpf = "checked";
        $nodename = "";
        $uecsid = "01100C00000B";
        $mac = "02:a2:73:00:00:00";
        $ihex = "NON";
        $ihexope = "NON";
    } else {
        $nodename = $AAA['INFO_NODENAME'];
        $uecsid = $AAA['INFO_UECSID'];
        $mac = $AAA['INFO_MAC'];
        $fixip = $AAA['INFO_FIXIP'];
        if ($AAA['INFO_DHCPF']=="on") {
            $dhcpf = "checked";
        } else {
            $dhcpf = "";
        }
        $ihex = $AAA['ihex'];
        $ihexope = $AAA['IHEXOPE'];
        //        var_dump($ihexope);
    }
    $s->assign("ihexope",$ihexope);
    $vender = array(
        array("code"=>"AMPSD", "name"=>"AMPSD"),
        array("code"=>"HOLLY", "name"=>"HOLLY&amp;Co.,Ltd."),
        array("code"=>"YSL",   "name"=>"LLC YS Lab")
    );
    $s->assign("VEN",$vender);

    $infomethod = array("uecsid"=>$uecsid,"nodename"=>$nodename,"vencode"=>$AAA['INFO_VEN'],
                        "vender"=>$vender,"mac"=>$mac,"dhcpf"=>$dhcpf,"ihex"=>$ihex,"fixip"=>$fixip
    );
    $s->assign("INFOMETHOD",$infomethod);

    $rlyopt = array(
        array("v"=>"N", "name"=>"-"),
        array("v"=>"B", "name"=>"B"),
        array("v"=>"M", "name"=>"M"),
        array("v"=>"T", "name"=>"T")
    );
    $s->assign("RLY",$rlyopt);

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
    $s->assign("CMPFUNC",$cmpfunc);

    $funcope = array(
        array("f"=>"FF", "name"=>"-"),
        array("f"=>"06", "name"=>"&amp;&amp;"),
        array("f"=>"07", "name"=>"||")
    );
    $s->assign("FUNCOPE",$funcope);

    $cmpope = array(
        array("ope"=>"FF", "name"=>"-"),
        array("ope"=>"01", "name"=>"=="),
        array("ope"=>"02", "name"=>"&gt;"),
        array("ope"=>"03", "name"=>"&lt;"),
        array("ope"=>"04", "name"=>"&ge;"),
        array("ope"=>"05", "name"=>"&le;"),
        array("ope"=>"08", "name"=>"!="),
    );
    $s->assign("CMPOPE",$cmpope);

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
    $s->assign("LV",$lvarr);

    //
    // OPE PARAMETERS
    //
    $ope_valid   = $AAA['OPE_VALID'];
    $ope_ccmtype = $AAA['OPE_CCMTYPE'];
    $ope_cmp     = $AAA['OPE_CMP'];
    $ope_fval    = $AAA['OPE_FVAL'];
    $ope_sum     = $AAA['OPE_SUM'];
    $ope_room    = $AAA['OPE_ROOM'];
    $ope_region  = $AAA['OPE_REGION'];
    $ope_order   = $AAA['OPE_ORD'];
    $ope_lifecnt = $AAA['OPE_LIFECNT'];
    $opemethod = array( );
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
    $s->assign("OPEMETHOD",$opemethod);
    
    $rxmethod = array(
    );
    $txmethod = array(
        array("ccm"=>"cnd",  "desc"=>"機器状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"29","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr1", "desc"=>"リレー1の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr2", "desc"=>"リレー2の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr3", "desc"=>"リレー3の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr4", "desc"=>"リレー4の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr5", "desc"=>"リレー5の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr6", "desc"=>"リレー6の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr7", "desc"=>"リレー7の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"RelayOpr8", "desc"=>"リレー8の状態",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"15","cast"=>"0","unit"=>"none","lv"=>"02"),
        array("ccm"=>"aux", "desc"=>"予備",
              "room"=>"1","region"=>"1","order"=>"1","pri"=>"29","cast"=>"0","unit"=>"none","lv"=>"02")
    );
    // if (count($AAA)>=0) {
    //     for ($i=0;$i<10;$i++) {
    //         $txmethod[$i]['room']   = $AAA['TX_ROOM'][$i];
    //         $txmethod[$i]['region'] = $AAA['TX_REGION'][$i];
    //         $txmethod[$i]['order']  = $AAA['TX_ORDER'][$i];
    //         $txmethod[$i]['priority'] = $AAA['TX_PRIORITY'][$i];
    //         $txmethod[$i]['ccm']      = $AAA['TX_CCMTYPE'][$i];
    //         $txmethod[$i]['cast']     = $AAA['TX_CAST'][$i];
    //         $txmethod[$i]['unit']     = $AAA['TX_UNIT'][$i];
    //         $txmethod[$i]['sellvl']   = $AAA['TX_LEV'][$i];
    //     }
    // }
    $s->assign("TXMETHOD",$txmethod);
}
?>
