<?php
ob_start();

require_once('m304.inc');

// GETパラメータからM304IDを取得
if (!isset($_GET['ID'])) {
    // GETパラメータからファイル名を取得
    if (!isset($_GET['F']) || empty($_GET['F'])) {
        die('No filename defined.');
    } else {
        // リクエストされたファイル名
        $fileName = basename($_GET['F']); // セキュリティ対策のため、basenameでディレクトリトラバーサルを防止
    }
} else {
    // M304IDで見つける場合
    try {
        $pdo = new PDO($dsn, $username, $password);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    } catch (PDOException $e) {
        echo 'Connection failed: ' . $e->getMessage();
    }

    $m304id = @$_GET['ID'];
    $query = sprintf("select hex(MACADDR) as MACADDR from blk_a where m304id=%d",$m304id);
    try {
        $reply = $pdo->query($query);
        $msg = "";
    } catch(PDOException $e) {
        $msg = $e->getMessage();
    }
    $today = getdate();
    $macaddr = $reply->fetch(PDO::FETCH_ASSOC);
    $fileName = $macaddr['MACADDR'].".sh";
}
$filePath = $ihexdir . $fileName;

// ファイルの存在を確認
if (!file_exists($filePath)) {
    //    header('Content-Description: File Transfer');
    //    header('Content-Type: ' . $mimeType);
    //    header('Content-Disposition: attachment; filename="filenotfound"');
    //    header('Content-Length: 0');
    //    header('Cache-Control: must-revalidate');
    //    header('Pragma: public');
    echo 0;
    exit;
}

// ファイルのMIMEタイプを取得（finfoを使用）
$finfo = finfo_open(FILEINFO_MIME_TYPE);
$mimeType = finfo_file($finfo, $filePath);
finfo_close($finfo);

// ファイルのサイズを取得
$fileSize = filesize($filePath);

// 出力バッファクリア
if (ob_get_level()) {
    ob_end_clean();
}


// ダウンロード用のHTTPヘッダーを設定
header('Content-Description: File Transfer');
header('Content-Type: ' . $mimeType);
header('Content-Disposition: attachment; filename="' . $fileName . '"');
header('Content-Length: ' . $fileSize);
header('Cache-Control: must-revalidate');
header('Pragma: public');

ob_end_flush();

// ファイルの内容を出力
readfile($filePath);
unlink($filePath);
exit;
?>
