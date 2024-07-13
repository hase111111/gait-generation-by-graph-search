
# Gait Pattern Generation for Graph Search

Visual Studioで開発した，グラフ探索を用いた6脚ロボットの歩容生成を行うためのシミュレータです．
卒業研究のために作成したプログラムで，研究の題目は"グラフ探索を用いた多脚ロボットの歩容パターン生成における脚軌道生成失敗時の歩容パターンの再評価手法"です．

## 概要

Doxygenによるドキュメントは[こちら](https://hase111111.github.io/gait-generation-by-graph-search/)にあります．

## 開発環境

私が開発に使用した環境は以下の通りです．

- Windows 10
- Visual Studio 2022
- DxLib: 3.24b
- Boost: 1.82.0
- magic_enum: 0.9.6
- toml11: 4.0.3
- strconv: 1.8.10
- doctest: 2.4.11

C++はVisual Studio 2022のコンパイラを使用してコンパイルしています．
また，バージョンはC++20を使用しています．

## 依存するライブラリ

本プログラムは以下のライブラリに依存しています．

- [DxLib](https://dxlib.xsrv.jp/) DirectXを用いたゲームライブラリ
- [Boost](https://www.boost.org/) C++の拡張ライブラリ
- [magic_enum](https://github.com/Neargye/magic_enum) C++の列挙型を扱いやすくするためのライブラリ
- [toml11](https://github.com/ToruNiina/toml11) TOML形式のファイルを扱うためのライブラリ
- [strconv](https://github.com/javacommons/strconv) マルチバイト文字列とワイド文字列の間の変換を行うためのライブラリ
- [doctest](https://github.com/doctest/doctest) C++向けのテストフレームワーク

DxLibとBoost以外のライブラリは，ヘッダオンリーライブラリのため，本プロジェクト内にソースコードが含まれています．
よって，新たにインストールする必要はありません．
DxLibとBoostについては，[DxLib，Boostの導入方法](../docs/mark_down/how_to_install_library.md)にしたがってインストールしてください．

## 導入方法

導入のためには，以下の手順を実行してください．

1. レポジトリのクローン
   ライブラリのインストール
2. 以下のコマンドを実行して，必要なライブラリをインストールしてください．

## 使い方

実行にはVisual Studioが必要です．まずは[Visual Studioの公式サイト](https://visualstudio.microsoft.com/ja/)からインストールしてください．
Visual Studioを用いて，プログラムを実行してください．Visual StudioではCtrl+F5を押すことで，プログラムが実行されます．
実行にあたって，プロジェクトの構成が「Release」「x64」になっていることを確認してください．

## 実行・コンパイルすることができない場合


