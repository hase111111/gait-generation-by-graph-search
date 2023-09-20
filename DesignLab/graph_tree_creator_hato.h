//! @file graph_tree_creator_hato.h 
//! @brief 波東さんのグラフを作成するプログラムを移植したクラスの実装

#ifndef DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_
#define DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_

#include "interface_graph_tree_creator.h"

#include <map>
#include <memory>

#include "interface_node_creator.h"


//! @class GraphTreeCreatorHato
//! @brief 波東さんのグラフを作成するプログラムを移植したもの
//! @details もともとのプログラムで行われた処理の中でコメントアウトされていたものはすべて削除したので，
//! @n 知りたければ過去のプログラムを参照すること．
class GraphTreeCreatorHato final : public IGraphTreeCreator
{
public:
	GraphTreeCreatorHato(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& map);
	~GraphTreeCreatorHato() = default;

	EGraphSearchResult CreateGraphTree(const SNode& current_node, int max_depth, std::vector<SNode>* output_graph) override;

private:

	// out_put_graphの値をリセットしてから，_current_nodeの子ノードを生成して，output_graphに代入する．
	void makeNewNodesByCurrentNode(const SNode& current_node, const int current_num, std::vector<SNode>* output_graph);


	std::map<EHexapodMove, std::unique_ptr<INodeCreator>> m_node_creator_map;	//!< ノード生成クラスのマップ．
};


#endif	//DESIGNLAB_GRAPH_TREE_CREATOR_HATO_H_