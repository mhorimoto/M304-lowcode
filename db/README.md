# Databaseについて

なるべく、MariaDBでもPostgreSQLでも使えることを前提とするが、
inet(IPアドレスやMACアドレスの扱いが苦手なMariaDBでは、余計な関数が必要になる)。

とりあえず、周りでユーザーが多いMariaDBをベースにする。

## 1. Tableの構造について

大きく4個のテーブルから構成される。

### 1-1. blk_A (ブロックA)

M304本体装置の個を特定するために必要な情報

#### 1-1-1. 装置識別のための情報

+ UECS ID
+ MAC Address
+ DHCPか否か  

> DHCPではない場合
> 
* IPアドレス
* NETMASK
* Default Gateway
* DNS

+ ベンダー名
+ ノード名
