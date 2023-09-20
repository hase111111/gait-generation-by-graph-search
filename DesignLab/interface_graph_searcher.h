//! @file interface_graph_searcher.h
//! @brief グラフ木を作成する抽象クラス．

#ifndef DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_
#define DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_


#include <memory>
#include <vector>

#include "abstract_hexapod_state_calculator.h"
#include "graph_search_result.h"
#include "node.h"
#include "target.h"


//! @class IGraphSearcher
//! @brief グラフ木を作成する抽象クラス．実体は作成できないのでこれを継承してたクラスを使ってください．
//! @n 継承の仕方や使い方が分からない場合は，GraphSearcherSampleを見てみてください．
class IGraphSearcher
{
public:

	IGraphSearcher() = default;
	virtual ~IGraphSearcher() = default;		//!< 継承をするクラスのデストラクタはvirtualにしておく．参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html


	//! @brief グラフを受け取り，その中から最適な次の動作を出力する．
	//! @param graph [in] グラフ木
	//! @param target [in] 目標地点
	//! @param output_result [out] 出力されるノード
	//! @return EGraphSearchResult 探索の結果
	virtual EGraphSearchResult SearchGraphTree(const std::vector<SNode>& graph, const STarget& target, SNode* output_result) = 0;

};


#endif	