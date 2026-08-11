# PC-98キーボードチェック 98KeyChk

## 起動方法

- Normal Mode(通常のPC-98キーボード)

```
98KEYCHK.EXE
```

- WIN・APP Mode(WINキー・APPキーがあるPC-98キーボード)
```
98KEYCHK.EXE /W
```

## プログラムの終了
[CTRL]+[C]

## 仕様
- Normal Mode(通常のPC-98キーボード)<br>
左SHIFTキーと右SHIFTキーを同一のキーとして判定します(どちらも左SHIFTキーとして扱います)

- WIN・APP Mode(WINキー・APPキーがあるPC-98キーボード)<br>
「拡張キーコードの送出を有効」の設定を行い、以下のキーを個別に判定できるようになります<br>
左SHIFTキーと右SHIFTキーを判定します<br>
左WINキーと右WINキーを判定します<br>
APPキーを判定します<br>


純正では無いキーボードについては、多少結果が変わるかもしれません<br>
以下、一例として…

- クラシックPC救済委員会 USB KEYBOARD for PC-98x1 ＋ 108キーボード(JIS配列)<br>
(Normal Mode でも WIN・APP Mode でも、以下の状態となります)<br>
左SHIFTキーと右SHIFTキーを同一のキーとして判定します(どちらも左SHIFTキーとして扱います)<br>
WINキーを右WINキーとして判定します(108キーボードではWINキーは左側に1つしか付いていません)<br>
APPキーを判定します

## テスト済み環境
- Neko Project 21/W ＋ WIN・APPキー付きキーボード
- PC-9821 Ce ＋ クラシックPC救済委員会 USB KEYBOARD for PC-98x1 ＋ 108キーボード(JIS配列)
- PC-9821 Ce ＋ ノーマルキーボード
- PC-9821 Ce ＋ WIN・APPキー付きキーボード
- PC-9821 Xa10 ＋ ノーマルキーボード ※SimK様
- PC-9821 Xa9 ＋ WIN・APPキー付きキーボード ※Kazurin000様

## Gitリポジトリフォルダー構成

```
/98KeyChk
├─dist
├─docs
│  ├─archive
│  └─handoff
├─PC98DEV
├─references
├─src
└─tools
    └─scripts
```

