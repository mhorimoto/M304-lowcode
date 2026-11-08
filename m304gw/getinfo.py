#! /usr/bin/env python3
#coding: utf-8

import sys
import os
import requests
import configparser

def load_config():
    config = configparser.ConfigParser()
    conf_paths = [
        os.environ.get("GETIHEX_CONF"),
        "getihex.conf",
        os.path.join(os.path.dirname(os.path.abspath(__file__)), "getihex.conf"),
        "/usr/local/etc/getihex.conf",
        "/etc/getihex.conf",
    ]
    for path in conf_paths:
        if path and os.path.isfile(path):
            config.read(path)
            if "COMMON" in config:
                return config, path
    return None, None

def GetInfoByID(m304_id):
    config, path = load_config()
    if not config:
        print("設定ファイル (getihex.conf) が見つかりません。")
        sys.exit(1)

    url = config.get("COMMON", "getinfoserver", fallback=None)
    if not url:
        base = config.get("COMMON", "server", fallback="")
        if "getihex.php" in base:
            url = base.replace("getihex.php", "getinfo.php")
        else:
            print("設定ファイルに 'getinfoserver' が定義されていません。")
            sys.exit(1)

    params = {"M304ID": m304_id}

    try:
        response = requests.get(url, params=params, timeout=5)
        response.raise_for_status()
        print(response.text.strip())
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    target_id = sys.argv[1] if len(sys.argv) > 1 else 36
    GetInfoByID(target_id)