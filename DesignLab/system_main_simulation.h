//! @file system_main_simulation.h
//! @brief 歩容生成シミュレーションを行うクラス．先行研究におけるint main()で行われていた処理をまとめたもの．

#ifndef DESIGNLAB_SYSTEM_MAIN_SIMULATION_H_
#define DESIGNLAB_SYSTEM_MAIN_SIMULATION_H_

#include <memory>
#include <string>

#include "application_setting_recorder.h"
#include "graphic_data_broker.h"
#include "interface_gait_pattern_generator.h"
#include "interface_map_creator.h"
#include "interface_system_main.h"
#include "map_state.h"
#include "result_file_exporter.h"
#include "simulation_end_checker.h"
#include "stopwatch.h"
#include "target_robot_state.h"


//! @class SystemMainSimulation
//! @brief 中～大規模な設計において，int mainになんでも詰め込むわけにはいかないため，このクラスにまとめる．
//! @details 処理の内容を書き換えるときには，int main()から呼ぶクラスを変えるだけでいい．
class SystemMainSimulation final : public ISystemMain
{
public:
	SystemMainSimulation() = delete;		//!< デフォルトコンストラクタは禁止．

	//! @param[in] pass_finder_ptr 自由歩容パターン生成を行うクラス．
	//! @param[in] graphic_ptr グラフィックを描画するクラス．
	//! @param[in] broker_ptr グラフィックデータを管理するクラス．
	//! @param[in] setting_ptr 設定ファイルの内容を格納する構造体．
	SystemMainSimulation(
		std::unique_ptr<IGaitPatternGenerator>&& pass_finder_ptr,
		std::unique_ptr<IMapCreator>&& map_creator_ptr,
		const std::shared_ptr<GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr);


	//! @brief いままでint mainで行われた処理をまとめたもの．
	//! @n 目標地点へ着くか，歩容計画に失敗した場合に，シミュレーションを終える．
	//! @n また，規定の回数シミュレーションしたら終了する．
	void Main() override;

private:

	void OutputSetting() const;

	static constexpr int kSimurateNum{5};	//!< 連続でシミュレーションを行う回数

	static constexpr int kGaitPatternGenerationLimit{ 1000 };	//!< 1シミュレーション当たりの最大歩容生成回数


	const std::unique_ptr<IGaitPatternGenerator> pass_finder_ptr_;	//!< 自由歩容パターン生成を行うクラス．

	const std::unique_ptr<IMapCreator> map_creator_ptr_;	//!< マップを生成するクラス．

	const std::shared_ptr<GraphicDataBroker> broker_ptr_;	//!< グラフィックデータを管理するクラス．

	const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr_;	//!< 設定ファイルの内容を格納する構造体．

	const std::unique_ptr<const SimulationEndChecker> simu_end_checker_ptr_;	//!< シミュレーションの終了を判定するクラス．

	MapState map_state_;		//!< 地形の状態

	TargetRobotState target_;	//!< 目標地点．

	Stopwatch timer_;			//!< 時間計測用のクラス．

	ResultFileExporter result_exporter_;	//!< 結果をファイルに出力するクラス．
};


#endif	// DESIGNLAB_SYSTEM_MAIN_SIMULATION_H_