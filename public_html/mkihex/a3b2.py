#! /usr/bin/env python3
#coding: utf-8
#

# Version: 3.31
# Author: Masafumi Horimoto <mh@ys-lab.tech>
# Comments:
#    停止間隔を周期に改めた。周期なので、周期ー動作時間＝停止間隔
#    として処理する。
#
# Comments:
# The conditions for relay operation were changed to a four-value setting.
#    0,N : 0b00: Don't care (Compatibility with older versions)
#    T   : 0b01: Both (same as toggle)
#    B   : 0b10: Break
#    1,M : 0b11: Make (Compatibility with older versions)
#
# arguments are as following
#  id valid sthr stmn edhr edmn inmn dumn rly(8characters)
#
# 
import sys
import struct
Version = "3.31"

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
    elif ((r[x]=="1") or (r[x]=="T")):
        return (0b01<<s)
    elif ((r[x]=="2") or (r[x]=="B")):
        return (0b10<<s)
    elif ((r[x]=="3") or (r[x]=="M")):
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
    if argc == 11:
        flag = True
    elif argc == 14:  # COND0
        flag = True
    elif argc == 18:  # COND1
        flag = True
    elif argc == 22:  # COND2
        flag = True
    elif argc == 26:  # COND3
        flag = True
    elif argc == 30:  # COND4
        flag = True
    else:
        flag = False

    if flag == False:
        print("# Version {0}".format(Version))
        print("# ERROR: argc={0} args={1}".format(argc,args))
        print("# Usage:")
        print("# arguments are as following")
        print("#  id valid sthr stmn stsc edhr edmn edsc inmn dumn rly(8characters) [COND0 [CMP1 COND1 [CMP2 COND2 [CMP3 COND3 [CMP4 COND4]]]]]")
        print("# ----------------")
        quit()  # 引数が合わなければここで終了

    id = int(args[1])
    valid = args[2]
    if valid=='e':
        ih_valid = byte_arrange(1)
    else: 
        ih_valid = byte_arrange(255)
    sthr = int(args[3])
    stmn = int(args[4])
    stsc = int(args[5])
    edhr = int(args[6])
    edmn = int(args[7])
    edsc = int(args[8])
    mnflag = 0x00
    inmn = int(args[9])
    dumn = int(args[10])
    if inmn>dumn:
        inmn = inmn - dumn   # 周期から停止時間を算出する
    rly  = args[11]
    rly_l = 0
    rly_h = 0
    for x in range(4):
        rly_l |= rly_setting(rly,x)
    for x in range(4,8):
        rly_h |= rly_setting(rly,x)

    ih_sthr     = byte_arrange(sthr)
    ih_stmn     = byte_arrange(stmn)
    ih_stsc     = byte_arrange(stsc)
    ih_edhr     = byte_arrange(edhr)
    ih_edmn     = byte_arrange(edmn)
    ih_edsc     = byte_arrange(edsc)
    ih_inmn     = wbyte_arrange(inmn)
    ih_dumn     = wbyte_arrange(dumn)
    ih_mnflag   = "00"
    ih_rly_l    = byte_arrange(rly_l)
    ih_rly_h    = byte_arrange(rly_h)

    ih_ccmid0 = "FF"
    ih_cmpope0 = "FF"
    ih_fval0 = "FFFFFFFF"
    ih_cmp1 = "FF"
    ih_ccmid1 = "FF"
    ih_cmpope1 = "FF"
    ih_fval1 = "FFFFFFFF"
    ih_cmp2 = "FF"
    ih_ccmid2 = "FF"
    ih_cmpope2 = "FF"
    ih_fval2 = "FFFFFFFF"
    ih_cmp3 = "FF"
    ih_ccmid3 = "FF"
    ih_cmpope3 = "FF"
    ih_fval3 = "FFFFFFFF"
    ih_cmp4 = "FF"
    ih_ccmid4 = "FF"
    ih_cmpope4 = "FF"
    ih_fval4 = "FFFFFFFF"
    
    if argc >= 12:
        ccmid0 = int(args[12])
        if (args[13].isdigit()):
            cmpope0 = int(args[13])
        else:
            cmpope0 = relope2num[args[13]]
        fval0 = float(args[14])
        #print("          CCMID0={0},CMPOPE0={1},FVAL0={2}".format(ccmid0,cmpope0,fval0))
        ih_ccmid0 = byte_arrange(ccmid0)
        ih_cmpope0 = byte_arrange(cmpope0)
        ih_fval0 = float_arrange(fval0)
        
    if argc >= 15:
        if (args[15].isdigit()):
            cmp1 = int(args[15])
        else:
            cmp1 = relope2num[args[15]]
        ccmid1 = int(args[16])
        if (args[17].isdigit()):
            cmpope1 = int(args[17])
        else:
            cmpope1 = relope2num[args[17]]
        fval1 = float(args[18])
        #print("CMBCMP1={3},CCMID1={0},CMPOPE1={1},FVAL1={2}".format(ccmid1,cmpope1,fval1,cmp1))
        ih_cmp1 = byte_arrange(cmp1)
        ih_ccmid1 = byte_arrange(ccmid1)
        ih_cmpope1 = byte_arrange(cmpope1)
        ih_fval1 = float_arrange(fval1)
    if argc >= 22:
        if (args[19].isdigit()):
            cmp2 = int(args[19])
        else:
            cmp2 = relope2num[args[19]]
        ccmid2 = int(args[20])
        if (args[21].isdigit()):
            cmpope2 = int(args[21])
        else:
            cmpope2 = relope2num[args[21]]
        fval2 = float(args[22])
        #print("CMBCMP2={3},CCMID2={0},CMPOPE2={1},FVAL2={2}".format(ccmid2,cmpope2,fval2,cmp2))
        ih_cmp2 = byte_arrange(cmp2)
        ih_ccmid2 = byte_arrange(ccmid2)
        ih_cmpope2 = byte_arrange(cmpope2)
        ih_fval2 = float_arrange(fval2)
    if argc >= 26:
        if (args[23].isdigit()):
            cmp3 = int(args[23])
        else:
            cmp3 = relope2num[args[23]]
        ccmid3 = int(args[24])
        if (args[25].isdigit()):
            cmpope3 = int(args[25])
        else:
            cmpope3 = relope2num[args[25]]
        fval3 = float(args[26])
        #print("CMBCMP3={3},CCMID3={0},CMPOPE3={1},FVAL3={2}".format(ccmid3,cmpope3,fval3,cmp3))
        ih_cmp3 = byte_arrange(cmp3)
        ih_ccmid3 = byte_arrange(ccmid3)
        ih_cmpope3 = byte_arrange(cmpope3)
        ih_fval3 = float_arrange(fval3)
    if argc >= 30:
        if (args[27].isdigit()):
            cmp4 = int(args[27])
        else:
            cmp4 = relope2num[args[27]]
        ccmid4 = int(args[28])
        cmpope4 = relope2num[args[29]]
        fval4 = float(args[30])
        #print("CMBCMP4={3},CCMID4={0},CMPOPE4={1},FVAL4={2}".format(ccmid4,cmpope4,fval4,cmp4))
        ih_cmp4 = byte_arrange(cmp4)
        ih_ccmid4 = byte_arrange(ccmid4)
        ih_cmpope4 = byte_arrange(cmpope4)
        ih_fval4 = float_arrange(fval4)

        
    baseaddr = 0x1000 # LC_SCH_START
    recstep  = 0x40   # LC_SCH_REC_SIZE

    
    ihtxt       = ih_valid+ih_sthr+ih_stmn+ih_edhr+ih_edmn+ih_mnflag+ih_inmn+ih_dumn+ih_stsc+ih_edsc+'FFFF'+ih_rly_l+ih_rly_h\
                    +ih_ccmid0+ih_cmpope0+ih_fval0+ih_cmp1+ih_ccmid1+ih_cmpope1+ih_fval1\
                    +ih_cmp2+ih_ccmid2+ih_cmpope2+ih_fval2+ih_cmp3+ih_ccmid3+ih_cmpope3+ih_fval3\
                    +ih_cmp4+ih_ccmid4+ih_cmpope4+ih_fval4
    for i in range(4):
        tp = i*32
        iht = ihtxt[tp:(tp+32)]
        sz  = hex(int(len(iht)/2)).replace('0x','').rjust(2,'0')
        addr  = hex(id*recstep+baseaddr+int(tp/2)).replace('0x','').rjust(4,'0')
        ih  =":"+sz+addr+"00"+iht+"FF"
        print(ih)
    quit()
