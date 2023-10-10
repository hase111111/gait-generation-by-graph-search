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


	const unsigned int COLOR_BODY;			// 胴体の色
	const unsigned int COLOR_LEG;			// 脚の色
	const unsigned int COLOR_LIFTED_LEG;	// 遊脚している脚の色
	const unsigned int COLOR_JOINT;			// ジョイントの色
	const unsigned int COLOR_LIFTED_JOINT;	// 遊脚しているジョイントの色
	const unsigned int COLOR_LEG_BASE;		// 脚の基部の色
	const unsigned int COLOR_KINE_LEG;
	const unsigned int COLOR_KINE_JOINT;
	const unsigned int COLOR_ERROR_JOINT;	// 文字の色
	const unsigned int COLOR_ERROR_TEXT;	// エラーの文字色

	const int CAPSULE_DIV_NUM;				// ロボットのモデルの円柱をどれだけ細かく描画するか．4 ～ 20ぐらいがちょうどよいと思う．
	const int SPHERE_DIV_NUM;				// ロボットのモデルの球をどれだけ細かく描画するか．16 ～ 32ぐらいがちょうどよいと思う．
	const float LEG_R = 10.0f;				// 脚の半径．このクラスでは脚を円柱に近似して描画している．描画時のデータのため，これを変化させてもシミュレーションに影響はない．
	const float JOINT_R = 20.0f;			// ジョイントの半径．描画時のデータのため，これを変化させてもシミュレーションに影響はない．

	const bool DO_OUTPUT_DEBUG_LOG = false;	// 脚状態を文字列で出力するかどうか


	std::shared_ptr<const AbstractHexapodStateCalculator> calculator_ptr_;	// ロボットの状態を計算するクラス

	RobotStateNode draw_node_;						// 描画するロボットの状態

	std::array<HexapodJointState, HexapodConst::LEG_NUM> draw_joint_state_;	// 描画するロボットのジョイントの状態


	// 後でまとめる＆修正
	VECTOR kCoxaJointPos[HexapodConst::LEG_NUM];
	VECTOR kFemurJointPos[HexapodConst::LEG_NUM];
	VECTOR kTibiaJointPos[HexapodConst::LEG_NUM];
	VECTOR kLegEndPos[HexapodConst::LEG_NUM];
	VECTOR kLegBasePos[HexapodConst::LEG_NUM];

	float kCoxaCos[HexapodConst::LEG_NUM];
	float kCoxaSin[HexapodConst::LEG_NUM];
	float kFemurCos[HexapodConst::LEG_NUM];
	float kFemurSin[HexapodConst::LEG_NUM];
	float kTibiaCos[HexapodConst::LEG_NUM];
	float kTibiaSin[HexapodConst::LEG_NUM];

	designlab::Vector3 kKineCoxaJointVec[HexapodConst::LEG_NUM];
	designlab::Vector3 kKineFemurJointVec[HexapodConst::LEG_NUM];
	designlab::Vector3 kKineTibiaJointVec[HexapodConst::LEG_NUM];
	designlab::Vector3 kKineLegVec[HexapodConst::LEG_NUM];

	VECTOR kKineCoxaJointPos[HexapodConst::LEG_NUM];
	VECTOR kKineFemurJointPos[HexapodConst::LEG_NUM];
	VECTOR kKineTibiaJointPos[HexapodConst::LEG_NUM];
	VECTOR kKineLegPos[HexapodConst::LEG_NUM];

	float kCoxaLinkLength[HexapodConst::LEG_NUM];
	float kFemurLinkLength[HexapodConst::LEG_NUM];
	float kTibiaLinkLength[HexapodConst::LEG_NUM];

	bool kIsAbleCoxaAngle[HexapodConst::LEG_NUM];
	bool kIsAbleFemurAngle[HexapodConst::LEG_NUM];
	bool kIsAbleTibiaAngle[HexapodConst::LEG_NUM];
};


#endif // !DESIGNLAB_HEXAPOD_RENDERER_H_