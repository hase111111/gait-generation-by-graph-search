
//! @file      interface_graph_searcher.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_
#define DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_

#include <memory>
#include <tuple>
#include <vector>

#include "divided_map_state.h"
#include "gait_pattern_graph_tree.h"
#include "graph_search_evaluator.h"
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

    //! 継承をするクラスのデストラクタは virtual にしておく．
    //! 参考 https://www.yunabe.jp/docs/cpp_virtual_destructor.html (アクセス日: 2023/12/27)
    virtual ~IGraphSearcher() = default;

    //! @brief グラフを受け取り，その中から最適な次の動作を出力する．
    //! @param[in] graph グラフ木．
    //! @param[in] operation 目標姿勢，方向．
    //! @param[in] divided_map_state 分割されたマップの状態．
    //! @param[in] max_depth 探索する最大深さ．
    //! @return グラフ探索の結果，評価値，選択されたノードのインデックスのタプル．
    virtual std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> SearchGraphTree(
        const GaitPatternGraphTree& graph,
        const RobotOperation& operation,
        const DividedMapState& divided_map_state,
        int max_depth) const = 0;

    virtual std::tuple<GraphSearchResult, GraphSearchEvaluationValue, RobotStateNode> SearchGraphTreeVector(
        const std::vector<GaitPatternGraphTree>& graph_vector,
        const RobotOperation& operation,
        const DividedMapState& divided_map_state,
        int max_depth) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_GRAPH_SEARCHER_H_
