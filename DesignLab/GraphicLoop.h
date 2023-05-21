#pragma once
#include "Fps.h"

class GraphicLoop final
{
public:
	GraphicLoop() = default;
	~GraphicLoop() = default;

	//画像表示の処理の中で呼ばれ続ける処理をまとめたもの．ループを抜けたい時はfalseを返す．
	bool loop();

private:

	//FPSを一定に制御するクラス．詳しくはFps.hへ
	Fps m_fps;
};
