#pragma once

#include "map_state.h"
#include "graphic_system.h"
#include "graphic_data_broker.h"
#include "abstract_pass_finder.h"
#include "application_setting_recorder.h"


//! @class GraphViewerSystemMain
//! @date 2023/08/13
//! @author 長谷川
//! @brief グラフを表示するシステムのメインクラス
class GraphViewerSystemMain final
{
public:

	GraphViewerSystemMain(const SApplicationSettingRecorder* const setting);

	//! @brief メイン関数
	void main();

private:

	// グラフを作成する
	void createGraph(const SNode parent, std::vector<SNode>& graph);

	//グラフを仲介人にセットする
	void setGraphToBroker(const std::vector<SNode>& graph);

	// y / n の質問をする
	bool askYesNo(const std::string& question) const;

	// グラフのステータスを表示する
	void showGraphStatus(const std::vector<SNode>& graph) const;


	MapState m_map_state;

	GraphicDataBroker m_graphic_data_broker;

	GraphicSystem m_graphic_system;

	std::unique_ptr<AbstractPassFinder> mp_pass_finder;

	const SApplicationSettingRecorder* const mp_setting;

};


//! @file graph_viewer_system_main.h
//! @date 2023/08/13
//! @author 長谷川
//! @brief グラフを表示するシステムのメインクラス
//! @n 行数 : @lineinfo
