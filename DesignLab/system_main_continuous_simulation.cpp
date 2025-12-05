
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
#include "map_creator_by_csv.h"
#include "string_util.h"
#include "toml_directory_exporter.h"

namespace gaitgen {

static std::string GetStem(const std::string& path) {
  return std::filesystem::path(path).stem().string();
}

SystemMainContinuousSimulation::SystemMainContinuousSimulation(
    std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
    std::unique_ptr<ISimulationEndChecker>&& simulation_end_checker_ptr,
    std::unique_ptr<IRobotOperator>&& robot_operator_ptr,
    std::unique_ptr<NodeInitializer>&& node_initializer_ptr,
    const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr,
    const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
    const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr)
    : gait_pattern_generator_ptr_(std::move(gait_pattern_generator_ptr)),
      simulation_end_checker_ptr_(std::move(simulation_end_checker_ptr)),
      robot_operator_ptr_(std::move(robot_operator_ptr)),
      node_initializer_ptr_(std::move(node_initializer_ptr)),
      setting_ptr_(setting_ptr),
      calculator_ptr_(calculator_ptr),
      converter_ptr_(converter_ptr) {
  assert(gait_pattern_generator_ptr_ != nullptr);
  assert(simulation_end_checker_ptr_ != nullptr);
  assert(robot_operator_ptr_ != nullptr);
  assert(setting_ptr_ != nullptr);
}

void SystemMainContinuousSimulation::Main() {
  using enum OutputDetail;
  using string_util::EnumToStringRemoveTopK;

  // コマンドラインにタイトルを表示する.
  cmdio::OutputTitle("!!! Continuous Simulation Mode !!!");

  // マップのファイルパスを全て取得する.
  const auto map_file_paths = GetMapFilePath();
  if (map_file_paths.empty()) {
    cmdio::ErrorOutput(
        "No map files found. Please place CSV files in the appropriate "
        "directory.");
    return;
  }

  for (const auto& map_file_path : map_file_paths) {
    // output に表示する.
    cmdio::OutputF(kSystem, "Load map file : {}", map_file_path);
  }

  // 出力する結果の名前を決定する.
  const std::string result_output_name = cmdio::InputDirName();

  // シミュレーションを行う回数分ループする.
  for (int i{0}; i < map_file_paths.size(); ++i) {
    ResultFileExporter result_file_exporter{calculator_ptr_, converter_ptr_};
    DeadLockChecker dead_lock_checker;

    // 現在のノードの状態を格納する変数.
    RobotStateNode current_node = node_initializer_ptr_->InitNode();

    RobotOperation operation =
        robot_operator_ptr_->Init();  // 目標地点を決定する.

    // マップの状態を読み込む.
    map_state_ = MapCreatorByCsv{map_file_paths[i]}.InitMap();

    // シミュレーションの結果を格納する変数.
    SimulationResultRecord record;

    record.graph_search_result_recorder.push_back(
        GraphSearchResultRecord{current_node, 0.0, 0, {}, true});

    cmdio::SystemOutputF("[*] Start simulation {} / {} | {}", i + 1,
                         map_file_paths.size(), map_file_paths[i]);
    cmdio::SpacedOutputF(kInfo, "[Initial node state]\n{}",
                         current_node.ToString());

    // 最大歩容生成回数分までループする.
    for (int j = 0; j < kGaitPatternGenerationLimit; ++j) {
      current_node.ChangeLootNode();

      // 目標地点を更新する.
      operation = robot_operator_ptr_->Update(current_node);

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
          gait_pattern_generator_ptr_->GetExpandedNodeCountPerDepth(),
          result_node.has_value()});

      // グラフ探索に失敗.
      if (!result_node) {
        // 次の歩容が生成できなかったら,このループを抜け,
        // 次のシミュレーションへ進む.

        // シミュレーションの結果を格納する変数を失敗に更新する.
        record.simulation_result =
            enums::SimulationResult::kFailureByGraphSearch;

        cmdio::SystemOutputF(
            "Simulation failed. SimulationResult = {}/ GraphSearch = {}",
            EnumToStringRemoveTopK(record.simulation_result),
            result_node.error_or("Success"));
        break;
      }

      // 次の歩容が生成できているならば,ノードを更新する.
      current_node = *result_node;

      cmdio::SpacedOutputF(kInfo, "[ Gait generation {} times ]\n{}", j + 1,
                           current_node.ToString());
      cmdio::OutputHorizontalLine("-", kInfo);

      // 動作チェッカーにもノードを通達する.
      dead_lock_checker.AddNode(current_node);

      if (dead_lock_checker.IsDeadLock()) {
        // 動作がループしてしまっているならば,
        // ループを一つ抜け,次のシミュレーションへ進む.

        // シミュレーションの結果を格納する変数を失敗に更新する.
        record.simulation_result =
            enums::SimulationResult::kFailureByLoopMotion;

        cmdio::SystemOutputF(
            "Simulation failed. SimulationResult = {} / GraphSearch = {}",
            EnumToStringRemoveTopK(record.simulation_result),
            result_node.error_or("Success"));

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

      // for の最後のループであるならば, シミュレーションを終了する.
      if (j == kGaitPatternGenerationLimit - 1) {
        // シミュレーションの結果を格納する変数を成功に更新する.
        record.simulation_result = enums::SimulationResult::kSuccess;

        cmdio::SystemOutputF(
            "The simulation was successful. SimulationResult = {}",
            EnumToStringRemoveTopK(record.simulation_result));
      }

      if (j % (kGaitPatternGenerationLimit / 10) ==
          kGaitPatternGenerationLimit / 10 - 1) {
        cmdio::SystemOutputF("    Gait generation {} times completed.", j + 1);
      }
    }  // 歩容生成のループ終了.

    // 結果を格納する変数にマップの状態を格納する.
    record.map_state = map_state_;
    result_file_exporter.PushSimulationResult(record);  // 結果を追加する.
    result_file_exporter.Export(
        result_output_name + "_" +
            (record.simulation_result == enums::SimulationResult::kSuccess
                 ? "success"
                 : "failure") +
            "_" + GetStem(map_file_paths[i]),
        true);

    cmdio::OutputNewLine(1, kSystem);
  }

  cmdio::SpacedOutput("Exit Simulation", kSystem);
}

std::vector<std::string> SystemMainContinuousSimulation::GetMapFilePath()
    const {
  // TomlDirectoryExporter::kContinuousSimulationMapDirPath
  // に存在する csv ファイルを全て取得して返す.

  // ディレクトリが無ければ作成しておく（空の vector
  // を返す前に確実にディレクトリを用意）
  TomlDirectoryExporter exporter;
  exporter.Export();

  namespace fs = std::filesystem;
  std::vector<std::string> file_paths;

  const fs::path dir_path{
      TomlDirectoryExporter::kContinuousSimulationMapDirPath};
  if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
    return file_paths;
  }

  for (const auto& entry : fs::directory_iterator(dir_path)) {
    if (!entry.is_regular_file()) continue;

    std::string ext = entry.path().extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
      return static_cast<char>(std::tolower(c));
    });

    if (ext == ".csv") {
      file_paths.push_back(entry.path().string());
    }
  }

  std::sort(file_paths.begin(), file_paths.end());
  return file_paths;
}

}  // namespace gaitgen
