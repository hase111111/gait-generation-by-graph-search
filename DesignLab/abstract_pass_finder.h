//! @file abstract_pass_finder.h
//! @brief パス探索を行うクラスの抽象クラス．

#ifndef DESIGNLAB_ABSTRACT_PASS_FINDER_H_
#define DESIGNLAB_ABSTRACT_PASS_FINDER_H_

#include <vector>

#include "abstract_graph_searcher.h"
#include "abstract_hexapod_state_calculator.h"
#include "application_setting_recorder.h"
#include "graph_search_result.h"
#include "interface_graph_tree_creator.h"
#include "interface_pass_finder_factory.h"
#include "map_state.h"
#include "node.h"
#include "target.h"


//! @class AbstractPassFinder
//! @brief グラフ探索を行うクラスの抽象クラス．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @details 継承をするクラスのデストラクタはvirtualにしておく．
//! @n 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html

class AbstractPassFinder
{
public:

	AbstractPassFinder() = default;
	virtual ~AbstractPassFinder() = default;


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

	int m_made_node_num = 0;			//!< 作成したグラフの数

	std::vector<SNode> m_graph_tree;	//!< グラフ木


	//! @brief グラフ木の生成に必要なクラスを生成する．
	//! @param [in] map マップ情報．
	//! @param [in] calculator_ptr_ ヘキサポッドの状態を計算するクラス．
	//! @return std::unique_ptr<IGraphTreeCreator> グラフ木の生成に必要なクラス．
	virtual std::unique_ptr<IGraphTreeCreator> createGraphTreeCreator(const MapState* const map, const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_) = 0;

	//! @brief グラフ探索を行うクラスを生成する．
	//! @param [in] calculator_ptr_ ヘキサポッドの状態を計算するクラス．
	//! @return std::unique_ptr<AbstractGraphSearcher> グラフ探索を行うクラス．
	virtual std::unique_ptr<AbstractGraphSearcher> createGraphSearcher(const std::shared_ptr<const AbstractHexapodStateCalculator>& calculator_ptr_) = 0;


	//std::unique_ptr<IPassFinderFactory> mp_factory;	//!< パス探索クラスのファクトリー

	//std::shared_ptr<AbstractHexapodStateCalculator> mp_calculator;	//!< ヘキサポッドの状態を計算するクラス

	//const SApplicationSettingRecorder* mp_setting;	//!< 設定を記録するクラス
};


#endif	// DESIGNLAB_ABSTRACT_PASS_FINDER_H_