
//! @file main.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include <boost/thread.hpp>
#include <memory>

#include "test_main.h"

#if !defined(DESIGNLAB_USE_TEST)

#include "application_setting_record.h"
#include "application_setting_record_validator.h"
#include "boot_mode_selector.h"
#include "cmdio_util.h"
#include "gpg_selector.h"
#include "graphic_main_basic.h"
#include "graphic_main_display_model.h"
#include "graphic_main_graph_viewer.h"
#include "graphic_main_robot_control.h"
#include "graphic_system.h"
#include "map_creator_selector.h"
#include "node_creator_sequence_body_rot.h"
#include "phantomx_mk2.h"
#include "robot_operator_factory.h"
#include "simulation_end_checker_factory.h"
#include "system_main_continuous_simulation.h"
#include "system_main_create_map_csv.h"
#include "system_main_graph_viewer.h"
#include "system_main_result_viewer.h"
#include "system_main_robot_control.h"
#include "system_main_simulation.h"
#include "toml_directory_exporter.h"
#include "toml_file_importer.h"
#include "xr_r1.h"

// このプロジェクトがコンパイルされない場合はソリューションエクスプローラーから
// DesignLabを右クリック →「スタートアッププロジェクトに設定」を選択.

namespace {

auto LoadApplicationSettingRecord() {
  using enum gaitgen::OutputDetail;
  using gaitgen::ApplicationSettingRecord;
  using gaitgen::ApplicationSettingRecordValidator;
  using gaitgen::TomlDirectoryExporter;
  using gaitgen::TomlFileImporter;

  gaitgen::cmdio::SetOutputLimit(kSystem);

  TomlDirectoryExporter toml_directory_exporter;
  toml_directory_exporter.Export();

  TomlFileImporter<ApplicationSettingRecord> application_setting_importer(
      std::make_unique<ApplicationSettingRecordValidator>());

  return std::make_shared<const ApplicationSettingRecord>(
      application_setting_importer.ImportOrUseDefault("./settings.toml"));
}

void InitOutputSetting(
    std::shared_ptr<const gaitgen::ApplicationSettingRecord> setting) {
  gaitgen::cmdio::DoOutput(setting->do_cmd_output);
  gaitgen::cmdio::SetOutputLimit(setting->cmd_output_detail);
}

auto LoadPhantomXMkII() {
  using gaitgen::PhantomXMkII;
  using gaitgen::PhantomXMkIIParameterRecord;
  using gaitgen::TomlFileImporter;

  TomlFileImporter<PhantomXMkIIParameterRecord> parameter_importer;
  const auto record = parameter_importer.ImportOrUseDefault(
      "./simulation_condition/phantomx_mk2.toml");

  return std::make_shared<PhantomXMkII>(record);
}

auto LoadXrR1() {
  using gaitgen::TomlFileImporter;
  using gaitgen::XrR1;
  using gaitgen::XrR1ParameterRecord;

  TomlFileImporter<XrR1ParameterRecord> parameter_importer;
  const auto record = parameter_importer.ImportOrUseDefault(
      "./simulation_condition/xr_r1.toml");

  return std::make_shared<XrR1>(record);
}

}  // namespace

int main() {
  // int main内では gaitgen:: を省略できる.
  // 推奨されない書き方だが,可読性のため,ここでのみ使用する.
  using namespace gaitgen;
  using enum OutputDetail;

  // 設定を読み込む.
  auto app_setting_record = LoadApplicationSettingRecord();

  // 出力設定を初期化する.
  InitOutputSetting(app_setting_record);

  // タイトルを表示する.
  cmdio::OutputTitle(
      "Free Gait Planning for a Hexapod Walking Robot by Graph Search", true);

  // GUIを別のスレッドで実行する.
  // このスレッドへはGraphicDataBrokerを通してデータを渡す.
  GraphicSystem graphic_system(app_setting_record);

  // グラフィックシステムを別スレッドで実行する.
  boost::thread graphic_thread(&GraphicSystem::Main, &graphic_system);

  cmdio::SystemOutput("Now Waiting for Dxlib to start.");

  while (!graphic_system.IsInitialized()) {
    // Dxlibの初期化が終わるまで待機する.
  }

  cmdio::SystemOutput("Dxlib has started.");
  cmdio::OutputNewLine(1, kSystem);

  // 処理を実行する.
  while (true) {
    // 起動モードを選択する.
    BootMode boot_mode = app_setting_record->default_mode;

    if (app_setting_record->ask_about_modes) {
      BootModeSelector boot_mode_selector;

      // デフォルトの起動モードを設定する.
      boot_mode_selector.SetDefaultBootMode(app_setting_record->default_mode);

      // 起動モードを選択する.
      boot_mode = boot_mode_selector.SelectBootMode();
    }

    // 選択が終わったら,選択されたモードに応じてシステムを作成する.
    std::unique_ptr<ISystemMain> system_main;

    switch (boot_mode) {
      case BootMode::kSimulation: {
        // シミュレーションシステムクラスを作成する.
        auto phantomx_mk2 = LoadXrR1();

        const auto sim_setting_record =
            TomlFileImporter<SimulationSettingRecord>{}.ImportOrUseDefault(
                "./simulation_condition/simulation_setting.toml");

        const auto gpg_builder = std::make_unique<GpgSelector>(
            phantomx_mk2, phantomx_mk2, phantomx_mk2,
            sim_setting_record.gpg_depth, sim_setting_record.gpg_memory_limit);
        auto gait_pattern_generator =
            gpg_builder->Select(sim_setting_record.gpg_type);

        auto map_creator = MapCreatorSelector{}.Select(sim_setting_record);
        auto simulation_end_checker =
            SimulationEndCheckerFactory::Create(sim_setting_record);
        auto robot_operator = RobotOperatorFactory::Create(sim_setting_record);
        auto node_initializer = std::make_unique<NodeInitializer>(
            sim_setting_record.initial_positions,
            sim_setting_record.initial_posture, sim_setting_record.initial_move,
            sim_setting_record.initial_leg_z_local);
        auto result_exporter =
            std::make_shared<ResultFileExporter>(phantomx_mk2, phantomx_mk2);

        const auto graphic_data_broker = std::make_shared<GraphicDataBroker>();

        system_main = std::make_unique<SystemMainSimulation>(
            std::move(gait_pattern_generator), std::move(map_creator),
            std::move(simulation_end_checker), std::move(robot_operator),
            std::move(node_initializer), graphic_data_broker,
            app_setting_record, result_exporter);

        auto graphic_main = std::make_unique<GraphicMainBasic>(
            graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2,
            app_setting_record);

        graphic_system.ChangeGraphicMain(std::move(graphic_main));

        break;
      }
      case BootMode::kViewer: {
        // グラフビューアシステムクラスを作成する.

        TomlFileImporter<SimulationSettingRecord> simulation_setting_importer;
        const SimulationSettingRecord simulation_setting_record =
            simulation_setting_importer.ImportOrUseDefault(
                "./simulation_condition/simulation_setting.toml");

        auto hexapod_param = LoadXrR1();
        auto graphic_data_broker = std::make_shared<GraphicDataBroker>();

        auto map_creator =
            MapCreatorSelector{}.Select(simulation_setting_record);
        auto node_creator_builder =
            std::make_unique<NodeCreatorSequenceBodyRot>(
                hexapod_param, hexapod_param, hexapod_param);
        auto graph_tree_creator =
            std::make_unique<GraphTreeCreator>(std::move(node_creator_builder));

        system_main = std::make_unique<SystemMainGraphViewer>(
            std::move(graph_tree_creator), std::move(map_creator),
            graphic_data_broker, app_setting_record);

        std::unique_ptr<IGraphicMain> graphic_main_viewer =
            std::make_unique<GraphicMainGraphViewer>(
                graphic_data_broker, hexapod_param, hexapod_param,
                hexapod_param, app_setting_record);

        graphic_system.ChangeGraphicMain(std::move(graphic_main_viewer));

        break;
      }
      case BootMode::kDisplayModel: {
        auto phantomx_mk2 = LoadXrR1();

        std::unique_ptr<IGraphicMain> graphic_main_test =
            std::make_unique<GraphicMainDisplayModel>(
                phantomx_mk2, phantomx_mk2, phantomx_mk2, app_setting_record);

        graphic_system.ChangeGraphicMain(std::move(graphic_main_test));

        break;
      }
      case BootMode::kResultViewer: {
        // 結果表示システムクラスを作成する.
        auto phantomx_mk2 = LoadXrR1();
        auto graphic_data_broker = std::make_shared<GraphicDataBroker>();

        system_main = std::make_unique<SystemMainResultViewer>(
            graphic_data_broker, app_setting_record, phantomx_mk2,
            phantomx_mk2);

        std::unique_ptr<IGraphicMain> graphic_main =
            std::make_unique<GraphicMainBasic>(
                graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2,
                app_setting_record);

        graphic_system.ChangeGraphicMain(std::move(graphic_main));

        break;
      }
      case BootMode::kRobotControl: {
        auto graphic_data_broker = std::make_shared<GraphicDataBroker>();
        auto phantomx_mk2 = LoadPhantomXMkII();
        system_main =
            std::make_unique<SystemMainRobotControl>(graphic_data_broker);

        std::unique_ptr<IGraphicMain> graphic_main_robot_control =
            std::make_unique<GraphicMainRobotControl>(
                graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2,
                app_setting_record);

        graphic_system.ChangeGraphicMain(std::move(graphic_main_robot_control));
        break;
      }
      case BootMode::kContinuousSimulation: {
        // シミュレーションシステムクラスを作成する.
        auto hexapod = LoadXrR1();

        const auto sim_setting_record =
            TomlFileImporter<SimulationSettingRecord>{}.ImportOrUseDefault(
                "./simulation_condition/simulation_setting.toml");

        const auto gpg_builder = std::make_unique<GpgSelector>(
            hexapod, hexapod, hexapod, sim_setting_record.gpg_depth,
            sim_setting_record.gpg_memory_limit);
        auto gait_pattern_generator =
            gpg_builder->Select(sim_setting_record.gpg_type);

        auto simulation_end_checker =
            SimulationEndCheckerFactory::Create(sim_setting_record);
        auto robot_operator = RobotOperatorFactory::Create(sim_setting_record);
        auto node_initializer = std::make_unique<NodeInitializer>(
            sim_setting_record.initial_positions,
            sim_setting_record.initial_posture, sim_setting_record.initial_move,
            sim_setting_record.initial_leg_z_local);

        system_main = std::make_unique<SystemMainContinuousSimulation>(
            std::move(gait_pattern_generator),
            std::move(simulation_end_checker), std::move(robot_operator),
            std::move(node_initializer), app_setting_record, hexapod, hexapod);
        break;
      }
      case BootMode::kCreateMapCsv: {
        const auto graphic_data_broker = std::make_shared<GraphicDataBroker>();
        const auto hexapod = LoadXrR1();
        system_main =
            std::make_unique<SystemMainCreateMapCsv>(graphic_data_broker);
        auto graphic_main = std::make_unique<GraphicMainBasic>(
            graphic_data_broker, hexapod, hexapod, hexapod, app_setting_record);

        graphic_system.ChangeGraphicMain(std::move(graphic_main));
        break;
      }
      default: {
        assert(false);  // 無効なモードが指定された.
        break;
      }
    }

    // システムを実行する.
    if (system_main) {
      system_main->Main();
    }

    // もう一度実行するかどうかを選択する.
    cmdio::OutputHorizontalLine("=", kSystem);
    cmdio::OutputNewLine(1, kSystem);

    if (!cmdio::InputYesNo("Continue?")) {
      break;
    }

    cmdio::OutputNewLine(1, kSystem);
    cmdio::OutputHorizontalLine("=", kSystem);
    cmdio::OutputNewLine(1, kSystem);
  }

  cmdio::SystemOutput(
      "Waiting for Dxlib to exit; press the X button on the GUI.");
  graphic_thread.join();

  return 0;
}

#endif  // ! defined (DESIGNLAB_USE_TEST)
