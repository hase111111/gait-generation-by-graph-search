
//! @file graphic_main_basic.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

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
#include "stopwatch.h"
#include "world_grid_renderer.h"

namespace gaitgen {

GraphicMainBasic::GraphicMainBasic(
    const std::shared_ptr<const GraphicDataBroker>& broker_ptr,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
    const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
    const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr,
    const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr)
    : kNodeGetCount(setting_ptr->window_fps * 2),
      kInterpolatedAnimeCount(1),
      broker_ptr_(broker_ptr),
      mouse_ptr_(std::make_shared<Mouse>()),
      display_node_switch_gui_(std::make_shared<DxlibGuiDisplayNodeSwitcher>(
          setting_ptr->window_size_x, setting_ptr->window_size_y)),
      map_renderer_ptr_(std::make_shared<MapRenderer>(
          setting_ptr->draw_flat_ground, setting_ptr->draw_robot_area)),
      movement_locus_renderer_{},
      interpolated_node_creator_{converter_ptr},
      robot_ground_point_renderer_{converter_ptr},
      map_state_(broker_ptr->map_state.GetData()),
      graph_({}) {
  if (setting_ptr->gui_display_quality == DisplayQuality::kHigh) {
    movement_locus_renderer_.SetIsHighQuality(true);
  }

  dxlib_camera_ptr_ = std::make_shared<DxlibCamera>();
  const auto camera_gui = std::make_shared<DxlibGuiCamera>(
      setting_ptr->window_size_x, setting_ptr->window_size_y,
      dxlib_camera_ptr_);
  camera_gui->SetPos(10, 10, gaitgen::kDxlibGuiAnchorLeftTop, true);
  display_node_switch_gui_->SetPos(10, setting_ptr->window_size_y - 10,
                                   gaitgen::kDxlibGuiAnchorLeftBottom, true);
  const auto camera_parameter_gui =
      std::make_shared<DxlibGuiCameraParameterDisplayer>(
          setting_ptr->window_size_x, setting_ptr->window_size_y,
          dxlib_camera_ptr_);
  camera_parameter_gui->SetPos(10, 10, gaitgen::kDxlibGuiAnchorLeftTop, true);
  camera_parameter_gui->SetVisible(false);
  const auto node_display_gui = std::make_shared<DxlibGuiNodeDisplayer>(
      setting_ptr->window_size_x, setting_ptr->window_size_y, converter_ptr,
      calculator_ptr, checker_ptr);
  node_display_gui->SetPos(setting_ptr->window_size_x - 10, 10,
                           gaitgen::kDxlibGuiAnchorRightTop, true);
  const auto camera_dragger =
      std::make_shared<CameraDragger>(dxlib_camera_ptr_);

  const auto [hexapod_renderer, hexapod_node_setter] =
      HexapodRendererBuilder::Build(converter_ptr, calculator_ptr,
                                    setting_ptr->gui_display_quality);
  const auto stability_margin_renderer =
      std::make_shared<StabilityMarginRenderer>(converter_ptr);
  const auto world_grid_renderer = std::make_shared<WorldGridRenderer>();

  gui_updater_.Register(display_node_switch_gui_,
                        DxlibGuiUpdater::kTopPriority);
  gui_updater_.Register(camera_parameter_gui, DxlibGuiUpdater::kTopPriority);
  gui_updater_.Register(camera_gui, DxlibGuiUpdater::kTopPriority);
  gui_updater_.Register(node_display_gui, DxlibGuiUpdater::kTopPriority);
  gui_updater_.Register(camera_dragger, DxlibGuiUpdater::kBottomPriority);

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

bool GraphicMainBasic::Update() {
  mouse_ptr_->Update();

  // G キーが押されたら,カメラパラメータ表示GUIの表示・非表示を切り替える.
  keyboard_.Update();
  if (keyboard_.GetPressingCount(KEY_INPUT_G) == 1) {
    draw_gui_ = !draw_gui_;
  }

  // C キーが押されたら,カメラの位置と注視点をリセットする.
  if (keyboard_.GetPressingCount(KEY_INPUT_C) == 1) {
    dxlib_camera_ptr_->SetCameraQuat(
        Quaternion{0.693f, 0.140f, -0.144f, 0.692f}.GetNormalized());
    dxlib_camera_ptr_->SetTargetPos({821.f, -53.f, 169.f});
    dxlib_camera_ptr_->SetCameraViewMode(
        enums::CameraViewMode::kFreeControlledAndMovableTarget);
    dxlib_camera_ptr_->AddCameraToTargetLength(-1000000);
    dxlib_camera_ptr_->AddCameraToTargetLength(2500);
  }

  // ctrl + P キーが押されたら,スクリーンショットを保存する.
  if (keyboard_.GetPressingCount(KEY_INPUT_P) == 1 &&
      keyboard_.GetPressingCount(KEY_INPUT_LCONTROL) > 0) {
    // 時間を取得して,ファイル名に使う.
    const std::string name =
        "shot/screenshot" + Stopwatch{}.GetNowTimeString() + ".png";

    DxLib::SaveDrawScreenToPNG(0, 0, 1280, 720, name.c_str());
  }

  if (map_update_count != broker_ptr_->map_state.GetUpdateCount()) {
    map_update_count = broker_ptr_->map_state.GetUpdateCount();
    map_state_ = broker_ptr_->map_state.GetData();

    map_renderer_ptr_->SetMapState(map_state_);
  }

  // ノードを読み出す時間になったら,仲介人からデータを読み出す.
  if (counter_ % kNodeGetCount == 0 &&
      graph_update_count != broker_ptr_->graph.GetUpdateCount()) {
    // 仲介人からデータを読み出す.
    graph_ = broker_ptr_->graph.GetData();

    std::vector<size_t> simu_end_index;

    simu_end_index = broker_ptr_->simulation_end_index.GetData();

    // ノードの情報を表示するGUIに情報を伝達する.
    display_node_switch_gui_->SetGraphData(graph_.size(), simu_end_index);

    // 移動軌跡を更新する.
    movement_locus_renderer_.SetMoveLocusPoint(graph_);

    movement_locus_renderer_.SetSimulationEndIndexes(simu_end_index);

    // ロボットの接地点を更新する.
    robot_ground_point_renderer_.SetNodeAndSimulationEndNodeIndex(
        graph_, simu_end_index);

    graph_update_count = broker_ptr_->graph.GetUpdateCount();
  }

  // ノードが存在しているのならば,各クラスに情報を伝達する
  if (!graph_.empty()) {
    // 表示ノードが更新されたら,表示するノードを変更する.
    if (display_node_index_ != display_node_switch_gui_->GetDisplayNodeNum()) {
      if (display_node_index_ > 0) {
        // 0番目のアニメーションを指定する.
        interpolated_anime_start_count_ = 0;

        interpolated_node_.clear();

        interpolated_node_ = interpolated_node_creator_.CreateInterpolatedNode(
            graph_[display_node_index_],
            graph_[display_node_switch_gui_->GetDisplayNodeNum()]);
      }

      // 表示するノードを取得する.
      display_node_index_ = display_node_switch_gui_->GetDisplayNodeNum();
      display_node_switch_gui_->InAnimation(true);

      // ノードの情報を設定するGUIに情報を伝達する.
      node_setter_group_.SetNode(graph_.at(display_node_index_));
    }

    if (0 < interpolated_node_.size() && 0 <= interpolated_anime_start_count_ &&
        interpolated_anime_start_count_ < interpolated_node_.size()) {
      // アニメーション中は interpolated_node_ の補完されたノードを表示する.
      if (counter_ % kInterpolatedAnimeCount == 0) {
        interpolated_anime_start_count_ +=
            display_node_switch_gui_->GetAnimeSpeed();
      }

      if (interpolated_anime_start_count_ < interpolated_node_.size()) {
        node_setter_group_.SetNode(
            interpolated_node_[interpolated_anime_start_count_]);
      }
    } else if (interpolated_node_.size() <= interpolated_anime_start_count_) {
      display_node_switch_gui_->InAnimation(false);
      interpolated_anime_start_count_ = -1;
      node_setter_group_.SetNode(graph_.at(display_node_index_));
    }
  }

  ++counter_;  // カウンタを進める.

  gui_updater_.Activate(mouse_ptr_);  // GUIをアクティブにする.

  return true;
}

void GraphicMainBasic::Draw() const {
  // 背景色をDrawBoxで直接描画することで,スクリーンショットに背景色が乗らないバグに対処する．
  DxLib::DrawBox(
      0, 0, 1280, 720,
      GetColor(GraphicConst::kBackColorRed, GraphicConst::kBackColorBlue,
               GraphicConst::kBackColorGreen),
      TRUE);

  render_group_.Draw();

  // 移動軌跡を描画する.
  movement_locus_renderer_.Draw(display_node_switch_gui_->GetSimulationNum());

  robot_ground_point_renderer_.Draw(
      display_node_switch_gui_->GetSimulationNum());

  // 2DのGUIの描画.
  if (draw_gui_) {
    gui_updater_.Draw();
  }
}

}  // namespace gaitgen
