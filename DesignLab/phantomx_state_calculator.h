#pragma once

#include "abstract_hexapod_state_calculator.h"

#include <algorithm>

#include "hexapod_const.h"


class PhantomXStateCalclator : public AbstractHexapodStateCalculator
{
public:

	PhantomXStateCalclator();


	bool CalculateAllJointState(const RobotStateNode& node, std::array<HexapodJointState, HexapodConst::LEG_NUM>* joint_state) const override;


	designlab::Vector3 ConvertGlobalToLegPosition(int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const override;


	designlab::Vector3 GetFreeLegPosition(int leg_index) const override;


	designlab::Vector3 GetLocalLegBasePosition(int leg_index) const override;

	designlab::Vector3 GetLocalLegPosition(int leg_index, const designlab::Vector3& leg_pos) const override;


	designlab::Vector3 GetGlobalLegBasePosition(int leg_index, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const override;

	designlab::Vector3 GetGlobalLegPosition(int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const override;


	virtual bool IsLegInRange(int leg_index, const designlab::Vector3& leg_pos) const override;

	bool IsLegInterfering(const std::array<designlab::Vector3, HexapodConst::LEG_NUM>& leg_pos) const override;


private:

	static constexpr int LEG_POS_DIV_NUM = 50;		//脚位置の離散化数

	static constexpr float LEG_POS_MARGIN = 2;

	static constexpr float LEG_POS_MIN = -(HexapodConst::PHANTOMX_COXA_LENGTH + HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH + LEG_POS_MARGIN);	//脚位置の最小値

	static constexpr float LEG_POS_MAX = (HexapodConst::PHANTOMX_COXA_LENGTH + HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH + LEG_POS_MARGIN);		//脚位置の最大値

	static constexpr float MIN_LEG_R = 120;


	constexpr int getLegPosIndex(const float leg_pos) const
	{
		constexpr float converter = LEG_POS_DIV_NUM / (LEG_POS_MAX - LEG_POS_MIN);
		int res = static_cast<int>((leg_pos - LEG_POS_MIN) * converter);			// 離散化した脚位置を取得

		return res;										// 離散化した脚位置を返す
	}

	bool initIsAbleLegPos(const int leg_index, const int x, const int y, const int z) const;		// 脚位置の有効無効を初期化する

	void calculateLocalJointState(const int leg_index, const designlab::Vector3& leg_pos, HexapodJointState* joint_state) const;		// 脚位置から関節角度を計算する．1脚版



	// 脚番号，x座標，y座標，z座標の順でアクセスすると，その座標が有効かどうかがboolで格納されている
	bool m_is_able_leg_pos[HexapodConst::LEG_NUM][LEG_POS_DIV_NUM][LEG_POS_DIV_NUM][LEG_POS_DIV_NUM];

	//!< 脚の付け根の座標( leg base position)．ロボットの重心を原点，向いている方向をx軸としたローカル(ロボット)座標系である．
	designlab::Vector3 m_local_leg_base_pos[HexapodConst::LEG_NUM];	

	//!< 遊脚する位置．脚座標系
	const std::array<designlab::Vector3, HexapodConst::LEG_NUM> free_leg_pos_;
};


