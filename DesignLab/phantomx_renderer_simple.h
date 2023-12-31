
//! @file      phantomx_renderer_simple.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_PHANTOMX_RENDERER_SIMPLE_H_
#define DESIGNLAB_PHANTOMX_RENDERER_SIMPLE_H_

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

//! @class PhantomXRendererSimple
//! @brief 3Dモデルを使用せず，多角形を組み合わせてPhantomXの描画を行うクラス．
class PhantomXRendererSimple final : public IDxlib3dRenderer, public IDxlibNodeSetter
{
public:
    PhantomXRendererSimple(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      enums::DisplayQuality display_quality);

    void SetNode(const RobotStateNode& node) override;

    void Draw() const override;

private:
    //! @brief 通常通りにロボットの描画をする
    void DrawHexapodNormal() const;

    const unsigned int kColorBody;          //!< 胴体の色．
    const unsigned int kColorLeg;           //!< 脚の色．
    const unsigned int kColorLiftedLeg;     //!< 遊脚している脚の色．
    const unsigned int kColorJoint;         //!< ジョイントの色．
    const unsigned int kColorLiftedJoint;   //!< 遊脚しているジョイントの色．
    const unsigned int kColorLegBase;       //!< 脚の基部の色．
    const unsigned int kColorErrorJoint;    //!< 文字の色．
    const unsigned int kColorErrorText;     //!< エラーの文字色．

    //! ロボットのモデルの円柱をどれだけ細かく描画するか．4 ～ 20ぐらいがちょうどよい．
    const int kCapsuleDivNum;

    //! ロボットのモデルの球をどれだけ細かく描画するか．16 ～ 32ぐらいがちょうどよい．
    const int kSphereDivNum;

    //! 脚の半径．このクラスでは脚を円柱に近似して描画している．
    //! 描画時のデータのため，これを変化させてもシミュレーションに影響はない．
    const float kLegRadius;

    //!< ジョイントの半径．描画時のデータのため，これを変化させてもシミュレーションに影響はない．
    const float kJointRadius;

    const bool kDoOutputDebugLog = false;  //!< 脚状態を文字列で出力するかどうか


    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
    const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;

    RobotStateNode draw_node_;  //!< 描画するロボットの状態

    //! 描画するロボットのジョイントの状態．
    std::array<HexapodJointState, HexapodConst::kLegNum> draw_joint_state_;

    enums::DisplayQuality display_quality_;  //!< 描画品質．
};

}  // namespace designlab


#endif  // DESIGNLAB_PHANTOMX_RENDERER_SIMPLE_H_
