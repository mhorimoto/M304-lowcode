<?php
// XMLメッセージを作成する関数
function create_xml_message() {
    $xmlmsg = '<?xml version="1.0"?><UECS ver="1.00-E10"><NODESCAN/></UECS>';
    return $xmlmsg;
}

// ブロードキャストでXMLメッセージを送信する関数
function broadcast_xml_message($qmac) {
    $udp_ip = "255.255.255.255";  // ブロードキャストアドレス
    $udp_port = 16529;            // 送信するポート番号

    // ソケット作成
    $sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
    if (!$sock) {
        die("ソケットの作成に失敗しました: " . socket_strerror(socket_last_error()));
    }

    // ソケットのオプション設定（ブロードキャスト許可）
    if (!socket_set_option($sock, SOL_SOCKET, SO_BROADCAST, 1)) {
        die("ソケットオプションの設定に失敗しました: " . socket_strerror(socket_last_error($sock)));
    }

    // 送信するXMLメッセージを取得
    $message = create_xml_message();

    // ブロードキャストメッセージの送信
    if (!socket_sendto($sock, $message, strlen($message), 0, $udp_ip, $udp_port)) {
        echo "送信に失敗しました: " . socket_strerror(socket_last_error($sock)) . "\n";
    } else {
        echo "送信しました: " . $message . "\n";
    }

    // ソケットを閉じる
    socket_close($sock);
}

// 実行
$qmac = "";  // 必要に応じて指定
broadcast_xml_message($qmac);
?>

