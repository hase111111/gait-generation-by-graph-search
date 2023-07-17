#pragma once
#include "Node.h"
#include "HexapodStateCalculator.h"


class HexapodRenderer
{
private:
	const unsigned int COLOR_BODY;			//胴体の色
	const unsigned int COLOR_LEG;			//脚の色
	const unsigned int COLOR_LIFTED_LEG;	//遊脚している脚の色
	const unsigned int COLOR_JOINT;			//ジョイントの色
	const unsigned int COLOR_LIFTED_JOINT;	//遊脚しているジョイントの色

	const int CAPSULE_DIV_NUM;				//ロボットのモデルの円柱をどれだけ細かく描画するか．4 ～ 20ぐらいがちょうどよいと思う．
	const int SPHERE_DIV_NUM;				//ロボットのモデルの球をどれだけ細かく描画するか．16 ～ 32ぐらいがちょうどよいと思う．
	const float LEG_R = 10.0f;				//脚の半径．このクラスでは脚を円柱に近似して描画している．描画時のデータのため，これを変化させてもシミュレーションに影響はない．
	const float JOINT_R = 20.0f;			//ジョイントの半径．描画時のデータのため，これを変化させてもシミュレーションに影響はない．

	const bool DO_OUTPUT_DEBUG_LOG = false;	//脚状態を文字列で出力するかどうか

	HexapodStateCalclator m_HexaCalc;		//ロボットの姿勢や座標を計算する．

	bool isAbleCoxaLeg(const my_vec::SVector _coxa_joint, const my_vec::SVector _femur_joint) const;
	bool isAbleFemurLeg(const my_vec::SVector _femur_joint, const my_vec::SVector _tibia_joint) const;
	bool isAbleTibiaLeg(const my_vec::SVector _tibia_joint, const my_vec::SVector _leg_joint) const;

public:
	HexapodRenderer();
	~HexapodRenderer() = default;

	//! @brief ロボットの状態を更新する．
	//! @param [in] _node 描画するロボットの状態
	void update(const SNode& _node);

	//! @brief ロボットを3D空間に描画する．
	//! @param [in] _node 描画するロボットの状態
	void draw(const SNode& _node) const;

};


//! @file HexapodRenderer.h
//! @brief ロボットの描画を行うクラスの実装．
//! @author 長谷川

//! @class HexapodRenderer
//! @brief ロボットの描画を行うクラス．
//! @details 
//! @author 長谷川
