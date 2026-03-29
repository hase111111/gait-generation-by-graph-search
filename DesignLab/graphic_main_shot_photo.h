
//! @file graphic_main_graph_viewer.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>
#include <vector>

#include "dxlib_3d_renderer_group.h"
#include "dxlib_gui_updater.h"
#include "dxlib_node_setter_group.h"
#include "graph_viewer_gui_controller.h"
#include "graphic_data_broker.h"
#include "interface_graphic_main.h"
#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_posture_validator.h"
#include "map_renderer.h"
#include "mouse.h"
#include "phantomx_mk2_renderer_model.h"
#include "phantomx_renderer_simple.h"

namespace gaitgen {

//! @class GraphicMainShotPhoto
class GraphicMainShotPhoto final : public IGraphicMain {
 public:
  GraphicMainShotPhoto(
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
      const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr);

  ~GraphicMainShotPhoto() = default;

  bool Update() override;

  void Draw() const override;

 private:
  std::vector<std::string> InitResultDirectoryPaths() const;
  void LoadGraphFromFile(const std::string& file_path);

  const std::shared_ptr<Mouse> mouse_ptr_;
  const std::vector<std::string> result_directory_paths_;
  std::shared_ptr<MapRenderer> map_renderer_ptr_;

  Keyboard keyboard_;  //!< キーボードの入力を制御するクラス.

  const std::unique_ptr<GraphViewerGUIController> gui_controller_ptr_;

  Dxlib3dRendererGroup renderer_group_;
  DxlibGuiUpdater gui_updater_;
  DxlibNodeSetterGroup node_setter_group_;

  PhantomXMkIIRendererModel phantomx_mk2_renderer_model_;
  PhantomXRendererSimple phantomx_renderer_simple_;

  std::vector<RobotStateNode> graph_;

  int counter_{0};
};

}  // namespace gaitgen
