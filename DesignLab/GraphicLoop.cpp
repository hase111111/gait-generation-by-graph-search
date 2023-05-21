#include "GraphicLoop.h"
#include "DxLib.h"

//描画の処理について，
// ScreenFlip関数とClearDrawScreen関数について．ウィンドウの画像表示はパラパラ漫画の様に画面を素早く切り替えることでアニメーションを再現している．
// しかし，単に画面を切り替えた場合，本物のパラパラ漫画の様にウィンドウにちらつきがでてしまう．
// そこでinit関数の中で呼ばれている SetDrawScreen(DX_SCREEN_BACK) によっていったん裏画面に絵を描画してから，ScreenFlip関数でウィンドウに絵を戻すことで画面のちらつきをなくしている．
// ClearDrawScreen も ScreenFlip も ProcessMessageと返す値が同じなので，以下の様な書き方となる．


bool GraphicLoop::loop()
{
	//処理を行う

	if (m_fps.skipDrawScene() == false) 
	{
		if (ClearDrawScreen() < 0) { return false; }	//裏画面に描画した絵を消す

		//描画する
		
		if (ScreenFlip() < 0) { return false; }		//スクリーンに裏画面に描画した内容を移す
	}

	m_fps.wait();	//FPSを一定に保つために待つ．

	return true;
}