//! @file interface_graph_searcher.h
//! @brief グラフ探索を行うインターフェース

#ifndef DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_
#define DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_


#include <memory>
#include <tuple>
#include <vector>

#include "graph_search_result_recoder.h"
#include "robot_state_node.h"
#include "target.h"


//! @class IGraphSearcher
//! @brief グラフ探索を行うインターフェース．実体は作成できないのでこれを継承してたクラスを使うこと．
class IGraphSearcher
{
public:

	IGraphSearcher() = default;
	virtual ~IGraphSearcher() = default;		//!< 継承をするクラスのデストラクタはvirtualにしておく．参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html


	//! @brief グラフを受け取り，その中から最適な次の動作を出力する．
	//! @param [in] graph グラフ木．
	//! @param [in] graph_size グラフ木のサイズ．
	//! @param [in] target 目標地点．
	//! @return std::tuple<GraphSearchResult, RobotStateNode, int> グラフ探索の結果，次のノード，最高評価ノードのindexのタプル．
	virtual std::tuple<GraphSearchResult, RobotStateNode, int> SearchGraphTree(
		const std::vector<RobotStateNode>& graph,
		int graph_size,
		const TargetRobotState& target
	) const = 0;

};


#endif	// DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_