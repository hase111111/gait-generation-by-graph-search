#include "GraphicLoop.h"
#include "DxLib.h"
#include "GraphicMainSample.h"
#include "Keyboard.h"

//描画の処理について，
// ScreenFlip関数とClearDrawScreen関数について．ウィンドウの画像表示はパラパラ漫画の様に画面を素早く切り替えることでアニメーションを再現している．
// しかし，単に画面を切り替えた場合，本物のパラパラ漫画の様にウィンドウにちらつきがでてしまう．
// そこでinit関数の中で呼ばれている SetDrawScreen(DX_SCREEN_BACK) によっていったん裏画面に絵を描画してから，ScreenFlip関数でウィンドウに絵を戻すことで画面のちらつきをなくしている．
// ClearDrawScreen も ScreenFlip も ProcessMessageと返す値が同じなので，以下の様な書き方となる．


GraphicLoop::GraphicLoop(std::unique_ptr<AbstractGraphicMain>&& mp_graphic_main)
{
	//グラフィックメインクラスをセットする．ユニークポインタを代入する場合はmoveを行う．参考 https://nojima.hatenablog.com/entry/2014/12/10/014131
	mp_GraphicMain = std::move(mp_graphic_main);
}

bool GraphicLoop::loop()
{
	//グラフィックメインクラスが空ならfalseを返す．
	if (!mp_GraphicMain) { return false; }

	//キー入力を更新する．
	Keyboard::getIns()->update();

	//処理を行う
	mp_GraphicMain->update();

	if (m_Fps.skipDrawScene() == false) 
	{
		if (ClearDrawScreen() < 0) { return false; }	//裏画面に描画した絵を消す

		//描画する
		mp_GraphicMain->draw();
		
		if (ScreenFlip() < 0) { return false; }		//スクリーンに裏画面に描画した内容を移す
	}

	m_Fps.wait();	//FPSを一定に保つために待つ．

	return true;
}