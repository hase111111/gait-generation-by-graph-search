
//! @file gait_pattern_generator_basic.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gait_pattern_generator_basic.h"

#include <string>
#include <utility>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "map_state.h"

namespace gaitgen {

GaitPatternGeneratorBasic::GaitPatternGeneratorBasic(
    std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
    std::unique_ptr<IGraphSearcher>&& graph_searcher, const int max_depth,
    const int max_node_num)
    : graph_tree_creator_ptr_(std::move(graph_tree_creator)),
      graph_searcher_ptr_(std::move(graph_searcher)),
      graph_tree_{max_node_num},  // ここでメモリを確保する.
      max_depth_(max_depth) {
  assert(graph_tree_creator_ptr_ != nullptr);
  assert(graph_searcher_ptr_ != nullptr);
  assert(0 < max_depth_);
  assert(0 < max_node_num);
}

nostd::expected<RobotStateNode, std::string>
GaitPatternGeneratorBasic::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node, const MapState& map_state,
    const RobotOperation& operation) {
  using unexpected = nostd::unexpected<std::string>;
  assert(current_node.IsLootNode());
  assert(graph_tree_creator_ptr_ != nullptr);
  assert(graph_searcher_ptr_ != nullptr);

  // 初期化処理を行う.
  DividedMapState divided_map;
  divided_map.Init(map_state, current_node.center_of_mass_global_coord);

  graph_tree_creator_ptr_->Init(divided_map);

  // グラフ探索をするための,歩容パターングラフを生成する
  graph_tree_.Reset();
  graph_tree_.AddNode(current_node);

  const auto create_result =
      graph_tree_creator_ptr_->CreateGraphTree(0, max_depth_, &graph_tree_);

  if (!create_result) {
    return unexpected{"GraphTreeCreator: " + create_result.error()};
  }

  // グラフ探索を行う
  const auto search_result = graph_searcher_ptr_->SearchGraphTree(
      graph_tree_, operation, divided_map, max_depth_);

  if (!search_result) {
    return unexpected{search_result.error()};
  }

  const auto& [next_evaluation_value, next_node] = *search_result;

  cmdio::DebugOutput("Graph search has been completed.");
  cmdio::DebugOutputF("The number of nodes in the graph tree is {}.",
                      graph_tree_.GetGraphSize());
  cmdio::DebugOutputF("The next node is at depth {}.", next_node.depth);
  cmdio::DebugOutputF("<evaluation value> \n{}",
                      next_evaluation_value.ToString());

  return next_node;
}

std::vector<int> GaitPatternGeneratorBasic::GetExpandedNodeCountPerDepth()
    const {
  std::vector<int> node_count_per_depth(max_depth_ + 1, 0);

  for (int depth = 0; depth <= max_depth_; ++depth) {
    node_count_per_depth[depth] = graph_tree_.GetNodeCountAtDepth(depth);
  }

  return node_count_per_depth;
}

}  // namespace gaitgen
