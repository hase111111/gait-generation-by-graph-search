//! @file graph_viewer_system_main.h
//! @brief グラフを表示するシステムのメインクラス

#ifndef DESIGNLAB_GRAPH_VIEWER_SYSTEM_MAIN_H_
#define DESIGNLAB_GRAPH_VIEWER_SYSTEM_MAIN_H_

#include <memory>

#include "map_state.h"
#include "graphic_system.h"
#include "graphic_data_broker.h"
#include "abstract_pass_finder.h"
#include "application_setting_recorder.h"


//! @class GraphViewerSystemMain
//! @brief グラフを表示するシステムのメインクラス
//! @details この研究の手法では木構造のグラフを作成する．
//! どのようなグラフが作成されるかを確認するために，このグラフを表示するシステムを作成した．

class GraphViewerSystemMain final
{
public:

	GraphViewerSystemMain(
		std::unique_ptr<AbstractPassFinder>&& pass_finder_ptr,
		std::unique_ptr<IGraphicMain>&& graphic_main_ptr,
		const std::shared_ptr<GraphicDataBroker>& broker_ptr,
		const std::shared_ptr<const SApplicationSettingRecorder>& setting_ptr
	);

	//! @brief メイン関数
	void Main();

private:

	// グラフを作成する
	void CreateGraph(const SNode parent, std::vector<SNode>& graph);

	//グラフを仲介人にセットする
	void SetGraphToBroker(const std::vector<SNode>& graph);

	// y / n の質問をする
	bool askYesNo(const std::string& question) const;

	// グラフのステータスを表示する
	void showGraphStatus(const std::vector<SNode>& graph) const;


	GraphicSystem graphic_system_;

	std::unique_ptr<AbstractPassFinder> pass_finder_ptr_;

	const std::shared_ptr<GraphicDataBroker> broker_ptr_;

	const std::shared_ptr<const SApplicationSettingRecorder> setting_ptr_;

	MapState map_state_;
};


#endif // !DESIGNLAB_GRAPH_VIEWER_SYSTEM_MAIN_H_