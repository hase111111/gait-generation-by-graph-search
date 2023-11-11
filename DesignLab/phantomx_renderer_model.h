//! @file phantomx_renderer_model.h
//! @brief ロボットの描画を行うクラス．

#ifndef DESIGNLAB_PHANTOMX_RENDERER_MODEL_H_
#define DESIGNLAB_PHANTOMX_RENDERER_MODEL_H_


#include <array>
#include <memory>

#include <Dxlib.h>

#include "define.h"
#include "display_quality.h"
#include "hexapod_const.h"
#include "interface_hexapod_renderer.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "robot_state_node.h"


#ifndef DESIGNLAB_DONOT_USE_DXLIB


//! @class PhantomXRendererModel
//! @brief PhantomXの描画を行うクラス．3Dモデルを使用する
class PhantomXRendererModel final : public IHexapodRenderer
{
public:

	PhantomXRendererModel(
		const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
		const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr
	);

	void SetDrawNode(const RobotStateNode& node) override;

	void Draw() const override;

private:

	void DrawBody() const;

	void DrawCoxaLink(int leg_index) const;

	void DrawFemurLink(int leg_index) const;

	void DrawTibiaLink(int leg_index) const;

	void DrawJointAxis(int leg_index) const;

	const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;	//!< ロボットの脚先座標を計算するクラス
	const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;	//!< ロボットの間接位置を計算するクラス

	RobotStateNode draw_node_;	//!< 描画するロボットの状態
	std::array<HexapodJointState, HexapodConst::kLegNum> draw_joint_state_;	//!< 描画するロボットのジョイントの状態
	DisplayQuality display_quality_;	//!< 描画品質
};

#endif	// #ifndef DESIGNLAB_DONOT_USE_DXLIB

#endif	// #ifndef DESIGNLAB_PHANTOMX_RENDERER_MODEL_H_