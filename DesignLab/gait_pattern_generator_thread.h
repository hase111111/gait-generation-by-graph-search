//! @file gait_pattern_generator_thread.h
//! @brief グラフ探索を行うスレッドを管理するクラス．

#ifndef DESIGNLAB_GAIT_PATTERN_GENERATOR_THREAD_H_
#define DESIGNLAB_GAIT_PATTERN_GENERATOR_THREAD_H_

#include "interface_gait_pattern_generator.h"

#include <memory>
#include <vector>

#include "interface_graph_searcher.h"
#include "interpolated_node_creator.h"
#include "graph_tree_creator.h"
#include "robot_state_node.h"


//! @class GaitPatternGeneratorThread
//! @brief 普通にグラフ探索を行い，歩容パターン生成を行うクラス．
class GaitPatternGeneratorThread final : public IGaitPatternGenerator
{
public:

	//! @param[in] graph_tree_creator グラフ探索を行う木構造のグラフを作成するクラス．unique_ptrで渡す．
	//! @param[in] graph_searcher グラフ探索を行うクラス．unique_ptrで渡す．
	GaitPatternGeneratorThread(
		std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
		std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr
	);

	~GaitPatternGeneratorThread() = default;


	GraphSearchResult GetNextNodebyGraphSearch(
		const RobotStateNode& current_node,
		const MapState& map_ref,
		const TargetRobotState& target,
		RobotStateNode* output_node
	) override;

private:

	const std::unique_ptr<GraphTreeCreator> graph_tree_creator_ptr_;	//!< グラフ探索を行う木構造のグラフを作成するクラス

	const std::unique_ptr<IGraphSearcher> graph_searcher_ptr_;			//!< グラフ探索を行うクラス

	//! グラフ探索を行う木構造のグラフ．
	//! push_backあるいはemplace_back関数とreserve関数を組み合わせるのは時間がかかるため，
	//! resize関数を使用して予め確保した領域に，ノードを追加する． 
	std::vector<RobotStateNode> graph_tree_;
	int graph_tree_size_;

	static constexpr int kThreadNum = 6;
	std::array<std::vector<RobotStateNode>, kThreadNum> graph_tree_array_;
	std::array<int, kThreadNum> graph_tree_size_array_;

	void AppendGraphTree(const std::array<std::tuple<GraphSearchResult, RobotStateNode, int>, kThreadNum>& search_result_array);
};


#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_THREAD_H_	