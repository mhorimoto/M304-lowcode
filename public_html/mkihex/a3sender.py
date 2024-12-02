#! /usr/bin/env python3
#
#coding: utf-8

import sys
import requests

def process_file(url):
    try:
        response = requests.get(url)
        response.raise_for_status()  # HTTPエラーが発生した場合に例外を投げる
        for line in response.text.splitlines():
            if len(line) > 6 and line[6] == ':':
                print(line.rstrip(), end="  ")
                for x in range(8, 54, 3):
                    hex_value = line[x:x+3]
                    y = int(hex_value, 16)
                    if 0x20 <= y <= 0x7f:
                        print(chr(y), end="")
                    else:
                        print('.', end="")
                print()
            else:
                print(line)
    except requests.exceptions.RequestException as e:
        print(f"URL取得エラー: {e}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        process_file(sys.argv[1])
    else:
        print("URLを指定してください。")

