#! /usr/bin/env python3
#coding: utf-8
#

# Version: 3.00
# Author: Masafumi Horimoto <mh@ys-lab.tech>
#
# Comments:
#
# arguments are as following
#  id VALID ROOM REGION ORDER ORDER PRIORITY LV CAST CCMTYPE UNIT
#
#define   LC_SEND_VALID     0x00 // Valid Flag (0x01:valid, 0xff:invalid)
#define   LC_SEND_ROOM      0x01
#define   LC_SEND_REGION    0x02
#define   LC_SEND_ORDER     0x03
#define   LC_SEND_PRIORITY  0x05
#define   LC_SEND_LV        0x06 // reference LV define
#define   LC_SEND_CAST      0x07
#define   LC_SEND_CCMTYPE   0x08 // char[20]
#define   LC_SEND_UNIT      0x1c // char[10]

import sys
import struct

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
    if argc==10:
        flag = True
    else:
        flag = False

    if flag == False:
        print("Usage:")
        print("arguments are as following")
        print(" id VALID ROOM REGION ORDER PRIORITY LV CAST CCMTYPE UNIT")
        print("----------------")
        print(" id:")
        print(" VALID: e)nable OR d)isable")
        print(" ROOM: room number (0..127)")
        print(" REGION: region number (0..127)")
        print(" ORDER:  Identify number (0..30000)")
        print(" PRIORITY: priority (0..30) 0 is higher, 29 is lower, 30 is no priority")
        print(" LV:       Level A-1S-0,A-1S-1,A-10S-0,A-10S-1,A-1M-0,A-1M-1,B-0,B-1,S-1S-0,S-1M-0")
        print("                 A1S0(1),A1S1(2),A10S0(3),A10S1(4),A1M0(5),A1M1(6),B0(7),B1(8),S1S0(9),S1M0(10)")
        print(" CAST:     Number of significant digits after the decimal point. 0 for integer.")
        print(" CCMTYPE:  CCM name less than 20characters")
        print(" UNIT:     unit less than 10characters")
        quit()  # 引数が合わなければここで終了

    id = int(args[1])
    valid = args[2]
    if valid=='e':
        ih_valid = byte_arrange(1)
    else:
        ih_valid = byte_arrange(255)
    room = int(args[3])
    region = int(args[4])
    order = int(args[5])
    priority = int(args[6])
    lv = args[7]
    cast = int(args[8])
    ccmtype = args[9]
    unit = args[10]

    ih_room     = byte_arrange(room)
    ih_region   = byte_arrange(region)
    ih_order    = wbyte_arrange(order)
    ih_priority = byte_arrange(priority)
    if ((lv=="A-1S-0") or (lv=="A1S0") or (lv=="1")):
        ih_lv   = byte_arrange(1)
    elif ((lv=="A-1S-1") or (lv=="A1S1") or (lv=="2")):
        ih_lv   = byte_arrange(2)
    elif ((lv=="A-10S-0") or (lv=="A10S0") or (lv=="3")):
        ih_lv   = byte_arrange(3)
    elif ((lv=="A-10S-1") or (lv=="A10S1") or (lv=="4")):
        ih_lv   = byte_arrange(4)
    elif ((lv=="A-1M-0") or (lv=="A1M0") or (lv=="5")):
        ih_lv   = byte_arrange(5)
    elif ((lv=="A-1M-1") or (lv=="A1M1") or (lv=="6")):
        ih_lv   = byte_arrange(6)
    elif ((lv=="B-0") or (lv=="B0") or (lv=="7")):
        ih_lv   = byte_arrange(7)
    elif ((lv=="B-1") or (lv=="B1") or (lv=="8")):
        ih_lv   = byte_arrange(8)
    elif ((lv=="S-1S-0") or (lv=="S1S0") or (lv=="9")):
        ih_lv   = byte_arrange(9)
    elif ((lv=="S-1M-1") or (lv=="S1M0") or (lv=="10")):
        ih_lv   = byte_arrange(10)
    else:
        ih_lv   = byte_arrange(255)
    ih_cast     = byte_arrange(cast)
    ih_ccmtype  = string_arrange(ccmtype,20)
    ih_unit     = string_arrange(unit,10)

    baseaddr = 0x3000 # LC_SEND_START
    recstep  = 0x30   # LC_SEND_REC_SIZE

#define   LC_SEND_VALID     0x00 // Valid Flag (0x01:valid, 0xff:invalid)
#define   LC_SEND_ROOM      0x01
#define   LC_SEND_REGION    0x02
#define   LC_SEND_ORDER     0x03
#define   LC_SEND_PRIORITY  0x05
#define   LC_SEND_LV        0x06 // reference LV define
#define   LC_SEND_CAST      0x07
#define   LC_SEND_CCMTYPE   0x08 // char[20]
#define   LC_SEND_UNIT      0x1c // char[10]
    
    ihtxt       = ih_valid+ih_room+ih_region+ih_order+ih_priority+ih_lv+ih_cast+ih_ccmtype+ih_unit

    for i in range(3):
        tp = i*32
        iht = ihtxt[tp:(tp+32)]
        sz  = hex(int(len(iht)/2)).replace('0x','').rjust(2,'0')
        addr  = hex(id*recstep+baseaddr+int(tp/2)).replace('0x','').rjust(4,'0')
        ih  =":"+sz+addr+"00"+iht+"FF"
        print(ih)
    quit()
