//! @file system_main.h
//! @brief このプログラムの処理をまとめたもの．処理の内容を大きく変えたい場合はint Main()から，全く別のクラスを呼べばよい．

//! @class SystemMain
//! @brief 中～大規模な設計において，int mainになんでも詰め込むわけにはいかないため，このクラスにまとめる．
//! @details 処理の内容を書き換えるときには，int main()から呼ぶクラスを変えるだけでいい．


#ifndef DESIGNLAB_SYSTEM_MAIN_H
#define DESIGNLAB_SYSTEM_MAIN_H

#include <memory>
#include <string>

#include "map_state.h"
#include "target.h"
#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "abstract_pass_finder.h"
#include "interface_graphic_main_builder.h"
#include "abstract_graphic_main.h"
#include "abstract_hexapod_state_calculator.h"
#include "designlab_timer.h"
#include "result_file_exporter.h"
#include "application_setting_recorder.h"


class SystemMain final
{
public:
	SystemMain() = delete;		//デフォルトコンストラクタは禁止．

	//! @brief コンストラクタ．
	//! @param[in] graph_search 歩容計画を行うクラス．
	//! @param[in] graph_search_factory 歩容計画を行うクラスのファクトリ．
	//! @param[in] builder グラフィックの描画を行うクラス．
	//! @param[in] calc 六脚歩行ロボットの状態を計算するクラス．これだけshared_ptrなので注意
	SystemMain(std::unique_ptr<AbstractPassFinder>&& graph_search, std::unique_ptr<IPassFinderFactory>&& graph_search_factory,
		std::unique_ptr<IGraphicMainBuilder>&& builder, std::shared_ptr<AbstractHexapodStateCalculator> calc, SApplicationSettingRecorder* recorder);


	//! @brief いままでint mainで行われた処理をまとめたもの．目標地点へ着くか，歩容計画に失敗した場合に，シミュレーションを終える．規定の回数シミュレーションしたら終了する．
	void Main();

private:

	void OutputTitle() const;

	void OutputSetting() const;


	MapState map_state_;

	STarget target_;

	GraphicDataBroker broker_;

	GraphicSystem graphic_system_;

	std::unique_ptr<AbstractPassFinder> pass_finder_ptr_;


	DesignlabTimer timer_;					//時間計測用のクラス．

	ResultFileExporter result_exporter_;	//結果をファイルに出力するクラス．

	const SApplicationSettingRecorder* const mp_setting;	//設定ファイルの内容を格納する構造体．
};


#endif