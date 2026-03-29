
#include "graphic_main_shot_photo.h"

#include <numbers>

#include "approximated_motion_range_render.h"
#include "camera_dragger.h"
#include "dxlib_camera.h"
#include "dxlib_gui_camera.h"
#include "dxlib_gui_camera_parameter_displayer.h"
#include "dxlib_gui_node_displayer.h"
#include "dxlib_util.h"
#include "graphic_main_display_model.h"
#include "hexapod_renderer_builder.h"
#include "keyboard.h"
#include "map_creator_selector.h"
#include "math_util.h"
#include "node_initializer.h"
#include "phantomx_mk2_const.h"
#include "result_file_exporter.h"
#include "result_file_importer.h"
#include "simulation_setting_record.h"
#include "toml_file_importer.h"
#include "world_grid_renderer.h"

namespace gaitgen {

GraphicMainShotPhoto::GraphicMainShotPhoto(
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
    const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr)
    : mouse_ptr_(std::make_shared<Mouse>()),
      result_directory_paths_(InitResultDirectoryPaths()),
      phantomx_mk2_renderer_model_{converter_ptr, calculator_ptr},
      phantomx_renderer_simple_{converter_ptr, calculator_ptr,
                                DisplayQuality::kMedium} {
  const auto camera = std::make_shared<DxlibCamera>();
  const auto camera_gui = std::make_shared<DxlibGuiCamera>(
      setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
  camera_gui->SetPos(10, 10, kDxlibGuiAnchorLeftTop, true);
  const auto camera_dragger = std::make_shared<CameraDragger>(camera);

  const auto camera_parameter_gui =
      std::make_shared<DxlibGuiCameraParameterDisplayer>(
          setting_ptr->window_size_x, setting_ptr->window_size_y, camera);
  camera_parameter_gui->SetPos(10, 10, kDxlibGuiAnchorLeftTop, true);
  camera_parameter_gui->SetVisible(false);

  const auto node_display_gui = std::make_shared<DxlibGuiNodeDisplayer>(
      setting_ptr->window_size_x, setting_ptr->window_size_y, converter_ptr,
      calculator_ptr, checker_ptr);
  node_display_gui->SetPos(setting_ptr->window_size_x - 10, 10,
                           kDxlibGuiAnchorRightTop, true);

  map_renderer_ptr_ = std::make_shared<MapRenderer>(
      setting_ptr->draw_flat_ground, setting_ptr->draw_robot_area);

  const auto world_grid_renderer = std::make_shared<WorldGridRenderer>();

  gui_updater_.Register(camera_parameter_gui, 1);
  gui_updater_.Register(camera_dragger, 0);
  gui_updater_.Register(node_display_gui, 1);
  gui_updater_.Register(camera_gui, 1);

  gui_updater_.OpenTerminal();

  node_setter_group_.Register(camera_gui);
  node_setter_group_.Register(node_display_gui);
  node_setter_group_.Register(map_renderer_ptr_);

  renderer_group_.Register(map_renderer_ptr_);
  renderer_group_.Register(world_grid_renderer);

  // 最初のデータをロードする．
  LoadGraphFromFile(result_directory_paths_[0]);
}

bool GraphicMainShotPhoto::Update() {
  assert(mouse_ptr_ != nullptr);

  counter_++;

  if (counter_ % 60 == 0) {
    LoadGraphFromFile(result_directory_paths_[counter_ / 60]);
  }

  mouse_ptr_->Update();

  gui_updater_.Activate(mouse_ptr_);

  // node_setter_group_.SetNode(robot_);

  keyboard_.Update();

  return true;
}

void GraphicMainShotPhoto::Draw() const {
  renderer_group_.Draw();

  gui_updater_.Draw();
}

std::vector<std::string> GraphicMainShotPhoto::InitResultDirectoryPaths()
    const {
  std::vector<std::string> paths;

  // result ディレクトリ以下にある全てのディレクトリのパスを取得する.
  const auto result_directory_path = ResultFileConst::kDirectoryPath;

  // ディレクトリが存在するかチェック
  if (!std::filesystem::exists(result_directory_path)) {
    return paths;
  }

  // result 配下を走査
  for (const auto& entry :
       std::filesystem::directory_iterator(result_directory_path)) {
    // ディレクトリのみ取得
    if (entry.is_directory()) {
      paths.push_back(entry.path().string());
    }
  }

  return paths;
}

void GraphicMainShotPhoto::LoadGraphFromFile(const std::string& file_path) {
  // file_path 直下にある node_list1.csv というファイルを読み込む．
  const ResultFileImporter result_importer;
  std::vector<RobotStateNode> graph;
  MapState map_state;
  cmdio::SystemOutputF("Loading data from the file: {}", file_path);

  if (result_importer.ImportNodeListAndMapState(file_path + "/node_list1.csv",
                                                &graph, &map_state)) {
    cmdio::SystemOutput("Data loaded successfully.");
    map_renderer_ptr_->SetMapState(map_state);
  } else {
    cmdio::SystemOutput("Failed to read the file. Exit.");
  }
}

}  // namespace gaitgen
