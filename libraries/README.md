# library フォルダ
## 説明
追加したライブラリのうち，ヘッダオンリーなものはここに追加しています．
ヘッダオンリーなライブラリとはインクルードするだけで使用することが可能なシンプルなライブラリです．

※編集日2023/12/7

>**参考**
> * [[C++]ヘッダーオンリー or ビルドライブラリ？](https://qiita.com/Kogia_sima/items/9a57e80087e1bc455f5a)

---

## 追加したもの一覧

### toml11
設定ファイルであるtomlファイルを読み込めるようにするためのライブラリ．
自身のソースコード内でこのライブラリをラッパするマクロや関数を作成しており，このライブラリに依存している箇所が多い．toml11という名前から分かるようにC++11用のライブラリだが，C++17でも使用することができる．C++20にはおそらく対応していないため，動作するかどうかは分からない．

>**参考**
> * [公式のgithubのページ](https://github.com/ToruNiina/toml11)


### strconv2.h
shift-jisをutf-8に変換するためのライブラリ．
Windows APIに依存しているため，Windows.hをインクルードできる環境でしか利用できない．
Tomlファイルはutf-8で出力する必要があるため，これを利用しているが余計な依存を増やさないために別のライブラリを導入したい．C++17で動作することができるが，C++20ではそのままでは動作しないらしいので編集が必要である．

>**参考**
> * [公式のgithubのページ](https://github.com/javacommons/strconv)
>
> * 作者さんのQiitaの記事．
>[マルチバイト文字列(std::string)とワイド文字列(std::wstring)の間の変換を行うライブラリを作りました(SJIS, UTF-8, UTF-16に対応。SJIS⇔UTF-8の変換も可能)](https://qiita.com/javacommons/items/9ea0c8fd43b61b01a8da)

### magic_enum
C++ではenumを文字列に変換することが難しい．(C#ではToString関数という便利な関数が用意されている．)
magic_enumではenum及びenum classの要素を文字列に変換する機能や，enumをfor範囲ループすることができるようにしてくれるライブラリ．
大変便利なので，自分のコードでは様々な場所で使用されている．

>**参考**
> * [[C++]enumを文字列に変換する汎用的な方法。](https://kenkyu-note.hatenablog.com/entry/2020/07/06/195821)
> * [公式のgithubのページ](https://github.com/Neargye/magic_enum)

### doctest
テストコードを書くためのライブラリ．
GoogleTestなどのC++用のテストコード用ライブラリは多くあるが，doctestはdoctest.hただ1つで動作させることができる非常にシンプルな点が素晴らしい．
死亡テストが書けないので，try-catchでなく，assertを使用している本プロジェクトにおいては若干使いづらさがあるのが玉に瑕．

>**参考**
> * [公式のgithubのページ](https://github.com/doctest/doctest)
> * [C++ doctest．Zenn](https://zenn.dev/nonanonno/scraps/3115057f950822)