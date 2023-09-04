#pragma once

#include <memory>

#include "node.h"
#include "hexapod_const.h"
#include "abstract_hexapod_state_calculator.h"


//! @class HexapodRenderer
//! @date 2023/08/09
//! @author 長谷川
//! @brief ロボットの描画を行うクラス．
class HexapodRenderer
{
public:
	HexapodRenderer(std::shared_ptr<AbstractHexapodStateCalculator> calc);
	~HexapodRenderer() = default;

	//! @brief ロボットの状態を更新する．
	//! @param [in] node 描画するロボットの状態
	void setNode(const SNode& node);

	//! @brief ロボットを3D空間に描画する．
	//! @param [in] node 描画するロボットの状態
	void draw(const SNode& node) const;

private:

	bool isAbleCoxaLeg(const dl_vec::SVector& coxa_joint, const dl_vec::SVector& femur_joint) const;
	bool isAbleFemurLeg(const dl_vec::SVector& femur_joint, const dl_vec::SVector& tibia_joint) const;
	bool isAbleTibiaLeg(const dl_vec::SVector& tibia_joint, const dl_vec::SVector& leg_joint) const;


	const unsigned int COLOR_BODY;			// 胴体の色
	const unsigned int COLOR_LEG;			// 脚の色
	const unsigned int COLOR_LIFTED_LEG;	// 遊脚している脚の色
	const unsigned int COLOR_JOINT;			// ジョイントの色
	const unsigned int COLOR_LIFTED_JOINT;	// 遊脚しているジョイントの色
	const unsigned int COLOR_LEG_BASE;		// 脚の基部の色

	const int CAPSULE_DIV_NUM;				// ロボットのモデルの円柱をどれだけ細かく描画するか．4 ～ 20ぐらいがちょうどよいと思う．
	const int SPHERE_DIV_NUM;				// ロボットのモデルの球をどれだけ細かく描画するか．16 ～ 32ぐらいがちょうどよいと思う．
	const float LEG_R = 10.0f;				// 脚の半径．このクラスでは脚を円柱に近似して描画している．描画時のデータのため，これを変化させてもシミュレーションに影響はない．
	const float JOINT_R = 20.0f;			// ジョイントの半径．描画時のデータのため，これを変化させてもシミュレーションに影響はない．

	const bool DO_OUTPUT_DEBUG_LOG = false;	// 脚状態を文字列で出力するかどうか


	std::shared_ptr<AbstractHexapodStateCalculator> mp_calculator;	// ロボットの状態を計算するクラス

	SNode m_draw_node;						// 描画するロボットの状態

	SHexapodJointState m_draw_joint_state[HexapodConst::LEG_NUM];	// 描画するロボットのジョイントの状態
};


//! @file hexapod_renderer.h
//! @date 2023/08/09
//! @author 長谷川
//! @brief ロボットの描画を行うHexapodRendererクラス．
//! @n 行数 : @lineinfo
