<?php

define("CCMTYPELIST",
       array(
           array("type"=>"InAirTemp","cast"=>1,"unit"=>"C"),
           array("type"=>"InAirHumid","cast"=>0,"unit"=>"%"),
           array("type"=>"InAirCO2","cast"=>0,"unit"=>"ppm"),
           array("type"=>"InRadiation","cast"=>2,"unit"=>"kW/m^2"),
           array("type"=>"WAirTemp","cast"=>1,"unit"=>"C"),
           array("type"=>"WAirHumid","cast"=>0,"unit"=>"%"),
           array("type"=>"WAirCO2","cast"=>0,"unit"=>"ppm"),
           array("type"=>"WRadiation","cast"=>2,"unit"=>"kW/m^2"),
           array("type"=>"WWindSpeed","cast"=>0,"unit"=>"m/s"),
           array("type"=>"WWindDir16","cast"=>0,"unit"=>""),
           array("type"=>"WRainfall","cast"=>0,"unit"=>""),
           array("type"=>"Time","cast"=>0,"unit"=>""),
           array("type"=>"Date","cast"=>0,"unit"=>"")
       )
);

define("R_NULL",0);
define("R_EQ",1);     // == Equal
define("R_GT",2);     // >  Greater Than
define("R_LT",3);     // <  Less Than
define("R_GE",4);     // >= Greater Than Equal
define("R_LE",5);     // <= Less Than Equal
define("R_AND",6);     // &  Logical AND
define("R_OR",7);     // |  Logical OR
define("R_NE",8);     // != Not Equal
define("R_ADDFLG",0x80);
define("R_FF",0xff);

define("RELAY_OPE",
       array(
           array("RC"=>R_FF,"CHR"=>" "),
           array("RC"=>R_EQ,"CHR"=>"=="),
           array("RC"=>R_GT,"CHR"=>"&gt;"),
           array("RC"=>R_LT,"CHR"=>"&lt;"),
           array("RC"=>R_GE,"CHR"=>"&ge;"),
           array("RC"=>R_LE,"CHR"=>"&le;"),
           array("RC"=>R_NE,"CHR"=>"!=")
       )
);

define("SELECT_RLYOPE",
       array(
           R_FF=>" ",
           R_EQ=>"==",
           R_GT=>"&gt;",
           R_LT=>"&lt;",
           R_GE=>"&ge;",
           R_LE=>"&le;",
           R_NE=>"!="
       )
);

define("CMB_OPEJ",
       array(
           array("RC"=>R_FF,"CHR"=>" "),
           array("RC"=>R_AND,"CHR"=>"且つ"),
           array("RC"=>R_OR,"CHR"=>"もしくは")
       )
);

define("CMB_OPE",
       array(
           array("RC"=>R_FF,"CHR"=>" "),
           array("RC"=>R_AND,"CHR"=>"&amp;&amp;"),
           array("RC"=>R_OR,"CHR"=>"||")
       )
);

define("SELECT_CMBOPEJ",
       array(
           R_FF=>" ",
           R_AND=>"且つ",
           R_OR=>"もしくは"
       )
);

define("SELECT_CMBOPEE",
       array(
           R_FF=>" ",
           R_AND=>"&amp;&amp;",
           R_OR=>"||"
       )
);

function mk_datalist($x) {
    switch($x) {
    case 'ccmlist':
        $data = sprintf("<datalist id=\"%s\">\n",$x);
        foreach(CCMTYPELIST as $ccmt) {
            $data .= sprintf("  <option value=\"%s\"></option>\n",$ccmt['type']);
        }
        $data .= "</datalist>\n";
        break;
    case 'relay_ope':
        $data = "";
        foreach(RELAY_OPE as $rlyope) {
            $data .= sprintf("  <option value=\"%d\">%s</option>\n",$rlyope['RC'],$rlyope['CHR']);
        }
        break;
    case 'cmb_ope':
        $data = "";
        foreach(CMB_OPE as $cmbope) {
            $data .= sprintf("  <option value=\"%d\">%s</option>\n",$cmbope['RC'],$cmbope['CHR']);
        }
        break;
    case 'cmb_opej':
        $data = "";
        foreach(CMB_OPEJ as $cmbope) {
            $data .= sprintf("  <option value=\"%d\">%s</option>\n",$cmbope['RC'],$cmbope['CHR']);
        }
        break;
    default:
        $data = "";
        break;
    }
    return($data);
}

?>



           
