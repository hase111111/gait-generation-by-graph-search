
/// @author    hasegawa
/// @copyright © 埼玉大学 設計工学研究室 2024. All right reserved.

#include "graphic_main_robot_control.h"

#include "camera_dragger.h"
#include "math_util.h"
#include "dxlib_camera.h"
#include "dxlib_gui_camera.h"
#include "dxlib_gui_camera_parameter_displayer.h"
#include "dxlib_gui_node_displayer.h"
#include "dxlib_gui_robot_control.h"
#include "dxlib_util.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "map_creator_for_simulation.h"
#include "map_renderer.h"
#include "node_initializer.h"
#include "phantomx_mk2_const.h"

namespace designlab
{

GraphicMainRobotControl::GraphicMainRobotControl(const std::shared_ptr<GraphicDataBroker>& broker,
                                                 const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
                                                 const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
                                                 const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
                                                 const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr) :
    mouse_ptr_(std::make_shared<Mouse>()),
    broker_(broker),
    converter_ptr_(converter_ptr),
    calculator_ptr_(calculator_ptr)
{
    assert(broker_ != nullptr);
    assert(converter_ptr_ != nullptr);
    assert(calculator_ptr_ != nullptr);

    NodeInitializer node_initializer{ Vector3{0.f, 0.f, 30.f}, enums::HexapodMove::kNone };
    robot_ = node_initializer.InitNode();

    const SimulationMapParameter map_param;
    MapCreatorForSimulation map_creator(map_param);

    map_state_ = map_creator.InitMap();
    divided_map_state_.Init(map_state_, {});

    const auto camera = std::make_shared<DxlibCamera>();
    const auto camera_gui = std::make_shared<DxlibGuiCamera>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_gui->SetPos(10, 10, kDxlibGuiAnchorLeftTop, true);

    const auto camera_dragger = std::make_shared<CameraDragger>(camera);

    const auto camera_parameter_gui = std::make_shared<DxlibGuiCameraParameterDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
    camera_parameter_gui->SetPos(10, 10, kDxlibGuiAnchorLeftTop, true);
    camera_parameter_gui->SetVisible(false);

    const auto node_display_gui = std::make_shared<DxlibGuiNodeDisplayer>(setting_ptr->window_size_x, setting_ptr->window_size_y, converter_ptr, calculator_ptr, checker_ptr);
    node_display_gui->SetPos(setting_ptr->window_size_x - 10, 10, kDxlibGuiAnchorRightTop, true);

    const auto robot_control_gui = std::make_shared<DxlibGuiRobotControl>(setting_ptr->window_size_x, setting_ptr->window_size_y, converter_ptr, calculator_ptr, checker_ptr);
    robot_control_gui->SetPos(10, setting_ptr->window_size_y - 10, kDxlibGuiAnchorLeftBottom, true);

    const auto [hexapod_renderer, hexapod_node_setter] =
        HexapodRendererBuilder::Build(converter_ptr_, calculator_ptr_, setting_ptr->gui_display_quality);

    const auto map_renderer = std::make_shared<MapRenderer>();
    map_renderer->SetMapState(map_state_);
    map_renderer->SetNode(robot_);

    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_parameter_gui), 1);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibDraggable>>(camera_dragger), 0);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(node_display_gui), 1);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(camera_gui), 1);
    gui_updater_.Register(static_cast<std::shared_ptr<IDxlibGui>>(robot_control_gui), 1);

    gui_updater_.OpenTerminal();

    node_setter_group_.Register(camera_gui);
    node_setter_group_.Register(node_display_gui);
    node_setter_group_.Register(hexapod_node_setter);
    node_setter_group_.Register(map_renderer);

    render_group_.Register(hexapod_renderer);
    render_group_.Register(map_renderer);
}

bool GraphicMainRobotControl::Update()
{
    assert(mouse_ptr_ != nullptr);

    mouse_ptr_->Update();

    gui_updater_.Activate(mouse_ptr_);

    node_setter_group_.SetNode(robot_);

    return true;
}

void GraphicMainRobotControl::Draw() const
{
    render_group_.Draw();

    gui_updater_.Draw();
}

}  // namespace designlab
