
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "gait_pattern_generator_thread.h"

#include <algorithm>
#include <format>
#include <string>
#include <utility>

#include <boost/thread.hpp>

#include "array_util.h"
#include "cassert_define.h"
#include "cmdio_util.h"
#include "graph_search_const.h"
#include "map_state.h"
#include "string_util.h"


namespace designlab
{

GaitPatternGeneratorThread::GaitPatternGeneratorThread(
    std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
    std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr,
    const int max_depth,
    const int max_node_num) :
    graph_tree_creator_ptr_(std::move(graph_tree_creator_ptr)),
    graph_searcher_ptr_(std::move(graph_searcher_ptr)),
    graph_tree_{ 1000 },
    graph_tree_array_(InitializeGraphTreeArray(kThreadNum, max_node_num)),
    max_depth_(max_depth)
{
    assert(graph_tree_creator_ptr_ != nullptr);
    assert(graph_searcher_ptr_ != nullptr);
    assert(max_depth_ > 0);
    assert(max_node_num > 0);
    assert(graph_tree_array_.size() == kThreadNum);
}

GraphSearchResult GaitPatternGeneratorThread::GetNextNodeByGraphSearch(
    const RobotStateNode& current_node,
    const MapState& map_state,
    const RobotOperation& operation,
    RobotStateNode* output_node)
{
    assert(current_node.IsLootNode());
    assert(output_node != nullptr);

    // 初期化処理を行う．
    DividedMapState divided_map;
    divided_map.Init(map_state, current_node.center_of_mass_global_coord);

    graph_tree_creator_ptr_->Init(divided_map);

    // グラフ探索をするための，歩容パターングラフを生成する．
    graph_tree_.Reset();
    graph_tree_.AddNode(current_node);

    const GraphSearchResult create_result =
        graph_tree_creator_ptr_->CreateGraphTree(0, 1, &graph_tree_);

    if (create_result.result != enums::Result::kSuccess) { return create_result; }

    CmdIOUtil::Output("深さ1までグラフ木の生成が終了しました．",
                      enums::OutputDetail::kDebug);
    CmdIOUtil::Output(std::format("グラフ木のノード数は{}です．", graph_tree_.GetGraphSize()),
                      enums::OutputDetail::kDebug);

    // 深さ0のノードを配列にコピーする．
    for (int i = 0; i < kThreadNum; i++)
    {
        graph_tree_array_[i].Reset();
        graph_tree_array_[i].AddNode(current_node);
    }

    // 深さ1のノードを配列に分けてコピーする．
    for (int i = 1; i < graph_tree_.GetGraphSize(); i++)
    {
        if (graph_tree_.GetNode(i).depth == 1)
        {
            // i を kThreadNum で割った余り番目の配列にコピーする．
            graph_tree_array_[i % kThreadNum].AddNode(graph_tree_.GetNode(i));
        }
    }

    // スレッドを分けて，最大深さまで探索する．
    boost::thread_group thread_group;

    for (size_t i = 0; i < kThreadNum; i++)
    {
        if (graph_tree_array_[i].GetGraphSize() > 1)
        {
            CmdIOUtil::Output("スレッド" + std::to_string(i) + "でグラフ木の生成を開始します．",
                              enums::OutputDetail::kDebug);
            CmdIOUtil::Output("スレッド" + std::to_string(i) + "で探索するノード数は" +
                              std::to_string(graph_tree_array_[i].GetGraphSize()) + "です．",
                              enums::OutputDetail::kDebug);

            thread_group.create_thread(
              boost::bind(
                &GraphTreeCreator::CreateGraphTree,
                graph_tree_creator_ptr_.get(),
                1,
                max_depth_,
                &graph_tree_array_[i]));
        }
    }

    thread_group.join_all();  // 全てのスレッドが終了するまで待機する．

    CmdIOUtil::Output("グラフ木の生成が終了しました．\n", enums::OutputDetail::kDebug);

    for (size_t i = 0; i < kThreadNum; i++)
    {
        CmdIOUtil::Output(std::format("スレッド{}で作成したノード数は{}です．", i, graph_tree_array_[i].GetGraphSize()),
                          enums::OutputDetail::kDebug);
    }


    // グラフ探索を行う．
    CmdIOUtil::Output("グラフ木を評価します．", enums::OutputDetail::kDebug);

    const auto [search_result, _, next_node] =
        graph_searcher_ptr_->SearchGraphTreeVector(graph_tree_array_, operation, divided_map, max_depth_);

    if (search_result.result != enums::Result::kSuccess)
    {
        CmdIOUtil::Output("グラフ木の評価に失敗しました．", enums::OutputDetail::kDebug);
        return search_result;
    }

    (*output_node) = next_node;

    CmdIOUtil::Output("グラフ木の評価が終了しました．グラフ探索に成功しました．",
                      enums::OutputDetail::kDebug);

    return { enums::Result::kSuccess, std::string("") };
}

std::vector<GaitPatternGraphTree> GaitPatternGeneratorThread::InitializeGraphTreeArray(const int thread_num, const int max_node_num) const
{
    std::vector<GaitPatternGraphTree> graph_tree_array;

    for (int i = 0; i < thread_num; i++)
    {
        graph_tree_array.emplace_back(max_node_num / thread_num);
    }

    return graph_tree_array;
}

}  // namespace designlab
