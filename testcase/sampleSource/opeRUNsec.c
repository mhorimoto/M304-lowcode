#include <stdio.h>

// 時刻を秒に変換する関数
int toSeconds(int hours, int minutes, int seconds) {
    return hours * 3600 + minutes * 60 + seconds;
}

// ON/OFF判定を行う関数
int isOn(int H1, int M1, int S1, int H2, int M2, int S2, int M0_sec, int D0_sec, int TH, int TM, int TS) {
    // 開始時刻と終了時刻を秒に変換
    int startTime = toSeconds(H1, M1, S1);
    int endTime = toSeconds(H2, M2, S2);
    int currentTime = toSeconds(TH, TM, TS);
    
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
    int cycleTime = M0_sec + D0_sec;   // サイクル全体の時間（秒）
    int elapsedTime = (currentTime - startTime) % cycleTime; // 開始時刻からの経過時間
    
    if (elapsedTime < M0_sec) {
        return 1; // ON
    } else {
        return 0; // OFF
    }
}

int main() {
    // 例として、指定された条件
    int H1 = 8, M1 = 30, S1 = 0;  // 開始時刻 8:30:00
    int H2 = 18, M2 = 45, S2 = 0; // 終了時刻 18:45:00
    int M0_sec = 900;             // ONの時間 900秒 (15分)
    int D0_sec = 600;             // OFFの時間 600秒 (10分)
    int TH = 10, TM = 35, TS = 30; // 現在時刻 10:35:30
    
    // ON/OFF判定
    if (isOn(H1, M1, S1, H2, M2, S2, M0_sec, D0_sec, TH, TM, TS)) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
    
    return 0;
}

