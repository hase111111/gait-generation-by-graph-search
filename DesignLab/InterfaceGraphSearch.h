#pragma once
#include "MapState.h"
#include "Node.h"
#include "Target.h"
#include "GraphSearchResult.h"
#include "InterfaceGraphTreeCreator.h"
#include "InterfaceGraphSearcher.h"
#include "AbstractPassFinderFactory.h"

class IGraphSearch
{
public:

	IGraphSearch(std::unique_ptr<AbstractPassFinderFactory>&& _factory) : mp_PassFinderFactory(std::move(_factory)) {};
	virtual ~IGraphSearch() = default;

	//! @brief グラフ探索を行い，次の動作として最適なノードを返す．
	//! @param [in] _current_node 現在の状態を表すノード
	//! @param [in] _p_map 現在のマップの状態
	//!	@param [in] _target 目標の状態
	//! @param [out] _output_node 結果のノード
	//! @return EGraphSearchResult グラフ探索の結果を返す．
	virtual EGraphSearchResult getNextNodebyGraphSearch(const SNode& _current_node, const MapState* const _p_map, const STarget& _target, SNode& _output_node) = 0;

	//! @brief 作成したグラフの数を返す
	//! @return int 作成したグラフの数
	int getMadeNodeNum() const { return m_made_node_num; }

protected:

	int m_made_node_num = 0;	//!< 作成したグラフの数

	std::unique_ptr<AbstractPassFinderFactory> mp_PassFinderFactory;	//!< パス探索クラスのファクトリー
	std::unique_ptr<IGraphTreeCreator> mp_GraphTreeCreator;	//!< グラフ木の作成クラス
	std::unique_ptr<IGraphSearcher> mp_GraphSearcher;	//!< グラフ探索クラス
};

//! @file IGraphSearch
//! @brief グラフ探索を行うクラスのインターフェイスの実装
//! @author 長谷川
//! @date 2023/07/24

//! @class IGraphSearch
//! @brief グラフ探索を行うクラスのインターフェイス．実体は作成できないのでこれを継承してたクラスを使うこと．
//! @date 2023/07/24
//! @auther 長谷川
//! @details 継承をするクラスのデストラクタはvirtualにしておく．<br>
//! 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html