flowchart TD
    %% ノードのスタイル定義
    classDef default fill:#fff,stroke:#333,stroke-width:2px;
    classDef highlight fill:#fff4dd,stroke:#d4a017,stroke-width:2px;
    classDef result fill:#e1f5fe,stroke:#01579b,stroke-width:2px;

    Start([開始: isOnSecond]) --> Input[/引数: 開始, 終了, 現在時刻, 動作M0, 休憩D0<br/>※すべて「秒」単位/]
    
    %% 時刻の正規化（1日は86400秒）
    Input --> Normalize[時刻データを日の総秒数に変換<br/>0 〜 86399]
    
    %% 日またぎ判定
    Normalize --> CheckCross{終了 < 開始 ?<br/>日またぎ設定}
    
    CheckCross -- YES --> AdjustNext[終了時刻に 86400秒 加算]
    AdjustNext --> CheckCur{現在 < 開始 ?}
    CheckCur -- YES --> AdjustCur[現在時刻に 86400秒 加算]
    CheckCur -- NO --> InRange
    AdjustCur --> InRange
    
    CheckCross -- NO --> InRange{範囲内か?<br/>開始 <= 現在 < 終了}:::highlight

    %% 範囲外判定
    InRange -- NO --> Ret0([戻り値 0: 範囲外OFF]):::result

    %% サイクル計算（間欠動作）
    InRange -- YES --> CalcCycle[サイクル = 動作M0 + 休憩D0]
    CalcCycle --> IsCycle0{サイクル == 0 ?}
    
    IsCycle0 -- YES --> Ret3([戻り値 3: 常時ON]):::result
    
    IsCycle0 -- NO --> CalcPos[サイクル内の経過時間算出<br/>pos = 現在 - 開始 % サイクル]
    
    CalcPos --> IsInWork{pos < 動作M0 ?}:::highlight
    
    IsInWork -- YES --> Ret1([戻り値 1: 動作時間内ON]):::result
    IsInWork -- NO --> Ret0_2([戻り値 0: 休憩時間中OFF]):::result