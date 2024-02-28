
//! @file      system_main_simulation.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_SYSTEM_MAIN_SIMULATION_H_
#define DESIGNLAB_SYSTEM_MAIN_SIMULATION_H_

#include <memory>
#include <string>

#include "application_setting_record.h"
#include "graphic_data_broker.h"
#include "interface_gait_pattern_generator.h"
#include "interface_map_creator.h"
#include "interface_simulation_end_checker.h"
#include "interface_robot_operator.h"
#include "interface_system_main.h"
#include "map_state.h"
#include "node_initializer.h"
#include "result_file_exporter.h"
#include "stopwatch.h"
#include "robot_operation.h"
#include "robot_operator_for_gpg.h"


namespace designlab
{

//! @class SystemMainSimulation
//! @brief 先行研究における int main()で行われていた処理をまとめたもので，
//! 歩容生成シミュレーションを行うクラス．
//! @details 処理の内容を書き換えるときには，int main()から呼ぶクラスを変えるだけでいい．
class SystemMainSimulation final : public ISystemMain
{
public:
    SystemMainSimulation() = delete;  //!< デフォルトコンストラクタは禁止．

    //! @param[in] gait_pattern_generator_ptr 自由歩容パターン生成を行うクラス．
    //! @param[in] map_creator_ptr マップを生成するクラス．
    //! @param[in] simulation_end_checker_ptr シミュレーションの終了を判定するクラス．
    //! @param[in] robot_operator_ptr 目標地点を決定するクラス．
    //! @param[in] node_initializer_ptr ノードを初期化するクラス．
    //! @param[in] broker_ptr グラフィックデータを別スレッドに送るための構造体．
    //! @param[in] setting_ptr 設定ファイルの内容を格納する構造体．
    SystemMainSimulation(
      std::unique_ptr<IGaitPatternGenerator>&& gait_pattern_generator_ptr,
      std::unique_ptr<IMapCreator>&& map_creator_ptr,
      std::unique_ptr<ISimulationEndChecker>&& simulation_end_checker_ptr,
      std::unique_ptr<IRobotOperator>&& robot_operator_ptr,
      std::unique_ptr<NodeInitializer>&& node_initializer_ptr,
      const std::shared_ptr<GraphicDataBroker>& broker_ptr,
      const std::shared_ptr<const ApplicationSettingRecord>& setting_ptr,
      const std::shared_ptr<ResultFileExporter>& result_exporter_ptr);


    //! @brief いままで int mainで行われた処理をまとめたもの．
    //! @n 目標地点へ着くか，歩容計画に失敗した場合に，シミュレーションを終える．
    //! @n また，規定の回数シミュレーションしたら終了する．
    void Main() override;

private:
    static constexpr int kSimulationNum{ 5 };  //!< 連続でシミュレーションを行う回数．

    //! 1シミュレーション当たりの最大歩容生成回数．
    static constexpr int kGaitPatternGenerationLimit{ 1000 };


    void OutputSetting() const;

    //! 自由歩容パターン生成を行うクラス．
    const std::unique_ptr<IGaitPatternGenerator> gait_pattern_generator_ptr_;

    //! マップを生成するクラス．
    const std::unique_ptr<IMapCreator> map_creator_ptr_;

    //! シミュレーションの終了を判定するクラス．
    const std::unique_ptr<const ISimulationEndChecker> simulation_end_checker_ptr_;

    //! ロボットの操作を行うクラス．
    const std::unique_ptr<IRobotOperator> robot_operator_ptr_;

    //! ノードを初期化するクラスを用意する．
    const std::unique_ptr<NodeInitializer> node_initializer_ptr_;

    //! グラフィックデータを管理するクラス．
    const std::shared_ptr<GraphicDataBroker> broker_ptr_;

    //! 設定ファイルの内容を格納する構造体．
    const std::shared_ptr<const ApplicationSettingRecord> setting_ptr_;

    const std::shared_ptr<ResultFileExporter> result_exporter_ptr_;  //!< 結果をファイルに出力するクラス．

    MapState map_state_;    //!< 地形の状態．

    Stopwatch timer_;       //!< 時間計測用のクラス．
};

}  // namespace designlab


#endif  // DESIGNLAB_SYSTEM_MAIN_SIMULATION_H_
