
//! @file system_main_continuous_simulation.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "system_main_continuous_simulation.h"

#include <boost/thread.hpp>
#include <magic_enum.hpp>
#include <utility>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "dead_lock_checker.h"
#include "map_creator_for_simulation.h"
#include "string_util.h"

namespace designlab {

SystemMainContinuousSimulation::SystemMainContinuousSimulation(
    std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
    std::unique_ptr<IMapCreator>&& map_creator_ptr,
    std::unique_ptr<ISimulationEndChecker>&& simulation_end_checker_ptr,
    std::unique_ptr<IRobotOperator>&& robot_operator_ptr,
    std::unique_ptr<NodeInitializer>&& node_initializer_ptr,
    const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr,
    const std::shared_ptr<ResultFileExporter>& result_exporter_ptr)
    : gait_pattern_generator_ptr_(std::move(gait_pattern_generator_ptr)),
      map_creator_ptr_(std::move(map_creator_ptr)),
      simulation_end_checker_ptr_(std::move(simulation_end_checker_ptr)),
      robot_operator_ptr_(std::move(robot_operator_ptr)),
      node_initializer_ptr_(std::move(node_initializer_ptr)),
      setting_ptr_(setting_ptr),
      result_exporter_ptr_(result_exporter_ptr) {
  assert(gait_pattern_generator_ptr_ != nullptr);
  assert(map_creator_ptr_ != nullptr);
  assert(simulation_end_checker_ptr_ != nullptr);
  assert(robot_operator_ptr_ != nullptr);
  assert(setting_ptr_ != nullptr);

  // 結果をファイルに出力するクラスを初期化する.
  result_exporter_ptr_->CreateRootDirectory();

  // マップを生成する.
  map_state_ = map_creator_ptr_->InitMap();
}

void SystemMainContinuousSimulation::Main() {
  using enum OutputDetail;
  using string_util::EnumToStringRemoveTopK;

  // コマンドラインにタイトルを表示する.
  cmdio::OutputTitle("!!! Continuous Simulation Mode !!!");

  DeadLockChecker dead_lock_checker;

  // シミュレーションを行う回数分ループする.
  for (int i = 0; i < kSimulationNum; ++i) {
    // 現在のノードの状態を格納する変数.
    RobotStateNode current_node = node_initializer_ptr_->InitNode();

    RobotOperation operation =
        robot_operator_ptr_->Init();  // 目標地点を決定する.

    // シミュレーションの結果を格納する変数.
    SimulationResultRecord record;

    record.graph_search_result_recorder.push_back(
        GraphSearchResultRecord{current_node, 0.0, 0, true});

    cmdio::OutputF(kSystem, "Start simulation {} times", i + 1);
    cmdio::SpacedOutputF(kInfo, "[Initial node state]\n{}",
                         current_node.ToString());

    // 最大歩容生成回数分までループする.
    for (int j = 0; j < kGaitPatternGenerationLimit; ++j) {
      current_node.ChangeLootNode();

      operation =
          robot_operator_ptr_->Update(current_node);  // 目標地点を更新する.

      timer_.Start();  // タイマースタート.

      // グラフ探索を行う.
      const auto result_node =
          gait_pattern_generator_ptr_->GetNextNodeByGraphSearch(
              current_node, map_state_, operation);

      timer_.End();  // タイマーストップ.

      // ノード,計算時間,結果を格納する.
      record.graph_search_result_recorder.push_back(GraphSearchResultRecord{
          result_node.value_or(RobotStateNode{}),
          timer_.GetElapsedMilliSecond(),
          gait_pattern_generator_ptr_->GetExpandedNodeCount(),
          result_node.has_value()});

      // グラフ探索に失敗.
      if (!result_node) {
        // シミュレーションの結果を格納する変数を失敗に更新する.
        record.simulation_result =
            enums::SimulationResult::kFailureByGraphSearch;

        cmdio::SystemOutputF(
            "Simulation failed. SimulationResult = {}/ GraphSearch = {}",
            EnumToStringRemoveTopK(record.simulation_result),
            result_node.error_or("Success"));

        // 次の歩容が生成できなかったら,このループを抜け,
        // 次のシミュレーションへ進む.
        break;
      }

      // 次の歩容が生成できているならば,ノードを更新する.
      current_node = *result_node;

      cmdio::SpacedOutputF(
          kInfo, "[ Simulation {} times / Gait generation {} times ]\n{}",
          i + 1, j + 1, current_node.ToString());
      cmdio::OutputHorizontalLine("-", kInfo);

      // 動作チェッカーにもノードを通達する.
      dead_lock_checker.AddNode(current_node);

      if (dead_lock_checker.IsDeadLock()) {
        // 動作がループして失敗した時.
        // シミュレーションの結果を格納する変数を失敗に更新する.

        record.simulation_result =
            enums::SimulationResult::kFailureByLoopMotion;

        cmdio::SystemOutputF(
            "Simulation failed. SimulationResult = {} / GraphSearch = {}",
            EnumToStringRemoveTopK(record.simulation_result),
            result_node.error_or("Success"));

        // 動作がループしてしまっているならば,
        // ループを一つ抜け,次のシミュレーションへ進む.
        break;
      }

      // 成功時の処理.
      if (simulation_end_checker_ptr_->IsEnd(current_node)) {
        // シミュレーションの結果を格納する変数を成功に更新する.
        record.simulation_result = enums::SimulationResult::kSuccess;

        cmdio::SystemOutputF(
            "The simulation was successful. SimulationResult = {}",
            EnumToStringRemoveTopK(record.simulation_result));

        break;  // 成功したら,このループを抜け,次のシミュレーションへ進む.
      }

      // forの最後のループであるならば,失敗したことを通達する.
      if (j == kGaitPatternGenerationLimit - 1) {
        // シミュレーションの結果を格納する変数を失敗に更新する.
        record.simulation_result =
            enums::SimulationResult::kFailureByNodeLimitExceeded;

        cmdio::OutputF(kSystem, "Simulation failed. SimulationResult = {}",
                       EnumToStringRemoveTopK(record.simulation_result));
      }
    }  // 歩容生成のループ終了.

    record.map_state =
        map_state_;  // 結果を格納する変数にマップの状態を格納する.
    result_exporter_ptr_->PushSimulationResult(record);  // 結果を追加する.

    cmdio::OutputNewLine(1, kSystem);
    cmdio::OutputHorizontalLine("=", kSystem);
    cmdio::OutputNewLine(1, kSystem);
  }  // シミュレーションのループ終了.

  // シミュレーションの結果を全てファイルに出力する.
  if (cmdio::InputYesNo("Do you want to output results?")) {
    result_exporter_ptr_->Export();
  }

  cmdio::SpacedOutput("Exit Simulation", kSystem);
}

}  // namespace designlab
