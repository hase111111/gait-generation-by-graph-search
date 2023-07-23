#pragma once
#include "InterfaceGraphTreeCreator.h"
#include "MapState.h"
#include "ComMoveNodeCreator.h"
#include "ComUpDownNodeCreator.h"
#include "LegHierarchyNodeCreator.h"
#include "LegUpDownNodeCreator.h"
#include "InterfaceNodeCreator.h"
#include <memory>

class GraphTreeCreatorHato final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorHato() = default;
	~GraphTreeCreatorHato() = default;

	EGraphSearchResult createGraphTree(const SNode& _current_node, const MapState* const _p_map, std::vector<SNode>& _output_graph, int& _make_node_num) override;

private:

	//_out_put_graphの値をリセットしてから，_current_nodeの子ノードを生成して，_output_graphに代入する．
	void makeNewNodesByCurrentNode(const SNode& _current_node, const int _current_num, std::vector<SNode>& _output_graph);

	const MapState* mp_Map;

	LegHierarchyNodeCreator m_LegHierarchy;
	LegUpDownNodeCreator m_LegUpDown;
	std::unique_ptr<INodeCreator> m_LegUp;
	std::unique_ptr<INodeCreator> m_LegDown;
	ComUpDownNodeCreator m_ComUpDown;
	ComMoveNodeCreator m_ComMove;
};


//! @file GraphTreeCreatorHato.h 
//! @brief 波東さんのグラフを作成するプログラムを移植したクラスの実装
//! @date 2023/7/23
//! @auther 長谷川

//! @class GraphTreeCreatorHato
//! @brief 波東さんのグラフを作成するプログラムを移植したもの
//! @details もともとのプログラムで行われた処理の中でコメントアウトされていたものはすべて削除したので，知りたければ過去のプログラムを参照すること．
//! @date 2023/7/23
//! @auther 長谷川