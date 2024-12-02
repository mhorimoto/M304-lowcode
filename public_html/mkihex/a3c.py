#! /usr/bin/env python3
#coding: utf-8
#
# Generate Intel hex data for Block C
#
# Version: 0.00
# Author: Masafumi Horimoto <mh@ys-lab.tech>
#
# Comments:
#
# arguments are as following
#  id VALID ROOM REGION ORDER ORDER LIFECNT CCMTYPE NAME
#
#define LC_CMPOPE_START     0x5000 // Compare Operators
#define LC_CMPOPE_REC_SIZE  0x20 //
#define   LC_COPE_VALID     0x00
#define   LC_COPE_ROOM      0x01
#define   LC_COPE_REGION    0x02
#define   LC_COPE_ORDER     0x03
#define   LC_COPE_LIFECNT   0x05
#define   LC_COPE_CCMTYPE   0x06 // char[20]
#define   LC_COPE_NAME      0x1a // char[10]
#define   LC_COPE_FVAL      0x1b
#define   LC_COPE_RESULT    0x1f
#define LC_END      0x7fff
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
    if argc==7:
        flag = True
    else:
        print("Usage:")
        print("arguments are as following")
        print(" id VALID ROOM REGION ORDER LIFECNT CCMTYPE")
        print("----------------")
        print(" id:")
        print(" VALID: e)nable OR d)isable")
        print(" ROOM: room number (0..127)")
        print(" REGION: region number (0..127)")
        print(" ORDER:  Identify number (0..30000)")
        print(" LIFECNT: 3,30,180sec")
        print(" CCMTYPE:  CCM name less than 20characters")
        quit()  # 引数が合わなければここで終了

    id = int(args[1])
    valid = args[2]
    if valid=='e':
        ih_valid = byte_arrange(1)
    else:
        ih_valid = byte_arrange(255)
    room    = int(args[3])
    region  = int(args[4])
    order   = int(args[5])
    lifecnt = int(args[6])
    ccmtype = args[7]

    ih_room     = byte_arrange(room)
    ih_region   = byte_arrange(region)
    ih_order    = wbyte_arrange(order)
    ih_lifecnt  = byte_arrange(lifecnt)
    ih_ccmtype  = string_arrange(ccmtype,20)

    baseaddr = 0x5000 # LC_SEND_START
    recstep  = 0x20   # LC_SEND_REC_SIZE

    ihtxt    = ih_valid+ih_room+ih_region+ih_order+ih_lifecnt+ih_ccmtype

    for i in range(2):
        tp = i*32
        iht = ihtxt[tp:(tp+32)]
        sz  = hex(int(len(iht)/2)).replace('0x','').rjust(2,'0')
        addr  = hex(id*recstep+baseaddr+int(tp/2)).replace('0x','').rjust(4,'0')
        ih  =":"+sz+addr+"00"+iht+"FF"
        print(ih)
    quit()
