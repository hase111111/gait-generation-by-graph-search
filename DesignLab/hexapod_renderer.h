//! @file hexapod_renderer.h
//! @brief ロボットの描画を行うクラス．

#ifndef DESIGNLAB_HEXAPOD_RENDERER_H_
#define DESIGNLAB_HEXAPOD_RENDERER_H_


#include <array>
#include <memory>

#include <Dxlib.h>

#include "abstract_hexapod_state_calculator.h"
#include "hexapod_const.h"
#include "robot_state_node.h"


//! @class HexapodRenderer
//! @brief ロボットの描画を行うクラス．
class HexapodRenderer
{
public:
	HexapodRenderer(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr);
	~HexapodRenderer() = default;

	//! @brief ロボットの状態を更新する．
	//! @param [in] node 描画するロボットの状態
	void SetDrawNode(const RobotStateNode& node);

	//! @brief ロボットを3D空間に描画する．
	void Draw() const;

private:

	bool IsAbleCoxaLeg(const designlab::Vector3& coxa_joint, const designlab::Vector3& femur_joint) const;
	bool IsAbleFemurLeg(const designlab::Vector3& femur_joint, const designlab::Vector3& tibia_joint) const;
	bool IsAbleTibiaLeg(const designlab::Vector3& tibia_joint, const designlab::Vector3& leg_joint) const;


	const unsigned int kColorBody;			//!< 胴体の色
	const unsigned int kColorLeg;			//!< 脚の色
	const unsigned int kColorLiftedLeg;		//!< 遊脚している脚の色
	const unsigned int kColorJoint;			//!< ジョイントの色
	const unsigned int kColorLiftedJoint;	//!< 遊脚しているジョイントの色
	const unsigned int kColorLegBase;		//!< 脚の基部の色
	const unsigned int kColorKineLeg;
	const unsigned int kColorKineJoint;
	const unsigned int kColorErrorJoint;	//!< 文字の色
	const unsigned int kColorErrorText;		//!< エラーの文字色

	const int kCapsuleDivNum;	//!< ロボットのモデルの円柱をどれだけ細かく描画するか．4 ～ 20ぐらいがちょうどよいと思う．
	const int kSphereDivNum;	//!< ロボットのモデルの球をどれだけ細かく描画するか．16 ～ 32ぐらいがちょうどよいと思う．
	const float kLegRadius;		//!< 脚の半径．このクラスでは脚を円柱に近似して描画している．描画時のデータのため，これを変化させてもシミュレーションに影響はない．
	const float kJointRadius;	//!< ジョイントの半径．描画時のデータのため，これを変化させてもシミュレーションに影響はない．

	const bool kDoOutputDebugLog = false;	//!< 脚状態を文字列で出力するかどうか


	std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	//!< ロボットの状態を計算するクラス

	RobotStateNode draw_node_;						//!< 描画するロボットの状態

	std::array<HexapodJointState, HexapodConst::kLegNum> draw_joint_state_;	//!< 描画するロボットのジョイントの状態


	// 後でまとめる＆修正
	VECTOR kCoxaJointPos[HexapodConst::kLegNum];
	VECTOR kFemurJointPos[HexapodConst::kLegNum];
	VECTOR kTibiaJointPos[HexapodConst::kLegNum];
	VECTOR kLegEndPos[HexapodConst::kLegNum];
	VECTOR kLegBasePos[HexapodConst::kLegNum];

	float kCoxaCos[HexapodConst::kLegNum];
	float kCoxaSin[HexapodConst::kLegNum];
	float kFemurCos[HexapodConst::kLegNum];
	float kFemurSin[HexapodConst::kLegNum];
	float kTibiaCos[HexapodConst::kLegNum];
	float kTibiaSin[HexapodConst::kLegNum];

	designlab::Vector3 kKineCoxaJointVec[HexapodConst::kLegNum];
	designlab::Vector3 kKineFemurJointVec[HexapodConst::kLegNum];
	designlab::Vector3 kKineTibiaJointVec[HexapodConst::kLegNum];
	designlab::Vector3 kKineLegVec[HexapodConst::kLegNum];

	VECTOR kKineCoxaJointPos[HexapodConst::kLegNum];
	VECTOR kKineFemurJointPos[HexapodConst::kLegNum];
	VECTOR kKineTibiaJointPos[HexapodConst::kLegNum];
	VECTOR kKineLegPos[HexapodConst::kLegNum];

	float kCoxaLinkLength[HexapodConst::kLegNum];
	float kFemurLinkLength[HexapodConst::kLegNum];
	float kTibiaLinkLength[HexapodConst::kLegNum];

	bool kIsAbleCoxaAngle[HexapodConst::kLegNum];
	bool kIsAbleFemurAngle[HexapodConst::kLegNum];
	bool kIsAbleTibiaAngle[HexapodConst::kLegNum];
};


#endif // !DESIGNLAB_HEXAPOD_RENDERER_H_