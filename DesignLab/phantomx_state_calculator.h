#pragma once

#include "abstract_hexapod_state_calculator.h"

#include <algorithm>

#include "hexapod_const.h"


class PhantomXStateCalclator final : public AbstractHexapodStateCalculator
{
public:

	void init() override;


	bool calculateAllJointState(const SNode& node, SHexapodJointState* const joint_state) const override;


	dl_vec::SVector getLocalLegPosition(const int leg_index, const dl_vec::SVector& leg_pos) const override;


	dl_vec::SVector getGlobalLegBasePosition(const int leg_index, const dl_vec::SVector& global_center_of_mass, const dl_vec::SRotator& robot_rot, const bool consider_rot) const override;

	dl_vec::SVector getGlobalLegPosition(const int leg_index, const dl_vec::SVector& leg_pos, const dl_vec::SVector& global_center_of_mass, const dl_vec::SRotator& robot_rot, const bool consider_rot) const override;


	bool isLegInRange(const int leg_index, const dl_vec::SVector& leg_pos) const override;

	bool isLegInterfering(const dl_vec::SVector leg_pos[HexapodConst::LEG_NUM]) const override;


private:

	static constexpr int LEG_POS_DIV_NUM = 50;		//脚位置の離散化数

	static constexpr float LEG_POS_MARGIN = 22;

	static constexpr float LEG_POS_MIN = -(HexapodConst::PHANTOMX_COXA_LENGTH + HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH + LEG_POS_MARGIN);	//脚位置の最小値

	static constexpr float LEG_POS_MAX = (HexapodConst::PHANTOMX_COXA_LENGTH + HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH + LEG_POS_MARGIN);		//脚位置の最大値


	constexpr int getLegPosIndex(const float leg_pos) const
	{
		constexpr float converter = LEG_POS_DIV_NUM / (LEG_POS_MAX - LEG_POS_MIN);
		int res = static_cast<int>((leg_pos - LEG_POS_MIN) * converter);			// 離散化した脚位置を取得

		res = (std::max)(res, 0);						// 0以上にする
		res = (std::min)(res, LEG_POS_DIV_NUM - 1);	// LEG_POS_DIV_NUM - 1以下にする

		return res;									// 離散化した脚位置を返す
	}

	bool initIsAbleLegPos(const int leg_index, const int x, const int y, const int z) const;		// 脚位置の有効無効を初期化する


	// 脚番号，x座標，y座標，z座標の順でアクセスすると，その座標が有効かどうかがboolで格納されている
	bool m_is_able_leg_pos[HexapodConst::LEG_NUM][LEG_POS_DIV_NUM][LEG_POS_DIV_NUM][LEG_POS_DIV_NUM];
};


