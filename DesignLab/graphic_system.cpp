#include "graphic_system.h"

#include "DxLib.h"

#include "dxlib_util.h"
#include "graphic_const.h"
#include "keyboard.h"
#include "mouse.h"


void GraphicSystem::Init(std::unique_ptr<IGraphicMainBuilder>&& graphic_main_builder, std::shared_ptr<AbstractHexapodStateCalculator> calc,
	const GraphicDataBroker* const broker, const SApplicationSettingRecorder* const setting)
{
	if (graphic_main_builder && calc && broker != nullptr && setting != nullptr)
	{
		//GraphicMainを作成する．
		graphic_main_ptr_ = graphic_main_builder->build(broker, calc, setting);
	}


	//設定ファイルを読み込む．
	setting_ptr_ = setting;


	//Fpsを設定する．
	if (setting_ptr_ != nullptr)
	{
		fps_ptr_ = std::make_unique<FpsController>((*setting_ptr_).window_fps);
	}
}


void GraphicSystem::Main()
{
	//そもそも描画処理を使わないならば即終了
	if (!(*setting_ptr_).gui_display) { return; }

	//設定ファイルを読み込めていなければ終了
	if (setting_ptr_ == nullptr) { return; }

	//GraphicMainが作成されていなければ終了
	if (!graphic_main_ptr_) { return; }

	// FpsController が作成されていなければ終了
	if (!fps_ptr_) { return; }

	// Dxlibの関数は複数スレッドで呼ぶことを考慮されていないので，複数のスレッドから呼ぶと必ず問題が起きます．そのため，初期化処理，描画，終了処理の全てをこの関数の中で呼ぶ必要があります．
	if (!DxlibInit(setting_ptr_)) { return; }


	// ProcessMessage関数はウィンドウの×ボタンがおされると失敗の値を返す．また，ウィンドウを維持するためには定期的に呼び出し続ける必要があるのでループで呼び続けている．
	// ProcessMessageは成功で0(C++におけるfalse)，失敗で-1(C++におけるtrueは0以外の値)を返す，そのため !ProcessMessage はこの関数が成功の時のみループする...頭の痛い処理である．
	while (!ProcessMessage())
	{
		// メインループ，falseが帰った場合，ループを抜ける．
		if (!Loop())
		{
			break;
		}
	}

	//終了処理を行う．
	DxlibFinalize();
}


bool GraphicSystem::DxlibInit(const SApplicationSettingRecorder* const setting)
{
	// 1部の初期化用関数はDxlib_Initを呼ぶ前に実行する必要があるのでここで実行する．

	designlab::dxlib_util::InitDxlib3DSetting();		// 3D関連の初期化を行う．		

	SetOutApplicationLogValidFlag(FALSE);				// ログ出力無しに変更．これをしないとLog.txtという邪魔なファイルが出力される．
	SetMainWindowText(GraphicConst::WIN_NAME.c_str());	// タイトルを変更．ウィンドウの左上に表示されるもの．
	SetWindowSizeChangeEnableFlag(FALSE);               // ウィンドウサイズを自由に変更できないようにする．
	SetAlwaysRunFlag(TRUE);								// ウインドウがアクティブではない状態でも処理を続行するように変更する．
	ChangeWindowMode(TRUE);								// ウインドウモードに変更．これをしないとフルスクリーンで表示される．

	//ウィンドウの横幅，縦幅，カラーを設定する．
	SetGraphMode((*setting).window_size_x, (*setting).window_size_y, GraphicConst::COLOR_BIT);

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() < 0)
	{
		return false;
	}

	//描画先を裏画面にする．説明が難しいが，画面のちらつきを押えてくれる効果があり，Dxlibを使う以上必須の項目．
	SetDrawScreen(DX_SCREEN_BACK);

	// 背景色の設定
	SetBackgroundColor(GraphicConst::BACK_COLOR_R, GraphicConst::BACK_COLOR_G, GraphicConst::BACK_COLOR_B);

	return true;
}


bool GraphicSystem::Loop()
{
	// [描画の処理について]
	// ScreenFlip関数とClearDrawScreen関数の詳細：ウィンドウの画像表示はパラパラ漫画の様に画面を素早く切り替えることでアニメーションを再現している．
	// しかし，単に画面を切り替えた場合，本物のパラパラ漫画の様にウィンドウにちらつきがでてしまう．
	// そこでGraphicSystemクラスのdxlibInit関数の中で呼ばれている SetDrawScreen(DX_SCREEN_BACK) によっていったん裏画面に絵を描画してから，
	// ScreenFlip関数でウィンドウに絵を戻すことで画面のちらつきをなくしている．
	// ClearDrawScreen も ScreenFlip も ProcessMessageと返す値が同じなので，loop関数の様な書き方となる．


	//グラフィックメインクラスが空ならfalseを返す．
	if (!graphic_main_ptr_) { return false; }


	//標準出力を消す
	clsDx();

	//キー入力を更新する．
	Keyboard::GetIns()->Update();
	Mouse::GetIns()->Update();

	//処理を行う
	if (!graphic_main_ptr_->Update()) { return false; }


	//描画する
	if (!fps_ptr_->SkipDrawScene())
	{
		//裏画面に描画した絵を消す
		if (ClearDrawScreen() < 0) { return false; }

		graphic_main_ptr_->Draw();

		//スクリーンに裏画面に描画した内容を移す
		if (ScreenFlip() < 0) { return false; }

	}

	//FPSを一定に保つために待つ．
	fps_ptr_->Wait();

	return true;
}


void GraphicSystem::DxlibFinalize() const
{
	// DXライブラリの終了処理を呼ぶ.
	DxLib_End();
}
