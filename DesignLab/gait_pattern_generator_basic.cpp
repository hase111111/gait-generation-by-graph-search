
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "gait_pattern_generator_basic.h"

#include <string>
#include <utility>

#include "cassert_define.h"
#include "cmdio_util.h"
#include "graph_search_const.h"
#include "map_state.h"


namespace designlab
{

GaitPatternGeneratorBasic::GaitPatternGeneratorBasic(
    std::unique_ptr<GraphTreeCreator>&& graph_tree_creator,
    std::unique_ptr<IGraphSearcher>&& graph_searcher,
    const int max_depth,
    const int max_node_num) :
    graph_tree_creator_ptr_(std::move(graph_tree_creator)),
    graph_searcher_ptr_(std::move(graph_searcher)),
    graph_tree_{ max_node_num },  // ここでメモリを確保する．
    max_depth_(max_depth)
{
    assert(graph_tree_creator_ptr_ != nullptr);
    assert(graph_searcher_ptr_ != nullptr);
    assert(0 < max_depth_);
    assert(0 < max_node_num);
}

GraphSearchResult GaitPatternGeneratorBasic::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node,
    const MapState& map_state,
    const RobotOperation& operation,
    RobotStateNode* output_node)
{
    assert(current_node.IsLootNode());
    assert(output_node != nullptr);
    assert(graph_tree_creator_ptr_ != nullptr);
    assert(graph_searcher_ptr_ != nullptr);

    // 初期化処理を行う．
    DividedMapState divided_map;
    divided_map.Init(map_state, current_node.center_of_mass_global_coord);

    graph_tree_creator_ptr_->Init(divided_map);


    // グラフ探索をするための，歩容パターングラフを生成する
    graph_tree_.Reset();
    graph_tree_.AddNode(current_node);

    const GraphSearchResult create_result = graph_tree_creator_ptr_->CreateGraphTree(0, max_depth_, &graph_tree_);

    if (create_result.result != enums::Result::kSuccess)
    {
        return create_result;
    }


    // グラフ探索を行う
    const auto [search_result, _, next_node] = graph_searcher_ptr_->SearchGraphTree(graph_tree_, operation, divided_map, max_depth_);

    if (search_result.result != enums::Result::kSuccess)
    {
        return search_result;
    }

    (*output_node) = next_node;

    return { enums::Result::kSuccess, "" };
}

}  // namespace designlab
