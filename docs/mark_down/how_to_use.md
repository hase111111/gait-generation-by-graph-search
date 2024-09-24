
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
    <img src="./simulation_gui.jpg" width="42%" class="center">
</div>

<br>

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

### GUIの操作方法

シミュレーション中のGUIの操作方法は以下の通りです．

- マウスの左ドラック：カメラの回転（画面中央を中心に回転）
- マウスの右ドラック：カメラの注視点の平行移動
- マウスのホイール回転：カメラのズームイン・アウト
- マウスのホイールドラック：カメラの回転（カメラの注視点を中心に回転）
- マウスの右左ドラック：カメラの回転（ホイールドラックと同じだが，回転のブレがない）
- ウィンドウの上でマウスの左ドラック：ウィンドウの移動
- ウィンドウの上でマウスの左クリック：決定

### GUI上のウィンドウ

GUI上には以下のようなウィンドウが表示されます．

- Camera：カメラの位置を変更できます．このウィンドウで位置を大まかに変更し，マウスで細かく調整します．
- NodeSwitcher : シミュレーションの切り替えやアニメーションの再生を行います．
- NodeDisplayer : ノードの情報（ロボットの状態）を表示します．
- CameraParameterDisplayer : カメラのパラメータを表示します．

<br>

<div align="center">
    <img src="./window_gui.jpg" width="70%" class="center">
</div>

<br>

ウィンドウを画面の外に移動させた場合やウィンドウの×ボタンを押した場合は，
画面からウィンドウが消えます．
再度表示する場合は画面上の三角形ボタンをクリックしてください．
するとウィンドウターミナルが表示されます．
消えたウィンドウをクリックすると，画面に再表示されます．

<br>

<div align="center">
    <img src="./window_terminal.jpg" width="50%" class="center">
</div>

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
