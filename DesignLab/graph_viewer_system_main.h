//! @file graph_viewer_system_main.h
//! @brief グラフを表示するシステムのメインクラス

#ifndef DESIGNLAB_GRAPH_VIEWER_SYSTEM_MAIN_H_
#define DESIGNLAB_GRAPH_VIEWER_SYSTEM_MAIN_H_

#include <memory>

#include "application_setting_recorder.h"
#include "graphic_data_broker.h"
#include "interface_pass_finder.h"
#include "interface_system_main.h"
#include "map_state.h"
#include "simulation_map_creator.h"
#include "stopwatch.h"


//! @class GraphViewerSystemMain
//! @brief グラフを表示するシステムのメインクラス
//! @details この研究の手法では木構造のグラフを作成する．
//! どのようなグラフが作成されるかを確認するために，このグラフを表示するシステムを作成した．
class GraphViewerSystemMain final : public ISystemMain
{
public:

	GraphViewerSystemMain(
		std::unique_ptr<IPassFinder>&& pass_finder_ptr,
		const std::shared_ptr<GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const ApplicationSettingRecorder>& setting_ptr
	);

	//! @brief メイン関数
	void Main() override;

private:

	//! @brief グラフを作成する．
	//! @param [in] parent 親ノード
	//! @param [out] graph 作成したグラフ
	void CreateGraph(const RobotStateNode parent, std::vector<RobotStateNode>* graph);


	//! @brief グラフのステータスを表示する．
	//! @n 全ノード数，木の深さ，各深さごとのノード数を表示する．
	//! @param [in] graph グラフ
	void OutputGraphStatus(const std::vector<RobotStateNode>& graph) const;

	//! @brief マップ生成のモードを入力する
	//! @return MapCreateMode マップ生成のモード
	MapCreateMode InputMapCreateMode() const;

	//! @brief マップ生成のオプションを入力する
	//! @return unsigned int MapCreateOption マップ生成のオプション
	unsigned int InputMapCreateOption() const;

	//! @brief グラフの中から1つのノードを選択する．グラフが空の場合は，初期状態のノードを返す．
	//! @param [in] graph グラフ
	//! @return RobotStateNode 選択されたノード
	RobotStateNode SelectNode(const std::vector<RobotStateNode>& graph) const;


	std::unique_ptr<IPassFinder> pass_finder_ptr_;

	const std::shared_ptr<GraphicDataBroker> broker_ptr_;

	const std::shared_ptr<const ApplicationSettingRecorder> setting_ptr_;

	MapState map_state_;

	Stopwatch stopwatch_;
};


#endif // DESIGNLAB_GRAPH_VIEWER_SYSTEM_MAIN_H_