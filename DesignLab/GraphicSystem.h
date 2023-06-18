//! @file GraphicSystem.h
//! @brief Dxlibの処理を行ってくれるGraphicSystemクラスを実装している．
//! @details Dxlib(デラックス ライブラリ)はウィンドウを表示して，所謂コマンドラインに文字を表示するだけの寂しいプログラムに彩りを与えてくれるやつです．<br>
//! 主にゲームプログラミングをする際に，ウィンドウを表示するためのライブラリとして使用されます．<br>
//! Dxlib以外にも OpenCVなどにもウィンドウを表示する機能がありますが，今回のプログラムではDxlibを用いて結果を表示します．<br>
//! Dxlibは WinAPI というWindowsのアプリケーションを作るための機能を，使いやすくしてくれるライブラリです．<br> 以下参考ページ <br>
//! https://dixq.net/rp2/ ←C++用の資料．少々難しいかも．<br> https://dixq.net/g/   ←C言語用の資料．あまり参考にならないかも<br>
//! https://dxlib.xsrv.jp/dxfunc.html ←公式の関数のリファレンス(関数の目次的なもの)．<br>
//! @author 長谷川

#pragma once
#include "GraphicDataBroker.h"

/**
 * @class GraphicSystem
 * @brief Dxlibの処理を行うクラス．
 * @details Dxlibを非同期処理で動かすことで別スレッドで行っているグラフ探索の処理をもとにロボットの状態を表示する．<br>
 * しかし，Dxlibは非同期処理を考慮して設計されていないので，動かし方によってはうまく動作しません．<br>
 * このプロジェクトではこの関数の中でのみDxlibの処理を動かすことで，エラーを防いでいますが，予期せぬエラーが発生する可能性はあります．<br> <br> [Dxlibの注意] <br>
 * 注意として，Dxlib系の関数は 真偽を大文字の TRUEとFALSEを使って表すので，従来のtrue falseを使用しないようにしましょう．<br>
 * まぁ，ぶっちゃけ小文字の方でも動くけど，バージョンの更新によって動かなくなる可能性があるのでDxlibに組み込まれているものを使うのが無難です．<br>
 * また，Dxlibはエラーが出たときに -1 を返す関数が非常に多いです．そのため例えば if(DxLib_Init() == false) と書いてもエラーを受け取れないことがあります．<br> 
 * 正しくは if(DxLib_Init() < 0) となります．これは bool型 がデフォルトで存在しないC言語でも使用することができるようにするための配慮であり，C++で書かれている本コードにおいては<br>
 * 混乱の元です(涙)．Dxlibのエラーはboolではなく，int型の負の値ということを覚えておいてください．<br>
 * @author 長谷川
 */
class GraphicSystem final
{
public:
	GraphicSystem() = default;
	~GraphicSystem() = default;

	/**
	 * GraphicSystemクラスの初期化をする．ブローカー(仲介人)クラスのポインタを受け取る.
	 * @param[in] _p_broker
	 */
	void init(const GraphicDataBroker* _p_broker);

	/**
	 * ウィンドウの表示を行ってくれる関数です．boost::threadにこの関数を渡して並列処理を行います．<br>initに失敗している，またはinitを呼ぶ前に実行した時は即座に終了します．<br>
	 * またメンバ関数のdxlibInit関数に失敗した場合も終了します．<br>dxlibは2窓できないので失敗する場合はタスクマネージャーからdxlibを落としてください．
	 */
	void main();

private:

	bool dxlibInit();			//Dxlibの初期化処理を行います．失敗した場合falseを返します．
	void dxlibFinalize() const;	//Dxlibの終了処理を行います．

	const GraphicDataBroker* mp_Broker;	// 画像表示を行うこのクラスと，データ処理を行う外部のクラスを繋ぐブローカー(仲介人)クラスのポインタを受け取るための変数．
};
