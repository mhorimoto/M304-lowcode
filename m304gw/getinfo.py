#! /usr/bin/env python3
#coding: utf-8

import requests

def GetInfoByID(id):
    url = "https://www.smart-agri.jp/~horimoto/m304/getinfo.php"  # 送信先のURLを指定
    params = {"M304ID": id}  # 必要に応じてパラメータを設定

    try:
        response = requests.get(url, params=params, timeout=5)
        response.raise_for_status()  # HTTPエラーがあれば例外を発生
        print(response.text.strip())  # 応答を表示（前後の空白を削除）
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")

#
####
#

if __name__ == "__main__":
    GetInfoByID(36)
    
