#pragma once

#include "interface_graph_tree_creator.h"


//! @class GraphTreeCreatorHato
//! @date 2023/7/23
//! @author 長谷川
//! @brief 波東さんのグラフを作成するプログラムを移植したもの
//! @details もともとのプログラムで行われた処理の中でコメントアウトされていたものはすべて削除したので，知りたければ過去のプログラムを参照すること．
class GraphTreeCreatorHato final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorHato(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& map);
	~GraphTreeCreatorHato() = default;

	EGraphSearchResult createGraphTree(const SNode& current_node, const MapState_Old* const p_map, std::vector<SNode>* output_graph) override;

private:

	// out_put_graphの値をリセットしてから，_current_nodeの子ノードを生成して，output_graphに代入する．
	void makeNewNodesByCurrentNode(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph);

};



//! @file graph_tree_creator_hato.h 
//! @date 2023/7/23
//! @author 長谷川
//! @brief 波東さんのグラフを作成するプログラムを移植したクラスの実装
//! @n 行数 : @lineinfo
