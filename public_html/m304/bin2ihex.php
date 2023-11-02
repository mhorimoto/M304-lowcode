<?php
// Intel HEXフォーマットのファイルを書き込むための準備
function bin2ihex($binaryData) {
    $hexLines = array();
    $address = 0;
    // バイナリデータを16バイトずつ処理
    for ($i = 0; $i < strlen($binaryData); $i += 16) {
        // 16バイトのデータを取得
        $dataChunk = substr($binaryData, $i, 16);
        // データの長さ
        $dataLength = strlen($dataChunk) / 2;
        // ヘックスレコードを生成
        $hexRecord = sprintf(
            ":%02X%04X00%s",
            $dataLength,
            $address,
            strtoupper(bin2hex($dataChunk))
        );
        // チェックサムを計算
        $checksum = 0;
        for ($j = 0; $j < strlen($hexRecord); $j += 2) {
            $byte = hexdec(substr($hexRecord, $j, 2));
            $checksum += $byte;
        }
        $checksum = sprintf('%02X', (256 - ($checksum % 256)) % 256);
        // ヘックスレコードにチェックサムを追加して配列に格納
        $hexLines[] = $hexRecord . $checksum;
        // 次のアドレスを計算
        $address += $dataLength;
    }
    return($hexLines);
}

// Intel HEXフォーマットのファイルを出力

$inputdata= file_get_contents('templates_c/46adf76b21d06476a5c5c07585f11d485eed179a_0.file.index.tpl.php');
$hexLines = bin2ihex($inputdata);
$fileContent = implode("\n", $hexLines);
file_put_contents('output.hex', $fileContent);

echo "Intel HEXファイルが作成されました。\n";

?>
