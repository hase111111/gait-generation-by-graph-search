#pragma once
#include "MapState.h"
#include "GraphicSystem.h"
#include "GraphicDataBroker.h"

class GraphViewerSystemMain final
{
public:

	GraphViewerSystemMain();

	void main();

private:
	MapState m_MapState;
	GraphicDataBroker m_GraphicDataBroker;
	GraphicSystem m_GraphicSystem;

	// y / n の質問をする
	bool askYesNo(const std::string& question) const;

	// グラフのステータスを表示する
	void showGraphStatus(const std::vector<SNode>& _graph) const;
};
