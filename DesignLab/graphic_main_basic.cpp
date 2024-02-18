
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "graphic_main_basic.h"

#include <Dxlib.h>

#include "camera_dragger.h"
#include "dxlib_gui_camera.h"
#include "dxlib_gui_camera_parameter_displayer.h"
#include "dxlib_gui_node_displayer.h"
#include "dxlib_util.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "map_renderer.h"
#include "world_grid_renderer.h"


namespace designlab
{

GraphicMainBasic::GraphicMainBasic(
  const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr
) :
    kNodeGetCount(setting_ptr->window_fps * 2),
    kInterpolatedAnimeCount(1),
    broker_ptr_(broker_ptr),
    mouse_ptr_(std::make_shared<Mouse>()),
    display_node_switch_gui_(std::make_shared<DxlibGuiDisplayNodeSwitcher>(setting_ptr->window_size_x, setting_ptr->window_size_y)),
    map_renderer_ptr_(std::make_shared<MapRenderer>()),
    movement_locus_renderer_{},
    interpolated_node_creator_{ converter_ptr },
    robot_ground_point_renderer_{ converter_ptr },
    map_state_(broker_ptr->map_state.GetData()),
    graph_({})
{
    if (setting_ptr->gui_display_quality == enums::DisplayQuality::kHigh)
    {
        movement_locus_renderer_.SetIsHighQuality(true);
    }

    const auto camera = std::make_shared<DxlibCamera>();
    const auto camera_gui = std::make_shared<DxlibGuiCamera>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_gui->SetPos(10, 10, designlab::kDxlibGuiAnchorLeftTop, true);
    display_node_switch_gui_->SetPos(10, setting_ptr->window_size_y - 10, designlab::kDxlibGuiAnchorLeftBottom, true);
    const auto camera_parameter_gui = std::make_shared<DxlibGuiCameraParameterDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_parameter_gui->SetPos(10, 10, designlab::kDxlibGuiAnchorLeftTop, true);
    camera_parameter_gui->SetVisible(false);
    const auto node_display_gui = std::make_shared<DxlibGuiNodeDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, converter_ptr, calculator_ptr, checker_ptr);
    node_display_gui->SetPos(setting_ptr->window_size_x - 10, 10, designlab::kDxlibGuiAnchorRightTop, true);
    const auto camera_dragger = std::make_shared<CameraDragger>(camera);

    const auto [hexapod_renderer, hexapod_node_setter] =
        HexapodRendererBuilder::Build(converter_ptr, calculator_ptr, setting_ptr->gui_display_quality);
    const auto stability_margin_renderer = std::make_shared<StabilityMarginRenderer>(converter_ptr);
    const auto world_grid_renderer = std::make_shared<WorldGridRenderer>();

    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(display_node_switch_gui_), DxlibGuiUpdater::kTopPriority);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_parameter_gui), DxlibGuiUpdater::kTopPriority);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_gui), DxlibGuiUpdater::kTopPriority);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(node_display_gui), DxlibGuiUpdater::kTopPriority);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibDraggable>>(camera_dragger), DxlibGuiUpdater::kBottomPriority);

    gui_updater_.OpenTerminal();

    render_group_.Register(map_renderer_ptr_);
    render_group_.Register(hexapod_renderer);
    // render_group_.Register(stability_margin_renderer);
    render_group_.Register(world_grid_renderer);

    node_setter_group_.Register(map_renderer_ptr_);
    node_setter_group_.Register(camera_gui);
    node_setter_group_.Register(hexapod_node_setter);
    node_setter_group_.Register(node_display_gui);
    node_setter_group_.Register(stability_margin_renderer);
}

bool GraphicMainBasic::Update()
{
    mouse_ptr_->Update();

    if (map_update_count != broker_ptr_->map_state.GetUpdateCount())
    {
        map_update_count = broker_ptr_->map_state.GetUpdateCount();
        map_state_ = broker_ptr_->map_state.GetData();

        map_renderer_ptr_->SetMapState(map_state_);
    }


    // ノードを読み出す時間になったら，仲介人からデータを読み出す．
    if (counter_ % kNodeGetCount == 0 && graph_update_count != broker_ptr_->graph.GetUpdateCount())
    {
        // 仲介人からデータを読み出す．
        graph_ = broker_ptr_->graph.GetData();

        std::vector<size_t> simu_end_index;

        simu_end_index = broker_ptr_->simulation_end_index.GetData();

        // ノードの情報を表示するGUIに情報を伝達する．
        display_node_switch_gui_->SetGraphData(graph_.size(), simu_end_index);


        // 移動軌跡を更新する．
        movement_locus_renderer_.SetMoveLocusPoint(graph_);

        movement_locus_renderer_.SetSimulationEndIndexes(simu_end_index);


        // ロボットの接地点を更新する．
        robot_ground_point_renderer_.SetNodeAndSimulationEndNodeIndex(graph_, simu_end_index);


        graph_update_count = broker_ptr_->graph.GetUpdateCount();
    }


    // ノードが存在しているのならば，各クラスに情報を伝達する
    if (!graph_.empty())
    {
        // 表示ノードが更新されたら，表示するノードを変更する．
        if (display_node_index_ != display_node_switch_gui_->GetDisplayNodeNum())
        {
            if (display_node_index_ > 0)
            {
                // 0番目のアニメーションを指定する．
                interpolated_anime_start_count_ = 0;

                interpolated_node_.clear();

                interpolated_node_ = interpolated_node_creator_.CreateInterpolatedNode(
                  graph_[display_node_index_],
                  graph_[display_node_switch_gui_->GetDisplayNodeNum()]);
            }

            // 表示するノードを取得する．
            display_node_index_ = display_node_switch_gui_->GetDisplayNodeNum();
            display_node_switch_gui_->InAnimation(true);

            // ノードの情報を設定するGUIに情報を伝達する．
            node_setter_group_.SetNode(graph_.at(display_node_index_));
        }

        if (0 < interpolated_node_.size() &&
            0 <= interpolated_anime_start_count_ &&
            interpolated_anime_start_count_ < interpolated_node_.size())
        {
            // アニメーション中は interpolated_node_ の補完されたノードを表示する．
            if (counter_ % kInterpolatedAnimeCount == 0)
            {
                interpolated_anime_start_count_ += display_node_switch_gui_->GetAnimeSpeed();
            }

            if (interpolated_anime_start_count_ < interpolated_node_.size())
            {
                node_setter_group_.SetNode(interpolated_node_[interpolated_anime_start_count_]);
            }
        }
        else if (interpolated_node_.size() <= interpolated_anime_start_count_)
        {
            display_node_switch_gui_->InAnimation(false);
            interpolated_anime_start_count_ = -1;
            node_setter_group_.SetNode(graph_.at(display_node_index_));
        }
    }

    ++counter_;  // カウンタを進める．

    gui_updater_.Activate(mouse_ptr_);  // GUIをアクティブにする．

    return true;
}


void GraphicMainBasic::Draw() const
{
    render_group_.Draw();

    // 移動軌跡を描画する．
    movement_locus_renderer_.Draw(display_node_switch_gui_->GetSimulationNum());

    // robot_ground_point_renderer_.Draw(display_node_switch_gui_->GetSimulationNum());

    // 2DのGUIの描画．
    gui_updater_.Draw();
}

}  // namespace designlab
