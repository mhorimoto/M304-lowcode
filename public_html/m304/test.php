<?php

// 16進数のテキスト
$hexText = "01100C00000B"; // "Hello World"
//$hexText = "48"; // "Hello World"
//$hexText = "48656C6C6F20576F726C64"; // "Hello World"

// 16進数をバイナリに変換
$binaryData = hex2bin($hexText);
$fp = fopen("test.dat","w");
fwrite($fp,$binaryData);
fclose($fp);
// バイナリデータを出力
var_dump($binaryData);
//echo $binaryData;

?>
