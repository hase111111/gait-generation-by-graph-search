#pragma once
#include "IGraphTreeCreator.h"
#include "MapState.h"
#include "ComUpDownNodeCreator.h"
#include "LegHierarchyNodeCreator.h"

//波東さんのグラフを作成するプログラムを移植したものです．
//もともとのプログラムで行われた処理の中でコメントアウトされたものはすべて削除したので，知りたければ過去のプログラムを参照してください．
class GraphTreeCreatorHato final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorHato() = default;
	~GraphTreeCreatorHato() = default;

	bool createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph) override;

private:

	void pushNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	const MapState * mp_Map;

	LegHierarchyNodeCreator m_LegHierarchy;
	ComUpDownNodeCreator m_ComUpDown;
};
