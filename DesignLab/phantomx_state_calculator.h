#pragma once

#include "abstract_hexapod_state_calculator.h"

#include <algorithm>

#include "hexapod_const.h"


class PhantomXStateCalclator final : public AbstractHexapodStateCalculator
{
public:

	PhantomXStateCalclator();


	bool calculateAllJointState(const SNode& node, SHexapodJointState joint_state[HexapodConst::LEG_NUM]) const override;


	designlab::Vector3 convertGlobalToLegPosition(const int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::SRotator& robot_rot, const bool consider_rot) const override;


	designlab::Vector3 getLocalLegPosition(const int leg_index, const designlab::Vector3& leg_pos) const override;


	designlab::Vector3 getGlobalLegBasePosition(const int leg_index, const designlab::Vector3& global_center_of_mass, const designlab::SRotator& robot_rot, const bool consider_rot) const override;

	designlab::Vector3 getGlobalLegPosition(const int leg_index, const designlab::Vector3& leg_pos, const designlab::Vector3& global_center_of_mass, const designlab::SRotator& robot_rot, const bool consider_rot) const override;


	bool isLegInRange(const int leg_index, const designlab::Vector3& leg_pos) const override;

	bool isLegInterfering(const designlab::Vector3 leg_pos[HexapodConst::LEG_NUM]) const override;


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

	void calculateLocalJointState(const int leg_index, const designlab::Vector3& leg_pos, SHexapodJointState* joint_state) const;		// 脚位置から関節角度を計算する．1脚版



	// 脚番号，x座標，y座標，z座標の順でアクセスすると，その座標が有効かどうかがboolで格納されている
	bool m_is_able_leg_pos[HexapodConst::LEG_NUM][LEG_POS_DIV_NUM][LEG_POS_DIV_NUM][LEG_POS_DIV_NUM];
};


