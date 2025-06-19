
//! @file gait_pattern_generator_thread.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "gait_pattern_generator_thread.h"

#include <boost/thread.hpp>
#include <format>
#include <string>
#include <utility>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "map_state.h"
#include "string_util.h"

namespace designlab {

GaitPatternGeneratorThread::GaitPatternGeneratorThread(
    std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
    std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr, const int max_depth,
    const int max_node_num)
    : graph_tree_creator_ptr_(std::move(graph_tree_creator_ptr)),
      graph_searcher_ptr_(std::move(graph_searcher_ptr)),
      graph_tree_{1000},
      graph_tree_array_(InitializeGraphTreeArray(kThreadNum, max_node_num)),
      max_depth_(max_depth) {
  assert(graph_tree_creator_ptr_ != nullptr);
  assert(graph_searcher_ptr_ != nullptr);
  assert(max_depth_ > 0);
  assert(max_node_num > 0);
  assert(graph_tree_array_.size() == kThreadNum);
}

nostd::expected<RobotStateNode, std::string>
GaitPatternGeneratorThread::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node, const MapState& map_state,
    const RobotOperation& operation) {
  assert(current_node.IsLootNode());

  // 初期化処理を行う.
  DividedMapState divided_map;
  divided_map.Init(map_state, current_node.center_of_mass_global_coord);

  graph_tree_creator_ptr_->Init(divided_map);

  // グラフ探索をするための,歩容パターングラフを生成する.
  graph_tree_.Reset();
  graph_tree_.AddNode(current_node);

  const auto create_result =
      graph_tree_creator_ptr_->CreateGraphTree(0, 1, &graph_tree_);

  if (!create_result) {
    return nostd::unexpected<std::string>{create_result.error()};
  }

  cmdio::DebugOutput("Graph tree generation has been completed to depth 1.");
  cmdio::DebugOutputF("The number of nodes in the graph tree is {}.",
                      graph_tree_.GetGraphSize());

  // 深さ0のノードを配列にコピーする.
  for (int i = 0; i < kThreadNum; i++) {
    graph_tree_array_[i].Reset();
    graph_tree_array_[i].AddNode(current_node);
  }

  // 深さ1のノードを配列に分けてコピーする.
  for (int i = 1; i < graph_tree_.GetGraphSize(); i++) {
    if (graph_tree_.GetNode(i).depth == 1) {
      // i を kThreadNum で割った余り番目の配列にコピーする.
      graph_tree_array_[i % kThreadNum].AddNode(graph_tree_.GetNode(i));
    }
  }

  // スレッドを分けて,最大深さまで探索する.
  boost::thread_group thread_group;

  for (size_t i = 0; i < kThreadNum; i++) {
    if (graph_tree_array_[i].GetGraphSize() > 1) {
      cmdio::DebugOutputF("Starts graph tree generation in thread {}.", i);
      cmdio::DebugOutputF("The number of nodes explored in thread {} is {}.", i,
                          graph_tree_array_[i].GetGraphSize());

      thread_group.create_thread(boost::bind(
          &GraphTreeCreator::CreateGraphTree, graph_tree_creator_ptr_.get(), 1,
          max_depth_, &graph_tree_array_[i]));
    }
  }

  thread_group.join_all();  // 全てのスレッドが終了するまで待機する.

  cmdio::DebugOutput("Graph tree generation is complete.\n");

  for (size_t i = 0; i < kThreadNum; i++) {
    cmdio::DebugOutputF("The number of nodes created in thread {} is {}.", i,
                        graph_tree_array_[i].GetGraphSize());
  }

  // グラフ探索を行う.
  cmdio::DebugOutput("Evaluates graph trees.");

  const auto search_result = graph_searcher_ptr_->SearchGraphTreeVector(
      graph_tree_array_, operation, divided_map, max_depth_);

  if (!search_result) {
    cmdio::DebugOutput("Failed to evaluate the graph tree.");
    return nostd::unexpected<std::string>{search_result.error()};
  }

  cmdio::DebugOutput(
      "Graph tree evaluation is completed. Graph search succeeded.");

  const auto& [next_evaluation_value, next_node] = *search_result;

  cmdio::DebugOutput("Graph search has been completed.");
  cmdio::DebugOutputF("The number of nodes in the graph tree is {}.",
                      graph_tree_.GetGraphSize());
  cmdio::DebugOutputF("The next node is at depth {}.", next_node.depth);
  cmdio::DebugOutputF("<evaluation value> \n {}",
                      next_evaluation_value.ToString());

  return next_node;
}

std::vector<GaitPatternGraphTree>
GaitPatternGeneratorThread::InitializeGraphTreeArray(
    const int thread_num, const int max_node_num) const {
  std::vector<GaitPatternGraphTree> graph_tree_array;

  for (int i = 0; i < thread_num; i++) {
    graph_tree_array.emplace_back(max_node_num / thread_num);
  }

  return graph_tree_array;
}

}  // namespace designlab
