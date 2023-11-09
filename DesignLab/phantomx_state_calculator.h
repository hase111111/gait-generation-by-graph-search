//! @file phantomx_state_calculator.h
//! @brief PhantomXの状態を計算するクラス．


#ifndef DESIGNLAB_PHANTOMX_STATE_CALCULATOR_H_
#define DESIGNLAB_PHANTOMX_STATE_CALCULATOR_H_


#include "abstract_hexapod_state_calculator.h"

#include <algorithm>
#include <array>

#include "designlab_vector2.h"
#include "hexapod_const.h"
#include "phantomx_const.h"


//! @class PhantomXStateCalclator
//! @brief PhantomXの状態を計算するクラス．
//! @todo いろいろ怪しい設計なので，全面的に改修する．
class PhantomXStateCalclator : public AbstractHexapodStateCalculator
{
public:

	PhantomXStateCalclator();


	void CalculateAllJointState(const RobotStateNode& node, std::array<HexapodJointState, HexapodConst::kLegNum>* joint_state) const override;

	bool IsVaildJointState(const RobotStateNode& node, const std::array<HexapodJointState, HexapodConst::kLegNum>& joint_state) const override;

	designlab::Vector3 ConvertGlobalToLegCoordinate(const designlab::Vector3& converted_position, int leg_index, const designlab::Vector3& center_of_mass_global, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const override;

	designlab::Vector3 ConvertLegToGlobalCoordinate(const designlab::Vector3& converted_position, int leg_index, const designlab::Vector3& center_of_mass_global, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const override;

	designlab::Vector3 ConvertRobotToGlobalCoordinate(const designlab::Vector3& converted_position, const designlab::Vector3& center_of_mass_global, const designlab::EulerXYZ& robot_rot, const bool consider_rot) const override;

	designlab::Vector3 GetFreeLegPosLegCoodinate(int leg_index) const override;

	designlab::Vector3 GetLegBasePosRobotCoodinate(int leg_index) const override;


	virtual bool IsLegInRange(int leg_index, const designlab::Vector3& leg_pos) const override;

	bool IsLegInterfering(const std::array<designlab::Vector3, HexapodConst::kLegNum>& leg_pos) const override;

protected:
	const std::array<::designlab::Vector2, HexapodConst::kLegNum> min_leg_pos_xy_;
	const std::array<::designlab::Vector2, HexapodConst::kLegNum> max_leg_pos_xy_;

private:

	static constexpr int kLegPosDivNum = 50;	//!< 脚位置の離散化数

	static constexpr float kLegPosMargin = 2;

	static constexpr float kLegPosMin = -(PhantomXConst::kCoxaLength + PhantomXConst::kFemurLength + PhantomXConst::kTibiaLength + kLegPosMargin);	//脚位置の最小値

	static constexpr float kLegPosMax = (PhantomXConst::kCoxaLength + PhantomXConst::kFemurLength + PhantomXConst::kTibiaLength + kLegPosMargin);	//脚位置の最大値

	static constexpr float kMinLegR = 120;

	constexpr int GetLegPosIndex(const float leg_pos) const
	{
		constexpr float converter = kLegPosDivNum / (kLegPosMax - kLegPosMin);
		int res = static_cast<int>((leg_pos - kLegPosMin) * converter);			// 離散化した脚位置を取得

		return res;										// 離散化した脚位置を返す
	}

	bool InitIsAbleLegPos(int leg_index, int x, int y, int z) const;		// 脚位置の有効無効を初期化する

	void CalculateLocalJointState(int leg_index, const designlab::Vector3& leg_pos, HexapodJointState* joint_state) const;		// 脚位置から関節角度を計算する．1脚版
	
	std::array<::designlab::Vector2, HexapodConst::kLegNum> InitMinLegPosXY() const;		// 脚の付け根の座標( leg base position)の最小値を計算する
	std::array<::designlab::Vector2, HexapodConst::kLegNum> InitMaxLegPosXY() const;		// 脚の付け根の座標( leg base position)の最小値を計算する


	// 脚番号，x座標，y座標，z座標の順でアクセスすると，その座標が有効かどうかがboolで格納されている
	bool is_able_leg_pos_[HexapodConst::kLegNum][kLegPosDivNum][kLegPosDivNum][kLegPosDivNum];

	//!< 脚の付け根の座標( leg base position)．ロボットの重心を原点，向いている方向をx軸としたローカル(ロボット)座標系である．
	designlab::Vector3 local_leg_base_pos_[HexapodConst::kLegNum];	

	//!< 遊脚する位置．脚座標系
	const std::array<designlab::Vector3, HexapodConst::kLegNum> free_leg_pos_;
};


#endif