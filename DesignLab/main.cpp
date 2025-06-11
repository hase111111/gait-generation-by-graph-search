
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
#include "gpg_builder_flat.h"
#include "graphic_main_basic.h"
#include "graphic_main_display_model.h"
#include "graphic_main_graph_viewer.h"
#include "graphic_main_robot_control.h"
#include "graphic_system.h"
#include "map_creator_selector.h"
#include "phantomx_mk2.h"
#include "robot_operator_factory.h"
#include "simulation_end_checker_factory.h"
#include "system_main_graph_viewer.h"
#include "system_main_result_viewer.h"
#include "system_main_robot_control.h"
#include "system_main_simulation.h"
#include "toml_directory_exporter.h"
#include "toml_file_importer.h"
#include "xr_r1.h"

// このプロジェクトがコンパイルされない場合はソリューションエクスプローラーから
// DesignLabを右クリック →「スタートアッププロジェクトに設定」を選択．

namespace {

auto LoadApplicationSettingRecord() {
  using designlab::CmdIOUtil;
  using enum designlab::OutputDetail;
  using designlab::ApplicationSettingRecord;
  using designlab::ApplicationSettingRecordValidator;
  using designlab::TomlDirectoryExporter;
  using designlab::TomlFileImporter;

  CmdIOUtil::SetOutputLimit(kSystem);

  TomlDirectoryExporter toml_directory_exporter;
  toml_directory_exporter.Export();

  TomlFileImporter<ApplicationSettingRecord> application_setting_importer(
      std::make_unique<ApplicationSettingRecordValidator>());

  return std::make_shared<const ApplicationSettingRecord>(
      application_setting_importer.ImportOrUseDefault("./settings.toml"));
}

void InitOutputSetting(
    std::shared_ptr<const designlab::ApplicationSettingRecord> setting) {
  using designlab::CmdIOUtil;

  CmdIOUtil::DoOutput(setting->do_cmd_output);
  CmdIOUtil::SetOutputLimit(setting->cmd_output_detail);
}

auto LoadPhantomXMkII() {
  using designlab::PhantomXMkII;
  using designlab::PhantomXMkIIParameterRecord;
  using designlab::TomlFileImporter;

  TomlFileImporter<PhantomXMkIIParameterRecord> parameter_importer;
  const auto record = parameter_importer.ImportOrUseDefault(
      "./simulation_condition/phantomx_mk2.toml");

  return std::make_shared<PhantomXMkII>(record);
}

auto LoadXrR1() {
  using designlab::XrR1;
  using designlab::XrR1ParameterRecord;

  return std::make_shared<XrR1>(XrR1ParameterRecord{});
}

}  // namespace

int main() {
  // int main内では designlab:: を省略できる．
  // 推奨されない書き方だが，可読性のため，ここでのみ使用する．
  using namespace designlab;
  using enum OutputDetail;

  // 設定を読み込む．
  auto app_setting_record = LoadApplicationSettingRecord();

  // 出力設定を初期化する．
  InitOutputSetting(app_setting_record);

  // タイトルを表示する．
  CmdIOUtil::OutputTitle(
      "Free Gait Planning for a Hexapod Walking Robot by Graph Search", true);

  // GUIを別のスレッドで実行する．
  // このスレッドへはGraphicDataBrokerを通してデータを渡す．
  GraphicSystem graphic_system(app_setting_record);

  // グラフィックシステムを別スレッドで実行する．
  boost::thread graphic_thread(&GraphicSystem::Main, &graphic_system);

  CmdIOUtil::SystemOutput("Now Waiting for Dxlib to start.");

  while (!graphic_system.IsInitialized()) {
    // Dxlibの初期化が終わるまで待機する．
  }

  CmdIOUtil::SystemOutput("Dxlib has started.");
  CmdIOUtil::OutputNewLine(1, kSystem);

  // 処理を実行する．
  while (true) {
    // 起動モードを選択する．
    BootMode boot_mode = app_setting_record->default_mode;

    if (app_setting_record->ask_about_modes) {
      BootModeSelector boot_mode_selector;

      // デフォルトの起動モードを設定する．
      boot_mode_selector.SetDefaultBootMode(app_setting_record->default_mode);

      // 起動モードを選択する．
      boot_mode = boot_mode_selector.SelectBootMode();
    }

    // 選択が終わったら，選択されたモードに応じてシステムを作成する．
    std::unique_ptr<ISystemMain> system_main;

    switch (boot_mode) {
      case BootMode::kSimulation: {
        // シミュレーションシステムクラスを作成する．
        auto phantomx_mk2 = LoadXrR1();

        const auto gpg_builder = std::make_unique<GpgBuilderFlat>(
            phantomx_mk2, phantomx_mk2, phantomx_mk2);
        auto gait_pattern_generator = gpg_builder->Build();

        const auto sim_setting_record =
            TomlFileImporter<SimulationSettingRecord>{}.ImportOrUseDefault(
                "./simulation_condition/simulation_setting.toml");

        auto map_creator = MapCreatorSelector{}.Select(sim_setting_record);
        auto simulation_end_checker =
            SimulationEndCheckerFactory::Create(sim_setting_record);
        auto robot_operator = RobotOperatorFactory::Create(sim_setting_record);
        auto node_initializer = std::make_unique<NodeInitializer>(
            sim_setting_record.initial_positions,
            sim_setting_record.initial_posture,
            sim_setting_record.initial_move);
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
        // グラフビューアシステムクラスを作成する．

        TomlFileImporter<SimulationSettingRecord> simulation_setting_importer;
        const SimulationSettingRecord simulation_setting_record =
            simulation_setting_importer.ImportOrUseDefault(
                "./simulation_condition/simulation_setting.toml");

        auto phantomx_mk2 = LoadPhantomXMkII();
        auto graphic_data_broker = std::make_shared<GraphicDataBroker>();

        auto map_creator =
            MapCreatorSelector{}.Select(simulation_setting_record);

        // system_main = std::make_unique<SystemMainGraphViewer>(
        //    std::move(graph_tree_creator),
        //    std::move(map_creator),
        //    graphic_data_broker,
        //    app_setting_record);

        CmdIOUtil::Output("Viewer is not implemented yet.", kSystem);

        std::unique_ptr<IGraphicMain> graphic_main_viewer =
            std::make_unique<GraphicMainGraphViewer>(
                graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2,
                app_setting_record);

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
        // 結果表示システムクラスを作成する．
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
      default: {
        assert(false);  // 無効なモードが指定された．
        break;
      }
    }

    // システムを実行する．
    if (system_main) {
      system_main->Main();
    }

    // もう一度実行するかどうかを選択する．
    CmdIOUtil::OutputHorizontalLine("=", kSystem);
    CmdIOUtil::OutputNewLine(1, kSystem);

    if (!CmdIOUtil::InputYesNo("Continue?")) {
      break;
    }

    CmdIOUtil::OutputNewLine(1, kSystem);
    CmdIOUtil::OutputHorizontalLine("=", kSystem);
    CmdIOUtil::OutputNewLine(1, kSystem);
  }

  CmdIOUtil::Output("Waiting for Dxlib to exit; press the X button on the GUI.",
                    kSystem);
  graphic_thread.join();

  return 0;
}

#endif  // ! defined (DESIGNLAB_USE_TEST)
