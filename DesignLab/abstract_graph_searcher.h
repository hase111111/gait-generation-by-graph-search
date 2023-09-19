#pragma once

#include <vector>
#include <memory>

#include "node.h"
#include "Target.h"
#include "graph_search_result.h"
#include "abstract_hexapod_state_calculator.h"


//! @class AbstractGraphSearcher
//! @date 2023/08/14
//! @author 長谷川
//! @brief グラフ木を作成する抽象クラス．実体は作成できないのでこれを継承してたクラスを使ってください．
//! @n 継承の仕方や使い方が分からない場合は，GraphSearcherSampleを見てみてください．
class AbstractGraphSearcher
{
public:

	AbstractGraphSearcher(const std::shared_ptr<const AbstractHexapodStateCalculator>& calc) : mp_calculator(calc) {};

	virtual ~AbstractGraphSearcher() = default;		//!< 継承をするクラスのデストラクタはvirtualにしておく．参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html


	//! @brief グラフを受け取り，その中から最適な次の動作を出力する．
	//! @param graph [in] グラフ木
	//! @param target [in] 目標地点
	//! @param output_result [out] 出力されるノード
	//! @return EGraphSearchResult 探索の結果
	virtual EGraphSearchResult searchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result) = 0;

protected:

	const std::shared_ptr<const AbstractHexapodStateCalculator> mp_calculator;	//!< ヘキサポッドの状態を計算するクラス
};


//! @file abstract_graph_searcher.h
//! @date 2023/08/14
//! @author 長谷川
//! @brief グラフ木を作成する抽象クラス．
//! @n 行数 : @lineinfo
