#pragma once
#include "vectorFunc.h"
#include "Node.h"

//ロボットの座標や状態などの値を計算するクラス．旧 Hexapodクラスの進化版
class HexapodStateCalclator
{
public:

	//脚座標は脚の付け根を原点とした座標系なので，それをグローバル座標に変換する．また，ロボットの回転を考慮する．
	myvector::SVector getGlobalLegPos(const SNode& _node, const int _leg_num) const;	

	// coxa joint (脚の付け根)の座標を返す．回転を考慮したグローバル座標.
	myvector::SVector getGlobalCoxaJointPos(const SNode& _node, const int _leg_num) const;	


private:
	myvector::SVector getLocalCoxaJointPos(const int _leg_num) const;	// coxa joint (脚の付け根)の座標を返す．重心を原点とするローカル座標．
};
