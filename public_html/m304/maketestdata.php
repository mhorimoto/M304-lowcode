<?php
function mksampledata() {
    $data['ID'] = 39;
    $data['NODE_NAME'] = 'irrig01';
    $data['FARM_NAME'] = '九州大学 x welzo共同研究農場';
    $dhcpflag = 0;
    if ($dhcpflag==0) {
        $data['DHCP'] = '固定';
    } else {
        $data['DHCP'] = 'DHCP';
    }
    $data['FIXED_IPADDRESS'] = "192.168.120.171";
    $data['FIXED_NETMASK']   = "255.255.254.0";
    $data['FIXED_GATEWAY']   = "192.168.120.1";
    $data['DNS']             = "192.168.120.1";
    $data['COMMENT']         = "M304葉面散布システム第1棟";
    $data['OPE_ID'][1] = 1;
    $data['OPE_ID'][2] = 10;
    $data['OPE_ID'][3] = 13;
    $data['OPE_ID'][4] = 20;
    $data['OPE_ID'][5] = 21;
    $data['OPE_ID'][6] = 22;
    $data['OPE_ID'][8] = 30;
    $data['OPE_ID'][7] = 23;
    $data['OPE_ID'][9] = 31;
    $data['OPE_ID'][10] = 40;
    // BLK_C
    for ($i=0;$i<10;$i++) {
        for ($j=0;$j<8;$j++) {
            $data['RLYSTATUS'][$i+1][$j+1]    = ($i+$j) % 4;
        }
        $data['BLK_C'][$i]['PKG_ID'] = 39;
        $data['BLK_C'][$i]['ID'] = $i;
        $data['BLK_C'][$i]['VALID'] = 0;
        $data['BLK_C'][$i]['ROOM'] = 1;
        $data['BLK_C'][$i]['REGION'] = 1;
        $data['BLK_C'][$i]['ORD'] = $i+1;
        $data['BLK_C'][$i]['CCMTYPE'] = "";
        $data['BLK_C'][$i]['NAME'] = "";
    }
    $data['BLK_C'][0]['CCMTYPE'] = "InAirTemp";
    $data['BLK_C'][0]['NAME'] = "室内気温";
    $data['BLK_C'][0]['VALID'] = 1;
    $data['BLK_C'][1]['CCMTYPE'] = "InAirHumid";
    $data['BLK_C'][1]['NAME'] = "室内湿度";
    $data['BLK_C'][1]['VALID'] = 1;
    $data['BLK_C'][2]['CCMTYPE'] = "InAirCO2";
    $data['BLK_C'][2]['NAME'] = "室内CO2";
    $data['BLK_C'][2]['VALID'] = 1;
    $data['BLK_C'][3]['CCMTYPE'] = "InRadiation";
    $data['BLK_C'][3]['NAME'] = "室内日射量";
    $data['BLK_C'][3]['VALID'] = 1;
    // BLK_B
    for ($i=0;$i<30;$i++) {
        $data['BLK_B'][$i]['PKG_ID'] = 39;
        $data['BLK_B'][$i]['ID'] = $i;
        $data['BLK_B'][$i]['VALID'] = 0;
    }
    
    return $data;
}

?>
    
