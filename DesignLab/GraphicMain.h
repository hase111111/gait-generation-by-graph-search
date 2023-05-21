#pragma once

// GraphicMainは Dxlibの処理を行ってくれるクラスです．
// Dxlib(デラックス ライブラリ)はウィンドウを表示して，所謂コマンドラインに文字を表示するだけの寂しいプログラムに彩りを与えてくれるやつです．
// 主にゲームプログラミングをする際に，ウィンドウを表示するためのライブラリとして使用されます．
// Dxlib以外にも OpenCVなどにもウィンドウを表示する機能がありますが，今回のプログラムではDxlibを用いて結果を表示します．
// Dxlibは WinAPI というWindowsのアプリケーションを作るための機能を，使いやすくしてくれるライブラリです．

class GraphicMain final
{
public:
	GraphicMain() = default;
	~GraphicMain() = default;

	//Dxlibの初期化を行います．失敗するとfalseを返します．
	bool init();

	// ウィンドウの表示を行ってくれる関数です．boost::threadにこの関数を渡して並列処理を行います．initに失敗している，またはinitを呼ぶ前に実行した時は即座に終了します．
	void main();

	//Dxlibの終了処理を行います．
	void finalize();

private:

	bool m_is_init_success = false;
};

//GraphicMain _gr;
//if (_gr.init()) { _gr.main(); }
//_gr.finalize();
//return 0;