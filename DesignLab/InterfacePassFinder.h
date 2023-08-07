#pragma once

#include <vector>

#include "map_state.h"
#include "Node.h"
#include "Target.h"
#include "GraphSearchResult.h"
#include "InterfaceGraphTreeCreator.h"
#include "InterfaceGraphSearcher.h"
#include "AbstractPassFinderFactory.h"

class IPassFinder
{
public:

	IPassFinder(std::unique_ptr<AbstractPassFinderFactory>&& factory) : mp_factory(std::move(factory)) {};
	virtual ~IPassFinder() = default;

	//! @brief グラフ探索を行い，次の動作として最適なノードを返す．
	//! @param [in] current_node 現在の状態を表すノード
	//! @param [in] p_map 現在のマップの状態
	//!	@param [in] target 目標の状態
	//! @param [out] output_node 結果のノード
	//! @return EGraphSearchResult グラフ探索の結果を返す．
	virtual EGraphSearchResult getNextNodebyGraphSearch(const SNode& current_node, const MapState* const p_map, const STarget& target, SNode& output_node) = 0;

	//! @brief 作成したグラフの数を返す
	//! @return int 作成したグラフの数
	int getMadeNodeNum() const { return m_made_node_num; }

	//! @brief 作成したグラフ木を返す．<br>
	//! この関数はデバッグ用なので，探索には使わないこと．
	//! @param [out] output_graph 作成したグラフ木
	void getGraphTree(std::vector<SNode>* output_graph) const
	{
		(*output_graph).clear();
		for (auto& i : m_graph_tree)
		{
			(*output_graph).emplace_back(i);
		}
	}

protected:

	int m_made_node_num = 0;	//!< 作成したグラフの数

	std::vector<SNode> m_graph_tree;	//!< グラフ木

	std::unique_ptr<AbstractPassFinderFactory> mp_factory;	//!< パス探索クラスのファクトリー
	std::unique_ptr<IGraphTreeCreator> mp_tree_creator;	//!< グラフ木の作成クラス
	std::unique_ptr<IGraphSearcher> mp_searcher;	//!< グラフ探索クラス
};

//! @file IPassFinder.h
//! @brief グラフ探索を行うクラスのインターフェイスの実装
//! @author 長谷川
//! @date 2023/07/24

//! @class IPassFinder
//! @brief グラフ探索を行うクラスのインターフェイス．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @date 2023/07/24
//! @auther 長谷川
//! @details 継承をするクラスのデストラクタはvirtualにしておく．<br>
//! 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html