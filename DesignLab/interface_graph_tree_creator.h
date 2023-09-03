#pragma once

#include <vector>

#include "map_state.h"
#include "node.h"
#include "graph_search_result.h"
#include "interface_node_creator.h"
#include "graph_search_const.h"


//! @class IGraphTreeCreator
//! @brief グラフ木を作成するクラスのインターフェース．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @details 継承をするクラスのデストラクタはvirtualにしておく．<br> 
//! 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html
//! @date 2023/07/23
//! @author 長谷川
class IGraphTreeCreator
{
public:

	IGraphTreeCreator(std::map<EHexapodMove, std::unique_ptr<INodeCreator>>& map);
	virtual ~IGraphTreeCreator() = default;

	//! @brief 木グラフを作成するクラス．完成したグラフは参照渡しする．
	//! @param current_node [in] 現在の状態を表すノード
	//! @param p_map [in] 現在のマップの状態
	//! @param output_graph [out] 出力される木グラフ
	//! @return EGraphSearchResult 生成に成功したならばtrue
	virtual EGraphSearchResult createGraphTree(const SNode& current_node, const MapState* const p_map, std::vector<SNode>* output_graph) = 0;

	void setMaxDepth(const int max_depth) { m_max_depth = max_depth; };

protected:

	int getMaxDepth() const { return m_max_depth; };

	std::map<EHexapodMove, std::unique_ptr<INodeCreator>> m_node_creator_map;

	int m_max_depth = GraphSearchConst::MAX_DEPTH;
};


//! @file interface_graph_tree_creator.h
//! @date 2023/09/03
//! @author 長谷川
//! @brief グラフ木の探索を行うクラスのインターフェイス．
//! @n 行数 : @lineinfo
