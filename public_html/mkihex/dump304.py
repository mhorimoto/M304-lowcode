#! /usr/bin/env python3
#coding: utf-8

# Version 3.00
# Author: Masafumi Horimoto <mh@ys-lab.tech>

import sys
import requests
import argparse

# バージョン取得用の関数 (PHPのgetVersion()を変換)
def get_version(host):
    url = f"http://{host}/version"
    try:
        response = requests.get(url)
        response.raise_for_status()  # HTTPエラーをチェック
        return response.text.strip()
    except requests.exceptions.RequestException as e:
        print(f"Error fetching version from {host}: {e}")
        return None

def display_info(fp, ver):
    hexdata = [0] * (8 * 16)  # 8行×16列のデータを格納するリスト

    for y in range(8):
        buffer = fp.readline().strip()  # 1行80文字を読み込む
        for x in range(16):
            z = int(buffer[(3 * x + 8):(3 * x + 11)], 16) & 0xff
            hexdata[y * 0x10 + x] = z

    print(f"     UECS-ID: {prt_LC_INFO(hexdata, 'uecsid')}")
    print(f"     VERSION: {ver}")
    print(f" MAC ADDRESS: {prt_LC_INFO(hexdata, 'mac')}")
    print(f"   IP ASSIGN: {prt_LC_INFO(hexdata, 'dhcpf')}")
    print(f"    FIXED IP: {prt_LC_INFO(hexdata, 'ipaddress')}")
    print(f"     NETMASK: {prt_LC_INFO(hexdata, 'netmask')}")
    print(f"     GATEWAY: {prt_LC_INFO(hexdata, 'defgw')}")
    print(f"         DNS: {prt_LC_INFO(hexdata, 'dns')}")
    print(f"      VENDER: {prt_LC_INFO(hexdata, 'vender')}")
    print(f"   NODE NAME: {prt_LC_INFO(hexdata, 'nodename')}")
    print(f"   DEBUG MSG: {prt_LC_INFO(hexdata, 'dbgmsg')}")

def prt_LC_INFO(dt, k):
    if k == "uecsid":
        odt = "{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}".format(dt[0], dt[1], dt[2], dt[3], dt[4], dt[5])
    elif k == "mac":
        odt = "{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}".format(dt[6], dt[7], dt[8], dt[9], dt[10], dt[11])
    elif k == "dhcpf":
        odt = "STATIC" if dt[0x0c] == 0x00 else "DHCP"
    elif k == "ipaddress":
        odt = _IPaddress(dt, 0x10)
    elif k == "netmask":
        odt = _IPaddress(dt, 0x14)
    elif k == "defgw":
        odt = _IPaddress(dt, 0x18)
    elif k == "dns":
        odt = _IPaddress(dt, 0x1c)
    elif k == "vender":
        odt = _LCText(dt, 0x40, 16)
    elif k == "nodename":
        odt = _LCText(dt, 0x50, 16)
    elif k == "dbgmsg":
        odt = _LCBitSW(dt, 0x60)
    else:
        odt = None
    return odt

# 仮の実装としてヘルパー関数を定義
def _IPaddress(dt, offset):
    return "{}.{}.{}.{}".format(dt[offset], dt[offset + 1], dt[offset + 2], dt[offset + 3])

def _LCText(dt, offset, length):
    return ''.join(chr(dt[offset + i]) for i in range(length)).strip()

def _LCBitSW(dt, offset):
    return "DEBUG" if dt[offset] != 0 else "NO DEBUG"


# データを取得し、表示する処理 (PHPのfopenなどの部分)
def process_file(host, addr, lcnt):
    for lc in range(lcnt):
        a = addr + (lc * 0x100)
        url = f"http://{host}/L00{a:04X}00"
        try:
            response = requests.get(url)
            response.raise_for_status()  # HTTPエラーをチェック
            print(response.text)
        except requests.exceptions.RequestException as e:
            print(f"Error fetching data from {url}: {e}")


def main():
    # コマンドライン引数の解析
    parser = argparse.ArgumentParser(description='dump304 script')
    parser.add_argument('host', help='Host to fetch data from')
    parser.add_argument('-d', help='Operation type (rx, sched, tx, ope, info)')
    parser.add_argument('-u', action='store_true', help='uasm option')
    parser.add_argument('-2', action='store_true', help='Version 2.x.x support')

    args = parser.parse_args()
    
    # オプションに基づく動作
    optd = args.d
    uasm = args.u
    verc = 2 if args.__dict__['2'] else 3

    # オプションに基づくアドレスとカウントの設定
    if optd == 'rx' or optd == 'sched':
        addr = 0x1000
        lcnt = 8
    elif optd == 'tx':
        addr = 0x3000
        lcnt = 3
    elif optd == 'ope':
        addr = 0x5000
        lcnt = 2
    elif optd == 'info':
        addr = 0x0000
        lcnt = 1
    else:
        print(f"Invalid operation: {optd}")
        return

    # バージョン取得
    ver = get_version(args.host)
    if ver:
        print(f"Version: {ver}")

    # データ取得処理
    process_file(args.host, addr, lcnt)

if __name__ == "__main__":
    main()
