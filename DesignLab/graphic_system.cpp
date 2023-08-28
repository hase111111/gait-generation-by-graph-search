#include "graphic_system.h"

#include "DxLib.h"

#include "graphic_const.h"
#include "mouse.h"
#include "keyboard.h"
#include "designlab_dxlib.h"
#include "Define.h"


void GraphicSystem::init(std::unique_ptr<IGraphicMainBuilder>&& graphic_main_builder, const GraphicDataBroker* const broker, const SApplicationSettingRecorder* const setting)
{
	if (graphic_main_builder && broker != nullptr && setting != nullptr)
	{
		//GraphicMainを作成する．
		mp_graphic_main = graphic_main_builder->build(broker, setting);
	}


	//設定ファイルを読み込む．
	mp_setting = setting;


	//Fpsを設定する．
	mp_fps = std::make_unique<Fps>((*mp_setting).window_fps);
}


void GraphicSystem::main()
{
	//そもそも描画処理を使わないならば即終了
	if (!(*mp_setting).gui_display) { return; }

	//設定ファイルを読み込めていなければ終了
	if (mp_setting == nullptr) { return; }

	//GraphicMainが作成されていなければ終了
	if (!mp_graphic_main) { return; }

	// Fpsが作成されていなければ終了
	if (!mp_fps) { return; }

	// Dxlibの関数は複数スレッドで呼ぶことを考慮されていないので，複数のスレッドから呼ぶと必ず問題が起きます．そのため，初期化処理，描画，終了処理の全てをこの関数の中で呼ぶ必要があります．
	if (!dxlibInit(mp_setting)) { return; }


	// ProcessMessage関数はウィンドウの×ボタンがおされると失敗の値を返す．また，ウィンドウを維持するためには定期的に呼び出し続ける必要があるのでループで呼び続けている．
	// ProcessMessageは成功で0(C++におけるfalse)，失敗で-1(C++におけるtrueは0以外の値)を返す，そのため !ProcessMessage はこの関数が成功の時のみループする...頭の痛い処理である．
	while (!ProcessMessage())
	{
		//falseが帰った場合，ループを抜ける．
		if (!loop())
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


bool GraphicSystem::loop()
{
	// [描画の処理について]
	// ScreenFlip関数とClearDrawScreen関数の詳細：ウィンドウの画像表示はパラパラ漫画の様に画面を素早く切り替えることでアニメーションを再現している．
	// しかし，単に画面を切り替えた場合，本物のパラパラ漫画の様にウィンドウにちらつきがでてしまう．
	// そこでGraphicSystemクラスのdxlibInit関数の中で呼ばれている SetDrawScreen(DX_SCREEN_BACK) によっていったん裏画面に絵を描画してから，
	// ScreenFlip関数でウィンドウに絵を戻すことで画面のちらつきをなくしている．
	// ClearDrawScreen も ScreenFlip も ProcessMessageと返す値が同じなので，loop関数の様な書き方となる．


	//グラフィックメインクラスが空ならfalseを返す．
	if (!mp_graphic_main) { return false; }


	//標準出力を消す
	clsDx();

	//キー入力を更新する．
	Keyboard::getIns()->update();
	Mouse::getIns()->update();

	//処理を行う
	if (!mp_graphic_main->update()) { return false; }


	//描画する
	if (!mp_fps->skipDrawScene())
	{
		//裏画面に描画した絵を消す
		if (ClearDrawScreen() < 0) { return false; }

		mp_graphic_main->draw();

		//スクリーンに裏画面に描画した内容を移す
		if (ScreenFlip() < 0) { return false; }

	}

	//FPSを一定に保つために待つ．
	mp_fps->wait();

	return true;
}


void GraphicSystem::dxlibFinalize() const
{
	// DXライブラリの終了処理を呼ぶ.
	DxLib_End();
}
