Copyright (C) 2015, 2016 onpon4 <onpon4@riseup.net>

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.

(注意: これは元のREADME.txtに関する注意書きである)

========================================================================


Project: Starfighterをダウンロードしてくれてありがとう! このreadmeにはゲ
ームの簡単な説明とビルドの方法しか書かれていない。ゲームの遊び方の詳しい
説明は docs/index.html を参照して欲しい。

Project: Starfighterは、元々はParallel Realitiesが2002年に開発し、2003年
に発表した宇宙を舞台としたシューティングゲームである。あなたはクリス・バ
ンフィールドとなり、全宇宙を支配する軍需企業WEAPCOを終わらせるために戦う
ことになる。

もし、あなたがParallel Realitiesが配布するいずれかのバージョンのProject:
Starfighterで遊んだことがあれば、オリジナルとこのバージョンとの違いに気
づくだろう。

* グラフィック、効果音、音楽は全て異なる。これは、元のバージョンのものは
  正式なライセンスに基づいていなかったため、100%自由なものに置き換える必
  要があったためである。ほとんどは以前のSourceForgeでのプロジェクトの段
  階で行われた。このプロジェクトで新しい音楽が追加され、バージョン1.2の
  段階で手違いで組み込まれた自由でないグラフィックは置き換えられた。

* ゲーム内容が一部変更された。ほとんどの変更はゲームバランスを良くするた
  めのものである。しかし、もしオリジナルと同じ内容で遊びたいならば、可能
  な限り元に近い「旧版」の難易度も選択できる。

* 多くのセリフが変更された。これには多くの理由－誤字、新しい音楽に合わせ
  る、登場人物の個性を強めるなどのためである。

* 多くの場面でインターフェースが再構築された。例えば、矢印はオリジナルで
  は1種類であったが、これは艦ごとに異なるように変更された。

* タイトルで"humansdoitbetter"と入力してもチートモードにはならない。SDL2
  に切り替えたところ、これを修正するのは難しく、別の方法に置き換えること
  にした。チートメニューにアクセスする新しい方法は自分で見つけて欲しい :)



ソースからコンパイルする

Project: Starfighterは次のライブラリに依存している:

* SDL2 <http://libsdl.org>
* SDL2_image <http://www.libsdl.org/projects/SDL_image/>
* SDL2_mixer <http://www.libsdl.org/projects/SDL_mixer/>

これらをインストールすれば、単に:

    make

として、必要なら(rootで):

    make install

とする。
もしうまく行かないならば、makefileを修正する必要がある。

プレーするときは、単に starfighter のバイナリを実行すればよい。

========================================================================


日本語版のビルド方法

1. オリジナル(英語)のProject: Starfighter v1.6 のソースを展開する。
2. 日本語版のアーカイブ starfighter-1.6-jp-src.tgz を展開する。
3. 日本語版の gfx/smallFont.png をオリジナルの gfx/ にコピーし上書きす
   る。
4. 日本語版の starfighter-1.6-jp.patch をオリジナルのルートディレクトリ
   にコピーする。
5. オリジナルのルートディレクトリで

   patch -p1 < starfighter-1.6-jp.patch

   を実行する。

6. make を実行する。

win32版をビルドする場合は starfighter-1.6-win32.patch.gz を展開し、パッ
チを当てる必要がある。
このパッチは日本語版・オリジナルの両方で使える。
