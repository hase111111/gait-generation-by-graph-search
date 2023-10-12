//! @file phantomx_state_calculator.h
//! @brief PhantomXの状態を計算するクラス．


#ifndef DESIGNLAB_PHANTOMX_STATE_CALCULATOR_H_
#define DESIGNLAB_PHANTOMX_STATE_CALCULATOR_H_


#include "abstract_hexapod_state_calculator.h"

#include <algorithm>

#include "hexapod_const.h"


//! @class PhantomXStateCalclator
//! @brief PhantomXの状態を計算するクラス．
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

	static constexpr int kLegPosDivNum = 50;	//!< 脚位置の離散化数

	static constexpr float kLegPosMargin = 2;

	static constexpr float kLegPosMin = -(HexapodConst::PHANTOMX_COXA_LENGTH + HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH + kLegPosMargin);	//脚位置の最小値

	static constexpr float kLegPosMax = (HexapodConst::PHANTOMX_COXA_LENGTH + HexapodConst::PHANTOMX_FEMUR_LENGTH + HexapodConst::PHANTOMX_TIBIA_LENGTH + kLegPosMargin);		//脚位置の最大値

	static constexpr float kMinLegR = 120;


	constexpr int GetLegPosIndex(const float leg_pos) const
	{
		constexpr float converter = kLegPosDivNum / (kLegPosMax - kLegPosMin);
		int res = static_cast<int>((leg_pos - kLegPosMin) * converter);			// 離散化した脚位置を取得

		return res;										// 離散化した脚位置を返す
	}

	bool InitIsAbleLegPos(int leg_index, int x, int y, int z) const;		// 脚位置の有効無効を初期化する

	void CalculateLocalJointState(int leg_index, const designlab::Vector3& leg_pos, HexapodJointState* joint_state) const;		// 脚位置から関節角度を計算する．1脚版



	// 脚番号，x座標，y座標，z座標の順でアクセスすると，その座標が有効かどうかがboolで格納されている
	bool is_able_leg_pos_[HexapodConst::LEG_NUM][kLegPosDivNum][kLegPosDivNum][kLegPosDivNum];

	//!< 脚の付け根の座標( leg base position)．ロボットの重心を原点，向いている方向をx軸としたローカル(ロボット)座標系である．
	designlab::Vector3 local_leg_base_pos_[HexapodConst::LEG_NUM];	

	//!< 遊脚する位置．脚座標系
	const std::array<designlab::Vector3, HexapodConst::LEG_NUM> free_leg_pos_;
};


#endif