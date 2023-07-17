#include "GraphicLoop.h"
#include "DxLib.h"
#include "GraphicMainSample.h"
#include "Keyboard.h"
#include "Mouse.h"


GraphicLoop::GraphicLoop(std::unique_ptr<IGraphicMain>&& mp_graphic_main)
{
	//グラフィックメインクラスを代入する．ユニークポインタを代入する場合はmoveを行う．参考 https://nojima.hatenablog.com/entry/2014/12/10/014131
	mp_GraphicMain = std::move(mp_graphic_main);
}

bool GraphicLoop::loop()
{
	//グラフィックメインクラスが空ならfalseを返す．
	if (!mp_GraphicMain) { return false; }

	//標準出力を消す
	clsDx();

	//キー入力を更新する．
	Keyboard::getIns()->update();
	Mouse::getIns()->update();

	//処理を行う
	if (mp_GraphicMain->update() == false) { return false; }

	//描画する
	if (m_Fps.skipDrawScene() == false)
	{
		if (ClearDrawScreen() < 0) { return false; }	//裏画面に描画した絵を消す

		mp_GraphicMain->draw();

		if (ScreenFlip() < 0) { return false; }		//スクリーンに裏画面に描画した内容を移す
	}

	//FPSを一定に保つために待つ．
	m_Fps.wait();

	return true;
}