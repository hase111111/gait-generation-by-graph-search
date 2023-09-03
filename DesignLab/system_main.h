#pragma once

#include <memory>
#include <string>

#include "map_state.h"
#include "Target.h"
#include "graphic_data_broker.h"
#include "graphic_system.h"
#include "abstract_pass_finder.h"
#include "interface_graphic_main_builder.h"
#include "abstract_graphic_main.h"
#include "abstract_hexapod_state_calculator.h"
#include "designlab_timer.h"
#include "result_file_exporter.h"
#include "application_setting_recorder.h"



//! @class SystemMain
//! @date 2023/08/06
//! @author 長谷川
//! @brief 中～大規模な設計において，int mainになんでも詰め込むわけにはいかないため，このクラスにまとめる．
//! @details 処理の内容を書き換えるときには，int mainから呼ぶクラスを変えるだけでいい．
class SystemMain final
{
public:
	SystemMain() = delete;		//デフォルトコンストラクタは禁止．

	SystemMain(std::unique_ptr<AbstractPassFinder>&& graph_search, std::unique_ptr<AbstractPassFinderFactory>&& graph_search_factory,
		std::unique_ptr<IGraphicMainBuilder>&& builder, std::shared_ptr<AbstractHexapodStateCalculator> calc, SApplicationSettingRecorder* recorder);

	~SystemMain() = default;


	//! @brief いままでint mainで行われた処理をまとめたもの．目標地点へ着くか，歩容計画に失敗した場合に，シミュレーションを終える．規定の回数シミュレーションしたら終了する．
	void main();

private:

	void outputTitle() const;

	void outputSetting() const;


	MapState m_map_state;

	STarget m_target;

	GraphicDataBroker m_broker;

	GraphicSystem m_graphic_system;

	std::unique_ptr<AbstractPassFinder> mp_pass_finder;


	DesignlabTimer m_timer;					//時間計測用のクラス．

	ResultFileExporter m_result_exporter;	//結果をファイルに出力するクラス．

	const SApplicationSettingRecorder* const mp_setting;	//設定ファイルの内容を格納する構造体．
};


//! @file system_main.h
//! @date 2023/08/06
//! @author 長谷川
//! @brief このプログラムの処理をまとめたもの．処理の内容を大きく変えたい場合はint main()から，全く別のクラスを呼べばよい．
//! @n 行数 : @lineinfo