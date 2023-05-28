#pragma once
#include "vectorFunc.h"
#include "Node.h"

//ロボットの座標や状態などの値を計算するクラス．旧 Hexapodクラスを軽くしたもの．
class HexapodStateCalclator
{
public:
	HexapodStateCalclator();

	//脚座標は脚の付け根を原点とした座標系なので，それをグローバル座標に変換する．また，ロボットの回転を考慮する．
	myvector::SVector getGlobalLegPos(const SNode& _node, const int _leg_num) const;	

	// coxa joint (脚の付け根 : 第1関節) の座標を返す．回転を考慮したグローバル座標.
	myvector::SVector getGlobalCoxaJointPos(const SNode& _node, const int _leg_num) const;	

	//ノードの情報は現在の脚位置と重心位置しか持たないので，ジョイントがどこにあるかが分からない．よってこの関数で計算する．
	void calclateJointPos(const SNode& _node);

	//【calclateJointPos関数を使用してから使うこと!!】femur joint (第2関節) の座標を返す．回転を考慮したグローバル座標.
	myvector::SVector getGlobalFemurJointPos(const SNode& _node, const int _leg_num) const;

	//【calclateJointPos関数を使用してから使うこと!!】tibia joint (第3関節) の座標を返す．回転を考慮したグローバル座標.
	myvector::SVector getGlobalTibiaJointPos(const SNode& _node, const int _leg_num) const;

private:
	myvector::SVector getLocalCoxaJointPos(const int _leg_num) const;	// coxa joint (脚の付け根)の座標を返す．重心を原点とするローカル座標．

	myvector::SVector m_local_femurjoint_pos[HexapodConst::LEG_NUM];	//FemurJoint(第2関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．
	myvector::SVector m_local_tibiajoint_pos[HexapodConst::LEG_NUM];	//TibiaJoint(第3関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．
};
