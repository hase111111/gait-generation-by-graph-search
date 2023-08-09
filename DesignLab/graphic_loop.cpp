#include "graphic_loop.h"

#include <iostream>

#include "DxLib.h"

#include "graphic_main_sample.h"
#include "keyboard.h"
#include "mouse.h"


GraphicLoop::GraphicLoop(std::unique_ptr<IGraphicMain>&& graphic_main)
{
	//グラフィックメインクラスを代入する．ユニークポインタを代入する場合はmoveを行う．参考 https://nojima.hatenablog.com/entry/2014/12/10/014131
	mp_graphic_main = std::move(graphic_main);
}


bool GraphicLoop::loop()
{
	// [描画の処理について]
	// ScreenFlip関数とClearDrawScreen関数の詳細：ウィンドウの画像表示はパラパラ漫画の様に画面を素早く切り替えることでアニメーションを再現している．
	// しかし，単に画面を切り替えた場合，本物のパラパラ漫画の様にウィンドウにちらつきがでてしまう．
	// そこでGraphicSystemクラスのdxlibInit関数の中で呼ばれている SetDrawScreen(DX_SCREEN_BACK) によっていったん裏画面に絵を描画してから，
	// ScreenFlip関数でウィンドウに絵を戻すことで画面のちらつきをなくしている．
	// ClearDrawScreen も ScreenFlip も ProcessMessageと返す値が同じなので，loop関数の様な書き方となる．


	//グラフィックメインクラスが空ならfalseを返す．
	if (!mp_graphic_main)
	{
		std::cout << "GraphicLoop::loop() : mp_graphic_main is empty.\n";
		return false;
	}

	//標準出力を消す
	clsDx();

	//キー入力を更新する．
	Keyboard::getIns()->update();
	Mouse::getIns()->update();

	//処理を行う
	if (!mp_graphic_main->update())
	{
		std::cout << "GraphicLoop::loop() : mp_graphic_main->update() is false.\n";
		return false;
	}

	//描画する
	if (!m_fps.skipDrawScene())
	{
		//裏画面に描画した絵を消す
		if (ClearDrawScreen() < 0)
		{
			std::cout << "GraphicLoop::loop() : ClearDrawScreen() < 0.\n";
			return false;
		}

		mp_graphic_main->draw();

		//スクリーンに裏画面に描画した内容を移す
		if (ScreenFlip() < 0)
		{
			std::cout << "GraphicLoop::loop() : ScreenFlip() < 0.\n";
			return false;
		}
	}

	//FPSを一定に保つために待つ．
	m_fps.wait();

	return true;
}