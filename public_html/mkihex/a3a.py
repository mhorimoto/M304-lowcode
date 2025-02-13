#! /usr/bin/env python3
#coding: utf-8
#

# Version: 3.00
# Author: Masafumi Horimoto <mh@ys-lab.tech>
#
# Comments:
#
# arguments are as following
#  id macaddress dhcp|fixed ip_address netmask default_gateway dns vender_name node_name 

#define LC_UECS_ID        0x00
#define LC_MAC            0x06
#define FIX_DHCP_FLAG     0x0c
#define FIXED_IPADDRESS   0x10
#define FIXED_NETMASK     0x14
#define FIXED_DEFGW       0x18
#define FIXED_DNS         0x1c
#define VENDER_NAME       0x40
#define NODE_NAME         0x50
#define LC_DBGMSG         0x60  /* bit pos 0x80: Serial out, 0x40: LCD out, 0x20: Info Serial out */
#define   SO_MSG    0x80
#define   LCD_MSG   0x40
#define   SO_INFO   0x02
#define LC_CMODE          0x62  /* Force CMODE Change */

import sys
import struct
Version = "3.00"

#
# 1 byte長データの変換
#
def byte_arrange(b):
    y = hex(b).replace('0x','').rjust(2,'0')
    return y
#
# 2 byte長データの変換  リトルエンディアン
#
def wbyte_arrange(b):
    u = (b & 0xff00) >> 8
    l = (b & 0xff)
    y = hex(l).replace('0x','').rjust(2,'0')+hex(u).replace('0x','').rjust(2,'0')
    return y
#
# 浮動小数点データの変換
#
def float_arrange(b):
    rt = hex(struct.unpack('>I',struct.pack('<f',b))[0]).replace('0x','').rjust(8,'0')
    return rt
#
# 文字列データの変換
#
def string_arrange(s,l):
    rt = ""
    cnt= 0
    for c in s:
        if cnt>=l:
            return rt
        b = hex(ord(c)).replace('0x','').rjust(2,'0')
        rt += b
        cnt += 1
    while cnt < l:
        rt += "00"
        cnt += 1
    return rt
#
#
##########################################################
if __name__ == '__main__':
    args = sys.argv
    argc = len(args)-1
    if argc == 9:
        flag = True
    else:
        flag = False

    if flag == False:
        print("Version {0}".format(Version))
        print("Usage:")
        print("arguments are as following")
        print("  id mac_address [dhcp|fixed] ip_address netmask default_gateway dns vender_name node_name ")
        print("----------------")
        print(" id:")
        print(" mac_address:     xx:yy:zz:xx:yy:zz OR xxyyzzxxyyzz")
        print(" dhcp or fixed")
        print(" ip_address:      192.168.xx.xx")
        print(" netmask:         255.255.255.0")
        print(" default_gateway: 192.168.xx.yy")
        print(" dns:             192.168.xx.xx")
        print(" vender_name:     192.168.xx.xx")
        print(" node_name:       192.168.xx.xx")
        quit()  # 引数が合わなければここで終了
    id = int(args[1])
    ih_id = byte_arrange(id)
    print(ih_id)
    mac = args[2]
    macds = [mac[i:i+2] for i in range(0,len(mac),2)]
    for a in macds:
        print(a)

    quit()
