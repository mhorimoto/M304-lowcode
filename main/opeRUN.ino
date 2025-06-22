#include <M304.h>

// 分に変換する関数
int toMinutes(int hours, int minutes) {
    return hours * 60 + minutes;
}

// 時間内判定を行う関数
// 開始・終了・間隔・動作時間のすべてが合致したときは1を返す。
// 開始・終了時間が合致し、間隔・動作時間がともに0のときは2を返す。
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
    if (cycleTime==0) return 3;
    if (elapsedTime < M0) {
        return 1; // ON
    } else {
        return 0; // OFF
    }
}

// 時刻を秒に変換するヘルパー関数
long toSeconds(int h, int m, int s) {
  return (long)h * 3600 + (long)m * 60 + s;
}
// 秒単位で時間範囲内かつ周期と動作時間を判定する関数
// 開始・終了・周期・動作時間のすべてが合致したときは1を返す。
// 開始・終了時間が合致し、周期・動作時間がともに0のときは2を返す。
// 2のときは、複合条件がかなったときだけ動作するように後判定が工夫する。
// 開始・終了時間が合致し、周期が0のときは3を返す。
// 3のときには時刻条件が許す範囲で最大D0秒を有効とする。

int isOnSecond(int H1, int Mi1, int S1, int H2, int Mi2, int S2, long S0, long D0, int TH, int TMi, int TS) {

  // 開始時刻と終了時刻を秒に変換
  long startTime = toSeconds(H1, Mi1, S1);
  long endTime = toSeconds(H2, Mi2, S2);
  long currentTime = toSeconds(TH, TMi, TS);

  // 開始時刻と終了時刻が逆転している場合の調整
  if (endTime < startTime) {
    endTime += 24 * 3600; // 翌日までの時間を考慮
    if (currentTime < startTime) {
      currentTime += 24 * 3600; // 現在時刻も調整
    }
  }

  // 現在の時刻が範囲外ならOFF
  if (currentTime < startTime || currentTime > endTime) {
    return 0; // OFF
  }

  // 範囲内ならサイクルに基づくON/OFF判定を行う
  long cycleTime = S0 + D0;   // サイクル全体の時間 (秒)
  if (cycleTime == 0) {
    return 2; // 開始・終了時間が合致し、周期・動作時間がともに0
  }
  long elapsedTime = (currentTime - startTime) % cycleTime; // 開始時刻からの経過時間 (秒)

  if (S0==0) {
    return 3;
  }
  if (elapsedTime < S0) {
    return 1; // ON
  } else {
    return 0; // OFF
  }
}


void opeRUN(int hr, int mn, int sec) {
  int id, i, j, k, x, y[CCM_TBL_CNT_RX],rt[CCM_TBL_CNT_RX];
  int r;
  char t[81], buf[8];
  float rval; // cval: 現在の値(flb_cmpope.fval)
  // rval: 比較数値(flb_rx_ccm.cmpval)
  int combinationCompare(byte,bool,int);
  void comparison_exp(int);
  static int prt[CCM_TBL_CNT_RX]={0,0,0,0,0};
  extern int rlyttl[];
  extern byte cmpope_result[];
  extern uecsM304Sched flb_rx_ccm[];
  extern uecsM304Send flb_tx_ccm[];
  extern uecsM304cmpope flb_cmpope[];

  for (i = 0; i < 8; i++) {
    rlyttl[i] = 0;
  }
  for (id = 0; id < CCM_TBL_CNT_RX; id++) {
    rt[id] = 0;
    if (flb_rx_ccm[id].valid != 0xff) {
      //
      // TOPIC!!
      //
      // 開始・終了・間隔・動作時間のすべてが合致したときは1を返す。
      // 開始・終了時間が合致し、間隔・動作時間がともに0のときは3を返す。
      //            rt[id] = isOn(flb_rx_ccm[id].sthr,flb_rx_ccm[id].stmn,flb_rx_ccm[id].edhr,flb_rx_ccm[id].edmn,
      //                        flb_rx_ccm[id].dumn,flb_rx_ccm[id].inmn,hr,mn);
      rt[id] = isOnSecond(flb_rx_ccm[id].sthr,flb_rx_ccm[id].stmn,flb_rx_ccm[id].stsc,flb_rx_ccm[id].edhr,flb_rx_ccm[id].edmn,0,
                          flb_rx_ccm[id].inmn,flb_rx_ccm[id].dumn,hr,mn,sec);
      sprintf(t,"%d:%d=isOnSecond(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)\n",id,rt[id],
              flb_rx_ccm[id].sthr,flb_rx_ccm[id].stmn,flb_rx_ccm[id].stsc,flb_rx_ccm[id].edhr,flb_rx_ccm[id].edmn,0,
              flb_rx_ccm[id].inmn,flb_rx_ccm[id].dumn,hr,mn,sec);
      debugUdpOut(t);
      if (rt[id]!=prt[id]) {
        sprintf(t,"ID=%02d ST=%02d:%02d:%02d ED=%02d:%02d CUR=%02d:%02d IN=%02d DU=%02d RT=%d",
                id,flb_rx_ccm[id].sthr,flb_rx_ccm[id].stmn,flb_rx_ccm[id].stsc,flb_rx_ccm[id].edhr,flb_rx_ccm[id].edmn,
                hr,mn,flb_rx_ccm[id].inmn,flb_rx_ccm[id].dumn,rt[id]);
        debugUdpOut(t);
        prt[id] = rt[id];
      }
      comparison_exp(id);
      if (flb_rx_ccm[id].cmpccmid[0]!=0xff) { // 複合条件がある場合
        y[id] = rt[id] & 0x03; // 3の場合もあるので
        for (i=0;i<5;i++) {
          k = y[id];
          y[id] = combinationCompare(flb_rx_ccm[id].cmbcmp[i],flb_rx_ccm[id].match_result[i],k);
          sprintf(t,"ID=%02d-%d:%d=combinationCompare(%d,%d,%d)",
                  id,i,y[id],flb_rx_ccm[id].cmbcmp[i],flb_rx_ccm[id].match_result[i],k);
          debugUdpOut(t);
        }
      } else {  // 複合条件がない場合
        y[id] = rt[id];
      }
    }
    set_rlyttl(y[id],id);
  }
  for (r = 1; r < CCM_TBL_CNT_TX; r++) {
    sendUECSpacket(r, itoa(rlyttl[r - 1], buf, DEC), 60);
  }
}

void comparison_exp(int id) {
  // id = blk_bのindex
  int i,x,y,r,j,k,cmpresult;
  float rval; // rval: 比較数値(flb_rx_ccm.cmpval)

  extern uecsM304Sched flb_rx_ccm[];
  extern uecsM304cmpope flb_cmpope[];

  wdt_reset();
  if (flb_rx_ccm[id].valid != 0xff) {
    for (i = 0; i < 5; i++) {           // 環境条件5個をチェックする
      y = flb_rx_ccm[id].cmpccmid[i]; // 比較するuecsM304cmpopeの位置を決定
      if ((i==0)&&(y==0xff)) {
        i = 100;                     // 最初のcmpccmidが無効ならばあとは評価しない。
      }
      if (y < 0xff) {                  // 比較するCCMTYPEがある場合
        if (flb_cmpope[y].remain > 0) {
          rval = flb_rx_ccm[id].cmpval[i]; // value index;
          switch (flb_rx_ccm[id].cmpope[i]) {
          case R_EQ: // ==
            flb_rx_ccm[id].match_result[i] = (flb_cmpope[y].fval == rval) ? 1 : 0;
            break;
          case R_GT: // >
            flb_rx_ccm[id].match_result[i] = (flb_cmpope[y].fval > rval) ? 1 : 0;
            break;
          case R_LT: // >
            flb_rx_ccm[id].match_result[i] = (flb_cmpope[y].fval < rval) ? 1 : 0;
            break;
          case R_GE: // >=
            flb_rx_ccm[id].match_result[i] = (flb_cmpope[y].fval >= rval) ? 1 : 0;
            break;
          case R_LE: // <=
            flb_rx_ccm[id].match_result[i] = (flb_cmpope[y].fval <= rval) ? 1 : 0;
            break;
          default:
            i = 5; // 比較演算子が無効の場合、離脱 force exit
            break;
          }
        } else {
          // flb_cmpope[y].remainが0ならば、flb_rx_ccm[id].match_result[i]もFalse(0)にする。
          flb_rx_ccm[id].match_result[i] = 0;
        }
      } 
    }
  }
}

int combinationCompare(byte c,bool px,int x) {
  // c  = 条件間論理演算子 (R_OR/R_AND)
  // px = 判定結果 (TRUE/FALSE)
  // x  = 事前の判定結果
  switch(c) {
  case R_OR:
    //        x = (px) ? 1 : x;
    break;
  case R_AND:
    x = (px) ? (3&x) : 0;
    break;
  }
  return(x);
}

void set_rlyttl(int x, int id) {
    // x: 条件合致=1,不一致=0
    // id: flb_rx_ccm[id]のid
  extern char lbf[];
  extern uecsM304Sched flb_rx_ccm[];
  extern int rlyttl[],p_rlyttl[];
  int rl[2],y,r,i;
  char mcchr,t[81];
  lbf[0] = 0;
  if (flb_rx_ccm[id].valid==0xff) return;
  //  r = (r == 1) ? 3 : 7 ;
  for (i=0;i<4;i++) { // リレーの動作を取得する
    rl[0] = (flb_rx_ccm[id].rly_l >> (i*2)) & 0x3;
    rl[1] = (flb_rx_ccm[id].rly_h >> (i*2)) & 0x3;
    if (x == 0) {   //  条件不一致
      for (y=0;y<2;y++) {
        r = (y==0) ? 3 : 7;
        sprintf(lbf,"X:rl[%d]=%d",y,rl[y]);
        switch (rl[y]) {
        case RLY_BOTH:
        case RLY_MAKE:
          rlyttl[r - i] |= 0; /* r==1:RLY1..4  r!=1:RLY5..8 */
          break;
        case RLY_BREAK:
          //          rlyttl[r - i] |= 1;
          break;
        }
        sprintf(lbf,"%s,rlyttl[%d]=%d,",lbf,(r-i),rlyttl[r-i]);
      }
      mcchr = 'X';
    } else { /* 条件に合う (MAKE,BREAK,BOTHの判定後処理を書く) */
      for (y=0;y<2;y++) {
        r = (y==0) ? 3 : 7;
        sprintf(lbf,"O:rl[%d]=%d",y,rl[y]);
        switch (rl[y]) {
        case RLY_BOTH:
        case RLY_MAKE:
          rlyttl[r - i] = 1;
          break;
        case RLY_BREAK:
          rlyttl[r - i] = 0;
          break;
        }
        sprintf(lbf,"%s,rlyttl[%d]=%d,",lbf,(r-i),rlyttl[r-i]);
      }
      mcchr = 'O';
    }
    debugUdpOut(lbf);
    //    sprintf(lbf,"%d:%c:x=%d:rly1-8=%1d%1d%1d%1d%1d%1d%1d%1d",id,mcchr,x,
    //            rlyttl[0],rlyttl[1],rlyttl[2],rlyttl[3],rlyttl[4],rlyttl[5],rlyttl[6],rlyttl[7]);
    //    debugUdpOut(lbf);
  }
}


