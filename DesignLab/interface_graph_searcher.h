
//! @file interface_graph_searcher.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "divided_map_state.h"
#include "gait_pattern_graph_tree.h"
#include "graph_search_evaluator.h"
#include "graph_search_result_record.h"
#include "my_expected.h"
#include "robot_operation.h"

namespace gaitgen {

//! @class IGraphSearcher
//! @brief グラフ探索の処理のインターフェース.
class IGraphSearcher {
 public:
  using return_type = std::tuple<GraphSearchEvaluationValue, RobotStateNode>;
  IGraphSearcher() = default;

  virtual ~IGraphSearcher() = default;

  //! @brief グラフを受け取り,その中から最適な次の動作を出力する.
  //! @param[in] graph グラフ木.
  //! @param[in] operation 目標姿勢,方向.
  //! @param[in] divided_map_state 分割されたマップの状態.
  //! @param[in] max_depth 探索する最大深さ.
  //! @return グラフ探索の結果,評価値,選択されたノードのインデックスのタプル.
  virtual nostd::expected<
      std::tuple<GraphSearchEvaluationValue, RobotStateNode>, std::string>
  SearchGraphTree(const GaitPatternGraphTree& graph,
                  const RobotOperation& operation,
                  const DividedMapState& divided_map_state,
                  int max_depth) const = 0;

  virtual nostd::expected<
      std::tuple<GraphSearchEvaluationValue, RobotStateNode>, std::string>
  SearchGraphTreeVector(const std::vector<GaitPatternGraphTree>& graph_vector,
                        const RobotOperation& operation,
                        const DividedMapState& divided_map_state,
                        int max_depth) const = 0;
};

}  // namespace gaitgen
