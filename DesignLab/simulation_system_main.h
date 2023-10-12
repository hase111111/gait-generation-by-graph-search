//! @file simulation_system_main.h
//! @brief 歩容生成シミュレーションを行うクラス．先行研究におけるint main()で行われていた処理をまとめたもの．

#ifndef SIMULATION_SYSTEM_MAIN_H_
#define SIMULATION_SYSTEM_MAIN_H_

#include <memory>
#include <string>

#include "abstract_hexapod_state_calculator.h"
#include "application_setting_recorder.h"
#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "interface_graphic_main.h"
#include "interface_pass_finder.h"
#include "interface_system_main.h"
#include "map_state.h"
#include "result_file_exporter.h"
#include "stopwatch.h"
#include "target.h"


//! @class SimulationSystemMain
//! @brief 中～大規模な設計において，int mainになんでも詰め込むわけにはいかないため，このクラスにまとめる．
//! @details 処理の内容を書き換えるときには，int main()から呼ぶクラスを変えるだけでいい．

class SimulationSystemMain final : public ISystemMain
{
public:
	SimulationSystemMain() = delete;		//デフォルトコンストラクタは禁止．

	//! @param[in] pass_finder_ptr 自由歩容パターン生成を行うクラス．
	//! @param[in] graphic_ptr グラフィックを描画するクラス．
	//! @param[in] broker_ptr グラフィックデータを管理するクラス．
	//! @param[in] setting_ptr 設定ファイルの内容を格納する構造体．
	SimulationSystemMain(
		std::unique_ptr<IPassFinder>&& pass_finder_ptr,
		std::unique_ptr<IGraphicMain>&& graphic_ptr,
		const std::shared_ptr<GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr);


	//! @brief いままでint mainで行われた処理をまとめたもの．
	//! @n 目標地点へ着くか，歩容計画に失敗した場合に，シミュレーションを終える．
	//! @n また，規定の回数シミュレーションしたら終了する．
	void Main() override;

private:

	void OutputSetting() const;


	std::unique_ptr<IPassFinder> pass_finder_ptr_;

	GraphicSystem graphic_system_;

	const std::shared_ptr<GraphicDataBroker> broker_ptr_;					//!< グラフィックデータを管理するクラス．

	const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr_;	//!< 設定ファイルの内容を格納する構造体．


	MapState map_state_;	//!< 地形の状態を管理するクラス．

	TargetRobotState target_;		//!< 目標地点．

	Stopwatch timer_;		//!< 時間計測用のクラス．

	ResultFileExporter result_exporter_;	//!< 結果をファイルに出力するクラス．
};


#endif