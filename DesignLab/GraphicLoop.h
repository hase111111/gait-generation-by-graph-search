#pragma once
#include "Fps.h"
#include "AbstractGraphicMain.h"
#include <memory>

class GraphicLoop final
{
public:
	GraphicLoop() = delete;	//通常のコンストラクタは呼べない．必ずstd::unique_ptr<AbstractGraphicMain>を引数にとる必要がある．

	GraphicLoop(std::unique_ptr<AbstractGraphicMain>&& mp_graphic_main);
	~GraphicLoop() = default;

	//画像表示の処理の中で呼ばれ続ける処理をまとめたもの．ループを抜けたい時はfalseを返す．
	bool loop();

private:

	//FPSを一定に制御するクラス．詳しくはFps.hへ
	Fps m_Fps;

	//描画処理を行うメインのクラス．unique_ptr(ユニークポインタ)については，参照→ https://qiita.com/seriru13/items/06d044cbe5bcc44cca10
	std::unique_ptr<AbstractGraphicMain> mp_GraphicMain;
};
