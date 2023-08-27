#pragma once

#include <memory>

#include "abstract_graphic_main.h"
#include "application_setting_recorder.h"


class GraphicSystem final
{
public:
	GraphicSystem() = default;
	~GraphicSystem() = default;


	//! @brief GraphicSystemクラスの初期化をする．ブローカー(仲介人)クラスのポインタ代入したGraphicMainクラスを受け取る.
	//! @param [in] graphic_main GraphicSystemクラスのメンバ関数を呼び出すためのブローカークラスのポインタ．
	void init(std::unique_ptr<AbstractGraphicMain>&& graphic_main, const SApplicationSettingRecorder* const setting);


	//! @brief ウィンドウの表示を行ってくれる関数です．boost::threadにこの関数を渡して並列処理を行います．<br>initに失敗している，またはinitを呼ぶ前に実行した時は即座に終了します．<br>
	//! またメンバ関数のdxlibInit関数に失敗した場合も終了します．<br>dxlibは2窓できないので失敗する場合はタスクマネージャーからdxlibを落としてください．
	void main();

private:

	bool dxlibInit(const SApplicationSettingRecorder* const setting);			//Dxlibの初期化処理を行います．失敗した場合falseを返します．
	void dxlibFinalize() const;	//Dxlibの終了処理を行います．

	std::unique_ptr<AbstractGraphicMain> mp_graphic_main;	// グラフィックの表示を行うクラスのポインタ．
	const SApplicationSettingRecorder* mp_setting = nullptr;		// 設定を保存するクラスのポインタ．
};


//! @class GraphicSystem
//! @date 2023/08/08
//! @author 長谷川
//! @brief Dxlibの処理を行うクラス．
//! @details Dxlibを非同期処理で動かすことで別スレッドで行っているグラフ探索の処理をもとにロボットの状態を表示する．
//! @n しかし，Dxlibは非同期処理を考慮して設計されていないので，動かし方によってはうまく動作しない．
//! @n このプロジェクトではこの関数の中でのみDxlibの処理を動かすことで，エラーを防いでいるが，予期せぬエラーが発生する可能性がある．@n @n [Dxlibの注意] 
//! @n 注意として，Dxlib系の関数は 真偽を大文字の TRUEとFALSEを使って表すので，従来のtrue falseを使用しないようにすること．
//! @n (実は小文字の方でも動くけど，バージョンの更新によって動かなくなる可能性があるのでDxlibに組み込まれているものを使うのが無難．)
//! @n また，Dxlibはエラーが出たときに - 1 を返す関数が非常に多い．そのため例えば if (DxLib_Init() == false) と書いてもエラーを受け取れないことがある．
//! @n 正しくは if (DxLib_Init() < 0) となる．これは bool型 がデフォルトで存在しないC言語でも使用することができるようにするための配慮であり，C++で書かれている本コードにおいては
//! @n 混乱の元でしかない(涙)．Dxlibのエラーはboolではなく，int型の負の値ということを覚えておくこと．


//! @file graphic_system.h
//! @date 2023/08/08
//! @author 長谷川
//! @brief Dxlibの処理を行うGraphicSystemクラス．
//! @n 行数 : @lineinfo
//! @details Dxlib(デラックス ライブラリ)はウィンドウを表示して，所謂コマンドラインに文字を表示するだけの寂しいプログラムに彩りを与えてくれるやつ．
//! @n 主にゲームプログラミングをする際に，ウィンドウを表示するためのライブラリとして使用される．
//! @n Dxlib以外にも OpenCVなどにもウィンドウを表示する機能があるが，今回のプログラムではDxlibを用いて結果を表示する．
//! @n Dxlibは WinAPI というWindowsのアプリケーションを作るための機能を，使いやすくしてくれるライブラリである．@n  @n 以下参考ページ @n
//! @n ・https://dixq.net/rp2/ ←C++用の資料．少々難しい @n ・https://dixq.net/g/   ←C言語用の資料．あまり参考にならないかも 
//! @n ・https://dxlib.xsrv.jp/dxfunc.html ←公式の関数のリファレンス(関数の目次)．
