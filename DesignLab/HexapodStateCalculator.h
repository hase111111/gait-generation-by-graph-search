#pragma once
#include "vectorFunc.h"

//ロボットの座標や状態などの値を計算するクラス．旧 Hexapodクラスの進化版
class HexapodStateCalclator
{
public:
	SNode getGlobalLegPos();	//脚座標は脚の付け根を原点とした座標系なので，それをグローバル座標に変換する．

private:

};
