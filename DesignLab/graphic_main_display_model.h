
//! @file      graphic_main_display_model.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_GRAPHIC_MAIN_DISPLAY_MODEL_H_
#define DESIGNLAB_GRAPHIC_MAIN_DISPLAY_MODEL_H_

#include <memory>

#include "application_setting_record.h"
#include "divided_map_state.h"
#include "dxlib_3d_renderer_group.h"
#include "dxlib_node_setter_group.h"
#include "dxlib_gui_updater.h"
#include "interface_graphic_main.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_posture_validator.h"
#include "keyboard.h"
#include "map_state.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class GraphicMainDisplayModel
//! @brief ロボットのモデルを表示して，正しく動作しているかを確認するためのクラス．
class GraphicMainDisplayModel final : public IGraphicMain
{
public:
    GraphicMainDisplayModel(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr);

    ~GraphicMainDisplayModel() = default;

    bool Update() override;

    void Draw() const override;

private:
    void MoveBody();

    void MoveLeg();

    std::shared_ptr<Mouse> mouse_ptr_;  //!< マウスの位置を制御するクラス．

    DxlibGuiUpdater gui_updater_;  //!< GUIの更新を行うクラス．
    DxlibNodeSetterGroup node_setter_group_;  //!< ノードの設定を行うクラス．
    Dxlib3dRendererGroup render_group_;  //!< 描画を行うクラス．

    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
    const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;

    MapState map_state_;  //!< マップの状態を保持するクラス．
    DividedMapState divided_map_state_;  //!< 分割したマップの状態を保持するクラス．
    RobotStateNode robot_;  //!< ロボットの状態．

    Keyboard keyboard_;  //!< キーボードの入力を制御するクラス．

    //!< 脚を接地しているデバイドマップのインデックス．
    int divided_map_tile_index_{ 0 };
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPHIC_MAIN_DISPLAY_MODEL_H_
