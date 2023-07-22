#pragma once
#include "Fps.h"
#include "InterfaceGraphicMain.h"
#include <memory>

class GraphicLoop final
{
public:
	GraphicLoop() = delete;

	GraphicLoop(std::unique_ptr<IGraphicMain>&& mp_graphic_main);
	~GraphicLoop() = default;

	
	//! GraphicSystemクラスのwhileループの中で毎フレーム呼ばれる処理 <br> <br> [描画の処理について]<br>
	//! ScreenFlip関数とClearDrawScreen関数の詳細：ウィンドウの画像表示はパラパラ漫画の様に画面を素早く切り替えることでアニメーションを再現している．<br>
	//! しかし，単に画面を切り替えた場合，本物のパラパラ漫画の様にウィンドウにちらつきがでてしまう．<br>
	//! そこでGraphicSystemクラスのdxlibInit関数の中で呼ばれている SetDrawScreen(DX_SCREEN_BACK) によっていったん裏画面に絵を描画してから，
	//! ScreenFlip関数でウィンドウに絵を戻すことで画面のちらつきをなくしている．<br>
	//! ClearDrawScreen も ScreenFlip も ProcessMessageと返す値が同じなので，loop関数の様な書き方となる．<br>
	//! @return bool mp_GraphicMainのupdate関数がfalseを返した場合，或いはmp_GraphicMainクラスがnullの場合falseを返しループを抜ける. 
	bool loop();

private:

	Fps m_Fps;	//FPSを一定に制御するクラス．詳しくはFps.hへ

	std::unique_ptr<IGraphicMain> mp_GraphicMain;	//描画処理を行うメインのクラス．
};

//! @class GraphicLoop
//! @brief GraphicSystemクラスのwhileループの中で毎フレーム呼ばれる処理を実装している．いわゆるゲームループである．
//! @details どんな描画処理を行うかをコンストラクタで受け取る．万が一受け取れなかった場合，処理がされない．<br>
//! 通常のコンストラクタは呼べないように削除されており，必ずstd::unique_ptr<AbstractGraphicMain>を引数にとる必要がある．<br> <br>
//! unique_ptr(ユニークポインタ)については，参照→ https://qiita.com/seriru13/items/06d044cbe5bcc44cca10
//! @author 長谷川

//! @file GraphicLoop.h
//! @brief GraphicLoopクラスの実装を行っている．
//! @author 長谷川
