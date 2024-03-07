
//! @file      dxlib_gui_node_displayer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_DXLIB_GUI_NODE_DISPLAYER_H_
#define DESIGNLAB_DXLIB_GUI_NODE_DISPLAYER_H_

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "abstract_dxlib_gui.h"
#include "interface_dxlib_node_setter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_posture_validator.h"
#include "robot_state_node.h"
#include "simple_button.h"


namespace designlab
{

//! @class DxlibGuiNodeDisplayer
//! @brief ノードの情報を表示するGUIのクラス．
class DxlibGuiNodeDisplayer final :
    public AbstractDxlibGui,
    public IDxlibNodeSetter
{
public:
    DxlibGuiNodeDisplayer(
      int window_x,
      int window_y,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr);

    void SetNode(const RobotStateNode& node) override;

    void Update() override;

    void Draw() const override;

private:
    enum class DisplayMode : int
    {
        kDefualt,       //!< デフォルト
        kJointState,    //!< 角度
        kGlobalPos,     //!< グローバル座標
    };

    void DrawNodeInfo() const;

    void DrawJointInfo() const;

    void DrawGlobalPosInfo() const;

    bool IsInWindow() const;

    //! 座標変換を行うクラス．
    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;

    //! 六脚歩行ロボットの状態を計算するクラス．
    const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;
    const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;

    RobotStateNode display_node_;  //!< 表示するノード．

    //! 関節の角度．
    std::array<HexapodJointState, HexapodConst::kLegNum> joint_state_;
    DisplayMode display_type_;  //!< 表示する情報の種類．

    //! GUIの右下座標．
    const int window_x_, window_y_;
};

}  // namespace designlab


#endif  // DESIGNLAB_DXLIB_GUI_NODE_DISPLAYER_H_
