//! @file interface_graph_searcher.h
//! @brief グラフ探索の処理のインターフェース．

#ifndef DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_
#define DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_

#include <memory>
#include <tuple>
#include <vector>

#include "devide_map_state.h"
#include "gait_pattern_graph_tree.h"
#include "graph_search_result_record.h"
#include "robot_operation.h"


namespace designlab
{

//! @class IGraphSearcher
//! @brief グラフ探索の処理のインターフェース．
class IGraphSearcher
{
public:

	IGraphSearcher() = default;
	virtual ~IGraphSearcher() = default;		//!< 継承をするクラスのデストラクタはvirtualにしておく．参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html

	//! @brief グラフを受け取り，その中から最適な次の動作を出力する．
	//! @param [in] graph グラフ木．
	//! @param [in] operation 目標姿勢，方向．
	//! @param [in] max_depth 探索する最大深さ．
	//! @param [in] devide_map_state 分割されたマップの状態．
	//! @return std::tuple<GraphSearchResult, int, int> グラフ探索の結果，選択されたノード(深さ1)のindex，最高評価ノード(深さmax_depth)のindexのタプル．
	virtual std::tuple<GraphSearchResult, int, int> SearchGraphTree(
		const GaitPatternGraphTree& graph,
		const RobotOperation& operation,
		const DevideMapState& devide_map_state,
		int max_depth
	) const = 0;
};

}	// namespace designlab


#endif	// DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_