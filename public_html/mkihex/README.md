# intelHEX data maker

M304 ver 3.x.x に合わせて変更を実施  
PHPを使っていた部分をなるべくPythonに変更中。

* a3b.py BLK_Bの部分を変換するスクリプト
* a3d.py BLK_Dの部分を変換するスクリプト
* a3sender.py : a2senderをPythonで書き直した
* dump304.py : Pythonへ移植中



## dump304の出力例

```data
    info
    0               1               2               3
    0123456789abcdef0123456789abcdef0123456789abcdef0123456
    0x0000: 10 10 0C 00 00 0B 02 A2 73 0B 00 01 FF FF FF FF  ........s.......
    0x0010: FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF  ................
    0x0020: FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF  ................
    0x0030: FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF  ................
    0x0040: 59 53 2D 4C 61 62 00 00 00 00 00 00 00 00 00 00  YS-Lab..........
    0x0050: 4D 33 30 34 2D 6C 6F 77 63 6F 64 65 2D 30 31 00  M304-lowcode-01.
    0x0060: 00 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF  ................
```
