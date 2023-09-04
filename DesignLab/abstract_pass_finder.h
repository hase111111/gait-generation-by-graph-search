#pragma once

#include <vector>

#include "map_state.h"
#include "node.h"
#include "Target.h"
#include "graph_search_result.h"
#include "application_setting_recorder.h"
#include "interface_graph_tree_creator.h"
#include "abstract_graph_searcher.h"
#include "abstract_pass_finder_factory.h"
#include "abstract_hexapod_state_calculator.h"


//! @class AbstractPassFinder
//! @brief グラフ探索を行うクラスの抽象クラス．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @date 2023/09/03
//! @author 長谷川
//! @details 継承をするクラスのデストラクタはvirtualにしておく．
//! @n 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html
class AbstractPassFinder
{
public:

	AbstractPassFinder() = default;
	virtual ~AbstractPassFinder() = default;


	void init(std::unique_ptr<AbstractPassFinderFactory>&& factory, std::shared_ptr<AbstractHexapodStateCalculator> calc, const SApplicationSettingRecorder* const setting);


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

	//! @brief 作成したグラフ木を返す．
	//! @n この関数はデバッグ用なので，探索には使わないこと．
	//! @param [out] output_graph 作成したグラフ木を渡す．
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

	std::shared_ptr<AbstractHexapodStateCalculator> mp_calculator;	//!< ヘキサポッドの状態を計算するクラス

	const SApplicationSettingRecorder* mp_setting;	//!< 設定を記録するクラス
};


//! @file interface_pass_finder.h
//! @date 2023/08/14
//! @author 長谷川
//! @brief グラフ探索を行うクラスのインターフェイスの実装
//! @n 行数 : @lineinfo
