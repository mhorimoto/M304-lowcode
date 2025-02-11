#! /usr/bin/env python3
import requests
import subprocess
import os

base_url = "https://www.smart-agri.jp/~horimoto/m304/m304log.php"  # ダウンロードスクリプトのURL
params = {"ID": 31}
data = {
    "FACILITY":15,
    "TBL":"BOOT",
    "MSG1":"LUCKY STAR"
    }
try:
    response = requests.post(base_url,params=params,data=data)
    # check status-code
    if response.status_code == 200:
        print(f"ok")
    else:
        print(f"error")
except requests.RequestException as e:
    print(f"Request error: {e}")
