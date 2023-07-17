#pragma once
#include <vector>
#include "Node.h"

class GraphViewerGUIController final
{
public:
	GraphViewerGUIController(const std::vector<SNode>* const _p_graph, size_t* const _p_display_node_index);
	~GraphViewerGUIController() = default;

	void update();

	void draw() const;

	void updateGraphNodeDepthData();

private:
	const std::vector<SNode>* const mp_graph;
	size_t* const mp_display_node_index;
	std::pair<int, std::vector<int>> m_childen_list = std::make_pair<int, std::vector<int>>(-1, {});	//子ノードのリスト
	int m_display_children_list_index = 0;	//表示する子ノードのリストのインデックス

	std::vector<int> m_graph_node_depth_data;	//各深さごとのノード数のデータ
	int m_input_number = -1;	//入力された数値

	void drawGraphData() const;
	void drawNodeControllPanel() const;

	void inputNumber();
	void changeDisplayNodeIndex();
	void updateChildrenList();
};
