
//! @file      gait_pattern_generator_thread.h
//! @author    hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GAIT_PATTERN_GENERATOR_THREAD_H_
#define DESIGNLAB_GAIT_PATTERN_GENERATOR_THREAD_H_

#include <memory>
#include <tuple>
#include <vector>

#include "interface_gait_pattern_generator.h"
#include "interface_graph_searcher.h"
#include "interpolated_node_creator.h"
#include "gait_pattern_graph_tree.h"
#include "graph_tree_creator.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class GaitPatternGeneratorThread
//! @brief 普通にグラフ探索を行い，歩容パターン生成を行うクラス．
class GaitPatternGeneratorThread final : public IGaitPatternGenerator
{
public:
    //! @param[in] graph_tree_creator グラフ探索を行う木構造のグラフを作成するクラス．
    //! unique_ptrで渡す．
    //! @param[in] graph_searcher グラフ探索を行うクラス．unique_ptrで渡す．
    GaitPatternGeneratorThread(
      std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
      std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr,
      int max_depth,
      int max_node_num);

    ~GaitPatternGeneratorThread() = default;


    GraphSearchResult GetNextNodeByGraphSearch(
      const RobotStateNode& current_node,
      const MapState& map_ref,
      const RobotOperation& operation,
      RobotStateNode* output_node) override;

private:
    static constexpr int kThreadNum = 6;

    void AppendGraphTree(
        const std::array<std::tuple<GraphSearchResult, int, int>, kThreadNum>& search_result_array);

    //! グラフ探索を行う木構造のグラフを作成するクラス．
    const std::unique_ptr<GraphTreeCreator> graph_tree_creator_ptr_;

    //! グラフ探索を行うクラス．
    const std::unique_ptr<IGraphSearcher> graph_searcher_ptr_;

    GaitPatternGraphTree graph_tree_;
    std::array<GaitPatternGraphTree, kThreadNum> graph_tree_array_;

    const int max_depth_;  //!< グラフ探索の最大深さ．


    static_assert(0 < kThreadNum, "スレッド数は正の数である必要があります．");
};

}  // namespace designlab


#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_THREAD_H_
