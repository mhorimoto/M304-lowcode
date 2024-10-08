#include <M304.h>

void opeRUN(int hr, int mn) {
    static int pmn = 61; // Nothing 61minute
    int id, i, j, k, x, y, p;
    byte s[2];
    int cmpresult, r;
    char t[81], buf[8];
    float cval, rval; // cval: 現在の値(flb_cmpope.fval)
    // rval: 比較数値(flb_rx_ccm.cmpval)
    extern int rlyttl[];
    extern byte cmpope_result[];
    extern uecsM304Sched flb_rx_ccm[];
    extern uecsM304Send flb_tx_ccm[];
    extern uecsM304cmpope flb_cmpope[];

    //  1min interval
    if (mn != pmn) {
        pmn = mn;
        for (id = 0; id < CCM_TBL_CNT_RX; id++) {
            if (flb_rx_ccm[id].valid != 0xff) {
                timeDecision(id, hr, mn);
            }
        }
        for (r = 1; r < CCM_TBL_CNT_TX; r++) {
            sendUECSpacket(r, itoa(rlyttl[r - 1], buf, DEC), 60);
        }
    }

    for (id = 0; id < CCM_TBL_CNT_RX; id++) {
        cmpresult = 0;
        wdt_reset();
        if (flb_rx_ccm[id].valid != 0xff) {
            for (i = 0; i < 5; i++) {
                x = 0;                          // cmpresultを生成するために必要な中間結果
                y = flb_rx_ccm[id].cmpccmid[i]; // 比較するuecsM304cmpopeの位置を決定
                if (y < 255) {                  // 比較するCCMTYPEがある場合
                    rval = flb_rx_ccm[id].cmpval[i]; // value index;
                    switch (flb_rx_ccm[id].cmpope[i]) {
                    case R_EQ: // ==
                        if (flb_cmpope[y].fval == rval) {
                            x = 1;
                        } else {
                            x = 0;
                        }
                        break;
                    case R_GT: // >
                        if (flb_cmpope[y].fval > rval) {
                            x = 1;
                        } else {
                            x = 0;
                        }
                        break;
                    case R_LT: // >
                        if (flb_cmpope[y].fval < rval) {
                            x = 1;
                        } else {
                            x = 0;
                        }
                        break;
                    case R_GE: // >=
                        if (flb_cmpope[y].fval >= rval) {
                            x = 1;
                        } else {
                            x = 0;
                        }
                        break;
                    case R_LE: // <=
                        if (flb_cmpope[y].fval <= rval) {
                            x = 1;
                        } else {
                            x = 0;
                        }
                        break;
                    default:
                        i = 5; // 比較演算子が無効の場合、離脱 force exit
                        break;
                    }
                } else {
                    x = 1; // y==0xff
                    i = 5; // 比較演算子が無効の場合、離脱 force exit
                }
                s[0] = flb_rx_ccm[id].rly_l;
                s[1] = flb_rx_ccm[id].rly_h;
                for (r = 0; r < 4; r++) {
                    j = (s[0] >> (r * 2)) & 0x3;
                    k = (s[1] >> (r * 2)) & 0x3;
                    set_rlyttl(x, r, j, 1, id);
                    set_rlyttl(x, r, k, 5, id);
                }
            }
        }
    }
}

void set_rlyttl(int x, int i, int j, int r, int id) {
    extern int rlyttl[];
    if (r == 1) {
        r = 3;
    } else {
        r = 7;
    }
    if (x == 0) {
        switch (j) {
        case RLY_BOTH:
            rlyttl[r - i] = 0; /* r==1:RLY1..4  r!=1:RLY5..8 */
            break;
        }
    } else { /* 条件に合う (MAKE,BREAK,BOTHの判定後処理を書く) */
        switch (j) {
        case RLY_BOTH:
            if (rlyttl[r - i] == 0) {
                rlyttl[r - i] = ((int)flb_rx_ccm[id].dumn) * 60; /* r==1:RLY1..4  r!=1:RLY5..8 */
            }
            break;
        case RLY_MAKE:
            rlyttl[r - i] = ((int)flb_rx_ccm[id].dumn) * 60; /* r==1:RLY1..4  r!=1:RLY5..8 */
            break;
        case RLY_BREAK:
            rlyttl[r - i] = 0;
            break;
        }
    }
}

void timeDecision(int id, int curhr, int curmn) {
    byte d, sthr, stmn, edhr, edmn, inmn, dumn, s[2], x, y, p;
    int i, j, k, sttime, edtime, inmntm, dumntm, startmin, curtim, pmin, did;
    //  char t[81];
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
