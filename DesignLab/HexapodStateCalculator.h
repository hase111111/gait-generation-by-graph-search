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

	//脚位置は離散化されて制御されるが，その時の4の位置をグローバル座標で出力する．また，ロボットの回転を考慮する．
	myvector::SVector getGlobalLeg2Pos(const SNode& _node, const int _leg_num) const;

	// coxa joint (脚の付け根 : 第1関節) の座標を返す．回転を考慮したグローバル座標.
	myvector::SVector getGlobalCoxaJointPos(const SNode& _node, const int _leg_num) const;	

	//ノードの情報は現在の脚位置と重心位置しか持たないので，ジョイントがどこにあるかが分からない．よってこの関数で計算する．
	void calclateJointPos(const SNode& _node);

	//【calclateJointPos関数を使用してから使うこと!!】femur joint (第2関節) の座標を返す．回転を考慮したグローバル座標.
	myvector::SVector getGlobalFemurJointPos(const SNode& _node, const int _leg_num) const;

	//【calclateJointPos関数を使用してから使うこと!!】tibia joint (第3関節) の座標を返す．回転を考慮したグローバル座標.
	myvector::SVector getGlobalTibiaJointPos(const SNode& _node, const int _leg_num) const;

	//静的メンバ変数の m_leg_max_r , m_leg_min_r の値を計算して初期化する．この関数自体が静的なのでSystemMainで一度だけ実行すればよい．
	static void initLegR();

	//【initLegR関数を使用してから使うこと!!】付け根から脚先までのZ座標の差を利用して，付け根から脚先までの最大半径を取得する．付け根の方が上にある前提で動きます．
	float getMaxLegR(const float _coxa_z_to_leg_z) const;

	//【initLegR関数を使用してから使うこと!!】付け根から脚先までのZ座標の差を利用して，付け根から脚先までの最小半径を取得する．付け根の方が上にある前提で動きます．
	float getMinLegR(const float _coxa_z_to_leg_z) const;

private:
	myvector::SVector getLocalCoxaJointPos(const int _leg_num) const;	// coxa joint (脚の付け根)の座標を返す．重心を原点とするローカル座標．

	myvector::SVector m_local_femurjoint_pos[HexapodConst::LEG_NUM];	//FemurJoint(第2関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．
	myvector::SVector m_local_tibiajoint_pos[HexapodConst::LEG_NUM];	//TibiaJoint(第3関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．

	constexpr static int MAX_DIF_Z = 200;	//付け根から脚先までのZ座標の差の最大値

	static float m_leg_max_r[MAX_DIF_Z];	//重心高さから脚位置を下げた時の，脚の取りうる最大半径を記録したもの．旧名 Leg_ROM_R
	static float m_leg_min_r[MAX_DIF_Z];	//重心高さから脚位置を下げた時の，脚の取りうる最小半径を記録したもの．旧名 Leg_ROM_R
};
