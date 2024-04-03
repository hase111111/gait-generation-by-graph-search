
//! @file      graphic_main_basic.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_GRAPHIC_MAIN_BASIC_H_
#define DESIGNLAB_GRAPHIC_MAIN_BASIC_H_

#include <memory>
#include <vector>

#include "application_setting_record.h"
#include "dxlib_gui_display_node_switcher.h"
#include "dxlib_3d_renderer_group.h"
#include "dxlib_gui_updater.h"
#include "dxlib_node_setter_group.h"
#include "graphic_const.h"
#include "graphic_data_broker.h"
#include "interface_graphic_main.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_posture_validator.h"
#include "interpolated_node_creator.h"
#include "map_state.h"
#include "map_renderer.h"
#include "movement_locus_renderer.h"
#include "mouse.h"
#include "robot_state_node.h"
#include "robot_ground_point_renderer.h"
#include "stability_margin_renderer.h"


namespace designlab
{

//! @class GraphicMainBasic
//! @brief シミュレーションの一覧表示，ロボットの動きの軌跡の表示
//! などベーシックな機能を提供するクラス．
class GraphicMainBasic final : public IGraphicMain
{
public:
    GraphicMainBasic() = delete;

    GraphicMainBasic(
      const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr);

    ~GraphicMainBasic() = default;

    bool Update() override;

    void Draw() const override;

private:
    const int kNodeGetCount;  //!< ノードを取得する間隔．

    const int kInterpolatedAnimeCount;  //!< 補間されたノードの表示を切り替える間隔．


    const std::shared_ptr<const GraphicDataBroker> broker_ptr_;  //!< データを受け取るクラス．

    std::shared_ptr<Mouse> mouse_ptr_;  //!< マウスの情報を取得するクラス．

    DxlibGuiUpdater gui_updater_;  //!< GUIをアクティブにするクラス．
    Dxlib3dRendererGroup render_group_;  //!< 描画を行うクラス．
    DxlibNodeSetterGroup node_setter_group_;  //!< ノードを設定するクラス．

    //!< ノードの表示を切り替えるGUI．
    std::shared_ptr<DxlibGuiDisplayNodeSwitcher> display_node_switch_gui_;

    //!< ロボットの動きの軌跡を表示するクラス．
    MovementLocusRenderer movement_locus_renderer_;

    //!< ロボットの足先の位置を表示するクラス．
    RobotGroundPointRenderer robot_ground_point_renderer_;

    //!< マップを表示するクラス．
    std::shared_ptr<MapRenderer> map_renderer_ptr_;

    InterpolatedNodeCreator interpolated_node_creator_;  //!< ノード間を補間するクラス．


    MapState map_state_;  //!< 表示するマップ．

    std::vector<RobotStateNode> graph_;  //!< ロボットの動きの遷移を記録する配列．

    size_t display_node_index_{ 0 };  //!< 描画しているノード．

    int counter_{ 0 };  //!< このクラスが実行されてから何回 update 関数が呼ばれたかカウントする．

    std::vector<RobotStateNode> interpolated_node_;  //!< 補間されたノードを記録する配列．

    int interpolated_anime_start_count_{ 0 };  //!< 補間されたノードを表示し始めるカウント．


    int graph_update_count{ 0 };  //!< 処理部でのグラフの更新回数．
    int map_update_count{ 0 };  //!< 処理部でのマップの更新回数．
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPHIC_MAIN_BASIC_H_
