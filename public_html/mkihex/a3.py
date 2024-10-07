#! /usr/bin/env python3
#coding: utf-8
#

# Version: 3.00
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
#  id sthr stmn edhr edmn inmn dumn rly(8characters)

import sys
import struct

#
# 比較演算子のデータ
#
relope2num = {
    'NULL':0,
    'EQ':1,
    'GT':2,
    'LT':3,
    'GE':4,
    'LE':5,
    'AND':6,
    'OR':7,
    'NE':8,
    'ADDFLG':0x80
    }
#
# 1 byte長データの変換
#
def byte_arrange(b):
    y = hex(b).replace('0x','').rjust(2,'0')
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
# リレー設定データの変換
#
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
#
#
##########################################################
if __name__ == '__main__':
    args = sys.argv
    argc = len(args)-1
    print(" argc={0}".format(argc))
    if argc == 9:
        flag = True
    elif argc == 12:  # COND0
        flag = True
    elif argc == 16:  # COND1
        flag = True
    elif argc == 20:  # COND2
        flag = True
    elif argc == 24:  # COND3
        flag = True
    elif argc == 28:  # COND4
        flag = True
    else:
        flag = False
    
    if flag == False:
        print("Usage:")
        print("arguments are as following")
        print(" id sthr stmn edhr edmn mnflag inmn dumn rly(8characters) [COND0 [CMP1 COND1 [CMP2 COND2 [CMP3 COND3 [CMP4 COND4]]]]]")
        print("----------------")
        print(" id:")
        print(" sthr stmn: hour,minute of start")
        print(" edhr edmn: hour,minute of end")
        print(" mnflag: current M fix")
        print(" inmn dumn: interval(minutes),during(minutes)")
        print(" rly: rly1..8 from leftside, 0,N=Dont care,T=BOTH/Toggle,B=Break,1,M=Make")
        print(" CONDn(n=0..4): compound condition format is as following")
        print("   -- CCMID OPE VAL")
        print("        CCMID: BLK_OPE CCM ID")
        print("        OPE: comparison operator (==:EQ),(!=:NE),(<:LT).(<=:LE),(>:GT),(>=:GE)")
        print("        VAL: Comparison numbers(floating number)")
        print(" CMPn(n=1..4): logical operators (&&:AND),(||:OR)")
        quit()  # 引数が合わなければここで終了

    id = int(args[1])
    sthr = int(args[2])
    stmn = int(args[3])
    edhr = int(args[4])
    edmn = int(args[5])
    mnflag = 0xff
    inmn = int(args[7])
    dumn = int(args[8])
    rly  = args[9]
    rly_l = 0
    rly_h = 0
    for x in range(4):
        rly_l |= rly_setting(rly,x)
    for x in range(4,8):
        rly_h |= rly_setting(rly,x)

    ccmid0 = 0xff
    cmpope0 = 0xff
    fval0 = float(0.0)
    cmp1 = 0xff
    ccmid1 = 0xff
    cmpope1 = 0xff
    fval1 = float(0.0)
    cmp2 = 0xff
    ccmid2 = 0xff
    cmpope2 = 0xff
    fval2 = float(0.0)
    cmp3 = 0xff
    ccmid3 = 0xff
    cmpope3 = 0xff
    fval3 = float(0.0)
    cmp4 = 0xff
    ccmid4 = 0xff
    cmpope4 = 0xff
    fval4 = float(0.0)
    
    if argc >= 12:
        ccmid0 = int(args[10])
        cmpope0 = relope2num(args[11])
        fval0 = float(args[12])
    if argc >= 16:
        cmp1 = relope2num(args[16])
        ccmid1 = int(args[17])
        cmpope1 = relope2num(args[18])
        fval1 = float(args[19])
    if argc >= 20:
        cmp2 = relope2num(args[20])
        ccmid2 = int(args[21])
        cmpope2 = relope2num(args[22])
        fval2 = float(args[23])
    if argc >= 24:
        cmp3 = relope2num(args[24])
        ccmid3 = int(args[25])
        cmpope3 = relope2num(args[26])
        fval3 = float(args[27])
    if argc >= 28:
        cmp4 = relope2num(args[28])
        ccmid4 = int(args[29])
        cmpope4 = relope2num(args[30])
        fval4 = float(args[31])

        
    baseaddr = 0x1000 # LC_SCH_START
    recstep  = 0x40   # LC_SCH_REC_SIZE

    ih_valid    = byte_arrange(1)
    ih_sthr     = byte_arrange(sthr)
    ih_stmn     = byte_arrange(stmn)
    ih_edhr     = byte_arrange(edhr)
    ih_edmn     = byte_arrange(edmn)
    ih_inmn     = byte_arrange(inmn)
    ih_dumn     = byte_arrange(dumn)
    ih_mnflag   = "FF"
    ih_rly_l    = byte_arrange(rly_l)
    ih_rly_h    = byte_arrange(rly_h)
    
    ihtxt       = ih_valid+ih_sthr+ih_stmn+ih_edhr+ih_edmn+ih_mnflag+ih_inmn+ih_dumn+'FFFFFFFFFF'+ih_rly_l+ih_rly_h
    for i in range(3):
        tp = i*32
        iht = ihtxt[tp:(tp+32)]
        sz  = hex(int(len(iht)/2)).replace('0x','').rjust(2,'0')
        addr  = hex(id*recstep+baseaddr+int(tp/2)).replace('0x','').rjust(4,'0')
        ih  =":"+sz+addr+"00"+iht+"FF"
        print(ih)
    quit()
