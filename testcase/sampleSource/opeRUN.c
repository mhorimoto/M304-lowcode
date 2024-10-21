#include <stdio.h>

// 分に変換する関数
int toMinutes(int hours, int minutes) {
    return hours * 60 + minutes;
}

// ON/OFF判定を行う関数
int isOn(int H1, int M1, int H2, int M2, int M0, int D0, int TH, int TM) {
    // 開始時刻と終了時刻を分に変換
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

int main() {
    // 例として、指定された条件
    int H1 = 8, M1 = 30;  // 開始時刻 8:30
/*    int H2 = 18, M2 = 45; // 終了時刻 18:45 */
    int H2 = 8, M2 = 45; // 終了時刻 8:45
    int M0 = 2;          // ONの時間 2分
    int D0 = 0;          // OFFの時間 0分
    int TH = 8, TM = 29; // 現在時刻 10:35
    for (TM=0;TM<60;TM++) {
	printf("START=%02d:%02d  END=%02d:%02d  ON=%02d  OFF=%02d  CUR=%02d:%02d  ",
            H1,M1,H2,M2,M0,D0,TH,TM);
        // ON/OFF判定
        if (isOn(H1, M1, H2, M2, M0, D0, TH, TM)) {
            printf("ON\n");
        } else {
            printf("OFF\n");
        }
    }
    return 0;
}

