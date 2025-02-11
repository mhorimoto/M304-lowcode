#! /usr/bin/env python3
#coding: utf-8

# getihex.py
# Version 1.0  11st Feb. 2025
#
# ファームウェアのダウンロードと実行を行うスクリプト
# UECSGW上で実行されるプログラムで、指定されたURLからファームウェアをダウンロードし、実行する
# ダウンロードしたファームウェアは、同じディレクトリに保存され、実行後に削除される
# ダウンロードしたファームウェアの実行結果は、標準出力に表示される
# ダウンロードしたファームウェアの実行結果は、HTTP POSTリクエストでサーバに送信され、ログとして残る
# この機能を実行するためには、a2senderが必要である。

# ライブラリのインポート
import sys
import os
import time
import requests
import subprocess
import configparser

dummyLoopCount = 0
servername = ""
logserver = ""
m304ids = []

def execute_shell_script(script_path):
    try:
        # 実行権限を付与
        os.chmod(script_path, 0o755)

        # シェルスクリプトを実行
        result = subprocess.run(["bash", script_path], check=True, text=True, capture_output=True)
        
#        print("シェルスクリプトの実行結果:")
#        print(result.stdout)
    except subprocess.CalledProcessError as e:
        logging(m304id,3,"SHELL SCRIPT ERROR",str(e),0)
        logging(m304id,3,"SHELL SCRIPT ERROR STDOUT",e.stdout,0)
        logging(m304id,3,"SHELL SCRIPT ERROR STDERR",e.stderr,0)
    else:
        filesize = os.path.getsize(script_path)
        logging(m304id,7,"PARAM EXECUTE DONE",script_path,filesize)


def logging(m304id,faci,msg0,msg1,msg2):
    global logserver
    base_url = "https://www.smart-agri.jp/~horimoto/m304/m304log.php"  # logスクリプトのURL
    params = {"ID": m304id}
    data = {
        "FACILITY":faci,
        "TBL":"WEB",
        "MSG0":msg0,
        "MSG1":msg1,
        "MSG2":msg2
    }
    try:
        response = requests.post(logserver,params=params,data=data)
        # check status-code
        if response.status_code == 200:
            pass
            # print(f"ok")
        else:
            print(f"error")
    except requests.RequestException as e:
        print(f"Request error: {e}")


def main(m304id):
    global servername,logserver,m304ids
    # ダウンロードするファイルのURL
    base_url = "https://www.smart-agri.jp/~horimoto/m304/getihex.php"  # ダウンロードスクリプトのURL
    params = {"ID":m304id}
    try:
        response = requests.get(servername,params=params)
        # check status-code
        if response.status_code == 200:
            if response.text.strip() == "0":
                global dummyLoopCount
                dummyLoopCount += 1
                if dummyLoopCount >= 360:
                    logging(m304id,9,"NOFILE","",0)
                    dummyLoopCount = 0
                return
                
            # save file
            # Content-Dispositionヘッダーからファイル名を取得
            content_disp = response.headers.get("Content-Disposition", "")
            file_name = "downloaded_file.dat"  # デフォルトのファイル名
        
            if "filename=" in content_disp:
                file_name = content_disp.split("filename=")[-1].strip().strip('"')
            save_path = f"/tmp/{file_name}"
            #sys.stdout.write(f"Downloading {save_path} ...")
            #sys.stdout.write("\n")
            #sys.stdout.flush()

            # ファイルの保存
            with open(save_path,"wb") as file:
                    file.write(response.content)
        else:
            print(f"error: {response.status_code}")
            logging(m304id,3,"HTTP ERROR",response.status_code,0)            
    except requests.RequestException as e:
        print(f"Request error: {e}")
        logging(m304id,3,"REQUEST ERROR",str(e),0)

    filesize = os.path.getsize(save_path)

    # シェルスクリプトの実行
    if filesize>0:
        logging(m304id,7,"PARAM DOWNLOADED",save_path,filesize)
        logging(m304id,7,"PARAM EXECUTE",save_path,filesize)
        execute_shell_script(save_path)
    if os.path.isfile(save_path):
        os.remove(save_path)


if __name__ == "__main__":

    config = configparser.ConfigParser()
    try:
        config.read("/usr/local/etc/getihex.conf")
    except FileNotFoundError:
        print("設定ファイル (getihex.conf) が見つかりません。")
        sys.exit(1)

    for section in config.sections():
        if section == "COMMON":
            servername = config[section]["server"]
            logserver = config[section]["logserver"]
        else:
            for option in config.options(section):
                m304ids.append(config[section][option])
    print(f"server: {servername}")
    print(f"logserver: {logserver}")
    if len(sys.argv) == 2:
        m304ids[0] = sys.argv[1]
    for m304id in m304ids:
        logging(m304id,1,"GETIHEX START",str(m304id),"")
    
    try:
        while(1):
            for m304id in m304ids:
                try:
                    main(m304id)
                except Exception as e:
                    logging(m304id,3,"MAIN ERROR",str(e),0)    
            time.sleep(10)
    except KeyboardInterrupt:
        for m304id in m304ids:
            logging(m304id,1,"GETIHEX END","Ctrl-C","")
    sys.exit(0)
