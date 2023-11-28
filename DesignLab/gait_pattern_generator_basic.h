//! @file gait_pattern_generator_basic.h
//! @brief 普通にグラフ探索を行い，歩容パターン生成を行うクラス

#ifndef DESIGNLAB_GAIT_PATTERN_GENERATOR_BASIC_H_
#define DESIGNLAB_GAIT_PATTERN_GENERATOR_BASIC_H_

#include "interface_gait_pattern_generator.h"

#include <memory>
#include <vector>

#include "interface_graph_searcher.h"
#include "interpolated_node_creator.h"
#include "gait_pattern_graph_tree.h"
#include "graph_tree_creator.h"
#include "robot_state_node.h"


//! @class GaitPatternGeneratorBasic
//! @brief 普通にグラフ探索を行い，歩容パターン生成を行うクラス
class GaitPatternGeneratorBasic final : public IGaitPatternGenerator
{
public:

	//! @param[in] graph_tree_creator グラフ探索を行う木構造のグラフを作成するクラス．unique_ptrで渡す．
	//! @param[in] graph_searcher グラフ探索を行うクラス．unique_ptrで渡す．
	GaitPatternGeneratorBasic(
		std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr, 
		std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr
	);

	~GaitPatternGeneratorBasic() = default;


	GraphSearchResult GetNextNodebyGraphSearch(
		const RobotStateNode& current_node,
		const MapState& map_ref,
		const TargetRobotState& target,
		RobotStateNode* output_node
	) override;

private:

	const std::unique_ptr<GraphTreeCreator> graph_tree_creator_ptr_;	//!< グラフ探索を行う木構造のグラフを作成するクラス

	const std::unique_ptr<IGraphSearcher> graph_searcher_ptr_;			//!< グラフ探索を行うクラス

	GaitPatternGraphTree graph_tree_;	//! グラフ探索を行う木構造のグラフ．
};


#endif  // DESIGNLAB_GAIT_PATTERN_GENERATOR_BASIC_H_