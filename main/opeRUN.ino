#include <M304.h>

// 分に変換する関数
int toMinutes(int hours, int minutes) {
    return hours * 60 + minutes;
}

// 時間内判定を行う関数
int isOn(int H1, int M1, int H2, int M2, int M0, int D0, int TH, int TM) {
    // 開始時刻と終了時刻を分に変換
    // M0: DUMN     作用時間
    // D0: INTERVAL 休憩時間

    int startTime = toMinutes(H1, M1);
    int endTime = toMinutes(H2, M2);
    int currentTime = toMinutes(TH, TM);
    
    // 開始時刻と終了時刻が逆転している場合の調整
    if (endTime < startTime) {
        endTime += 24 * 60; // 翌日までの時間を考慮
        if (currentTime < startTime) {
            currentTime += 24 * 60; // 現在時刻も調整
        }
    }
    
    // 現在の時刻が範囲外ならOFF
    if (currentTime < startTime || currentTime > endTime) {
        return 0; // OFF
    }
    
    // 範囲内ならサイクルに基づくON/OFF判定を行う
    int cycleTime = M0 + D0;   // サイクル全体の時間
    int elapsedTime = (currentTime - startTime) % cycleTime; // 開始時刻からの経過時間
    
    if (elapsedTime < M0) {
        return 1; // ON
    } else {
        return 0; // OFF
    }
}

void opeRUN(int hr, int mn) {
    static int pmn = 61; // Nothing 61minute
    int id, i, j, k, x, y, p,rt[5];
    int cmpresult, r;
    char t[81], buf[8];
    float cval, rval; // cval: 現在の値(flb_cmpope.fval)
                      // rval: 比較数値(flb_rx_ccm.cmpval)
    bool combinationCompare(byte,int);
    static int prt[5]={0,0,0,0,0};
    extern int rlyttl[];
    extern byte cmpope_result[];
    extern uecsM304Sched flb_rx_ccm[];
    extern uecsM304Send flb_tx_ccm[];
    extern uecsM304cmpope flb_cmpope[];

    for (id = 0; id < CCM_TBL_CNT_RX; id++) {
        if (flb_rx_ccm[id].valid != 0xff) {
            rt[id] = isOn(flb_rx_ccm[id].sthr,flb_rx_ccm[id].stmn,flb_rx_ccm[id].edhr,flb_rx_ccm[id].edmn,
                        flb_rx_ccm[id].dumn,flb_rx_ccm[id].inmn,hr,mn);
            if (rt[id]!=prt[id]) {
                sprintf(t,"ID=%02d ST=%02d:%02d ED=%02d:%02d CUR=%02d:%02d IN=%02d DU=%02d RT=%d",
                    id,flb_rx_ccm[id].sthr,flb_rx_ccm[id].stmn,flb_rx_ccm[id].edhr,flb_rx_ccm[id].edmn,
                    hr,mn,flb_rx_ccm[id].inmn,flb_rx_ccm[id].dumn,rt[id]);
                Serial.println(t);
                prt[id] = rt[id];
            }
            //timeDecision(id, hr, mn);
            //comparison_exp(id,rt);
        }
    }
    for (r = 1; r < CCM_TBL_CNT_TX; r++) {
        sendUECSpacket(r, itoa(rlyttl[r - 1], buf, DEC), 60);
    }
}

/* void comparison_exp(int id,int rt) {
    int i,x,y,r,j,k,cmpresult;
    byte s[2],cmbcmp;
    float cval, rval; // cval: 現在の値(flb_cmpope.fval)
                      // rval: 比較数値(flb_rx_ccm.cmpval)
    bool combinationCompare(byte,int);

    extern int rlyttl[];
    extern byte cmpope_result[];
    extern uecsM304Sched flb_rx_ccm[];
    extern uecsM304Send flb_tx_ccm[];
    extern uecsM304cmpope flb_cmpope[];

    cmpresult = 0;
    wdt_reset();
    if (flb_rx_ccm[id].valid != 0xff) {
        for (i = 0; i < 5; i++) {
            x = 0;                          // cmpresultを生成するために必要な中間結果
            y = flb_rx_ccm[id].cmpccmid[i]; // 比較するuecsM304cmpopeの位置を決定
            if ((i==0)&&(y==0xff)) {
                i = 100;                     // 最初のcmpccmidが無効ならばあとは評価しない。
            }
            if (y < 0xff) {                  // 比較するCCMTYPEがある場合
                rval = flb_rx_ccm[id].cmpval[i]; // value index;
                cmbcmp = flb_rx_ccm[id].cmbcmp[i];
                switch (flb_rx_ccm[id].cmpope[i]) {
                case R_EQ: // ==
                    if (flb_cmpope[y].fval == rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_GT: // >
                    if (flb_cmpope[y].fval > rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_LT: // >
                    if (flb_cmpope[y].fval < rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_GE: // >=
                    if (flb_cmpope[y].fval >= rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_LE: // <=
                    if (flb_cmpope[y].fval <= rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                default:
                    i = 5; // 比較演算子が無効の場合、離脱 force exit
                    break;
                }
                x &= rt;
            } else {
                x = rt; // y==0xff 比較するCCMTYPEがない場合
            }
            s[0] = flb_rx_ccm[id].rly_l;
            s[1] = flb_rx_ccm[id].rly_h;
            //Serial.print("set_rlyttl is ");
            //Serial.println(rt);
            for (r = 0; r < 4; r++) {
                j = (s[0] >> (r * 2)) & 0x3;
                k = (s[1] >> (r * 2)) & 0x3;
                set_rlyttl(x, r, j, 1, id);
                set_rlyttl(x, r, k, 5, id);
            }
        }
    }
}
 */
void set_rlyttl(int x, int id) {
    // x: 条件合致=1,不一致=0
    // id: flb_rx_ccm[id]のid

    extern uecsM304Sched flb_rx_ccm[];
    extern int rlyttl[];
    int rl[2],y,r,i;
    char t[81];
    r = (r == 1) ? 3 : 7 ;
    for (i=0;i<4;i++) { // リレーの動作を取得する
        rl[0] = (flb_rx_ccm[id].rly_l >> (i*2)) & 0x3;
        rl[1] = (flb_rx_ccm[id].rly_h >> (i*2)) & 0x3;
        if (x == 0) {   //  条件不一致
            for (y=0;y<2;y++) {
                r = (y==0) ? 3 : 7;
                switch (rl[y]) {
                case RLY_BOTH:
                case RLY_MAKE:
                    rlyttl[r - i] = 0; /* r==1:RLY1..4  r!=1:RLY5..8 */
                    break;
                case RLY_BREAK:
                    rlyttl[r - i] = 1;
                    break;
                }
            }
        } else { /* 条件に合う (MAKE,BREAK,BOTHの判定後処理を書く) */
            for (y=0;y<2;y++) {
                r = (y==0) ? 3 : 7;
                switch (rl[y]) {
                case RLY_BOTH:
                case RLY_MAKE:
                    rlyttl[r - i] = 1;
                    break;
                case RLY_BREAK:
                    rlyttl[r - i] = 0;
                    break;
                }
            }
        }
    }
    if (id==1) {
      sprintf(t,"x=%d,r=%d,rly%d=%d",x,r,r-i,rlyttl[r-i]);
      Serial.println(t);
    }
}

void timeDecision(int id, int curhr, int curmn) {
    byte d, sthr, stmn, edhr, edmn, inmn, dumn, s[2], x, y, p;
    int i, j, k, sttime, edtime, inmntm, dumntm, startmin, curtim, pmin, did;
    //      char t[81];
    extern int rlyttl[];

    sthr = (int)flb_rx_ccm[id].sthr;
    stmn = (int)flb_rx_ccm[id].stmn;
    if (sthr > 24)
        return ERROR;
    sttime = sthr * 60 + stmn;
    edhr = (int)flb_rx_ccm[id].edhr;
    edmn = (int)flb_rx_ccm[id].edmn;
    if (edhr > 24)
        return ERROR;
    edtime = edhr * 60 + edmn;
    inmntm = (int)flb_rx_ccm[id].inmn;
    dumntm = (int)flb_rx_ccm[id].dumn;

    if ((inmntm + dumntm) == 0)
        return; // If either is 0, the process is aborted and returns.
    curtim = curhr * 60 + curmn;

    for (startmin = sttime; startmin < edtime; startmin += (inmntm + dumntm)) {
        if (startmin == curtim) {
            s[0] = flb_rx_ccm[id].rly_l;
            s[1] = flb_rx_ccm[id].rly_h;
            for (i = 0; i < 4; i++) {
                j = (s[0] >> (i * 2)) & 0x3;
                k = (s[1] >> (i * 2)) & 0x3;
                if (x != 0) x = 1; // True is true
                if (j) {
                    rlyttl[3 - i] = dumntm * 60 * x; // RLY1..4
                }
                if (k) {
                    rlyttl[7 - i] = dumntm * 60 * x; // RLY5..8
                }
            }
        }
    }
}

/* bool combinationCompare(byte c,int x) {
  switch(c) {
  case R_OR:
    x |= 1;
    break;
  case R_AND:
    x &= 1;
    break;
  default:
    x = 0;
  }
  return(x);
}
*/