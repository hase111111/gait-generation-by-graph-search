
//! @file      interface_graph_searcher.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_
#define DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_

#include <memory>
#include <tuple>
#include <vector>

#include "divided_map_state.h"
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

    //! 継承をするクラスのデストラクタは virtualにしておく．
    //! 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html (アクセス日: 2023/12/27)
    virtual ~IGraphSearcher() = default;

    //! @brief グラフを受け取り，その中から最適な次の動作を出力する．
    //! @param [in] graph グラフ木．
    //! @param [in] operation 目標姿勢，方向．
    //! @param [in] max_depth 探索する最大深さ．
    //! @param [in] divided_map_state 分割されたマップの状態．
    //! @return グラフ探索の結果，選択されたノード(深さ1)の index，
    //! 最高評価ノード(深さ max_depth) の indexのタプル．
    virtual std::tuple<GraphSearchResult, int, int> SearchGraphTree(
      const GaitPatternGraphTree& graph,
      const RobotOperation& operation,
      const DividedMapState& divided_map_state,
      int max_depth) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_
