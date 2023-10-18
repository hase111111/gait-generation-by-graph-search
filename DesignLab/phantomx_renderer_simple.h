//! @file phantomx_renderer_simple.h
//! @brief ロボットの描画を行うクラス．

#ifndef DESIGNLAB_PHANTOMX_RENDERER_SIMPLE_H_
#define DESIGNLAB_PHANTOMX_RENDERER_SIMPLE_H_


#include <array>
#include <memory>

#include "abstract_hexapod_state_calculator.h"
#include "define.h"
#include "display_quality.h"
#include "hexapod_const.h"
#include "robot_state_node.h"

#ifndef DESIGNLAB_DONOT_USE_DXLIB

#include <Dxlib.h>

#include "interface_hexapod_renderer.h"


//! @class PhantomXRendererSimple
//! @brief PhantomXの描画を行うクラス．3Dモデルを使用せず，多角形を組み合わせてロボットを描画する．
class PhantomXRendererSimple final : public IHexapodRenderer
{
public:

	PhantomXRendererSimple(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr, DisplayQuality display_quality);

	void SetDrawNode(const RobotStateNode& node) override;

	void Draw() const override;

private:

	//! @brief 通常通りにロボットの描画をする
	void DrawHexapodNormal() const;

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

	DisplayQuality display_quality_;	//!< 描画品質
};

#endif

#endif