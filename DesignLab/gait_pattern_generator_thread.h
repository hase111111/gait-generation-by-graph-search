
//! @file      gait_pattern_generator_thread.h
//! @author    Hasegawa
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
    GaitPatternGeneratorThread() = delete;  //!< デフォルトコンストラクタは禁止．

    //! @param[in] graph_tree_creator グラフ探索を行う木構造のグラフを作成するクラス．
    //! unique_ptrで渡す．
    //! @param[in] graph_searcher グラフ探索を行うクラス．unique_ptrで渡す．
    //! @param[in] max_depth グラフ探索の最大深さ．
    //! @param[in] max_node_num グラフ探索の最大ノード数．
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
    static constexpr int kThreadNum = 6;  //!< スレッド数．


    std::vector<GaitPatternGraphTree> InitializeGraphTreeArray(int thread_num, int max_node_num) const;

    //! グラフ探索を行う木構造のグラフを作成するクラス．
    const std::unique_ptr<GraphTreeCreator> graph_tree_creator_ptr_;

    //! グラフ探索を行うクラス．
    const std::unique_ptr<IGraphSearcher> graph_searcher_ptr_;

    GaitPatternGraphTree graph_tree_;  //!< このグラフに深さ1までのノードを追加する．

    //! 深さ2以上のノードは並列に探索するため，スレッドごとにグラフを持つ．
    std::vector<GaitPatternGraphTree> graph_tree_array_;

    const int max_depth_;  //!< グラフ探索の最大深さ．


    static_assert(0 < kThreadNum, "スレッド数は正の数である必要があります．");
};

}  // namespace designlab


#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_THREAD_H_
