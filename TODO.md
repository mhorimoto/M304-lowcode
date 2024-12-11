# TO DO LIST

## 11st Dec 2024
  構造変更の一環で現在recv16520.ino内部にcopyRXdata2flb_cmpope()を作っている。
  これは、受信した電文でcmpopeに合致したデータだけをコピーする関数で、
  この構造体のfvalとblk_bの比較対象の変数を比較する準備で。そのための準備関数。


## 3rd Dec 2024
  ver 3.0.0化に伴い、大幅に複合比較制御部分の変更がある。
  opeRUN.ino内の再構築を行う。
  それに伴い、githubのブランチを devel3 と main にする。
  
## 8th May 2024
  opeSCH.inoまでは修正済み。
  
## 2nd May 2024
  v3.x.xということで、環境条件比較のロジックを大改修する。  
  v2.6.0D1でmainは一旦固定。  
  WebによるEEPROMイメージ作成も一旦終了。
  
## 9th Feb. 2024
  Phase-3までできた。
  devel 2.5.0D として複合環境条件の取得と判定ロジックを追加中

## 13rd Dec. 2023
  2.3.6D
  opeHttpd.inoのF()マクロなどを使用する。53986/6355
  common.inoを作成したので、これに汎用ルーティンは移行していく。
  
## 12nd Dec. 2023
  2.3.5D
  ATMEMの内容をRAMにコピーするようにした。opeRUN.ino内のatmemによる読み込みはやめる。
  RAMの容量を開ける努力が必要。53352/6477

## 3rd Dec. 2023
 * CCMTYPEの比較の要があるRXのレコードのFAST_BUFFERにはCCMTYPEを入れておく。文字列比較なので速度性能上重要。
 * DHCPモードでルーターが遅れて起動した場合に備えて、setup時にIPアドレスが取得できるまで繰り返すなどの方法が必要。

##  test-eeprom-001
 2nd Dec. 2023
 ATMEM(EEPROM)から100レコード、3バイトを読み込むと200mSec要する。
 1バイトの場合には90mSecを要した。
 そこで、RAMに当該する3バイトを事前に読込コピーして比較すると、
 0mSec(ミリ秒)未満の所要時間で済んだ。よって、setup()の段階でコピーしてRAMで比較することにする。

## OUTLINE
  30th Nov. 2023
  Version:2.2.3D
  CCMテーブルのSend/Receiveの分けの変更を行う。
  M304.hはしばらくの間、手元に置いておく。


  last modified: 19th Sept. 2023
  version 1.3までは、M304-Kansuiで作成した。その後、汎用性を目指して大きく改版することになった。low-code用としてHTMLソースコードと連携する事になる。

## SCHEDULE:

### Phase-1:
なにも制御も計測もしない単純な制御用UECSループだけのプログラムを完成させる。
 1. M304-libを用いた、Ethernet,LCDなどの操作および表示の妥当性を確認。
 2. タイマー割り込みを用いた毎秒のカウントと10sec,1minのオペレーションの確認を行う。
 3. 問い合わせパケットの受信と応答の実装。


### Phase-2:
制御ロジックの実装を行う。
 1. 以前の学会発表の内容を参考にして制御テーブルを作成実装。
 2. タイマーによる制御ロジックを実装。
ここまでで動くUECS制御システムが出来た。

### Phase-3:
Phase-2までのプログラムをWebサーバからダウンロード出来る仕組みをPHPなどで作る。

## BUG:

* COMPLETED(1.00A 2023/06/01) 灌水間隔と灌水時間がともに0のときにopeRUN()のforループから抜け出せず、すなわちデッドループ化してしまう不具合


## REQUEST:
* SCHEDULE configで設定を書き込んだときに表示で分かるようにしたい。
* SCHEDULEコンフィグをグループ分けして、どのグループで運用するかを切り替えられるようにしたい。
* NET UDP backdoor
* NET UECSパケットの受信と判定
* COMPLETED(1.00C 2023/06/02) 開始時刻から終了時刻まで連続してMAKEさせ続ける方法が欲しい。
  灌水間隔を0とし、灌水時間を1以上の数値を書き込むことで実現する。
* COMPLETED(1.3.0 2023/06/13) UECS cndパケットなどを送信する
* COMPLETED(1.3.0 2023/06/13) ライブラリM304-libをArduinoライブラリ管理に反映させたい。

## Coding

* main.inoで定義されている res_xmlnode1[]などの名称を内容がわかる命名に変更したい。
  res_xmlnode1 = res_NODE_NAME みたいに
* NAMEとVENDERの出力を行う。

