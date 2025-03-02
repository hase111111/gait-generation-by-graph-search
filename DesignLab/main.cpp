
//! @file main.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include <memory>

#include <boost/thread.hpp>

#include "test_main.h"

#if !defined (DESIGNLAB_USE_TEST)

#include "application_setting_record.h"
#include "application_setting_record_validator.h"
#include "boot_mode_selector.h"
#include "cmdio_util.h"
#include "gait_pattern_generator_basic.h"
#include "gait_pattern_generator_revaluation.h"
#include "gait_pattern_generator_switch_move.h"
#include "gait_pattern_generator_thread.h"
#include "graph_tree_creator.h"
#include "graph_searcher_straight_move.h"
#include "graph_searcher_spot_turn.h"
#include "graphic_main_basic.h"
#include "graphic_main_graph_viewer.h"
#include "graphic_main_display_model.h"
#include "graphic_main_robot_control.h"
#include "graphic_system.h"
#include "node_creator_builder_turn.h"
#include "node_creator_builder_turn_spot.h"
#include "node_creator_builder_straight_move.h"
#include "node_creator_builder_body_rot.h"
#include "map_creator_factory.h"
#include "robot_operator_factory.h"
#include "simulation_end_checker_factory.h"
#include "system_main_graph_viewer.h"
#include "system_main_result_viewer.h"
#include "system_main_robot_control.h"
#include "system_main_simulation.h"
#include "toml_file_importer.h"
#include "toml_directory_exporter.h"
#include "phantomx_mk2.h"


// このプロジェクトがコンパイルされない場合はソリューションエクスプローラーから
// DesignLabを右クリック →「スタートアッププロジェクトに設定」を選択．


namespace {

auto LoadApplicationSettingRecord() {
    using designlab::CmdIOUtil;
    using enum designlab::OutputDetail;
    using designlab::ApplicationSettingRecord;
    using designlab::ApplicationSettingRecordValidator;
    using designlab::TomlFileImporter;
    using designlab::TomlDirectoryExporter;

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

}  // namespace


int main() {
    // int main内では designlab:: を省略できる．
    // 推奨されない書き方だが，可読性のため，ここでのみ使用する．
    using namespace designlab;
    using enum OutputDetail;

    // 設定を読み込む．
    auto application_setting_record = LoadApplicationSettingRecord();

    // 出力設定を初期化する．
    InitOutputSetting(application_setting_record);

    // タイトルを表示する．
    CmdIOUtil::OutputTitle("Free Gait Planning for a Hexapod Walking Robot by Graph Search", true);


    // GUIを別のスレッドで実行する．
    // このスレッドへはGraphicDataBrokerを通してデータを渡す．
    GraphicSystem graphic_system(application_setting_record);

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
        BootMode boot_mode = application_setting_record->default_mode;

        if (application_setting_record->ask_about_modes) {
            BootModeSelector boot_mode_selector;

            // デフォルトの起動モードを設定する．
            boot_mode_selector.SetDefaultBootMode(application_setting_record->default_mode);

            // 起動モードを選択する．
            boot_mode = boot_mode_selector.SelectBootMode();
        }


        // 選択が終わったら，選択されたモードに応じてシステムを作成する．
        auto graphic_data_broker = std::make_shared<GraphicDataBroker>();

        TomlFileImporter<PhantomXMkIIParameterRecord> parameter_importer;
        const PhantomXMkIIParameterRecord parameter_record = parameter_importer.ImportOrUseDefault("./simulation_condition/phantomx_mk2.toml");

        auto phantomx_mk2 = std::make_shared<PhantomXMkII>(parameter_record);

        auto node_creator_builder_straight = std::make_unique<NodeCreatorBuilderStraightMove>(phantomx_mk2, phantomx_mk2, phantomx_mk2);
        auto node_creator_builder_turn_spot = std::make_unique<NodeCreatorBuilderTurnSpot>(phantomx_mk2, phantomx_mk2, phantomx_mk2);

        auto graph_tree_creator_straight = std::make_unique<GraphTreeCreator>(std::move(node_creator_builder_straight));
        auto graph_tree_creator_turn_spot = std::make_unique<GraphTreeCreator>(std::move(node_creator_builder_turn_spot));

        auto graph_searcher_straight = std::make_unique<GraphSearcherStraightMove>(phantomx_mk2);
        auto graph_searcher_turn_spot = std::make_unique<GraphSearcherSpotTurn>(phantomx_mk2, phantomx_mk2);

        std::unique_ptr<ISystemMain> system_main;

        switch (boot_mode) {
            case BootMode::kSimulation: {
                // シミュレーションシステムクラスを作成する．

                auto gait_pattern_generator_straight = std::make_unique<GaitPatternGeneratorBasic>(std::move(graph_tree_creator_straight), std::move(graph_searcher_straight), 4, 20000000);
                auto gait_pattern_generator_turn_spot = std::make_unique<GaitPatternGeneratorBasic>(std::move(graph_tree_creator_turn_spot), std::move(graph_searcher_turn_spot), 4, 20000000);
                auto gait_pattern_generator_switch_move = std::make_unique<GaitPatternGeneratorSwitchMove>(std::move(gait_pattern_generator_straight), std::move(gait_pattern_generator_turn_spot));

                const SimulationSettingRecord simulation_setting_record = TomlFileImporter<SimulationSettingRecord>{}.ImportOrUseDefault("./simulation_condition/simulation_setting.toml");

                auto map_creator = MapCreatorFactory::Create(simulation_setting_record);
                auto simulation_end_checker = SimulationEndCheckerFactory::Create(simulation_setting_record);
                auto robot_operator = RobotOperatorFactory::Create(simulation_setting_record);
                auto node_initializer = std::make_unique<NodeInitializer>(simulation_setting_record.initial_positions,
                                                                          simulation_setting_record.initial_posture,
                                                                          simulation_setting_record.initial_move);
                auto result_exporter = std::make_shared<ResultFileExporter>(phantomx_mk2);

                system_main = std::make_unique<SystemMainSimulation>(
                  std::move(gait_pattern_generator_switch_move),
                  std::move(map_creator),
                  std::move(simulation_end_checker),
                  std::move(robot_operator),
                  std::move(node_initializer),
                  graphic_data_broker,
                  application_setting_record,
                  result_exporter);

                auto graphic_main = std::make_unique<GraphicMainBasic>(graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2, application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main));

                break;
            }
            case BootMode::kViewer: {
                // グラフビューアシステムクラスを作成する．

                TomlFileImporter<SimulationSettingRecord> simulation_setting_importer;
                const SimulationSettingRecord simulation_setting_record =
                    simulation_setting_importer.ImportOrUseDefault("./simulation_condition/simulation_setting.toml");

                auto map_creator = MapCreatorFactory::Create(simulation_setting_record);

                system_main = std::make_unique<SystemMainGraphViewer>(
                    std::move(graph_tree_creator_straight),
                    std::move(map_creator),
                    graphic_data_broker,
                    application_setting_record);

                std::unique_ptr<IGraphicMain> graphic_main_viewer =
                    std::make_unique<GraphicMainGraphViewer>(graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2, application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main_viewer));

                break;
            }
            case BootMode::kDisplayModel: {
                std::unique_ptr<IGraphicMain> graphic_main_test =
                    std::make_unique<GraphicMainDisplayModel>(
                    phantomx_mk2,
                    phantomx_mk2,
                    phantomx_mk2,
                    application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main_test));

                break;
            }
            case BootMode::kResultViewer: {
                // 結果表示システムクラスを作成する．
                system_main = std::make_unique<SystemMainResultViewer>(graphic_data_broker, application_setting_record, phantomx_mk2, phantomx_mk2);

                std::unique_ptr<IGraphicMain> graphic_main = std::make_unique<GraphicMainBasic>(graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2, application_setting_record);

                graphic_system.ChangeGraphicMain(std::move(graphic_main));

                break;
            }
            case BootMode::kRobotControl: {
                system_main = std::make_unique<SystemMainRobotControl>(graphic_data_broker);

                std::unique_ptr<IGraphicMain> graphic_main_robot_control =
                    std::make_unique<GraphicMainRobotControl>(graphic_data_broker, phantomx_mk2, phantomx_mk2, phantomx_mk2, application_setting_record);

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

    CmdIOUtil::Output("Waiting for Dxlib to exit; press the X button on the GUI.", kSystem);
    graphic_thread.join();

    return 0;
}

#endif  // ! defined (DESIGNLAB_USE_TEST)
