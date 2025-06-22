#!/usr/bin/env python3

import socket

UDP_IP = "0.0.0.0"      # すべてのインターフェースで受信
UDP_PORT = 16528        # 監視するUDPポート
BUFFER_SIZE = 4096      # 受信バッファサイズ

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on UDP port {UDP_PORT}...")

while True:
    data, addr = sock.recvfrom(BUFFER_SIZE)
    # データ部分だけを表示
    print(data.decode(errors='replace'))  # バイナリの場合は.decodeを外す

