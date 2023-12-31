
//! @file      phantomx_mk2_renderer_model.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_PHANTOMX_MK2_RENDERER_MODEL_H_
#define DESIGNLAB_PHANTOMX_MK2_RENDERER_MODEL_H_

#include <array>
#include <memory>

#include <Dxlib.h>

#include "display_quality.h"
#include "hexapod_const.h"
#include "interface_dxlib_3d_renderer.h"
#include "interface_dxlib_node_setter.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class PhantomXMkIIRendererModel
//! @brief 3Dモデルを使用して PhantomX の描画を行うクラス．
//! @todo リネームすること．
class PhantomXMkIIRendererModel final : public IDxlib3dRenderer, public IDxlibNodeSetter
{
public:
    PhantomXMkIIRendererModel(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr);

    void SetNode(const RobotStateNode& node) override;

    void Draw() const override;

private:
    void DrawBody() const;

    void DrawCoxaLink(int leg_index) const;

    void DrawFemurLink(int leg_index) const;

    void DrawTibiaLink(int leg_index) const;

    void DrawJointAxis(int leg_index) const;

    //! ロボットの脚先座標を計算するクラス．
    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;

    //! ロボットの間接位置を計算するクラス．
    const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;

    //! 描画するロボットの状態．
    RobotStateNode draw_node_;

    //! 描画するロボットのジョイントの状態．
    std::array<HexapodJointState, HexapodConst::kLegNum> draw_joint_state_;
    enums::DisplayQuality display_quality_;  //!< 描画品質．
};

}  // namespace designlab


#endif  // DESIGNLAB_PHANTOMX_MK2_RENDERER_MODEL_H_
