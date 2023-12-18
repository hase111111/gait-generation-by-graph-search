#include "graphic_system.h"

#include <Dxlib.h>

#include "cassert_define.h"
#include "dxlib_util.h"
#include "graphic_const.h"
#include "keyboard.h"


namespace designlab
{

GraphicSystem::GraphicSystem(const std::shared_ptr<const ApplicationSettingRecord> setting_ptr) :
	setting_ptr_(setting_ptr),
	fps_controller_{ setting_ptr != nullptr ? setting_ptr->window_fps : 60 }		// setting_ptr が null かどうかを調べ，そうでなければwindow_fpsの値を取り出す．
{
	assert(setting_ptr_ != nullptr);
}


void GraphicSystem::Main()
{
	//そもそも描画処理を使わないならば即終了．
	if (!setting_ptr_->do_gui_display)
	{
		return;
	}

	// Dxlibの関数は複数スレッドで呼ぶことを考慮されていないので，複数のスレッドから呼ぶと必ず問題が起きる．
	// そのため，初期化処理，描画，終了処理の全てをこの関数の中で呼ぶ必要がある．

	if (!MyDxlibInit())
	{
		return;
	}

	// ProcessMessage関数はウィンドウの×ボタンがおされると失敗の値を返す．
	// また，ウィンドウを維持するためには定期的に呼び出し続ける必要があるのでループで呼び続けている．
	// ProcessMessageは成功で0(C++におけるfalse)，失敗で-1(C++におけるtrueは0以外の値)を返す，そのため，失敗するまでループする場合は以下のように記述する
	while (ProcessMessage() >= 0)
	{
		// メインループ，falseが帰った場合，ループを抜ける．
		if (!Loop())
		{
			break;
		}
	}

	//終了処理を行う．
	MyDxlibFinalize();
}

void GraphicSystem::ChangeGraphicMain(std::unique_ptr<IGraphicMain>&& graphic_main_ptr)
{
	// mutexで排他制御を行う．GraphicMainを切り替える際に，Main関数を呼んでしまうとエラーが発生する可能性があるため，それを抑制する．
	boost::mutex::scoped_lock lock(mutex_);

	//もともと持っていたIGraphicMainクラスのインスタンスを破棄する．
	if (graphic_main_ptr_)
	{
		graphic_main_ptr_.reset();
	}

	graphic_main_ptr_ = std::move(graphic_main_ptr);
}


bool GraphicSystem::MyDxlibInit()
{
	// 1部の初期化用関数はDxlib_Initを呼ぶ前に実行する必要があるのでここで実行する．	

	SetOutApplicationLogValidFlag(FALSE);					// ログ出力無しに変更．これをしないとLog.txtという邪魔なファイルが出力される．
	SetMainWindowText(GraphicConst::kWindowName.c_str());	// タイトルを変更．ウィンドウの左上に表示されるもの．
	SetWindowSizeChangeEnableFlag(FALSE);					// ウィンドウサイズを自由に変更できないようにする．
	SetAlwaysRunFlag(TRUE);									// ウインドウがアクティブではない状態でも処理を続行するように変更する．
	SetWaitVSyncFlag(FALSE);								// 垂直同期信号を待たないように変更．これをしないとFPSが60固定になる．	
	ChangeWindowMode(TRUE);									// ウインドウモードに変更．これをしないとフルスクリーンで表示される．
	SetUseDirectInputFlag(TRUE);							// DirectInputを使用するように変更．これをしないとマウス入力でサイドボタンが受け付けられない．

	//ウィンドウの横幅，縦幅，カラーを設定する．
	SetGraphMode(setting_ptr_->window_size_x, setting_ptr_->window_size_y, GraphicConst::kColorBit);

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() < 0)
	{
		return false;
	}

	//描画先を裏画面にする．説明が難しいが，画面のちらつきを押えてくれる効果があり，Dxlibを使う以上必須の項目．
	SetDrawScreen(DX_SCREEN_BACK);

	// 背景色の設定
	SetBackgroundColor(GraphicConst::kBackColorRed, GraphicConst::kBackColorGreen, GraphicConst::kBackColorBlue);

	// 3D関連の初期化を行う．	
	if (setting_ptr_ != nullptr && setting_ptr_->gui_display_quality == enums::DisplayQuality::kHigh)
	{
		dxlib_util::InitDxlib3DSetting(true);
	}
	else
	{
		dxlib_util::InitDxlib3DSetting(false);
	}

	return true;
}


bool GraphicSystem::Loop()
{
	// [描画の処理について]
	// ScreenFlip関数とClearDrawScreen関数の詳細：ウィンドウの画像表示はパラパラ漫画の様に画面を素早く切り替えることでアニメーションを再現している．
	// しかし，単に画面を切り替えた場合，本物のパラパラ漫画の様にウィンドウにちらつきがでてしまう．
	// そこでGraphicSystemクラスのdxlibInit関数の中で呼ばれている SetDrawScreen(DX_SCREEN_BACK) によっていったん裏画面に絵を描画してから，
	// ScreenFlip関数でウィンドウに絵を戻すことで画面のちらつきをなくしている．
	// なお，ClearDrawScreen も ScreenFlip も ProcessMessageと返す値が同じなので，loop関数の様な書き方となる．


	// mutexで排他制御を行う．Main関数を呼ぶと同時に，GraphicMainを切り替えるとエラーが発生する可能性があるため，それを抑制する．
	boost::mutex::scoped_lock lock(mutex_);

	// GUI画面への標準出力をリセットする
	clsDx();

	// キー入力を更新する．
	Keyboard::GetIns()->Update();

	// 処理を行う．graphic_main_ptr_がfalseならば(nullであるならば)，処理を行わない．
	if (graphic_main_ptr_ != nullptr)
	{
		if (!graphic_main_ptr_->Update()) { return false; }
	}

	// 描画する
	if (!fps_controller_.SkipDrawScene())
	{
		// 裏画面に描画した絵を消す
		if (ClearDrawScreen() < 0) { return false; }

		// 描画処理を行う．graphic_main_ptr_がfalseならば(nullであるならば)，処理を行わない．
		if (graphic_main_ptr_ != nullptr)
		{
			graphic_main_ptr_->Draw();
		}

		//fps_controller_.DrawFps();

		// スクリーンに裏画面に描画した内容を移す
		if (ScreenFlip() < 0) { return false; }

	}

	// FPSを一定に保つために待つ．
	fps_controller_.Wait();

	return true;
}


void GraphicSystem::MyDxlibFinalize() const
{
	// DXライブラリの終了処理を呼ぶ.
	DxLib_End();

	//ほかにも処理があればここに追記する
}

}	// namespace designlab