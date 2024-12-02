#! /usr/bin/env python3
#coding: utf-8

import socket
import xml.etree.ElementTree as ET

# 送信するXMLデータの作成
def create_xml_message():
    xmlmsg = '<?xml version="1.0"?><UECS ver="1.00-E10"><NODESCAN/></UECS>'
    return xmlmsg.encode('utf-8')

# ブロードキャストでXMLを送信する関数
def broadcast_xml_message(qmac):
    udp_ip = "192.168.121.255"  # ブロードキャストアドレス
    udp_ip = "255.255.255.255"  # ブロードキャストアドレス
    udp_port = 16529  # 送信するポート番号

    # ソケットの作成
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    # XMLメッセージの作成と送信
    message = create_xml_message()
    sock.sendto(message, (udp_ip, udp_port))
    # print(f"Sent broadcast message: {message.decode('utf-8')}")

    # 応答受信
    sock.settimeout(10)  # 応答待機時間
    while(1):
        try:
            data, addr = sock.recvfrom(1024)  # 応答を受信
            # print(f"Received response from {addr}: {data.decode('utf-8')}")
            parse_xml_response(data,qmac)
        except socket.timeout:
            print("No response received.")
            quit()
# 応答XMLをパースして内容を表示する関数
def parse_xml_response(response_data,qmac):
    try:
        root = ET.fromstring(response_data)
        for c1 in root:
            sp = c1.tag
            txt= c1.text
            # print("SPd=:{0}:{1}".format(sp,txt))
            ip  = ""
            mac = ""
            for c2 in c1:
                if (c2.tag=="IP"):
                    ip = c2.text
                if (c2.tag=="MAC"):
                    mac = c2.text
                if ((mac==qmac) and (ip!="")):
                    print("{0}".format(ip))
                    quit()
    except ET.ParseError:
        print("Failed to parse XML response.")

# 実行
if __name__ == "__main__":
#    import argparse

    # ArgumentParserを設定
#    parser = argparse.ArgumentParser(description="MACアドレスからIPアドレスを取り出すコマンド")
#    parser.add_argument("argv1", help="MAC address")

    # 引数の解析
#    args = parser.parse_args()

    # 引数に基づいて処理を分岐
#    if args.argv1 != "":
        #formatted_hex = ":".join(args.argv1[i:i+2] for i in range(0, len(args.argv1), 2))
#        broadcast_xml_message(args.argv1)
	broadcast_xml_message("02A2730B0001")

