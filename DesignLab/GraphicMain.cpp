#include "GraphicMain.h"
#include "DxLib.h"
#include "GraphicConst.h"
#include "GraphicLoop.h"

//注意として，Dxlib系の関数は 真偽を大文字の TRUEとFALSEを使って表すので，従来のtrue falseを使用しないようにしましょう．
//まぁ，ぶっちゃけ小文字の方でも動くけど，バージョンの更新によって動かなくなる可能性があるのでDxlibに組み込まれているものを使うのが無難です．
//また，Dxlibはエラーが出たときに -1 を返す関数が非常に多いです．そのため例えば if(DxLib_Init() == false) と書いてもエラーを受け取れないことがあります．
//正しくは if(DxLib_Init() < 0) となります．これは bool型 がデフォルトで存在しないC言語でも使用することができるようにするための配慮であり，C++で書かれている本コードにおいては
//混乱の元です(涙)．Dxlibのエラーはboolではなく，int型の負の値ということを覚えておいてください．


bool GraphicMain::init()
{	
	// 1部の初期化用関数はDxlib_Initを呼ぶ前に実行する必要があるのでここで実行します．

	SetMainWindowText(GraphicConst::WIN_NAME.c_str());	//タイトルを変更．ウィンドウの左上に表示されるものです．
	ChangeWindowMode(TRUE);								//ウインドウモードに変更．これをしないとフルスクリーンで表示されます．
	SetWindowSizeChangeEnableFlag(FALSE);               //ウィンドウサイズを自由に変更できないようにする．
	SetOutApplicationLogValidFlag(FALSE);				//ログ出力無しに変更．これをしないとLog.txtという邪魔なファイルが出力されます．
	SetAlwaysRunFlag(TRUE);								//ウインドウがアクティブではない状態でも処理を続行するように変更．

	//ウィンドウの横幅，縦幅，カラーを設定します．
	SetGraphMode(GraphicConst::WIN_X, GraphicConst::WIN_Y, GraphicConst::COLOR_BIT);

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() < 0)
	{
		//エラーが出ると負の値を返すので，その場合はfalse．
		m_is_init_success = false;
		return false;
	}

	//描画先を裏画面にする．説明が難しいのですが，画面のちらつきを押えてくれる効果があり，Dxlibを使う以上必須の項目です．
	SetDrawScreen(DX_SCREEN_BACK);						

	//初期化に成功したので，フラグを立てて終了.
	m_is_init_success = true;
	return true;
}

void GraphicMain::main()
{
	//初期化をしていない or 失敗した場合終了
	if (m_is_init_success == false) { return; }

	GraphicLoop _Looper;

	// ProcessMessage関数はウィンドウの×ボタンがおされると失敗の値を返す．また，ウィンドウを維持するためには定期的に呼び出し続ける必要があるのでループで呼び続けている．
	// ProcessMessageは成功で0(C++におけるfalse)，失敗で-1(C++におけるtrueは0以外の値)を返す，そのため !ProcessMessage はこの関数が成功の時のみループする...頭の痛い処理である．
	while (!ProcessMessage())
	{
		//falseが帰った場合，ループを抜ける．
		if (_Looper.loop() == false) 
		{
			break;
		}
	}

	//終了する
	finalize();
}

void GraphicMain::finalize() const
{
	// DXライブラリの終了処理を呼ぶ.
	DxLib_End();
}
