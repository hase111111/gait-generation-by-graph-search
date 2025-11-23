
//! @file system_main_continuous_simulation.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_SYSTEM_MAIN_CONTINUOUS_SIMULATION_H_
#define DESIGNLAB_SYSTEM_MAIN_CONTINUOUS_SIMULATION_H_

#include <memory>
#include <string>

#include "application_setting_record.h"
#include "interface_gait_pattern_generator.h"
#include "interface_robot_operator.h"
#include "interface_simulation_end_checker.h"
#include "interface_system_main.h"
#include "map_state.h"
#include "node_initializer.h"
#include "result_file_exporter.h"
#include "robot_operation.h"
#include "stopwatch.h"

namespace designlab {

//! @class SystemMainContinuousSimulation
class SystemMainContinuousSimulation final : public ISystemMain {
 public:
  SystemMainContinuousSimulation() =
      delete;  //!< デフォルトコンストラクタは禁止.

  //! @param[in] gait_pattern_generator_ptr 自由歩容パターン生成を行うクラス.
  //! @param[in] map_creator_ptr マップを生成するクラス.
  //! @param[in] simulation_end_checker_ptr
  //! シミュレーションの終了を判定するクラス.
  //! @param[in] robot_operator_ptr 目標地点を決定するクラス.
  //! @param[in] node_initializer_ptr ノードを初期化するクラス.
  //! @param[in] broker_ptr グラフィックデータを別スレッドに送るための構造体.
  //! @param[in] setting_ptr 設定ファイルの内容を格納する構造体.
  SystemMainContinuousSimulation(
      std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
      std::unique_ptr<ISimulationEndChecker>&& simulation_end_checker_ptr,
      std::unique_ptr<IRobotOperator>&& robot_operator_ptr,
      std::unique_ptr<NodeInitializer>&& node_initializer_ptr,
      const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr,
      const std::shared_ptr<const IHexapodJointCalculator>& calculator_ptr,
      const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr);

  //! @brief いままで int mainで行われた処理をまとめたもの.
  //! @n 目標地点へ着くか,歩容計画に失敗した場合に,シミュレーションを終える.
  //! @n また,規定の回数シミュレーションしたら終了する.
  void Main() override;

 private:
  std::vector<std::string> GetMapFilePath() const;

  //! 1シミュレーション当たりの最大歩容生成回数.
  static constexpr int kGaitPatternGenerationLimit{5000};

  //! 自由歩容パターン生成を行うクラス.
  const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_ptr_;

  //! シミュレーションの終了を判定するクラス.
  const std::unique_ptr<const ISimulationEndChecker>
      simulation_end_checker_ptr_;

  //! ロボットの操作を行うクラス.
  const std::unique_ptr<IRobotOperator> robot_operator_ptr_;

  //! ノードを初期化するクラスを用意する.
  const std::unique_ptr<NodeInitializer> node_initializer_ptr_;

  //! 設定ファイルの内容を格納する構造体.
  const std::shared_ptr<const ApplicationSettingRecord> setting_ptr_;

  const std::shared_ptr<const IHexapodJointCalculator> calculator_ptr_;
  const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;

  MapState map_state_;  //!< 地形の状態.

  Stopwatch timer_;  //!< 時間計測用のクラス.
};

}  // namespace designlab

#endif  // DESIGNLAB_SYSTEM_MAIN_CONTINUOUS_SIMULATION_H_
