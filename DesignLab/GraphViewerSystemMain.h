#pragma once
#include "map_state.h"
#include "graphic_system.h"
#include "graphic_data_broker.h"
#include "InterfacePassFinder.h"


class GraphViewerSystemMain final
{
public:

	GraphViewerSystemMain();

	void main();

private:
	MapState m_map_state;
	GraphicDataBroker m_graphic_data_broker;
	GraphicSystem m_graphic_system;
	std::unique_ptr<IPassFinder> mp_pass_finder;

	// グラフを作成する
	void createGraph(const SNode parent, std::vector<SNode>& graph);

	//グラフを仲介人にセットする
	void setGraphToBroker(const std::vector<SNode>& graph);

	// y / n の質問をする
	bool askYesNo(const std::string& question) const;

	// グラフのステータスを表示する
	void showGraphStatus(const std::vector<SNode>& graph) const;
};
