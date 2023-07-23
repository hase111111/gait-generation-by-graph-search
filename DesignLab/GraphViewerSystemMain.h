#pragma once
#include "MapState.h"
#include "GraphicSystem.h"
#include "GraphicDataBroker.h"
#include "InterfaceGraphTreeCreator.h"

class GraphViewerSystemMain final
{
public:

	GraphViewerSystemMain();

	void main();

private:
	MapState m_MapState;
	GraphicDataBroker m_GraphicDataBroker;
	GraphicSystem m_GraphicSystem;
	std::unique_ptr<IGraphTreeCreator> mp_GraphTreeCreator;

	// グラフを作成する
	void createGraph(const SNode _parent, std::vector<SNode>& _graph);

	//グラフを仲介人にセットする
	void setGraphToBroker(const std::vector<SNode>& _graph);

	// y / n の質問をする
	bool askYesNo(const std::string& question) const;

	// グラフのステータスを表示する
	void showGraphStatus(const std::vector<SNode>& _graph) const;
};
