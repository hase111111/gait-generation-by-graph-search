
//! @file system_main_simulation.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "system_main_simulation.h"

#include <utility>

#include <boost/thread.hpp>
#include <magic_enum.hpp>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "dead_lock_checker.h"
#include "string_util.h"
#include "map_creator_for_simulation.h"


namespace designlab {

SystemMainSimulation::SystemMainSimulation(
    std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
    std::unique_ptr<IMapCreator>&& map_creator_ptr,
    std::unique_ptr<ISimulationEndChecker>&& simulation_end_checker_ptr,
    std::unique_ptr<IRobotOperator>&& robot_operator_ptr,
    std::unique_ptr<NodeInitializer>&& node_initializer_ptr,
    const std::shared_ptr<GraphicDataBroker>& broker_ptr,
    const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr,
    const std::shared_ptr<ResultFileExporter>& result_exporter_ptr) :
    gait_pattern_generator_ptr_(std::move(gait_pattern_generator_ptr)),
    map_creator_ptr_(std::move(map_creator_ptr)),
    simulation_end_checker_ptr_(std::move(simulation_end_checker_ptr)),
    robot_operator_ptr_(std::move(robot_operator_ptr)),
    node_initializer_ptr_(std::move(node_initializer_ptr)),
    broker_ptr_(broker_ptr),
    setting_ptr_(setting_ptr),
    result_exporter_ptr_(result_exporter_ptr) {
    assert(gait_pattern_generator_ptr_ != nullptr);
    assert(map_creator_ptr_ != nullptr);
    assert(simulation_end_checker_ptr_ != nullptr);
    assert(robot_operator_ptr_ != nullptr);
    assert(broker_ptr_ != nullptr);
    assert(setting_ptr_ != nullptr);

    // 結果をファイルに出力するクラスを初期化する．
    result_exporter_ptr_->CreateRootDirectory();

    // マップを生成する．
    map_state_ = map_creator_ptr_->InitMap();

    // 仲介人にマップを渡す．
    broker_ptr_->map_state.SetData(map_state_);
}


void SystemMainSimulation::Main() {
    using enum OutputDetail;
    using enum enums::Result;
    using string_util::EnumToStringRemoveTopK;

    // コマンドラインにタイトルを表示する．
    CmdIOUtil::OutputTitle("Simulation mode");
    OutputSetting();  // コマンドラインに設定を表示する．

    DeadLockChecker dead_lock_checker;

    // シミュレーションを行う回数分ループする．
    for (int i = 0; i < kSimulationNum; ++i) {
        // 現在のノードの状態を格納する変数．
        RobotStateNode current_node = node_initializer_ptr_->InitNode();

        RobotOperation operation = robot_operator_ptr_->Init();  // 目標地点を決定する．

        // シミュレーションの結果を格納する変数．
        SimulationResultRecord record;

        record.graph_search_result_recorder.push_back(GraphSearchResultRecord{ current_node, 0, { kSuccess, ""} });

        CmdIOUtil::FormatOutput(kSystem, "Start simulation {} times", i + 1);
        CmdIOUtil::SpacedFormatOutput(kInfo, "[Initial node state]\n{}", current_node.ToString());

        if (setting_ptr_->do_step_execution_each_simulation) {
            CmdIOUtil::OutputNewLine(1, kSystem);

            if (!CmdIOUtil::InputYesNo("Do you want to start the simulation?")) {
                break;
            }
        }


        if (setting_ptr_->do_gui_display) {
            // グラフィックが有効ならば，仲介人に最初のノードの状態を通達する．
            broker_ptr_->graph.PushBack(current_node);
        }


        // 最大歩容生成回数分までループする．
        for (int j = 0; j < kGaitPatternGenerationLimit; ++j) {
            current_node.ChangeLootNode();

            operation = robot_operator_ptr_->Update(current_node);  // 目標地点を更新する．

            timer_.Start();  // タイマースタート．

            RobotStateNode result_node;  // グラフ探索の結果を格納する変数．

            // グラフ探索を行う．
            const GraphSearchResult result_state = gait_pattern_generator_ptr_->GetNextNodeByGraphSearch(current_node, map_state_, operation, &result_node);

            timer_.End();  // タイマーストップ．

            // ノード，計算時間，結果を格納する．
            record.graph_search_result_recorder.push_back(GraphSearchResultRecord{ result_node, timer_.GetElapsedMilliSecond(), result_state });

            // グラフ探索に失敗．
            if (result_state.result != kSuccess) {
                // シミュレーションの結果を格納する変数を失敗に更新する．
                record.simulation_result = enums::SimulationResult::kFailureByGraphSearch;

                CmdIOUtil::FormatOutput(kSystem, "Simulation failed. SimulationResult = {}/ GraphSearch = {}",
                                  EnumToStringRemoveTopK(record.simulation_result), result_state.ToString());

                // 次の歩容が生成できなかったら，このループを抜け，
                // 次のシミュレーションへ進む．
                break;
            }

            // 次の歩容が生成できているならば，ノードを更新する．
            current_node = result_node;

            if (setting_ptr_->do_gui_display) {
                // グラフィックが有効ならば仲介人に結果を通達する．
                broker_ptr_->graph.PushBack(current_node);
            }

            CmdIOUtil::SpacedFormatOutput(kInfo, "[ Simulation {} times / Gait generation {} times ]\n{}", i + 1, j + 1, current_node.ToString());
            CmdIOUtil::OutputHorizontalLine("-", kInfo);

            // 動作チェッカーにもノードを通達する．
            dead_lock_checker.AddNode(current_node);

            if (dead_lock_checker.IsDeadLock()) {
                // 動作がループして失敗した時．
                // シミュレーションの結果を格納する変数を失敗に更新する．

                record.simulation_result = enums::SimulationResult::kFailureByLoopMotion;

                CmdIOUtil::FormatOutput(kSystem, "Simulation failed. SimulationResult = {} / GraphSearch = {}",
                                  EnumToStringRemoveTopK(record.simulation_result), result_state.ToString());

                // 動作がループしてしまっているならば，
                // ループを一つ抜け，次のシミュレーションへ進む．
                break;
            }

            // 成功時の処理．
            if (simulation_end_checker_ptr_->IsEnd(current_node)) {
                // シミュレーションの結果を格納する変数を成功に更新する．
                record.simulation_result = enums::SimulationResult::kSuccess;

                CmdIOUtil::FormatOutput(kSystem, "The simulation was successful. SimulationResult = {}", EnumToStringRemoveTopK(record.simulation_result));

                break;  // 成功したら，このループを抜け，次のシミュレーションへ進む．
            }

            // forの最後のループであるならば，失敗したことを通達する．
            if (j == kGaitPatternGenerationLimit - 1) {
                // シミュレーションの結果を格納する変数を失敗に更新する．
                record.simulation_result = enums::SimulationResult::kFailureByNodeLimitExceeded;

                CmdIOUtil::FormatOutput(kSystem, "Simulation failed. SimulationResult = {}", EnumToStringRemoveTopK(record.simulation_result));
            }

            // ステップ実行にしているならば，ここで一時停止する．
            if (setting_ptr_->do_step_execution_each_gait) {
                CmdIOUtil::OutputNewLine(1, kSystem);
                CmdIOUtil::WaitAnyKey("Generates the next step by key input");
            }
        }  // 歩容生成のループ終了．

        record.map_state = map_state_;  // 結果を格納する変数にマップの状態を格納する．
        result_exporter_ptr_->PushSimulationResult(record);  // 結果を追加する．

        // 仲介人にシミュレーション終了を通達する．
        broker_ptr_->simulation_end_index.PushBack(broker_ptr_->graph.GetSize() - 1);

        CmdIOUtil::OutputNewLine(1, kSystem);
        CmdIOUtil::OutputHorizontalLine("=", kSystem);
        CmdIOUtil::OutputNewLine(1, kSystem);
    }  // シミュレーションのループ終了


    // シミュレーションの結果を全てファイルに出力する．
    if (CmdIOUtil::InputYesNo("Do you want to output results?")) {
        result_exporter_ptr_->Export();
    }

    CmdIOUtil::SpacedOutput("Exit Simulation", kSystem);
}


void SystemMainSimulation::OutputSetting() const {
    using enum OutputDetail;

    CmdIOUtil::Output("[Setting]", kSystem);
    CmdIOUtil::OutputNewLine(1, kSystem);


    if (setting_ptr_->do_cmd_output) {
        CmdIOUtil::Output("・Output to command line.", kSystem);

        const std::string output_str = magic_enum::enum_name(setting_ptr_->cmd_output_detail).data();

        CmdIOUtil::FormatOutput(kSystem, "　　・Only those with a priority of {} or higher will be output.", output_str);
    }
    else {
        const std::string output_str = magic_enum::enum_name(kSystem).data();

        CmdIOUtil::FormatOutput(kSystem,
            "・No output to the command line. (The exception is the output of the one whose priority is {}.", output_str);
    }

    CmdIOUtil::OutputNewLine(1, kSystem);


    if (setting_ptr_->do_step_execution_each_simulation) {
        CmdIOUtil::Output("・Step through the simulation.", kSystem);
    }
    else {
        CmdIOUtil::Output("・Does not step through the simulation.", kSystem);
    }

    CmdIOUtil::OutputNewLine(1, kSystem);


    if (setting_ptr_->do_step_execution_each_gait) {
        CmdIOUtil::Output("・Step through each step.", kSystem);
    }
    else {
        CmdIOUtil::Output("・Does not step through each step.", kSystem);
    }

    CmdIOUtil::OutputNewLine(1, kSystem);

    if (setting_ptr_->do_gui_display) {
        CmdIOUtil::Output("・Display GUI.", kSystem);
    }
    else {
        CmdIOUtil::Output("・Do not display GUI.", kSystem);
    }

    CmdIOUtil::OutputNewLine(1, kSystem);
    CmdIOUtil::OutputHorizontalLine("-", kSystem);
    CmdIOUtil::OutputNewLine(1, kSystem);
}

}  // namespace designlab
