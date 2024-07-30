このプログラムは2024年春学期開講 「電磁波情報学」の最終レポートです。
一部変更点や修正点が出た際には随時更新していきます。

作成者：岩田快道
最終更新日：7月30日


必須課題1->TouchSemafo.cpp
  ESP32に付属しているpin2つを利用して、触れたピンの番号をシリアル通信で出力する。

  表示例：touch pin ID=0 : touched!!

  pin の検出には、FreeRTOS の割り込み検出機能とピンのレベル検出機能を使わなければならない。
  Arduino,ESP32 のタッチセンサに関するファンクションをそのまま使うのは禁止。
  また、割り込み処理には"排他制御"を必要とする。

  詰まる点：割り込み処理の排他制御の書き方が慣れないと何をしている理解できない。

必須課題2->LEDSemafo.cpp
  ESP32で利用できるhttp サーバを用いた、LED遠隔制御プログラムの作成。
  クライアント上に3つのボタンを表示し、以下の条件を満たすように実行しなければならない。

  条件1：1番目のボタンには”ON/OFF”を表示する。ボタンを押すと、LEDの点滅の状態が
        ON → OFF → ON →・と交互に入れ替わる。

  条件2：2番目のボタンには”+”を表示する。1回ボタンを押す毎にLEDの点滅の間隔が100
        ミリ秒早くなる。最小100ミリ秒

  条件3：3番目のボタンには”-”を表示する。1回ボタンを押す毎にLEDの点滅の間隔が100
        ミリ秒遅くなる。最大2000ミリ秒

  詰まる点：LED点灯の速度アルゴリズム

自由課題3-B->HollSensingSemafo.cpp
  1. ESP32 と任意のセンサを利用して、各自のアイデアでシステムを作成する。
  2. 無線通信を必ず利用すること。
  3. FreeRTOS の機能を一つ以上使うこと
  4. センサは、各自で用意する。

  また、これに加え、レポート以下を記述する必要がある。
  1. システムの背景->なぜ、このシステムを思いついたのかを、第３者が読んでも分かるように記述する。
  2. システムの目的->提案するシステムが、このシステムが実現することで、誰がどのようなメリットがあるのかを記述する。
  3. システム設計図、回路図、プログラムコード->実装したシステムについて、設計図（ダイアグラム）、回路図、プログラムコードを
     載せ、それぞれについて、説明を行う。

   ESP32の内部のホールセンサーを利用して、磁気探知機を作成。
   -概要-
   磁石に反応する内部の内部のホールセンサーを利用して、
   磁石との距離を丸めて計算し、
   距離によってLEDを光らせるプログラム。

   磁石との距離はコア1内で丸めて計算し、コア0ではLEDを黄>青>赤の順で近いことを表示する。
   FreeRTOSの機能として、"マルチコア"を利用した。無線通信はhttp通信で行っている。

   詰まる点：FreeRTOSで何を使うかによって変わる。マルチコアは比較的簡単。

first commit -> 最終レポート用プログラムのコミット
