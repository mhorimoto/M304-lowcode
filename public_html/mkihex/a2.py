#! /usr/bin/env python3
#coding: utf-8
#

# Version: 2.00
# Author: Masafumi Horimoto <mh@ys-lab.tech>
#
# Comments:
# The conditions for relay operation were changed to a four-value setting.
#    0,N : 0b00: Don't care (Compatibility with older versions)
#    T   : 0b01: Both (same as toggle)
#    B   : 0b10: Break
#    1,M : 0b11: Make (Compatibility with older versions)
#
# arguments are as following
#  id room region order priority lv cast sr ccm_type unit sthr stmn edhr edmn inmn dumn rly(8characters)

import sys
import struct

def byte_arrange(b):
    y = hex(b).replace('0x','').rjust(2,'0')
    return y

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

def rly_setting(r,x):
    if ((x>=0) and (x<4)):
        s = 6-(x*2)
    elif (x<8):
        s = 6-((x-4)*2)
    else:
        print("ERROR in rly_setting() out of range x")
        quit()
    if ((r[x]=="0") or (r[x]=="N")):
        return 0<<s
    elif (r[x]=="T"):
        return (0b01<<s)
    elif (r[x]=="B"):
        return (0b10<<s)
    elif ((r[x]=="1") or (r[x]=="M")):
        return (0b11<<s)
    else:
        print("ERROR in rly_setting()")
    quit()
    
##########################################################
if __name__ == '__main__':
    args = sys.argv
    argc = len(args)

    if (argc != 18):
        print("Usage:")
        print("arguments are as following")
        print(" id room region order priority lv cast sr ccm_type unit sthr stmn edhr edmn inmn dumn rly(8characters)")
        print("----------------")
        print(" id:")
        print(" room,region,order,priority: UECS number")
        print(" lv: A-1S-0=1,A-1S-1=2,A-10S-0=3,A-10S-1=4,A-1M-0=5,A-1M-1=6,B-0=7,B-1=8,S-1S-0=9,S-1M-0=10")
        print(" cast:")
        print(" sr: Sender=S, Receiver=R")
        print(" ccm_type:")
        print(" unit:")
        print(" sthr,stmn: hour/minute of start")
        print(" edhr,edmn: hour/minute of end")
        print(" inmn,dumn: interval(minutes),during(minutes)")
        print(" rly: rly1,2,3 from leftside, 0,N=Dont care,T=BOTH/Toggle,B=Break,1,M=Make")
        quit()
    id = int(args[1])
    room = int(args[2])
    region = int(args[3])
    order = int(args[4])
    priority = int(args[5])
    lv = int(args[6])
    cast = int(args[7])
    sr = args[8]
    ccmtype = args[9]
    unit = args[10]
    sthr = int(args[11])
    stmn = int(args[12])
    edhr = int(args[13])
    edmn = int(args[14])
    inmn = int(args[15])
    dumn = int(args[16])
    rly  = args[17]
    rly_l = 0
    rly_h = 0
    for x in range(4):
        rly_l |= rly_setting(rly,x)
    for x in range(4,8):
        rly_h |= rly_setting(rly,x)

######### OLD version
#    for x in range(4):
#        if (rly[x]=="1"):
#            rly_l |= 0b11 << 6-(x*2)
#    for x in range(4,8):
#        if (rly[x]=="1"):
#            rly_h |= 0b11 << 6-((x-4)*2)

    if sr=="S":
        sr = "53"
        baseaddr = 0x3000 # LC_SEND_START
        recstep  = 0x40   # LC_SEND_REC_SIZE
    else:
        sr = "52"
        baseaddr = 0x1000 # LC_SCH_START
        recstep  = 0x40   # LC_SCH_REC_SIZE
        

    ih_valid    = byte_arrange(1)
    ih_room     = byte_arrange(room)
    ih_region   = byte_arrange(region)
    order_o     = hex(order).replace('0x','').rjust(4,'0')
    ih_order    = order_o[2:4]+order_o[0:2]
    ih_priority = byte_arrange(priority)
    ih_lv       = byte_arrange(lv)
    ih_cast     = byte_arrange(cast)
    ih_sr       = sr
    ih_ccmtype  = string_arrange(ccmtype,20)
    ih_unit     = string_arrange(unit,10)
    ih_sthr     = byte_arrange(sthr)
    ih_stmn     = byte_arrange(stmn)
    ih_edhr     = byte_arrange(edhr)
    ih_edmn     = byte_arrange(edmn)
    ih_inmn     = byte_arrange(inmn)
    ih_dumn     = byte_arrange(dumn)
    ih_rly_l    = byte_arrange(rly_l)
    ih_rly_h    = byte_arrange(rly_h)
    
    ihtxt       = ih_valid+ih_room+ih_region+ih_order+ih_priority+ih_lv+ih_cast+ih_sr+ \
        ih_ccmtype+ih_unit+ih_sthr+ih_stmn+ih_edhr+ih_edmn+ih_inmn+ih_dumn+ \
        ih_rly_l+ih_rly_h
    for i in range(3):
        tp = i*32
        iht = ihtxt[tp:(tp+32)]
        sz  = hex(int(len(iht)/2)).replace('0x','').rjust(2,'0')
        addr  = hex(id*recstep+baseaddr+int(tp/2)).replace('0x','').rjust(4,'0')
        ih  =":"+sz+addr+"00"+iht+"FF"
        print(ih)
    quit()
