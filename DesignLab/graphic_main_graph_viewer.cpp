
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "graphic_main_graph_viewer.h"

#include <memory>

#include "cassert_define.h"
#include "camera_dragger.h"
#include "dxlib_gui_camera.h"
#include "dxlib_gui_camera_parameter_displayer.h"
#include "dxlib_gui_node_displayer.h"
#include "dxlib_util.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "map_renderer.h"
#include "node_initializer.h"
#include "world_grid_renderer.h"


namespace designlab
{

GraphicMainGraphViewer::GraphicMainGraphViewer(
  const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
  const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
  const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
  const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
  const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr) :
    broker_ptr_(broker_ptr),
    mouse_ptr_(std::make_shared<Mouse>()),
    graph_({}),
    gui_controller_ptr_(std::make_unique<GraphViewerGUIController>(&graph_, &display_node_index_, setting_ptr))
{
    assert(broker_ptr_ != nullptr);

    // 適当なノードを生成して，描画クラスを初期化する
    NodeInitializer node_initializer{ Vector3{0.f, 0.f, 30.f}, EulerXYZ(), enums::HexapodMove::kNone };
    RobotStateNode init_node = node_initializer.InitNode();


    const auto camera = std::make_shared<DxlibCamera>();

    const auto camera_gui = std::make_shared<DxlibGuiCamera>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_gui->SetPos(10, setting_ptr->window_size_y - 10, kDxlibGuiAnchorLeftBottom, true);

    const auto camera_dragger = std::make_shared<CameraDragger>(camera);

    const auto camera_parameter_gui = std::make_shared<DxlibGuiCameraParameterDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_parameter_gui->SetPos(10, 10, kDxlibGuiAnchorLeftTop, true);
    camera_parameter_gui->SetVisible(false);

    const auto node_display_gui = std::make_shared<DxlibGuiNodeDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, converter_ptr, calculator_ptr, checker_ptr);
    node_display_gui->SetPos(setting_ptr->window_size_x - 10, 10, kDxlibGuiAnchorRightTop, true);

    const auto [hexapod_renderer_, hexapod_node_setter] =
        HexapodRendererBuilder::Build(converter_ptr, calculator_ptr, setting_ptr->gui_display_quality);

    const auto map_renderer = std::make_shared<MapRenderer>();
    map_renderer->SetMapState(broker_ptr->map_state.GetData());

    const auto world_grid_renderer = std::make_shared<WorldGridRenderer>();


    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibDraggable>>(camera_dragger), DxlibGuiUpdater::kBottomPriority);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_parameter_gui), DxlibGuiUpdater::kTopPriority);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_gui), DxlibGuiUpdater::kTopPriority);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(node_display_gui), DxlibGuiUpdater::kTopPriority);

    gui_updater_.OpenTerminal();

    renderer_group_.Register(hexapod_renderer_);
    renderer_group_.Register(map_renderer);
    renderer_group_.Register(world_grid_renderer);

    node_setter_group_.Register(camera_gui);
    node_setter_group_.Register(node_display_gui);
    node_setter_group_.Register(hexapod_node_setter);
    node_setter_group_.Register(map_renderer);
}


bool GraphicMainGraphViewer::Update()
{
    mouse_ptr_->Update();

    gui_controller_ptr_->Update();

    if (graph_update_count_ != broker_ptr_->graph.GetUpdateCount())
    {
        graph_.clear();  // グラフを初期化する．

        graph_ = broker_ptr_->graph.GetData();  // データを更新する．

        // グラフの中身が空でないならば，表示するノードを初期化する．
        if (!graph_.empty())
        {
            display_node_index_ = 0;
        }

        gui_controller_ptr_->UpdateGraphNodeDepthData();

        graph_update_count_ = broker_ptr_->graph.GetUpdateCount();
    }

    if (display_node_index_ < graph_.size() && graph_.size() != 0)
    {
        node_setter_group_.SetNode(graph_.at(display_node_index_));
    }

    gui_updater_.Activate(mouse_ptr_);

    return true;
}


void GraphicMainGraphViewer::Draw() const
{
    renderer_group_.Draw();

    gui_controller_ptr_->Draw();

    gui_updater_.Draw();
}

}  // namespace designlab
