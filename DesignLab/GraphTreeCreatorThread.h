#pragma once
#include "InterfaceGraphTreeCreator.h"

class GraphTreeCreatorThread final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorThread(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& map) : IGraphTreeCreator(map) {};
	~GraphTreeCreatorThread() = default;

	EGraphSearchResult createGraphTree(const SNode& current_node, const MapState* const p_map, std::vector<SNode>& output_graph, int& make_node_num) override;

private:

	//_out_put_graphの値をリセットしてから，_current_nodeの子ノードを生成して，_output_graphに代入する．
	void makeNewNodesByCurrentNode(const SNode& current_node, const int current_num, std::vector<SNode>& output_graph);

	void makeGraphToMaxDepth(const SNode& current_node, std::vector<SNode>* output_graph);
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