
# 使い方

ここではこのソフトウェアの使い方について説明します．
ビルド済みのファイルは，Releaseから入手可能です．

起動すると，コマンドプロンプトとGUIが表示されます．
基本的には，コマンドプロンプトから操作を行い，結果をGUIで確認します．
GUIを閉じてもコマンドプロンプトは残りますが，
コマンドプロンプトを閉じるとGUIを含めてプログラムが終了します．

<div align="center">
    <img src="./menu.png" width="70%" class="center">
</div>

起動が終了すると，上記のようなメニューが表示されます．
このメニューから，5つのモードを選択できます．
プロンプトに数字を打ち込んで，Enterを押すことでモードを選択します．
5つのモードそれぞれの使い方について以下に説明します．

## Simulation

グラフ探索を行い歩容生成を行うことができるモードです．
実行すると次のように表示されます．
左側がコマンドプロンプト，右側がGUIです．

<div align="center">
    <img src="./simulation_cui.jpg" width="45%" class="center">
    <img src="./simulation_gui.jpg" width="45%" class="center">
</div>

コマンドプロンプトには，以下のようなメッセージが表示されます．
yesと入力することでシミュレーションを開始します．

```bash
====================================================================================================

                                          Simulation mode

====================================================================================================

[Setting]

・Output to command line.
　　・Only those with a priority of kInfo or higher will be output.

・Step through the simulation.

・Does not step through each step.

・Display GUI.

----------------------------------------------------------------------------------------------------

Start simulation 1 times

Do you want to start the simulation? ( y / n )
>>
```

## Viewer

生成されたグラフを表示することができるモードです．
現在，このモードは不具合があるため使用しないでください．

## DisplayModel

ロボットのモデルを表示することができるモードです．

## ResultViewer

歩容生成の結果を表示することができるモードです．

## RobotControl

ロボットを操作することができるモードです．
Trossen Robotics社のロボット「PhantomX Mk-III」とUSBシリアル通信を行います．

## はじめて実行する場合

はじめて実行する場合は設定ファイルを生成する必要があります．

```bash
Loads a file. file_path : ./settings.toml
Do you want to output a default file? ( y / n )
>>
```

次のようなメッセージが表示されるので，`y`を入力してEnterを押してください．

<div align="center">
    <img src="./init_file.jpg" width="60%" class="center">
</div>
