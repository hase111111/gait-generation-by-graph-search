//! @file graphic_system.h
//! @brief Dxlibの処理を行うGraphicSystemクラス．
//! @details Dxlib(デラックス ライブラリ)はウィンドウを表示して，所謂コマンドラインに文字を表示するだけの寂しいプログラムに彩りを与えてくれるやつ．
//! @n 主にゲームプログラミングをする際に，ウィンドウを表示するためのライブラリとして使用される．
//! @n Dxlib以外にも OpenCVなどにもウィンドウを表示する機能があるが，今回のプログラムではDxlibを用いて結果を表示する．
//! @n Dxlibは WinAPI というWindowsのアプリケーションを作るための機能を，使いやすくしてくれるライブラリである．
//! @n  
//! @n 以下参考ページ 
//! @n
//! @n ・https://dixq.net/rp2/ ←C++用の資料．少々難しい 
//! @n ・https://dixq.net/g/   ←C言語用の資料．あまり参考にならないかも 
//! @n ・https://dxlib.xsrv.jp/dxfunc.html ←公式の関数のリファレンス(関数の目次)．

//! @class GraphicSystem
//! @brief Dxlibの処理を行うクラス．
//! @details Dxlibを非同期処理で動かすことで別スレッドで行っているグラフ探索の処理をもとにロボットの状態を表示する．
//! @n しかし，Dxlibは非同期処理を考慮して設計されていないので，動かし方によってはうまく動作しない．
//! @n このプロジェクトではこの関数の中でのみDxlibの処理を動かすことで，エラーを防いでいるが，予期せぬエラーが発生する可能性がある．
//! @n 
//! @n [Dxlibの注意] 
//! @n 注意として，Dxlib系の関数は 真偽を大文字の TRUEとFALSEを使って表すので，従来のtrue falseを使用しないようにすること．
//! @n (実は小文字の方でも動くけど，バージョンの更新によって動かなくなる可能性があるのでDxlibに組み込まれているものを使うのが無難．)
//! @n また，Dxlibはエラーが出たときに - 1 を返す関数が非常に多い．そのため例えば if (DxLib_Init() == false) と書いても
//! @n エラーを受け取れないことがある．
//! @n 正しくは if (DxLib_Init() < 0) となる．
//! @n これは bool型 がデフォルトで存在しないC言語でも使用することができるようにするための配慮であり，
//! @n C++で書かれている本コードにおいては混乱の元でしかない(涙)．
//! @n Dxlibのエラーはboolではなく，int型の負の値ということを覚えておくこと．
//! @n 
//! @n また，Dxlibは2窓できないので，実行に失敗する場合はタスクマネージャーからdxlibを落としてください．


#ifndef DESIGNLAB_GRAPHIC_SYSTEM_H
#define DESIGNLAB_GRAPHIC_SYSTEM_H

#include <memory>

#include "interface_graphic_main_builder.h"
#include "interface_graphic_main.h"
#include "graphic_data_broker.h"
#include "application_setting_recorder.h"
#include "abstract_hexapod_state_calculator.h"
#include "fps_controller.h"


class GraphicSystem final
{
public:


	//! @brief GraphicSystemクラスの初期化をする．ブローカー(仲介人)クラスのポインタ代入したGraphicMainクラスを受け取る.
	//! @param [in] graphic_main_builder GraphicMainクラスのポインタを代入するためのインターフェースクラスのユニークポインタ．
	//! @param [in] calc ロボットの状態を計算するクラスのシェアードポインタ．
	//! @param [in] broker GraphicMainクラスにデータを渡すための仲介人クラスのポインタ．
	//! @param [in] setting アプリケーションの設定を記録するクラスのポインタ．
	void Init(std::unique_ptr<IGraphicMainBuilder>&& graphic_main_builder, std::shared_ptr<AbstractHexapodStateCalculator> calc,
		const GraphicDataBroker* const broker, const SApplicationSettingRecorder* const setting);

	//! @brief ウィンドウの表示を行ってくれる関数です．boost::threadにこの関数を渡して並列処理を行います．
	//! @n initに失敗している，またはinitを呼ぶ前に実行した時は即座に終了します．
	//! @n またメンバ関数のdxlibInit関数に失敗した場合も終了します．
	void Main();

private:

	//! @brief Dxlibの初期化処理を行います．
	//! @param [in] setting アプリケーションの設定を記録するクラスのポインタ．
	//! @return bool 初期化に成功したかどうか．
	bool DxlibInit(const SApplicationSettingRecorder* const setting);

	//! @brief GraphicSystemクラスのwhileループの中で毎フレーム呼ばれる処理
	//! @return bool ループを続けるかどうか．falseならばループを抜ける．以上が起きた場合もfaseを返す．
	bool Loop();

	//! @brief Dxlibの終了処理を行います．
	void DxlibFinalize() const;


	std::unique_ptr<IGraphicMain> graphic_main_ptr_;		// グラフィックの表示を行うクラスのポインタ．

	const SApplicationSettingRecorder* setting_ptr_;				// 設定を保存するクラスのポインタ．

	std::unique_ptr<FpsController> fps_ptr_;					// FPSを一定に制御するクラス．詳しくはfps_controller.hへ
};


#endif	// DESIGNLAB_GRAPHIC_SYSTEM_H