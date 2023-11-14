//! @file pass_finder_revaluation.h
//! @brief 再評価手法を実装したクラス


#ifndef PASS_FINDER_REVALUATION_H_
#define PASS_FINDER_REVALUATION_H_


#include <memory>
#include <vector>

#include "graph_tree_creator.h"
#include "interface_pass_finder.h"
#include "interface_graph_searcher.h"
#include "robot_state_node.h"


class PassFinderRevaluation final : public IPassFinder
{
public:

	PassFinderRevaluation(
		std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_ptr,
		std::unique_ptr<GraphTreeCreator>&& graph_tree_creator_revaluation_ptr,
		std::unique_ptr<IGraphSearcher>&& graph_searcher_ptr
	);

	~PassFinderRevaluation() = default;


	GraphSearchResult GetNextNodebyGraphSearch(
		const RobotStateNode& current_node,
		const MapState& map, 
		const TargetRobotState& target,
		RobotStateNode* output_node
	) override;

private:

	const std::unique_ptr<GraphTreeCreator> graph_tree_creator_ptr_;	//!< グラフ探索を行う木構造のグラフを作成するクラス

	const std::unique_ptr<GraphTreeCreator> graph_tree_creator_revaluation_ptr_;	//!< 再評価用の木構造のグラフを作成するクラス

	const std::unique_ptr<IGraphSearcher> graph_searcher_ptr_;			//!< グラフ探索を行うクラス

	bool IsVaildNode(const RobotStateNode& current_node, const RobotStateNode& next_node) const;
};


#endif	//	PASS_FINDER_REVALUATION_H_