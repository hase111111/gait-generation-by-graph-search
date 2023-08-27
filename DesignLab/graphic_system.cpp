#include "graphic_system.h"

#include "DxLib.h"

#include "graphic_const.h"
#include "graphic_loop.h"
#include "graphic_main_sample.h"
#include "graphic_main_basic.h"
#include "graphic_main_test.h"
#include "graphic_main_graph_viewer.h"
#include "designlab_dxlib.h"
#include "Define.h"


void GraphicSystem::init(std::unique_ptr<AbstractGraphicMain>&& graphic_main, const SApplicationSettingRecorder* const setting)
{
	mp_graphic_main = std::move(graphic_main);
	mp_setting = setting;
}


void GraphicSystem::main()
{
	//設定ファイルを読み込めていなければ終了
	if (mp_setting == nullptr) { return; }

	//そもそも描画処理を使わないならば即終了
	if (!(*mp_setting).gui_display) { return; }

	//ブローカーがnull(存在しない)なら終了
	if (!mp_graphic_main) { return; }

	// Dxlibの関数は複数スレッドで呼ぶことを考慮されていないので，複数のスレッドから呼ぶと必ず問題が起きます．そのため，初期化処理，描画，終了処理の全てをこの関数の中で呼ぶ必要があります．
	if (!dxlibInit(mp_setting)) { return; }


	//描画の処理を行うクラスをセットする．実行する描画の内容を変更したいならば，このようにIGraphicMainを継承した他のクラスを<>に入れてください．
	GraphicLoop looper(std::move(mp_graphic_main));


	// ProcessMessage関数はウィンドウの×ボタンがおされると失敗の値を返す．また，ウィンドウを維持するためには定期的に呼び出し続ける必要があるのでループで呼び続けている．
	// ProcessMessageは成功で0(C++におけるfalse)，失敗で-1(C++におけるtrueは0以外の値)を返す，そのため !ProcessMessage はこの関数が成功の時のみループする...頭の痛い処理である．
	while (!ProcessMessage())
	{
		//falseが帰った場合，ループを抜ける．
		if (!looper.loop())
		{
			break;
		}
	}

	//終了処理を行う．
	dxlibFinalize();
}


bool GraphicSystem::dxlibInit(const SApplicationSettingRecorder* const setting)
{
	// 1部の初期化用関数はDxlib_Initを呼ぶ前に実行する必要があるのでここで実行します．

	dl_dxlib::initDxlib3D();							//3D関連の初期化を行う．		

	SetOutApplicationLogValidFlag(FALSE);				//ログ出力無しに変更．これをしないとLog.txtという邪魔なファイルが出力されます．
	SetMainWindowText(GraphicConst::WIN_NAME.c_str());	//タイトルを変更．ウィンドウの左上に表示されるものです．
	SetWindowSizeChangeEnableFlag(FALSE);               //ウィンドウサイズを自由に変更できないようにする．
	SetAlwaysRunFlag(TRUE);								//ウインドウがアクティブではない状態でも処理を続行するように変更．
	ChangeWindowMode(TRUE);								//ウインドウモードに変更．これをしないとフルスクリーンで表示されます．

	//ウィンドウの横幅，縦幅，カラーを設定します．
	SetGraphMode((*setting).window_size_x, (*setting).window_size_y, GraphicConst::COLOR_BIT);

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() < 0)
	{
		return false;
	}

	//描画先を裏画面にする．説明が難しいのですが，画面のちらつきを押えてくれる効果があり，Dxlibを使う以上必須の項目です．
	SetDrawScreen(DX_SCREEN_BACK);

	// 背景色の設定
	SetBackgroundColor(GraphicConst::BACK_COLOR_R, GraphicConst::BACK_COLOR_G, GraphicConst::BACK_COLOR_B);

	return true;
}


void GraphicSystem::dxlibFinalize() const
{
	// DXライブラリの終了処理を呼ぶ.
	DxLib_End();
}
