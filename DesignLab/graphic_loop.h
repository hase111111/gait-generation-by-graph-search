#pragma once

#include <memory>

#include "fps.h"
#include "interface_graphic_main.h"


//! @class GraphicLoop
//! @date 2023/08/08
//! @author 長谷川
//! @brief GraphicSystemクラスのwhileループの中で毎フレーム呼ばれる処理を実装している．いわゆるゲームループである．
//! @details どんな描画処理を行うかをコンストラクタで受け取る．万が一受け取れなかった場合，処理がされない．
//! @n 通常のコンストラクタは呼べないように削除されており，必ずstd::unique_ptr<AbstractGraphicMain>を引数にとる必要がある．@n
//! @n unique_ptr(ユニークポインタ)については，参照→ https://qiita.com/seriru13/items/06d044cbe5bcc44cca10
class GraphicLoop final
{
public:
	GraphicLoop() = delete;

	GraphicLoop(std::unique_ptr<IGraphicMain>&& graphic_main);
	~GraphicLoop() = default;


	//! @brief GraphicSystemクラスのwhileループの中で毎フレーム呼ばれる処理
	//! @return bool mp_graphic_mainのupdate関数がfalseを返した場合や，mp_graphic_mainがnullの場合など，描画処理を終了する場合にループを抜け，falseを返す. 
	bool loop();

private:

	Fps m_fps;	//FPSを一定に制御するクラス．詳しくはFps.hへ

	std::unique_ptr<IGraphicMain> mp_graphic_main;	//描画処理を行うメインのクラス．
};


//! @file graphic_loop.h
//! @date 2023/08/08
//! @author 長谷川
//! @brief 画像描画処理のメインループで呼ばれる処理であるGraphicLoopクラス
//! @n 行数 : @lineinfo
