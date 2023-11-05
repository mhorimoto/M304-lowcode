#! /usr/bin/env php
<?php
//  This program is
//     sending target M304 with test URL
//     CLI interface
//

//$url = "http://192.168.38.50/:1001100020636F6C6C6973696F6E0A007907FEFC6F";
$url = "http://192.168.38.50/:1003000043004865617020616E6420737461636BA3";

$f = fopen($url,"r");

if ($f) {
    while (($buffer = fgets($f, 4096)) !== false) {
        echo $buffer;
    }
    if (!feof($f)) {
        echo "Error: unexpected fgets() fail\n";
    }
    fclose($f);
}
?>
