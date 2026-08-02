# Neko Project 21/W(PC-98 MS-DOS環境)で行った設定

エミュレータは「Neko Project 21/W ver.0.86 rev.103」を使用

「Neko Project 21/W」でHDDイメージファイルを作成<br>
「Emulate」→ 「New disk」→「Hard disk image…」で作成できる

先ほど作ったHDDイメージを「Neko Project 21/W」にマウントする<br>
※パーティション作られてないので当然見えない

FreeDOS(98)起動フロッピーディスクイメージ
でFreeDOSを起動する

HDDのフォーマットは`FORMAT`コマンドではなく`BTNPART`コマンドで行う

フォーマット後、
システムの転送は`SYS`コマンドで行う
```
SYS C:
```
とか？(Aがフロッピードライブ1、Bがフロッピードライブ2、Cが先ほどフォーマットしたHDD)

## LSIC86
LSIC86

LSI C-86 v3.30c 試食版
を丸っと展開した物を「LSI86」フォルダとして置いてある

### AUTOEXEC.BAT
AUTOEXEC.BATで

```
A:\LSIC86\BIN
```

のパスを通す、現在は

```
PATH=A:\;A:\FILMTN;A:\LSIC86\BIN
```

となっている

### CONFIG.SYS

CONFIG.SYSのFILESを20以上にする
```
FILES=20
```

### _LCC
```
A:\LSIC86\BIN\_LCC
```

のファイルの内容を変更

```
-XA:\LSIC86\BIN -LA:\LSIC86\LIB -IA:\LSIC86\INCLUDE -T -O
```

を

```
-XA:\LSIC86\BIN -LA:\LSIC86\LIB -IA:\LSIC86\INCLUDE -T -O -j
```

に変更(後ろに -j を付けた)

これで、エラーが日本語で表示される

