#pragma once
#include "MyVector.h"
#include "Node.h"


class HexapodStateCalclator
{
public:
	HexapodStateCalclator();

	//! @brief 脚座標は脚の付け根を原点とした座標系なので，それをグローバル座標に変換する．
	//! @param [in] _node ロボットの状態を表すノード
	//! @param [in] _leg_num 脚番号 0～5
	//! @param [in] _consider_rot 回転を考慮するかどうか
	//! @return my_vec::SVector グローバル座標の脚座標
	inline my_vec::SVector getGlobalLegPos(const SNode& _node, const int _leg_num, const bool _consider_rot) const
	{
		if (_consider_rot == true) { return rotVector(getLocalCoxaJointPos(_leg_num) + _node.leg_pos[_leg_num], _node.rot) + _node.global_center_of_mass; }
		else { return _node.global_center_of_mass + getLocalCoxaJointPos(_leg_num) + _node.leg_pos[_leg_num]; }
	}

	//! @brief 第2引数の座標を現在の重心座標と回転から，脚の付け根を原点としたローカル座標に変換する．
	//! @param [in] _node ロボットの状態を表すノード
	//! @param [in] _global_pos 変換するグローバル座標
	//! @param [in] _leg_num 脚番号 0～5
	//! @return my_vec::SVector ローカル座標の脚座標
	my_vec::SVector convertLocalLegPos(const SNode& _node, const my_vec::SVector& _global_pos, const int _leg_num) const;

	//! @brief 脚位置は離散化されて制御されるが，その時の4の位置をグローバル座標で出力する．また，ロボットの回転を考慮する．
	my_vec::SVector getGlobalLeg2Pos(const SNode& _node, const int _leg_num) const;

	// coxa joint (脚の付け根 : 第1関節) の座標を返す．回転を考慮したグローバル座標.
	my_vec::SVector getGlobalCoxaJointPos(const SNode& _node, const int _leg_num) const;

	//! @brief ノードの情報は現在の脚位置と重心位置しか持たないので，ジョイントがどこにあるかが分からない．よってこの関数で計算する．<br>
	//! 三角関数を多く使用するので，計算量が多い．
	//! @param [in] _node ロボットの状態を表すノード
	//! @details この関数を使用すると，メンバ変数が更新される．
	void calclateJointPos(const SNode& _node);

	//【calclateJointPos関数を使用してから使うこと!!】femur joint (第2関節) の座標を返す．回転を考慮したグローバル座標.
	my_vec::SVector getGlobalFemurJointPos(const SNode& _node, const int _leg_num) const;

	//【calclateJointPos関数を使用してから使うこと!!】tibia joint (第3関節) の座標を返す．回転を考慮したグローバル座標.
	my_vec::SVector getGlobalTibiaJointPos(const SNode& _node, const int _leg_num) const;

	//静的メンバ変数の m_leg_max_r , m_leg_min_r の値を計算して初期化する．この関数自体が静的なのでSystemMainで一度だけ実行すればよい．
	static void initLegR();

	//【initLegR関数を使用してから使うこと!!】付け根から脚先までのZ座標の差を利用して，付け根から脚先までの最大半径を取得する．付け根の方が上にある前提で動きます．
	float getMaxLegR(const float _coxa_z_to_leg_z) const;

	//【initLegR関数を使用してから使うこと!!】付け根から脚先までのZ座標の差を利用して，付け根から脚先までの最小半径を取得する．付け根の方が上にある前提で動きます．
	float getMinLegR(const float _coxa_z_to_leg_z) const;

	//! @brief 脚の干渉をチェックする．
	//! @param _node ノード情報
	//! @return 干渉している場合はtrueを返す．
	bool isLegInterfering(const SNode& _node) const;

	//! @brief 脚が可動範囲内かチェックする．速度重視のため，ざっくりとした計算を行う．
	//! @param _node ノード情報
	//! @param _leg_num 脚番号
	//! @return 可動範囲内ならtrueを返す．
	bool isLegInRange(const SNode& _node, const int _leg_num) const;

	//! @brief 全ての脚が可動範囲内かチェックする．速度重視のため，ざっくりとした計算を行う．
	//! @param _node ノード情報
	//! @return 可動範囲内ならtrueを返す．
	bool isAllLegInRange(const SNode& _node) const;

	//! @brief 転ばない姿勢かどうか調べる．
	//! @param _node ノード情報
	//! @return 転ばない姿勢ならtrueを返す．
	bool isAblePause(const SNode& _node) const;

private:
	my_vec::SVector getLocalCoxaJointPos(const int _leg_num) const;	// coxa joint (脚の付け根)の座標を返す．重心を原点とするローカル座標．

	my_vec::SVector m_local_femurjoint_pos[HexapodConst::LEG_NUM];	//FemurJoint(第2関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．
	my_vec::SVector m_local_tibiajoint_pos[HexapodConst::LEG_NUM];	//TibiaJoint(第3関節)の位置．脚の付け根を原点とするローカル座標．calclateJointPos関数で値をセットする．

	constexpr static int MAX_DIF_Z = 200;	//付け根から脚先までのZ座標の差の最大値

	static float m_leg_max_r[MAX_DIF_Z];	//重心高さから脚位置を下げた時の，脚の取りうる最大半径を記録したもの．旧名 Leg_ROM_R
	static float m_leg_min_r[MAX_DIF_Z];	//重心高さから脚位置を下げた時の，脚の取りうる最小半径を記録したもの．旧名 Leg_ROM_R
};


//! @file HexapodStateCalclator.h
//! @brief HexapodStateCalclatorクラスのヘッダファイル．
//! @date 2023/07/19
//! @auther 長谷川

//! @class HexapodStateCalclator
//! @brief ロボットの座標や状態などの値を計算するクラス．旧 Hexapodクラスを軽くしたもの．
//! @date 2023/07/19
//! @auther 長谷川